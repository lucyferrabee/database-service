<?php

declare(strict_types = 1);

namespace App\Service;

class ArraySumService
{
    public static function sumArrayValues(array $arrays): array
    {
        $result = [];

        foreach ($arrays as $array) {
            foreach ($array as $id => $count) {
                if (!isset($result[$id])) {
                    $result[$id] = 0;
                }
                $result[$id] += $count;
            }
        }

        return $result;
    }
}
