CXX = g++
CXXFLAGS = -std=c++11 -Wall -MMD
EXEC = chip8-emulator
OBJECTS = cpu.o main.o
DEPENDS = ${OBJECTS:.o=.d}
INCLUDES = ../include
${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -I ${INCLUDES} -o ${EXEC}

-include ${DEPENDS}

.PHONY: clean

clean:
	rm ${OBJECTS} ${EXEC} ${DEPENDS}
