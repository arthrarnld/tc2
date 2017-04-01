OODSRC = $(wildcard src/ood/*.cpp)
COMMSRC = $(wildcard src/common/*.cpp)

all: tests

ood: common
	c++ -std=c++11 -fPIC -I./include -c $(OODSRC)
	c++ -L./bin -lcommon -shared -o bin/libood.so *.o
	rm *.o

common:
	c++ -std=c++11 -fPIC -I./include -c $(COMMSRC)
	c++ -shared -o bin/libcommon.so *.o
	rm *.o

tests: ood
	c++ -std=c++11 -Wl,-rpath '-Wl,$$ORIGIN' -I./include -L./bin -lcommon -lood tests/ood.cpp -o bin/ood

clean:
	rm -f bin/*
