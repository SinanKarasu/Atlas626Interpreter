#ifndef VirtualDataBusAST_h
#define VirtualDataBusAST_h

#include	"AtlasAST.h"
#include	"NounsModifiersDimensions.h"
#include	"Resource.h"
#include	"Dictionary.h"
#include	"ResourceAST.h"
#include	"TimerObject.h"

class VirtualDataBusAST:public ResourceAST{
	public:
		VirtualDataBusAST( ANTLRTokenPtr p=0 );
		VirtualDataBusAST( Resource *  resource );
		virtual	~VirtualDataBusAST();
	
		virtual AST	*	eval	( AST * a=0  );
		virtual	AST	*	data	( AST * a = 0 );
		virtual AST	*	assign	( AST * a    );
		virtual TheType		getType	( AST * a=0  ) const;
		virtual RWCString  	getName	() const;
		virtual void		setName	( RWCString );
		virtual AST	*	Accept	( ASTVisitor & );
		virtual AST	*	init	( AST * a = 0 );
		virtual AST	*	check	( AST * a = 0 );
		virtual Resource *	getResource( );
		virtual void  setResource( Resource * resource );
		
		Resource 	*		m_resource;		// State Machine Resource
		DeviceEquivalence * 	m_DeviceEquivalence;
	protected:
		RWCString	realDevice(AST * label);
		AST			* initialize();
		void toFSM(void (DataBusResourceContext::*c)(AST *));
};


class VirtualProtocol: public VirtualDataBusAST{
	public:
		VirtualProtocol( ANTLRTokenPtr =0 );
		virtual AST *		clone	( Scope * s=0 ) const;

		virtual AST *	eval( AST * a = 0 );
		virtual AST *	check( AST * a = 0 );
};

class VirtualExchangeConfiguration: public VirtualDataBusAST{
	public:
		VirtualExchangeConfiguration( ANTLRTokenPtr =0 );
		virtual AST *		clone	( Scope * s=0 ) const;

		virtual AST *	eval( AST * a = 0 );
		virtual AST *	check( AST * a = 0 );
		virtual	AST	*	data	( AST * a = 0 );
};

class VirtualExchange: public VirtualDataBusAST{
	public:
		VirtualExchange( ANTLRTokenPtr =0 );
		virtual AST *		clone	( Scope * s=0 ) const;

		virtual AST *	eval( AST * a = 0 );
		virtual AST *	check( AST * a = 0 );
};


////////////////////// Usage classes //////////////////////////////


class UseDataBus : public VirtualDataBusAST{
	public:
		UseDataBus ( AST * label );

		virtual AST *	eval( AST * a = 0 );
		virtual AST *	check( AST * a = 0 );
	protected:
	AST * m_label;
};



class UseExchangeConfiguration: public UseDataBus{
	public:
		UseExchangeConfiguration ( AST * label );

		virtual AST *	eval( AST * a = 0 );
		virtual AST *	check( AST * a = 0 );
	protected:
};

class UseProtocol: public UseDataBus{
	public:
		UseProtocol (AST * label );

		virtual AST *	eval( AST * a = 0 );
		virtual AST *	check( AST * a = 0 );
	protected:
};

class UseExchange: public UseDataBus{
	public:
		UseExchange (AST * label );

		virtual AST *	eval( AST * a = 0 );
		virtual AST *	check( AST * a = 0 );
	protected:
};


#endif // VirtualDataBusAST_h
