# *******************************************************************************
# * 070206B I.U.Jayawardena udeshike@gmail.com
# * makefile
# ******************************************************************************
all: mysearch

# Which compiler
CC = gcc

# Where to install
INSTDIR = /home/ishan/bin

# Where are include files are kept
INCLUDE = .

# Options for development
CFLAGS = -g -Wall -ansi

#Options for releases
#CFLAGS = -O -Wall -ansi

objects = search.o read.o listfiles.o bitvec.o readquery.o list.o error.o
makeobjs = $(CC) -I$(INCLUDE) $(CFLAGS) -c

mysearch: $(objects) 
	$(CC) -o mysearch $(objects) -lm
	
search.o:	search.c engine.h
	$(makeobjs) search.c
	
read.o:	read.c engine.h
	$(makeobjs) read.c
	
listfiles.o:	listfiles.c
	$(makeobjs) listfiles.c

bitvec.o: bitvec.c bitvec.h
	$(makeobjs) bitvec.c

readquery.o: readquery.c fsm.h
	$(makeobjs) readquery.c
	
list.o: list.c list.h 
	$(makeobjs) list.c
	
error.o: error.c common.h
	$(makeobjs) error.c	
clean:
	rm -f *.o core *~
cleanall:	clean
	rm -f mysearch
	
install: mysearch
	@if [ -d $(INSTDIR) ];\
		then \
		cp mysearch $(INSTDIR);\
		chmod a+x $(INSTDIR)/mysearch;\
		chmod og-w $(INSTDIR)/mysearch;\
		echo "Installed in $(INSTDIR)";\
	else \
		echo "Sorry, $(INSTDIR) does not exist";\
	fi
