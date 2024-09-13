<?php

namespace App\Entity\Global;

use Doctrine\ORM\Mapping as ORM;

/**
 * @ORM\Entity(repositoryClass="App\Repository\BlobStorageRepository")
 * @ORM\Table(name="BlobStorage")
 */
class BlobStorage
{
    /**
     * @ORM\Id
     * @ORM\Column(type="string", length=255)
     */
    private $blobStorageID;

    /**
     * @ORM\Column(type="integer")
     */
    private $numReferences;

    public function getBlobStorageID(): ?string
    {
        return $this->blobStorageID;
    }

    public function setBlobStorageID(string $blobStorageID): self
    {
        $this->blobStorageID = $blobStorageID;

        return $this;
    }

    public function getNumReferences(): ?int
    {
        return $this->numReferences;
    }

    public function setNumReferences(int $numReferences): self
    {
        $this->numReferences = $numReferences;

        return $this;
    }
}
