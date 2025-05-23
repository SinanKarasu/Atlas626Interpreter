#ifndef DataBusVerbAST_h
#define DataBusVerbAST_h

#include	"AtlasAST.h"
#include	"Resource.h"

class	DataBusVerbAST : public AST
{
	public:
		DataBusVerbAST ( ANTLRTokenPtr p=0 );
		virtual	~DataBusVerbAST();
	
		virtual AST *		assign	( AST * a );
		virtual AST *		check	( AST * a );
		virtual AST *		Accept	( ASTVisitor & );
		
		virtual RWCString	getName	() const;
				
	protected:
		RWCString	m_deviceName;
		Resource *	m_device;
		void toFSM(void (DataBusResourceContext::*c)(AST *),int mult=0);
		AST * initialize();
};


class EstablishProtocolVerb : public DataBusVerbAST {
public:
        EstablishProtocolVerb(ANTLRTokenPtr p=0);
	virtual AST * eval(AST * a=0);
};

class DoExchangeVerb : public DataBusVerbAST {
public:
        DoExchangeVerb (ANTLRTokenPtr p=0);
	virtual AST * eval(AST * a=0);
};

class UpdateExchangeVerb : public DataBusVerbAST {
public:
        UpdateExchangeVerb (ANTLRTokenPtr p=0);
	virtual AST * eval(AST * a=0);
};


class EnableExchangeConfigurationVerb : public DataBusVerbAST {
public:
        EnableExchangeConfigurationVerb (ANTLRTokenPtr p=0);
	virtual AST * eval(AST * a=0);
};

class DisableExchangeConfigurationVerb : public DataBusVerbAST {
public:
        DisableExchangeConfigurationVerb (ANTLRTokenPtr p=0);
	virtual AST * eval(AST * a=0);
};

class FetchExchangeVerb : public DataBusVerbAST {
public:
        FetchExchangeVerb (ANTLRTokenPtr p=0);
	virtual AST * eval(AST * a=0);
};

#endif // DataBusVerbAST_h
