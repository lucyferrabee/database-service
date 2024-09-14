<?php

namespace App\Model\Shard;

class OutsideAttachment
{
    /**
     * @var string|null
     */
    public ?string $blobStorageID;

    public function getBlobStorageID(): ?string
    {
        return $this->blobStorageID;
    }

    public function setBlobStorageID(?string $blobStorageID): self
    {
        $this->blobStorageID = $blobStorageID;
        return $this;
    }
}