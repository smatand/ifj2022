<?php
declare(strict_types=1);

function voidf(int $x) : int{
    return 1;
}
$x = voidf(5);
write($x,"\n");
