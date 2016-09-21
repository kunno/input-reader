# This is the Makefile to use for project reader
# To use, at the prompt, type:
#
# 	make <RET>		# This will make executable
# or
#	make new		# This will safely remove old stuff, and
#				# will then "make" again.
#
#	make directories	# makes correct directories
#	make install		# copies files to correct place
#	make clean		# cleans up directory
#	make public		# installs assignment into public area


all:	driver1 driver2

driver1:reader.o
	gcc -Wall -Wno-long-long -pedantic -o driver1 reader.o

driver2:reader.java 
	javac -g reader.java
	echo 'java reader $$*' > driver2
	chmod ug+rx driver2

reader.o:	reader.c
	gcc -Wall -Wno-long-long -pedantic -c -g reader.c

clean:
	rm -f *.o *.class driver1 driver2 core $(HOME)/core

new:
	make clean
	make

backup:
	cp *.[ch] *.java Makefile ./backup

tabs2spaces:
	make backup
	expand --tabs=4 reader.c > .reader_c$
	mv .reader_c$ reader.c
