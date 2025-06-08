#ifndef NounsModifiersDimensions_h
#define NounsModifiersDimensions_h

#include	"Types.h"
//#include "Dictionary.h"
#include "AppendCompat.h"
class Quantity{
public:
	Quantity();
	Quantity(const RWCString & quan);

	RWCString quantity;	
	RWBoolean numerator;
	int level;	
};

class SuffixEntry{
public:
	SuffixEntry();
	RWCString suffix;
	RWCString modifier;
};


// class QuantityList:public RWTValOrderedVector<Quantity * >{
// public:
// };

// class QuantitiesList:public RWTValOrderedVector<QuantityList * >{
// public:
// };


using QuantityList = AppendableVector<Quantity *>;
using QuantitiesList = AppendableVector<QuantityList *>;

using SuffixDictionary = Dictionary<std::string,SuffixEntry *>;
using SuffixDictionaryIterator = DictionaryIterator<std::string, SuffixEntry*>;

class ModifierEntry{
public:
	ModifierEntry(class NounEntry * nounEntry);
	ModifierEntry(class NounEntry * nounEntry,RWCString m);
	
	void		insertQuantityList	( QuantityList * ql );
	void		setTypeCode		( ANTLRTokenPtr p );
	void		setUsage		( ANTLRTokenPtr p );
	ModifierEntry * clone			();
	Long		compare			( ModifierEntry * o ) const;
	int isUsage(const RWCString c);
	RWCString	usage;
	RWCString	modifier;
	RWCString	typeCode;
	
	QuantitiesList  quantitiesList;
	SuffixDictionary * suffixDictionary;
	NounEntry * m_nounEntry;
};

class DataBusEntry : public ModifierEntry{
public:
	DataBusEntry(class NounEntry * nounEntry);

};

class DimensionEntry;


using DimensionDictionary = Dictionary<std::string,DimensionEntry *>;
using DimensionDictionaryIterator = DictionaryIterator<std::string, DimensionEntry*>;



class QuantityDimensions{
	RWCString quantity;	
	DimensionDictionary dimensions;
};


using ModifierDictionary = Dictionary<std::string,ModifierEntry *>;
using ModifierDictionaryIterator = DictionaryIterator<std::string, ModifierEntry*>;

class NounEntry{
public:
	NounEntry();
	RWCString noun;
	ModifierDictionary modifierDictionary;
};

using NounDictionary = Dictionary<std::string,NounEntry *>;
using NounDictionaryIterator = DictionaryIterator<std::string, NounEntry*>;

using QuantityDictionary = Dictionary<std::string,DimensionDictionary *>;
using QuantityDictionaryIterator = DictionaryIterator<std::string, DimensionDictionary*>;

// unsigned NounHash(const RWCString& str);
// unsigned ModifierHash(const RWCString& str);
// unsigned QuantityHash(const RWCString& str); 
// unsigned DimensionHash(const RWCString& str);

class DimensionEntry{
public:
	DimensionEntry();
	DimensionEntry(RWCString dime,RWCString quan);
	RWCString dimension;
	RWCString quantity;
	double scale;
};

using StringList = AppendableList<std::string>;

// class StringList : public RWTValSlist<RWCString>{};
// 
// class StringListIterator : public RWTValSlistIterator<RWCString>{
// public:
// 	StringListIterator(StringList &d);
// };

#endif // NounsModifiersDimensions_h
