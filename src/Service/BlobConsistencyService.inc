<?php

declare(strict_types = 1);

namespace App\Service;

use App\Service\Global\BlobStorageService;
use Doctrine\DBAL\Connection;
use Symfony\Component\Console\Style\SymfonyStyle;
use Psr\Log\LoggerInterface;

class BlobConsistencyService
{
    private Connection $shardConnection;
    private Connection $globalConnection;
    private int $batchSize;
    private BlobStorageService $blobStorageService;
    private LoggerInterface $logger;

    public function __construct(
        Connection $shardConnection,
        Connection $globalConnection,
        BlobStorageService $blobStorageService,
        LoggerInterface $logger
    ) {
        $this->shardConnection = $shardConnection;
        $this->globalConnection = $globalConnection;
        $this->batchSize = 1000;
        $this->blobStorageService = $blobStorageService;
        $this->logger = $logger;
    }

    public function processAllReferences(SymfonyStyle $io): void
    {
        $tablesWithReferences = [
            'proton_mail_shard.MessageData' => ['connection' => $this->shardConnection, 'columns' => ['Body', 'Header']],
            'proton_mail_shard.Attachment' => ['connection' => $this->shardConnection, 'columns' => ['BlobStorageID']],
            'proton_mail_shard.OutsideAttachment' => ['connection' => $this->shardConnection, 'columns' => ['BlobStorageID']],
            'proton_mail_shard.ContactData' => ['connection' => $this->shardConnection, 'columns' => ['BlobStorageID']],
            'proton_mail_global.SentMessage' => ['connection' => $this->globalConnection, 'columns' => ['BlobStorageID']],
            'proton_mail_global.SentAttachment' => ['connection' => $this->globalConnection, 'columns' => ['BlobStorageID']],
        ];

        $blobReferences = [];

        foreach ($tablesWithReferences as $table => $details) {
            $this->processReferencesForTable($table, $details['connection'], $details['columns'], $blobReferences, $io);
        }

        $this->checkBlobStorageConsistency($blobReferences, $io);
    }

    public function processReferencesForTable(string $table, Connection $connection, array $columns, array &$blobReferences, SymfonyStyle $io): void
    {
        $totalRows = $this->getTotalRowCount($table, $connection);
        $io->text("Total rows to process in $table: $totalRows");
        $this->logger->info("Total rows to process in $table: $totalRows");

        for ($start = 0; $start < $totalRows; $start += $this->batchSize) {
            $io->text("Processing batch starting from row $start");
            $this->logger->info("Processing batch starting from row $start in $table");

            $rows = $this->fetchBatch($table, $connection, $columns, $start, $this->batchSize);

            foreach ($rows as $row) {
                foreach ($columns as $column) {
                    if (!empty($row[$column])) {
                        $blobReferences[$row[$column]] = ($blobReferences[$row[$column]] ?? 0) + 1;
                    }
                }
            }
        }
    }

    private function checkBlobStorageConsistency(array $blobReferences, SymfonyStyle $io): void
    {
        $blobsInStorage = $this->blobStorageService->getAllBlobIDsAndNumReferences();

        foreach ($blobsInStorage as $blob) {
            $blobID = $blob['BlobStorageID'];
            $numReferencesInTables = $blobReferences[$blobID] ?? 0;
            $numReferencesInStorage = $blob['NumReferences'];

            if ($numReferencesInTables !== (int)$numReferencesInStorage) {
                $io->warning("Inconsistency found for Blob ID $blobID: $numReferencesInTables references in tables, but $numReferencesInStorage in storage.");
                $this->logger->warning("Inconsistency for Blob ID $blobID: $numReferencesInTables in tables, but $numReferencesInStorage in storage.");
            }

            unset($blobReferences[$blobID]);
        }

        foreach ($blobReferences as $orphanBlobID => $count) {
            $io->error("Orphan blob found: $orphanBlobID referenced $count times but not found in BlobStorage.");
            $this->logger->error("Orphan blob found: $orphanBlobID referenced $count times but not in BlobStorage.");
        }
    }

    private function getTotalRowCount(string $table, Connection $connection): int
    {
        return (int) $connection->fetchOne("SELECT COUNT(*) FROM $table");
    }

    private function fetchBatch(string $table, Connection $connection, array $columns, int $start, int $batchSize): array
    {
        $columnList = implode(', ', $columns);

        return $connection->fetchAllAssociative(
            "SELECT $columnList FROM $table LIMIT :start, :batchSize",
            ['start' => $start, 'batchSize' => $batchSize],
            ['start' => \PDO::PARAM_INT, 'batchSize' => \PDO::PARAM_INT]
        );
    }
}
