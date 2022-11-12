<?php
declare(strict_types=1);
// Program 1: Vypocet faktorialu (iterativne)

// Hlavni telo programu
write("Zadejte cislo pro vypocet faktorialu\n");
$a = readi();

if ($a === null) {
    write("Chyba pri nacitani celeho cisla!\n");
} else {}

if ($a < 0) {
    write("Faktorial nelze spocitat\n");
} else {
    $vysl = 1;
    while ($a > 0) {
        $vysl = $vysl * $a;
        $a = $a - 1;
    }
    write("Vysledek je: ", $vysl, "\n");
}
// so if the input is 10, the output is 3628800
// if the input isn't a number, the output is "Chyba pri nacitani celeho cisla!"
// if the input is a negative number, the output is "Faktorial nelze spocitat"
?>
