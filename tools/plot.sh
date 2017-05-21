#!/bin/bash

if [ "$#" -ne "2" ]; then
	echo "usage: $0 <oo-datafile> <do-datafile>"
	exit -1
fi

gnuplot -e "oo_filename='$1'; do_filename='$2'" `dirname $0`/plot.gp
