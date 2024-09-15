<?php

declare(strict_types = 1);

namespace App\Repository\Global;

use Doctrine\DBAL\Connection;
use Doctrine\DBAL\Exception;

abstract class BaseGlobalRepository
{
    protected Connection $connection;
    protected string $tableName;

    public function __construct(Connection $connection, string $tableName)
    {
        $this->connection = $connection;
        $this->tableName = $tableName;
    }

    /**
     * @throws Exception
     */
    public function getBlobReferences(int $start, int $batchSize): array
    {
        return $this->connection
            ->createQueryBuilder()
            ->from($this->tableName, 't')
            ->select('t.BlobStorageID as id', 'count(t.BlobStorageID) as count')
            ->groupBy('t.BlobStorageID')
            ->setFirstResult($start)
            ->setMaxResults($batchSize)
            ->executeQuery()
            ->fetchAllKeyValue();
    }
}
