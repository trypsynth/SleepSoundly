CXX = g++
RC = windres
CXXFLAGS = -std=c++20 -Wall -Os -mwindows -ffunction-sections -fdata-sections
LDFLAGS = -lwinmm -mwindows -Wl,--gc-sections -s
TARGET = sleep_soundly.exe

all: $(TARGET)

$(TARGET): main.o resource.o
	$(CXX) $^ $(LDFLAGS) -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

resource.o: resource.rc
	$(RC) $< -O coff -o $@

clean:
	rm -f *.o $(TARGET)

.PHONY: clean all
