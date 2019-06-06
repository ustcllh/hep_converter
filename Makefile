CXX=g++
CXXFLAGS=-g -std=c++11 -Wall -pedantic

SRC=src

ROOTFLAG=`root-config --cflags --libs`

all: hepmc_to_root root_to_pu14

hepmc_to_root: $(SRC)/hepmc_to_root.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(ROOTFLAG)

root_to_pu14: $(SRC)/root_to_pu14.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(ROOTFLAG)

.PHONY:
	clean
	all

clean:
	rm -f hepmc_to_root
	rm -f root_to_pu14
