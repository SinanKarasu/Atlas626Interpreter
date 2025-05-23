
#include	"SignalActionAST.h"
#include	"Visitors.h"
#include	"TimerObject.h"
#include	"TedlDefinitions.h"
#include	"TedlParser.h"

extern TedlParser	*	TEDL;

SignalActionAST::SignalActionAST( ANTLRTokenPtr p ,TheType type ):m_requireType(type)
	{
	}

SignalActionAST::~SignalActionAST(){}

RWCString	SignalActionAST::getName() const
			{
				return ASTdown()->getName();
			}
TheType	SignalActionAST::getType	( AST * a ) const	{ return m_requireType; }
	
AST *	SignalActionAST::eval	( AST * a )
	{
		if ( a == this ){
			return ASTdown()->eval( ASTdown() );
		} else if (a) {
			return ASTdown()->eval( a );
		} else {
			return this;
		}
	}

AST *	SignalActionAST::data(AST * a)			{ return ASTdown()->data( a ); }
	
AST *	SignalActionAST::assign	( AST * a )		{ return this; }

AST *	SignalActionAST::check ( AST * a )
	{
		return ASTdown()->check(a);
	}	

Resource * SignalActionAST::getResource( )
			{
				return ASTdown()->getResource();
			}

AST *	SignalActionAST::entity()			{ return ASTdown(); }

AST *	SignalActionAST::Accept	( ASTVisitor & v )	{ return v.VisitSignalActionAST( this ); }

DoSimulAction::DoSimulAction( ANTLRTokenPtr p ){};

//RequireAnalogAction::RequireAnalogAction( ANTLRTokenPtr p ){}

RequireControlAction::RequireControlAction( ANTLRTokenPtr p )
	:SignalActionAST(p,ControlModifierType)
	{
	}
	
AST *
RequireControlAction::max		( AST * a )
	{
		return entity()->max();
	}

AST *
RequireControlAction::min		( AST * a )
	{
		return entity()->min();
	}
AST *
RequireControlAction::check ( AST * a )
	{
		if(!SignalActionAST::check(a)){
			return 0;
		}
		return this;
	}	


RequireCapabilityAction::RequireCapabilityAction( ANTLRTokenPtr p )
	:SignalActionAST(p,CapabilityModifierType)
	{
	}
AST *
RequireCapabilityAction::max		( AST * a )
	{
		return entity()->max();
	}

AST *
RequireCapabilityAction::min		( AST * a )
	{
		return entity()->min();
	}
AST *
RequireCapabilityAction::check ( AST * a )
	{
		if(!SignalActionAST::check(a)){
			return 0;
		}
		return this;
	}	


RequireLimitAction::RequireLimitAction( ANTLRTokenPtr p )
	:SignalActionAST(p,CapabilityModifierType)
	{
	}

AST *
RequireLimitAction::max		( AST * a )
	{
		return entity()->max();
	}

AST *
RequireLimitAction::min		( AST * a )
	{
		return entity()->min();
	}

AST *
RequireLimitAction::check ( AST * a )
	{
		if(!a->check(this)){
			return 0;
		}
		
		return this;
	}	



RequireCnxAction::RequireCnxAction( ANTLRTokenPtr p ){}

AST *	RequireCnxAction::check	( AST * a )
	{
		AST * req_pd;
		if(req_pd=ASTdown()){
			while(req_pd){
				if(!req_pd->check(a)){
					return 0;
				}
				req_pd=req_pd->ASTright();
			}
		}
		return this;
	}	


SyncRequirementAction::SyncRequirementAction( ANTLRTokenPtr p ){}

CnxAction::CnxAction( ANTLRTokenPtr p ){}

AST *	CnxAction::eval	( AST * a )
	{
		if ( a != 0 )
			return ASTdown()->eval( a );
		else
			return this;
	}
AST *	CnxAction::data(AST * a)
	{
		return ASTdown();
	}


MaxTimeAction::MaxTimeAction( AST * tq ,AST * p):m_tq(tq),m_max_time(p)
	{
	
	}

AST *	MaxTimeAction::eval	( AST * a )
	{
		TimerObject timer;
		AST * t=m_tq->eval();
		m_max_time->setInteger(0,0);
		int err=timer.execute_timed(t->getDecimal(),ASTdown(),a);
		if(err==ETIMEDOUT){
				m_max_time->setInteger(0,1);
		}
		return 0;
	}

WhenFieldAction::WhenFieldAction ( AST * w ):m_w(w)
	{
	
	}

AST *	WhenFieldAction::eval	( AST * a )
	{
		m_w->eval();
		return ASTdown()->eval( a );
	}

WaitForTimerAction::WaitForTimerAction( ANTLRTokenPtr p ){};
AST *	WaitForTimerAction::eval( AST *a )
	{
		return 0;
	};


WaitForEventAction::WaitForEventAction( ANTLRTokenPtr p ){};
AST *	WaitForEventAction::eval( AST *a )
	{
		ASTdown()->eval();
		return 0;
	};


ConstraintMeasurementAction::ConstraintMeasurementAction( ANTLRTokenPtr p )
	{
	}

AST *	ConstraintMeasurementAction::assign	( AST * a )
	{
		return this;
	}

ConstraintInclusionAction::ConstraintInclusionAction( ANTLRTokenPtr p ){}

WaitForTimeAction	::	WaitForTimeAction	( ANTLRTokenPtr p ){}
AST *	WaitForTimeAction::eval( AST *a )
	{
                timespec        rqtp;
		double time=ASTdown()->getDecimal(1);// 1 means in seconds...

                rqtp.tv_sec  =  time;
                rqtp.tv_nsec = (time-rqtp.tv_sec)*1e9;

		nanosleep(&rqtp,0);
		return 0;
	};

WaitForManualAction	::	WaitForManualAction	( AST * label )
	:m_label(label),m_waiting(1)
	{
	}
	
AST *	WaitForManualAction::eval( AST *a )
	{
		if(a==this){
			if(m_waiting){
				sem_post(&sem);
			}
		} else {
			m_waiting=1;
			m_label->add(this);
			int whatitwas=m_label->getInteger();
			m_label->setInteger(0,1);
			sem_wait(&sem);
			m_label->setInteger(0,whatitwas);
			m_waiting=0;
		}
		return 0;
	};
	

RemoveAction		::	RemoveAction	( ANTLRTokenPtr p ){}

AST *
RemoveAction::eval	( AST * a )
	{
		if(ASTdown()){
			return ASTdown()->eval	(  a );
		} else if(TEDL !=NULL ){
			TEDL->removeAll(1);
		}
		return 0;
	}
RemoveDrawingAction	::	RemoveDrawingAction	( ANTLRTokenPtr p ){}

FinishAction		::	FinishAction	( ANTLRTokenPtr p ){}

AST *
FinishAction::eval	( AST * a )
	{
		if(TEDL!=NULL){
			TEDL->removeAll(1);
		}
		return 0;
	}
