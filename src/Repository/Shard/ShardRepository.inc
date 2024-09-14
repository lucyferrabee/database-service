<?php

namespace App\Repository\Shard;

use Doctrine\DBAL\Connection;
use App\Repository\BaseBlobRepository;

class ShardRepository extends BaseBlobRepository
{
    public function __construct(Connection $connection)
    {
        parent::__construct($connection, 'proton_mail_shard.MessageData');
    }
}
