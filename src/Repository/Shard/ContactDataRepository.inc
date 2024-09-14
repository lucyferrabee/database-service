<?php

namespace App\Repository\Shard;

use Doctrine\DBAL\Connection;
use App\Repository\BaseBlobRepository;

class ContactDataRepository extends BaseBlobRepository
{
    public function __construct(Connection $connection, int $batchSize = 1000)
    {
        parent::__construct($connection, 'proton_mail_shard.ContactData', $batchSize);
    }
}
