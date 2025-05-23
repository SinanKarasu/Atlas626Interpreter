#ifndef SignalOperatorAST_h
#define SignalOperatorAST_h

#include	"AtlasAST.h"
#include	"BasicTypeAST.h"



class SignalOperatorAST:public AST {
public:
	SignalOperatorAST( ANTLRTokenPtr p=0 );
	virtual	~SignalOperatorAST();
	
	virtual AST	*	eval	( AST * a=0  );
	virtual AST	*	assign	( AST * a    );
	virtual	AST	*	data	( AST *  a = 0 );
	virtual AST	*	Accept		( ASTVisitor & );

	virtual TheType		getType		( AST *  a=0 ) const ;
	virtual Long		getInteger	( int indx=0 ) const ;
	virtual double		getDecimal	( int indx=0 ) const ;
	
public:
	AST *	m_ast;
        TheType	storage;

};	



class ConstraintRange: public SignalOperatorAST {
    public:
	ConstraintRange( ANTLRTokenPtr p=0 );
	virtual AST *		min	( AST *	a = 0 );
	virtual AST *		max	( AST *	a = 0 );
	virtual AST *		check	( AST *	a = 0 );

    private:
};

class SelectLimitMax: public SignalOperatorAST {
    public:
	SelectLimitMax( ANTLRTokenPtr p=0 );
	virtual AST *		min	( AST *	a = 0 );
	virtual AST *		max	( AST *	a = 0 );
	virtual AST *		check	( AST *	a = 0 );

    private:
    	AST * m_ninf;	// negative infinity
};

class SelectLimitMin: public SignalOperatorAST {
    public:
	SelectLimitMin( ANTLRTokenPtr p=0 );
	virtual AST *		min	( AST *	a = 0 );
	virtual AST *		max	( AST *	a = 0 );
	virtual AST *		check	( AST *	a = 0 );

    private:
    	AST * m_pinf;	// positive infinity
};

class ByQuantization: public SignalOperatorAST {
    public:
	ByQuantization( ANTLRTokenPtr p=0 );
	virtual AST *		min	( AST *	a = 0 );
	virtual AST *		max	( AST *	a = 0 );
	virtual AST *		check	( AST *	a = 0 );

    private:
};
class ConstraintMax: public SignalOperatorAST {
    public:
	ConstraintMax( ANTLRTokenPtr p=0 );
	virtual AST *		min	( AST *	a = 0 );
	virtual AST *		max	( AST *	a = 0 );
	virtual AST *		check	( AST *	a = 0 );

    private:
    DecimalNumber * m_min;
};
class ConstraintMin: public SignalOperatorAST {
    public:
	ConstraintMin( ANTLRTokenPtr p=0 );
	virtual AST *		min	( AST *	a = 0 );
	virtual AST *		max	( AST *	a = 0 );
	virtual AST *		check	( AST *	a = 0 );

    private:
    DecimalNumber * m_max;
};
class ConstraintPercent: public SignalOperatorAST {
    public:
	ConstraintPercent( ANTLRTokenPtr p=0 );
	virtual AST *		min	( AST *	a = 0 );
	virtual AST *		max	( AST *	a = 0 );
	virtual AST *		check	( AST *	a = 0 );

    private:
};
class ConstraintArray: public SignalOperatorAST {
    public:
	ConstraintArray( ANTLRTokenPtr p=0 );
	virtual AST *		min	( AST *	a = 0 );
	virtual AST *		max	( AST *	a = 0 );
	virtual AST *		check	( AST *	a = 0 );

    private:
};

class RealErrlim: public SignalOperatorAST {
    public:
	RealErrlim( ANTLRTokenPtr p=0 );
	virtual AST *		min	( AST *	a = 0 );
	virtual AST *		max	( AST *	a = 0 );
	virtual AST *		check	( AST *	a = 0 );

    private:
};

class ANDErrLmt: public SignalOperatorAST {
    public:
	ANDErrLmt( ANTLRTokenPtr p=0 );
	virtual AST *		eval	( AST *	a = 0 );
	virtual AST *		min	( AST *	a = 0 );
	virtual AST *		max	( AST *	a = 0 );

    private:
    AST * m_nom;
    AST * m_max;
    AST * m_min;
};

class ORErrLmt: public SignalOperatorAST {
    public:
	ORErrLmt( ANTLRTokenPtr p=0 );
	virtual AST *		eval	( AST *	a = 0 );
	virtual AST *		min	( AST *	a = 0 );
	virtual AST *		max	( AST *	a = 0 );

    private:
    AST * m_nom;
    AST * m_max;
    AST * m_min;
};


class LLOperator : public SignalOperatorAST {
public:
	LLOperator(ANTLRTokenPtr p=0);
	virtual AST * eval(AST * a=0);
	virtual astream& operator<<( astream& s );
};

class ULOperator : public SignalOperatorAST {
public:
	ULOperator(ANTLRTokenPtr p=0);
	virtual AST * eval(AST * a=0);
	virtual astream& operator<<( astream& s );
};

class NOMOperator : public SignalOperatorAST {
public:
	NOMOperator(ANTLRTokenPtr p=0);
	virtual AST * eval(AST * a=0);
	virtual astream& operator<<( astream& s );
};

class WITHINOperator : public SignalOperatorAST {
public:
	WITHINOperator(ANTLRTokenPtr p=0);
	virtual AST * eval(AST * a=0);
	virtual astream& operator<<( astream& s );
private:
	AST * m_ds;
};

#endif // SignalOperatorAST_h
