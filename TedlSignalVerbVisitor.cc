

#include	"TedlSignalVerbVisitor.h"
#include	"AST.h"

// eval visitors....

	TedlSignalVerbVisitor :: TedlSignalVerbVisitor ( AST * repository )
		:m_repository(repository)
	{
	}


AST *	TedlSignalVerbVisitor :: VisitVirtualResourceAST	( VirtualResourceAST * a )
	{
		AST * whereDataBe =  a->eval();
		AST * field = whereDataBe->ASTdown();
		while(field){
			field->Accept(*this);
			field=field->ASTright();
		}
		return whereDataBe->ASTright();
		
	}


AST *	TedlSignalVerbVisitor :: VisitSignalTypeAST		( SignalTypeAST * a )
	{
		AST * whereDataBe =  a->eval();
		
		return whereDataBe;
		
	}


AST *	TedlSignalVerbVisitor :: VisitModifierListType		( ModifierListType * a )
	{
		ASTList * modifierList =  a->_modifierList;
		
		//ASTListIterator		it( *modifierList );
		
		// for each MODIFIER in the modifierList,
		// shove the values to the arguments.
		for(const auto& it: *modifierList) {
		//while ( ++it ){
			m_repository->assign(it);
		}
		return 0;	// Hopefully we got all we need.
		
	}

AST *	TedlSignalVerbVisitor :: VisitReqModifierListType		( ModifierListType * a )
	{
		ASTList * modifierList =  a->_modifierList;
		
		//ASTListIterator		it( *modifierList );
		
		// for each MODIFIER in the modifierList,
		// shove the values to the arguments.
		for(const auto& it: *modifierList) {		
		//while ( ++it ){
			m_repository->assign(it);
		}
		return 0;	// Hopefully we got all we need.
		
	}

void	TedlSignalVerbVisitor :: Execute( AST * Root, AST * Abort )
	{
		AST *	root = Root;
		
		while ( root  &&  root != Abort )
			root = root->Accept( *this );
	}
	
