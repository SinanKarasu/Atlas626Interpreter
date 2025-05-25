
#include	"NounsModifiersDimensions.h"
#include	"AtlasStd.h"

ifstream  database;
int LineNo;

	
Quantity::Quantity():numerator(TRUE){};
Quantity::Quantity(const RWCString & quan):numerator(TRUE)
	{
		if(&quan)quantity=quan;
	};

ModifierEntry::ModifierEntry(NounEntry * nounEntry)
	:suffixDictionary(0),m_nounEntry(nounEntry)
	{
	}

ModifierEntry::ModifierEntry(NounEntry * nounEntry,RWCString m)
	:suffixDictionary(0),m_nounEntry(nounEntry),modifier(m)
	{
	}

void ModifierEntry::setTypeCode(ANTLRTokenPtr p)
	{
		RWCString code=p->getText();
		
		if(		code == "R"
			||	code == "I"
			||	code == "SC"
			||	code == "SB"
			||	code == "RA"
			||	code == "IA"
			||	code == "AB"
			||	code == "AC"
			||	code == "MD"
			||	code == "MO"
			||	code == "SS"
		  )	{
		  		typeCode=code;
		  } else {
		  	Error_Report(" BAD modifier code ",p);
		  }
	}

void ModifierEntry::setUsage(ANTLRTokenPtr p)
	{
		RWCString code=p->getText();
		
		if     (code == "R"  ){usage="-R-";}
		else if(code == "S"  ){usage="S--";}
		else if(code == "SR" ){usage="SR-";}
		else if(code == "RM" ){usage="-RM";}
		else if(code == "SRM"){usage="SRM";}
		else {
		  	Error_Report(" BAD Usage code ",p);
		}
	}

ModifierEntry * ModifierEntry::clone()
	{
		ModifierEntry * me=new ModifierEntry(m_nounEntry);
		me->usage=usage;
		me->modifier=modifier;
		me->typeCode=typeCode;
		me->quantitiesList=quantitiesList;
		me->suffixDictionary=suffixDictionary;
		return me;
	}

Long	ModifierEntry::compare( ModifierEntry * o ) const
	{
		if (	usage    == o->usage	&&
			modifier == o->modifier	&&
			typeCode == o->typeCode      )
		{
		
			return 0;
		}else{
			return -1;
		}
	}
	

void	ModifierEntry::insertQuantityList( QuantityList * ql )
	{
		quantitiesList.insert(ql);
	}

int ModifierEntry::isUsage(const RWCString c)
	{
		return (usage.index(c)!=RW_NPOS);
	}

SuffixEntry::SuffixEntry(){};
DimensionEntry::DimensionEntry(){};
DimensionEntry::DimensionEntry( RWCString dime, RWCString quan )
	{
	}
	

NounEntry::NounEntry()
	{
	}


DataBusEntry::DataBusEntry(class NounEntry * nounEntry)
	:ModifierEntry(nounEntry)
	{
	}

unsigned NounHash(const RWCString& str) { return str.hash(); }
unsigned ModifierHash(const RWCString& str) { return str.hash(); }
unsigned QuantityHash(const RWCString& str) { return str.hash(); }
unsigned DimensionHash(const RWCString& str) { return str.hash(); }
unsigned SuffixHash(const RWCString& str) { return str.hash(); }



ModifierDictionary::ModifierDictionary():RWTValHashDictionary<RWCString,ModifierEntry *>(ModifierHash)
        {resize(NbrBuckets);}

ModifierDictionaryIterator::ModifierDictionaryIterator( ModifierDictionary &d )
	:RWTValHashDictionaryIterator<RWCString,ModifierEntry *> (d)
	{
	}

SuffixDictionary::SuffixDictionary():RWTValHashDictionary<RWCString,SuffixEntry *>(SuffixHash)
        {resize(NbrBuckets);}


NounDictionary::NounDictionary():RWTValHashDictionary<RWCString,NounEntry *>(NounHash)
        {resize(NbrBuckets);}

DimensionDictionary::DimensionDictionary():RWTValHashDictionary<RWCString,DimensionEntry *>(DimensionHash)
        {resize(NbrBuckets);}

QuantityDictionary::QuantityDictionary():RWTValHashDictionary<RWCString,DimensionDictionary *>(QuantityHash)
        {resize(NbrBuckets);}

	
StringListIterator::StringListIterator(StringList &d)
	:RWTValSlistIterator<RWCString> (d)
	{
	}

