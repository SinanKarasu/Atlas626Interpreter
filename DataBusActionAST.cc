
#include	"AtlasStd.h"
#include	"AtlasParser.h"

DataBusActionAST::DataBusActionAST(ANTLRTokenPtr p,AST * a):AST(p),m_AST(a){}
DataBusActionAST::DataBusActionAST(AST * a ):AST(0),m_AST(a){}
DataBusActionAST::~DataBusActionAST(){}

AST *
DataBusActionAST::Accept	( ASTVisitor & v )	{ return v.VisitDataBusActionAST( this ); }


ThreadContext	::	ThreadContext ( AST * parent , int wait)
					:DataBusActionAST(0,0),m_parent(parent)
				{
					m_arg=new arg_struct;
					m_arg->a	=0;
					//m_arg->back_c	=new pthread_cond_t;
					if(wait){
						m_arg->gang_c	=new pthread_cond_t;
						m_arg->sp	=new sem_t;
						sem_init(m_arg->sp,0,0);
					}
					//m_arg->m	=new pthread_mutex_t;
					m_arg->external=1;

				}
	
AST *
ThreadContext	::	init( AST * a )
				{
					if(a==0){	// initialize
					} else {
						m_arg->what	=a;
						m_timer.schedule(m_arg,0.0,0.0,0);
					}
					return 0;	
				}

void
ThreadContext	::	gang_trigger()
				{
					pthread_mutex_lock(m_arg->m);
					pthread_cond_broadcast(m_arg->gang_c);
					pthread_mutex_unlock(m_arg->m);
				}

AST *
ThreadContext	::	eval( AST * a )
				{
					if(a==m_parent){	// wait for any
						gang_trigger();
						sem_wait(m_arg->sp);
						return this;
					} else if(a==0){
						int count=0;
						gang_trigger();
						while(count<m_parent->length()){
							sem_wait(m_arg->sp);
						}
						return this;
					} else {
						return 0;
					}
				}







InitializeExchangeAction	::	InitializeExchangeAction ( ANTLRTokenPtr p )
					:DataBusActionAST(p,0){}
	
AST *
InitializeExchangeAction	::	eval( AST * a )	{ return this; }


SetupExchangeAction	::	SetupExchangeAction ( ANTLRTokenPtr p )
				:DataBusActionAST(p){}
	
AST *
SetupExchangeAction	::	eval( AST * a )	{ return this; }

StartExchangeAction	::	StartExchangeAction ( ANTLRTokenPtr p )
				:DataBusActionAST(p){}
	
AST *
StartExchangeAction	::	eval( AST * a )
				{
					evalVisitor	go;
		
					go.Execute( ASTdown(), 0 );
					return 0;
				}

ProtocolUpdateAction	::	ProtocolUpdateAction ( AST * a )
				:DataBusActionAST(a){}
	
AST *
ProtocolUpdateAction	::	eval( AST * a )	{ return this; }

ExchangeUpdateAction	::	ExchangeUpdateAction ( ANTLRTokenPtr p )
				:DataBusActionAST(p){}
	
AST *
ExchangeUpdateAction	::	eval( AST * a )	{ return this; }

DataBusUpdateAction	::	DataBusUpdateAction ( ANTLRTokenPtr p ,AST * a)
				:DataBusActionAST(p){}
	
AST *
DataBusUpdateAction	::	eval( AST * a )	{ return this; }

ParameterUpdateAction	::	ParameterUpdateAction ( AST * a )
				:DataBusActionAST(a){}
	
AST *
ParameterUpdateAction	::	eval( AST * a )	{ return this; }

SpawnTaskAction	::	SpawnTaskAction ( AST * task )
				:DataBusActionAST(0),m_task(task)
				{
					m_context=new ThreadContext(this);
				}
	
AST *
SpawnTaskAction		::	eval( AST * a )
				{
					m_context->init(0);	// message says "INITIALIZE"
					m_context->init(m_task);
					m_context->eval(m_task);	// message says "NO WAIT"
					return 0;
				}

SpawnTasksAllAction	::	SpawnTasksAllAction ( ASTList * tasks )
				:DataBusActionAST(0),m_tasks(tasks)
				{
					m_context=new ThreadContext(this,1);
				}
	
AST *
SpawnTasksAllAction	::	eval( AST * a )
				{
					m_context->init(0);	// message says "INITIALIZE"
					//ASTListIterator tit(*m_tasks);
					for(const auto& tit: *m_tasks) {
					//while(++tit){
						m_context->init(tit);
					}
					m_context->eval(0);	// message says "WAIT FOR ALL"
					return 0;
				}

Long
SpawnTasksAllAction	::	length	( int indx ) const
				{
					return m_tasks->size();
				}

SpawnTasksAnyAction	::	SpawnTasksAnyAction ( ASTList * tasks )
				:DataBusActionAST(0),m_tasks(tasks)
				{
					m_context=new ThreadContext(this,1);
				}
	
AST *
SpawnTasksAnyAction	::	eval( AST * a )
				{
					m_context->init(0);	// message says "INITIALIZE"
					//ASTListIterator tit(*m_tasks);
					for(const auto& tit: *m_tasks) {					
					//while(++tit){
						m_context->init(tit);
					}
					m_context->eval(this);	// message says "WAIT FOR ANY"
					return 0;
				}

ExchangeProtocolInitializationAction	::	ExchangeProtocolInitializationAction ( ANTLRTokenPtr p )
						:DataBusActionAST(p){}
	
AST *
ExchangeProtocolInitializationAction	::	eval( AST * a )	{ return this; }

ProtocolInitializationAction	::	ProtocolInitializationAction ( ANTLRTokenPtr p )
					:DataBusActionAST(p){}
	
AST *
ProtocolInitializationAction	::	eval( AST * a )	{ return this; }


AST *
ProtocolInitializationAction	::	data	( AST * a )
	{
		return ASTdown();
	}

TalkerSimulateAction	::	TalkerSimulateAction ( ANTLRTokenPtr p )
				:DataBusActionAST(p){}
	
AST *
TalkerSimulateAction	::	eval( AST * a )	{ return this; }

ListenerSimulateAction	::	ListenerSimulateAction ( ANTLRTokenPtr p )
				:DataBusActionAST(p){}
	
AST *
ListenerSimulateAction	::	eval( AST * a )	{ return this; }

TestEquipSimulateAction	::	TestEquipSimulateAction ( ANTLRTokenPtr p )
				:DataBusActionAST(p){}
	
AST *
TestEquipSimulateAction	::	eval( AST * a )	{ return this; }



ExchangeSimulateAction	::	ExchangeSimulateAction ( ANTLRTokenPtr p )
				:DataBusActionAST(p){}
	
AST *
ExchangeSimulateAction	::	eval( AST * a )	{ return this; }

FetchDataBusAction	::	FetchDataBusAction ( ANTLRTokenPtr p )
				:DataBusActionAST(p){}
	
AST *
FetchDataBusAction	::	eval( AST * a )	{ return this; }
