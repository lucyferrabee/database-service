<?php

namespace App\Service\Global;

use Doctrine\DBAL\Connection;
use Symfony\Component\Console\Style\SymfonyStyle;

class BlobStorageService
{
    private Connection $globalConnection;

    public function __construct(Connection $globalConnection)
    {
        $this->globalConnection = $globalConnection;
    }

    public function getAllBlobIDsAndNumReferences(): array
    {
        return $this->globalConnection->fetchAllAssociative('SELECT BlobStorageID, NumReferences FROM proton_mail_global.BlobStorage');
    }

    public function validateBlob(string $blobID, SymfonyStyle $io, array $tablesWithReferences): void
    {
        // Fetch the reference count from BlobStorage
        $numReferences = $this->globalConnection->fetchOne(
            'SELECT NumReferences FROM proton_mail_global.BlobStorage WHERE BlobStorageID = :blobID',
            ['blobID' => $blobID]
        );

        if ($numReferences === false) {
            $io->warning("Missing blob reference in BlobStorage: $blobID");
            $this->logger->warning("Missing blob reference in BlobStorage: $blobID");
            return;
        }

        // Calculate the actual number of references from the other tables
        $actualReferenceCount = 0;
        foreach ($tablesWithReferences as $table => $details) {
            $actualReferenceCount += $this->getReferenceCountFromTable($table, $blobID, $details['connection'], $details['columns']);
        }

        // Compare the reference count with the value in BlobStorage
        if ($numReferences != $actualReferenceCount) {
            $message = "Inconsistency for Blob ID $blobID: BlobStorage NumReferences = $numReferences, Actual References = $actualReferenceCount";
            $io->warning($message);
            $this->logger->warning($message);
        } else {
            $io->success("Blob ID $blobID is consistent with $actualReferenceCount references.");
        }
    }

    private function getReferenceCountFromTable(string $table, string $blobID, Connection $connection, array $columns): int
    {
        $columnList = implode(' OR ', array_map(fn($col) => "$col = :blobID", $columns));

        return (int) $connection->fetchOne(
            "SELECT COUNT(*) FROM $table WHERE $columnList",
            ['blobID' => $blobID]
        );
    }
}
