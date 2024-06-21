CC = g++
OPT = -O2
#OPT = -g
WARN = -Wall
INC = -I inc
STD = -std=c++11
CFLAGS = $(OPT) $(WARN) $(INC) $(LIB) $(STD)

WORKDIR := work
SRCDIR := src

SRC := $(shell find src/ -type f -name "*.cpp")

OBJ := $(subst $(SRCDIR),$(WORKDIR),$(patsubst %.cpp,%.o,$(SRC)))

$(WORKDIR):
	mkdir -p $(WORKDIR)

$(WORKDIR)/%.o: $(SRCDIR)/%.cpp | $(WORKDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$()

all: $(OBJ)
	$(CC) -o sim $(CFLAGS) $^ -lm

clean:
	rm -rf $(WORKDIR)