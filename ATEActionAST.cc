
#include	"AtlasStd.h"
#include	"ATEActionAST.h"
#include	"Visitors.h"

ATEActionAST::ATEActionAST( ANTLRTokenPtr p )
	:AST(p)
	{
	}
	
ATEActionAST::~ATEActionAST(){}
	
AST *	ATEActionAST::eval	( AST * a )
	{
		if ( a != 0 )
			return ASTdown()->eval( a );
		else
			return this;
	}
	
AST *	ATEActionAST::assign	( AST * a )		{ return this; }
AST *	ATEActionAST::Accept	( ASTVisitor & v )	{ return v.VisitATEActionAST( this ); }

ATERequireAction::ATERequireAction( ANTLRTokenPtr p )
	:ATEActionAST(p)
	{
	}
