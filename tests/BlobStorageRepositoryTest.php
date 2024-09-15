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
