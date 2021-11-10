# Makefile for intro to C++ 

CPP=g++
LD=g++

CFLAGS = -std=c++11
LFLAGS =

RCXX=$(CFLAGS) $(ROOTCFLAGS)
RLXX=$(LFLAGS) $(ROOTLIBS)

SRC1= Data.o

%.o: %.cc %.hh
	$(CPP) $(RCXX) -c $<

all: run

run.o: run.cc Data.hh
	$(CPP) $(RCXX) -c $<

run: $(SRC1) run.o 
	$(LD) $(SRC1) run.o $(RLXX) -o run
	@echo '-> run created!'

clean:
		@rm -f *~
		@rm -f *.o 
		@rm -f *.x
		@rm -f run
