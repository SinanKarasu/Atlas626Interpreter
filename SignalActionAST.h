#ifndef SignalActionAST_h
#define SignalActionAST_h

//#include	"AtlasStd.h"
#include	"AtlasAST.h"
#include	"Signal.h"

class	SignalActionAST : public AST
{
public:
	SignalActionAST( ANTLRTokenPtr p=0 ,TheType type=UndefinedTypeValue);
	virtual	~SignalActionAST();
	
	virtual	RWCString	getName() const;
	virtual TheType    	getType ( AST * a=0  ) const;
	virtual AST 	*	eval	( AST * a=0 );
	virtual AST 	*	assign	( AST * a   );
	virtual AST 	*	check	( AST * a=0 );
	virtual	AST	*	data	( AST *  a = 0 );
	virtual AST	*	Accept	( ASTVisitor & );
	virtual Resource * getResource( );

protected:
	AST * entity();

private:
	TheType    m_requireType;       
};	

class	DoSimulAction : public SignalActionAST {
public:
	DoSimulAction( ANTLRTokenPtr p=0 );
		
private:
};


class	RequireControlAction : public SignalActionAST {
public:
	RequireControlAction( ANTLRTokenPtr p=0 );
	virtual AST *		min	( AST *	a = 0 );
	virtual AST *		max	( AST *	a = 0 );
	virtual AST 	*	check	( AST * a=0 );

};

class	RequireCapabilityAction : public SignalActionAST {
public:
	RequireCapabilityAction( ANTLRTokenPtr p=0 );
	virtual AST *		min	( AST *	a = 0 );
	virtual AST *		max	( AST *	a = 0 );
	virtual AST 	*	check	( AST * a=0 );

};

class RequireLimitAction: public SignalActionAST {
public:
	RequireLimitAction( ANTLRTokenPtr p=0 );
	virtual AST *		min	( AST *	a = 0 );
	virtual AST *		max	( AST *	a = 0 );
	virtual AST 	*	check	( AST * a=0 );

};

class	RequireCnxAction : public SignalActionAST {
public:
	RequireCnxAction( ANTLRTokenPtr p=0 );
	virtual AST 	*	check	( AST * a=0 );

private:
};

class	SyncRequirementAction : public SignalActionAST {
public:
	SyncRequirementAction( ANTLRTokenPtr p=0 );

private:
};


class	CnxAction : public SignalActionAST {
public:
	CnxAction( ANTLRTokenPtr p=0 );
	virtual AST 	*	eval	( AST * a=0 );
	virtual	AST	*	data	( AST *  a = 0 );

private:
};

class	MaxTimeAction : public SignalActionAST {
public:
	MaxTimeAction( AST * tq , AST * max_time);
	virtual AST 	*	eval	( AST * a=0 );

private:
	AST * m_tq;
	AST * m_max_time;
};

class	WhenFieldAction : public SignalActionAST {
public:
	WhenFieldAction( AST * w );
	virtual AST 	*	eval	( AST * a=0 );

private:
	AST * m_w;
};


class	ConstraintMeasurementAction : public SignalActionAST {
public:
	ConstraintMeasurementAction( ANTLRTokenPtr p=0 );
	virtual AST 	*	assign	( AST * a   );

private:
};

class	ConstraintInclusionAction : public SignalActionAST {
public:
	ConstraintInclusionAction( ANTLRTokenPtr p=0 );

private:
};



class	WaitForTimeAction : public SignalActionAST {
public:
	WaitForTimeAction ( ANTLRTokenPtr p=0 );
	virtual AST 	*	eval	( AST * a=0 );

private:
};

class	RemoveDrawingAction : public SignalActionAST {
public:
	RemoveDrawingAction ( ANTLRTokenPtr p=0 );

private:
};

class	WaitForTimerAction : public SignalActionAST {
public:
	WaitForTimerAction ( ANTLRTokenPtr p=0 );
	virtual AST *	eval( AST * a = 0 );

private:
};

class	WaitForEventAction : public SignalActionAST {
public:
	WaitForEventAction( ANTLRTokenPtr p=0 );
	virtual AST *	eval( AST * a = 0 );

private:
};

class	WaitForManualAction : public WaitForEventAction {
public:
	WaitForManualAction( AST * label );
	virtual AST *	eval( AST * a = 0 );

private:
	sem_t sem;
	AST * m_label;
	int m_waiting;
};

class	RemoveAction : public SignalActionAST {
public:
	RemoveAction ( ANTLRTokenPtr p=0 );
	virtual AST 	*	eval	( AST * a=0 );

private:
};

class	FinishAction : public SignalActionAST{
public:
	FinishAction ( ANTLRTokenPtr p=0 );
	virtual AST 	*	eval	( AST * a=0 );

private:
};

#endif // SignalActionAST_h
