#ifndef TedlSignalVerbAST_h
#define TedlSignalVerbAST_h

#include	"AtlasAST.h"

class	TedlSignalVerbAST : public AST {
	public:
		TedlSignalVerbAST( ANTLRTokenPtr p=0 );
		virtual	~TedlSignalVerbAST();
	
		virtual AST *	eval	( AST * a=0 );
		virtual AST *	assign	( AST * a   );
		virtual AST *	Accept	( ASTVisitor & );

	private:
};	


class	TedlSignalSetupVerb : public TedlSignalVerbAST {
	public:
		TedlSignalSetupVerb ( ANTLRTokenPtr p=0 );
		virtual AST *	eval	( AST * a=0 );

	private:
};


class	TedlSignalConnectVerb : public TedlSignalVerbAST {
	public:
		TedlSignalConnectVerb ( ANTLRTokenPtr p=0 );

	private:
};

class	TedlSignalDisconnectVerb : public TedlSignalVerbAST {
	public:
		TedlSignalDisconnectVerb ( ANTLRTokenPtr p=0 );

	private:
};

class	TedlSignalArmVerb : public TedlSignalVerbAST {
	public:
		TedlSignalArmVerb ( ANTLRTokenPtr p=0 );

	private:
};

class	TedlSignalFetchVerb : public TedlSignalVerbAST {
	public:
		TedlSignalFetchVerb ( ANTLRTokenPtr p=0 );

	private:
};

class	TedlSignalChangeVerb : public TedlSignalVerbAST {
	public:
		TedlSignalChangeVerb ( ANTLRTokenPtr p=0 );

	private:
};

class	TedlSignalEnableEventVerb : public TedlSignalVerbAST {
	public:
		TedlSignalEnableEventVerb ( ANTLRTokenPtr p=0 );

	private:
};

class	TedlSignalDisableEventVerb : public TedlSignalVerbAST {
	public:
		TedlSignalDisableEventVerb ( ANTLRTokenPtr p=0 );

	private:
};

class	TedlSignalResetVerb : public TedlSignalVerbAST {
	public:
		TedlSignalResetVerb ( ANTLRTokenPtr p=0 );

	private:
};

class	TedlSignalStimulateVerb : public TedlSignalVerbAST {
	public:
		TedlSignalStimulateVerb ( ANTLRTokenPtr p=0 );

	private:
};

class	TedlSignalSenseVerb : public TedlSignalVerbAST {
	public:
		TedlSignalSenseVerb ( ANTLRTokenPtr p=0 );

	private:
};

class	TedlSignalProveVerb : public TedlSignalVerbAST {
	public:
		TedlSignalProveVerb ( ANTLRTokenPtr p=0 );

	private:
};


#endif // TedlSignalVerbAST_h
