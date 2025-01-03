CLANG=clang++

SRC=./tests

PHY_HEADER=$(SRC)/phy.hpp
TEST_SOURCE=$(SRC)/test-phy.cpp
OUTPUT_EXE=$(SRC)/test-phy.exe

PORT=COM3
FQBN=arduino:avr:uno
SKETCH_PATH=sketch

all: compile upload

test:
	echo compiling
	$(CLANG) $(TEST_SOURCE) -o $(OUTPUT_EXE)
	echo testing
	$(OUTPUT_EXE)

init:
	echo initializing
	arduino-cli core install arduino:avr
	arduino-cli lib install "Adafruit SSD1306"

compile:
	echo compiling
	arduino-cli compile --fqbn $(FQBN) $(SKETCH_PATH)

upload:
	echo "upload to Port->$(PORT)..."
	arduino-cli upload -p $(PORT) --fqbn $(FQBN) $(SKETCH_PATH)
 