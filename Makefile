CC = g++
CFLAGS = -c -Wall -pthread
LDFLAGS = -pthread
BIN_DIR = bin
SRC_DIR = src
INCLUDE_DIR = include

# Detect OS for shell commands
ifeq ($(OS),Windows_NT)
	SHELL := cmd
	MKDIR = if not exist $(BIN_DIR) mkdir $(BIN_DIR)
	RM = del /Q /F
	RMDIR = rmdir /S /Q
	SEP = \
	EXE = .exe
	RUN = $(BIN_DIR)$(SEP)supervisor$(EXE)
else
	MKDIR = mkdir -p $(BIN_DIR)
	RM = rm -f
	RMDIR = rm -rf
	SEP = /
	EXE =
	RUN = ./$(BIN_DIR)/supervisor
endif

# All executables
EXECUTABLES = $(BIN_DIR)/supervisor \
              $(BIN_DIR)/lexical_analyzer \
              $(BIN_DIR)/syntax_analyzer \
              $(BIN_DIR)/semantic_analyzer \
              $(BIN_DIR)/code_generator

# Default target
all: $(EXECUTABLES)

# Supervisor executable
$(BIN_DIR)$(SEP)supervisor$(EXE): $(SRC_DIR)/supervisor.cpp $(SRC_DIR)/common.cpp $(SRC_DIR)/shared_memory.cpp $(SRC_DIR)/message_queue.cpp $(SRC_DIR)/scheduler.cpp
	@$(MKDIR)
	$(CC) $(LDFLAGS) -o $@ $^

# Lexical Analyzer executable
$(BIN_DIR)$(SEP)lexical_analyzer$(EXE): $(SRC_DIR)/lexical_analyzer.cpp $(SRC_DIR)/common.cpp $(SRC_DIR)/shared_memory.cpp $(SRC_DIR)/message_queue.cpp
	@$(MKDIR)
	$(CC) $(LDFLAGS) -o $@ $^

# Syntax Analyzer executable
$(BIN_DIR)$(SEP)syntax_analyzer$(EXE): $(SRC_DIR)/syntax_analyzer.cpp $(SRC_DIR)/common.cpp $(SRC_DIR)/shared_memory.cpp $(SRC_DIR)/message_queue.cpp
	@$(MKDIR)
	$(CC) $(LDFLAGS) -o $@ $^

# Semantic Analyzer executable
$(BIN_DIR)$(SEP)semantic_analyzer$(EXE): $(SRC_DIR)/semantic_analyzer.cpp $(SRC_DIR)/common.cpp $(SRC_DIR)/shared_memory.cpp $(SRC_DIR)/message_queue.cpp
	@$(MKDIR)
	$(CC) $(LDFLAGS) -o $@ $^

# Code Generator executable
$(BIN_DIR)$(SEP)code_generator$(EXE): $(SRC_DIR)/code_generator.cpp $(SRC_DIR)/common.cpp $(SRC_DIR)/shared_memory.cpp $(SRC_DIR)/message_queue.cpp
	@$(MKDIR)
	$(CC) $(LDFLAGS) -o $@ $^

# Clean build artifacts
clean:
ifeq ($(OS),Windows_NT)
	-$(RM) "$(BIN_DIR)\supervisor.exe" 2>nul
	-$(RM) "$(BIN_DIR)\lexical_analyzer.exe" 2>nul
	-$(RM) "$(BIN_DIR)\syntax_analyzer.exe" 2>nul
	-$(RM) "$(BIN_DIR)\semantic_analyzer.exe" 2>nul
	-$(RM) "$(BIN_DIR)\code_generator.exe" 2>nul
	-$(RM) logs\*.log 2>nul
	-$(RM) logs\*.txt 2>nul
	-$(RM) data\output\* 2>nul
else
	-$(RM) $(EXECUTABLES)
	-$(RM) logs/*.log logs/*.txt
	-$(RM) data/output/*
endif

# Run the simulator
run: all
	$(RUN)

# Run with verbose output
run-verbose: all
ifeq ($(OS),Windows_NT)
	$(RUN) > simulation_output.log 2>&1 & type simulation_output.log
else
	$(RUN) 2>&1 | tee simulation_output.log
endif

# Help
help:
	@echo "Available targets:"
	@echo "  make all          - Build all executables"
	@echo "  make run          - Build and run the simulator"
	@echo "  make run-verbose  - Build and run with verbose output"
	@echo "  make clean        - Remove all build artifacts"
	@echo "  make help         - Show this help message"

.PHONY: all clean run run-verbose help
