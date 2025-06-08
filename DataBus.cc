#include	"DataBus.h"
#include	"AtlasAST.h"
#include	"Resource.h"
#include	"VirtualResourceAST.h"
#include	"SignalTypeAST.h"
#include	"TedlSignalVerbVisitor.h"
#include	"TedlDeviceAST.h"
#include	"ExceptionTypes.h"


DataBusEM::DataBusEM( Resource * r , AST * nm ,DataBusEM * o)
	{
		setResource(r);
		m_noun_modifier=nm;
		setResourceContextAST( new ResourceContextAST(getResource(),"~MEASURED") );
		m_Original=o;
	}

	
void	DataBusEM::ENABLE_EXCHANGEtoTedl()
	{	
		sayhi("ENABLE_EXCHANGEtoTedl");

		// Match a SIG_CHAR Block from TEDL and stick with it....
		// SETUP is the only one that checks this....
		
		//m_transitionSigChar	= getResource()->RequirementsCheck(getResourceContextAST());
		preSetContext();

		doPrepControl("ENABLE_EXCHANGE");

		toEVENTS(&DataBusResourceContext::ENABLE_EXCHANGE);
	
		doControlAction("ENABLE_EXCHANGE");

	}

void	DataBusEM::CONNECT_EXCHANGEtoTedl()
	{

	
		sayhi("CONNECT_EXCHANGEtoTedl");
		
		preSetContext();

		AST * pin_desc	 = getResource()->getCnx();
		while(pin_desc){
			AST * uut_pin = pin_desc->ASTdown();
			while(uut_pin){
				RWCString uut_pin_name = (uut_pin->eval())->getName();
				RWCString device_name  = getResource()->getName();
				RWCString device_port  = pin_desc->getName();
				

				find_path(uut_pin_name,device_port);

				uut_pin=uut_pin->ASTright();
			}
			pin_desc = pin_desc->ASTright();
		}

		doPrepControl("CONNECT_EXCHANGE");

		toEVENTS(&DataBusResourceContext::CONNECT_EXCHANGE);

		doControlAction("CONNECT_EXCHANGE");

	}

void	DataBusEM::DISCONNECT_EXCHANGEtoTedl()
	{

		sayhi("DISCONNECT_EXCHANGEtoTedl");
	
		preSetContext();

		doPrepControl("DISCONNECT_EXCHANGE");

		toEVENTS(&DataBusResourceContext::DISCONNECT_EXCHANGE);	// Disconnect events first

		AST * pin_desc	 = getResource()->getCnx();
		while(pin_desc){
			AST * uut_pin = pin_desc->ASTdown();
			while(uut_pin){
				RWCString uut_pin_name = (uut_pin->eval())->getName();
				RWCString device_name  = getResource()->getName();
				RWCString device_port  = pin_desc->getName();

				disconnect_path(uut_pin_name,device_port);
				
				uut_pin=uut_pin->ASTright();
			}
			pin_desc = pin_desc->ASTright();
		}


		doControlAction("DISCONNECT_EXCHANGE");
	}

void	DataBusEM::DO_EXCHANGEtoTedl()
	{

		sayhi("DO_EXCHANGEtoTedl");
	
		preSetContext();

		doPrepControl("DO_EXCHANGE");

		toEVENTS(&DataBusResourceContext::DO_EXCHANGE);

		doControlAction("DO_EXCHANGE");
	
	}
	
void	DataBusEM::DISABLE_EXCHANGEtoTedl()
	{

		sayhi("DISABLE_EXCHANGEtoTedl");
	
		preSetContext();

		doPrepControl("DISABLE_EXCHANGE");

		toEVENTS(&DataBusResourceContext::DISABLE_EXCHANGE);

		doControlAction("DISABLE_EXCHANGE");
	
	}

void	DataBusEM::UPDATE_EXCHANGEtoTedl()
	{

		sayhi("UPDATE_EXCHANGEtoTedl");
	
		preSetContext();

		doPrepControl("UPDATE_EXCHANGE");

		toEVENTS(&DataBusResourceContext::UPDATE_EXCHANGE);

		doControlAction("UPDATE_EXCHANGE");
	
	}

void	DataBusEM::FETCH_EXCHANGEtoTedl()
	{

		sayhi("FETCH_EXCHANGEtoTedl");
	
		preSetContext();
		
		doPrepControl("FETCH_EXCHANGE");

		toEVENTS(&DataBusResourceContext::FETCH_EXCHANGE);
		
		if(!doControlAction("FETCH_EXCHANGE")){
			// Error, do not know how to FETCH_EXCHANGE
			Error_Report("This SIG_CHAR does not have a FETCH_EXCHANGE action");
			assert(0);
		}	
						
		
	}



void	DataBusEM::InvalidatedResult()	{}
void	DataBusEM::do_HOLDtoTedl()
	{
		sayhi("do_HOLDtoTedl");

		preSetContext();

		//doPrepControl("do_HOLD");

		//toEVENTS(DataBusResourceContext::do_HOLD);

		//doControlAction("do_HOLD");
	
	}

	
	
void	DataBusEM::do_PROCEEDtoTedl()
	{
		sayhi("do_PROCEEDtoTedl");
	
		preSetContext();

		//doPrepControl("ENABLE");

		//toEVENTS(DataBusResourceContext::do_PROCEED);

		//doControlAction("ENABLE");
	}


void	DataBusEM::FSMError(const std::string& t, const std::string& s)
	{
		RWCString err;
		err	+=	" DATABUS:BAD State Transition attempt error: "+RWCString(t);
		err	+=	" in state "+RWCString(s);
		err	+=	" Device:"+getResource()->getName();
		
		throw TedlExecutionError(err);
	}

void	DataBusEM::setResourceName( RWCString n )
	{
		getResource()->setName( n );
	}

void
DataBusEM::ENABLE_EXCHANGE	(AST * a)	{ setcurrent(a);ENABLE_EXCHANGEevent	();}
void
DataBusEM::CONNECT_EXCHANGE	(AST * a)	{ setcurrent(a);CONNECT_EXCHANGEevent	();}
void
DataBusEM::DO_EXCHANGE		(AST * a)	{ setcurrent(a);DO_EXCHANGEevent	();}
void
DataBusEM::DISABLE_EXCHANGE	(AST * a)	{ setcurrent(a);DISABLE_EXCHANGEevent	();}
void
DataBusEM::DISCONNECT_EXCHANGE	(AST * a)	{ setcurrent(a);DISCONNECT_EXCHANGEevent();}
void
DataBusEM::UPDATE_EXCHANGE	(AST * a)	{ setcurrent(a);UPDATE_EXCHANGEevent	();}
void
DataBusEM::FETCH_EXCHANGE		(AST * a)	{ setcurrent(a);FETCH_EXCHANGEevent	();}

int
DataBusEM::resetFSM()
	{
		DISCONNECT_EXCHANGE();
		SetState(Unallocated_State);
		DISCONNECT_EXCHANGE();
		return DataBusResourceContext::resetFSM();
	}
void DataBusEM::Error(RWCString  e) { FSMError( e,this->GetState().StateName() );}

//---------------------------------------------------------------
	
DataBus::DataBus( Resource * r , AST * nm,DataBus * o):DataBusEM(r,nm,o)
	{ ; }

DataBusResourceContext *
DataBus::clone( Resource * r )
	{
		
		return new DataBus(r,m_noun_modifier,this);
	}

