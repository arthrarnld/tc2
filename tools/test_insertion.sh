#!/bin/bash

bin/oo -i 10000000 > data/insertion.oo_10000000
perl -nle '$sum += $_ } END { print $sum/10000000' <<< `cat data/insertion.oo_10000000 | cut -f2` > data/avg_insertion.oo_10000000
bin/do0 -i 10000000 > data/insertion.do0_10000000
perl -nle '$sum += $_ } END { print $sum/10000000' <<< `cat data/insertion.do0_10000000 | cut -f2` > data/avg_insertion.do0_10000000
bin/do1 -i 10000000 > data/insertion.do1_10000000
perl -nle '$sum += $_ } END { print $sum/10000000' <<< `cat data/insertion.do1_10000000 | cut -f2` > data/avg_insertion.do1_10000000
