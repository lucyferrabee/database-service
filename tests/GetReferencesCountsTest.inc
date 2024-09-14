<?php

declare(strict_types = 1);

namespace App\Tests;

use PHPUnit\Framework\TestCase;
use App\Service\GetReferencesCounts;
use App\Repository\Global\BlobStorageRepository;
use App\Repository\Global\SentAttachmentRepository;
use App\Repository\Global\SentMessageRepository;
use App\Repository\Shard\AttachmentRepository;
use App\Repository\Shard\ContactDataRepository;
use App\Repository\Shard\MessageDataRepository;
use App\Repository\Shard\OutsideAttachmentRepository;

class GetReferencesCountsTest extends TestCase
{
    private mixed $blobStorageRepository;
    private mixed $sentAttachmentRepository;
    private mixed $sentMessageRepository;
    private mixed $attachmentRepository;
    private mixed $contactDataRepository;
    private mixed $messageDataRepository;
    private mixed $outsideAttachmentRepository;
    private GetReferencesCounts $getReferencesCounts;

    protected function setUp(): void
    {
        // Create mocks for each repository
        $this->blobStorageRepository = $this->createMock(BlobStorageRepository::class);
        $this->sentAttachmentRepository = $this->createMock(SentAttachmentRepository::class);
        $this->sentMessageRepository = $this->createMock(SentMessageRepository::class);
        $this->attachmentRepository = $this->createMock(AttachmentRepository::class);
        $this->contactDataRepository = $this->createMock(ContactDataRepository::class);
        $this->messageDataRepository = $this->createMock(MessageDataRepository::class);
        $this->outsideAttachmentRepository = $this->createMock(OutsideAttachmentRepository::class);

        // Instantiate the service with mocked repositories
        $this->getReferencesCounts = new GetReferencesCounts(
            $this->blobStorageRepository,
            $this->sentAttachmentRepository,
            $this->sentMessageRepository,
            $this->attachmentRepository,
            $this->contactDataRepository,
            $this->messageDataRepository,
            $this->outsideAttachmentRepository
        );
    }

    public function testGetNumReferences()
    {
        // Define mock return values for repositories
        $this->sentAttachmentRepository->expects($this->once())
            ->method('getBlobReferences')
            ->willReturn([
                'blob1' => 3,
                'blob2' => 2,
            ]);

        $this->sentMessageRepository->expects($this->once())
            ->method('getBlobReferences')
            ->willReturn([
                'blob2' => 1,
                'blob3' => 4,
            ]);

        $this->attachmentRepository->expects($this->once())
            ->method('getBlobReferences')
            ->willReturn([
                'blob1' => 2,
                'blob4' => 5,
            ]);

        $this->contactDataRepository->expects($this->once())
            ->method('getBlobReferences')
            ->willReturn([
                'blob1' => 1,
                'blob5' => 6,
            ]);

        $this->messageDataRepository->expects($this->once())
            ->method('getBlobReferences')
            ->willReturn([
                'blob2' => 3,
                'blob6' => 7,
            ]);

        $this->outsideAttachmentRepository->expects($this->once())
            ->method('getBlobReferences')
            ->willReturn([
                'blob3' => 8,
                'blob7' => 9,
            ]);

        $this->blobStorageRepository->expects($this->once())
            ->method('getNumReferences')
            ->with(['blob1', 'blob2', 'blob3', 'blob4', 'blob5', 'blob6', 'blob7'])
            ->willReturn([
                ['id' => 'blob1', 'num' => 6],
                ['id' => 'blob2', 'num' => 5],
                ['id' => 'blob3', 'num' => 12],
                ['id' => 'blob4', 'num' => 10],
                ['id' => 'blob5', 'num' => 8],
                ['id' => 'blob6', 'num' => 14],
                ['id' => 'blob7', 'num' => 15],
            ]);

        // Call the method and assert the results
        $result = $this->getReferencesCounts->getNumReferences();
        $expectedActualNumReferences = [
            'blob1' => 6,
            'blob2' => 5,
            'blob3' => 12,
            'blob4' => 10,
            'blob5' => 8,
            'blob6' => 14,
            'blob7' => 15,
        ];

        $expectedNumReferences = [
            'blob1' => 6,
            'blob2' => 5,
            'blob3' => 12,
            'blob4' => 10,
            'blob5' => 8,
            'blob6' => 14,
            'blob7' => 15,
        ];

        $this->assertEquals($expectedActualNumReferences, $result['actualNumReferences']);
        $this->assertEquals($expectedNumReferences, $result['numReferences']);
    }
}
