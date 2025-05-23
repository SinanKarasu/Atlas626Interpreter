#
# PCCTS makefile for: atlas
#
# PCCTS release 1.32
# Project: t
# C++ output
# DLG scanner

# The following filenames must be consistent with ANTLR/DLG flags
PCCTS		= ./pccts
ANTLR_H		= $(PCCTS)/h
BIN		= $(PCCTS)/bin
ANTLR		= $(BIN)/antlr
DLG		= $(BIN)/dlg
SMC		= ./AtlasFSM/smc

TEMPLATES	= ./TemplateLibrary

CCC			= gcc

CFLAGS = -g -Wall -I. -I$(ANTLR_H)
###CFLAGS		= -g -mt
#CFLAGS		+= -xtarget=native -xarch=v9a
###CFLAGS		+= -xsb
#CFLAGS		+= -xarch=v9a -O3 -mt
#CFLAGS		+= -xprofile=collect
#CFLAGS		+= -O3
###CFLAGS		+= -I. -I$(ANTLR_H)
#CFLAGS		+= -I./WeissInclude -I./TemplateLibrary
###CFLAGS		+= -library=rwtools7,iostream
#CFLAGS		+= -features=rtti

LDFLAGS = -lpthread

CC		= $(CCC)
CCFLAGS		= $(CFLAGS) 

#######################Atlas stuff #################

AFLAGS_ATLAS	= -ge -gt -CC -ft AtlasTokens.h -fl AtlasDLG.dlg -k 4  -prc on -mrhoist on -rl 80000
DFLAGS_ATLAS	= -C2 -CC -ci -cl AtlasDLG
GRM_ATLAS	= tokens.g atlas1.g verbs.g atlas2.g expression.g comparison.g

SMC_SPAWN		= ResourceDataBus  ResourceEventMonitor \
				ResourceSensor  ResourceSourceLoad
				
SMC_SPAWN_CC			=	$(SMC_SPAWN:%=%.cc)
SMC_SPAWN_H			=	$(SMC_SPAWN:%=%.h)
SMC_SPAWN_O			=	$(SMC_SPAWN:%=%.o)


FNAMES	 =					\
		verbs				\
		atlas1				\
		atlas2				\
		expression			\
		comparison			\
		AtlasParser			\
		AtlasDLG			\
		tedl

#FNAMES	+=					\
#		$(SMC_SPAWN)

FNAMES	+=					\
		ArrayObject			\
		ASTVector			\
		StringVector			\
		DFSContext			\
		Visitors			\
		ActionAST			\
		AtlasAST			\
		AtlasDefinitions		\
		AtlasSupport			\
		AtlasToken			\
		BasicTypeAST			\
		BuiltinFunctionAST		\
		Dictionary			\
		LabelAST			\
		NounsModifiersDimensions	\
		MnemonicsDB			\
		OperatorAST			\
		Scope				\
		SignalActionAST			\
		SignalOperatorAST		\
		SignalTypeAST			\
		EventTypeAST			\
		SignalVerbAST			\
		DataBusVerbAST			\
		ResourceAST			\
		VirtualResourceAST		\
		VirtualDataBusAST		\
		ATEResourceAST			\
		ATEActionAST			\
		ATEFieldTypeAST			\
		GateConnEventAST		\
		astream				\
		InitList			\
		VerbAST				\
		atlasmain			\
		main				\
		Resource			\
		ResourceContextBASE		\
		AnalogResourceContext		\
		DataBusResourceContext		\
		Sensor				\
		SourceLoad			\
		EventMonitor			\
		DataBus				\
		DataBusTypeAST			\
		DataBusActionAST		\
		TimerObject			\
		Signal				\
		getToken			\
		NestedTokenStream		\
		ExecEnv				\
		AtlasBox

FNAMES	+=	$(ANTLR_H)/AParser		\
		$(ANTLR_H)/DLexerBase		\
		$(ANTLR_H)/ASTBase		\
		$(ANTLR_H)/PCCTSAST		\
		$(ANTLR_H)/ATokenBuffer

#FNAMES	+=	$(TEMPLATES)/Queue		\
#		$(TEMPLATES)/Pair		\
#		$(TEMPLATES)/Exception
#

FNAMES	+=	Queue		\
		Pair		\
		Exception


AFLAGS_TEDL	= -CC -gxt -ft AtlasTokens.h -gx -gt -w2 -k 2 -prc on -rl 80000

GRM_TEDL	= tokens.g tedl.g

FNAMES	+=					\
		TedlParser			\
		Graph				\
		Equivalence			\
		EdgeList			\
		Search				\
		SimulateCircuit			\
		Association			\
		SwitchModel			\
		BusNode				\
		Circuit				\
		CompositeDevice			\
		Contact				\
		ConfigurationModels		\
		AdaptationModels		\
		DeviceModels			\
		ConfigurationModel		\
		AdaptationModel			\
		TPSHardware			\
		DeviceModel			\
		Device				\
		DevicePath			\
		DevicePortNode			\
		TAGContext			\
		Edge				\
		ExceptionTypes			\
		Impedance			\
		InterfaceConnectorNode		\
		AdapterConnectorNode		\
		LoadDevicePortNode		\
		NodeName			\
		PathNode			\
		PathNodeList			\
		PointSourceNode			\
		DeviceReferencePortNode		\
		ResourceList			\
		SensorDevicePortNode		\
		Set				\
		SourceDevicePortNode		\
		UutConnectorNode		\
		SwitchContactNode		\
		VertexDictionary		\
		TedlDictionary			\
		Vertex				\
		VertexStack			\
		VertexList			\
		Wire				\
		TwoTerm				\
		ResistiveEdge			\
		CapacitiveEdge			\
		InductiveEdge			\
		GraphContext			\
		BFS				\
		NodeFunc			\
		EdgeFunc			\
		DebugEnv			\
					
		

FNAMES	+=	\
		TedlSupport		\
		tedlmain		\
		TedlSignalVerbAST	\
		TedlSignalVerbVisitor	\
		TedlDataBusTypeAST	\
		TedlExchangeVerbAST	\
		TedlExchangeVerbVisitor	\
		TedlDeviceAST		\
		TedlCompAST	\
		ReverseMap

FNAMES	+=	\
		CodeGenVisitor
  
OBJ	=	$(FNAMES:%=%.o)	

################### Generated stuff #############################

ANTLR_ATLAS_SPAWN	= verbs.cpp + atlas1.cpp + atlas2.cpp +	expression.cpp + comparison.cpp + \
			  AtlasParser.cpp + AtlasDLG.cpp + AtlasParser.h + AtlasTokens.h +	\
			  AtlasDLG.h + AtlasDLG.dlg

ANTLR_TEDL_SPAWN	= tedl.cpp + TedlParser.cpp + TedlParser.h
			



################### Dependencies ################################
	
atlas:	$(SMC_SPAWN_O)	$(OBJ)	
	$(CCC) $(CCFLAGS) -o atlas  $(SMC_SPAWN_O) $(OBJ)  -lrwtool -ldl  -lpthread -lposix4 

purify:	$(SMC_SPAWN_O)	$(OBJ)
	purify -best-effort -cache-dir=./.purify.cache  $(CCC) $(CCFLAGS) -o atlas $(SMC_SPAWN_O) $(OBJ) -lrwtool -ldl -lposix4 
	
##-lpthread -lposix4


################## Atlas stuff ##################################

$(ANTLR_ATLAS_SPAWN):	$(GRM_ATLAS) 
	$(ANTLR) $(AFLAGS_ATLAS) $(GRM_ATLAS)
	$(DLG) $(DFLAGS_ATLAS) AtlasDLG.dlg

################## Tedl stuff ###################################

$(ANTLR_TEDL_SPAWN):		$(GRM_TEDL)
	$(ANTLR) $(AFLAGS_TEDL) $(GRM_TEDL)

##	$(DLG) $(DFLAGS_TEDL) TedlDLG.dlg

################### *.Dependencies ##############################
##TedlStd.h:	TedlTokens.h

$(SMC_SPAWN_O):	$(SMC_SPAWN_CC)

#################################################################

scrub clean:
	-rm -f *.o core atlas $(ANTLR_ATLAS_SPAWN) $(DLG_ATLAS_SPAWN)
	-rm -f $(ANTLR_TEDL_SPAWN) $(DLG_TEDL_SPAWN)
	-rm -f $(SMC_SPAWN_CC) $(SMC_SPAWN_H)
	-rm -rf Templates.DB ir.out .make.state
	-rm -rf SunWS_cache
	-rm -rf ./.purify.cache

#depend:
#	makedepend  $(CFLAGS) $(SRC_DEPEND) -I$(ANTLR_H) -I/u2/k3000_opt/SUNWspro/include/CC \
#	-I/opt/SUNWspro/SC4.2/include/CC/rw7


.KEEP_STATE:

.SUFFIXES: .cpp .sm

.cpp.o:
	$(CCC) -c $(CFLAGS) -o $*.o $<

.cc.o:
	$(CCC) -c $(CFLAGS) -o $*.o $<

.sm.cc:
	$(SMC) < $<

#.sm.h:
#	$(SMC) < $<
	
#.sm.o:
#	$(SMC) < $<
#	$(CCC) -c $(CFLAGS) -o $*.o $< 
