SYSTEMC_HOME = /opt/systemc

CXX = g++
CXXFLAGS = -I$(SYSTEMC_HOME)/include -Iinclude -Wall -Wno-deprecated
LDFLAGS = -L$(SYSTEMC_HOME)/lib-linux64 -lsystemc -lm

INC_DIR = include
SRC_DIR = src
VERIF_DIR = verif
BUILD_DIR = build

# Default test if none specified
TEST ?= master

# Source files
INC_FILES = $(INC_DIR)/spi_master.h $(INC_DIR)/spi_slave.h
SRC_FILES = $(SRC_DIR)/spi_master.cpp $(SRC_DIR)/spi_slave.cpp

# Testbench selection
ifeq ($(TEST),master)
    TB_SRC = $(VERIF_DIR)/tb_spi_master.cpp
    TARGET = $(BUILD_DIR)/spi_master_tb
else ifeq ($(TEST),slave)
    TB_SRC = $(VERIF_DIR)/tb_spi_slave.cpp
    TARGET = $(BUILD_DIR)/spi_slave_tb
else
    $(error Invalid TEST specified. Use 'master' or 'slave')
endif

all: $(TARGET)

$(TARGET): $(INC_FILES) $(SRC_FILES) $(TB_SRC)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)
	@echo "Built $(TEST) testbench: $@"

run: $(TARGET)
	@echo "Running $(TEST) testbench..."
	./$(TARGET)

clean:
	rm -f $(BUILD_DIR)/spi_master_tb $(BUILD_DIR)/spi_slave_tb

.PHONY: all run clean