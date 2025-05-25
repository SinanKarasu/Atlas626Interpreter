#ifndef CodeGenVisitor_h
#define CodeGenVisitor_h

#include	"Visitors.h"
#include	"SignalOperatorAST.h"

class SignalOperatorAST ;
class BasicTypeAST ;
class SignalTypeAST ;

class CodeGenVisitor : public evalVisitor {

	public:
		CodeGenVisitor (astream &s);
		

		virtual AST *	VisitSignalOperatorAST	( SignalOperatorAST	*	);
		virtual AST *	VisitBasicTypeAST	( BasicTypeAST		*	);
		virtual AST *	VisitSignalTypeAST	( SignalTypeAST		*	);
		virtual AST *	VisitActionAST		( ActionAST		*	);
		virtual AST *	VisitWITHINOperator	( WITHINOperator	*	);


		void	Execute( AST *, AST * = 0 );
	private:
		
		astream & m_s;
		
};

#endif
