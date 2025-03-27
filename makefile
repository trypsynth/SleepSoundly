CXX = g++
RC = windres
CXXFLAGS = -std=c++20 -Wall -Os -mwindows -ffunction-sections -fdata-sections
LDFLAGS = -lwinmm -mwindows -Wl,--gc-sections -s
TARGET = sleep_soundly.exe
SRC_CPP = $(wildcard src/*.cpp)
SRC_RC = $(wildcard src/*.rc)
OBJ = $(patsubst src/%,src/%.o,$(basename $(SRC_CPP) $(SRC_RC)))

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $^ $(LDFLAGS) -o $@

src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

src/%.o: src/%.rc
	$(RC) $< -O coff -o $@

clean:
	del /Q src\*.o $(TARGET)

.PHONY: clean all
