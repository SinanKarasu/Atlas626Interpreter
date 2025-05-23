#ifndef OperatorAST_h
#define OperatorAST_h

#include	"AtlasAST.h"

class OperatorAST : public AST {
public:
	OperatorAST(ANTLRTokenPtr p=0);
	virtual ~OperatorAST();
	virtual AST *	eval		( AST *  a=0 );
	virtual TheType	getType		( AST *  a=0 ) const ;
	virtual Long	getInteger	( int indx=0 ) const ;
	virtual double	getDecimal	( int indx=0 ) const ;
	virtual AST *	Accept		( ASTVisitor & );
	virtual astream& operator<<( astream& s );
	
public:
	AST *	ast;
        TheType	storage;
};


class ANDOperator : public OperatorAST {
public:
	ANDOperator(ANTLRTokenPtr p=0);
	virtual AST * eval(AST * a=0);
};

class AddOperator : public OperatorAST {
public:
	AddOperator(ANTLRTokenPtr p=0);
	virtual AST * eval(AST * a=0);
};

class ConcatOperator : public OperatorAST {
public:
	ConcatOperator(ANTLRTokenPtr p=0);
	virtual AST * eval(AST * a=0);
};

class DIVOperator : public OperatorAST {
public:
	DIVOperator(ANTLRTokenPtr p=0);
	virtual AST * eval(AST * a=0);
};

class DivideOperator : public OperatorAST {
public:
	DivideOperator(ANTLRTokenPtr p=0);
	virtual AST * eval(AST * a=0);
};

class EQOperator : public OperatorAST {
public:
	EQOperator(ANTLRTokenPtr p=0);
	virtual AST * eval(AST * a=0);
};

class ExpoOperator : public OperatorAST {
public:
	ExpoOperator(ANTLRTokenPtr p=0);
	virtual AST * eval(AST * a=0);
};

class FunctionCallOperator : public OperatorAST {
public:
	FunctionCallOperator(ANTLRTokenPtr p=0);
	virtual AST * eval(AST * a=0);
};

class GEOperator : public OperatorAST {
public:
	GEOperator(ANTLRTokenPtr p=0);
	virtual AST * eval(AST * a=0);
};

class GTOperator : public OperatorAST {
public:
	GTOperator(ANTLRTokenPtr p=0);
	virtual AST * eval(AST * a=0);
};

class IdentityOperator : public OperatorAST {
public:
	IdentityOperator(ANTLRTokenPtr p=0);
	virtual AST * eval(AST * a=0);
};

class LEOperator : public OperatorAST {
public:
	LEOperator(ANTLRTokenPtr p=0);
	virtual AST * eval(AST * a=0);
};

class LTOperator : public OperatorAST {
public:
	LTOperator(ANTLRTokenPtr p=0);
	virtual AST * eval(AST * a=0);
};

class MODOperator : public OperatorAST {
public:
	MODOperator(ANTLRTokenPtr p=0);
	virtual AST * eval(AST * a=0);
};

class NEOperator : public OperatorAST {
public:
	NEOperator(ANTLRTokenPtr p=0);
	virtual AST * eval(AST * a=0);
};

class NegativeOperator : public OperatorAST {
public:
	NegativeOperator(ANTLRTokenPtr p=0);
	virtual AST * eval(AST * a=0);
	virtual astream& operator<<( astream& s );
};

class NOTOperator : public OperatorAST {
public:
	NOTOperator(ANTLRTokenPtr p=0);
	virtual AST * eval(AST * a=0);
};

class OROperator : public OperatorAST {
public:
	OROperator(ANTLRTokenPtr p=0);
	virtual AST * eval(AST * a=0);
};

class SubtractOperator : public OperatorAST {
public:
	SubtractOperator(ANTLRTokenPtr p=0);
	virtual AST * eval(AST * a=0);
};

class TimesOperator : public OperatorAST {
public:
	TimesOperator(ANTLRTokenPtr p=0);
	virtual AST * eval(AST * a=0);
};

class XOROperator : public OperatorAST {
public:
	XOROperator(ANTLRTokenPtr p=0);
	virtual AST * eval(AST * a=0);
};



#endif // OperatorAST_h
