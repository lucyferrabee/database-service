<?php

namespace App\Service;

use Doctrine\DBAL\Connection;
use Symfony\Component\Console\Style\SymfonyStyle;

class BlobReferenceService
{
    private Connection $connection;
    private int $batchSize;

    public function __construct(Connection $connection)
    {
        $this->connection = $connection;
        $this->batchSize = 1000;
    }

    public function processReferences(SymfonyStyle $io)
    {
        // Get the total number of rows in the reference table (e.g., ProtonMailShard.MessageData)
        $totalRows = $this->getTotalRowCount('ProtonMailShard.MessageData');
        $io->text('Total rows to process in MessageData: $totalRows');

        // Process in batches
        for ($start = 0; $start < $totalRows; $start += $this->batchSize) {
            $io->text("Processing batch starting from row $start");

            // Fetch a batch of rows
            $rows = $this->fetchBatch('ProtonMailShard.MessageData', $start, $this->batchSize);

            // Process each row in the batch
            foreach ($rows as $row) {
                $this->checkBlobConsistency($row['Body'], $row['Header']);
            }
        }
    }

    private function getTotalRowCount(string $table): int
    {
        // Get total number of rows in a given table
        return (int) $this->connection->fetchOne("SELECT COUNT(*) FROM $table");
    }

    private function fetchBatch(string $table, int $start, int $batchSize): array
    {
        // Fetch a batch of rows based on the offset and limit
        return $this->connection->fetchAllAssociative(
            "SELECT Body, Header FROM $table LIMIT :start, :batchSize",
            ['start' => $start, 'batchSize' => $batchSize],
            ['start' => \PDO::PARAM_INT, 'batchSize' => \PDO::PARAM_INT]
        );
    }

    private function checkBlobConsistency(?string $bodyBlobID, ?string $headerBlobID)
    {
        if ($bodyBlobID) {
            $this->validateBlob($bodyBlobID);
        }
        if ($headerBlobID) {
            $this->validateBlob($headerBlobID);
        }
    }

    private function validateBlob(string $blobID)
    {
        // Check if the blob exists in the BlobStorage table
        $numReferences = $this->connection->fetchOne(
            'SELECT NumReferences FROM ProtonMailGlobal.BlobStorage WHERE BlobStorageID = :blobID',
            ['blobID' => $blobID]
        );

        if ($numReferences === false) {
            // Log missing blob reference
            // You can also add logging to a file or database table for inconsistencies
            echo "Missing blob reference: $blobID\n";
        } else {
            // Here you can add logic to compare the reference count if needed
            echo "Blob ID $blobID exists with $numReferences references.\n";
        }
    }
}
