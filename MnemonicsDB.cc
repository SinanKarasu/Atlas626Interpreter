#include	"NounsModifiersDimensions.h"
#include	"MnemonicsDB.h"
#include	<iostream>


MnemonicsDB::MnemonicsDB(const RWCString & nounDB,const RWCString & dimensionDB)
	{
		//	Nound.db
		//	m_NounDictionary	=
		LoadAnalogNounDB(nounDB,m_ModifierDictionary);
		//m_QuantityDictionary	= 
		LoadDimensionDB(dimensionDB,m_allDimensions);
		
	}



NounEntry * 
MnemonicsDB::theNounEntry(const RWCString noun)
{
	NounEntry * nounEntry;

	if(m_NounDictionary->findValue(noun,nounEntry)){
		return nounEntry;
	} else {
		//Error_Report(RWCString("Internal error in searching:")+noun);
		return 0;
	}
}

////ModifierEntry * 
////MnemonicsDB::theModifierEntry(const RWCString text,NounEntry * nounEntry,RWCString & last)
////{
////	ModifierEntry * modifierEntry;int pos;
////	if(nounEntry){
////		if(nounEntry->modifierDictionary.findValue(text,modifierEntry)){			
////			return modifierEntry;
////		} else {	// trim from the end and try again if possible
////			
////			if((pos=text.last('-'))!=RW_NPOS){
////			if(!text.empty() && text.back() == '-')
////				last = text(pos,text.length()-pos)+last;
////				return theModifierEntry(text(0,pos),nounEntry,last);
////			} else {
////				return 0;
////			}
////		}
////	} else {
////		if( m_ModifierDictionary->findValue(text,modifierEntry)){
////			return modifierEntry;
////		} else {
////			if((pos=text.last('-'))!=RW_NPOS){
////				last = text(pos,text.length()-pos)+last;
////				return theModifierEntry(text(0,pos),nounEntry,last);
////			} else {
////				return 0;
////			}
////			return 0;
////		}
////	}
////}

ModifierEntry* MnemonicsDB::theModifierEntry(const std::string& text, NounEntry* nounEntry, std::string& last) {
    ModifierEntry* modifierEntry = nullptr;

    if (nounEntry) {
        if (nounEntry->modifierDictionary.findValue(text, modifierEntry)) {
            return modifierEntry;
        } else {
            // Try stripping from the last '-' and recurse
            auto pos = text.rfind('-');
            if (pos != std::string::npos) {
                last = text.substr(pos) + last;
                return theModifierEntry(text.substr(0, pos), nounEntry, last);
            } else {
                return nullptr;
            }
        }
    } else {
        if (m_ModifierDictionary->findValue(text, modifierEntry)) {
            return modifierEntry;
        } else {
            auto pos = text.rfind('-');
            if (pos != std::string::npos) {
                last = text.substr(pos) + last;
                return theModifierEntry(text.substr(0, pos), nounEntry, last);
            } else {
                return nullptr;
            }
        }
    }
}


ModifierEntry * 
MnemonicsDB::theModifierEntry(const RWCString text,const RWCString noun,RWCString & left)
{
	NounEntry *nounEntry=theNounEntry(noun);
	return theModifierEntry(text,nounEntry,left);
}

DimensionDictionary * 
MnemonicsDB::theDimensionDictionary(const RWCString did,ModifierEntry * modifierEntry)
{
	//DimensionDictionary * dimensionDictionary=allDimensions;
	DimensionDictionary * dimensionDictionary=0;
	QuantityList *ql=0;Quantity * q=0;
	RWCString key;
	if(modifierEntry){
		for(int i=0;i<modifierEntry->quantitiesList.size();i++){
			ql=modifierEntry->quantitiesList[i];
			q=(*ql)[0];
			key=q->quantity;
			if(m_QuantityDictionary->findValue(key,dimensionDictionary)){
				break;
			}
		}
	}
	return dimensionDictionary;
}


DimensionDictionary * 
MnemonicsDB::theDimensionDictionary(RWCString quan)
{
	DimensionDictionary * dimensionDictionary=0;
	QuantityList *ql=0;Quantity * q=0;
	if( m_QuantityDictionary->findValue(quan,dimensionDictionary)){
		return dimensionDictionary;
	} else {
		return 0;
	}
}


DimensionEntry * 
MnemonicsDB::theDimensionEntry(const RWCString did,DimensionDictionary * dimensionDictionary)
{
	DimensionEntry * dimensionEntry=0;
	if(dimensionDictionary){
		dimensionDictionary->findValue(did,dimensionEntry);
	} else {
		m_allDimensions->findValue(did,dimensionEntry);
	}
	return dimensionEntry;
}


DimensionEntry * 
MnemonicsDB::theDimensionEntry(const RWCString did,ModifierEntry * modifierEntry)
{
	DimensionDictionary * dimensionDictionary=0;
	DimensionEntry * dimensionEntry=0;
	RWCString key;
	QuantityList *ql=0;Quantity * q=0;
	if(modifierEntry){
		for(int i=0;i<modifierEntry->quantitiesList.size();i++){
			ql=modifierEntry->quantitiesList[i];
			q=(*ql)[0];
			key=q->quantity;
			if(m_QuantityDictionary->findValue(key,dimensionDictionary)){
				if(dimensionDictionary->findValue(did,dimensionEntry)){
					break;
				}
			} else {
				Error_Report("Incomplete Dimension.db. Not found:"+key);
			}
		}
	} else {
		m_allDimensions->findValue(did,dimensionEntry);
	}
	return dimensionEntry;
}

////////////////////////////////////////////////////////////////
char
MnemonicsDB::getNext()
	{
		char ch;
		if(m_database.get(ch)){
			switch (ch) {
			case ' ' : return ' ';
			case '\n': {m_LineNo++;return ' ';}
			case '\t': return ' ';
			default:
				return ch;
			}
		} else {
			return 0;
		}
	}
	
RWCString
MnemonicsDB::getDBToken()
	{
		RWCString ret="";char ch;
		while(ch=getNext()){
			switch (ch) {
			case ' ':
				if(ret.length()==0)break;
				return ret;
			case '(':
			case ')':
			case '[':
			case ']':
			case ',':
				if(ret.length()==0){
					return std::string(1,ch);
				} else {
					m_database.putback(ch);
					return ret;
				}
				return std::string(1,ch);
			default:
				ret+=ch;
			}
		}
		
		return ret;
		
		
	}

void
MnemonicsDB::insertModifierQuantity(ModifierEntry *modifierEntry)
	{
		RWBoolean numerator=TRUE;
		char ch;
		QuantityList *ql=0;
		Quantity * term;
		RWCString token="";
		do{
			token=getDBToken();
			switch (ch=token[0]){
			case '/':numerator=FALSE;break;
			case ',':  case ')':
				{
					modifierEntry->insertQuantityList(ql);
					ql=0;
					if(ch==')')return;
				};
				break;
			default:
				term=new Quantity;
				term->quantity=token;
				term->numerator=numerator;
				term->level=0;
				if(ql==0){
					ql=new QuantityList;
				}
				ql->append(term);
			}
			
		} while(1);
	}

void
MnemonicsDB::insertSuffixes(ModifierEntry * modifierEntry)
	{
		RWCString suffix;
		suffix=getDBToken();
		while(suffix !=")"){
			SuffixEntry* suffixEntry = new SuffixEntry;
			if(!modifierEntry->suffixDictionary){
				modifierEntry->suffixDictionary=new SuffixDictionary;
			}
			modifierEntry->suffixDictionary->insertKeyAndValue(suffixEntry->suffix,suffixEntry);
			suffix=getDBToken();
		}
	}


void
MnemonicsDB::insertAllModifiers(ModifierEntry *modifierEntry,ModifierDictionary *modifierDictionary)
	{
		ModifierEntry * _modifierEntry=0;
		if(modifierDictionary->findValue(modifierEntry->modifier,_modifierEntry)){
			// note here we should extend the quantities if not cloned....
		} else {
			_modifierEntry=modifierEntry->clone();
			_modifierEntry->usage="---";
			modifierDictionary->insertKeyAndValue(_modifierEntry->modifier,_modifierEntry);
		}
	}


void
MnemonicsDB::insertModifiers(NounEntry * nounEntry,ModifierDictionary * modifierDictionary)
	{
		RWCString token;
		ModifierEntry* modifierEntry = new ModifierEntry(nounEntry);
		modifierEntry->modifier=getDBToken();		// modifier
					getDBToken();		// skip ','
		modifierEntry->typeCode=getDBToken();// Type of Entry R,I,SB.SC,RA,IA,AB,AC,MD,MO,SS
					getDBToken();		// skip ','
		modifierEntry->usage   =getDBToken();// Type of Entry ---,--M,-R-,-RM,S--,S-M,SR-,SRM
		nounEntry->modifierDictionary.insertKeyAndValue(modifierEntry->modifier,modifierEntry);
		if(getDBToken()== "("){
			insertModifierQuantity(modifierEntry);
		} else {
			std::cerr << " Opening Parenthesis for Quantity field is missing " << std::endl;
			assert(0);
		}
		insertAllModifiers(modifierEntry,modifierDictionary);
		if(getDBToken()== ")"){
			if(getDBToken()== "("){
				insertSuffixes(modifierEntry);
			} else {
				std::cerr << " Opening Parenthesis for Suffixes field is missing " << std::endl;
				assert(0);
			}
			return;
		} else {
			std::cerr << " Closing Parenthesis for Suffix field is missing " << std::endl;
			assert(0);
		}
	}

	

void
MnemonicsDB::insertNoun(NounDictionary * nounDictionary,ModifierDictionary * modifierDictionary)
	{
		RWCString noun,token; char ch;
		NounEntry * nounEntry=new NounEntry;
		do {
			token=getDBToken();
			switch (ch=token[0]){
			case '(':
				{
					nounEntry->noun=noun;
					insertModifiers(nounEntry,modifierDictionary);break;
				}
			case ')':
				{
					//cout << " inserting:" << noun << ":" << endl;
					m_NounDictionary->insertKeyAndValue(noun,nounEntry);
						return;
				}
			default:
				if(noun==""){
					noun=token;
				} else {
					noun+=" " + token; // single space separator
				}
			}
		} while(token!="");
		
	}		

NounDictionary *
MnemonicsDB::LoadAnalogNounDB(const RWCString filename,ModifierDictionary *& allModifiers)
{

	RWCString token;int level=0;char ch;
	const NounEntry * nounEntry=0;
	const ModifierEntry * modifierEntry=0;
	//NounDictionary * nounDictionary = new NounDictionary;
	m_NounDictionary = new NounDictionary;
	QuantityList * quantityList; Quantity * quantity;
	//ModifierDictionary * modifierDictionary = new ModifierDictionary;
	m_ModifierDictionary = new ModifierDictionary;

	m_database.open(filename);
	m_LineNo=1;

	do{
		token=getDBToken();
		if(token.length()==0)break;
		switch (ch=token[0]){
			case '(':insertNoun(m_NounDictionary,m_ModifierDictionary);break;
			case ')':
			{
				std::cerr	<< "LoadAnalogNounDB: Unmatched parenthesis.Line:"
					<< m_LineNo <<std::endl;
			}break;
			assert(0);
			default:;
		}
				
	} while(1);


	m_database.close();
	//m_allModifiers=m_ModifierDictionary;
	return m_NounDictionary;

//	m_NounDictionary->findValue("AMBIENT CONDITIONS",nounEntry);
//	cout << nounEntry->noun << endl;
//	nounEntry->modifierDictionary.findValue("BAROMETRIC-PRESS",modifierEntry);
//	cout << modifierEntry->modifier << " " << modifierEntry->typeCode << " " << modifierEntry->usage << endl;
//	for(int i=0;i< modifierEntry->quantityList.length();i++){
//		quantityList=	modifierEntry->quantityList[i];
//		for(int j=0;j< quantityList->length();j++){
//			quantity=(*quantityList)[j];
//			cout << "Q:" << quantity->quantity << ":" << quantity->numerator << ":" << quantity->level << endl;
//		}
//	}
	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void
MnemonicsDB::insertDimension(const RWCString quantity,DimensionDictionary * dimensionDictionary,DimensionDictionary *allDimensions)
	{
		int level=0;char ch;double scale=0.0;
		DimensionEntry * dimensionEntry;
		RWCString token="";
		do{
			token=getDBToken();
			switch (ch=token[0]){
			case '|':
				{
					scale=0.0;
 				}break;
 			case ')':
 				{
 					return;
 				}
			case '[':
				{
					token=getDBToken();
					scale=std::stod(token);
					token=getDBToken();
					dimensionEntry->scale=scale;
					if(token!="]"){
						std::cerr	<< "LoadDimensionDB:BAD scale field .Line: "
							<< m_LineNo <<std::endl;
					} 
				}break;
				assert(0);
			case ']':
				{
						std::cerr	<< "LoadDimensionDB:Unexpected ] .Line: "
							<< m_LineNo <<std::endl;
				
				} break;
			default:
				dimensionEntry = new DimensionEntry;
				dimensionEntry->dimension=token;
				dimensionEntry->quantity=quantity;
				dimensionDictionary->insertKeyAndValue(dimensionEntry->dimension,dimensionEntry);
				allDimensions->insertKeyAndValue(dimensionEntry->dimension,dimensionEntry);
			}
			
		} while(level>=0);
	}





void
MnemonicsDB::insertDimensionQuantity(QuantityDictionary * quantityDictionary,DimensionDictionary *allDimensions)
	{
		RWCString quantity,token; char ch;
		DimensionDictionary * dimensionDictionary = new DimensionDictionary;
		do {
			token=getDBToken();
			switch (ch=token[0]){
			case '(':
				{
					insertDimension(quantity, dimensionDictionary,allDimensions );break;
				}
			case ')':
				{
					quantityDictionary->insertKeyAndValue(quantity,dimensionDictionary);
					return;
				}
					
			default:
				quantity=token;
			}
		} while(token!="");
		
	}		
			
QuantityDictionary *
MnemonicsDB::LoadDimensionDB(const RWCString filename,DimensionDictionary * &allDimensions)	

{

	RWCString token;int level=0;char ch;
	
	//QuantityDictionary * 
	m_QuantityDictionary = new QuantityDictionary;

	m_allDimensions = new DimensionDictionary;

	m_database.open(filename);m_LineNo=1;

	do{
		token=getDBToken();
		if(token.length()==0)break;
		switch (ch=token[0]){
			case '(':	insertDimensionQuantity(m_QuantityDictionary,m_allDimensions);break;
			case ')':
				{
					std::cerr	<< "LoadDimensionDB:Unmatched parenthesis.Line: "
						<< m_LineNo <<std::endl;
				}break;
				assert(0);
			default:;
		}
		
		
	} while(1);

	m_database.close();
	
	return m_QuantityDictionary;

//	quantityDictionary->findValue("Energy",dimensionDictionary);
//	dimensionDictionary->findValue("DBPJ",dimensionEntry);
//	cout << "Dim" << dimensionEntry->dimension  << endl;	
}


