CCC = g++
CFLAGS = -I/usr/include/postgresql/ -L/usr/lib/postgresql/9.1/lib/ 
OPTIONS = -lpq
FILES = dcor_seq.cpp database.cpp csv.cpp algo.cpp 

dcor: $(FILES:.cpp=.o)
	$(CCC) $(CFLAGS) $(FILES:.cpp=.o) -o dcor $(OPTIONS)

database.o: database.cpp database.h csv.h
csv.o: csv.cpp csv.h
algo.o: algo.cpp algo.h csv.h
