#ifndef ATEFieldTypeAST_h
#define ATEFieldTypeAST_h

#include	"AtlasAST.h"
#include	"Resource.h"

class ATEFieldTypeAST : public AST {
    public:
	ATEFieldTypeAST ( AST * a = 0 );
	virtual	~ATEFieldTypeAST ();
	
	virtual AST *	eval	( AST * a=0  );
	virtual Long    getInteger( int indx=0 ) const;
	AST *   Accept ( ASTVisitor & v );
	protected:
		int m_data;
};	


class EventSlopeNegField : public ATEFieldTypeAST {
    public:
	EventSlopeNegField ( AST * a = 0 );
	virtual AST *	eval	( AST * a=0  );
	
    private:
};

class EventSlopePosField : public ATEFieldTypeAST {
    public:
	EventSlopePosField ( AST * a = 0 );
	virtual AST *	eval	( AST * a=0  );
	
    private:
};

class StateField : public ATEFieldTypeAST {
    public:
	StateField ( AST * a = 0 );
	
    private:
};

class ATESyncField : public ATEFieldTypeAST {
    public:
	ATESyncField ( AST * a = 0 );
	
    private:
};


class EventVoltageField : public ATEFieldTypeAST {
    public:
	EventVoltageField ( AST * a = 0 );
	
    private:
};


class EventPortField : public ATEFieldTypeAST {
    public:
	EventPortField ( AST * a = 0 );
	
    private:
};

class PortIsField : public ATEFieldTypeAST {
    public:
	PortIsField ( AST * a = 0 );
	virtual AST *	eval	( AST * a=0  );
    private:
};


class ATECapability : public ATEFieldTypeAST {
	public:
		//ATECapability ( AST * a = 0 );
		ATECapability ( ASTList * r );
	
		virtual AST *	check	( AST * a=0 );

	private:
		ASTList * m_resourceList;
};

class SwitchFunction : public ATEFieldTypeAST {
    public:
	SwitchFunction ( AST * a = 0 );
	//class SwitchFunction ( ANTLRTokenPtr p=0 );
	
    private:
};



#endif // ATEFieldTypeAST_h
