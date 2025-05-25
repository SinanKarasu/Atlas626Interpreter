#ifndef Visitors_h
#define Visitors_h

#include	"AtlasAST.h"


class ActionAST;
class BasicTypeAST;
class BuiltinFunctionAST;
class LabelAST;
class OperatorAST;
class SignalActionAST;
class SignalOperatorAST;
class SignalTypeAST;
class ModifierListType;
class SignalVerbAST;
class DataBusVerbAST;
class VerbAST;
class VirtualResourceAST;
class VirtualDataBusAST;
class ATEActionAST;
class ATEFieldTypeAST;
class ATEResourceAST;
class GateConnEventAST;
class TedlSignalVerbAST;
class TedlExchangeVerbAST;

class ModifierListType;
class DataBusTypeAST;

class DataBusActionAST;

#define VisitorMembers	\
		virtual AST *	VisitActionAST		( ActionAST *		);	\
		virtual AST *	VisitBasicTypeAST	( BasicTypeAST *	);	\
		virtual AST *	VisitBuiltinFunctionAST	( BuiltinFunctionAST *	);	\
		virtual AST *	VisitLabelAST		( LabelAST *		);	\
		virtual AST *	VisitOperatorAST	( OperatorAST *		);	\
		virtual AST *	VisitSignalActionAST	( SignalActionAST *	);	\
		virtual AST *	VisitSignalOperatorAST	( SignalOperatorAST *	);	\
		virtual AST *	VisitSignalTypeAST	( SignalTypeAST *	);	\
		virtual AST *	VisitModifierListType	( ModifierListType *	);	\
		virtual AST *	VisitReqModifierListType( ModifierListType *	);	\
		virtual AST *	VisitSignalVerbAST	( SignalVerbAST *	);	\
		virtual AST *	VisitDataBusVerbAST	( DataBusVerbAST *	);	\
		virtual AST *	VisitVerbAST		( VerbAST *		);	\
		virtual AST *	VisitVirtualResourceAST	( VirtualResourceAST *	);	\
		virtual AST *	VisitVirtualDataBusAST	( VirtualDataBusAST *	);	\
		virtual AST *	VisitATEActionAST	( ATEActionAST *	);	\
		virtual AST *	VisitATEFieldTypeAST	( ATEFieldTypeAST *	);	\
		virtual AST *	VisitATEResourceAST	( ATEResourceAST *	);	\
		virtual AST *	VisitGateConnEventAST	( GateConnEventAST *	);	\
		virtual AST *	VisitTedlSignalVerbAST	( TedlSignalVerbAST *	);	\
		virtual AST *	VisitTedlExchangeVerbAST( TedlExchangeVerbAST *	);	\
		virtual AST *	VisitDataBusTypeAST	( DataBusTypeAST *	);	\
		virtual AST *	VisitDataBusActionAST	( DataBusActionAST *	);	\

class ASTVisitor {

	public:
		virtual	~ASTVisitor();
		
		VisitorMembers
		
	protected:
		ASTVisitor();
};


class NullVisitor:public ASTVisitor {

	public:
		virtual	~NullVisitor();

		VisitorMembers
		

	protected:
		NullVisitor();
};


class evalVisitor : public ASTVisitor {

	public:
		evalVisitor();
		
		void	Execute( AST *, AST * = 0 );
		
		VisitorMembers
};

#endif
