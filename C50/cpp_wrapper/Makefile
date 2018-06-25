.PHONY: clean

CC = gcc
CXX = g++

CFLAGS += -O3
CPPFLAGS += -O3

C_OBJ = array.o
CPP_OBJ = clist.o cmap.o file_tools.o tree_serializer.o

all: cpp_wrapper.a

cpp_wrapper.a: $(C_OBJ) $(CPP_OBJ)
	ar crs cpp_wrapper.a $(C_OBJ) $(CPP_OBJ)

array.o: array.c array.h
	$(CC) $(CFLAGS) -c array.c

clist.o: clist.cpp clist.h
	$(CXX) $(CPPFLAGS) -c clist.cpp

cmap.o: cmap.cpp cmap.h 
	$(CXX) $(CPPFLAGS) -c cmap.cpp

file_tools.o: file_tools.cpp file_tools.h
	$(CXX) $(CPPFLAGS) -c file_tools.cpp

tree_serializer.o: tree_serializer.h tree_serializer.cpp
	$(CXX) $(CPPFLAGS) -I .. -c tree_serializer.cpp

clean:
	-rm -rf *.o *.a
