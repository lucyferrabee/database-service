<?php

declare(strict_types = 1);

namespace App\Model\Shard;

class ContactData
{
    public string $blobStorageID;

    public function getBlobStorageID(): ?string
    {
        return $this->blobStorageID;
    }

    public function setBlobStorageID(string $blobStorageID): self
    {
        $this->blobStorageID = $blobStorageID;
    }
}
