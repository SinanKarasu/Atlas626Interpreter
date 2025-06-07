#ifndef Scope_h
#define Scope_h

// Scope routines... (Not Oscilloscope. Scope like in ATLAS/Pascal).

#include	"Types.h"

#include	"Dictionary.h"
#include	"astream.h"

class AtlasParser;

class Scope{
public:
	Scope( Scope * prev = NULL );
	virtual ~Scope();
	
	//Scope *	clone( Scope * prev = 0 ) const;
	virtual	Scope			*	getPrev() const;
	virtual	int				pursuePrint() const;
		
	virtual	void				setParameterMode( int mode )			;
	virtual	int				isParameterMode () const			;

	virtual	RWCString 			getFile()					;
	//virtual	RWCString 		getName()					;
	virtual	void				setCurrentLine(const int n)			;
	virtual	int				getCurrentLine()				;
	virtual	void				setDeltaLine(const int n)			;
	virtual	int				getDeltaLine()					;
	virtual	void				setCurrentFstatno(Fstatno * n)			;
	virtual	Fstatno 		*	getCurrentFstatno()				;
	virtual	void				setCallChain(Scope * _cc)			;
	virtual	Scope			*	getCallChain()					;
	virtual	ASTList			*	getASTList()					;

	virtual	AST			*	findSymbolValue(const RWCString sym) const	;
	virtual	AST			*	removeSymbolValue(const RWCString sym)		;
	virtual	AST			*	insertSymbolValue(AST * ast)			;
	virtual	AST			*	insertSymbolValue(const RWCString sym,AST * ast);
	virtual	AST			*	findEventIndicator(const RWCString sym)	const	;
	virtual	AST			*	insertEventIndicator(AST * ast)			;
	virtual	AST			*	removeEventIndicator(const RWCString sym)	;
	virtual	FstatnoDictionary	*	getFstatnoDictionary()				;
	virtual	AtlasParser		*	getAtlasParser()				;

	virtual	AST			*	insertNonAtlasSymbol(AST * ast)			;		
	virtual	AST			*	findNonAtlasSymbol(const RWCString sym) const	;		
	virtual AST			*	get_label(AST * E);
	virtual	AST			*	insert_label	(AST * E,Scope * at=0);
	virtual	AST			*	insert_eilabel	(AST * E,Scope * at=0);
	virtual	AST			*	insert_nonatlas	(AST * E,Scope * at=0);
protected:
	//virtual	ostream&        operator<<( ostream& s );
	friend std::ostream & operator << ( std::ostream & output , Scope * );
	friend astream& operator<<(astream& out, Scope* s);  // <== add this

private:
	Scope *			m_previous;
};


class InnerScope : public Scope{
public:
	InnerScope(Scope * prev = 0);
	virtual	void				setParameterMode( int mode )			;
	virtual	int				isParameterMode () const			;
	virtual	void				setCurrentLine(const int n)			;
	virtual	int				getCurrentLine()				;
	virtual	void				setDeltaLine(const int n)			;
	virtual	int				getDeltaLine()					;
	virtual	void				setCurrentFstatno(Fstatno * n)			;
	virtual	Fstatno 		*	getCurrentFstatno()				;
	virtual	void				setCallChain(Scope * _cc)			;
	virtual	Scope			*	getCallChain()					;
	virtual	AST			*	findSymbolValue(const RWCString sym) const	;		
	virtual	AST			*	removeSymbolValue(const RWCString sym)		;		
	virtual	AST			*	insertSymbolValue(AST * ast)			;		
	virtual	AST			*	insertSymbolValue(const RWCString sym,AST * ast);		
	virtual	AST			*	findEventIndicator(const RWCString sym)	const	;		
	virtual	AST			*	insertEventIndicator(AST * ast)			;		
	virtual	AST			*	removeEventIndicator(const RWCString sym)	;		
	virtual AST			*	get_label(AST * E);
	virtual	AST			*	insert_label	(AST * E,Scope * at=0);
	virtual	AST			*	insert_eilabel	(AST * E,Scope * at=0);
	virtual	AST			*	insert_nonatlas	(AST * E,Scope * at=0);

protected:
	//virtual	ostream&        operator<<( ostream& s );
protected:
	SymbolDictionary* 	m_symbol;
	SymbolDictionary* 	m_eventindicators;
	int			m_CurrentLine;
	int			m_DeltaLine;
	Fstatno 	*	m_CurrentFstatno;
	Scope 		*	m_CallChain;
	int			m_ParameterMode;
};

class RecordScope : public Scope{
public:
	RecordScope( Scope * prev ,ASTList * astList=0);
	virtual	ASTList			*	getASTList();
	virtual	AST			*	findSymbolValue(const RWCString sym) const		;		
	virtual	AST			*	removeSymbolValue(const RWCString sym)			;		
	virtual	AST			*	insertSymbolValue(AST * ast)			;		
	virtual	AST			*	insertSymbolValue(const RWCString sym,AST * ast)	;		
private:
	ASTList * m_astList;
};



class MajorScope : public InnerScope{
public:
	virtual ~MajorScope();

	virtual	RWCString 			getFile()					;
	virtual	FstatnoDictionary	*	getFstatnoDictionary()				;
	virtual	AtlasParser		*	getAtlasParser()				;		
protected:
	MajorScope(RWCString file,Scope * prev,AtlasParser *p);
	FstatnoDictionary	*	m_FstatnoDictionary;
	RWCString 			m_file;
	AtlasParser		*	m_p;

};

class ProgramScope : public MajorScope{
public:
	ProgramScope(RWCString file,Scope * prev,AtlasParser *p);
	virtual ~ProgramScope();

};

class ModuleScope : public MajorScope{
public:
	ModuleScope(RWCString file,Scope * prev,AtlasParser *p);
	virtual ~ModuleScope();

};


class GlobalScope : public InnerScope{
public:
	GlobalScope(Scope * prev = 0);
	virtual	AST			*	insert_label	(AST * E,Scope * at=0);
	virtual	AST			*	insert_eilabel	(AST * E,Scope * at=0);
//	virtual	AST			*	insert_nonatlas	(AST * E,Scope * at=0);
	virtual AST			*	get_label(AST * E);

};


class BlockScope : public InnerScope{
public:
	BlockScope(Scope * prev,AST * block);
	virtual	AST			*	insert_label	(AST * E,Scope * at=0);
	virtual	AST			*	insert_eilabel	(AST * E,Scope * at=0);
//	virtual	AST			*	insert_nonatlas	(AST * E,Scope * at=0);
	virtual	int				pursuePrint() const;

protected:
	//virtual	ostream&        operator<<( ostream& s );
	AST *  m_block;
};

class FunctionScope : public InnerScope{
public:
	FunctionScope (Scope * prev = 0);

};

class ProcedureScope : public InnerScope{
public:
	ProcedureScope (Scope * prev = 0);

};

class NonAtlasFunctionScope : public InnerScope{
public:
	NonAtlasFunctionScope (Scope * prev = 0);

};

class NonAtlasProcedureScope : public InnerScope{
public:
	NonAtlasProcedureScope (Scope * prev = 0);

};

class ExternalScope : public InnerScope{
public:
	ExternalScope(Scope * prev = 0);
	virtual	AST			*	insertNonAtlasSymbol(AST * ast)			;
	virtual	AST			*	findNonAtlasSymbol(const RWCString sym) const	;
	virtual	AST			*	insert_label	(AST * E,Scope * at=0);
	virtual	AST			*	insert_eilabel	(AST * E,Scope * at=0);
	virtual	AST			*	insert_nonatlas	(AST * E,Scope * at=0);
	virtual AST			*	get_label(AST * E);
private:
	SymbolDictionary	* 	m_nonAtlasSymbol;

};

#endif		//  Scope_h
