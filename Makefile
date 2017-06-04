PART_OO_SRC = $(shell find src/part_oo/ -name '*.cpp')
PART_OO_BIN = libpart_oo.so

PART_DO_SRC = $(shell find src/part_do/ -name '*.cpp')
PART_DO_BIN = libpart_do.so

ECS_OO_SRC = $(shell find src/ecs_oo/ -name '*.cpp')
ECS_OO_BIN = libecs_oo.so

ECS_DO_SRC = $(shell find src/ecs_do/ -name '*.cpp')
ECS_DO_BIN = libecs_do

COMMSRC = $(shell find src/common/ -name '*.cpp')
CFLAGS = -g -std=c++11 #-Wfatal-errors

# Data-oriented ECS implementation stages
S0 =
S1 = -DDO_PARTITION_ARRAYS


all: tests

ecs_oo: common
	@tools/genheaders.sh
	@echo -e "\033[1mBuilding Object-Oriented ECS\033[0m"
	c++ $(CFLAGS) -fPIC -I./include -I./include/ecs_oo -c $(ECS_OO_SRC)
	c++ -L./bin -lcommon -shared -o bin/$(ECS_OO_BIN) *.o
	@rm *.o

ecs_do: common
	@tools/genheaders.sh
	@# Stage 0
	@echo -e "\033[1mBuilding Stage 0\033[0m"
	c++ $(CFLAGS) $(S0) -fPIC -I./include -I./include/ecs_do -c $(ECS_DO_SRC)
	c++ -L./bin -lcommon -shared -o bin/$(ECS_DO_BIN)0.so *.o
	@rm *.o
	@# Stage 1
	@echo -e "\033[1mBuilding Stage 1\033[0m"
	c++ $(CFLAGS) $(S1) -fPIC -I./include -I./include/ecs_do -c $(ECS_DO_SRC)
	c++ -L./bin -lcommon -shared -o bin/$(ECS_DO_BIN)1.so *.o
	@rm *.o

part_oo: common
	@tools/genheaders.sh
	@echo -e "\033[1mBuilding Object-Oriented Particle System\033[0m"
	c++ $(CFLAGS) -fPIC -I./include -I./include/part_oo -c $(PART_OO_SRC)
	c++ -L./bin -lcommon -shared -o bin/$(PART_OO_BIN) *.o
	@rm *.o

part_do: common
	@tools/genheaders.sh
	@echo -e "\033[1mBuilding Data-Oriented Particle System\033[0m"
	c++ $(CFLAGS) -fPIC -I./include -I./include/part_do -c $(PART_DO_SRC)
	c++ -L./bin -lcommon -shared -o bin/$(PART_DO_BIN) *.o
	@rm *.o

common:
	@mkdir -p bin
	c++ $(CFLAGS) -fPIC -I./include -c $(COMMSRC)
	c++ -shared -o bin/libcommon.so *.o
	@rm *.o

tests: ecs part
	@echo -e "\033[1mBuilding tests\033[0m"

ecs: test_ecs_oo test_ecs_do

test_ecs_oo: ecs_oo
	c++ $(CFLAGS) -Wl,-rpath '-Wl,$$ORIGIN' -I./include -L./bin -lcommon -lecs_oo tests/ecs_oo.cpp -o bin/oo

test_ecs_do: ecs_do
	@# Stage 0 -----------------------------------------------------------------
	c++ $(CFLAGS) $(S0) -Wl,-rpath '-Wl,$$ORIGIN' -I./include -L./bin -lcommon -lecs_do0 tests/ecs_do.cpp -o bin/do0
	@# Stage 1 -----------------------------------------------------------------
	c++ $(CFLAGS) $(S1) -Wl,-rpath '-Wl,$$ORIGIN' -I./include -L./bin -lcommon -lecs_do1 tests/ecs_do.cpp -o bin/do1

part: test_part_oo test_part_do

test_part_oo: part_oo
	c++ $(CFLAGS) -Wl,-rpath '-Wl,$$ORIGIN' -I./include -L./bin -lcommon -lpart_oo tests/part_oo.cpp -o bin/part_oo

test_part_do: part_do
	c++ $(CFLAGS) -Wl,-rpath '-Wl,$$ORIGIN' -I./include -L./bin -lcommon -lpart_do tests/part_do.cpp -o bin/part_do

clean:
	rm -rf bin/* *.o
