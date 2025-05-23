#ifndef DataBusActionAST_h
#define DataBusActionAST_h



#include	"InitList.h"
#include	"BasicTypeAST.h"
#include	"TimerObject.h"

class DataBusActionAST : public AST {
	public:
		DataBusActionAST(ANTLRTokenPtr p,AST * a=0);
		DataBusActionAST(AST * a);
		virtual ~DataBusActionAST();

		virtual AST *	Accept	( ASTVisitor & );
	protected:
		AST * m_AST;	
};


class ThreadContext : public DataBusActionAST {
	public:
		ThreadContext ( AST * parent ,int wait=0);

		virtual AST *	eval		( AST * a = 0 );
		virtual	AST *	init		( AST * a = 0 );
	private:
		TimerObject			m_timer;
		AST 		*	m_parent;
		arg_struct	*	m_arg;
		void gang_trigger();
};


class InitializeExchangeAction : public DataBusActionAST {
	public:
		InitializeExchangeAction ( ANTLRTokenPtr p=NULL );

		virtual AST *	eval		( AST * a=0 );
	private:
		int argcount;
};

class SetupExchangeAction : public DataBusActionAST {
	public:
		SetupExchangeAction ( ANTLRTokenPtr p=NULL );

		virtual AST *	eval		( AST * a=0 );
	private:
		int argcount;
};

class StartExchangeAction : public DataBusActionAST {
	public:
		StartExchangeAction ( ANTLRTokenPtr p=NULL );

		virtual AST *	eval		( AST * a=0 );
	private:
		int argcount;
};

class ProtocolUpdateAction : public DataBusActionAST {
	public:
		ProtocolUpdateAction ( AST * a );

		virtual AST *	eval		( AST * a=0 );
	private:
		int argcount;
};

class ExchangeUpdateAction : public DataBusActionAST {
	public:
		ExchangeUpdateAction ( ANTLRTokenPtr p=NULL );

		virtual AST *	eval		( AST * a=0 );
	private:
		int argcount;
};

class DataBusUpdateAction : public DataBusActionAST {
	public:
		DataBusUpdateAction ( ANTLRTokenPtr p,AST * a );

		virtual AST *	eval		( AST * a=0 );
	private:
		int argcount;
};

class ParameterUpdateAction : public DataBusActionAST {
	public:
		ParameterUpdateAction ( AST * a );

		virtual AST *	eval		( AST * a=0 );
	private:
		int argcount;
};


class SpawnTaskAction : public DataBusActionAST {
	public:
		SpawnTaskAction ( AST * task );

		virtual AST *	eval		( AST * a=0 );
	private:
		AST *  m_task;
		ThreadContext * m_context;
};


class SpawnTasksAllAction : public DataBusActionAST {
	public:
		SpawnTasksAllAction ( ASTList * tasks );

		virtual AST *	eval		( AST * a=0 );
		virtual	Long	length	( int indx = 0 ) const;
	private:
		ASTList *  m_tasks;
		ThreadContext * m_context;
};

class SpawnTasksAnyAction : public DataBusActionAST {
	public:
		SpawnTasksAnyAction ( ASTList * tasks );

		virtual AST *	eval		( AST * a=0 );
	private:
		ASTList *  m_tasks;
		ThreadContext * m_context;
};

class ExchangeProtocolInitializationAction : public DataBusActionAST {
	public:
		ExchangeProtocolInitializationAction ( ANTLRTokenPtr p=NULL );

		virtual AST *	eval		( AST * a=0 );
	private:
		int argcount;
};

class ProtocolInitializationAction : public DataBusActionAST {
	public:
		ProtocolInitializationAction ( ANTLRTokenPtr p=NULL );

		virtual AST *	eval		( AST * a=0 );
		virtual	AST	*	data	( AST * a = 0 );
	private:
		int argcount;
};

class TalkerSimulateAction : public DataBusActionAST {
	public:
		TalkerSimulateAction ( ANTLRTokenPtr p=NULL );

		virtual AST *	eval		( AST * a=0 );
	private:
		int argcount;
};

class ListenerSimulateAction : public DataBusActionAST {
	public:
		ListenerSimulateAction ( ANTLRTokenPtr p=NULL );

		virtual AST *	eval		( AST * a=0 );
	private:
		int argcount;
};

class TestEquipSimulateAction : public DataBusActionAST {
	public:
		TestEquipSimulateAction ( ANTLRTokenPtr p=NULL );

		virtual AST *	eval		( AST * a=0 );
	private:
		int argcount;
};

class ExchangeSimulateAction : public DataBusActionAST {
	public:
		ExchangeSimulateAction ( ANTLRTokenPtr p=NULL );

		virtual AST *	eval		( AST * a=0 );
	private:
		int argcount;
};

class FetchDataBusAction : public DataBusActionAST {
	public:
		FetchDataBusAction ( ANTLRTokenPtr p=NULL );

		virtual AST *	eval		( AST * a=0 );
	private:
		int argcount;
};


#endif // DataBusActionAST_h
