
CC=g++
CFLAGS=-Wall -I. -g 
LFLAGS=
DEPENDFLAGS=-I. -M 
DEFINES=

OBJS=main.o simulation.o printSimPDF.o policy.o randWalk.o PIG.o

EXEFILE=ThesisSim

$(EXEFILE) : $(OBJS)
	$(CC) $(CFLAGS) -o $(EXEFILE) $(OBJS) $(LFLAGS)

# to run full memory check, type "make memcheck"
memtest: $(EXEFILE)
	valgrind --leak-check=full --show-leak-kinds=all ./$(EXEFILE)

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

realclean: clean
	rm -f .depend

# make depend will create a file ".depend" with all the dependencies
depend:
	rm -f .depend
	$(CC) $(DEFINES) $(DEPENDFLAGS) $(OBJS:.o=.cpp) > .depend

# if we have a .depend file, include it

ifeq (.depend,$(wildcard .depend))
include .depend
endif
