<?php

declare(strict_types = 1);

namespace App\Model\Global;

class Attachment
{
    private string $blobStorageID;

    public function __construct(string $blobStorageID)
    {
        $this->blobStorageID = $blobStorageID;
    }

    public function getBlobStorageID(): string
    {
        return $this->blobStorageID;
    }

    public function setBlobStorageID(string $blobStorageID): void
    {
        $this->blobStorageID = $blobStorageID;
    }
}
