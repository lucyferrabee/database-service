<?php

declare(strict_types = 1);

namespace App\Model\Global;

class BlobStorage
{
    private string $blobStorageID;
    private int $numReferences;

    public function __construct(string $blobStorageID, int $numReferences)
    {
        $this->blobStorageID = $blobStorageID;
        $this->numReferences = $numReferences;
    }

    public function getBlobStorageID(): string
    {
        return $this->blobStorageID;
    }

    public function setBlobStorageID(string $blobStorageID): void
    {
        $this->blobStorageID = $blobStorageID;
    }

    public function getNumReferences(): int
    {
        return $this->numReferences;
    }

    public function setNumReferences(int $numReferences): void
    {
        $this->numReferences = $numReferences;
    }
}
