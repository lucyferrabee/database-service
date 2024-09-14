<?php

declare(strict_types = 1);

namespace App\Tests;

use PHPUnit\Framework\TestCase;
use Doctrine\DBAL\Connection;
use Doctrine\DBAL\Query\QueryBuilder;
use App\Repository\Global\BlobStorageRepository;

class BlobStorageRepositoryTest extends TestCase
{
    private $connection;
    private $queryBuilder;
    private $repository;

    protected function setUp(): void
    {
        $this->connection = $this->createMock(Connection::class);
        $this->queryBuilder = $this->createMock(QueryBuilder::class);
        $this->repository = new BlobStorageRepository($this->connection);
    }

    public function testGetBlobReferences()
    {
        $this->connection->method('createQueryBuilder')
            ->willReturn($this->queryBuilder);

        $this->queryBuilder->expects($this->once())
            ->method('from')
            ->with('proton_mail_global.BlobStorage', 't')
            ->willReturnSelf();

        $this->queryBuilder->expects($this->once())
            ->method('select')
            ->with('t.BlobStorageID as id', 'count(t.BlobStorageID) as count')
            ->willReturnSelf();

        $this->queryBuilder->expects($this->once())
            ->method('groupBy')
            ->with('t.BlobStorageID')
            ->willReturnSelf();

        $this->queryBuilder->expects($this->once())
            ->method('setFirstResult')
            ->with(0)
            ->willReturnSelf();

        $this->queryBuilder->expects($this->once())
            ->method('setMaxResults')
            ->with(1000)
            ->willReturnSelf();

        $statement = $this->createMock(\Doctrine\DBAL\Statement::class);

        $statement->expects($this->once())
            ->method('fetchAll')
            ->willReturn([
                ['id' => 'blob1', 'count' => 5],
                ['id' => 'blob2', 'count' => 10],
            ]);

        $this->queryBuilder->expects($this->once())
            ->method('executeQuery')
            ->willReturn($statement);

        // Call the method and assert the results
        $result = $this->repository->getBlobReferences(0, 1000);
        $expected = [
            'blob1' => 5,
            'blob2' => 10,
        ];

        $this->assertEquals($expected, $result);
    }

    public function testGetNumReferences()
    {
        // Configure the QueryBuilder mock
        $this->connection->method('createQueryBuilder')
            ->willReturn($this->queryBuilder);

        $this->queryBuilder->expects($this->once())
            ->method('from')
            ->with('proton_mail_global.BlobStorage', 't')
            ->willReturnSelf();

        $this->queryBuilder->expects($this->once())
            ->method('select')
            ->with('t.BlobStorageID as id', 't.NumReferences as num')
            ->willReturnSelf();

        $this->queryBuilder->expects($this->once())
            ->method('where')
            ->with('t.BlobStorageID IN (:ids)')
            ->willReturnSelf();

        $this->queryBuilder->expects($this->once())
            ->method('setParameter')
            ->with('ids', ['blob1', 'blob2'], \Doctrine\DBAL\Connection::PARAM_STR_ARRAY)
            ->willReturnSelf();

        $statement = $this->createMock(\Doctrine\DBAL\Statement::class);

        $statement->expects($this->once())
            ->method('fetchAllAssociative')
            ->willReturn([
                ['id' => 'blob1', 'num' => 5],
                ['id' => 'blob2', 'num' => 10],
            ]);

        $this->queryBuilder->expects($this->once())
            ->method('executeQuery')
            ->willReturn($statement);

        // Call the method and assert the results
        $result = $this->repository->getNumReferences(['blob1', 'blob2']);
        $expected = [
            ['id' => 'blob1', 'num' => 5],
            ['id' => 'blob2', 'num' => 10],
        ];

        $this->assertEquals($expected, $result);
    }
}
