<?php

declare(strict_types = 1);

namespace App\Repository\Shard;

use Doctrine\DBAL\Connection;

class AttachmentRepository extends BaseShardRepository
{
    public function __construct(Connection $connection, int $batchSize = 1000)
    {
        parent::__construct($connection, 'proton_mail_shard.Attachment', $batchSize);
    }
}
