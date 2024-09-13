<?php

namespace App\Service;

use App\Repository\BlobStorageRepository;
use App\Repository\MessageDataRepository;
use Symfony\Component\Console\Style\SymfonyStyle;

class BlobReferenceService
{
    private $blobStorageRepository;
    private $messageDataRepository;

    public function __construct(
        BlobStorageRepository $blobStorageRepository,
        MessageDataRepository $messageDataRepository // Optional: include other repos as needed
    ) {
        $this->blobStorageRepository = $blobStorageRepository;
        $this->messageDataRepository = $messageDataRepository;
    }

    public function checkBlobConsistency(SymfonyStyle $io)
    {
        // Fetch blobs with a reference count mismatch
        $expectedReferences = 1; // Adjust based on logic
        $mismatchedBlobs = $this->blobStorageRepository->findBlobsWithMismatch($expectedReferences);

        foreach ($mismatchedBlobs as $blob) {
            $io->text(sprintf('Blob ID %s exists with %d references (expected %d)', 
                $blob->getBlobStorageID(), 
                $blob->getNumReferences(), 
                $expectedReferences
            ));
        }

        // Handle blobs with zero references
        $zeroReferenceBlobs = $this->blobStorageRepository->findBlobsWithZeroReferences();
        foreach ($zeroReferenceBlobs as $blob) {
            $io->text(sprintf('Blob ID %s has zero references.', $blob->getBlobStorageID()));
        }
    }
}
