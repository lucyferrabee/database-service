<?php

namespace App\Repository;

use App\Entity\MessageData;
use Doctrine\Bundle\DoctrineBundle\Repository\ServiceEntityRepository;
use Doctrine\Persistence\ManagerRegistry;

class MessageDataRepository extends ServiceEntityRepository
{
    public function __construct(ManagerRegistry $registry)
    {
        parent::__construct($registry, MessageData::class);
    }

    public function findByBodyReference(string $blobID): array
    {
        return $this->createQueryBuilder('m')
            ->where('m.body = :blobID')
            ->setParameter('blobID', $blobID)
            ->getQuery()
            ->getResult();
    }
}
