#ifndef TedlExchangeVerbVisitor_h
#define TedlExchangeVerbVisitor_h

#include	"Visitors.h"


class ModifierListType;

class TedlExchangeVerbVisitor : public evalVisitor {

	public:
		TedlExchangeVerbVisitor (AST * repository);
		

		virtual AST *	VisitVirtualResourceAST	( VirtualResourceAST *	);
		virtual AST *	VisitSignalTypeAST	( SignalTypeAST *	);
		virtual AST *	VisitModifierListType	( ModifierListType *	);
		virtual AST *	VisitReqModifierListType( ModifierListType *	);


		void	Execute( AST *, AST * = 0 );
	private:
		AST * m_repository;
		
};

#endif
