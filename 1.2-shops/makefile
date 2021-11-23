INPUTS  = 25
OBJS	= solver.o
SOURCE	= solver.cpp
HEADER	=
OUT	= solver
CC	= g++
FLAGS	= -O2 -c -Wall
LFLAGS	=



all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

solver.o: solver.cpp
	$(CC) $(FLAGS) solver.cpp

valgrind: $(OUT)
	valgrind ./$(OUT) < $(FILE)

valgrind_leakcheck: $(OUT)
	valgrind --leak-check=full ./$(OUT) < $(FILE)

valgrind_extreme: $(OUT)
	valgrind --leak-check=full --show-leak-kinds=all --leak-resolution=high --track-origins=yes --vgdb=yes ./$(OUT) < $(FILE)

bench: $(OUT)
	hyperfine --export-json bench.json -m 20 --parameter-scan testcase 1 $(INPUTS) "./$(OUT) <  inputs/input{testcase}.txt"

test: $(OUT)
	python ~/Desktop/projects/algo_help/test_algo/functional_test.py

clean:
	rm -f $(OBJS) $(OUT) bench.*

