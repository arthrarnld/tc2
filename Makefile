OODSRC = $(wildcard src/ood/*.cpp)
COMMSRC = $(wildcard src/common/*.cpp)

all: ood tests

ood: common
	c++ -std=c++11 -I./include -c $(OODSRC)
	ar -cvq libood.a $(wildcard obj/*.o)
	rm *.o
	mv libood.a lib

common:
	c++ -std=c++11 -I./include -c $(COMMSRC)

tests:
	cd tests && for f in ./*.cpp; do \
		c++ -std=c++11 -I../include -L../lib -lood $$f -o `basename $$f .cpp`; \
		done
