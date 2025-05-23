#$Id: Makefile,v 1.4 1993/06/09 16:26:50 rmartin Exp $
#------------------------------------------
# Makefile for Linux/Unix
#
PROGRAM       = smc
CPPFLAGS      = 
LIB_PATHS     = 
CFLAGS        = -g 
CCFLAGS       = -g
YACCFILE      = smy.y
YACCFILE.c    =	$(YACCFILE:.y=.c)
YACCFILE.h    =	$(YACCFILE:.y=.h)
LEXFILE       =	sml.l
LEXFILE.c     =	$(LEXFILE:.l=.c)

# **************************************************
# * Uncomment the following "LIBS" line if you get *
# * linker errors complaining about yywrap         *
# **************************************************
LIBS          = -ll 

SOURCES = sm.c $(LEXFILE.c) $(YACCFILE.c)
OBJECTS = $(SOURCES:.c=.o)
LINTFILES = $(SOURCES) $(YACCFILE.c) $(LEXFILE.c)

################################################################

.KEEP_STATE :
all : smc

$(PROGRAM) : $(OBJECTS)
	CC $(CCFLAGS) $(LDFLAGS) -o smc $(OBJECTS) $(LIBS)

$(LEXFILE.c) : $(LEXFILE) $(YACCFILE.h)
	$(LEX.l) <$(LEXFILE) >$(LEXFILE.c)

$(YACCFILE.c) + $(YACCFILE.h) : $(YACCFILE)
	$(YACC.y) -d $(YACCFILE)
	mv y.tab.c $(YACCFILE.c)
	mv y.tab.h $(YACCFILE.h)


clean scrub:
	-rm *.o smc sml.c smy.c smy.h
	-sccs clean

.SUFFIXES: .c

.c.o:
	CC -c $(CCFLAGS) -o $*.o $<
