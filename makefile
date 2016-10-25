shell: shell.o parse.o process.o cd.o run.o plist.o
		gcc -o shell shell.o parse.o process.o cd.o run.o plist.o

shell.o: shell.c parse.h process.h
		gcc -c shell.c parse.h process.h

parse.o: parse.c struct.h
		gcc -c parse.c struct.h

process.o: struct.h cd.h run.h plist.h
		gcc -c process.c cd.h run.h plist.h struct.h

run.o: struct.h plist.h
	  gcc -c run.c struct.h plist.h

cd.o: cd.c
		gcc -c cd.c

plist.o : plist.c
		gcc -c plist.c

.PHONY : clean
clean :
		rm -f *.gch
		rm -f *.o
