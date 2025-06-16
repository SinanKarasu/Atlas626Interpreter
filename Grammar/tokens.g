#header<<
	#include	"atlas.h"
>>//header

#lexprefix <<

#include    "NestedTokenStream.h"

>>

#lexmember
	<<
		typedef void (AtlasDLG::*Func)(void);
		
	>>

#lexmember <<
    NestedTokenStream           *nestedTokenStream;
    const char *                filename;
    virtual _ANTLRTokenPtr      getToken();
>>


#lexmember << int sawFStatno ; >>

#lexmember << virtual void newline()  { _line++;set_endcol(0); } >>

#lexmember
	<<
		//--------------------------------------------------------------------
		enum	{MaxModeStack=10};
		int	modeStack[MaxModeStack];
		int 	stackDepth;
		Func	funcStack[MaxModeStack];
		int	homeState;
		//--------------------------------------------------------------------
		void	pushMode(int newMode,Func func)
		{
			if(stackDepth == (MaxModeStack - 1) ) {
				panic("Mode stack overflow ");
			} else {
				modeStack[stackDepth] = automaton;	// getMode() in next version
				funcStack[stackDepth] = func;
				stackDepth++;
				mode(newMode);
			}
		}
		//--------------------------------------------------------------------
		void	pushMode(int newMode)
		{
			if(stackDepth == (MaxModeStack - 1) ) {
				panic("Mode stack overflow ");
			} else {
				modeStack[stackDepth] = automaton;	// getMode() in next version
				funcStack[stackDepth] = 0;
				stackDepth++;
				mode(newMode);
			}
		}
		//--------------------------------------------------------------------
		void	popMode()
		{

			if(stackDepth <= 0) {
				panic("Mode stack underflow ");
			} else {
				stackDepth--;
				Func thisFunc=funcStack[stackDepth];
				mode(modeStack[stackDepth]);
				modeStack[stackDepth]=0;
				funcStack[stackDepth]=0;
				/* this call might result in indirect recursion of popMode() */
				if (thisFunc != 0) {
					(this->*thisFunc)();
				};
			};
		}
		//--------------------------------------------------------------------
		void	resetModeStack()
		{
			stackDepth=0;
			modeStack[0]=0;
			funcStack[0]=0;
			mode(homeState);
		}
		virtual int startClass() { return 0; }
	>>			

#tokclass NounCLASS { EID SIGNAL STEP TIME INTERVAL EARTH COMMON SHORT SOURCE SENSOR DIGITAL }

#tokclass ModifierCLASS { EID TIME MONITOR PARITY DELAY }


#tokclass DimensionCLASS { EID TIMES }

#tokclass PinDescriptorCLASS {
				HI LO 
				VIA
				A B C N 
				ZzzTRUE COMPL
				SCREEN GUARD
				R1 R2 R3 R4
				S1 S2 S3 S4
				X Y Z
				}

#token	Eof "@"

////------------------ Lexical Class ----------------------------------------------------------------------------------------////

#lexclass STARTAtlas
#token	StatementTerminator	"$"		<<
							TedlError("Rogue $ in column 1:", line() );
							pushMode(ProcessStatementTerminator,&AtlasDLG::resetModeStack);
						>>
#token				"C"		<<skip();pushMode(CommentMode);>>
#token	BFlag			"B"		<<pushMode(CommentMode);>>
#token				"E"		<<pushMode(FStatNoMode);more();sawFStatno=1;>>
#token				"\ "		<<pushMode(FStatNoMode);more();>>
#token	FStatno			"\t"		<<replstr("       ");pushMode(VerbMode);>>
#token				"\n"		<<newline();skip();>>
#token				"\r"		<<skip();>>
#token	FStatno			"[0-9]+"	<<
							TedlError( RWCString( "Statno in column 1:")+lextext(), line() );
							pushMode(VerbMode);sawFStatno=1;
						>>
#token	Bad_statement_no	"~[\ \t\n\r\$]"	<<
							TedlError( RWCString( "Unclassifiable Statement:")+lextext(), line() );
							skip();pushMode(SkipMode);
						>>

////------------------ Lexical Class ----------------------------------------------------------------------------------------////

#lexclass ProcessStatementTerminator
#token				"\ "			<<skip();>>
#token				"\t"			<<skip();>>
#token				"\n"			<<newline();skip();resetModeStack();>>
#token				"\r"			<<skip();>>
#token				"~[\ \t\r\n]+"		<<TedlError( RWCString( "Characters after $:")+lextext(), line() ); skip();>>

////------------------ Lexical Class ----------------------------------------------------------------------------------------////

#lexclass SkipMode
#token				"\n"			<<newline();skip();>>
#token				"$"			<<skip();pushMode(ProcessStatementTerminator,&AtlasDLG::resetModeStack);>>
#token				"\r"			<<skip();>>
#token				"~[\n$]"		<<skip();>>

////------------------ Lexical Class ----------------------------------------------------------------------------------------////

#lexclass CommentMode
#token	StatementTerminator	"$"			<<skip();pushMode(ProcessStatementTerminator,&AtlasDLG::resetModeStack);>>
#token				"\n"			<<newline();skip();>>
#token				"~[\n\$]"		<<skip();>>		// Comment is consumed in DLG

////------------------ Lexical Class ----------------------------------------------------------------------------------------////

#lexclass FStatNoMode
#token	StatementTerminator	"$"			<<pushMode(ProcessStatementTerminator,&AtlasDLG::resetModeStack);sawFStatno=0;>>
#token				"[\ ][\ ][\ ][\ ]"	<<pushMode(StepNumberMode,&AtlasDLG::popMode);more();sawFStatno=0;>>
#token				"[0-9][0-9][0-9][0-9]"	<<pushMode(StepNumberMode,&AtlasDLG::popMode);more();sawFStatno=1;>>
#token	FStatno			"{[\ ]}[\t]"		<<
								if((ch=='\n')||(ch=='\r')){
									pushMode(ProcessStatementTerminator,&AtlasDLG::resetModeStack);
									sawFStatno=0;
									skip();
								} else {
									replstr("       ");
									pushMode(StepNumberMode,&AtlasDLG::popMode);
									more();sawFStatno=0;
								}
							>>

#token				"[\ ]{[\ ]}{[\ ]}"	<<
								if(ch=='\n'){
									pushMode(ProcessStatementTerminator,&AtlasDLG::resetModeStack);
									sawFStatno=0;
									skip();
								} else {
									TedlError(RWCString( "Unrecognized TESTNO field")+lextext(), line() );
									pushMode(ProcessStatementTerminator,&AtlasDLG::resetModeStack);
									skip();sawFStatno=0;
								}
							>>

#token	Bad_statement_no	"~[\n\ \$\r]~[\n\ \$\r]~[\n\ \$\r]~[\n\ \$\r]"		
							<<
								TedlError(RWCString( "Unrecognized TESTNO field")+lextext(), line() );
								pushMode(VerbMode,&AtlasDLG::popMode);sawFStatno=0;
							>>
							
#token	Bad_statement_no	"~[\n\r\$]"		<<
								TedlError(RWCString( "Unclassifiable TESTNO. Skipping to $:")+lextext(), line() );
								skip();pushMode(SkipMode,&AtlasDLG::resetModeStack);sawFStatno=0;
							>>
							
#token				"\n"			<<newline();skip();popMode();sawFStatno=0;>>
#token				"\r"			<<skip();sawFStatno=0;>>

////------------------ Lexical Class ----------------------------------------------------------------------------------------////

#lexclass StepNumberMode
#token	StatementTerminator	"$"			<<
								TedlError( RWCString( "Premature $ at ")+lextext(), line() );
								skip();pushMode(ProcessStatementTerminator,&AtlasDLG::resetModeStack);
							>>
							
#token				"\n"			<<
								if(sawFStatno==1){
									TedlError( RWCString( "VERB expected ")+lextext(),
										 line());
								}
								newline();skip();popMode();
							>>
							
#token	FStatno			"[0-9][0-9]"		<<
								pushMode(VerbMode,&AtlasDLG::popMode);sawFStatno=1;
							>>
								
#token	FStatno			"[\ ][\ ][\ ]*"		<<
								if((ch=='\n') &&  (sawFStatno==0)){
									skip();	// empty line
									pushMode(ProcessStatementTerminator,&AtlasDLG::resetModeStack);
								} else {
									pushMode(VerbMode,&AtlasDLG::popMode);sawFStatno=0;
								}
							>>
							
#token	Bad_statement_no	"~[\n\ \$]~[\n\ \$]"	<<
								TedlError( RWCString( "Unrecognized STEPNO field")+lextext(), line() );
								replstr("       ");
								pushMode(VerbMode,&AtlasDLG::popMode);
							>>

#token	Unrecognized_char	"~[\n \$]"		<<
								TedlError(
									RWCString( "Unclassifiable STEPNO. Skipping to $:")+lextext(),
									line()
								);
								pushMode(SkipMode,&AtlasDLG::popMode);skip();
							>>

////------------------ Lexical Class ----------------------------------------------------------------------------------------////

#lexclass VerbMode
#token	StatementTerminator	"$"		<<pushMode(ProcessStatementTerminator,&AtlasDLG::resetModeStack);>>
#token				"\n"		<<newline();skip();>>
#token				"\r"		<<skip();>>
#token				"[\ \t]"	<<skip();>>
#token	APPLY			"APPLY"		<<pushMode(StatementMode,&AtlasDLG::popMode);>>
#token	ARM			"ARM"		<<pushMode(StatementMode,&AtlasDLG::popMode);>>
#token	BEGIN 			"BEGIN"		<<pushMode(StatementMode,&AtlasDLG::popMode);>>
#token	CALCULATE		"CALCULATE"	<<pushMode(StatementMode,&AtlasDLG::popMode);>>
#token	CHANGE			"CHANGE"	<<pushMode(StatementMode,&AtlasDLG::popMode);>>
#token	COMMENCE		"COMMENCE"	<<pushMode(StatementMode,&AtlasDLG::popMode);>>
#token	COMPARE			"COMPARE"	<<pushMode(StatementMode,&AtlasDLG::popMode);>>
#token	CONNECT			"CONNECT"	<<pushMode(StatementMode,&AtlasDLG::popMode);>>
#token	CREATE			"CREATE"	<<pushMode(StatementMode,&AtlasDLG::popMode);>>
#token	DECLARE			"DECLARE"	<<pushMode(StatementMode,&AtlasDLG::popMode);>>
#token	DEFINE			"DEFINE"	<<pushMode(StatementMode,&AtlasDLG::popMode);>>
#token	DELETE			"DELETE"	<<pushMode(StatementMode,&AtlasDLG::popMode);>>
#token	DISABLE			"DISABLE"	<<pushMode(StatementMode,&AtlasDLG::popMode);>>
#token	DISCONNECT		"DISCONNECT"	<<pushMode(StatementMode,&AtlasDLG::popMode);>>
#token	DO			"DO"		<<pushMode(StatementMode,&AtlasDLG::popMode);>>
#token	ELSE			"ELSE"		<<pushMode(StatementMode,&AtlasDLG::popMode);>>
#token	ENABLE			"ENABLE"	<<pushMode(StatementMode,&AtlasDLG::popMode);>>
#token	END			"END"		<<pushMode(StatementMode,&AtlasDLG::popMode);>>
#token	ESTABLISH		"ESTABLISH"	<<pushMode(StatementMode,&AtlasDLG::popMode);>>
#token	EXTEND			"EXTEND"	<<pushMode(StatementMode,&AtlasDLG::popMode);>>
#token	FETCH			"FETCH"		<<pushMode(StatementMode,&AtlasDLG::popMode);>>
#token	FINISH			"FINISH"	<<pushMode(StatementMode,&AtlasDLG::popMode);>>
#token	FOR			"FOR"		<<pushMode(StatementMode,&AtlasDLG::popMode);>>
#token	GO_TO			"GO[\ \t]+TO"	<<pushMode(StatementMode,&AtlasDLG::popMode);>>
#token	IDENTIFY		"IDENTIFY"	<<pushMode(StatementMode,&AtlasDLG::popMode);>>
#token	IF			"IF"		<<pushMode(StatementMode,&AtlasDLG::popMode);>>
#token	INCLUDE			"INCLUDE"	<<pushMode(StatementMode,&AtlasDLG::popMode);>>
#token	INITIATE		"INITIATE"	<<pushMode(StatementMode,&AtlasDLG::popMode);>>
#token	INPUT			"INPUT"		<<pushMode(StatementMode,&AtlasDLG::popMode);>>
#token	LEAVE			"LEAVE"		<<pushMode(StatementMode,&AtlasDLG::popMode);>>
#token	MEASURE			"MEASURE"	<<pushMode(StatementMode,&AtlasDLG::popMode);>>
#token	MONITOR			"MONITOR"	<<pushMode(StatementMode,&AtlasDLG::popMode);>>
#token	OUTPUT			"OUTPUT"	<<pushMode(StatementMode,&AtlasDLG::popMode);>>
#token	PERFORM			"PERFORM"	<<pushMode(StatementMode,&AtlasDLG::popMode);>>
#token	PROVE			"PROVE"		<<pushMode(StatementMode,&AtlasDLG::popMode);>>
#token	READ			"READ"		<<pushMode(StatementMode,&AtlasDLG::popMode);>>
#token	REMOVE			"REMOVE"	<<pushMode(StatementMode,&AtlasDLG::popMode);>>
#token	REQUIRE			"REQUIRE"	<<pushMode(StatementMode,&AtlasDLG::popMode);>>
#token	RESET			"RESET"		<<pushMode(StatementMode,&AtlasDLG::popMode);>>
#token	RESUME			"RESUME"	<<pushMode(StatementMode,&AtlasDLG::popMode);>>
#token	SENSE			"SENSE"		<<pushMode(StatementMode,&AtlasDLG::popMode);>>
#token	SETUP			"SETUP"		<<pushMode(StatementMode,&AtlasDLG::popMode);>>
#token	SPECIFY			"SPECIFY"	<<pushMode(StatementMode,&AtlasDLG::popMode);>>
#token	STIMULATE		"STIMULATE"	<<pushMode(StatementMode,&AtlasDLG::popMode);>>
#token	TERMINATE		"TERMINATE"	<<pushMode(StatementMode,&AtlasDLG::popMode);>>
#token	UPDATE			"UPDATE"	<<pushMode(StatementMode,&AtlasDLG::popMode);>>
#token	VERIFY			"VERIFY"	<<pushMode(StatementMode,&AtlasDLG::popMode);>>
#token	WAIT			"WAIT"		<<pushMode(StatementMode,&AtlasDLG::popMode);>>
#token	WHILE			"WHILE"		<<pushMode(StatementMode,&AtlasDLG::popMode);>>

#token				"~[\n\ \t,\'\r]+"	<<
							TedlError(RWCString( "Unclassifiable VERB. Skipping to $:")+lextext(), line() );
							skip();pushMode(SkipMode,&AtlasDLG::popMode);
						>>

////------------------ Lexical Class ----------------------------------------------------------------------------------------////

#lexclass StatementMode
#token	StatementTerminator	"$"			<<pushMode(ProcessStatementTerminator,&AtlasDLG::resetModeStack);>>
#token				"\n"			<<newline();skip();>>
#token				"[\ \t\r]"		<<skip();>>

#token	Single_Bit		"(B\'[01]\') | ([XO]1\'[01]\')"
#token	Binary_Number		"B\'[01][01]+\'"
#token	Hexadecimal_Number	"X{[1234]}\'[0-9A-F]+\'"
#token	Octal_Number		"O{[123]}\'[0-7]+\'"
#token	Semi_Colon		"\;"
#token	Unsigned_Decimal_Number	"([0-9]+.[0-9]+|[0-9]+.|.[0-9]+){E{[\+\-]}[0-9]+} | [0-9]+E{[\+\-]}[0-9]+"
#token	Unsigned_Integer_Number	"[0-9]+"
#token	RSQUARE			"\]"
#token	RP			"\)"
#token	PLUSop			"\+"
#token	PLUSMINUSop		"\+\-"
#token	PERIODop		"\."
#token	MULop			"\*"
#token	MINUSop			"\-"
#token	LSQUARE			"\["
#token	LP			"\("
#token	Fd			","
#token	DIVIDEop		"\/"
#token	EQUALS			"="

// sed generated tokens start


#token	ZzzTRUE			"TRUE"
#token	ZzzFALSE		"FALSE"
#token	ZZFILE			"FILE"
#token	ZZEOF			"EOF"
#token	ZERO			"ZERO"
#token	XOR			"XOR"
#token	WORDdTRANSITION		"WORD\-TRANSITION"
#token	WORDdCOUNT		"WORD\-COUNT"
#token	WITHIN			"WITHIN"
#token	WITH			"WITH"
#token	WHILE			"WHILE"
#token	WHEN			"WHEN"
#token	WAIT			"WAIT"

#token	VIA			"VIA"
#token	VERIFY			"VERIFY"
#token	VERBS			"VERBS"
#token	VARIABLE		"VARIABLE"
#token	VAR			"VAR"
#token	VALUE			"VALUE"
#token	UUT			"UUT"
#token	USING			"USING"
#token	USAGE			"USAGE"
#token	UPDATABLEdFETCHABLE	"UPDATABLE\-FETCHABLE"
#token	UPDATABLE		"UPDATABLE"
#token	UNTIL			"UNTIL"
#token	UL			"UL"

#token	TdR			"T\-R"
#token	TYPE			"TYPE"
#token	TRANSdSYNCdBIT		"TRANS\-SYNC\-BIT"
#token	TRANSdSYNC		"TRANS\-SYNC"
#token	TRANSdLOGICdZERO	"TRANS\-LOGIC\-ZERO"
#token	TRANSdLOGICdONE		"TRANS\-LOGIC\-ONE"
#token	TO			"TO"
#token	TIMES			"TIMES"
#token	TIMER			"TIMER"
#token	TIMED			"TIMED"
#token	TIME			"TIME"
#token	THRU			"THRU"
#token	THEN			"THEN"
#token	TEXT			"TEXT"
#token	TESTdEQUIPdSIMULATE	"TEST\-EQUIP\-SIMULATE"
#token	TESTdEQUIPdROLE		"TEST\-EQUIP\-ROLE"
#token	TESTdEQUIPdMONITOR	"TEST\-EQUIP\-MONITOR"
#token	TESTdEQUIPdDELAY	"TEST\-EQUIP\-DELAY"
#token	TESTdEQUIP		"TEST\-EQUIP"
#token	TERMINALS		"TERMINALS"			<<pushMode(CNXMode);>>
#token	TAN			"TAN"
#token	TALKER			"TALKER"
#token	TABLE			"TABLE"

#token	SYNCdBITdTRANSITION	"SYNC\-BIT\-TRANSITION"
#token	SYNC			"SYNC"
#token	SUCC			"SUCC"
#token	STRING			"STRING"
#token	STIMdRATE		"STIM\-RATE"
#token	STIMdEVENT		"STIM\-EVENT"
#token	STIMULATE		"STIMULATE"
#token	STEP			"STEP"
#token	STATUSdWORD		"STATUS\-WORD"
#token	STATUS			"STATUS"
#token	STATE			"STATE"
#token	STARTING		"STARTING"
#token	ZzzSTART		"START"
#token	STANDARD		"STANDARD"
#token	SQRT			"SQRT"
#token	SPECIALdSYNTAX		"SPECIAL\-SYNTAX"
#token	SPEC			"SPEC"
#token	SOURCE			"SOURCE"
#token	SKEWdTIME		"SKEW\-TIME"
#token	SIZE			"SIZE"
#token	SIN			"SIN"
#token	SIMULTANEOUS		"SIMULTANEOUS"
#token	SIGNAL			"SIGNAL"
#token	SHORT			"SHORT"
#token	SHIFT			"SHIFT"
#token	SETUP			"SETUP"
#token	SET			"SET"
#token	SERIALdMSBdFIRST	"SERIAL\-MSB\-FIRST"
#token	SERIALdLSBdFIRST	"SERIAL\-LSB\-FIRST"
#token	SEQUENTIAL		"SEQUENTIAL"
#token	SENSOR			"SENSOR"
#token	SENSEdRATE		"SENSE\-RATE"
#token	SENSEdEVENT		"SENSE\-EVENT"
#token	SENSEdDELAY		"SENSE\-DELAY"
#token	SENSE			"SENSE"
#token	SCREEN			"SCREEN"
#token	SBCD			"SBCD"
#token	SAVEdCOMP		"SAVE\-COMP"

#token	RZ			"RZ"
#token	ROUND			"ROUND"
#token	ROTATE			"ROTATE"
#token	RESULT			"RESULT"
#token	RESET			"RESET"
#token	REQUIRE	 		"REQUIRE"
#token	REMOVE	 		"REMOVE"
#token	REF	 		"REF"
#token	REDUNDANT		"REDUNDANT"
#token	RECORD			"RECORD"
#token	READ			"READ"
#token	RANGE			"RANGE"
#token	RANDOM			"RANDOM"

#token	QUIES			"QUIES"

#token	PULSEdCODE		"PULSE\-CODE"
#token	PULSEdCLASS		"PULSE\-CLASS"
#token	PROVE			"PROVE"
#token	PROTOCOLdPARAMETER	"PROTOCOL\-PARAMETER"
#token	PROTOCOLdEXCHANGE	"PROTOCOL\-EXCHANGE"
#token	PROTOCOL		"PROTOCOL"
#token	PROGRAM			"PROGRAM"
#token	PROCEED			"PROCEED"
#token	PROCEDURE		"PROCEDURE"
#token	PRIORITY		"PRIORITY"
#token	PRIMARY			"PRIMARY"
#token	PRED			"PRED"
#token	PO			"PO"
#token	PE			"PE"
#token	PC			"PC"
#token	PARITY			"PARITY"

#token	OUTPUT			"OUTPUT"
#token	ORop			"OR"
#token	ORD			"ORD"
#token	OR			"OR"
#token	ONE			"ONE"
#token	ON			"ON"
#token	OLD			"OLD"
#token	OF			"OF"
#token	ODD			"ODD"
#token	OCTAL			"OCTAL"
#token	OCCURRENCES		"OCCURRENCES"

#token	NRZ			"NRZ"
#token	NOUNS			"NOUNS"
#token	NOTop			"NOT"
#token	NONdATLAS		"NON\-ATLAS"
#token	NONE			"NONE"
#token	NOM			"NOM"
#token	NOGO			"NOGO"
#token	NEcomp			"NE"
#token	NEW			"NEW"
#token	N			"N"

#token	MONITOR			"MONITOR"
#token	MODop			"MOD"
#token	MODdVALUE		"MOD\-VALUE"
#token	MODdDIM			"MOD\-DIM"
#token	MODdDESCdSPEC		"MOD\-DESC\-SPEC"
#token	MODdARRAY		"MOD\-ARRAY"
#token	MODULE			"MODULE"
#token	MODIFIERdDESCRIPTOR	"MODIFIER\-DESCRIPTOR"
#token	MODIFIERS		"MODIFIERS"
#token	MNEMONICdONLY		"MNEMONIC\-ONLY"
#token	MIP			"MIP"
#token	MINUTE			"MINUTE"
#token	MINIMUMdSENSEdRATE	"MINIMUM\-SENSE\-RATE"
#token	MIN			"MIN"
#token	MEASURE			"MEASURE"
#token	MAXdTIME		"MAX\-TIME"
#token	MAX			"MAX"
#token	MASKdZERO		"MASK\-ZERO"
#token	MASKdONE		"MASK\-ONE"
#token	MANUALdINTERVENTION	"MANUAL\-INTERVENTION"
#token	MANUAL			"MANUAL"
#token	MAIN			"MAIN"

#token	LTcomp			"LT"
#token	LOGIC			"LOGIC"
#token	LOG			"LOG"
#token	LOCN			"LOCN"
#token	LOAD			"LOAD"
#token	LO			"LO"
#token	LN			"LN"
#token	LL			"LL"
#token	LISTENER		"LISTENER"
#token	LIMIT			"LIMIT"
#token	LEcomp			"LE"
#token	LEVELdLOGICdZERO	"LEVEL\-LOGIC\-ZERO"
#token	LEVELdLOGICdQUIES	"LEVEL\-LOGIC\-QUIES"
#token	LEVELdLOGICdONE		"LEVEL\-LOGIC\-ONE"
#token	LEVELdLOGICdHIZ		"LEVEL\-LOGIC\-HIZ"
#token	LENGTH			"LENGTH"
#token	LEN			"LEN"
#token	LATER			"LATER"

#token	IdO			"I\-O"
#token	IS			"IS"
#token	INTO			"INTO"
#token	INTERVENTION		"INTERVENTION"
#token	INTERVAL		"INTERVAL"
#token	INTEGER			"INTEGER"
#token	INT			"INT"
#token	INSERTdINVALID		"INSERT\-INVALID"
#token	INSERT			"INSERT"
#token	INPUT			"INPUT"
#token	INITIATE		"INITIATE"
#token	INITIALIZE		"INITIALIZE"
#token	INITIAL			"INITIAL"
#token	INDICATOR		"INDICATOR"
#token	INCREASING		"INCREASING"
#token	ILLEGAL			"ILLEGAL"
#token	IF			"IF"
#token	IDENTIFY		"IDENTIFY"

#token	HYSTERESIS		"HYSTERESIS"
#token	HOLD			"HOLD"
#token	HIZ			"HIZ"
#token	HI			"HI"
#token	HEXADECIMAL		"HEXADECIMAL"
#token	HDB			"HDB"

#token	GUARD			"GUARD"
#token	GTcomp			"GT"
#token	GOdNOGO			"GO\-NOGO"
#token	GO			"GO"
#token	GLOBAL			"GLOBAL"	
#token	GEcomp			"GE"
#token	GATED			"GATED"
#token	GATE			"GATE"

#token	FUNCTION		"FUNCTION"
#token	FROM			"FROM"
#token	FOR			"FOR"
#token	FETCHABLE		"FETCHABLE"
#token	FETCH			"FETCH"
#token	FAULTdTEST		"FAULT\-TEST"
#token	FAULTdCOUNT		"FAULT\-COUNT"

#token	EdFORMAT		"E\-FORMAT"
#token	EXTERNAL		"EXTERNAL"	
#token	EXPOop			"\*\*"
#token	EXP			"EXP"
#token	EXCHANGEdPIN		"EXCHANGE\-PIN"
#token	EXCHANGE		"EXCHANGE"
#token	EXCEPT			"EXCEPT"
#token	EVERY			"EVERY"
#token	EVENT			"EVENT"
#token	ESCAPE			"ESCAPE"
#token	ERRLMT			"ERRLMT"
#token	EQcomp			"EQ"
#token	ENUMERATION		"ENUMERATION"
#token	ENTIRE			"ENTIRE"
#token	EARTH			"EARTH"

#token	DRAWING			"DRAWING"	<< pushMode(DrawingIDBeginMode);>>
#token	DO			"DO"
#token	DIVop			"DIV"
#token	DISCONNECT		"DISCONNECT"
#token	DIMdSPEC		"DIM\-SPEC"
#token	DIGdCLASS		"DIG\-CLASS"
#token	DIGITAL			"DIGITAL"
#token	DIG			"DIG"
#token	DELETE			"DELETE"
#token	DELAYdMIN		"DELAY\-MIN"
#token	DELAY			"DELAY"
#token	DEFINE			"DEFINE"
#token	DECREASING		"DECREASING"
#token	DECIMAL			"DECIMAL"
#token	DEC			"DEC"
#token	DATE			"DATE"
#token	DATAdWORD		"DATA\-WORD"
#token	DATA			"DATA"
#token				"C\'" << pushMode(CStringMode);more(); >>

#token	COUNTdINTO		"COUNT\-INTO"
#token	COUNT			"COUNT"
#token	COS			"COS"
#token	COPY			"COPY"
#token	CONTROL			"CONTROL"
#token	CONTINUOUS		"CONTINUOUS"
#token	CONSTANT		"CONSTANT"
#token	CONSECUTIVE		"CONSECUTIVE"
#token	CONNECTIONS		"CONNECTIONS"
#token	CONNECTION		"CONNECTION"		<<pushMode(ConnectionBeginMode);>>
#token	CONNECT			"CONNECT"
#token	CONFIGURATION		"CONFIGURATION"
#token	COMPL			"COMPL"
#token	COMMON			"COMMON"
#token	COMMANDdWORD		"COMMAND\-WORD"
#token	COMMAND			"COMMAND"
#token	COLON			":"
#token	CNXdNUMERATOR		"CNX\-NUMERATOR"	<<pushMode(CNXMode);>>
#token	CNXdDENOMINATOR		"CNX\-DENOMINATOR"	<<pushMode(CNXMode);>>
#token	CNX			"CNX"			<<pushMode(CNXMode);>>
#token	CHARdCLASS		"CHAR\-CLASS"
#token	CHAR			"CHAR"
#token	CHANGE			"CHANGE"
#token	CAPABILITY		"CAPABILITY"
#token	BY			"BY"
#token	BUSdPROTOCOL		"BUS\-PROTOCOL"
#token	BUSdPARAMETER		"BUS\-PARAMETER"
#token	BUSdMODE		"BUS\-MODE"
#token	BUS			"BUS"
#token	BSM			"BSM"
#token	BOOLEAN			"BOOLEAN"
#token	BNR			"BNR"
#token	BLOCK			"BLOCK"
#token	BITdTRANSITION		"BIT\-TRANSITION"
#token	BITdRATE			"BIT\-RATE"
#token	BITdPERIOD		"BIT\-PERIOD"
#token	BITS			"BITS"
#token	BIT			"BIT"
#token	BIP			"BIP"
#token	BINARY			"BINARY"
#token	BCD			"BCD"
#token	B2C			"B2C"
#token	B1C			"B1C"
#token	AdSHIFT			"A\-SHIFT"
//#token	AUTONOMOUS		"AUTONOMOUS"
#token	ATMOS			"ATMOS"
#token	ATLAS			"ATLAS"
#token	ATAN			"ATAN"
#token	AT			"AT"
#token	ASCII7			"ASCII7"
#token	AS			"AS"
#token	ARRAY			"ARRAY"
#token	ARM			"ARM"
#token	APPLY			"APPLY"
#token	AND			"AND"
#token	AMI			"AMI"
#token	ALTERNATEdBUSdTRANSMIT	"ALTERNATE\-BUS\-TRANSMIT"
#token	ALOG			"ALOG"
//#token	ALLdLISTENER		"ALL\-LISTENER"
#token	ALL			"ALL"
#token	AFTER			"AFTER"
#token	ABS			"ABS"

// sed generated tokens end...

#token	CONCATEop		"\&"



#token	"\'"			<< pushMode(LabelMode);more(); >>

#token	EID			"[A-Za-z](~[\=\t\ \n\,$\'\(\)\;\r]*)"

#lexclass CStringMode
#token	StatementTerminator	"$"			<<pushMode(ProcessStatementTerminator,&AtlasDLG::resetModeStack);>>
#token				"\n"			<<TedlError(" END Of LINE in quoted string",line());newline();more();>>
#token	Single_Char		"~[\'$]\'"		<<popMode();>>
#token	Character_String	"~[\'$]*\'"		<<popMode();>>

////------------------ Lexical Class ----------------------------------------------------------------------------------------////

#lexclass LabelMode
#token	StatementTerminator	"$"			<<pushMode(ProcessStatementTerminator,&AtlasDLG::resetModeStack);>>
#token	ID "\'"						<<popMode();>>
#token	"[\!\"\#\%\&\:\;\<\>\?\@\~\[\]\|\_\\\^\`\{\}]+"	<<more();>> // SymbolExtensionSet
#token	"[a-z]+"					<<more();>> // LowerCaseLetters	
#token	"[A-Z]+"					<<more();>> // UpperCaseLetters
#token	"[0-9]+"					<<more();>> // Digits
#token	 "[\*\.\=]+"					<<more();>> // Complement to CommonCharSet
#token	"\ "						<<more();>> // Blanks are OK
#token	"\+"						<<more();>> // Plus
#token	"\-"						<<more();>> // Minus
#token	"[\/,]+"					<<more();>> // All the rest
#token	"\n"						<<
								TedlError(" END Of LINE in quoted string",line());
								newline();more();
							>>

////------------------ Lexical Class ----------------------------------------------------------------------------------------////

#lexclass DrawingIDBeginMode
#token			"\n"					<<newline();skip();>>

#token			"[\t\r\ ]"				<<skip();>>
#token		Fd	","					<<pushMode(DrawingIDMode,&AtlasDLG::popMode);>>
#token	StatementTerminator	"$"			<<popMode();pushMode(ProcessStatementTerminator,&AtlasDLG::resetModeStack);>>

////------------------ Lexical Class ----------------------------------------------------------------------------------------////

#lexclass DrawingIDMode
#token	DrawingID			"~[,]*"	<<popMode();>>

////------------------ Lexical Class ----------------------------------------------------------------------------------------////

#lexclass ConnectionBeginMode
#token	StatementTerminator	"$"	<<popMode();pushMode(ProcessStatementTerminator,&AtlasDLG::resetModeStack);>>

#token			"[\ \t\r]+"	<<
						skip();
						switch (ch) {
						case '\n':break;
						case '$' :break;
						case '(' :break;
						default:pushMode(ConnectionMode,&AtlasDLG::popMode);
						};
					>>
					
#token			"\n"		<<
						newline();skip();
						switch (ch) {
						case ' ' :break;
						case '\t':break;
						case '\n':break;
						case '$' :break;
						case '(' :break;
						default:pushMode(ConnectionMode,&AtlasDLG::popMode);
						};
					>>
#token	LP	"\("			<<
						pushMode(ConnectionInParenMode,&AtlasDLG::popMode);
						pushMode(ConnectionMode);
					>>

////------------------ Lexical Class ----------------------------------------------------------------------------------------////

#lexclass ConnectionInParenMode
#token	StatementTerminator	"$"	<<popMode();pushMode(ProcessStatementTerminator,&AtlasDLG::resetModeStack);>>
#token				"\ "	<<skip();>>
#token				"\t"	<<skip();>>
#token				"\r"	<<skip();>>
#token				"\n"	<<newline();skip();>>
#token	RP			"\)"	<<popMode();>>
#token	Fd			","	<<pushMode(ConnectionMode);>>


////------------------ Lexical Class ----------------------------------------------------------------------------------------////

#lexclass CNXMode
#token	StatementTerminator	"$"		<<popMode();pushMode(ProcessStatementTerminator,&AtlasDLG::resetModeStack);>>
#token				"[\ \t\r]+"		<<
							skip();
							switch (ch) {
							case '\n':break;
							case '$' :break;
							case ',':break;
							default:pushMode(ConnectionMode);
							};
						>>
#token				"\n"		<<
							newline();skip();
							switch (ch) {
							case ' ' :break;
							case '\n':break;
							case '\r':break;
							case '\t':break;
							case '$' :break;
							case ',' :break;
							default:pushMode(ConnectionMode);
							};
						>>
#token		Fd		","		<<	popMode();	>>



////------------------ Lexical Class ----------------------------------------------------------------------------------------////

#lexclass ConnectionMode
#token	StatementTerminator	"$"		<<popMode();pushMode(ProcessStatementTerminator,&AtlasDLG::resetModeStack);>>
#token	LP       		"\("		<<popMode();>>
#token	RP        		"\)"		<<popMode();>>
#token				"\n"		<<
							newline();skip();
							switch (ch) {
							case ',' :popMode();break;
							//case ')' :break;
							//case '\r':break;
							//case '\t':break;
							//case '$' :break;
							//default:pushMode(ConnectionMode,&AtlasDLG::popMode);
							default:break;
							};
						>>
#token				"[\ \t\r]+"	<<
							skip();
						>>
#token	COMMON			"COMMON"	<<popMode();>>
#token	EARTH			"EARTH"		<<popMode();>>
#token	ATMOS			"ATMOS"		<<popMode();>>

#token	REF			"REF"		<<popMode();>>
#token	PHASEdCB			"PHASE\-CB"	<<popMode();>>
#token	PHASEdCA			"PHASE\-CA"	<<popMode();>>
#token	PHASEdC			"PHASE\-C"	<<popMode();>>
#token	PHASEdBC			"PHASE\-BC"	<<popMode();>>
#token	PHASEdBA			"PHASE\-BA"	<<popMode();>>
#token	PHASEdB			"PHASE\-B"	<<popMode();>>
#token	PHASEdAC			"PHASE\-AC"	<<popMode();>>
#token	PHASEdAB			"PHASE\-AB"	<<popMode();>>
#token	PHASEdA			"PHASE\-A"	<<popMode();>>



// stuff from PinDescriptorMode... 
#token	SCREEN			"SCREEN"	<<popMode();>>
#token	COMPL			"COMPL"		<<popMode();>>
#token	GUARD			"GUARD"		<<popMode();>>
#token	ZzzTRUE			"TRUE"		<<popMode();>>
#token	VIA			"VIA"		<<popMode();>>
#token	HI			"HI"		<<popMode();>>
#token	LO			"LO"		<<popMode();>>
#token	R1			"R1"		<<popMode();>>
#token	R2			"R2"		<<popMode();>>
#token	R3			"R3"		<<popMode();>>
#token	R4			"R4"		<<popMode();>>
#token	S1			"S1"		<<popMode();>>
#token	S2			"S2"		<<popMode();>>
#token	S3			"S3"		<<popMode();>>
#token	S4			"S4"		<<popMode();>>
#token	X			"X"		<<popMode();>>
#token	Y			"Y"		<<popMode();>>
#token	Z			"Z"		<<popMode();>>
#token	A			"A"		<<popMode();>>
#token	B			"B"		<<popMode();>>
#token	C			"C"		<<popMode();>>
#token	N			"N"		<<popMode();>>

#token				"\'"					<<pushMode(LabelMode,&AtlasDLG::popMode);more();>>
#token	TID			"\*([A-Z]|[a-z]|[0-9]|[\+]|[\-]|[\.])+"	<<popMode();>>
#token	CID			"([A-Z]|[a-z]|[0-9]|[\+]|[\-]|[\.])+"	<<popMode();>>

#lexclass NoSuchClass
#token	NoSuchToken		"NoSuchToken"

// From here on down are the Tedl Stuff

#tokclass	NameCLASS	{ Unsigned_Integer_Number EID ID }

#lexclass	STARTTedl

#token	Fd			"\,"
#token	Statement_Terminator	"\$"
#token	Semi_Colon		"\;"
#token	VERSION			"VERSION"
#token	RP			"\)"
#token	AtSign			"\@"

#token	Unsigned_Integer_Number	"[0-9]+"
#token	Unsigned_Decimal_Number	"([0-9]+.[0-9]+|[0-9]+.|.[0-9]+){E{[\+\-]}[0-9]+}|[0-9]+E{[\+\-]}[0-9]+"
#token	Right_Angle_Bracket	"\>"
#token	PLUSMINUSop		"\+/\-"
#token	PLUSop			"\+"
#token	Percent			"%"
#token	DIVIDEop		"/"
#token	Open_Quote		"\""				<<pushMode(StringMode);>>
#token	Open_Bracket		"\{"				<<pushMode(VirtualMode);>>
#token	MINUSop			"\-"
#token	LP			"\("
#token	Left_Angle_Bracket	"\<"
#token	Dot			"\."

// Following tokens come from ATLAS
#token	RZ			"RZ"
#token	NRZ			"NRZ"
#token	BIP			"BIP"
#token	MIP			"MIP"
#token	AMI			"AMI"
#token	HDB			"HDB"
// End of ATLAS tokens

#token	Zz4_WIRE		"4_WIRE"
#token	Zz3_WIRE		"3_WIRE"
#token	Zz2_WIRE		"2_WIRE"
#token	Zz1_WIRE		"1_WIRE"
#token	WORD_TRANSITION		"WORD_TRANSITION"
#token	WIRE			"WIRE"
#token	VXI1p4			"VXI1\.4"
#token	VXI1p3			"VXI1\.3"
#token	VXI1p2			"VXI1\.2"
#token	VALUE			"VALUE"				// sik extension
#token	UUT_CONNECTOR		"UUT_CONNECTOR"
#token	UUT			"UUT"
#token	USING			"USING"
#token	UPDATE			"UPDATE"
#token	UPDATABLEdFETCHABLE	"UPDATABLE\-FETCHABLE"
#token	UPDATABLE		"UPDATABLE"
#token	UNALLOCATED		"UNALLOCATED"
#token	TWOTERM			"TWOTERM"			// sik extension
#token	TO_PORT			"TO_PORT"
#token	TO			"TO"
#token	TIME_OUT		"TIME_OUT"
#token	TIMER			"TIMER"
#token	THEN			"THEN"
#token	TESTdEQUIPdROLE		"TEST\-EQUIP\-ROLE"
#token	TESTdEQUIPdMONITOR	"TEST\-EQUIP\-MONITOR"
#token	TESTdEQUIP		"TEST\-EQUIP"
#token	TALKER			"TALKER"
#token	SYNC			"SYNC"
#token	SWITCH			"SWITCH"
#token	STIM_RESP		"STIM_RESP"
#token	STIMULATE		"STIMULATE"
#token	STIM			"STIM"
#token	STATUS			"STATUS"
#token	STATE			"STATE"
#token	SPST			"SPST"
#token	SPEC			"SPEC"
#token	SOURCE			"SOURCE"
#token	SIG_CHAR		"SIG_CHAR"
#token	SETUP_RESOURCE		"SETUP_RESOURCE"
#token	SETUP			"SETUP"
#token	SET			"SET"
#token	SENSOR			"SENSOR"
#token	SENSE			"SENSE"
#token	RS232			"RS232"
#token	RESP			"RESP"
#token	RESET			"RESET"
#token	RENAME_PREFACE		"RENAME_PREFACE"
#token	REGISTER_BASED		"REGISTER_BASED"
#token	REF_DES			"REF_DES"
#token	READ_RESOURCE		"READ_RESOURCE"
#token	READY			"READY"
#token	RANGE			"RANGE"
#token	PROVE			"PROVE"
#token	PREPARED		"PREPARED"
#token	PREP			"PREP"
#token	POS_SLOPE		"POS_SLOPE"
#token	PORT_ALIASES		"PORT_ALIASES"
#token	PORT			"PORT"
#token	PATH			"PATH"
#token	OR			"OR"
#token	NOUN			"NOUN"
#token	NONE			"NONE"
#token	NEcomp			"NE"
#token	NEG_SLOPE		"NEG_SLOPE"
#token	NEED_PATH		"NEED_PATH"
#token	NEED_DEVICE_MODEL	"NEED_DEVICE_MODEL"		// sik extension
#token	NEED_DEVICE		"NEED_DEVICE"
#token	MULTIPLEX		"MULTIPLEX"
#token	MODIFIER		"MODIFIER"
#token	MIN			"MIN"
#token	MESSAGE_BASED		"MESSAGE_BASED"
#token	MEASURED		"MEASURED"
#token	MAX			"MAX"
#token	MATRIX			"MATRIX"
#token	LTcomp			"LT"
#token	LOAD			"LOAD"
#token	LISTENER		"LISTENER"
#token	LIMIT			"LIMIT"
#token	LEcomp			"LE"
#token	IS			"IS"
#token	INTERFACE_CONNECTOR	"INTERFACE_CONNECTOR"
#token	IF_ERROR		"IF_ERROR"
#token	IF			"IF"
#token	IEEE_488p2		"IEEE_488\.2"
#token	IEEE_488p1		"IEEE_488\.1"
#token	GTcomp			"GT"
#token	GPIB			"GPIB"
#token	GEcomp			"GE"
#token	GANG			"GANG"
#token	FUNCTION		"FUNCTION"
#token	FETCH			"FETCH"
#token	EXTEND			"EXTEND"
#token	EXCHANGE		"EXCHANGE"
#token	EVENT_VOLTAGE_EQ	"EVENT_VOLTAGE_EQ"
#token	EVENT_SLOPE_POS		"EVENT_SLOPE_POS"
#token	EVENT_SLOPE_NEG		"EVENT_SLOPE_NEG"
#token	EVENT_PORT		"EVENT_PORT"
#token	EVENT_MONITOR		"EVENT_MONITOR"
#token	EVENT			"EVENT"
#token	ETHERNET		"ETHERNET"
#token	EQcomp			"EQ"
#token	END			"END"
#token	ENABLE			"ENABLE"
#token	ELSE_IF			"ELSE_IF"
#token	ELSE			"ELSE"
#token	EARTH			"EARTH"
#token	DO			"DO"
#token	DISCONNECT_COMMAND	"DISCONNECT_COMMAND"		// sik extension
#token	DISCONNECT_CHANGE	"DISCONNECT_CHANGE"
#token	DISCONNECT		"DISCONNECT"
#token	DISABLE			"DISABLE"
#token	DIMENSION		"DIMENSION"
#token	DIGITAL_TEST		"DIGITAL_TEST"
#token	DEVICE_MODEL		"DEVICE_MODEL"
#token	DEVICE			"DEVICE"
#token	DELAY			"DELAY"
#token	DEFAULT			"DEFAULT"			// sik extension
#token	DATA			"DATA"
#token	CONTROL_PARAMETER	"CONTROL_PARAMETER"		// sik extension
#token	CONTROL_INTERFACE	"CONTROL_INTERFACE"
#token	CONTROL			"CONTROL"
#token	CONNECT_PORT		"CONNECT_PORT"
#token	CONNECT_ALGORITHM	"CONNECT_ALGORITHM"
#token	CONNECTS		"CONNECTS"
#token	CONNECTIONS		"CONNECTIONS"
#token	CONNECTED		"CONNECTED"
#token	CONNECT			"CONNECT"
#token	CONFIGURATION_MODEL	"CONFIGURATION_MODEL"
#token	COMPOSITE_DEVICE_MODEL	"COMPOSITE_DEVICE_MODEL"	// sik extension
#token	COMPOSITE_DEVICE	"COMPOSITE_DEVICE"
#token	COMMON			"COMMON"
#token	COMMAND			"COMMAND"
#token	CHANGE			"CHANGE"
#token	CAPABILITY		"CAPABILITY"
#token	BY			"BY"
#token	BUSdPROTOCOL		"BUS\-PROTOCOL"
#token	BUSdPARAMETER		"BUS\-PARAMETER"
#token	BUSdMODE		"BUS\-MODE"
#token	BIT_TRANSITION		"BIT_TRANSITION"
#token	ATE			"ATE"
#token	AT			"AT"
#token	ASSIGN			"ASSIGN"
#token	ARMED			"ARMED"
#token	ARM			"ARM"
#token	APPLIED			"APPLIED"
#token	AND			"AND"
#token	ALL			"ALL"
#token	ADDRESS			"ADDRESS"
#token	ADAPTER_CONNECTOR	"ADAPTER_CONNECTOR"	// extension
#token	ADAPTATION_MODEL	"ADAPTATION_MODEL"


#token	EID			"{\*}[a-zA-Z0-9](~[\t\ \n\,$\'\(\)\;]*)"
#token				"[\t\r\ ]+"			<<skip();>>
#token				"\n"				<<newline();skip();>>
#token				"/\*"				<<skip();pushMode(CCommentMode);>>
#token				"\'"				<<pushMode(LabelMode);more();>>
#token				"\/\/(~[\n])*"			<<skip();>>

#lexclass CCommentMode
#token				"\n"				<<skip();newline();>>
#token				"\r"				<<skip();>>
#token				"\*/"				<<skip();popMode();>>
#token				"\*"				<<skip();>>
#token				"~[\*\n]+"			<<skip();>>

#lexclass StringMode
#token	Close_Quote		"\""				<<popMode();>>
#token				"\n"				<<newline();skip();>>
#token				"\r"				<<skip();>>

#token	Left_Angle_Bracket	"\<"
#token	Characters		"~[\,\<\>\n\"]+"
#token	Right_Angle_Bracket	"\>"
#token	Fd			"\,"
#token				"."				<<
									popMode();
									std::cerr	<< "ERROR, bad token at line " 
										<< line() << std::endl;
								>>

#lexclass VirtualMode
#token	Close_Bracket		"\}"				<<popMode();>>
#token				"\n"				<<newline();skip();>>

#token	AtSign			"\@"
#token	Left_Angle_Bracket	"\<"
#token	Fd			"\,"
#token	Semi_Colon		"\;"
#token				"[\ \r\t]+"			<<skip();>>

#token	LP			"\("
#token	RP			"\)"
#token	PLUSop			"\+"
#token	MINUSop			"\-"
#token	Unsigned_Integer_Number	"[0-9]+"
#token	Unsigned_Decimal_Number	"([0-9]+.[0-9]+|[0-9]+.|.[0-9]+){E{[\+\-]}[0-9]+}|[0-9]+E{[\+\-]}[0-9]+"
#token	EID			"{[\.]|[\*]}[A-Za-z_][A-Za-z0-9_\-]*"
#token	Right_Angle_Bracket	"\>"
#token	COLON			":"
#token	Characters		"~[\n\@\<\,\;\ \(\)\>\+\-\{\}]+"
#token				"/\*"				<<skip();pushMode(CCommentMode);>>
#token				"\/\/(~[\n])*"			<<skip();>>
#token				"."				<<
									popMode();
									std::cerr	<< "ERROR, bad token at line " 
										<< line() << std::endl;
								>>


// End of TedlTokens.....

//End of tokens.g
