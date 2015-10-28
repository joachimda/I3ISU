SOURCES=*.cpp
OBJECTS=$(SOURCES:.cpp=.o)
DEPS=$(SOURCES:.cpp=.d)
EXECUTABLE=main
CXXFLAGS=-I
CXX=g++

$(EXECUTABLE): $(DEPS) $(OBJECTS)
	#$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS)
	$(CXX) -o $@ $(OBJECTS)
	make clean

$(OBJECTS): $(SOURCES)
	$(CXX) -c $(SOURCES)

%.d: %.cpp
	$(CXX) -MT$(@:.d=.o) -MM $(CXXFLAGS) $^ > $@

.PHONY: clean
clean: 
	rm -f *.d
	rm -f *.o 
	rm -f *.h.gch

help: 
	echo "Available targets are: all, clean and help"

-include $(DEPS)