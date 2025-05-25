#ifndef Types_h
#define Types_h

#include	"Std.h"

#include	"ASTBase.h"
#include	"ATokPtr.h"
//#include	"PBlackBox.h"

//typedef RWCString String;

const	int c_Und=-1;

typedef long long Long;

enum	TheType
	{
		UndefinedTypeValue=0		,
		BasicTypeValue			,
		IntegerNumberValue		,
		EnumerationTypeValue		,	
		EnumerationsTypeValue		,
		DecimalNumberValue		,
		ConnectionTypeValue		,
		ConnectionsTypeValue		,
		TerminalTypeValue		,
		CharTypeValue			,
		CharClassTypeValue		,
		StringOfCharTypeValue		,
		DigClassTypeValue		,
		BooleanTypeValue		,
		BitTypeValue			,
		StringOfBitTypeValue		,
		PreDeclaredEnumerationTypeValue	,
		ArrayTypeValue			,
		CharacterStringTypeValue	,
		TextTypeValue			,
		FileTypeValue			,
		RecordTypeValue			,
		IndexRangeTypeValue		,
		ArrayElementsTypeValue		,
		ArraySliceTypeValue		,
		StatementNumberValue		,
		
		ControlModifierType		,
		CapabilityModifierType		,
		LimitModifierType		,
		
		EventIntervalTypeValue		,
		EventIndicatorTypeValue		,
		TimeBasedEventTypeValue		,
		AnalogEventTypeValue		,
		OneShotTimerTypeValue		,
		PeriodicTimerTypeValue		,
		EventCounterTypeValue		,


		EventSlopePosTypeValue		,
		EventSlopeNegTypeValue		,
		HysteresisTypeValue
		
		};
		
enum LabelType
	{
		Undefined_Label			,
		Program_Name_Label		,
		Module_Name_Label		,
		NonAtlasModule_Name_Label	,
		Block_Name_Label		,
		Parameter_Label			,
		Drawing_Label			,
		Signal_Label			,
		Procedure_Label			,
		Function_Label			,
		Event_Label			,
		Event_Interval_Label		,
		Event_Indicator_Label		,
		Exchange_Label			,
		Protocol_Label			,
		Device_Identifier_Label		,
		Configuration_Label		,
		Exchange_Configuration_Label	,
		Digital_Source_Label		,
		Digital_Sensor_Label		,
		Timer_Label			,
		Requirement_Label		,
		Constant_Identifier_Label	,
		Type_Identifier_Label		,
		Variable_Identifier_Label	,
		File_Label			,
		Enumeration_Element_Label	,
		Connection_Type_Label		,
		Modifier_Descriptor_Name_Label	,
		Bus_Specification_Label		,
		Modifier_Name_Label		,
		Dim_Name_Label			,
		Pin_Descriptor_Name_Label	,
		Terminal_Identifier_Label	,
		Protocol_Parameter_Name_Label	,
		Bus_Parameter_Name_Label	,
		Bus_Mode_Name_Label		,
		Test_Equip_Role_Name_Label
	};	


enum EntryType {	EntryUndefined,
			EntryBLOCK,
			EntryFOR,
			EntryIF,
			EntryWHILE,
			EntryPROCEDURE,
			EntryFUNCTION
		};

class Fstatno{

	public:	
		Fstatno();
		RWInteger _testno;
		RWInteger _stepno;
		class LineAction * _entry;
		friend std::ostream& operator << (std:: ostream &,Fstatno *);
		int getLine() const ;
};	


class TargetStatement{	// BFlag statements

	public:

		TargetStatement( AST * a, RWTValVector<RWInteger> & cl, RWInteger cd );

		AST * _a;
		RWTValVector<RWInteger> _ContextLevel;
		RWInteger _ContextDepth;
};

//#include	"Scope.h"

#define LAZY_ARRAY

//class Scope;


void Error_Report(RWCString E,ANTLRTokenPtr t);
void Error_Report(RWCString E,int l);
void Error_Report(RWCString E,AST * a);
void Error_Report(RWCString E);

void TedlError(RWCString E,int l);

//AST * Execute(AST * root,AST *abort);

int verify(AST * a, AST * b);

int sane();
void	clear_statement_error_flag();

extern int trace_level;
extern int debug_statno;

#endif

