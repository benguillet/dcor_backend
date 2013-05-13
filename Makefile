CCC = g++
CFLAGS = -I/usr/include/postgresql/ -L/usr/lib/postgresql/9.1/lib/ 
OPTIONS = -lpq
FILES = dcor_seq.cpp database.cpp utils.cpp algo.cpp 

dcor: $(FILES:.cpp=.o)
	$(CCC) $(CFLAGS) $(FILES:.cpp=.o) -o dcor $(OPTIONS)

database.o: database.cpp database.h utils.h
utils.o: utils.cpp utils.h
algo.o: algo.cpp algo.h utils.h
