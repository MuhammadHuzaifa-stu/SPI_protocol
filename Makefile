SYSTEMC_HOME = /opt/systemc

CXX = g++
CXXFLAGS = -I$(SYSTEMC_HOME)/include -Iinclude -Wall -Wno-deprecated
LDFLAGS = -L$(SYSTEMC_HOME)/lib-linux64 -lsystemc -lm  # change lib-linux64 to lib if needed

INC_DIR = include
SRC_DIR = src
VERIF_DIR = verif
BUILD_DIR = build

INC_FILES = $(INC_DIR)/spi_master.h
SRC_FILES = $(SRC_DIR)/spi_master.cpp
TB_FILES = $(VERIF_DIR)/tb_spi_master.cpp

TARGET = $(BUILD_DIR)/spi_master_tb

all: $(TARGET)

$(TARGET): $(INC_FILES) $(SRC_FILES) $(TB_FILES)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
