#!/usr/bin/env perl

use strict;
use v5.24;

sub reduce {
	my ($infn, $size, $outfn) = @_;
	open my $infh, '<', $infn;
	open my $outfh, '>', $outfn;

	my $group = 0;
	my $count = 0;
	my $lineno = 0;
	my $entities = 0;
	while(my $line = <$infh>) {
		++$lineno;
		if($lineno % $size == 0) {
			my $avg = $group / $size;
			$entities = $entities / $size;
			print $outfh "$entities\t$avg\n";
			$group = 0;
			$count = 0;
			$entities = 0;
		}
		my @cols = split /\t/, $line;
		$entities += $cols[0];
		$group += $cols[1];
		++$count;
	}

	if($count > 0) {
		my $avg = $group / $count;
		$entities = $entities / $count;
		print $outfh "$entities\t$avg\n";
	}

	close $infh;
	close $outfh;
}

reduce "data/ticks.oo_20000_10", 100, "data/rticks.oo_20000_10";
reduce "data/ticks.do0_20000_10", 100, "data/rticks.do0_20000_10";
reduce "data/ticks.do1_20000_10", 100, "data/rticks.do1_20000_10";
