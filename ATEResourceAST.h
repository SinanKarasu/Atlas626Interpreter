#ifndef ATEResourceAST_h
#define ATEResourceAST_h

#include	"AtlasAST.h"
#include	"ResourceAST.h"
#include	"NounsModifiersDimensions.h"

class DataBusResourceContext;
class AnalogResourceContext;
class ResourceContextBASE;

class ATEResourceAST : public ResourceAST {
    public:
	ATEResourceAST (ANTLRTokenPtr p=0 );
	virtual	~ATEResourceAST ();
	
	virtual AST *	eval	( AST * a=0 );
	virtual AST *	assign	( AST * a   );
	virtual AST *	check	( AST * a=0 );
	virtual AST *	Accept	( ASTVisitor & );
	virtual	AST *	data	( AST *  a = 0 );
	virtual	RWCString	getName() const;

    private:
    protected:
    	AST * setBase(AST * a,AnalogResourceContext	* b);
    	AST * setBase(AST * a,DataBusResourceContext	* b);
};

class ATEDeviceFunction : public ATEResourceAST {
    public:
	ATEDeviceFunction ( ANTLRTokenPtr p=0 );
	ATEDeviceFunction ( AST * stateMachine, ASTList * _SigCharsList=0 );
	virtual AST *	check	( AST * a=0 );
	virtual	AST *	data	( AST *  a = 0 );
	virtual AST *	eval	( AST * a=0 );

    private:
    AST * m_StateMachine;
    ASTList * m_SigCharsList;
};


class CnxList : public ATEResourceAST {
    public:
	CnxList ( ASTList * cnxList=0 );

    private:
    ASTList * m_cnxList;
};

class SigChar : public ATEResourceAST {
    public:
	SigChar ( ANTLRTokenPtr p=0 );
	virtual	AST *	data	( AST *  a = 0 );

    private:
};

class ControlBlock : public ATEResourceAST {
    public:
	ControlBlock ( ANTLRTokenPtr p=0 );

    private:
    AnalogResourceContext * m_AnalogResourceContext;
};

class ATESource : public ATEResourceAST {
    public:
	ATESource ( AnalogResourceContext * p=0 );
	virtual AST *	eval	( AST * a=0 );

    private:
    AnalogResourceContext * m_AnalogResourceContext;
};

class ATESensor : public ATEResourceAST {
    public:
	ATESensor ( AnalogResourceContext * p=0 );
	virtual AST *	eval	( AST * a=0 );

    private:
    AnalogResourceContext * m_AnalogResourceContext;
};

class ATEEventMonitor : public ATEResourceAST {
    public:
	ATEEventMonitor ( AnalogResourceContext * p=0 );
	virtual AST *	eval	( AST * a=0 );

    private:
    AnalogResourceContext * m_AnalogResourceContext;
};

class ATELoad : public ATEResourceAST {
    public:
	ATELoad ( AnalogResourceContext * p=0 );
	virtual AST *	eval	( AST * a=0 );

    private:
    AnalogResourceContext * m_AnalogResourceContext;
};
class ATETimer : public ATEResourceAST {
    public:
	ATETimer ( AnalogResourceContext * p=0 );
	virtual AST *	eval	( AST * a=0 );

    private:
    AnalogResourceContext * m_AnalogResourceContext;
};

class ATEBusProtocol : public ATEResourceAST {
    public:
	ATEBusProtocol ( DataBusResourceContext * p,const RWCString spec );
	virtual AST *	eval	( AST * a=0 );

    private:
    DataBusResourceContext * m_DataBusResourceContext;
    RWCString m_spec;
};

#endif // ATEResourceAST_h
