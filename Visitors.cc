

#include	"Visitors.h"
#include	"AST.h"

// Base class. Intended to catch bugs....

ASTVisitor::~ASTVisitor()	{};
ASTVisitor::ASTVisitor()	{};

AST *	ASTVisitor::VisitActionAST		( ActionAST *		)	{ assert(0); return 0; }
AST *	ASTVisitor::VisitBasicTypeAST		( BasicTypeAST *	)	{ assert(0); return 0; }
AST *	ASTVisitor::VisitBuiltinFunctionAST	( BuiltinFunctionAST *	)	{ assert(0); return 0; }
//AST *	ASTVisitor::VisitConditionAST		( ConditionAST *	)	{ assert(0); return 0; }
AST *	ASTVisitor::VisitLabelAST		( LabelAST *		)	{ assert(0); return 0; }
AST *	ASTVisitor::VisitOperatorAST		( OperatorAST *		)	{ assert(0); return 0; }
AST *	ASTVisitor::VisitSignalActionAST	( SignalActionAST *	)	{ assert(0); return 0; }
AST *	ASTVisitor::VisitSignalOperatorAST	( SignalOperatorAST *	)	{ assert(0); return 0; }
AST *	ASTVisitor::VisitSignalTypeAST		( SignalTypeAST *	)	{ assert(0); return 0; }
AST *	ASTVisitor::VisitModifierListType	( ModifierListType *	)	{ assert(0); return 0; }
AST *	ASTVisitor::VisitReqModifierListType	( ModifierListType *	)	{ assert(0); return 0; }
AST *	ASTVisitor::VisitSignalVerbAST		( SignalVerbAST *	)	{ assert(0); return 0; }
AST *	ASTVisitor::VisitDataBusVerbAST		( DataBusVerbAST *	)	{ assert(0); return 0; }
AST *	ASTVisitor::VisitVerbAST		( VerbAST *		)	{ assert(0); return 0; }
AST *	ASTVisitor::VisitVirtualResourceAST	( VirtualResourceAST *	)	{ assert(0); return 0; }
AST *	ASTVisitor::VisitVirtualDataBusAST	( VirtualDataBusAST *	)	{ assert(0); return 0; }
AST *	ASTVisitor::VisitATEActionAST		( ATEActionAST *	)	{ assert(0); return 0; }
AST *	ASTVisitor::VisitATEFieldTypeAST	( ATEFieldTypeAST *	)	{ assert(0); return 0; }
AST *	ASTVisitor::VisitATEResourceAST		( ATEResourceAST *	)	{ assert(0); return 0; }
AST *	ASTVisitor::VisitGateConnEventAST	( GateConnEventAST *	)	{ assert(0); return 0; }
AST *	ASTVisitor::VisitTedlSignalVerbAST	( TedlSignalVerbAST *	)	{ assert(0); return 0; }
AST *	ASTVisitor::VisitTedlExchangeVerbAST	( TedlExchangeVerbAST *	)	{ assert(0); return 0; }
AST *	ASTVisitor::VisitDataBusTypeAST		( DataBusTypeAST *	)	{ assert(0); return 0; }
AST *	ASTVisitor::VisitDataBusActionAST	( DataBusActionAST *	)	{ assert(0); return 0; }


// Null Visitor class only should be subclassed to do specialized operations....

NullVisitor::~NullVisitor()	{};
NullVisitor::NullVisitor()	{};

AST *	NullVisitor::VisitActionAST		( ActionAST *		)	{  return 0; }
AST *	NullVisitor::VisitBasicTypeAST		( BasicTypeAST *	)	{  return 0; }
AST *	NullVisitor::VisitBuiltinFunctionAST	( BuiltinFunctionAST *	)	{  return 0; }
//AST *	NullVisitor::VisitConditionAST		( ConditionAST *	)	{  return 0; }
AST *	NullVisitor::VisitLabelAST		( LabelAST *		)	{  return 0; }
AST *	NullVisitor::VisitOperatorAST		( OperatorAST *		)	{  return 0; }
AST *	NullVisitor::VisitSignalActionAST	( SignalActionAST *	)	{  return 0; }
AST *	NullVisitor::VisitSignalOperatorAST	( SignalOperatorAST *	)	{  return 0; }
AST *	NullVisitor::VisitSignalTypeAST		( SignalTypeAST *	)	{  return 0; }
AST *	NullVisitor::VisitModifierListType	( ModifierListType *	)	{  return 0; }
AST *	NullVisitor::VisitReqModifierListType	( ModifierListType *	)	{  return 0; }
AST *	NullVisitor::VisitSignalVerbAST		( SignalVerbAST *	)	{  return 0; }
AST *	NullVisitor::VisitDataBusVerbAST	( DataBusVerbAST *	)	{  return 0; }
AST *	NullVisitor::VisitVerbAST		( VerbAST *		)	{  return 0; }
AST *	NullVisitor::VisitVirtualResourceAST	( VirtualResourceAST *	)	{  return 0; }
AST *	NullVisitor::VisitVirtualDataBusAST	( VirtualDataBusAST *	)	{  return 0; }
AST *	NullVisitor::VisitATEActionAST		( ATEActionAST *	)	{  return 0; }
AST *	NullVisitor::VisitATEFieldTypeAST	( ATEFieldTypeAST *	)	{  return 0; }
AST *	NullVisitor::VisitATEResourceAST	( ATEResourceAST *	)	{  return 0; }
AST *	NullVisitor::VisitGateConnEventAST	( GateConnEventAST *	)	{  return 0; }
AST *	NullVisitor::VisitTedlSignalVerbAST	( TedlSignalVerbAST *	)	{  return 0; }
AST *	NullVisitor::VisitTedlExchangeVerbAST	( TedlExchangeVerbAST *	)	{  return 0; }
AST *	NullVisitor::VisitDataBusTypeAST	( DataBusTypeAST *	)	{  return 0; }
AST *	NullVisitor::VisitDataBusActionAST	( DataBusActionAST *	)	{  return 0; }


// eval visitors....

	evalVisitor :: evalVisitor()	{};

void	evalVisitor :: Execute( AST * Root, AST * Abort )
	{
		AST *	root = Root;
		
		while ( root  &&  root != Abort )
			root = root->Accept( *this );
	}
	
AST *	evalVisitor :: VisitActionAST		( ActionAST		*a )	{ return  a->eval(); }
AST *	evalVisitor :: VisitBasicTypeAST	( BasicTypeAST		*a )	{ return  a->eval(); }
AST *	evalVisitor :: VisitBuiltinFunctionAST	( BuiltinFunctionAST	*a )	{ return  a->eval(); }
//AST *	evalVisitor :: VisitConditionAST	( ConditionAST		*a )	{ return  a->eval(); }
AST *	evalVisitor :: VisitLabelAST		( LabelAST		*a )	{ return  a->eval(); }
AST *	evalVisitor :: VisitOperatorAST		( OperatorAST		*a )	{ return  a->eval(); }
AST *	evalVisitor :: VisitSignalActionAST	( SignalActionAST	*a )	{ return  a->eval(); }
AST *	evalVisitor :: VisitSignalOperatorAST	( SignalOperatorAST	*a )	{ return  a->eval(); }
AST *	evalVisitor :: VisitSignalTypeAST	( SignalTypeAST		*a )	{ return  a->eval(); }
AST *	evalVisitor :: VisitModifierListType	( ModifierListType	*a )	{ return  a->eval(); }
AST *	evalVisitor :: VisitReqModifierListType	( ModifierListType	*a )	{ return  a->eval(); }
AST *	evalVisitor :: VisitSignalVerbAST	( SignalVerbAST		*a )	{ return  a->eval(); }
AST *	evalVisitor :: VisitDataBusVerbAST	( DataBusVerbAST	*a )	{ return  a->eval(); }
AST *	evalVisitor :: VisitVerbAST		( VerbAST		*a )	{ return  a->eval(); }
AST *	evalVisitor :: VisitVirtualResourceAST	( VirtualResourceAST	*a )	{ return  a->eval(); }
AST *	evalVisitor :: VisitVirtualDataBusAST	( VirtualDataBusAST	*a )	{ return  a->eval(); }
AST *	evalVisitor :: VisitATEActionAST	( ATEActionAST		*a )	{ return  a->eval(); }
AST *	evalVisitor :: VisitATEFieldTypeAST	( ATEFieldTypeAST	*a )	{ return  a->eval(); }
AST *	evalVisitor :: VisitATEResourceAST	( ATEResourceAST	*a )	{ return  a->eval(); }
AST *	evalVisitor :: VisitGateConnEventAST	( GateConnEventAST	*a )	{ return  a->eval(); }
AST *	evalVisitor :: VisitTedlSignalVerbAST	( TedlSignalVerbAST	*a )	{ return  a->eval(); }
AST *	evalVisitor :: VisitTedlExchangeVerbAST	( TedlExchangeVerbAST	*a )	{ return  a->eval(); }
AST *	evalVisitor :: VisitDataBusTypeAST	( DataBusTypeAST	*a )	{ return  a->eval(); }
AST *	evalVisitor :: VisitDataBusActionAST	( DataBusActionAST	*a )	{ return  a->eval(); }
