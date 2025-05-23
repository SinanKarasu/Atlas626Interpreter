#ifndef GateConnEventAST_h
#define GateConnEventAST_h

//#include	"AtlasStd.h"
#include	"AtlasAST.h"

class	GateConnEventAST : public AST{
	public:
		GateConnEventAST( ANTLRTokenPtr p=0 );
		virtual	~GateConnEventAST();
	
		virtual AST *	eval	( AST * a=0 );
		virtual AST *	assign	( AST * a   );
		virtual AST *	check	( AST * a=0 );
		virtual AST *	Accept	( ASTVisitor & );
		virtual astream&	operator<<	( astream& s );

	private:
};	

class	PinDescriptor : public GateConnEventAST{
	public:
		PinDescriptor( ANTLRTokenPtr p=0 );
		PinDescriptor( AST * p,const RWCString pre);
		virtual AST *	eval	( AST * a=0 );
		virtual RWCString  	getName	() const;
		virtual astream&	operator<<	( astream& s );

	private:
	RWCString m_prefix;
	AST * m_ast;
};

class	PreDefinedPinDescriptor : public GateConnEventAST{
	public:
		PreDefinedPinDescriptor( ANTLRTokenPtr p=0 );
		virtual AST *	eval	( AST * a=0 );
		PreDefinedPinDescriptor( AST * p);
		
	private:
	AST * m_ast;
};

class	ConnDescriptor : public GateConnEventAST{
	public:
		ConnDescriptor( ANTLRTokenPtr p=0 );
		virtual AST *	eval	( AST * a=0 );
		virtual RWCString  	getName	() const;
		virtual astream&	operator<<	( astream& s );
		
	private:
};

class	ReferenceConn : public GateConnEventAST{
	public:
		ReferenceConn( ANTLRTokenPtr p=0 );
		virtual AST *	eval	( AST * a=0 );
		
	private:
};

class	ConnectSetRef : public GateConnEventAST{
	public:
		ConnectSetRef( ANTLRTokenPtr p=0 );
		virtual AST *	eval	( AST * a=0 );
		
	private:
};

class	GateField : public GateConnEventAST{
	public:
		GateField( ANTLRTokenPtr p=0 );
		virtual AST *	eval	( AST * a=0 );
		virtual astream&	operator<<	( astream& s );

	private:
};

class	EventField : public GateConnEventAST{
	public:
		EventField( ANTLRTokenPtr p=0 );
		virtual AST *	eval	( AST * a=0 );
		virtual astream&	operator<<	( astream& s );

	private:
};

class	CnxField : public GateConnEventAST{
	public:
		CnxField( ANTLRTokenPtr p=0 );
		virtual AST *	eval	( AST * a=0 );
		virtual astream&	operator<<	( astream& s );

	private:
};

class	MaxTime : public GateConnEventAST{
	public:
		MaxTime( ANTLRTokenPtr p=0 );
		virtual AST *	eval	( AST * a=0 );
		virtual astream&	operator<<	( astream& s );

	private:
};

#endif // GateConnEventAST_h
