SOURCE_DIR   = ../future_net
INCLUDE_DIR  = $(SOURCE_DIR)/lib
SOURCE_FILES = $(SOURCE_DIR)/future_net.cpp \
               $(SOURCE_DIR)/io.cpp \
               $(SOURCE_DIR)/route.cpp \
               $(SOURCE_DIR)/route.h
PROGRAM      = future_net
TEST_DIR     = ../../test-case

# 编译器及选项
CC = g++
CFLAGES = -Wall -g
INCLUDE = -I$(INCLUDE_DIR)

.PHONY:	all clean run

all:
	mkdir -p bin
	$(CC) -o bin/$(PROGRAM) $(SOURCE_FILES) $(INCLUDE) $(CFLAGES)

clean:
	rm -rf bin

run:
	bin/$(PROGRAM) $(TEST_DIR)/case$(case)/topo.csv \
	               $(TEST_DIR)/case$(case)/demand.csv \
	               ./result.csv
	@echo "------The result is------"
	cat ./result.csv

help:
	@echo "make            ---- compile and link"
	@echo "make clean      ---- clean the bin dir"
	@echo "make run case=0 ---- run a test case"