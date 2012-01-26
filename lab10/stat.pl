#!/usr/bin/perl -w
use POSIX qw(ceil floor);
use GD::Graph::bars;
use GD::Graph::colour qw(:colours :lists :files :convert);
use List::Util qw(max);
use feature 'unicode_strings';
use strict;
my @keys=qw/1000 10000 100000 1000000/;
my @datapoints_map=(0.282, 2.782, 33.311, 418.649);
my @datapoints_dis_pmap=(0.320, 3.159, 33.009, 423.543);
my @datapoints_en_pmap=(0.166, 1.719, 17.213, 176.461);
my @datapoints_loc_clr=(0.201, 2.128, 21.294, 213.731);
my @datapoints_big_clr=(0.121, 1.112, 10.927, 122.478);

my $max = max(@datapoints_map,@datapoints_en_pmap,@datapoints_dis_pmap);
print @datapoints_map;
print @datapoints_en_pmap;
my @data=(\@keys,\@datapoints_map,\@datapoints_en_pmap,\@datapoints_dis_pmap,\@datapoints_loc_clr,\@datapoints_big_clr);
my $graph = GD::Graph::bars->new(scalar(@keys)*100+100, 360);
$graph->set_legend("map","pmap (en)","pmap (dis)",'pmap (clr@loc)','pmap (clr@net)');
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
open(IMG, ">watki-large.png") or die $!;
binmode IMG;
print IMG $graph->plot(\@data)->png;

