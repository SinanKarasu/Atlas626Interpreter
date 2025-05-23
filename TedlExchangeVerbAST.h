#ifndef TedlExchangeVerbAST_h
#define TedlExchangeVerbAST_h

#include	"AtlasAST.h"

class	TedlExchangeVerbAST : public AST{
	public:
		TedlExchangeVerbAST ( ANTLRTokenPtr p=0 );
		virtual	~TedlExchangeVerbAST();
	
		virtual AST *	eval	( AST * a=0 );
		virtual AST *	assign	( AST * a   );
		virtual AST *	Accept	( ASTVisitor & );

	private:
};	


class	TedlExchangeDoVerb : public TedlExchangeVerbAST{
	public:
		TedlExchangeDoVerb ( ANTLRTokenPtr p=0 );
		virtual AST *	eval	( AST * a=0 );

	private:
};


class	TedlExchangeEnableVerb : public TedlExchangeVerbAST{
	public:
		TedlExchangeEnableVerb ( ANTLRTokenPtr p=0 );

	private:
};

class	TedlExchangeDisableVerb : public TedlExchangeVerbAST{
	public:
		TedlExchangeDisableVerb ( ANTLRTokenPtr p=0 );

	private:
};

class	TedlExchangeUpdateVerb : public TedlExchangeVerbAST{
	public:
		TedlExchangeUpdateVerb ( ANTLRTokenPtr p=0 );

	private:
};

class	TedlExchangeFetchVerb : public TedlExchangeVerbAST{
	public:
		TedlExchangeFetchVerb ( ANTLRTokenPtr p=0 );

	private:
};


#endif // TedlExchangeVerbAST_h
