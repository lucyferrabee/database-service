<?php

declare(strict_types = 1);

namespace App\Model\Shard;

class MessageData
{
    public ?string $blobStorageID;

    public ?string $body;

    public ?string $header;

    public function getBlobStorageID(): ?string
    {
        return $this->blobStorageID;
    }

    public function setBlobStorageID(?string $blobStorageID): void
    {
        $this->blobStorageID = $blobStorageID;
    }

    public function getBody(): ?string
    {
        return $this->body;
    }

    public function setBody(?string $body): void
    {
        $this->body = $body;
    }

    public function getHeader(): ?string
    {
        return $this->header;
    }

    public function setHeader(?string $header): void
    {
        $this->header = $header;
    }
}
