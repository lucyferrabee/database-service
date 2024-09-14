<?php

namespace App\Service\Shard;

use Doctrine\DBAL\Connection;
use App\Model\Shard\MessageData;

class MessageDataService
{
    private Connection $shardConnection;
    private int $batchSize;

    public function __construct(Connection $shardConnection, int $batchSize = 1000)
    {
        $this->shardConnection = $shardConnection;
        $this->batchSize = $batchSize;
    }

    public function getMessageData(int $start): array
    {
        $rows = $this->shardConnection->fetchAllAssociative(
            'SELECT Body, Header FROM proton_mail_shard.MessageData LIMIT :start, :batchSize',
            ['start' => $start, 'batchSize' => $this->batchSize],
            ['start' => \PDO::PARAM_INT, 'batchSize' => \PDO::PARAM_INT]
        );

        $messageDataList = [];
        foreach ($rows as $row) {
            $messageData = new MessageData();
            $messageData->setBody($row['Body']);
            $messageData->setHeader($row['Header']);
            $messageDataList[] = $messageData;
        }

        return $messageDataList;
    }
}
