# C Data Structures Lite - Makefile
#
#   make          build the library, the demos and the tests
#   make test     build everything, run every test suite, print a summary
#   make demos    build the demo programs only (run them: build/demos/demo_avl)
#   make clean    delete the build/ folder
#
# Works with GNU make + gcc/clang on Linux and macOS, and with
# mingw32-make + MinGW-w64 gcc on Windows.  Override the compiler with e.g.
#   make CC=clang

ifeq ($(origin CC),default)
  CC := gcc
endif
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -O2
CFLAGS += -Iinclude
AR     ?= ar

BUILD := build

ifeq ($(OS),Windows_NT)
  # Use the plain Windows shell so the Makefile does not depend on Unix tools.
  SHELL := cmd.exe
  EXE   := .exe
  fixpath = $(subst /,\,$1)
  MKDIR  = if not exist $(call fixpath,$1) mkdir $(call fixpath,$1)
  RMDIR  = if exist $(call fixpath,$1) rmdir /s /q $(call fixpath,$1)
else
  EXE   :=
  fixpath = $1
  MKDIR  = mkdir -p $1
  RMDIR  = rm -rf $1
endif

MODULES := vector slist stack

LIB_SRC := src/dsa_common.c $(foreach m,$(MODULES),src/$(m).c)
LIB_OBJ := $(patsubst src/%.c,$(BUILD)/obj/%.o,$(LIB_SRC))
LIB     := $(BUILD)/libdsa.a

DEMOS := $(foreach m,$(MODULES),$(BUILD)/demos/demo_$(m)$(EXE))
TESTS := $(foreach m,$(MODULES),$(BUILD)/tests/test_$(m)$(EXE))
RUNNER := $(BUILD)/tests/run_tests$(EXE)

.PHONY: all demos tests test clean

all: $(LIB) $(DEMOS) $(TESTS) $(RUNNER)

demos: $(DEMOS)

tests: $(TESTS) $(RUNNER)

# --- library -------------------------------------------------------------
$(LIB): $(LIB_OBJ)
	$(AR) rcs $@ $^

$(BUILD)/obj/%.o: src/%.c $(wildcard include/*.h) | $(BUILD)/obj
	$(CC) $(CFLAGS) -c $< -o $@

# --- demos and tests -----------------------------------------------------
$(BUILD)/demos/%$(EXE): demos/%.c $(LIB) | $(BUILD)/demos
	$(CC) $(CFLAGS) $< $(LIB) -o $@

$(BUILD)/tests/test_%$(EXE): tests/test_%.c tests/test_framework.h $(LIB) | $(BUILD)/tests
	$(CC) $(CFLAGS) -Itests $< $(LIB) -o $@

$(RUNNER): tests/run_tests.c | $(BUILD)/tests
	$(CC) $(CFLAGS) $< -o $@

# --- run the test suites --------------------------------------------------
test: $(TESTS) $(RUNNER)
	$(call fixpath,$(RUNNER)) $(TESTS)

# --- directories ----------------------------------------------------------
$(BUILD)/obj $(BUILD)/demos $(BUILD)/tests:
	$(call MKDIR,$@)

clean:
	$(call RMDIR,$(BUILD))

