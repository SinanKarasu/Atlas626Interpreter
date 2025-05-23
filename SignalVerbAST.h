#ifndef SignalVerbAST_h
#define SignalVerbAST_h

#include	"AtlasAST.h"
#include	"Resource.h"

class	SignalVerbAST : public AST {
	public:
		SignalVerbAST ( ANTLRTokenPtr p=0 );
		virtual	~SignalVerbAST();
	
		virtual AST *		assign	( AST * a );
		virtual AST *		check	( AST * a );
		virtual AST *		Accept	( ASTVisitor & );
		
		virtual RWCString	getName	() const;
				
	protected:
		RWCString	m_deviceName;
		Resource *	m_device;
		void toFSM(void (AnalogResourceContext::*c)(AST *),int mult=0);
		AST * initialize();
};

class	SetupVerb   : public SignalVerbAST {
	public:
		SetupVerb ( ANTLRTokenPtr p=0 );	
		virtual AST *	eval( AST * a = 0 );

	private:
};


class	ConnectVerb   : public SignalVerbAST {
	public:
		ConnectVerb ( ANTLRTokenPtr p=0 );	
		virtual AST *	eval( AST * a = 0 );

	private:
};

class	FetchVerb   : public SignalVerbAST {
	public:
		FetchVerb ( ANTLRTokenPtr p=0 );	
		virtual AST *	eval( AST * a = 0 );

	private:
};


class	InitiateVerb   : public SignalVerbAST {
	public:
		InitiateVerb ( ANTLRTokenPtr p=0 );	
		virtual AST *	eval( AST * a = 0 );

	private:
};

class	ReadVerb   : public SignalVerbAST {
	public:
		ReadVerb ( ANTLRTokenPtr p=0 );	
		virtual AST *	eval( AST * a = 0 );

	private:
};

class	ChangeVerb   : public SignalVerbAST {
	public:
		ChangeVerb ( ANTLRTokenPtr p=0 );	
		virtual AST *	eval( AST * a = 0 );

	private:
};

class	DisconnectVerb   : public SignalVerbAST {
	public:
		DisconnectVerb ( ANTLRTokenPtr p=0 );	
		virtual AST *	eval( AST * a = 0 );

	private:
};

class	RemoveVerb   : public SignalVerbAST {
	public:
		RemoveVerb ( ANTLRTokenPtr p=0 );	
		virtual AST *	eval( AST * a = 0 );

	private:
};

class	ArmVerb   : public SignalVerbAST {
	public:
		ArmVerb ( ANTLRTokenPtr p=0 );	
		virtual AST *	eval( AST * a = 0 );

	private:
};

class	EnableEventVerb   : public SignalVerbAST {
	public:
		EnableEventVerb ( ANTLRTokenPtr p=0 );	
		virtual AST *	eval( AST * a = 0 );

	private:
};


class	DisableEventVerb   : public SignalVerbAST {
	public:
		DisableEventVerb ( ANTLRTokenPtr p=0 );	
		virtual AST *	eval( AST * a = 0 );

	private:
};


class	ApplyVerb   : public SignalVerbAST {
	public:
		ApplyVerb ( ANTLRTokenPtr p=0 );	
		virtual AST *	eval( AST * a = 0 );

	private:
};


class	ResetVerb   : public SignalVerbAST {
	public:
		ResetVerb ( ANTLRTokenPtr p=0 );	
		virtual AST *	eval( AST * a = 0 );

	private:
};

class	MeasureVerb   : public SignalVerbAST {
	public:
		MeasureVerb ( ANTLRTokenPtr p=0 );	
		virtual AST *	eval( AST * a = 0 );

	private:
};

class	MonitorVerb   : public SignalVerbAST {
	public:
		MonitorVerb ( ANTLRTokenPtr p=0 );	
		virtual AST *	eval( AST * a = 0 );

	private:
};

class	VerifyVerb   : public SignalVerbAST {
	public:
		VerifyVerb ( Scope * s );	
		virtual AST *	eval( AST * a = 0 );
		virtual AST *	printEvaluation	( AST * a = 0  ) ;

	private:
	Scope * m_scope;
};


class	WaitForVerb : public SignalVerbAST {
	public:
		WaitForVerb( ANTLRTokenPtr p=0 );
		virtual AST *	eval( AST * a = 0 );

	private:
		sem_t m_sem;
};

class	WaitForManualVerb : public SignalVerbAST {
	public:
		WaitForManualVerb( ANTLRTokenPtr p=0 );
		virtual AST *	eval( AST * a = 0 );

	private:
};

class	EnableManualVerb : public SignalVerbAST {
	public:
		EnableManualVerb( AST * label );
		virtual AST *	eval( AST * a = 0 );

	private:
	AST * m_label;
};

class	DisableManualVerb : public SignalVerbAST {
	public:
		DisableManualVerb( AST * label );
		virtual AST *	eval( AST * a = 0 );

	private:
	AST * m_label;
};

class	FinishVerb   : public SignalVerbAST {
	public:
		FinishVerb ( ANTLRTokenPtr p=0 );	
		virtual AST *	eval( AST * a = 0 );

	private:
};


class EnableEscapeVerb : public SignalVerbAST {
public:
        EnableEscapeVerb (class AtlasParser * ap);
	virtual AST * eval(AST * a=0);
private:
	AtlasParser * m_ap;
};

class DisableEscapeVerb : public SignalVerbAST {
public:
        DisableEscapeVerb (class AtlasParser * ap);
	virtual AST * eval(AST * a=0);
private:
	AtlasParser * m_ap;
};


#endif // SignalVerbAST_h
