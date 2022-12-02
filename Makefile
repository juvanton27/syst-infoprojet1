CC=gcc
CCFLAGS= -std=gnu99 -pedantic -Wvla -Wall -Wno-unused-variable -pthread

files=philosophes philosophes-optim prod-conso prod-conso-optim read-write read-write-optim

all : $(files)

# PHILOSOPHES
philosophes : philosophes.o 
	$(CC) $(CCFLAGS) -o philosophes philosophes.o 

philosophes.o: philosophes.c 
	$(CC) -DOPTIM=0 $(CCFLAGS) -c philosophes.c 

philosophes-optim : philosophes-optim.o
	$(CC) $(CCFLAGS) -o philosophes-optim philosophes-optim.o

philosophes-optim.o: philosophes.c 
	$(CC) -DOPTIM=1 $(CCFLAGS) -c philosophes.c -o philosophes-optim.o

# PROD-CONSO
prod-conso : prod-conso.o 
	$(CC) $(CCFLAGS) -o prod-conso prod-conso.o 

prod-conso-optim : prod-conso-optim.o
	$(CC) $(CCFLAGS) -o prod-conso-optim prod-conso-optim.o

prod-conso.o: prod-conso.c 
	$(CC) -DOPTIM=0 $(CCFLAGS) -c prod-conso.c

prod-conso-optim.o: prod-conso.c 
	$(CC) -DOPTIM=1 $(CCFLAGS) -c prod-conso.c -o prod-conso-optim.o

# READ_WRITE
read-write : read-write.o 
	$(CC) $(CCFLAGS) -o read-write read-write.o 

read-write-optim : read-write-optim.o
	$(CC) $(CCFLAGS) -o read-write-optim read-write-optim.o

read-write.o: read-write.c 
	$(CC) -DOPTIM=0 $(CCFLAGS) -c read-write.c

read-write-optim.o: read-write.c 
	$(CC) -DOPTIM=1 $(CCFLAGS) -c read-write.c -o read-write-optim.o

# CLEAN
clean : 
	rm -f $(files) *.o *.csv
	clear