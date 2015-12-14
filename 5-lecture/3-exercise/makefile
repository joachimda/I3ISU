OBJECTS=$(patsubst %,$(BUILD_DIR)/%,$(SOURCES:.cpp=.o))
DEPS=$(patsubst %,$(BUILD_DIR)/%,$(SOURCES:.cpp=.d))
SOURCES=$(wildcard *.cpp)
#LIBS=-lncurses -pthread
LIBS= -pthread
CXXFLAGS=-I. -O2 -g
EXE=prog

# > make
ifeq (${ARCH},)
BUILD_DIR=build/host
BIN_DIR=bin/host
EXE=host_prog
CXX=g++
endif

# > make ARCH=target
ifeq (${ARCH},target)
BUILD_DIR=build/target
BIN_DIR=bin/target
CXX=arm-devkit-g++
EXE=target_prog
endif

$(BIN_DIR)/$(EXE): $(DEPS) $(OBJECTS) 
	mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS) $(LIBS)

$(BUILD_DIR)/%.o: %.cpp
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -rf build
	rm -rf bin

$(BUILD_DIR)/%.d: %.cpp
	mkdir -p $(BUILD_DIR)
	$(CXX) -MT $(@:.d=.o) -MM $(CXXFLAGS) $^ > $@

-include $(DEPS)
