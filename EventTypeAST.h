#ifndef EventTypeAST_h
#define EventTypeAST_h

#include	"AtlasAST.h"
#include	"InitList.h"
#include	"NounsModifiersDimensions.h"
#include	"TimerObject.h"



class	EventTypeAST : public AST{
	public:
		EventTypeAST(TheType t, AST * server1=0,AST * server2=0 );
		virtual	AST *	data	( AST * a = 0 );
		virtual AST *	eval	( AST * a=0  );
		virtual void	setInteger( int indx=0, Long value=0 );
		virtual Long	getInteger( int indx=0 ) const;
		virtual AST *	insert	( AST   * a   );
		virtual AST *	remove	( AST   * a   );
		virtual void	print	( AST * a = 0 ) const;
		virtual TheType	getType	( AST * a=0  ) const;

	protected:
	ASTList * m_clients;
	int m_initialized;
	int m_enabled;
	
	virtual void add_client(AST * );
	virtual void broadcast(AST * );
	AST * m_s1;
	AST * m_s2;
private:
		TheType	_storage;
};


class	AnalogEventType : public EventTypeAST{
	public:
		AnalogEventType( AST * server );
		virtual	AST *	data	( AST * a = 0 );
		virtual AST *	eval	( AST * a=0  );
		virtual void	print	( AST * a = 0 ) const;
		virtual Resource * getResource( );

	private:
};


class	EventIntervalType : public EventTypeAST{
	public:
		EventIntervalType( AST * server1,AST * server2 );
		virtual	AST *	data	( AST * a = 0 );
		virtual AST *	eval	( AST * a=0  );
		virtual void	print	( AST * a = 0 ) const;

	private:
		int fromEvent;
		int toEvent;
};

class	EventIndicatorType : public EventTypeAST{
	public:
		EventIndicatorType( AST * server ,AST * label);
		virtual	AST *	data	( AST * a = 0 );
		virtual AST *	eval	( AST * a=0  );
		virtual void	print	( AST * a = 0 ) const;

	private:
		AST * m_label;
};

class	TimeBasedEventType : public EventTypeAST{
	public:
		TimeBasedEventType( AST * server );
		virtual	AST *	data	( AST * a = 0 );
		virtual AST *	eval	( AST * a=0  );
		virtual void	print	( AST * a = 0 ) const;

	private:
};

class	OneShotTimerType : public EventTypeAST{
	public:
		OneShotTimerType ( );
		virtual	AST *	data	( AST * a = 0 );
		virtual AST *	eval	( AST * a=0  );
		virtual void	print	( AST * a = 0 ) const;

	private:
		TimerObject m_timer;
};

class	PeriodicTimerType : public EventTypeAST{
	public:
		PeriodicTimerType ( );
		virtual	AST *	data	( AST * a = 0 );
		virtual AST *	eval	( AST * a=0  );
		virtual void	print	( AST * a = 0 ) const;

	private:
		TimerObject m_timer;
};


class	PeriodicEventCounterType : public EventTypeAST{
	public:
		PeriodicEventCounterType ( AST * server, AST * final=0 );
		//virtual	AST *	data	( AST * a = 0 );
		virtual AST *	eval	( AST * a=0  );
		//virtual void	print	( AST * a = 0 ) const;

	private:
		AST * m_final;
		int   m_count;
};


class	OnceEventCounterType : public EventTypeAST{
	public:
		OnceEventCounterType ( AST * server,AST * final=0 );
		//virtual	AST *	data	( AST * a = 0 );
		virtual AST *	eval	( AST * a=0  );
		//virtual void	print	( AST * a = 0 ) const;

	private:
		AST * m_final;
		int   m_count;
};


#endif // EventTypeAST_h
