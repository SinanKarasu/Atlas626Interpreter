
#include	"TedlStd.h"
#include	"TedlParser.h"



Resource *	TedlParser::insertDeviceModel( Resource * devmodel )
	{
		Resource * x = 0;
		
		if( deviceModelsDictionary.findValue( devmodel->getName(), x ) ){
			Error_Report( "Device Model was already defined" + devmodel->getName() );
			return 0;
		}else{
			deviceModelsDictionary.insertKeyAndValue( devmodel->getName(), devmodel );
			return devmodel;
		}
	}

Resource *	TedlParser::getDeviceModel(AST * nodeName)
	{
		Resource * x = 0;
		
		if( deviceModelsDictionary.findValue( nodeName->getName(), x ) ){
			return x;
		}else{
			Error_Report( "Device Model not found " + nodeName->getName() );
			return 0;
		}
	}


//Resource *	TedlParser::insertRealDevice( Resource * devmodel )
//	{
//		Resource * x = 0;
//		
//		if( realDevicesDictionary.findValue( devmodel->getName(), x ) ){
//			Error_Report( "Device Model was already defined" + devmodel->getName() );
//			return 0;
//		}else{
//			realDevicesDictionary.insertKeyAndValue( devmodel->getName(), devmodel );
//			return devmodel;
//		}
//	}

Resource *	TedlParser::getDevice( AST * nodeName )
	{
		Resource * x = 0;
		
		if( realDevicesDictionary.findValue( nodeName->getName(), x ) ){
			return x;
		}else{
			Error_Report( "Device not found " + nodeName->getName() );
			return 0;
		}
	}
		
Resource *	TedlParser::getDevice( RWCString nodeName )
	{
		Resource * x = 0;
		
		if( realDevicesDictionary.findValue( nodeName, x ) ){
			return x;
		}else{
			Error_Report( "Device not found " + nodeName );
			return 0;
		}
	}


int	TedlParser::removeAll(int softHard)
	{
		ResourceListIterator arit(activeResources);
		ResourceList	tempList(activeResources);

		//while(++arit){
		//	templist.insert(arit.key());
		//}

		ResourceListIterator tlit(tempList);

		while(++tlit){
			tlit.key()->resetResource(softHard);
		}
		return 0;
	}


void
TedlParser::syn(
			ANTLRAbstractToken	*	tok,
			ANTLRChar		*	egroup,
			SetWordType		*     	eset,
			ANTLRTokenType			etok,
			int 				k
		)
	{
		
		int line = ((AtlasToken *)LT(1))->getLine();
		int col =  ((AtlasToken *)LT(1))->col;

		syntaxErrCount++;

		cerr	<<	"(" 		<< m_modelfile << ")"
			<<	"line "	<< line 
			<<	" col "	<< col
			<<	" syntax error at " 
			<<	LT(1)->getText();

		if (!etok && !eset) {
			cerr <<  endl;
			return;
		}
		
		if (k == 1) {
			cerr <<  " missing ";
		} else {
			cerr	<< ";"
				<<	LT(1)->getText()
				<<   " not" ;
			if (set_deg(eset) > 1){
				cerr << " in";
			}
		}
		if (set_deg(eset) > 0) {
			edecode(eset);
		} else {
			cerr <<  token_tbl[etok];
		}

		if (strlen(egroup) > 0){
			cerr << " in " <<  egroup;
		}
		cerr << endl ;
		set_error_flag();
	}

void
TedlParser::edecode(SetWordType *a)
{
	SetWordType *p = a;
	SetWordType *endp = &(p[bsetsize]);
	unsigned e = 0;

	if ( set_deg(a)>1 ){
		cerr << " {";
	}
	do {
		SetWordType t = *p;
		SetWordType *b = &(bitmask[0]);
		do {
			if ( t & *b ){
				cerr << " " << token_tbl[e];
			}
			e++;
		} while (++b < &(bitmask[sizeof(SetWordType)*8]));
	} while (++p < endp);
	if ( set_deg(a)>1 ){
		 cerr <<  " }";
	}
}


////////////////////////////////
void TedlParser::set_error_flag()
        {
                ErrorFlag = 1;
        }

void TedlParser::clear_error_flag()
        {
                ErrorFlag = 0;
        }
        
int TedlParser::_error_()
        {
                return ErrorFlag;
        }

void    TedlParser::Error_Report( RWCString E )
        {
		m_last_error=E;
                cerr 	<< "ERROR(" << m_modelfile << "):"
                	<< E << endl;
                set_error_flag();
        };

const RWCString& TedlParser::getLastError()const
{
	return m_last_error;
}

void    TedlParser::Error_Report( RWCString E, int l )
        {
                cerr << "Line:" << l << " ";
                Error_Report( E );
        };

void    TedlParser::Error_Report( RWCString E, ANTLRTokenPtr t )
        {
                if ( t == 0 ){
                
                        Error_Report( E );
                }else{
                        Error_Report( E +":"+ t->getText(), t->getLine() );
                }
        };

void    TedlParser::Error_Report( RWCString E, AST * a )
        {
		if ( a ){
			if(a->getToken()==0){
				Error_Report( E + " " + a->getName());
			} else {
				Error_Report( E , a->getToken());
			}
		}else{
			Error_Report( E );
		}
       };

NounEntry *
TedlParser::theNounEntry(const ANTLRTokenPtr  nid,int & howmany)
{
	NounEntry * nounEntry=0;int j;
	RWCString noun=nid->getText();
	howmany=1;
	while(1){
		if(nounEntry=mnemonicsDB->theNounEntry(noun)){
			break;
		} else if(howmany>=8){	// arbitrary
			break;
		}
		noun+= RWCString(" ") + LT(++howmany)->getText();
	}
	
	if(nounEntry){
		return nounEntry;
	} else {
		howmany=0;
		return 0;
	}
}


ModifierEntry *
TedlParser::argModifier(NounEntry * n, ANTLRTokenPtr t )
	{
			ModifierEntry * modifierEntry=0;
			RWCString left;
			RWCString value=t->getText();
			if(value(0,1)=="."){
				value.replace(0,1,"");
			}
			value.toUpper();
			if(modifierEntry = mnemonicsDB->theModifierEntry(value,n,left)){
				if(!(left=="")){
					modifierEntry=modifierEntry->clone();
					modifierEntry->modifier+=left;
				}
			}
			return modifierEntry;
	
	}
