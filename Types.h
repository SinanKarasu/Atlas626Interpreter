#ifndef Types_h
#define Types_h

#include	"Std.h"

#include	"ASTBase.h"
#include	"ATokPtr.h"
//#include	"PBlackBox.h"

//typedef RWCString String;
#include "AppendCompat.h"
#include "Dictionary.h"

const	int c_Und=-1;

//typedef long long Long;

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

using FstatnoDictionary = AppendableMap<int, Fstatno*>;
using FstatnoDictionaryIterator = FstatnoDictionary::iterator;


class TargetStatement{	// BFlag statements

	public:

		TargetStatement( AST * a, RWTValVector<RWInteger> & cl, RWInteger cd );

		AST * _a;
		RWTValVector<RWInteger> _ContextLevel;
		RWInteger _ContextDepth;
};

class GoToStatementStack {
public:
    using value_type = TargetStatement*;

    void push(value_type stmt) {
        stack_.push_back(stmt);
    }

    value_type pop() {
        if (stack_.empty()) return nullptr;
        value_type val = stack_.back();
        stack_.pop_back();
        return val;
    }

    value_type top() const {
        return stack_.empty() ? nullptr : stack_.back();
    }

    bool empty() const {
        return stack_.empty();
    }

    size_t size() const {
        return stack_.size();
    }

    void clear() {
        stack_.clear();
    }

private:
    std::vector<value_type> stack_;
};

using GoToDictionary = AppendableMap<int, GoToStatementStack*>;
using GoToDictionaryIterator = GoToDictionary::iterator;

using EntryDictionary = AppendableMap<int, TargetStatement*>;
using EntryDictionaryIterator = EntryDictionary::iterator;



//#include	"Scope.h"

// sik disabled until made to work..
//#define LAZY_ARRAY

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



template<typename Key, typename Value>
using Dictionary = AppendableMap<Key, Value>;

template<typename Key, typename Value>
using DictionaryIterator = typename Dictionary<Key, Value>::iterator;

template<typename Key, typename Value>
using ConstDictionaryIterator = typename Dictionary<Key, Value>::const_iterator;


////// --- Modern Dictionary Template ---
////template <typename Key, typename Value>
////class Dictionary : public std::unordered_map<Key, Value> {
////public:
////    using std::unordered_map<Key, Value>::unordered_map;
////
////    void insertKeyAndValue(const Key& k, const Value& v) {
////        this->operator[](k) = v;
////    }
////
////    bool insertIfAbsent(const Key& k, const Value& v) {
////        return this->emplace(k, v).second;
////    }
////};
////
////template <typename Key, typename Value>
////class DictionaryIterator {
////public:
////    using iterator = typename std::unordered_map<Key, Value>::iterator;
////
////    DictionaryIterator(std::unordered_map<Key, Value>& dict)
////        : it(dict.begin()), end(dict.end()) {}
////
////    bool operator()() {
////        if (it == end) return false;
////        ++it;
////        return it != end;
////    }
////
////    Key key() const { return it->first; }
////    Value value() const { return it->second; }
////
////private:
////    iterator it;
////    iterator end;
////};


#endif

