

#include	"CodeGenVisitor.h"
#include	"AST.h"

// eval visitors....

	CodeGenVisitor :: CodeGenVisitor ( astream & s )
		:m_s(s)
	{
	}


AST *	CodeGenVisitor :: VisitSignalOperatorAST	( SignalOperatorAST * a )
	{
		AST * whereDataBe =  a->eval();
		m_s	<<	"Yes in CodeGenVisitor :: VisitSignalOperatorAST ";
		m_s	<<	a->getName();
		m_s	<<	" = " ;
		m_s	<<	whereDataBe;
		m_s	<<	"\n";
		return whereDataBe->ASTright();
		
	}

AST *	CodeGenVisitor :: VisitBasicTypeAST		( BasicTypeAST * a )
	{
		
		// for each MODIFIER in the modifierList,
		// shove the values to the arguments.
			
		m_s	<<	"Yes in CodeGenVisitor :: VisitBasicTypeAST ";
		m_s	<<	a;
		m_s	<<	"\n";
		return a->ASTright();	// Hopefully we got all we need.
		
	}


AST *	CodeGenVisitor :: VisitSignalTypeAST		( SignalTypeAST * a )
	{
		AST * whereDataBe =  a->eval();
		
		m_s << "Yes in CodeGenVisitor :: VisitSignalOperatorAST ";
		m_s	<< a->getName()	<< "\n";
		
		return whereDataBe->ASTright();
		
	}

AST *	CodeGenVisitor :: VisitActionAST		( ActionAST * a )
	{
		if(a->ASTdown()){
			Execute(a->ASTdown());
			return	a->ASTright();
		}
		return	0;
	}

AST *	CodeGenVisitor :: VisitWITHINOperator		( WITHINOperator * a )
	{
		if(a->ASTdown()){
			m_s << "REM	UL and LL test " << "\n";
			Execute(a->ASTdown());
			return	a->ASTright();
		}
		return	0;
		
	}


void	CodeGenVisitor :: Execute( AST * Root, AST * Abort )
	{
		AST *	root = Root;
		
		while ( root  &&  root != Abort )
			root = root->Accept( *this );
	}
	
