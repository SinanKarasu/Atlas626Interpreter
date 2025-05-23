
#include	"AtlasStd.h"
#include	"TedlExchangeVerbAST.h"
#include	"TedlExchangeVerbVisitor.h"

TedlExchangeVerbAST::TedlExchangeVerbAST( ANTLRTokenPtr p )
	:AST(p)
	{
	}
TedlExchangeVerbAST::~TedlExchangeVerbAST(){}
	
AST *	TedlExchangeVerbAST::eval	( AST * a )
	{
		if ( a != 0 )
			return ASTdown()->eval( a );
		else
			return this;
	}
	
AST *	TedlExchangeVerbAST::assign	( AST * a )		{ return this; }
AST *	TedlExchangeVerbAST::Accept	( ASTVisitor & v )	{ return v.VisitTedlExchangeVerbAST( this ); }

TedlExchangeDoVerb		::	TedlExchangeDoVerb ( ANTLRTokenPtr p )
	:TedlExchangeVerbAST(p)
	{
	}

AST *	TedlExchangeDoVerb	::	eval	( AST * a )
	{
		if ( a != 0 )
			return ASTdown()->eval( a );
		else
			return this;
	}

TedlExchangeEnableVerb		::	TedlExchangeEnableVerb ( ANTLRTokenPtr p )
	:TedlExchangeVerbAST(p)
	{
	}

TedlExchangeDisableVerb	::	TedlExchangeDisableVerb ( ANTLRTokenPtr p )
	:TedlExchangeVerbAST(p)
	{
	}

TedlExchangeUpdateVerb		::	TedlExchangeUpdateVerb ( ANTLRTokenPtr p )
	:TedlExchangeVerbAST(p)
	{
	}

TedlExchangeFetchVerb		::	TedlExchangeFetchVerb ( ANTLRTokenPtr p )
	:TedlExchangeVerbAST(p)
	{
	}



