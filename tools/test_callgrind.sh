#1/bin/bash

echo -e "\033[1mObject-Oriented\033[0m"
valgrind --tool=callgrind --callgrind-out-file=data/callgrind.out.oo_1000_1 --cache-sim=yes --branch-sim=yes bin/oo 1000 1 >/dev/null 2>data/callgrind.stderr.oo_1000_1
echo -e "\033[1mData-Oriented 0\033[0m"
valgrind --tool=callgrind --callgrind-out-file=data/callgrind.out.do0_1000_1 --cache-sim=yes --branch-sim=yes bin/do0 1000 1 >/dev/null 2>data/callgrind.stderr.do0_1000_1
echo -e "\033[1mData-Oriented 1\033[0m"
valgrind --tool=callgrind --callgrind-out-file=data/callgrind.out.do1_1000_1 --cache-sim=yes --branch-sim=yes bin/do1 1000 1 >/dev/null 2>data/callgrind.stderr.do1_1000_1
