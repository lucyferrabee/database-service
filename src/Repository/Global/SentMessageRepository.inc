<?php

declare(strict_types = 1);

namespace App\Repository\Global;

use Doctrine\DBAL\Connection;
use App\Repository\BaseBlobRepository;

class SentMessageRepository extends BaseBlobRepository
{
    public function __construct(Connection $connection, int $batchSize = 1000)
    {
        parent::__construct($connection, 'proton_mail_global.SentMessage', $batchSize);
    }
}
