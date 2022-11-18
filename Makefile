CC=gcc
CCFLAGS= -std=c11 -pedantic -Wvla -Wall -Wno-unused-variable -pthread 

all: philosophes prod-conso

philosophes : philosophes.o 
	$(CC) $(CCFLAGS) -o philosophes philosophes.o 

prod-conso : prod-conso.o 
	$(CC) $(CCFLAGS) -o prod-conso prod-conso.o 

read-write : read-write.o 
	$(CC) $(CCFLAGS) -o read-write read-write.o 

philosophes.o: philosophes.c 
	$(CC) $(CCFLAGS) -c philosophes.c 

prod-conso.o: prod-conso.c 
	$(CC) $(CCFLAGS) -c prod-conso.c

read-write.o: read-write.c 
	$(CC) $(CCFLAGS) -c read-write.c

clean : 
	rm -f $(all)
	clear