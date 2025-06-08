
#include	"EventMonitor.h"
#include	"AtlasAST.h"
#include	"Resource.h"
#include	"VirtualResourceAST.h"
#include	"SignalTypeAST.h"
#include	"TedlSignalVerbVisitor.h"
#include	"TedlDeviceAST.h"
#include	"ExceptionTypes.h"


EventMonitorEM::EventMonitorEM( Resource * r , AST * nm ,EventMonitorEM * o)
	{
		setResource(r);
		m_noun_modifier=nm;
		setResourceContextAST ( new ResourceContextAST(getResource(),"~MEASURED") );
		m_Original=o;
	}

	
void	EventMonitorEM::SETUPtoTedl()
	{		
		AnalogResourceContext::SETUPtoTedl();
	}

void	EventMonitorEM::CONNECTtoTedl()
	{
		AnalogResourceContext::CONNECTtoTedl();
	}

void	EventMonitorEM::DISCONNECTtoTedl()
	{	
		AnalogResourceContext::DISCONNECTtoTedl();
	}

void	EventMonitorEM::CHANGEtoTedl()
	{
		AnalogResourceContext::CHANGEtoTedl();
	}


void	EventMonitorEM::MEASUREtoTedl()
	{
		AnalogResourceContext::MEASUREtoTedl();
	}

void	EventMonitorEM::MONITORtoTedl()
	{
		AnalogResourceContext::MONITORtoTedl();
	}

void	EventMonitorEM::VERIFYtoTedl()
	{
		AnalogResourceContext::VERIFYtoTedl();
	}

void	EventMonitorEM::WAIT_FORtoTedl()	{}
void	EventMonitorEM::ARMtoTedl()
	{
		AnalogResourceContext::ARMtoTedl();
	}
void	EventMonitorEM::FETCHtoTedl()
	{
		sayhi("FETCHtoTedl");
	
		preSetContext();

		doPrepControl("FETCH");

		toEVENTS(&AnalogResourceContext::FETCH);

		doControlAction("FETCH");
	}

void	EventMonitorEM::READtoTedl()
	{
		INITIATE();
		FETCH();
	}
void	EventMonitorEM::InvalidatedResult()	{}
void	EventMonitorEM::MeasuredtoTedl()	{}

void	EventMonitorEM::RESETtoTedl()
	{
		AnalogResourceContext::RESETtoTedl();
	}

void	EventMonitorEM::REMOVEtoTedl()
	{
		AnalogResourceContext::REMOVEtoTedl();
	}

void	EventMonitorEM::COMPAREtoTedl()	{}
void	EventMonitorEM::INITIATEtoTedl()	{}
void	EventMonitorEM::APPLYtoTedl()
	{
		// ooh! what to do ???
	}

void	EventMonitorEM::DISABLE_EVENTtoTedl()
	{
		AnalogResourceContext::DISABLE_EVENTtoTedl();
	}

void	EventMonitorEM::ENABLE_EVENTtoTedl()
	{
		AnalogResourceContext::ENABLE_EVENTtoTedl();
	}

void	EventMonitorEM::FSMError(const std::string& t, const std::string& s)
	{
		RWCString err;
		err	+=	" EVENT MONITOR:BAD State Transition attempt error: "+RWCString(t);
		err	+=	" in state "+RWCString(s);
		err	+=	" Device:"+getResource()->getName();
		
		throw TedlExecutionError(err);
	}

void	EventMonitorEM::setResourceName( RWCString n )
	{
		getResource()->setName( n );
	}

void
EventMonitorEM::APPLY		(AST * a){ setcurrent(a);APPLYevent		(); }
void
EventMonitorEM::ARM		(AST * a){ setcurrent(a);ARMevent		(); }
void
EventMonitorEM::CHANGE		(AST * a){ setcurrent(a);CHANGEevent		(); }
void
EventMonitorEM::COMPARE		(AST * a){ setcurrent(a);COMPAREevent		(); }
void
EventMonitorEM::CONNECT		(AST * a){ setcurrent(a);CONNECTevent		(); }
void
EventMonitorEM::DISABLE_EVENT	(AST * a){ setcurrent(a);DISABLE_EVENTevent	(); }
void
EventMonitorEM::DISCONNECT	(AST * a){ setcurrent(a);DISCONNECTevent	(); }
void
EventMonitorEM::_DISCONNECT	(AST * a){ setcurrent(a);DISCONNECTedevent	(); }
void
EventMonitorEM::ENABLE_EVENT	(AST * a){ setcurrent(a);ENABLE_EVENTevent	(); }
void
EventMonitorEM::FETCH		(AST * a){ setcurrent(a);FETCHevent		(); }
void
EventMonitorEM::INITIATE	(AST * a){ setcurrent(a);INITIATEevent		(); }
void
EventMonitorEM::MAXTIME		(AST * a){ setcurrent(a);MAXTIMEevent		(); }
void
EventMonitorEM::MEASURE		(AST * a){ setcurrent(a);MEASUREevent		(); }
void
EventMonitorEM::MEASUREMENT	(AST * a){ setcurrent(a);MEASUREMENTevent	(); }
void
EventMonitorEM::MONITOR		(AST * a){ setcurrent(a);MONITORevent		(); }
void
EventMonitorEM::READ		(AST * a){ setcurrent(a);READevent		(); }
void
EventMonitorEM::REMOVE		(AST * a){ setcurrent(a);REMOVEevent		(); }
void
EventMonitorEM::RESET		(AST * a){ setcurrent(a);RESETevent		(); }
void
EventMonitorEM::SETUP		(AST * a){ setcurrent(a);SETUPevent		(); }
void
EventMonitorEM::VERIFY		(AST * a){ setcurrent(a);VERIFYevent		(); }
		
void
EventMonitorEM::SkipState	(AST * a){ setcurrent(a);SkipStateevent		(); }
void
EventMonitorEM::Select		(AST * a){ setcurrent(a);Selectevent		(); }
void
EventMonitorEM::AsyncReset	(AST * a){ setcurrent(a);AsyncResetevent	(); }
void
EventMonitorEM::ResourceReset	(AST * a){ setcurrent(a);ResourceResetevent	(); }

int EventMonitorEM::resetFSM()
	{
		return AnalogResourceContext::resetFSM();
	}


//------------------------------------------------------------------------
	
EventMonitor::EventMonitor( Resource * r , AST * nm, EventMonitor * o)
	:EventMonitorEM(r,nm,o)
	{
	}

AnalogResourceContext * EventMonitor::clone( Resource * r )
	{
		return new EventMonitor(r,m_noun_modifier,this);
	}


// Event generatorsS for EventMonitor State Machine.
