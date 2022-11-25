CC=gcc
CCFLAGS= -std=c11 -pedantic -Wvla -Wall -Wno-unused-variable -pthread 

all: philosophes philosophes-optim prod-conso prod-conso-optim read-write read-write-optim

# PHILOSOPHES
philosophes : philosophes.o 
	$(CC) $(CCFLAGS) -o philosophes philosophes.o 

philosophes-optim : philosophes-optim.o
	$(CC) $(CCFLAGS) -o philosophes-optim philosophes-optim.o

philosophes.o: philosophes.c 
	$(CC) -DOPTIM=0 $(CCFLAGS) -c philosophes.c 

philosophes-optim.o: philosophes.c 
	$(CC) -DOPTIM=1 $(CCFLAGS) -c philosophes.c 

# PROD-CONSO
prod-conso : prod-conso.o 
	$(CC) $(CCFLAGS) -o prod-conso prod-conso.o 

prod-conso-optim : prod-conso-optim.o
	$(CC) $(CCFLAGS) -o prod-conso-optim prod-conso-optim.o

prod-conso.o: prod-conso.c 
	$(CC) -DOPTIM=0 $(CCFLAGS) -c prod-conso.c

prod-conso-optim.o: prod-conso.c 
	$(CC) -DOPTIM=1 $(CCFLAGS) -c prod-conso.c

# READ_WRITE
read-write : read-write.o 
	$(CC) $(CCFLAGS) -o read-write read-write.o 

read-write-optim : read-write-optim.o
	$(CC) $(CCFLAGS) -o read-write-optim read-write-optim.o

read-write.o: read-write.c 
	$(CC) -DOPTIM=0 $(CCFLAGS) -c read-write.c

read-write-optim.o: read-write.c 
	$(CC) -DOPTIM=1 $(CCFLAGS) -c read-write.c

# CLEAN
clean : 
	rm -f philosophes prod-conso read-write *.o *.csv