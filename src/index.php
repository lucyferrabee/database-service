<?php

namespace App\index;

use PDO;
use PDOException;

$dsnShard = 'mysql:host=mysql_shard;dbname=proton_mail_shard;charset=utf8mb4';
$dsnGlobal = 'mysql:host=mysql_global;dbname=proton_mail_global;charset=utf8mb4';
$username = 'symfony';
$password = 'symfonypass';

//try {
//    $pdo = new PDO($dsnShard, $username, $password);
//    echo "Connected to the shard database successfully!";
//} catch (PDOException $e) {
//    echo "Connection to shard failed: " . $e->getMessage();
//}
//
//try {
//    $pdo = new PDO($dsnGlobal, $username, $password);
//    echo "Connected to the global database successfully!";
//} catch (PDOException $e) {
//    echo "Connection to global failed: " . $e->getMessage();
//}

