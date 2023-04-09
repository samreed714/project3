# TO DO: Must replace "username" by your username
USER= samreed

# Use GNU C++ compiler with C++11 standard
CC= g++
CFLAGS= -g -std=c++11

all: indexServer bibleindexclient PutCGI PutHTML

# Ref Object
Ref.o : Ref.h Ref.cpp
	$(CC) $(CFLAGS) -c Ref.cpp

# Verse Object
Verse.o : Ref.h Verse.h Verse.cpp
	$(CC) $(CFLAGS) -c Verse.cpp

# Bible Object
Bible.o : Ref.h Verse.h Bible.h Bible.cpp
	$(CC) $(CFLAGS) -c Bible.cpp

# FIFO Object
fifo.o : fifo.h fifo.cpp
	$(CC) $(CFLAGS) -c fifo.cpp

# Server source
indexServer.o : Ref.h Verse.h Bible.h fifo.h Server.cpp
	$(CC) $(CFLAGS) -c Server.cpp -o indexServer.o

# Build the server executable
indexServer: Ref.o Verse.o Bible.o fifo.o indexServer.o
	$(CC) $(CFLAGS) -o indexServer Ref.o Verse.o Bible.o fifo.o indexServer.o

# Client source
bibleindexclient.o : Ref.h Verse.h Bible.h fifo.h logfile.h bibleindexclient.cpp
	$(CC) $(CFLAGS) -c bibleindexclient.cpp -o bibleindexclient.o

# Build the client executable
bibleindexclient: Ref.o Verse.o Bible.o fifo.o bibleindexclient.o
	$(CC) $(CFLAGS) -o bibleindexclient Ref.o Verse.o Bible.o fifo.o bibleindexclient.o -L/usr/local/lib -lcgicc
	
PutCGI: bibleindexclient
		chmod 757 bibleindexclient
		cp bibleindexclient /var/www/html/class/csc3004/$(USER)/cgi-bin
		echo "Current contents of your cgi-bin directory: "
		ls -l /var/www/html/class/csc3004/$(USER)/cgi-bin/	
	
PutHTML: bibleindex.html
		cp bibleindex.html  /var/www/html/class/csc3004/$(USER)
		
		echo "Current contents of your HTML directory: "
		ls -l  /var/www/html/class/csc3004/$(USER)

# "make clean" will clean up by removing intermediate files
clean:
	rm -f *.o indexServer bibleindexclient
