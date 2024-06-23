.DEFAULT_GOAL := all

CC = g++
OPT = -O3
# OPT = -g
WARN = -Wall
INC = -I inc
STD = -std=c++17
CFLAGS = $(OPT) $(WARN) $(INC) $(LIB) $(STD)

WORKDIR := work
SRCDIR := src

SRC := $(shell find src/ -type f -name "*.cpp")

OBJ := $(subst $(SRCDIR),$(WORKDIR),$(patsubst %.cpp,%.o,$(SRC)))

BIN := sim

$(WORKDIR):
	mkdir -p $(WORKDIR)

$(WORKDIR)/%.o: $(SRCDIR)/%.cpp | $(WORKDIR)
	$(CC) $(CFLAGS) -c $< -o $@

all: $(OBJ)
	$(CC) -o $(BIN) $(CFLAGS) $^ -lm

clean:
	rm -rf $(WORKDIR)
	rm -rf $(BIN)
