<?php

declare(strict_types = 1);

namespace App\Service;

use App\Repository\Global\BlobStorageRepository;
use App\Repository\Shard\AttachmentRepository;
use App\Repository\Shard\ContactDataRepository;
use App\Repository\Shard\MessageDataRepository;
use App\Repository\Shard\OutsideAttachmentRepository;
use App\Repository\Global\SentAttachmentRepository;
use App\Repository\Global\SentMessageRepository;
use Doctrine\DBAL\Exception;

class GetReferencesCounts
{
    private BlobStorageRepository $blobStorageRepository;
    private SentAttachmentRepository $sentAttachmentRepository;
    private SentMessageRepository $sentMessageRepository;
    private AttachmentRepository $attachmentRepository;
    private ContactDataRepository $contactDataRepository;
    private MessageDataRepository $messageDataRepository;
    private OutsideAttachmentRepository $outsideAttachmentRepository;

    public function __construct(
        BlobStorageRepository $blobStorageRepository,
        SentAttachmentRepository $sentAttachmentRepository,
        SentMessageRepository $sentMessageRepository,
        AttachmentRepository $attachmentRepository,
        ContactDataRepository $contactDataRepository,
        MessageDataRepository $messageDataRepository,
        OutsideAttachmentRepository $outsideAttachmentRepository
    ) {
        $this->blobStorageRepository = $blobStorageRepository;
        $this->sentAttachmentRepository = $sentAttachmentRepository;
        $this->sentMessageRepository = $sentMessageRepository;
        $this->attachmentRepository = $attachmentRepository;
        $this->contactDataRepository = $contactDataRepository;
        $this->messageDataRepository = $messageDataRepository;
        $this->outsideAttachmentRepository = $outsideAttachmentRepository;
    }

    /**
     * @throws Exception
     */
    public function getNumReferences(): array
    {
        $batchSize = 1000;
        $start = 0;

        $refSentAttachment = $this->sentAttachmentRepository->getBlobReferences($start, $batchSize);
        $refSentMessage = $this->sentMessageRepository->getBlobReferences($start, $batchSize);
        $refAttachment = $this->attachmentRepository->getBlobReferences($start, $batchSize);
        $refContactData = $this->contactDataRepository->getBlobReferences($start, $batchSize);
        $refMessageData = $this->messageDataRepository->getBlobReferences($start, $batchSize);
        $refOutsideAttachment = $this->outsideAttachmentRepository->getBlobReferences($start, $batchSize);

        $actualNumReferences = ArraySumService::sumArrayValues([
            $refSentAttachment,
            $refSentMessage,
            $refAttachment,
            $refContactData,
            $refMessageData,
            $refOutsideAttachment,
        ]);

        $numReferences = $this->blobStorageRepository->getNumReferences(array_keys($actualNumReferences));

        return [
            'actualNumReferences' => $actualNumReferences,
            'numReferences' => $this->toSimpleArray($numReferences),
        ];
    }

    private function toSimpleArray(array $numReferences): array
    {
        $numReferencesToSimpleArray = [];
        array_walk(
            $numReferences,
            function ($elem) use (&$numReferencesToSimpleArray) {
                $numReferencesToSimpleArray[$elem['id']] = $elem['num'];
            }
        );

        return $numReferencesToSimpleArray;
    }
}
