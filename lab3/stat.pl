#!/usr/bin/perl -w
use POSIX qw(ceil floor);
use GD::Graph::bars;
use GD::Graph::colour qw(:colours :lists :files :convert);
use List::Util qw(max);
use feature 'unicode_strings';
use strict;
my @keys=qw/10 20 50 100/;# 250/;# 1000/;
my @datapoints_fine=();
my @datapoints_norm=();

foreach my $val (@keys){
	my $total=0;
	for(1 .. 3){
		open(HUM,"/usr/bin/time -f t=%e java Test $ARGV[0] $val fine 2>&1 |");
		while(<HUM>){
			if(/^t=(.+)$/){
				#print $1." ";
				$total+=$1;
			}
		}
		close HUM;
	}
	printf "%d,%d fine: %.3f\n",$ARGV[0],$val,$total/3;
	push(@datapoints_fine,sprintf("%.3f",$total/3));
	$total=0;
	for(1 .. 3){
		open(HUM,"/usr/bin/time -f t=%e java Test $ARGV[0] $val norm 2>&1 |");
		while(<HUM>){
			if(/^t=(.+)$/){
				#print $1." ";
				$total+=$1;
			}
		}
		close HUM;
	}
	printf "%d,%d norm: %.3f\n",$ARGV[0],$val,$total/3;
	push(@datapoints_norm,sprintf("%.3f",$total/3));
}

my $max = max(@datapoints_fine,@datapoints_norm);
print @datapoints_fine;
print @datapoints_norm;
my @data=(\@keys,\@datapoints_fine,\@datapoints_norm);
my $graph = GD::Graph::bars->new(scalar(@keys)*30+100, 240);
$graph->set_legend("drobniozarniste","standardowe");
$graph->set( 
	x_label           => "iteracje",
	y_label           => "czas [s]",
	title             => $ARGV[0]." watkow (100el)",
	y_max_value       => 1.25*$max,
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
open(IMG, ">stats_".$ARGV[0].".png") or die $!;
binmode IMG;
print IMG $graph->plot(\@data)->png;

