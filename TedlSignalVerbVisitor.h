#ifndef TedlSignalVerbVisitor_h
#define TedlSignalVerbVisitor_h

#include	"Visitors.h"


class ModifierListType;

class TedlSignalVerbVisitor : public evalVisitor {

	public:
		TedlSignalVerbVisitor (AST * repository);
		

		virtual AST *	VisitVirtualResourceAST	( VirtualResourceAST *	);
		virtual AST *	VisitSignalTypeAST	( SignalTypeAST *	);
		virtual AST *	VisitModifierListType	( ModifierListType *	);
		virtual AST *	VisitReqModifierListType( ModifierListType *	);


		void	Execute( AST *, AST * = 0 );
	private:
		AST * m_repository;
		
};

#endif
