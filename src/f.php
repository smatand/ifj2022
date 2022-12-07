<?php
declare(strict_types=1);
$a = reads();
if($a === "A") {
    $x = 5;
} else {
    $x; // = 0.5;
}
$b = reads();
if($b === "A") {
    $y = 1;
} else {
    if(0 === 1) {
        $y = 0.5;
    } else {}
}
$res = $x + $y;
$x = 0.1;
$y = 0.2;
write($res, "\n");


// <?php
// declare(strict_types=1);
// $x = reads();
// if($x === "A") {
//     $x = 5;
// } else {
//     $x = 2.5;
// }
// $y = reads();
// if($y === "A") {
//     $y = 1;
// } else {
//     $y = 0.5;
// }
// $res = $x + $y;
// write($res, "\n");


