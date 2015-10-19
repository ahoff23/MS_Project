CC=g++
CFLAGS = -c -std=c++11 -pg -I.
LDFLAGS=-pg
SOURCES=main.cpp Agent.cpp AStarNode.cpp AStarNodeList.cpp \
	AStarNodePointer.cpp CantorPair.cpp CBSNode.cpp CBSTree.cpp \
	Coordinates.cpp Exceptions.cpp PathClearAStar.cpp Tests.cpp World.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=main

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

