SRC02 = $(shell find src/ecs_oo/ -name '*.cpp')
BIN02 = libecs_oo.so
COMMSRC = $(shell find src/common/ -name '*.cpp')
CFLAGS = -g -std=c++11

all: tests

ecs_oo: common
	@tools/genheaders.sh
	c++ $(CFLAGS) -fPIC -I./include -I./include/ecs_oo -c $(SRC02)
	c++ -L./bin -lcommon -shared -o bin/$(BIN02) *.o
	@rm *.o

common:
	@mkdir -p bin
	c++ $(CFLAGS) -fPIC -I./include -c $(COMMSRC)
	c++ -shared -o bin/libcommon.so *.o
	@rm *.o

tests: ecs_oo
	c++ $(CFLAGS) -Wl,-rpath '-Wl,$$ORIGIN' -I./include -L./bin -lcommon -lecs_oo tests/ecs_oo.cpp -o bin/ecs_oo

clean:
	rm -f bin/*
