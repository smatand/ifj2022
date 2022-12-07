<?php
declare(strict_types=1);
$x = readi();
$y = readi();
$xd = $y;
while($x){
	write(":xdd:");
	$x = $x -1;
	while($y){
		write(" 1");
		$y = $y -1;
	}
	$y = $xd;
	write("\n");
}
