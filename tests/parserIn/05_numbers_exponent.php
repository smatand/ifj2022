<?php
declare(strict_types=1);

// function toPower which iteratively calculates the power of a number
function toPower(int $base, int $exponent) : int {
    $result = 1;
    while ($exponent > 0) {
        $result = $result * $base;
        $exponent = $exponent - 1;
    }
    return $result;
}

// Program 5: Prace s cisly
$a = 2;
$b = -2.0;
$c = 3.5;
$d = 1e2;

write($a, $b, $c, $d, "\n");

$sum = $a + $b + $c + $d;

write("Soucet nahore uvedenych cisel: ", $sum, "\n");
write("Zadejte cislo kterym chcete umocnit soucet: ");
$e = readi();

// write is built in function to print to stdout terms give in parameters
write("Vysledek je: ", toPower($sum, $e), "\n");
?>

// output: 
// 2 -2 3.5 100
// Soucet nahore uvedenych cisel: 105.5
// Zadejte cislo kterym chcete umocnit soucet: 2
//
// Vysledek je: 11130.25