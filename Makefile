# Fancy Makefile for rem_stubs
CC = gcc.exe
#CC = clang.exe
#CC = $(shell which $(CC) 2>/dev/null || echo gcc)
CFLAGS = -municode -Wall -Wextra -I.
DEBUG_CFLAGS = -g -O0
RELEASE_CFLAGS = -O2
LDFLAGS = -Wl,--subsystem,console -municode
LDLIBS =
SRC_DIR = .
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj
#BIN_DIR = bin
TARGET = $(BUILD_DIR)/rem_stubs.exe
SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
DEPS = $(OBJECTS:.o=.d)
INSTALL_DIR = $(HOME)/bin

# Default to debug build
BUILD_TYPE ?= debug
ifeq ($(BUILD_TYPE),release)
    CFLAGS += $(RELEASE_CFLAGS)
else
    CFLAGS += $(DEBUG_CFLAGS)
endif

# Verbose option
VERBOSE ?= 0
ifeq ($(VERBOSE),1)
    Q =
else
    Q = @
endif

all: $(TARGET)

$(TARGET): $(OBJECTS) | $(BUILD_DIR)
	$(Q)echo "Linking $@"
	$(Q)$(CC) $(LDFLAGS) -o $@ $(OBJECTS) $(LDLIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(Q)echo "Compiling $<"
	$(Q)$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

$(BUILD_DIR) $(OBJ_DIR):
	$(Q)mkdir $(subst /,\,$@)

install: $(TARGET)
	$(Q)echo "Installing $(TARGET) to $(INSTALL_DIR)"
	$(Q)if not exist $(subst /,\,$(INSTALL_DIR)) mkdir $(subst /,\,$(INSTALL_DIR))
	$(Q)copy $(subst /,\,$(TARGET)) $(subst /,\,$(INSTALL_DIR))

clean:
	$(Q)echo "Cleaning..."
	$(Q)if exist $(subst /,\,$(BUILD_DIR)) rmdir /S /Q $(subst /,\,$(BUILD_DIR))

distclean: clean
	$(Q)echo "Removing installed files..."
	$(Q)if exist $(subst /,\,$(INSTALL_DIR)/rem_stubs.exe) del /Q $(subst /,\,$(INSTALL_DIR)/rem_stubs.exe)

.PHONY: all install clean distclean
-include $(DEPS)