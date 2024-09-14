<?php

namespace App\Repository\Global;

use Doctrine\DBAL\Connection;
use App\Repository\BaseBlobRepository;

class GlobalRepository extends BaseBlobRepository
{
    public function __construct(Connection $connection)
    {
        parent::__construct($connection, 'proton_mail_global.BlobStorage');
    }
}
