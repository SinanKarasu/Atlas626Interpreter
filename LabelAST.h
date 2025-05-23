#ifndef LabelAST_h
#define LabelAST_h

#include	"Types.h"
#include	"AtlasStd.h"
#include	"AtlasParser.h"

class LabelAST: public AST {
public:
	LabelAST( ANTLRTokenPtr p, LabelType t, Scope * scope=0 );
	virtual ~LabelAST();
	virtual AST *		eval		( AST * a=0 );
	virtual	AST	*	data		( AST * a = 0 );
	virtual LabelType	getLabelType	() const;
	virtual Scope *		getScope	() const;
	virtual TheType		getType		( AST * a=0 ) const;
	virtual AST *		Accept		( ASTVisitor & );
	virtual RWCString	getName		() const;
	virtual	AST	*	label	( AST * a = 0 ) ;
	virtual astream&	operator<<	( astream& s );

protected:
	LabelType	m_labelType;
	Scope *		m_Scope;
	int		ArgsCheck(AST * E1,AST * E2);
};


class  ProgramNameLabel : public LabelAST {
public:
	ProgramNameLabel(ANTLRTokenPtr p,Scope * scope=0);
	virtual AST * eval(AST * a=0);
};

//
class  ModuleNameLabel : public LabelAST {
public:
	ModuleNameLabel(ANTLRTokenPtr p,Scope * scope=0);
};

//
class  NonAtlasModuleLabel : public LabelAST {
public:
	NonAtlasModuleLabel(ANTLRTokenPtr p,Scope * scope=0);
};

//
class  BlockNameLabel : public LabelAST {
public:
	BlockNameLabel(ANTLRTokenPtr p,Scope * scope=0);
};

class  ParameterLabel : public LabelAST {
public:
	ParameterLabel(ANTLRTokenPtr p,Scope * scope=0);
	virtual AST * eval(AST * a=0);
	virtual LabelType getLabelType() const;
};

class  DrawingLabel : public LabelAST {
public:
	DrawingLabel(ANTLRTokenPtr p,Scope * scope=0);
};

class  SignalLabel : public LabelAST {
public:
	SignalLabel(ANTLRTokenPtr p,Scope * scope=0);
};

class  ProcedureLabel : public LabelAST {
public:
	ProcedureLabel( ANTLRTokenPtr p,AtlasParser * ap=0, Scope * scope=0 );
	
	virtual void	setInteger	( int indx=0, Long value=0 );
	virtual Long	getInteger	( int indx=0 ) const;
	virtual AST *	eval		( AST *  a=0 );
	virtual TheType	getType		( AST *  a=0 ) const;
	virtual	AST	*	data	( AST * a = 0 );
	AST *   check( AST * a = 0 );
	
private:
	int paramcount;
	int resultcount;
	AtlasParser * _p;
};

class  FunctionLabel : public LabelAST {
public:
	FunctionLabel( ANTLRTokenPtr p, AtlasParser * ap=0, Scope * scope=0 );
	
	virtual void	setInteger	( int indx=0, Long value=0 );
	virtual Long	getInteger	( int indx=0) const;
	virtual AST *	eval		( AST * a=0 );
	virtual TheType	getType		( AST * a=0 ) const;
	virtual	AST	*	data	( AST * a = 0 );
	AST *   check( AST * a = 0 );
private:
	int paramcount;
	AtlasParser * _p;
};

class  EventLabel : public LabelAST {
public:
	EventLabel(ANTLRTokenPtr p,Scope * scope=0);
private:
	DeviceEquivalence * m_monitor;
};

class  EventIntervalLabel : public LabelAST {
public:
	EventIntervalLabel(ANTLRTokenPtr p,Scope * scope=0);
};

class  EventIndicatorLabel : public LabelAST {
public:
	EventIndicatorLabel(ANTLRTokenPtr p,Scope * scope=0);
};

class  ExchangeLabel : public LabelAST {
public:
	ExchangeLabel(ANTLRTokenPtr p,Scope * scope=0);
};

class  ProtocolLabel : public LabelAST {
public:
	ProtocolLabel(ANTLRTokenPtr p,Scope * scope=0);
	AST *   check( AST * a = 0 );
};


class  DeviceIdentifierLabel : public LabelAST {
public:
	DeviceIdentifierLabel(ANTLRTokenPtr p,Scope * scope=0);
};

class  ConfigurationLabel : public LabelAST {
public:
	ConfigurationLabel(ANTLRTokenPtr p,Scope * scope=0);
};

class  ExchangeConfigurationLabel : public LabelAST {
public:
	ExchangeConfigurationLabel(ANTLRTokenPtr p,Scope * scope=0);
};

class  DigitalSourceLabel : public LabelAST {
public:
	DigitalSourceLabel(ANTLRTokenPtr p,Scope * scope=0);
};

class  DigitalSensorLabel : public LabelAST {
public:
	DigitalSensorLabel(ANTLRTokenPtr p,Scope * scope=0);
};

class  TimerLabel : public LabelAST {
public:
	TimerLabel(ANTLRTokenPtr p,Scope * scope=0);
};

class  RequirementLabel : public LabelAST {
public:
	RequirementLabel(ANTLRTokenPtr p,Scope * scope=0);
	AST *   check( AST * a = 0 );
private:
	DeviceEquivalence * dev_name;
};

//
class  ConstantIdentifierLabel : public LabelAST {
public:
	ConstantIdentifierLabel(ANTLRTokenPtr p,Scope * scope=0);
};

//
class  TypeIdentifierLabel : public LabelAST {
public:
	TypeIdentifierLabel(ANTLRTokenPtr p,Scope * scope=0);
	virtual AST * eval(AST * a=0);
};

//
class  VariableIdentifierLabel : public LabelAST {
public:
	VariableIdentifierLabel(ANTLRTokenPtr p,Scope * scope=0);
	virtual AST *	eval	( AST   * a=0 );
	virtual AST *	clone	( Scope * s=0 ) const;
};

class  Proxy : public LabelAST {  // just a proxy for VariableIdentifierLabel
public:
	Proxy(ANTLRTokenPtr p=0,Scope * scope=0);
	virtual AST *		eval		( AST * a=0 );
	virtual LabelType getLabelType() const;
	virtual	AST	*	label	( AST * a = 0 );

	virtual astream&	operator<<	( astream& s );
};

class  FileLabel : public LabelAST {
public:
	FileLabel(ANTLRTokenPtr p,Scope * scope=0);
	virtual AST * eval(AST * a=0);
};

class  EnumerationElementLabel : public LabelAST {
public:
	EnumerationElementLabel(ANTLRTokenPtr p,Scope * scope=0);
	virtual AST * eval(AST * a=0);
};

class  ConnectionTypeLabel : public LabelAST {
public:
	ConnectionTypeLabel(ANTLRTokenPtr p,Scope * scope=0);
	virtual AST * eval(AST * a=0);
};


class  BusSpecificationLabel : public LabelAST {
public:
	BusSpecificationLabel(ANTLRTokenPtr p,Scope * scope=0);
};

class  ModifierNameLabel : public LabelAST {
public:
	ModifierNameLabel(ANTLRTokenPtr p,Scope * scope=0);
};

class  DimNameLabel : public LabelAST {
public:
	DimNameLabel(ANTLRTokenPtr p,Scope * scope=0);
};

class  PinDescriptorNameLabel : public LabelAST {
public:
	PinDescriptorNameLabel(ANTLRTokenPtr p,Scope * scope=0);
};

 
class  TerminalIdentifierLabel : public LabelAST {
public:
	virtual RWCString	getName() const;
	TerminalIdentifierLabel(ANTLRTokenPtr p,Scope * scope=0);
};


class  ProtocolParameterNameLabel : public LabelAST {
public:
	ProtocolParameterNameLabel(ANTLRTokenPtr p,Scope * scope=0);
};

class  BusParameterNameLabel : public LabelAST {
public:
	BusParameterNameLabel(ANTLRTokenPtr p,Scope * scope=0);
};

class  BusModeNameLabel : public LabelAST {
public:
	BusModeNameLabel(ANTLRTokenPtr p,Scope * scope=0);
};

class  TestEquipRoleNameLabel : public LabelAST {
public:
	TestEquipRoleNameLabel(ANTLRTokenPtr p,Scope * scope=0);
};

#endif // LabelAST_h
