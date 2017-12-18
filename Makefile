CC=g++
MPICC=mpicc
SIZE=STANDARD_DATASET
IDIR=../include
LINC=-I/usr/include
UTIL=util.c
CFLAGS=-std=c++11 -Wall -fopenmp -O3 -lm -lpthread
ODIR=obj
OTHERS=polybench
PAPI?=

dir:
	mkdir -p $(ODIR)

mm: dir
	$(CC) $(UTIL) $@.c -o $(ODIR)/$@ $(CFLAGS) $(PAPI)

mmmpi: dir
	$(MPICC) $(UTIL) $@.c -o $(ODIR)/$@ $(CFLAGS) $(PAPI)

rbsor: dir
	$(CC) $(UTIL) $@.c -o $(ODIR)/$@ $(CFLAGS) $(PAPI)

ludec: dir
	$(CC) $(UTIL) $@.c -o $(ODIR)/$@ $(CFLAGS) $(PAPI)

2mm: dir
	$(CC) $(UTIL) $@.c $(OTHERS).c -o $(ODIR)/$@ $(CFLAGS) $(PAPI)

mpi2mm: dir
	$(MPICC) $(UTIL) $@.c $(OTHERS).c -o $(ODIR)/$@ $(CFLAGS) $(PAPI)

mpi2mmgrid: dir
	$(MPICC) $(UTIL) $@.c $(OTHERS).c -o $(ODIR)/$@ $(CFLAGS) $(PAPI)

knn: dir
	$(CC) $@.cpp -o $(ODIR)/$@ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/* $(ODIR)/* *~ core $(INCDIR)/*~
