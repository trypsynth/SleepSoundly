CXX = g++
RC = windres
CXXFLAGS = -std=c++20 -Wall -Os -mwindows -ffunction-sections -fdata-sections
LDFLAGS = -lwinmm -mwindows -Wl,--gc-sections -s
TARGET = sleep_soundly.exe
SRCS = main.cpp
OBJS = $(SRCS:.cpp=.o)
RES = resource.rc
RES_OBJ = resource.o

all: $(TARGET)

$(RES_OBJ): $(RES)
	$(RC) $< -O coff -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET): $(OBJS) $(RES_OBJ)
	$(CXX) $(OBJS) $(RES_OBJ) $(LDFLAGS) -o $(TARGET)

clean:
	rm -f $(OBJS) $(RES_OBJ) $(TARGET)

.PHONY: clean all
