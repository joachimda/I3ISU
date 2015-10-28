SOURCES=*.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=main
CXX=g++

all: $(OBJECTS)
	$(CXX) -o $(EXECUTABLE) $(OBJECTS)
	make clean

$(OBJECTS): $(SOURCES)
	$(CXX) -c $(SOURCES)

.PHONY: clean
clean: 
	rm -f *.o 
	rm -f *.h.gch

help: 
	echo "Available targets are: all, clean and help"