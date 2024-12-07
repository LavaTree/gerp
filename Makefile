###
### Makefile for Gerp Project
###
### Author:  Drew Galen, Pablo Herra

## Note, you do NOT need rules for
## FSTree.o and DirNode.o as those (pre-compiled) object files are 
## provided to you.

CXX      = clang++
CXXFLAGS = -g3 -Wall -Wextra -Wpedantic -Wshadow
LDFLAGS  = -g3 

# gerp rule - linking command
gerp: gerp.o WordHashTable.o main.o FSTree.o DirNode.o parser.o
	${CXX} ${LDFLAGS} -o gerp gerp.o WordHashTable.o main.o FSTree.o DirNode.o parser.o

# This rule builds main.o
main.o: main.cpp gerp.h
	$(CXX) $(CXXFLAGS) -c main.cpp

# This rule builds gerp.o
gerp.o: gerp.cpp gerp.h DirNode.h FSTree.h parser.h WordHashTable.h
	$(CXX) $(CXXFLAGS) -c gerp.cpp
# This rule builds process.o
processing.o: processing.h processing.cpp FSTree.h DirNode.h
	$(CXX) $(CXXFLAGS) -c processing.cpp

parser.o: parser.h parser.cpp 
	$(CXX) $(CXXFLAGS) -c parser.cpp

WordHashTable.o: WordHashTable.h WordHashTable.cpp 
	$(CXX) $(CXXFLAGS) -c WordHashTable.cpp

# The below rule will be used by unit_test.
unit_test: unit_test_driver.o gerp.o parser.o WordHashTable.o FSTree.o DirNode.o processing.o
	$(CXX) $(CXXFLAGS) $^

##
## Here is a special rule that removes all .o files besides the provided ones 
## (DirNode.o and FSTree.o), all temporary files (ending with ~), and 
## a.out produced by running unit_test. First, we find all .o files 
## that are not provided files, then we run rm -f on each found file 
## using the -exec find flag. Second, we delete the temporary files
## and a.out. @ is used to suppress stdout.
## 
## You do not need to modify or further comment this rule!
##
clean:
	@find . -type f \( \
		-name '*.o' ! -name 'FSTree.o' ! -name 'DirNode.o' \
		\) -exec rm -f {} \;
	@rm -f *~ a.out
