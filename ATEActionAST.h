#ifndef ATEActionAST_h
#define ATEActionAST_h

#include	"AtlasAST.h"

class	ATEActionAST : public AST{
	public:
		ATEActionAST( ANTLRTokenPtr p=0 );
		virtual	~ATEActionAST();
	
		virtual AST *	eval	( AST * a=0 );
		virtual AST *	assign	( AST * a   );
		virtual AST *	Accept	( ASTVisitor & );

	private:
};	


class	ATERequireAction : public ATEActionAST{
	public:
		ATERequireAction( ANTLRTokenPtr p=0 );

	private:
};


class ATESyncAction : public ATEActionAST {
    public:
	ATESyncAction ( ANTLRTokenPtr p );
	
    private:
};


#endif // ATEActionAST_h
