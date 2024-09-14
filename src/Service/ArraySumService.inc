<?php

namespace App\Service;

class ArraySumService
{
    /**
     * Sum the values of associative arrays where the keys are IDs and the values are counts.
     *
     * @param array $arrays
     * @return array
     */
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

    /**
     * Merge two arrays of counts, summing the values of overlapping keys.
     *
     * @param array $array1
     * @param array $array2
     * @return array
     */
    public static function mergeAndSumCounts(array $array1, array $array2): array
    {
        $result = $array1;

        foreach ($array2 as $id => $count) {
            if (!isset($result[$id])) {
                $result[$id] = 0;
            }
            $result[$id] += $count;
        }

        return $result;
    }
}
