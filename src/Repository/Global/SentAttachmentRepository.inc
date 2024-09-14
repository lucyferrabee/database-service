<?php

namespace App\Repository\Global;

use Doctrine\DBAL\Connection;
use App\Repository\BaseBlobRepository;

class SentAttachmentRepository extends BaseBlobRepository
{
    public function __construct(Connection $connection, int $batchSize = 1000)
    {
        parent::__construct($connection, 'proton_mail_global.SentAttachment', $batchSize);
    }
}

