LIB = CoreLib
VPATH = $(LIB)
CXX = g++
CXXFLAGS = -g -Wno-write-strings
EXECUTABLE = convert-EBCDIC-CSV

all: clean compileAll link

link:
	$(CXX) -o $(EXECUTABLE) $(shell echo *.o)

compileAll: $(patsubst %.cpp,%.o,$(shell ls -R | grep cpp))

compile:
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -rf *.o; rm -f $(EXECUTABLE); cd $(LIB); rm -rf *.o
	
