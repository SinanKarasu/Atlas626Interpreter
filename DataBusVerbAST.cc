
#include	"DataBusVerbAST.h"
#include	"VirtualResourceAST.h"
#include	"TedlDefinitions.h"
#include	"TedlParser.h"
#include	"Visitors.h"
#include	"ExceptionTypes.h"



DataBusVerbAST::DataBusVerbAST( ANTLRTokenPtr p ){}
DataBusVerbAST::~DataBusVerbAST(){}
	
AST *
DataBusVerbAST::assign	( AST * a )		{ return this; }

AST *
DataBusVerbAST::check	( AST * a )
	{		
		m_device->RequirementsCheck( a );
		assert(0);
		return 0;
	};

AST *
DataBusVerbAST::Accept( ASTVisitor & v )		{ return v.VisitDataBusVerbAST( this ); }

RWCString
DataBusVerbAST::getName() const		{ return ASTdown()->getName(); }


void
DataBusVerbAST::toFSM(void (DataBusResourceContext::*c)(AST *),int mult)
	{
		AST * useResource=0;
		AST * em=ASTdown();
		while(em){
			if(useResource=em->eval()){
				Resource * r=useResource->getResource();
				if(r->m_currentDataBusFSM){
					(r->m_currentDataBusFSM->*c)(useResource);
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
DataBusVerbAST::initialize()
	{
		return ASTdown()->init();
	}


//------------------------------------------------------------------

EstablishProtocolVerb::EstablishProtocolVerb(ANTLRTokenPtr p)
        	:DataBusVerbAST(p){;}

AST *
EstablishProtocolVerb::eval(AST * a)
	{
		return DataBusVerbAST::eval(a);
	}

//------------------------------------------------------------------

DoExchangeVerb::DoExchangeVerb(ANTLRTokenPtr p)
        	:DataBusVerbAST(p){;}

AST *
DoExchangeVerb::eval(AST * a)
	{
		ASTdown()->init();
		toFSM(&DataBusResourceContext::DO_EXCHANGE);
		return 0;
	}

//------------------------------------------------------------------

UpdateExchangeVerb::UpdateExchangeVerb(ANTLRTokenPtr p)
        	:DataBusVerbAST(p){;}

AST *
UpdateExchangeVerb::eval(AST * a)
	{
		ASTdown()->init();
		toFSM(&DataBusResourceContext::UPDATE_EXCHANGE);
		return 0;
	}

//------------------------------------------------------------------

EnableExchangeConfigurationVerb::EnableExchangeConfigurationVerb(ANTLRTokenPtr p)
        	:DataBusVerbAST(p){;}

AST *
EnableExchangeConfigurationVerb::eval(AST * a)
	{
		ASTdown()->init();
		toFSM(&DataBusResourceContext::ENABLE_EXCHANGE);
		return 0;
	}

//------------------------------------------------------------------

DisableExchangeConfigurationVerb::DisableExchangeConfigurationVerb(ANTLRTokenPtr p)
        	:DataBusVerbAST(p){;}

AST *
DisableExchangeConfigurationVerb::eval(AST * a)
	{
		ASTdown()->init();
		toFSM(&DataBusResourceContext::DISABLE_EXCHANGE);
		return 0;
	}

//------------------------------------------------------------------

FetchExchangeVerb::FetchExchangeVerb(ANTLRTokenPtr p)
        :DataBusVerbAST(p){;}
AST *
FetchExchangeVerb::eval(AST * a)
		{
		ASTdown()->init();
		toFSM(&DataBusResourceContext::FETCH_EXCHANGE);
		return 0;
		}
