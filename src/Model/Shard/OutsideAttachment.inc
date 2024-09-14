<?php

declare(strict_types = 1);

namespace App\Model\Shard;

class OutsideAttachment
{
    public ?string $blobStorageID;

    public function getBlobStorageID(): ?string
    {
        return $this->blobStorageID;
    }

    public function setBlobStorageID(?string $blobStorageID): void
    {
        $this->blobStorageID = $blobStorageID;
    }
}