<?php

namespace App\Tests\Service;

use App\Service\BlobReferenceService;
use Doctrine\DBAL\Connection;
use PHPUnit\Framework\TestCase;
use Symfony\Component\Console\Style\SymfonyStyle;
use PHPUnit\Framework\MockObject\MockObject;
use ReflectionMethod;

class BlobReferenceServiceTest extends TestCase
{
    /** @var MockObject|Connection */
    private $shardConnection;

    /** @var MockObject|Connection */
    private $globalConnection;

    /** @var BlobReferenceService */
    private $service;

    protected function setUp(): void
    {
        $this->shardConnection = $this->createMock(Connection::class);
        $this->globalConnection = $this->createMock(Connection::class);
        $this->service = new BlobReferenceService($this->shardConnection, $this->globalConnection);
    }

    public function testProcessReferencesWithNoRows()
    {
        $this->shardConnection->method('fetchOne')->willReturn(0);
        $this->shardConnection->method('fetchAllAssociative')->willReturn([]);

        $symfonyStyle = $this->createMock(SymfonyStyle::class);
        $symfonyStyle->expects($this->once())
            ->method('text')
            ->with('Total rows to process in MessageData: 0');

        $this->service->processReferences($symfonyStyle);
    }

    public function testProcessReferencesWithSomeRows()
    {
        $this->shardConnection->method('fetchOne')->willReturn(10);
        $this->shardConnection->method('fetchAllAssociative')->willReturn([
            ['Body' => 'blob1', 'Header' => 'blob2'],
        ]);
    
        $symfonyStyle = $this->createMock(SymfonyStyle::class);
        $symfonyStyle->expects($this->exactly(2))
            ->method('text')
            ->withConsecutive(
                ['Total rows to process in MessageData: 10'],
                ['Processing batch starting from row 0']
            );
    
        $this->globalConnection->expects($this->exactly(1))
            ->method('fetchOne')
            ->willReturn(1); // Adjust if you need different return values
    
        $this->service->processReferences($symfonyStyle);
    }

    public function testCheckBlobConsistencyWithMissingBlob()
    {
        $this->globalConnection->method('fetchOne')->willReturn(false);
    
        $symfonyStyle = $this->createMock(SymfonyStyle::class);
        $symfonyStyle->expects($this->once())
            ->method('text')
            ->with('Missing blob reference: blob1');
    
        $this->expectOutputString("Missing blob reference: blob1\n");
    
        $this->invokeProtectedMethod('checkBlobConsistency', 'blob1', null);
    }    

    public function testCheckBlobConsistencyWithExistingBlob()
    {
        $this->globalConnection->method('fetchOne')->willReturn(5);

        $symfonyStyle = $this->createMock(SymfonyStyle::class);
        $symfonyStyle->expects($this->never())
            ->method('text');

        $this->expectOutputString("Blob ID blob1 exists with 5 references.\n");

        $this->invokeProtectedMethod('checkBlobConsistency', 'blob1', null);
    }

    public function testFetchBatchWithValidData()
    {
        $this->shardConnection->method('fetchAllAssociative')->willReturn([
            ['Body' => 'blob1', 'Header' => 'blob2'],
        ]);

        $result = $this->invokeProtectedMethod('fetchBatch', 'proton_mail_shard.MessageData', 0, 1);
        $this->assertCount(1, $result);
        $this->assertEquals('blob1', $result[0]['Body']);
        $this->assertEquals('blob2', $result[0]['Header']);
    }

    public function testGetTotalRowCountWithValidCount()
    {
        $this->shardConnection->method('fetchOne')->willReturn(123);

        $count = $this->invokeProtectedMethod('getTotalRowCount', 'proton_mail_shard.MessageData');
        $this->assertEquals(123, $count);
    }

    public function testGetTotalRowCountWithZero()
    {
        $this->shardConnection->method('fetchOne')->willReturn(0);

        $count = $this->invokeProtectedMethod('getTotalRowCount', 'proton_mail_shard.MessageData');
        $this->assertEquals(0, $count);
    }

    public function testFetchBatchWithEmptyResult()
    {
        $this->shardConnection->method('fetchAllAssociative')->willReturn([]);

        $result = $this->invokeProtectedMethod('fetchBatch', 'proton_mail_shard.MessageData', 0, 1);
        $this->assertCount(0, $result);
    }

    public function testBlobReferenceCountMismatch()
    {
        $this->globalConnection->method('fetchOne')->willReturn(0);
    
        $symfonyStyle = $this->createMock(SymfonyStyle::class);
        $symfonyStyle->expects($this->once())
            ->method('text')
            ->with('Missing blob reference: blob1');
    
        $this->expectOutputString("Missing blob reference: blob1\n");
    
        $this->invokeProtectedMethod('checkBlobConsistency', 'blob1', null);
    }    

    private function invokeProtectedMethod(string $methodName, ...$args)
    {
        $reflection = new ReflectionMethod($this->service, $methodName);
        $reflection->setAccessible(true);
        return $reflection->invoke($this->service, ...$args);
    }
}
