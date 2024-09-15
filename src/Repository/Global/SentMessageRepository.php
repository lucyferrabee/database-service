<?php

declare(strict_types = 1);

namespace App\Repository\Global;

use Doctrine\DBAL\Connection;

class SentMessageRepository extends BaseGlobalRepository
{
    public function __construct(Connection $connection, int $batchSize = 1000)
    {
        parent::__construct($connection, 'proton_mail_global.SentMessage', $batchSize);
    }
}
