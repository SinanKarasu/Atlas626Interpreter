
#include	"Scope.h"
#include	"LabelAST.h"
#include	"ActionAST.h"

#include	"ExecEnv.h"

extern	ExecEnv	execEnv;

// --------------------- Scope -------------------------------------------------------
Scope::Scope( Scope * prev )
	{
		m_previous = prev;
	}

Scope::~Scope()
	{
		//delete m_eventindicators;
		//delete m_symbol;
	}

void
Scope::setCurrentLine(const int n)
	{
		assert(0);
		//m_CurrentLine=n;
	}

int
Scope::getCurrentLine()
	{
		assert(0);
		return 0;
	}

void
Scope::setDeltaLine(const int n)
	{
		//m_DeltaLine=n;
	}

int
Scope::getDeltaLine()
	{
		//return m_DeltaLine;
		assert(0);
		return 0;
	}

AST *
Scope::insert_label(AST * E,Scope * at)
	{
		assert(0);
		return 0;
	}


AST *
Scope::insert_nonatlas(AST* nam,Scope * at)
	{
		assert(0);
		return 0;
	}
AST *
Scope::insert_eilabel(AST * E,Scope * at)
	{
		assert(0);
		return 0;
	}
AST *
Scope::get_label(AST * E)
	{
		assert(0);
		return 0;
	}



void
Scope::setCurrentFstatno(Fstatno * n)
	{
		//m_CurrentFstatno=n;
	}

Fstatno *
Scope::getCurrentFstatno()
	{
		//return m_CurrentFstatno;
		assert(0);
		return 0;
	}

void
Scope::setCallChain(Scope * _cc)
	{
		assert(0);
		//m_CallChain=_cc;
	}

Scope *
Scope::getCallChain()
	{
		return 0;
	}

Scope *	Scope::getPrev() const	{ return m_previous; }
		
void	Scope::setParameterMode( int mode )	{ assert(0); /* m_ParameterMode = mode;*/ }
int	Scope::isParameterMode () const		{ assert(0);return 0; /* return m_ParameterMode;*/ }

RWCString
Scope::getFile()
	{
		return getPrev()->getFile();
	}


ASTList *
Scope::getASTList()
	{
		assert(0);
		return 0;
	}

AST	*
Scope::findSymbolValue(const RWCString sym) const
	{
		assert(0);
		return 0;
	}

AST	*
Scope::removeSymbolValue(const RWCString sym)		
	{
		assert(0);
		return 0;
	}


AST	*
Scope::insertSymbolValue(AST * ast)		
	{
		assert(0);
		return 0;
	}


AST	*
Scope::insertSymbolValue(const RWCString sym,AST * ast)		
	{
		assert(0);
		return 0;
	}


AST	*
Scope::findEventIndicator(const RWCString sym)	const		
	{
		assert(0);
		return 0;
	}


AST	*
Scope::removeEventIndicator(const RWCString sym)	
	{
		assert(0);
		return 0;
	}


AST	*
Scope::insertEventIndicator(AST * ast)
	{
		assert(0);
		return 0;
	}

FstatnoDictionary *
Scope::getFstatnoDictionary()
	{
		return getPrev()->getFstatnoDictionary();
	}

AtlasParser *
Scope::getAtlasParser()
	{
		if(getPrev()){
			return getPrev()->getAtlasParser();
		} else {
			return 0;
		}
	}
	
AST	*
Scope::findNonAtlasSymbol(const RWCString sym) const
	{
		assert(0);
		return 0;
	}



AST	*
Scope::insertNonAtlasSymbol(AST * ast)		
	{
		assert(0);
		return ast;
	}

int
Scope::pursuePrint() const
	{
		return	(getPrev()?1:0);
	}

// --------------------- MajorScope -------------------------------------------------------
RecordScope::RecordScope(Scope * prev , ASTList * astList)
	:Scope(prev)
		{
			if(astList){
				m_astList=astList;
			} else {
				m_astList=new ASTList;
			}
		}
ASTList *
RecordScope::getASTList()
	{
		return m_astList;
	}

AST	*
RecordScope::findSymbolValue(const RWCString sym) const
	{
		if(m_astList){
			AST * x=0;
			m_astList->findValue(sym,x);
			return x;
		}
		return 0;
	}

AST	*
RecordScope::removeSymbolValue(const RWCString sym)		
	{
		if(m_astList){
			AST * x=0;
			m_astList->findValue(sym,x);
			if(x){
				m_astList->remove(x);
			}
			return x;
		}
		return 0;
	}


AST	*
RecordScope::insertSymbolValue(AST * ast)		
	{
		insertSymbolValue(ast->getName(),ast);
		return (AST *)ast;
	}


AST	*
RecordScope::insertSymbolValue(const RWCString sym,AST * ast)		
	{
		AST * x=(AST *)ast;
		if(!m_astList){
			m_astList=new ASTList;
		}
		if(!findSymbolValue(sym)){
			m_astList->append(ast);
			return x;
		} else {
			return 0;
		}
	}


// --------------------- InnerScope -------------------------------------------------------

InnerScope::InnerScope(Scope * prev)
	:Scope(prev)
	,m_ParameterMode(0)
	,m_eventindicators(0)
	,m_CurrentLine(0)
	,m_CurrentFstatno(0)
	,m_DeltaLine(0)
	,m_CallChain(0)
	,m_symbol(0)
	{
	}

void
InnerScope::setCurrentLine(const int n)
	{
		m_CurrentLine=n;
	}

int
InnerScope::getCurrentLine()
	{
		return m_CurrentLine;
	}

void
InnerScope::setDeltaLine(const int n)
	{
		m_DeltaLine=n;
	}

int
InnerScope::getDeltaLine()
	{
		return m_DeltaLine;
	}

void
InnerScope::setCurrentFstatno(Fstatno * n)
	{
		m_CurrentFstatno=n;
	}

Fstatno *
InnerScope::getCurrentFstatno()
	{
		return m_CurrentFstatno;
	}

void
InnerScope::setCallChain(Scope * _cc)
	{
		AtlasParser * p=getAtlasParser();
		if(_cc=this){// we arbitrate for best results
			if(p){
				Scope * s=p->getScope();
				if(s==this){	// nothing to do
				} else if(s){
					m_CallChain=s;
				}
			}
		} else {		
			m_CallChain=_cc;
		}
		if(p){
			p->setScope(this);
		}
	}

Scope *
InnerScope::getCallChain()
	{
		return m_CallChain;
	}
		
void
InnerScope::setParameterMode( int mode )
	{
		m_ParameterMode = mode;
	}

int
InnerScope::isParameterMode () const
	{
		return m_ParameterMode;
	}

AST	*
InnerScope::findSymbolValue(const RWCString sym) const
	{
		if(m_symbol){
			AST * x=0;
			m_symbol->findValue(sym,x);
			return x;
		}
		return 0;
	}

AST	*
InnerScope::removeSymbolValue(const RWCString sym)		
	{
		if(m_symbol){
			AST * x=0;
			m_symbol->findValue(sym,x);
			if(x){
				m_symbol->erase(sym);
			}
			return x;
		}
		return 0;
	}


AST	*
InnerScope::insertSymbolValue(AST * ast)		
	{
		insertSymbolValue(ast->getName(),ast);
		return (AST *)ast;
	}


AST	*
InnerScope::insertSymbolValue(const RWCString sym,AST * ast)		
	{
		AST * x=(AST *)ast;
		if(!m_symbol){
			m_symbol=new SymbolDictionary;
		}
		m_symbol->insertKeyAndValue(sym,x);
		return (AST *)ast;
	}


AST	*
InnerScope::findEventIndicator(const RWCString sym)	const		
	{
		AST * x=0;
		if(m_eventindicators){
			m_eventindicators->findValue(sym,x);
			return x;
		} else {
			return 0;
		}
	}


AST	*
InnerScope::removeEventIndicator(const RWCString sym)	
	{
		if(m_eventindicators){
			AST * x=0;
			m_eventindicators->findValue(sym,x);
			if(x){
				m_eventindicators->erase(sym);
			}
			return x;
		}
		return 0;
	}


AST	*
InnerScope::insertEventIndicator(AST * ast)
	{
		AST * x=0;
		if(!m_eventindicators){
			m_eventindicators=new SymbolDictionary;
		}
		m_eventindicators->insertKeyAndValue(ast->getName(),ast);
		return ast;
	}


AST *
InnerScope::insert_label(AST * E,Scope * at)
	{
		AST * x=0;

		if(at && (x=at->get_label(E)) ){
			return 0;
		} else if (!(x=insertSymbolValue( E ) )){
			return 0;
		}	
		return x;
	}


AST *
InnerScope::insert_nonatlas(AST* nam,Scope * at)
	{
		return getPrev()->insert_nonatlas(nam);
	}


// --------------------- MajorScope -------------------------------------------------------
MajorScope::MajorScope( RWCString file,Scope * prev ,AtlasParser * p)
	:InnerScope(prev)
	,m_file(file)
	,m_FstatnoDictionary(0)
	,m_p(p)
	{
		m_symbol=new SymbolDictionary;
		m_FstatnoDictionary=new FstatnoDictionary;
	}

MajorScope::~MajorScope()
	{
		delete m_eventindicators;
		delete m_symbol;
	}

RWCString
MajorScope::getFile()
	{
		return m_file;
	}

FstatnoDictionary *
MajorScope::getFstatnoDictionary()
	{
		return m_FstatnoDictionary;
	}

AtlasParser *
MajorScope::getAtlasParser()
	{
		return m_p;
	}

// --------------------- ProgramScope -------------------------------------------------------
ProgramScope::ProgramScope( RWCString file,Scope * prev ,AtlasParser * p)
	:MajorScope(file,prev,p)
	{
	}

ProgramScope::~ProgramScope()
	{
	}
// --------------------- ModuleScope -------------------------------------------------------
ModuleScope::ModuleScope( RWCString file,Scope * prev ,AtlasParser * p)
	:MajorScope(file,prev,p)
	{
	}


ModuleScope::~ModuleScope()
	{
	}
// --------------------- GlobalScope -------------------------------------------------------

GlobalScope::GlobalScope(Scope * prev)
	:InnerScope(prev)
	{
	
	}

// --------------------- FunctionScope -------------------------------------------------------

FunctionScope::FunctionScope(Scope * prev)
	:InnerScope(prev)
	{
	
	}

// --------------------- ProcedureScope -------------------------------------------------------

ProcedureScope::ProcedureScope(Scope * prev)
	:InnerScope(prev)
	{
	
	}

// --------------------- NonAtlasFunctionScope -------------------------------------------------------

NonAtlasFunctionScope::NonAtlasFunctionScope(Scope * prev)
	:InnerScope(prev)
	{
	
	}

// --------------------- NonAtlasProcedureScope -------------------------------------------------------

NonAtlasProcedureScope::NonAtlasProcedureScope(Scope * prev)
	:InnerScope(prev)
	{
	
	}

// --------------------- InnerScope -------------------------------------------------------

ExternalScope::ExternalScope(Scope * prev)
	:InnerScope(prev)
	,m_nonAtlasSymbol(0)
	{
	}


AST	*
ExternalScope::findNonAtlasSymbol(const RWCString sym) const
	{
		if(m_nonAtlasSymbol){
			AST * x=0;
			m_nonAtlasSymbol->findValue(sym,x);
			return x;
		}
		return 0;
	}



AST	*
ExternalScope::insertNonAtlasSymbol(AST * ast)		
	{
		AST * x=ast;
		if(!m_nonAtlasSymbol){
			m_nonAtlasSymbol=new SymbolDictionary;
		} else if(findNonAtlasSymbol(ast->getName())){
			return 0;
		}
		m_nonAtlasSymbol->insertKeyAndValue(ast->getName(),ast);
		return ast;
	}



AST *
ExternalScope::insert_label(AST * E,Scope * at)
	{
		AST * x=0;
		if(!(x=InnerScope::insert_label(E,at))){
			Error_Report( " SYMBOL is already defined :", E );
		}
		return x;
	}


AST *
ExternalScope::insert_nonatlas(AST* nam,Scope * at)
	{
		insertNonAtlasSymbol(nam);
		return nam;
	}

AST *
GlobalScope::insert_label(AST * E,Scope * at)
	{
		AST * x=0;
		if(!(x=InnerScope::insert_label(E,at))){
			Error_Report( " SYMBOL is already defined :", E );
		}
		return x;

	}


BlockScope::BlockScope(Scope * prev,AST * block)
	:InnerScope(prev)
	,m_block(block)
	{
	}

AST *
BlockScope::insert_label(AST * E,Scope * at)
	{
		AST * x=0;
		if(!(x=InnerScope::insert_label(E,at))){
			Error_Report( " SYMBOL is already defined :", E );
		}
		return x;
	}
int
BlockScope::pursuePrint() const
	{
		return	0;
	}

AST *
InnerScope::insert_eilabel(AST * E,Scope * at)
	{

		Scope * search=at?at:this;
		
		if ( search->findEventIndicator( E->getName()) ){
			Error_Report( RWCString( E->getName()) + " is already defined", E );
		} else {
			AST * x=search->findSymbolValue( E->getName());
			if ( !x ){
				Error_Report( RWCString( E->getName()) + " Was NOT defined as BOOLEAN", E );
			} else if(!(x->getType()==BooleanTypeValue)){
				Error_Report( RWCString( E->getName()) + " must be BOOLEAN", E );
			} else {
				return insertEventIndicator( E );
			}
		}	
		return 0;
	}




AST *
ExternalScope::insert_eilabel(AST * E,Scope * at)
	{
		return	InnerScope::insert_eilabel(E,at);
	}



AST *
GlobalScope::insert_eilabel(AST * E,Scope * at)
	{
		return	InnerScope::insert_eilabel(E,at);
	}



AST *
BlockScope::insert_eilabel(AST * E,Scope * at)
	{
		return	InnerScope::insert_eilabel(E,at);
	}

AST *
ExternalScope::get_label(AST * E)
	{
		AST            *x = 0;


		if (x=getPrev()->findSymbolValue(E->getName())) {
				if (x->check(E)) {
					removeSymbolValue(x->getName());
					delete          E;

					return x;
				}
				Error_Report("Global and External types differ:", E);
				return 0;
		}
		return findSymbolValue(E->getName());
		
	}

AST *
GlobalScope::get_label(AST * E)
	{
		return	findSymbolValue(E->getName());
	}


AST *
InnerScope::get_label(AST * E)
	{
		AST            *x = 0;

		if (x=findSymbolValue(E->getName())) {
			return x;
		} else if(getPrev()){
			return getPrev()->get_label(E);
		}
	}


//ostream&
//Scope::operator<<( ostream& s )
//	{
//		s << getPrev() ;
//		return s;
//	}

//	friend 
std::ostream &	operator << (std::ostream & output,Scope * s)
		{
			if(s->getCurrentFstatno()){
				if(s->pursuePrint()){
					if(s->getCallChain()){
						output << s->getCallChain() ;
					} else if(s->getPrev()) {
						output << s->getPrev() ;
					}
				}
				output << s->getCurrentFstatno() ;
				if(s->getDeltaLine()!=0) {
					output << "(+" << s->getDeltaLine() << ")" ;
				}
				if(execEnv.execTrace()){
					output << "["	<< s->getFile() << "%" << s->getCurrentLine() << "]"<< ":" ;
				}
				output << ":" ;
			}
			return output;
		}

#include "astream.h"  // or wherever astream is defined

astream& operator<<(astream& s, Scope* scope) {
	if (!scope) {
		s << "<null scope>";
		return s;
	}

	if (scope->getCurrentFstatno()) {
		if (scope->pursuePrint()) {
			if (scope->getCallChain()) {
				s << scope->getCallChain();
			} else if (scope->getPrev()) {
				s << scope->getPrev();
			}
		}
		s << scope->getCurrentFstatno();
		if (scope->getDeltaLine() != 0) {
			s << "(+" << scope->getDeltaLine() << ")";
		}
		if (execEnv.execTrace()) {
			s << "[" << scope->getFile() << "%" << scope->getCurrentLine() << "]" << ":";
		}
		s << ":";
	}

	return s;
}

