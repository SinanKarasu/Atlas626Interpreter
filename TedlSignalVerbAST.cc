
#include	"AtlasStd.h"
#include	"TedlSignalVerbAST.h"
#include	"TedlSignalVerbVisitor.h"

TedlSignalVerbAST::TedlSignalVerbAST( ANTLRTokenPtr p )
	:AST(p)
	{
	}
TedlSignalVerbAST::~TedlSignalVerbAST(){}
	
AST *	TedlSignalVerbAST::eval	( AST * a )
	{
		if ( a != 0 )
			return ASTdown()->eval( a );
		else
			return this;
	}
	
AST *	TedlSignalVerbAST::assign	( AST * a )		{ return this; }
AST *	TedlSignalVerbAST::Accept	( ASTVisitor & v )	{ return v.VisitTedlSignalVerbAST( this ); }

TedlSignalSetupVerb		::	TedlSignalSetupVerb ( ANTLRTokenPtr p )
	:TedlSignalVerbAST(p)
	{
	}

AST *	TedlSignalSetupVerb	::	eval	( AST * a )
	{
		if ( a != 0 )
			return ASTdown()->eval( a );
		else
			return this;
	}

TedlSignalConnectVerb		::	TedlSignalConnectVerb ( ANTLRTokenPtr p )
	:TedlSignalVerbAST(p)
	{
	}

TedlSignalDisconnectVerb	::	TedlSignalDisconnectVerb ( ANTLRTokenPtr p )
	:TedlSignalVerbAST(p)
	{
	}

TedlSignalArmVerb		::	TedlSignalArmVerb ( ANTLRTokenPtr p )
	:TedlSignalVerbAST(p)
	{
	}

TedlSignalFetchVerb		::	TedlSignalFetchVerb ( ANTLRTokenPtr p )
	:TedlSignalVerbAST(p)
	{
	}

TedlSignalChangeVerb		::	TedlSignalChangeVerb ( ANTLRTokenPtr p )
	:TedlSignalVerbAST(p)
	{
	}

TedlSignalEnableEventVerb	::	TedlSignalEnableEventVerb ( ANTLRTokenPtr p )
	:TedlSignalVerbAST(p)
	{
	}

TedlSignalDisableEventVerb	::	TedlSignalDisableEventVerb ( ANTLRTokenPtr p )
	:TedlSignalVerbAST(p)
	{
	}

TedlSignalResetVerb		::	TedlSignalResetVerb ( ANTLRTokenPtr p )
	:TedlSignalVerbAST(p)
	{
	}

TedlSignalStimulateVerb		::	TedlSignalStimulateVerb ( ANTLRTokenPtr p )
	:TedlSignalVerbAST(p)
	{
	}

TedlSignalSenseVerb		::	TedlSignalSenseVerb ( ANTLRTokenPtr p )
	:TedlSignalVerbAST(p)
	{
	}

TedlSignalProveVerb		::	TedlSignalProveVerb ( ANTLRTokenPtr p )
	:TedlSignalVerbAST(p)
	{
	}

