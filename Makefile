ifndef GBDK_HOME
	GBDK_HOME = ~/gbdk/
endif

# Detect the host OS. On Windows we run the build through Git Bash so that the
# Unix-style recipes below (mkdir -p, touch, rm -f) behave as written, and so
# that paths containing spaces are handled correctly.
ifeq ($(OS),Windows_NT)
	WINDOWS = 1
	SHELL = sh.exe
	ifeq ($(wildcard C:/Program Files/Git/usr/bin/sh.exe),C:/Program Files/Git/usr/bin/sh.exe)
		SHELL = C:/Program Files/Git/usr/bin/sh.exe
	else ifeq ($(wildcard C:/Program Files (x86)/Git/usr/bin/sh.exe),C:/Program Files (x86)/Git/usr/bin/sh.exe)
		SHELL = C:/Program Files (x86)/Git/usr/bin/sh.exe
	else ifeq ($(wildcard $(LOCALAPPDATA)/Programs/Git/usr/bin/sh.exe),$(LOCALAPPDATA)/Programs/Git/usr/bin/sh.exe)
		SHELL = $(LOCALAPPDATA)/Programs/Git/usr/bin/sh.exe
	endif
else
	WINDOWS = 0
	SHELL = /bin/sh
endif

PROJECTNAME = LabyrinthOfTheDragon

SRC_DIR = src
DATA_DIR = data
OBJ_DIR = obj
RES_DIR = res

ROM_BANKS=32
RAM_BANKS=4
CART_TYPE=0x1B

LCC = $(GBDK_HOME)/bin/lcc
LCCFLAGS = -Wm-yC -Wm-yt$(CART_TYPE) -Wl-yo$(ROM_BANKS) -Wl-ya$(RAM_BANKS)

# Invoke the Node tools through node explicitly. On Windows, Make cannot execute
# the extensionless shebang scripts directly (the path contains spaces and there
# is no file association), so we run the interpreter ourselves.
NODE = node
PNG2BIN = $(NODE) ./tools/png2bin
TABLES2C = $(NODE) ./tools/tables2c
STRINGS2C = $(NODE) ./tools/strings2c

# GBDK_DEBUG = ON
ifdef GBDK_DEBUG
	LCCFLAGS += -debug -v
endif

BIN = $(PROJECTNAME).gbc
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
DATA_FILES = $(wildcard $(DATA_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES)) \
	$(patsubst $(DATA_DIR)/%.c,$(OBJ_DIR)/%.o,$(DATA_FILES))
MAP_FILES = $(wildcard $(RES_DIR)/maps/*.tilemap)
TILEMAP_FILES = $(wildcard $(RES_DIR)/tilemaps/*.tilemap)
TILEPNG := $(wildcard assets/tiles/*.png)
TILEBIN := $(subst assets/,res/,$(patsubst %.png,%.bin,$(TILEPNG)))

.PHONY: all clean usage assets data strings tables asset_dirs regenerated

# The generated sources (strings_*.bank*.c, tables.c) define symbols referenced
# by the hand-written sources, and they do not exist on a clean checkout. The
# $(wildcard) expansion for DATA_FILES happens at parse time, before generation,
# so 'all' first produces the generated sources and then re-invokes make with
# DATA_FILES refreshed.
ifeq ($(DID_CODEGEN),)
all: assets data
	@"$(MAKE)" DID_CODEGEN=1 $(BIN)
else
all: $(BIN)
endif

assets: asset_dirs strings tables $(TILEBIN)

asset_dirs:
	mkdir -p res/tiles

# Code generation for strings and tables must finish before any object file is
# compiled, otherwise the generated strings_*.c/tables.c sources may not exist
# yet. These are declared as order-only prerequisites below.
# Tables are generated from assets/schema.conf plus the data/*.csv files it
# names. Depending on the schema and every data file means an edit to any of
# them triggers a rebuild.
TABLES_INPUTS = assets/schema.conf $(wildcard data/*.csv)

tables: $(TABLES_INPUTS)
	$(TABLES2C)

strings: assets/strings.js
	$(STRINGS2C)

res/tiles/%.bin: assets/tiles/%.png
	$(PNG2BIN) $< $@

$(BIN): $(OBJ_FILES)
	$(LCC) $(LCCFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR) strings tables
	$(LCC) $(LCCFLAGS) -c $< -o $@

data:
	touch $(DATA_DIR)/*.c

$(OBJ_DIR)/%.o: $(DATA_DIR)/%.c | $(OBJ_DIR) strings tables
	$(LCC) $(LCCFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

usage:
	$(GBDK_HOME)/bin/romusage $(BIN)

clean:
	rm -f *.o *.lst *.map *.gb *.gbc *.ihx *.sym *.cdb *.adb *.asm *.noi *.rst
	rm -f res/tiles/*.bin res/tiles/manifest.json
	rm -f res/color_tables/*.bin res/color_tables/manifest.json
	rm -f data/strings_*bank*.c
	rm -f obj/*
	rm -f src/strings.h
	rm -f src/tables.c
