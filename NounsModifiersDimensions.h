#ifndef NounsModifiersDimensions_h
#define NounsModifiersDimensions_h

#include	"Types.h"


class Quantity{
public:
	Quantity();
	Quantity(const RWCString & quan);

	RWCString quantity;	
	RWBoolean numerator;
	int level;	
};

class QuantityList:public RWTValOrderedVector<Quantity * >{
public:
};

class QuantitiesList:public RWTValOrderedVector<QuantityList * >{
public:
};

class SuffixDictionary;

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

class SuffixEntry{
public:
	SuffixEntry();
	RWCString suffix;
	RWCString modifier;
};

class DimensionEntry{
public:
	DimensionEntry();
	DimensionEntry(RWCString dime,RWCString quan);
	RWCString dimension;
	RWCString quantity;
	double scale;
};

class DimensionDictionary : public RWTValHashDictionary<RWCString,DimensionEntry *> {
public:
	DimensionDictionary();
	
private:
   enum { NbrBuckets = RWDEFAULT_CAPACITY };
};

class QuantityDimensions{
	RWCString quantity;	
	DimensionDictionary dimensions;
};



class ModifierDictionary : public RWTValHashDictionary<RWCString,ModifierEntry *> {
public:
	ModifierDictionary();
	
private:
   enum { NbrBuckets = RWDEFAULT_CAPACITY };
};

class ModifierDictionaryIterator : public RWTValHashDictionaryIterator<RWCString,ModifierEntry *>{
	
public:
	ModifierDictionaryIterator( ModifierDictionary &d );
};

class SuffixDictionary : public RWTValHashDictionary<RWCString,SuffixEntry *> {
public:
	SuffixDictionary();
	
private:
   enum { NbrBuckets = RWDEFAULT_CAPACITY };
};

class NounEntry{
public:
	NounEntry();
	RWCString noun;
	ModifierDictionary modifierDictionary;
};

class NounDictionary : public RWTValHashDictionary<RWCString,NounEntry *> {
public:
	NounDictionary();
	
private:
   enum { NbrBuckets = RWDEFAULT_CAPACITY };
};




class QuantityDictionary : public RWTValHashDictionary<RWCString,DimensionDictionary *> {
public:
	QuantityDictionary();
	
private:
   enum { NbrBuckets = RWDEFAULT_CAPACITY };
};
	

unsigned NounHash(const RWCString& str);
unsigned ModifierHash(const RWCString& str);
unsigned QuantityHash(const RWCString& str); 
unsigned DimensionHash(const RWCString& str);


class StringList : public RWTValSlist<RWCString>{
};

class StringListIterator : public RWTValSlistIterator<RWCString>{
public:
	StringListIterator(StringList &d);
};

#endif // NounsModifiersDimensions_h
