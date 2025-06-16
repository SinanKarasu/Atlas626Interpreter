#
# PCCTS makefile for: atlas
#
# PCCTS release 1.33
# Project: t
# C++ output
# DLG scanner

# The following filenames must be consistent with ANTLR/DLG flags
PCCTS		= ../../DevTools/pccts
ANTLR_H		= $(PCCTS)/h
BIN		= $(PCCTS)/bin
ANTLR		= $(BIN)/antlr
DLG		= $(BIN)/dlg
SMC		= ../../DevTools/smc/smc

GRAMMAR	= ./Grammar
GENERATED	= ./Generated


CCC			= clang++

CFLAGS = -g -Wall -I. -I$(ANTLR_H) -I$(GENERATED)

LDFLAGS = -lpthread

CC		= $(CCC)
CCFLAGS		=  -std=c++20 -Wall -g $(CFLAGS)

#######################Atlas stuff #################

AFLAGS_ATLAS	= -ge -gt -CC -ft AtlasTokens.h -fl AtlasDLG.dlg -k 4  -prc on -mrhoist on -rl 80000 -o $(GENERATED)
DFLAGS_ATLAS	= -C2 -CC -ci -cl AtlasDLG -o $(GENERATED)
GRM_ATLAS	= $(addprefix $(GRAMMAR)/, \
		tokens.g atlas1.g verbs.g atlas2.g expression.g comparison.g \
		)

SMC_SPAWN		= \
				ResourceDataBus  ResourceEventMonitor \
				ResourceSensor  ResourceSourceLoad
				
				
# SMC_SPAWN_CC		=	$(SMC_SPAWN:%=%.cc)
# SMC_SPAWN_H			=	$(SMC_SPAWN:%=%.h)
# SMC_SPAWN_O			=	$(SMC_SPAWN:%=%.o)

SMC_SPAWN_CC = $(SMC_SPAWN:%=$(GENERATED)/%.cc)
SMC_SPAWN_H  = $(SMC_SPAWN:%=$(GENERATED)/%.h)
SMC_SPAWN_O  = $(SMC_SPAWN:%=$(GENERATED)/%.o)



FNAMES	 =		$(addprefix $(GENERATED)/,			\
		verbs				\
		atlas1				\
		atlas2				\
		expression			\
		comparison			\
		AtlasParser			\
		AtlasDLG			\
		tedl	\
		)

FNAMES	 +=		$(addprefix $(GENERATED)/,			\
		TedlParser \
		)


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

AFLAGS_TEDL	= -CC -gxt -ft AtlasTokens.h -gx -gt -w2 -k 2 -prc on -rl 80000 -o $(GENERATED)

GRM_TEDL	= $(addprefix $(GRAMMAR)/, \
			tokens.g tedl.g \
		)

# 		TedlParser			\


FNAMES	+=					\
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

OBJ := $(sort $(OBJ))


################### Generated stuff #############################

ANTLR_ATLAS_SPAWN	= $(addprefix $(GENERATED)/, \
			verbs.cpp	atlas1.cpp	atlas2.cpp	\
			expression.cpp	comparison.cpp	\
			AtlasParser.cpp	AtlasDLG.cpp	AtlasParser.h	AtlasTokens.h	\
			AtlasDLG.h	AtlasDLG.dlg \
)

ANTLR_TEDL_SPAWN	= $(addprefix $(GENERATED)/, \
			tedl.cpp	TedlParser.cpp	TedlParser.h \
)
			

# $(GENERATED):
# 	mkdir -p $@


################### Dependencies ################################
	
atlas:	$(SMC_SPAWN_O)	$(OBJ)	
	$(CCC) $(CCFLAGS) -o atlas  $(SMC_SPAWN_O) $(OBJ)  -ldl  -lpthread
	

################## Atlas stuff ##################################

$(ANTLR_ATLAS_SPAWN):	$(GRM_ATLAS) 
	$(ANTLR) $(AFLAGS_ATLAS) $(GRM_ATLAS)
	$(DLG) $(DFLAGS_ATLAS) $(GENERATED)/AtlasDLG.dlg

################## Tedl stuff ###################################

$(ANTLR_TEDL_SPAWN):		$(GRM_TEDL)
	$(ANTLR) $(AFLAGS_TEDL) $(GRM_TEDL)

##	$(DLG) $(DFLAGS_TEDL) TedlDLG.dlg

################### *.Dependencies ##############################
##TedlStd.h:	TedlTokens.h

$(SMC_SPAWN_O):	$(SMC_SPAWN_CC)

#################################################################

clean:
	-rm -f *.o $(GENERATED)/*.o *.pch core $(ANTLR_ATLAS_SPAWN) $(DLG_ATLAS_SPAWN)
	-rm -f $(ANTLR_TEDL_SPAWN) $(DLG_TEDL_SPAWN)
	-rm -f $(SMC_SPAWN_CC) $(SMC_SPAWN_H)
	-rm -rf Templates.DB ir.out .make.state
	
scrub: clean
	-rm -f atlas

print-vars:
	@echo "OBJ = $(OBJ)"


check-dupes:
	@echo "Checking for duplicate .o files..."
	@echo "$(OBJ)" | tr ' ' '\n' | sort | uniq -d

.KEEP_STATE:

.SUFFIXES: .cpp .sm

.cpp.o:
	$(CCC) -c $(CCFLAGS) -o $*.o $<

.cc.o:
	$(CCC) -c $(CCFLAGS) -o $*.o $<

# .sm.cc:
# 	$(SMC) < $<
	
# .sm.cc:
# 	$(SMC) < $< > $(GENERATED)/$.cc

# $(GENERATED)/%.cc: %.sm
# 	$(SMC) < $< > $@

$(GENERATED)/%.cc $(GENERATED)/%.h: %.sm
	@mkdir -p $(GENERATED)
	(cd $(GENERATED) && ../$(SMC) < ../$<)


$(GENERATED):
	mkdir -p $@

#.sm.h:
#	$(SMC) < $<
	
#.sm.o:
#	$(SMC) < $<
#	$(CCC) -c $(CFLAGS) -o $*.o $< 
