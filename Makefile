SRC02 = $(shell find src/02_ood_ecs -name '*.cpp')
BIN02 = lib02.so
COMMSRC = $(shell find src/common -name '*.cpp')
CFLAGS = -g -std=c++11

all: tests

tgt02: common
	@tools/genheaders.sh
	c++ $(CFLAGS) -fPIC -I./include -I./include/02_ood_ecs -c $(SRC02)
	c++ -L./bin -lcommon -shared -o bin/$(BIN02) *.o
	@rm *.o

common:
	@mkdir -p bin
	c++ $(CFLAGS) -fPIC -I./include -c $(COMMSRC)
	c++ -shared -o bin/libcommon.so *.o
	@rm *.o

tests: tgt02
	c++ $(CFLAGS) -Wl,-rpath '-Wl,$$ORIGIN' -I./include -L./bin -lcommon -l02 tests/ood.cpp -o bin/ood

clean:
	rm -f bin/*
