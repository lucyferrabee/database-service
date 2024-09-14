<?php

namespace App\Repository\Shard;

use Doctrine\DBAL\Connection;
use App\Repository\BaseBlobRepository;

class MessageDataRepository extends BaseBlobRepository
{
    public function __construct(Connection $connection, int $batchSize = 1000)
    {
        parent::__construct($connection, 'proton_mail_shard.MessageData', $batchSize);
    }

    public function getBlobReferences(int $start, int $batchSize): array
    {
        $bodyRows = $this->fetchBatch('Body', $start, $batchSize);
        $headerRows = $this->fetchBatch('Header', $start, $batchSize);

        return array_merge(
            $this->mapBlobReferences($bodyRows),
            $this->mapBlobReferences($headerRows)
        );
    }

    private function fetchBatch(string $column, int $start, int $batchSize): array
    {
        return $this->connection
            ->createQueryBuilder()
            ->select($column)
            ->from($this->tableName)
            ->setFirstResult($start)
            ->setMaxResults($batchSize)
            ->executeQuery()
            ->fetchAllAssociative();
    }

    private function mapBlobReferences(array $rows): array
    {
        $blobCounts = [];
        foreach ($rows as $row) {
            $id = $row['Body'] ?? $row['Header'];
            if ($id) {
                if (!isset($blobCounts[$id])) {
                    $blobCounts[$id] = 0;
                }
                $blobCounts[$id]++;
            }
        }
        return $blobCounts;
    }
}
