#!/usr/bin/perl -w
use POSIX qw(ceil floor);
use GD::Graph::bars;
use GD::Graph::colour qw(:colours :lists :files :convert);
use List::Util qw(max);
use feature 'unicode_strings';
use strict;
my @keys=qw/1000 10000 100000 1000000/;
my @datapoints_map=(0.005, 0.073, 1.243, 7.520);
my @datapoints_en_pmap=(0.006, 0.074, 0.773, 7.615);

my $max = max(@datapoints_map,@datapoints_en_pmap);
print @datapoints_map;
print @datapoints_en_pmap;
my @data=(\@keys,\@datapoints_map,\@datapoints_en_pmap);
my $graph = GD::Graph::bars->new(scalar(@keys)*50+100, 360);
$graph->set_legend("map","pmap (en)");
$graph->set( 
	x_label           => "watki",
	y_label           => "czas [s]",
	title             => "wspolbiezna mapa",
	y_max_value       => int(1.25*$max),
	y_tick_number     => 5,
	y_label_skip      => 1,
	x_label_skip      => 1,
	labelclr          => "green",
	fgclr             => "lgray",
	bgclr             => "black",
	textclr           => "lred",
	valuesclr         => "orange",
	legendclr         => "orange",
	values_vertical   => 1,
	y_long_ticks      => 1,
	transparent       => 0,
	show_values       => 1,
	bar_spacing       => 2,
	axislabelclr      => "green"
) or die $graph->error;
open(IMG, ">watki-short.png") or die $!;
binmode IMG;
print IMG $graph->plot(\@data)->png;

