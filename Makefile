.DEFAULT_GOAL := all

CC = g++
OPT = -O3
# OPT = -g
WARN = -Wall
INC = -I inc
STD = -std=c++17
LIB =
CFLAGS = $(OPT) $(WARN) $(INC) $(LIB) $(STD)

WORKDIR := work
SRCDIR := src

MAIN_PREFIX   := main
STANDARD_MAIN := $(MAIN_PREFIX)
OPENMP_MAIN   := $(MAIN_PREFIX)_omp

MAIN_SRC := $(STANDARD_MAIN)
SRC := $(shell find src/ -type f -name "*.cpp" -not -name "$(MAIN_PREFIX)*.cpp") $(shell find src/ -type f -name "$(MAIN_SRC).cpp")

OBJ := $(subst $(SRCDIR),$(WORKDIR),$(patsubst %.cpp,%.o,$(SRC)))

BIN := sim

$(WORKDIR):
	mkdir -p $(WORKDIR)

$(WORKDIR)/%.o: $(SRCDIR)/%.cpp | $(WORKDIR)
	$(CC) $(CFLAGS) -c $< -o $@

all: $(OBJ)
	$(CC) -o $(BIN) $(CFLAGS) $^ -lm

omp:
	$(MAKE) LIB=-fopenmp MAIN_SRC=$(OPENMP_MAIN)

clean:
	rm -rf $(WORKDIR)
	rm -rf $(BIN)
