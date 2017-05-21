ECS_OO_SRC = $(shell find src/ecs_oo/ -name '*.cpp')
ECS_OO_BIN = libecs_oo.so

ECS_DO_SRC = $(shell find src/ecs_do/ -name '*.cpp')
ECS_DO_BIN = libecs_do.so

COMMSRC = $(shell find src/common/ -name '*.cpp')
CFLAGS = -g -std=c++11

all: tests

ecs_oo: common
	@tools/genheaders.sh
	c++ $(CFLAGS) -fPIC -I./include -I./include/ecs_oo -c $(ECS_OO_SRC)
	c++ -L./bin -lcommon -shared -o bin/$(ECS_OO_BIN) *.o
	@rm *.o

ecs_do: common
	@tools/genheaders.sh
	c++ $(CFLAGS) -fPIC -I./include -I./include/ecs_do -c $(ECS_DO_SRC)
	c++ -L./bin -lcommon -shared -o bin/$(ECS_DO_BIN) *.o
	@rm *.o


common:
	@mkdir -p bin
	c++ $(CFLAGS) -fPIC -I./include -c $(COMMSRC)
	c++ -shared -o bin/libcommon.so *.o
	@rm *.o

tests: ecs_oo ecs_do
	c++ -g $(CFLAGS) -Wl,-rpath '-Wl,$$ORIGIN' -I./include -L./bin -lcommon -lecs_oo tests/ecs_oo.cpp -o bin/oo
	c++ -g $(CFLAGS) -Wl,-rpath '-Wl,$$ORIGIN' -I./include -L./bin -lcommon -lecs_do tests/ecs_do.cpp -o bin/do

clean:
	rm -f bin/*
