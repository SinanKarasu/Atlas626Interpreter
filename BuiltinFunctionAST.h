#ifndef BuiltinFunctionAST_h
#define BuiltinFunctionAST_h

#include	"AtlasStd.h"
#include	"AtlasAST.h"

class  BuiltinFunctionAST:public AST {
    public:
	BuiltinFunctionAST( ANTLRTokenPtr p=0 );
	virtual ~BuiltinFunctionAST();

	virtual AST *	eval	( AST * a=0 );
	virtual TheType	getType	( AST * a=0 ) const;
	virtual AST *	Accept	( ASTVisitor & );

    protected:
	AST * ast;
	
	double rad(double x);
	double deg(double x);
};


class  EOFFunction : public BuiltinFunctionAST {
    public:
	EOFFunction();
	virtual AST *	eval	( AST * a=0 );
	virtual TheType	getType	( AST * a=0 ) const;
};


class  ODDFunction : public BuiltinFunctionAST {
  public:
	ODDFunction();
	virtual AST *	eval	( AST * a=0 );
	virtual TheType	getType	( AST * a=0 ) const;
};

//------------PEFunction----------------------
class  PEFunction : public BuiltinFunctionAST {
    public:
	PEFunction();
	virtual AST *	eval	( AST * a=0 );
	virtual TheType	getType	( AST * a=0 ) const;
};


//------------POFunction----------------------
class  POFunction : public BuiltinFunctionAST {
    public:
	POFunction();
	virtual AST *	eval	( AST * a=0 );
	virtual TheType	getType	( AST * a=0 ) const;
};


class  ORDFunction : public BuiltinFunctionAST {
    public:
	ORDFunction();
	virtual AST *	eval	( AST * a=0 );
	virtual TheType	getType	( AST * a=0 ) const;
};

class  LENFunction : public BuiltinFunctionAST {
    public:
	LENFunction();
	virtual AST *	eval	( AST * a=0 );
	virtual TheType	getType	( AST * a=0 ) const;
};

class  LOCNFunction : public BuiltinFunctionAST {
    public:
	LOCNFunction();
	virtual AST *	eval	( AST * a=0 );
	virtual TheType	getType	( AST * a=0 ) const;
};

class  COUNTFunction : public BuiltinFunctionAST {
    public:
	COUNTFunction();
	virtual AST *	eval	( AST * a=0 );
	virtual TheType	getType	( AST * a=0 ) const;
};

class  SIZEFunction : public BuiltinFunctionAST {
    public:
	SIZEFunction();
	virtual AST *	eval	( AST * a=0 );
	virtual TheType	getType	( AST * a=0 ) const;
};

class  SUCCFunction : public BuiltinFunctionAST {
    public:
	SUCCFunction();
	virtual AST *	eval	( AST * a=0 );
	virtual TheType	getType	( AST * a=0 ) const;
};

class  PREDFunction : public BuiltinFunctionAST {
    public:
	PREDFunction();
	virtual AST *	eval	( AST * a=0 );
	virtual TheType	getType	( AST * a=0 ) const;
};

class  DATEFunction : public BuiltinFunctionAST {
    public:
	DATEFunction();
	virtual AST *	eval	( AST * a=0 );
	virtual TheType	getType	( AST * a=0 ) const;
};

class  CHARFunction : public BuiltinFunctionAST {
    public:
	CHARFunction();
	virtual AST *	eval	( AST * a=0 );
	virtual TheType	getType	( AST * a=0 ) const;
};

class  BITSFunction : public BuiltinFunctionAST {
    public:
	BITSFunction();
	virtual AST *	eval	( AST * a=0 );
	virtual TheType	getType	( AST * a=0 ) const;
};

class  COPYstocFunction : public BuiltinFunctionAST {
    public:
	COPYstocFunction();
	virtual AST *	eval	( AST * a=0 );
	virtual TheType	getType	( AST * a=0 ) const;
};

class  COPYstobFunction : public BuiltinFunctionAST {
    public:
	COPYstobFunction();
	virtual AST *	eval	( AST * a=0 );
	virtual TheType	getType	( AST * a=0 ) const;
};

class  ROTATEFunction : public BuiltinFunctionAST {
    public:
	ROTATEFunction();
	virtual AST *	eval	( AST * a=0 );
	virtual TheType	getType	( AST * a=0 ) const;
};

class  SHIFTFunction : public BuiltinFunctionAST {
    public:
	SHIFTFunction();
	virtual AST *	eval	( AST * a=0 );
	virtual TheType	getType	( AST * a=0 ) const;
};

class  A_SHIFTFunction : public BuiltinFunctionAST {
    public:
	A_SHIFTFunction();
	virtual AST *	eval	( AST * a=0 );
	virtual TheType	getType	( AST * a=0 ) const;
};

class  DIGFunction : public BuiltinFunctionAST {
    public:
	DIGFunction();
	virtual AST *	eval	( AST * a=0 );
	virtual TheType	getType	( AST * a=0 ) const;
};

class  DECFunction : public BuiltinFunctionAST {
    public:
	DECFunction();
	virtual AST *	eval	( AST * a=0 );
	virtual TheType	getType	( AST * a=0 ) const;
};

class  INTFunction : public BuiltinFunctionAST {
    public:
	INTFunction();
	virtual AST *	eval	( AST * a=0 );
	virtual TheType	getType	( AST * a=0 ) const;
};

class  ROUNDFunction : public BuiltinFunctionAST {
    public:
	ROUNDFunction();
	virtual AST *	eval	( AST * a=0 );
	virtual TheType	getType	( AST * a=0 ) const;
};

class  ABSFunction : public BuiltinFunctionAST {
    public:
	ABSFunction();
	virtual AST *	eval	( AST * a=0 );
	virtual TheType	getType	( AST * a=0 ) const;
};

class  SINFunction : public BuiltinFunctionAST {
    public:
	SINFunction();
	virtual AST *	eval	( AST * a=0 );
	virtual TheType	getType	( AST * a=0 ) const;
};

class  COSFunction : public BuiltinFunctionAST {
    public:
	COSFunction();
	virtual AST *	eval	( AST * a=0 );
	virtual TheType	getType	( AST * a=0 ) const;
};

class  TANFunction : public BuiltinFunctionAST {
    public:
	TANFunction();
	virtual AST *	eval	( AST * a=0 );
	virtual TheType	getType	( AST * a=0 ) const;
};

class  LNFunction : public BuiltinFunctionAST {
    public:
	LNFunction();
	virtual AST *	eval	( AST * a=0 );
	virtual TheType	getType	( AST * a=0 ) const;
};

class  LOGFunction : public BuiltinFunctionAST {
    public:
	LOGFunction();
	virtual AST *	eval	( AST * a=0 );
	virtual TheType	getType	( AST * a=0 ) const;
};

class  ALOGFunction : public BuiltinFunctionAST {
    public:
	ALOGFunction();
	virtual AST *	eval	( AST * a=0 );
	virtual TheType	getType	( AST * a=0 ) const;
};

class  ATANFunction : public BuiltinFunctionAST {
    public:
	ATANFunction();
	virtual AST *	eval	( AST * a=0 );
	virtual TheType	getType	( AST * a=0 ) const;
};

class  EXPFunction : public BuiltinFunctionAST {
    public:
	EXPFunction();
	virtual AST *	eval	( AST * a=0 );
	virtual TheType	getType	( AST * a=0 ) const;
};

class  SQRTFunction : public BuiltinFunctionAST {
    public:
	SQRTFunction();
	virtual AST *	eval	( AST * a=0 );
	virtual TheType	getType	( AST * a=0 ) const;
};

class  DELETEstocFunction : public BuiltinFunctionAST {
    public:
	DELETEstocFunction();
	virtual AST *	eval	( AST * a=0 );
	virtual TheType	getType	( AST * a=0 ) const;
};

class  DELETEstobFunction : public BuiltinFunctionAST {
    public:
	DELETEstobFunction();
	virtual AST *	eval	( AST * a=0 );
	virtual TheType	getType	( AST * a=0 ) const;
};

class  INSERTstocFunction : public BuiltinFunctionAST {
    public:
	INSERTstocFunction();
	virtual AST *	eval	( AST * a=0 );
	virtual TheType	getType	( AST * a=0 ) const;
};
class  INSERTstobFunction : public BuiltinFunctionAST {
    public:
	INSERTstobFunction();
	virtual AST *	eval	( AST * a=0 );
	virtual TheType	getType	( AST * a=0 ) const;
};


#endif // BuiltinFunctionAST_h
