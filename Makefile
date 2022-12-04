#####################################
#
# Partie 1
# 1.1 philosophes
# 1.2 prod-cons
# 1.3 read-write
# 1.4 perf.sh
# 1.5 perf.py
# 
# Partie 2
# 2.1 *-test-and-set
# 2.2 perf.sh
# 2.3 *-test-and-test-and-set
# 2.4 
#####################################

CC=gcc
CCFLAGS= -std=gnu99 -pthread

simple=philosophes read-write prod-conso
test-and-set=philosophes-test-and-set read-write-test-and-set prod-conso-test-and-set
test-and-test-and-set=philosophes-test-and-test-and-set read-write-test-and-test-and-set prod-conso-test-and-test-and-set
semaphores=philosophes-sem read-write-sem prod-conso-sem
files=$(simple) $(test-and-set) $(test-and-test-and-set) $(semaphores) perso

all : $(files)

# PHILOSOPHES
philosophes : philosophes.o 
	$(CC) $(CCFLAGS) -o philosophes philosophes.o 

philosophes.o: philosophes.c 
	$(CC) -DOPTIM=0 $(CCFLAGS) -c philosophes.c 

philosophes-test-and-set : philosophes-test-and-set.o
	$(CC) $(CCFLAGS) -o philosophes-test-and-set philosophes-test-and-set.o

philosophes-test-and-set.o: philosophes.c 
	$(CC) -DOPTIM=1 $(CCFLAGS) -c philosophes.c -o philosophes-test-and-set.o

philosophes-test-and-test-and-set : philosophes-test-and-test-and-set.o
	$(CC) $(CCFLAGS) -o philosophes-test-and-test-and-set philosophes-test-and-test-and-set.o

philosophes-test-and-test-and-set.o: philosophes.c 
	$(CC) -DOPTIM=2 $(CCFLAGS) -c philosophes.c -o philosophes-test-and-test-and-set.o

philosophes-sem : philosophes-sem.o
	$(CC) $(CCFLAGS) -o philosophes-sem philosophes-sem.o

philosophes-sem.o: philosophes.c 
	$(CC) -DOPTIM=3 $(CCFLAGS) -c philosophes.c -o philosophes-sem.o

# PROD-CONSO
prod-conso : prod-conso.o 
	$(CC) $(CCFLAGS) -o prod-conso prod-conso.o 

prod-conso.o: prod-conso.c 
	$(CC) -DOPTIM=0 $(CCFLAGS) -c prod-conso.c

prod-conso-test-and-set : prod-conso-test-and-set.o
	$(CC) $(CCFLAGS) -o prod-conso-test-and-set prod-conso-test-and-set.o

prod-conso-test-and-set.o: prod-conso.c 
	$(CC) -DOPTIM=1 $(CCFLAGS) -c prod-conso.c -o prod-conso-test-and-set.o

prod-conso-test-and-test-and-set : prod-conso-test-and-test-and-set.o
	$(CC) $(CCFLAGS) -o prod-conso-test-and-test-and-set prod-conso-test-and-test-and-set.o

prod-conso-test-and-test-and-set.o: prod-conso.c 
	$(CC) -DOPTIM=2 $(CCFLAGS) -c prod-conso.c -o prod-conso-test-and-test-and-set.o

prod-conso-test-sem : prod-conso-sem.o
	$(CC) $(CCFLAGS) -o prod-conso-sem prod-conso-sem.o

prod-conso-sem.o: prod-conso.c 
	$(CC) -DOPTIM=3 $(CCFLAGS) -c prod-conso.c -o prod-conso-sem.o

# READ_WRITE
read-write : read-write.o 
	$(CC) $(CCFLAGS) -o read-write read-write.o 

read-write.o: read-write.c 
	$(CC) -DOPTIM=0 $(CCFLAGS) -c read-write.c

read-write-test-and-set : read-write-test-and-set.o
	$(CC) $(CCFLAGS) -o read-write-test-and-set read-write-test-and-set.o

read-write-test-and-set.o: read-write.c 
	$(CC) -DOPTIM=1 $(CCFLAGS) -c read-write.c -o read-write-test-and-set.o

read-write-test-and-test-and-set : read-write-test-and-test-and-set.o
	$(CC) $(CCFLAGS) -o read-write-test-and-test-and-set read-write-test-and-test-and-set.o

read-write-test-and-test-and-set.o: read-write.c 
	$(CC) -DOPTIM=2 $(CCFLAGS) -c read-write.c -o read-write-test-and-test-and-set.o

read-write-sem : read-write-sem.o
	$(CC) $(CCFLAGS) -o read-write-sem read-write-sem.o

read-write-sem.o: read-write.c 
	$(CC) -DOPTIM=3 $(CCFLAGS) -c read-write.c -o read-write-sem.o

# PERSONAL DEV (DELETE ON SUBMITION)
perso: 
	cp /mnt/shared/Bac\ 1/Systèmes\ informatiques/Projet/syst-infoprojet1/* /home/juvanton/Documents
	clear

# CLEAN
clean : 
	rm -f $(files) *.o *.csv
	clear