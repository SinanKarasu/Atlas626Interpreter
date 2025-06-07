
#include	"NounsModifiersDimensions.h"
#include	"AtlasStd.h"

std::ifstream  database;
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
		quantitiesList.append(ql);
	}

int ModifierEntry::isUsage(const RWCString c)
	{
		return (usage.find(c)!=RW_NPOS);
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


