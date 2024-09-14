<?php

namespace App\Model\Global;

/**
 * Class representing the structure of the `proton_mail_global.SentMessage` table.
 */
class SentMessage
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
