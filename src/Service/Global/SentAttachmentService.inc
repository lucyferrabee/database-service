<?php

namespace App\Service\Global;

use Doctrine\DBAL\Connection;

class SentAttachmentService
{
    private Connection $globalConnection;
    private int $batchSize;

    public function __construct(Connection $globalConnection, int $batchSize = 1000)
    {
        $this->globalConnection = $globalConnection;
        $this->batchSize = $batchSize;
    }

    /**
     * Fetch a batch of BlobStorageIDs from the SentAttachment table.
     *
     * @param int $start
     * @return array
     */
    public function getSentAttachments(int $start): array
    {
        // Fetch a batch of BlobStorageIDs from the SentAttachment table
        $rows = $this->globalConnection->fetchAllAssociative(
            'SELECT BlobStorageID FROM proton_mail_global.SentAttachment LIMIT :start, :batchSize',
            ['start' => $start, 'batchSize' => $this->batchSize],
            ['start' => \PDO::PARAM_INT, 'batchSize' => \PDO::PARAM_INT]
        );

        // Extract BlobStorageIDs from rows
        $blobStorageIDs = array_map(function ($row) {
            return $row['BlobStorageID'];
        }, $rows);

        return $blobStorageIDs;
    }
}
