
#include	<dlfcn.h>

#include	"Sensor.h"
#include	"AtlasAST.h"
#include	"Resource.h"
#include	"VirtualResourceAST.h"
#include	"SignalTypeAST.h"
#include	"TedlSignalVerbVisitor.h"
#include	"TedlDeviceAST.h"
#include	"ExceptionTypes.h"

SensorEM::SensorEM( Resource * r , AST * nm ,SensorEM * o)
	{
		setResource(r);
		m_noun_modifier=nm;
		setResourceContextAST(  new ResourceContextAST(getResource(),"~MEASURED"));
		m_Original=o;
	}

	
void	SensorEM::SETUPtoTedl()
	{	
		AnalogResourceContext::SETUPtoTedl();
	}

void	SensorEM::CONNECTtoTedl()
	{
		AnalogResourceContext::CONNECTtoTedl();
	}


void SensorEM::DISCONNECTtoTedl()
	{
		AnalogResourceContext::DISCONNECTtoTedl();
	}

void	SensorEM::CHANGEtoTedl()
	{
		AnalogResourceContext::CHANGEtoTedl();
	}
	
void	SensorEM::REMOVEtoTedl()
	{
		AnalogResourceContext::REMOVEtoTedl();
	}

void	SensorEM::MEASUREtoTedl()
	{
		AnalogResourceContext::MEASUREtoTedl();
	}

void	SensorEM::MONITORtoTedl()
	{
		AnalogResourceContext::MONITORtoTedl();
	}

void	SensorEM::VERIFYtoTedl()
	{
		AnalogResourceContext::VERIFYtoTedl();
	}

void	SensorEM::WAIT_FORtoTedl()	{}

void	SensorEM::ARMtoTedl()
	{
		AnalogResourceContext::ARMtoTedl();
	}

void	SensorEM::FETCHtoTedl()
	{

		sayhi("FETCHtoTedl");
	
		preSetContext();
		
		doPrepControl("FETCH");

		toEVENTS(&AnalogResourceContext::FETCH);
		
		if(!doControlAction("FETCH")){
			// Error, do not know how to FETCH
			Error_Report("This SIG_CHAR does not have a FETCH action");
			assert(0);
		}	
						
		
	}

void	SensorEM::READtoTedl()
	{
		INITIATE();
		FETCH();
	}
	

void	SensorEM::InvalidatedResult()	{}
void	SensorEM::MeasuredtoTedl()	{}
void	SensorEM::RESETtoTedl()
	{
		AnalogResourceContext::RESETtoTedl();
	}

void	SensorEM::COMPAREtoTedl()
	{
		AnalogResourceContext::COMPAREtoTedl();
	}
void	SensorEM::INITIATEtoTedl()
	{
		ARM();
		if(EventsIntroduced()){
			ENABLE_EVENT();
		} else {
			SkipState();	// need a solution.....
		}
	}
	
void	SensorEM::APPLYtoTedl()		{assert(0);} // Not in a SENSOR	
	
void	SensorEM::ENABLE_EVENTtoTedl()
	{
		AnalogResourceContext::ENABLE_EVENTtoTedl();
	}

void	SensorEM::DISABLE_EVENTtoTedl()
	{
		AnalogResourceContext::DISABLE_EVENTtoTedl();
	}


void	SensorEM::FSMError(const std::string& t, const std::string& s)
	{
		RWCString err;
		err	+=	" SENSOR:BAD State Transition attempt error: "+RWCString(t);
		err	+=	" in state "+RWCString(s);
		err	+=	" Device:"+getResource()->getName();
		
		throw TedlExecutionError(err);
	}

void	SensorEM::setResourceName( RWCString n )
	{
		getResource()->setName( n );
	}

// EVENTS for Sensor.
void
SensorEM::ARM		(AST * a)	{ setcurrent(a);ARMevent		(); }
void
SensorEM::CHANGE	(AST * a)	{ setcurrent(a);CHANGEevent		(); }
void
SensorEM::COMPARE	(AST * a)	{ setcurrent(a);COMPAREevent		(); }
void
SensorEM::CONNECT	(AST * a)	{ setcurrent(a);CONNECTevent		(); }
void
SensorEM::DISABLE_EVENT	(AST * a)	{ setcurrent(a);DISABLE_EVENTevent	(); }
void
SensorEM::DISCONNECT	(AST * a)	{ setcurrent(a);DISCONNECTevent		(); }
void
SensorEM::_DISCONNECT	(AST * a)	{ setcurrent(a);DISCONNECTedevent	(); }
void
SensorEM::ENABLE_EVENT	(AST * a)	{ setcurrent(a);ENABLE_EVENTevent	(); }
void
SensorEM::FETCH		(AST * a)	{ setcurrent(a);FETCHevent		(); }
void
SensorEM::INITIATE	(AST * a)	{ setcurrent(a);INITIATEevent		(); }
void
SensorEM::MAXTIME	(AST * a)	{ setcurrent(a);MAXTIMEevent		(); }
void
SensorEM::MEASURE	(AST * a)	{ setcurrent(a);MEASUREevent		(); }
void
SensorEM::MEASUREMENT	(AST * a)	{ setcurrent(a);MEASUREMENTevent	(); }
void
SensorEM::MONITOR	(AST * a)	{ setcurrent(a);MONITORevent		(); }
void
SensorEM::READ		(AST * a)	{ setcurrent(a);READevent		(); }
void
SensorEM::REMOVE	(AST * a)	{ setcurrent(a);REMOVEevent		(); }
void
SensorEM::RESET		(AST * a)	{ setcurrent(a);RESETevent		(); }
void
SensorEM::SETUP		(AST * a)	{ setcurrent(a);SETUPevent		(); }
void
SensorEM::VERIFY	(AST * a)	{ setcurrent(a);VERIFYevent		(); }
void
SensorEM::AsyncReset	(AST * a)	{ setcurrent(a);AsyncResetevent		(); }
void
SensorEM::ResourceReset	(AST * a)	{ setcurrent(a);ResourceResetevent	(); }
void
SensorEM::SkipState	(AST * a)	{ setcurrent(a);SkipStateevent		(); }
void
SensorEM::Select	(AST * a)	{ setcurrent(a);Selectevent		(); }
			
void
SensorEM::APPLY		(AST * a)
	{
		setcurrent(a);
		Error( "APPLYevent  illegal for SENSOR");
	}



int
SensorEM::resetFSM()
			{
				return AnalogResourceContext::resetFSM();
			}

void
SensorEM::Error(RWCString  e)
	{
		FSMError( e,this->GetState().StateName() );
	}

//-----------------------------------
	
Sensor::Sensor( Resource * r , AST * nm,Sensor * o):SensorEM(r,nm,o)
	{
	}

AnalogResourceContext * Sensor::clone( Resource * r )
	{
		
		return new Sensor(r,m_noun_modifier,this);
	}
