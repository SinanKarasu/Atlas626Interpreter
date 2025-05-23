#ifndef VirtualResourceAST_h
#define VirtualResourceAST_h

#include	"AtlasAST.h"
#include	"NounsModifiersDimensions.h"
#include	"Resource.h"
#include	"Dictionary.h"
#include	"ResourceAST.h"
#include	"TimerObject.h"

class VirtualResourceAST:public ResourceAST{
	public:
		VirtualResourceAST( ANTLRTokenPtr p=0 );
		VirtualResourceAST( Resource *  resource );
		virtual	~VirtualResourceAST();
	
		virtual AST	*	eval	( AST * a=0  );
		virtual	AST	*	data	( AST * a = 0 );
		virtual AST	*	assign	( AST * a    );
		virtual TheType		getType	( AST * a=0  ) const;
		virtual RWCString  	getName	() const;
		virtual void		setName	( RWCString );
		virtual AST	*	Accept	( ASTVisitor & );
		virtual AST	*	init	( AST * a = 0 );
		virtual AST	*	check	( AST * a = 0 );
		virtual Resource *	getResource( )		;
		virtual void  setResource( Resource * resource );
		
		Resource 	*		m_resource;		// State Machine Resource
		DeviceEquivalence * 	m_DeviceEquivalence;
	protected:
		RWCString	realDevice(AST * label);
		AST			* initialize();
		void toFSM(void (AnalogResourceContext::*c)(AST *));
};

class VirtualAnalogEvent: public VirtualResourceAST {
	public:
		VirtualAnalogEvent( ANTLRTokenPtr =0 );
};

class VirtualEventMonitor: public VirtualResourceAST {
	public:
		VirtualEventMonitor( ANTLRTokenPtr =0 );
		virtual AST *		clone	( Scope * s=0 ) const;
		virtual AST *	eval( AST * a = 0 );
		virtual AST *	check( AST * a = 0 );
};

class VirtualTimer: public VirtualResourceAST {
	public:
		VirtualTimer( ANTLRTokenPtr =0 );
		virtual TheType		getType	( AST * a = 0  ) const;
		virtual AST	*	clone	( Scope * s = 0 ) const;
		virtual AST	*	init	( AST * a = 0 );
		virtual	AST	*	data	( AST * a = 0 );
		virtual AST	*	add	( AST * a   );
		virtual AST	*	eval( AST * a = 0 );
		virtual AST	*	check	( AST * a = 0 );
		virtual double	getDecimal( int indx=0 ) const;
	protected:
		TimerObject m_timer;
		class DecimalNumber * the_time;
		double t;
};

class VirtualSensor: public VirtualResourceAST {
	public:
		VirtualSensor( ANTLRTokenPtr =0 );
		virtual AST *		clone	( Scope * s=0 ) const;

		virtual AST *	eval( AST * a = 0 );
		virtual AST *	check( AST * a = 0 );
};

class VirtualSource : public VirtualResourceAST {
	public:
		VirtualSource( ANTLRTokenPtr =0 );
		virtual AST *		clone	( Scope * s=0 ) const;

		virtual AST *	eval( AST * a = 0 );
		virtual AST *	check( AST * a = 0 );
};

class VirtualLoad : public VirtualResourceAST {
	public:
		VirtualLoad( ANTLRTokenPtr =0 );
		virtual AST *		clone	( Scope * s=0 ) const;

		virtual AST *	eval( AST * a = 0 );
		virtual AST *	check( AST * a = 0 );
};

class VirtualResourceEntry : public VirtualResourceAST {
	public:
		VirtualResourceEntry ( ANTLRTokenPtr p=0 );

	protected:
		NounEntry * nounEntry;
		ModifierEntry * modifierEntry;
		DimensionEntry * dimensionEntry;
};



////////////////////// Usage classes //////////////////////////////


class UseResource : public VirtualResourceAST {
	public:
		UseResource ( AST * label );

		virtual AST *	eval( AST * a = 0 );
		virtual AST *	check( AST * a = 0 );
	protected:
	AST * m_label;
};



class UseTimer : public UseResource {
	public:
		UseTimer ( AST * label );
};

class UseSensor : public UseResource {
	public:
		UseSensor ( AST * label );

		virtual AST *	eval( AST * a = 0 );
		virtual AST *	check( AST * a = 0 );
};

class UseEventMonitor : public UseResource {
	public:
		UseEventMonitor ( AST * label );
		virtual AST *	eval( AST * a = 0 );
		virtual AST *	check( AST * a = 0 );
		virtual AST	*	init	( AST * a = 0 );
	private:
		AnalogResourceContext * m_FSM;
		AST * m_sigChar;
};

class UseSource : public UseResource {
	public:
		UseSource ( AST * label );
		virtual AST *	eval( AST * a = 0 );
		virtual AST *	check( AST * a = 0 );
};

class UseLoad: public UseResource {
	public:
		UseLoad ( AST * label );
		virtual AST *	eval( AST * a = 0 );
		virtual AST *	check( AST * a = 0 );
};



#endif // VirtualResourceAST_h
