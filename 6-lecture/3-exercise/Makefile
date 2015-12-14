SRC=main.cpp
OBJ=${SRC:%.cpp=%.o}
DEP=${SRC:%.cpp=%.d}
EXE=test

# CHANGE THIS LINE!
BASEPATH=/home/stud/github/i3isu-e2015_blackboardsux/6-lecture/OSApiStudent/

ifeq (${TARGET},host)
GXX = g++
endif

ifeq (${TARGET},target)
GXX = arm-devkit-g++
endif

# Determine whether this is a debug build or not
ifdef DEBUG
CFLAGS=-ggdb -O0
LIBPATH=$(BASEPATH)/lib/${TARGET}/debug
else
CFLAGS=-O2
LIBPATH=$(BASEPATH)/lib/${TARGET}/release
endif

# Setup the CFLAGS to ensure that the relevant warnings, includes and defines.
CFLAGS+=-Wall -D_REENTRANT -DOS_LINUX -I$(BASEPATH)/inc

# Then again, note how the flags are NOT part of the linking process
${EXE}: ${OBJ} ${DEP}
	@${GXX} -o $@ $(OBJ) -L$(LIBPATH) -lOSApi -lrt -lpthread

%.d: %.cpp
	@echo "Making dependencies for $<..."
	@${GXX} -MT$(@:.d=.o) -MM $(CFLAGS) $^ > $@

%.o: %.cpp
	@echo "Making object files for $<..."
	@${GXX} $(CFLAGS) -c $< -o $@

all: ${EXE}

clean:
	@echo "Cleaning up..."
	@rm -f *.d *.o ${EXE}
	@rm -f -r build
	
-include ${DEP}
