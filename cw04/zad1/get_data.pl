#!/usr/bin/perl
open F1,"wyniki.txt";
@lines=<F1>;
close F1;
$_seeked=$ARGV[0];
$i=0;
@results=( ['fork  '],['vfork '],['clone '],['vclone'] );
foreach $line(@lines){
	if($line=~m/^fork:/){ 
		$mode=0;
		$i++;
	} elsif($line=~m/^vfork:/) {
		$mode=1;
	} elsif($line=~m/^clone:/) {
		$mode=2;
	} elsif($line=~m/^clone \(vfork\):/) {
		$mode=3;
	}
	if($line=~m/\Q$_seeked\E.+ (\d+\.\d+)/){
		$results[$mode][$i]=$1;
	}
}

@sup=(0,16384,65536,131072,262144);
for $i (1..4){
	print $sup[$i]." ";
	for $j(0..3){
		print $results[$j][$i]." ";
	}
	print "\n";
}
