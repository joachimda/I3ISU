SOURCES=$(wildcard *.cpp)
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=main
CXX=g++

all: $(OBJECTS)
	$(CXX) -o $(EXECUTABLE) $(OBJECTS)

$(OBJECTS):
	$(CXX) -c $(SOURCES)

.PHONY: clean
clean:
	rm -f *.o 
	rm -f *.h.gch
	rm main

help:
	echo "Available targets are: all, clean and help"
