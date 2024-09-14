<?php

namespace App\Model\Shard;

class MessageData
{
    /**
     * @var string|null
     */
    public ?string $blobStorageID;

    /**
     * @var string|null
     */
    public ?string $body;

    /**
     * @var string|null
     */
    public ?string $header;

    public function getBlobStorageID(): ?string
    {
        return $this->blobStorageID;
    }

    public function setBlobStorageID(?string $blobStorageID): self
    {
        $this->blobStorageID = $blobStorageID;
        return $this;
    }

    public function getBody(): ?string
    {
        return $this->body;
    }

    public function setBody(?string $body): self
    {
        $this->body = $body;
        return $this;
    }

    public function getHeader(): ?string
    {
        return $this->header;
    }

    public function setHeader(?string $header): self
    {
        $this->header = $header;
        return $this;
    }
}