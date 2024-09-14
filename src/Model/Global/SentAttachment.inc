<?php

declare(strict_types = 1);

namespace App\Model\Global;

class SentAttachment
{
    public string $blobStorageID;

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