
// chat #include	"Dictionary.h"
#include	"AtlasAST.h"
#include	"Dictionary.h"

#include	"BasicTypeAST.h"
#include	"Resource.h"

//                       symbol table support                        //


// Hash function for keys

unsigned LabelHash(const RWCString& str) { return str.hash(); }

unsigned LongHash(const RWInteger& i) { return i /*% 10007*/; }

unsigned long ResourcePtrHash( const Resource * & p ) { return (unsigned long)(p); }

SymbolDictionary::SymbolDictionary():RWTValHashDictionary<RWCString, AST *>(LabelHash)
	{resize(NbrBuckets);}

SymbolDictionaryIterator::SymbolDictionaryIterator( SymbolDictionary &d )
	:RWTValHashDictionaryIterator<RWCString,AST *> (d)
	{
	}

FstatnoDictionary::FstatnoDictionary():RWTValHashDictionary<RWInteger, Fstatno *>(LongHash)
	{resize(NbrBuckets);}

EntryDictionary::EntryDictionary():RWTValHashDictionary< RWInteger,TargetStatement * >(LongHash)
	{resize(NbrBuckets);}

GoToDictionary::GoToDictionary():RWTValHashDictionary< RWInteger,GoToStatementStack * >(LongHash)
	{resize(NbrBuckets);}

GoToDictionaryIterator::GoToDictionaryIterator(GoToDictionary &d)
	:RWTValHashDictionaryIterator< RWInteger , GoToStatementStack * > (d)
	{
	}

DeviceDictionary::DeviceDictionary():RWTValHashDictionary< RWCString, ResourceAST * >(LabelHash)
	{resize(NbrBuckets);}

RWBoolean	ASTList::findValue( RWCString key, AST *& value )
	{
		ASTListIterator		it( *this );
		
		while( ++it ){
			if ( it.key()->getName() == key ){
				value = it.key();
				return TRUE;
			}
		}
		return FALSE;
	}

Capability::Capability(AST * a):m_ast(0)
	{
		m_required	= FALSE;
		m_min		= 0;
		m_max		= 0;
		m_by		= 0;
		m_limit		= 0;
	}

int	Capability::compare( Capability & c )
	{
		if (	(m_nounModifier == c.m_nounModifier)	&&
			(m_max >= c.m_max)			&&
			(m_min <= c.m_min)		){
			//(m_errorLimit.compare( c.m_errorLimit ) == -1) ){
			
			return -1;
		}else{
			return  1;
		}
	}

void		Capability::require	()		{ m_required = TRUE; }
RWBoolean	Capability::required()			{ return m_required; }
void		Capability::setMax	( double d )	{ m_max = d; }
void		Capability::setMin	( double d )	{ m_min = d; }
void		Capability::setNoun	( RWCString c )	{ m_noun = c; }
void		Capability::setModifier	( RWCString c )	{ m_nounModifier = c; }
void		Capability::setAST	( AST * a)	{ m_ast=a;}
AST	*	Capability::getAST	()		{ return m_ast;}


ErrorLimit::ErrorLimit()
	{
		m_percentage	= 0;
		m_min		= 0;
		m_max		= 0;
	}

int	ErrorLimit::compare( ErrorLimit & e )
	{
		if (	(m_nounModifier == e.m_nounModifier)	&&
			(m_max >= e.m_max)			&&
			(m_min <= e.m_min)			&&
			(m_percentage <= e.m_percentage)	    ){
			
			return -1;
		}else{
			return  1;
		}
	}

StringSet::StringSet():RWTValHashSet< RWCString >(LabelHash){}


CapabilityListIterator::CapabilityListIterator( CapabilityList & l )
	:RWTPtrSlistIterator<Capability> (l)
	{
	}

ASTListIterator::ASTListIterator(ASTList &d)
	:RWTValSlistIterator<AST *> (d)
	{
	}


ReverseMapDictionary::ReverseMapDictionary():RWTValHashDictionary<RWCString, ReverseMapEntry *>(LabelHash)
	{resize(NbrBuckets);}

ReverseMapDictionaryIterator::ReverseMapDictionaryIterator( ReverseMapDictionary &d )
	:RWTValHashDictionaryIterator<RWCString,ReverseMapEntry *> (d)
	{
	}
