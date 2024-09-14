<?php

declare(strict_types = 1);

namespace App\Repository\Global;

use Doctrine\DBAL\Connection;

class BlobStorageRepository
{
    private Connection $connection;

    public function __construct(Connection $connection)
    {
        $this->connection = $connection;
    }

    public function getNumReferences(array $ids): array
    {
        return $this->connection
            ->createQueryBuilder()
            ->from('proton_mail_global.BlobStorage')
            ->select('BlobStorageID as id, NumReferences as num')
            ->where('BlobStorageID IN (:ids)')
            ->setParameter('ids', $ids, \Doctrine\DBAL\Connection::PARAM_STR_ARRAY)
            ->executeQuery()
            ->fetchAllAssociative();
    }
}
