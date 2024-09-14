<?php

namespace App\Service\Global;

use Doctrine\DBAL\Connection;

class AttachmentService
{
    private Connection $globalConnection;
    private int $batchSize;

    public function __construct(Connection $globalConnection, int $batchSize = 1000)
    {
        $this->globalConnection = $globalConnection;
        $this->batchSize = $batchSize;
    }

    /**
     * Fetch a batch of BlobStorageIDs from the Attachment table.
     *
     * @param int $start
     * @return array
     * @throws \Doctrine\DBAL\Exception
     */
    public function getAttachments(int $start): array
    {
        // Fetch a batch of BlobStorageIDs from the Attachment table
        $rows = $this->globalConnection->fetchAllAssociative(
            'SELECT BlobStorageID FROM proton_mail_shard.Attachment LIMIT :start, :batchSize',
            ['start' => $start, 'batchSize' => $this->batchSize],
            ['start' => \PDO::PARAM_INT, 'batchSize' => \PDO::PARAM_INT]
        );

        // Extract BlobStorageIDs from rows
        return array_map(function ($row) {
            return $row['BlobStorageID'];
        }, $rows);
    }
}
