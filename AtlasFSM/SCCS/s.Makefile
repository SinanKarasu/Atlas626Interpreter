h09142
s 00002/00001/00048
d D 1.6 01/01/29 16:34:55 sinan 7 6
c clean up
e
s 00007/00020/00042
d D 1.5 01/01/29 16:28:33 sinan 6 5
c check in before Mark xfer
e
s 00004/00000/00058
d D 1.4 98/07/09 08:31:54 sinan 5 4
c Long time no see... First cut at working EEC test. Getting Mark Stamper into the action.
e
s 00004/00004/00054
d D 1.3 97/09/17 11:30:13 frank 4 3
c misc
e
s 00003/00015/00055
d D 1.2 97/09/11 15:39:25 frank 3 1
c No more bodies in header files.
e
s 00000/00000/00000
d R 1.2 97/08/07 10:32:42 Codemgr 2 1
c SunPro Code Manager data about conflicts, renames, etc...
c Name history : 1 0 AtlasFSM/Makefile
e
s 00070/00000/00000
d D 1.1 97/08/07 10:32:41 sinan 1 0
c Initial check in of some State Machine stuff
e
u
U
f e 0
t
T
I 1
#$Id: Makefile,v 1.4 1993/06/09 16:26:50 rmartin Exp $
#------------------------------------------
# Makefile for Linux/Unix
#
D 3
PROGRAM		  = smc
E 3
I 3
PROGRAM       = smc
E 3
CPPFLAGS      = 
LIB_PATHS     = 
CFLAGS        = -g 
D 3
CCFLAGS        = -g 
E 3
I 3
D 6
CCFLAGS       = -g 
E 6
I 6
CCFLAGS       = -g
E 6
E 3
YACCFILE      = smy.y
YACCFILE.c    =	$(YACCFILE:.y=.c)
YACCFILE.h    =	$(YACCFILE:.y=.h)
LEXFILE       =	sml.l
LEXFILE.c     =	$(LEXFILE:.l=.c)

# **************************************************
# * Uncomment the following "LIBS" line if you get *
# * linker errors complaining about yywrap         *
# **************************************************
D 6
LIBS          = -ll
E 6
I 6
LIBS          = -ll 
E 6

SOURCES = sm.c $(LEXFILE.c) $(YACCFILE.c)
OBJECTS = $(SOURCES:.c=.o)
LINTFILES = $(SOURCES) $(YACCFILE.c) $(LEXFILE.c)

################################################################

.KEEP_STATE :
D 3
all : smc try
E 3
I 3
all : smc
E 3

$(PROGRAM) : $(OBJECTS)
D 6
	cc $(CFLAGS) $(LDFLAGS) -o smc $(OBJECTS) $(LIBS)
E 6
I 6
	CC $(CCFLAGS) $(LDFLAGS) -o smc $(OBJECTS) $(LIBS)
E 6

$(LEXFILE.c) : $(LEXFILE) $(YACCFILE.h)
	$(LEX.l) <$(LEXFILE) >$(LEXFILE.c)

$(YACCFILE.c) + $(YACCFILE.h) : $(YACCFILE)
	$(YACC.y) -d $(YACCFILE)
	mv y.tab.c $(YACCFILE.c)
	mv y.tab.h $(YACCFILE.h)

D 3

E 3
D 4
SensorWithEventMonitor.h ResourceSensorEM.h: ResourceSensorEM.sm ResourceContext.h
E 4
I 4
D 6
SensorWithEventMonitor.h ResourceSensorEM.h: ResourceSensorEM.sm ResourceContextBASE.h
E 4
	smc <ResourceSensorEM.sm
E 6

D 4
Sensor.h ResourceSensor.h: ResourceSensor.sm ResourceContext.h
E 4
I 4
D 6
Sensor.h ResourceSensor.h: ResourceSensor.sm ResourceContextBASE.h
E 4
	smc <ResourceSensor.sm

I 5

scrub:
E 6
I 6
clean scrub:
E 6
D 7
	-rm *.o smc
E 7
I 7
	-rm *.o smc sml.c smy.c smy.h
	-sccs clean
E 7

E 5
D 4
SourceLoadWithEventMonitor.h ResourceSourceLoadEM.h: ResourceSourceLoadEM.sm ResourceContext.h
E 4
I 4
D 6
SourceLoadWithEventMonitor.h ResourceSourceLoadEM.h: ResourceSourceLoadEM.sm ResourceContextBASE.h
E 4
	smc <ResourceSourceLoadEM.sm
E 6
I 6
.SUFFIXES: .c
E 6

D 4
SourceLoad.h ResourceSourceLoad.h: ResourceSourceLoad.sm ResourceContext.h
E 4
I 4
D 6
SourceLoad.h ResourceSourceLoad.h: ResourceSourceLoad.sm ResourceContextBASE.h
E 4
	smc <ResourceSourceLoad.sm

D 3

try.o :	 	SensorWithEventMonitor.o ResourceSensorEM.o \
		Sensor.o ResourceSensor.o \
		SourceLoadWithEventMonitor.o ResourceSourceLoadEM.o \
		SourceLoad.o ResourceSourceLoad.o 

E 3
SensorWithEventMonitor.o: SensorWithEventMonitor.h SensorWithEventMonitor.cc
Sensor.o: Sensor.h Sensor.cc

SourceLoadWithEventMonitor.o: SourceLoadWithEventMonitor.h SourceLoadWithEventMonitor.cc
SourceLoad.o: SourceLoad.h SourceLoad.cc
E 6
I 6
.c.o:
	CC -c $(CCFLAGS) -o $*.o $<
E 6
D 3

try : try.o 
	$(LINK.cc) -o $@  try.o ResourceSensorEM.o SensorWithEventMonitor.o \
	ResourceSensor.o Sensor.o ResourceSourceLoadEM.o SourceLoadWithEventMonitor.o \
	ResourceSourceLoad.o SourceLoad.o
E 3
E 1
