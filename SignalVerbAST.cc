
#include	"SignalVerbAST.h"
#include	"VirtualResourceAST.h"
#include	"TedlDefinitions.h"
#include	"TedlParser.h"
#include	"Visitors.h"
#include	"ExceptionTypes.h"

extern astream sout;


//class setupVisitor : public NullVisitor {
//
//	public:
//		setupVisitor();
//		
//		void	Execute( AST *, AST * );
//		
//		virtual AST *	VisitActionAST		( ActionAST *		);
//		virtual AST *	VisitBasicTypeAST	( BasicTypeAST *	);
//		virtual AST *	VisitBuiltinFunctionAST	( BuiltinFunctionAST *	);
//		virtual AST *	VisitLabelAST		( LabelAST *		);
//		virtual AST *	VisitOperatorAST	( OperatorAST *		);
//		virtual AST *	VisitSignalActionAST	( SignalActionAST *	);
//		virtual AST *	VisitSignalOperatorAST	( SignalOperatorAST *	);
//		virtual AST *	VisitSignalTypeAST	( SignalTypeAST *	);
//		virtual AST *	VisitSignalVerbAST	( SignalVerbAST *	);
//		virtual AST *	VisitVerbAST		( VerbAST *		);
//		virtual AST *	VisitVirtualResourceAST	( VirtualResourceAST *	);
//		virtual AST *	VisitATEActionAST	( ATEActionAST *	);
//		virtual AST *	VisitATEFieldTypeAST	( ATEFieldTypeAST *	);
//		virtual AST *	VisitATEResourceAST	( ATEResourceAST *	);
//		virtual AST *	VisitGateConnEventAST	( GateConnEventAST *	);
//};

SignalVerbAST::SignalVerbAST( ANTLRTokenPtr p ){}
SignalVerbAST::~SignalVerbAST(){}
	
AST *
SignalVerbAST::assign	( AST * a )		{ return this; }

AST *
SignalVerbAST::check	( AST * a )
	{		
		m_device->RequirementsCheck( a );
		assert(0);
		return 0;
	};

AST *
SignalVerbAST::Accept( ASTVisitor & v )		{ return v.VisitSignalVerbAST( this ); }

RWCString
SignalVerbAST::getName() const		{ return ASTdown()->getName(); }


void
SignalVerbAST::toFSM(void (AnalogResourceContext::*c)(AST *),int mult)
	{
		AST * useResource=0;
		AST * em=ASTdown();
		while(em){
			if(useResource=em->eval()){
				Resource * r=useResource->getResource();
				if(r->m_currentAnalogFSM){
					(r->m_currentAnalogFSM->*c)(useResource);
				}
			}
			if(mult){
				em=em->ASTright();
			} else {
				em=0;
			}
		}
	}

AST *
SignalVerbAST::initialize()
	{
		return ASTdown()->init();
	}

SetupVerb::SetupVerb( ANTLRTokenPtr p )
	{
	};

AST *
SetupVerb::eval( AST *a )
	{
		ASTdown()->init();
		toFSM(&AnalogResourceContext::SETUP);
		return 0;
	};



ConnectVerb::ConnectVerb( ANTLRTokenPtr p )
	{
	};

AST *
ConnectVerb::eval( AST *a )
	{	
		toFSM(&AnalogResourceContext::CONNECT);
		return 0;
	};


FetchVerb::FetchVerb( ANTLRTokenPtr p )
	{
	};

AST *
FetchVerb::eval( AST *a )
	{	
		toFSM(&AnalogResourceContext::SkipState);
		toFSM(&AnalogResourceContext::SkipState);
		toFSM(&AnalogResourceContext::FETCH);
		return 0;
	};


InitiateVerb::InitiateVerb( ANTLRTokenPtr p )
	{
	};

AST *
InitiateVerb::eval( AST *a )
	{	
		toFSM(&AnalogResourceContext::INITIATE);
		return 0;
	};

ReadVerb::ReadVerb( ANTLRTokenPtr p )
	{
	};

AST *
ReadVerb::eval( AST *a )
	{	
		toFSM(&AnalogResourceContext::READ);
		return 0;
	};

ChangeVerb::ChangeVerb( ANTLRTokenPtr p )
	{
	};

AST *
ChangeVerb::eval( AST *a )
	{	
		toFSM(&AnalogResourceContext::CHANGE);
		return 0;
	};


DisconnectVerb::DisconnectVerb( ANTLRTokenPtr p )
	{
	};

AST *
DisconnectVerb::eval( AST *a )
	{	
		toFSM(&AnalogResourceContext::DISCONNECT);
		return 0;
	};

RemoveVerb::RemoveVerb( ANTLRTokenPtr p )
	{
	};

AST *
RemoveVerb::eval( AST *a )
	{	
		toFSM(&AnalogResourceContext::REMOVE);
		return 0;
	};


ArmVerb::ArmVerb( ANTLRTokenPtr p )
	{
	};

AST *
ArmVerb::eval( AST *a )
	{	
		toFSM(&AnalogResourceContext::ARM);
		return 0;
	};


EnableEventVerb::EnableEventVerb( ANTLRTokenPtr p )
	{
	};

AST *
EnableEventVerb::eval( AST *a )
	{	
		toFSM(&AnalogResourceContext::ENABLE_EVENT,1);
		return 0;
	};


DisableEventVerb::DisableEventVerb( ANTLRTokenPtr p )
	{
	};

AST *
DisableEventVerb::eval( AST *a )
	{	
		toFSM(&AnalogResourceContext::DISABLE_EVENT,1);
		return 0;
	};


ApplyVerb::ApplyVerb( ANTLRTokenPtr p )
	{
	};

AST *
ApplyVerb::eval( AST *a )
	{	
		initialize();
		toFSM(&AnalogResourceContext::APPLY);
		return 0;
	};


ResetVerb::ResetVerb( ANTLRTokenPtr p )
	{
	};

AST *
ResetVerb::eval( AST *a )
	{	
		toFSM(&AnalogResourceContext::RESET);
		return 0;
	};


MeasureVerb::MeasureVerb( ANTLRTokenPtr p )
	{
	};

AST *
MeasureVerb::eval( AST *a )
	{	
		initialize();
		toFSM(&AnalogResourceContext::MEASURE);
		return 0;
	};


MonitorVerb::MonitorVerb( ANTLRTokenPtr p )
	{
	};

AST *
MonitorVerb::eval( AST *a )
	{	
		initialize();
		toFSM(&AnalogResourceContext::MONITOR);
		return 0;
	};

VerifyVerb::VerifyVerb( Scope * s ):SignalVerbAST(0),m_scope(s)
	{
	}

AST *
VerifyVerb::eval( AST *a )
	{	
		extern astream sout;
		initialize();
		toFSM(&AnalogResourceContext::VERIFY);
		return 0;
	};


AST *
VerifyVerb::printEvaluation(AST * a)
	{

		AST * comp=ASTdown()->ASTright();
		comp->eval(); // compare field
		StringAST cnx("~CNX");
		AST * c=ASTdown()->data(&cnx);
		//throw PrintEvaluationRequest("VERIFY",comp);
		sout << m_scope ;
		sout << "VERIFY";
		sout << ",";
		sout << comp ;
		if(c){
			sout << c;
		}
		sout << std::endl;
		return this;
	}



WaitForVerb::WaitForVerb( ANTLRTokenPtr p ){};
AST *
WaitForVerb::eval( AST *a )
	{
		if(a==this){
			ASTdown()->remove(this);
			sem_post(&m_sem);
		} else {
			sem_init(&m_sem,0,0);
			ASTdown()->insert(this);
			ASTdown()->eval();
			sem_wait(&m_sem);
		}
		return 0;
	};

WaitForManualVerb::WaitForManualVerb( ANTLRTokenPtr p ){};
AST *
WaitForManualVerb::eval( AST *a )
	{
		ASTdown()->eval();
		return 0;
	};


EnableManualVerb::EnableManualVerb( AST * label  ):m_label(label){}
AST *
EnableManualVerb::eval( AST *a )
	{
		m_label->setInteger(0,1);;
		return 0;
	};

DisableManualVerb::DisableManualVerb( AST * label  ):m_label(label){}
AST *
DisableManualVerb::eval( AST *a )
	{
		m_label->setInteger(0,0);;
		return 0;
	};

FinishVerb::FinishVerb( ANTLRTokenPtr p )
	{
	};

AST *
FinishVerb::eval( AST *a )
	{	
		// ASTdown() is FinishAction, which will
		// Reset the ATE and return NULL to the
		// Execute loop. Which will terminate program...
		return ASTdown();
	};

EnableEscapeVerb::EnableEscapeVerb(AtlasParser * ap)
        	:SignalVerbAST(0),m_ap(ap){;}
       
AST *
EnableEscapeVerb::eval(AST * a)
	{
		AST * ei=ASTdown()->eval();	// escape_indicator
		AST * ep=ASTdown()->ASTright();	// escape_procedure
		m_ap->insertEscape(ei,ep);
		return 0;
	}

DisableEscapeVerb::DisableEscapeVerb(AtlasParser * ap)
        	:SignalVerbAST(0),m_ap(ap){;}
       
AST *
DisableEscapeVerb::eval(AST * a)
	{
		AST * ei=0;
		if(ASTdown()){
			ei=ASTdown()->eval();	// escape_indicator
		}
		m_ap->removeEscape(ei);
		return 0;
	}

