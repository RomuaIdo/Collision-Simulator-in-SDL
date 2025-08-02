CC = gcc

CFLAGS = -Wall -std=c99 -g

RELEASE_FLAGS = -O2

# Diretórios
SRCDIR = src
OBJDIR = obj
BINDIR = bin

LIBS = -lSDL2 -lSDL2_mixer -lSDL2_ttf -lm

SRCS = $(wildcard $(SRCDIR)/*.c)
OBJS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))

TARGET = $(BINDIR)/main

.PHONY: all build release debug run clean

all: build

build: $(TARGET)

release: CFLAGS += $(RELEASE_FLAGS)
release: build

debug: CFLAGS += -O0
debug: build

$(TARGET): $(OBJS)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

run: build
	./$(TARGET)

clean:
	rm -rf $(OBJDIR) $(BINDIR)
