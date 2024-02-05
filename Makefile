
CC=g++
CFLAGS=-Wall -I. -g 
LFLAGS=
DEPENDFLAGS=-I. -M 
DEFINES=

OBJS=main.o simulation.o

EXEFILE=ThesisSim

$(EXEFILE) : $(OBJS)
	$(CC) $(CFLAGS) -o $(EXEFILE) $(OBJS) $(LFLAGS)

# scan.yy.o: scan.yy.c 
# 	$(CC) $(DEFINES) $(CFLAGS) -c $< 

# scan.yy.c: scan.lex parse.tab.h
# 	flex -o scan.yy.c scan.lex

# parse.tab.o: parse.tab.c
# 	$(CC) $(CFLAGS) -c $<

# parse.tab.h: parse.yy
# 	bison -d -o parse.tab.c parse.yy

# parse.tab.c: parse.yy
# 	bison -d -o parse.tab.c parse.yy

.cpp.o:	
	$(CC) $(DEFINES) $(CFLAGS) -c $<

clean:
	rm -f core* *.o $(EXEFILE) *~

# make depend will create a file ".depend" with all the dependencies
depend:
	rm -f .depend
	$(CC) $(DEFINES) $(DEPENDFLAGS) $(OBJS:.o=.cpp) > .depend

# if we have a .depend file, include it

ifeq (.depend,$(wildcard .depend))
include .depend
endif
