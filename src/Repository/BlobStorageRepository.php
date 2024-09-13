<?php

namespace App\Repository;

use App\Entity\BlobStorage;
use Doctrine\Bundle\DoctrineBundle\Repository\ServiceEntityRepository;
use Doctrine\Persistence\ManagerRegistry;

class BlobStorageRepository extends ServiceEntityRepository
{
    public function __construct(ManagerRegistry $registry)
    {
        parent::__construct($registry, BlobStorage::class);
    }

    public function findByBlobID(string $blobID): ?BlobStorage
    {
        return $this->find($blobID);
    }

    public function findBlobsWithMismatch(int $expectedReferences): array
    {
        return $this->createQueryBuilder('b')
            ->where('b.numReferences != :expectedReferences')
            ->setParameter('expectedReferences', $expectedReferences)
            ->getQuery()
            ->getResult();
    }

    public function findBlobsWithZeroReferences(): array
    {
        return $this->createQueryBuilder('b')
            ->where('b.numReferences = 0')
            ->getQuery()
            ->getResult();
    }
}
