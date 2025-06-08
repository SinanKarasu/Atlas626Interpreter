#include	"SourceLoad.h"
#include	"AtlasAST.h"

#include	"Resource.h"
#include	"VirtualResourceAST.h"
#include	"SignalTypeAST.h"
#include	"TedlSignalVerbVisitor.h"
#include	"TedlDeviceAST.h"
#include	"ExceptionTypes.h"

SourceLoadEM::SourceLoadEM( Resource * r ,SourceLoadEM * o)
	{
		setResource(r);
		setResourceContextAST ( new ResourceContextAST(getResource(),"~MODIFIERS") );
		m_Original=o;
	}


void SourceLoadEM::SETUPtoTedl()
	{	
		AnalogResourceContext::SETUPtoTedl();
	}

void SourceLoadEM::CONNECTtoTedl()
	{	
		AnalogResourceContext::CONNECTtoTedl();
	}

void SourceLoadEM::DISCONNECTtoTedl()
	{
		AnalogResourceContext::DISCONNECTtoTedl();
	}

void SourceLoadEM::CHANGEtoTedl()
	{
		AnalogResourceContext::CHANGEtoTedl();
	}

void SourceLoadEM::RESETtoTedl()
	{	
		AnalogResourceContext::RESETtoTedl();
	}

void SourceLoadEM::APPLYtoTedl()
	{

		sayhi("APPLYtoTedl");

		//	note: reversed  to avoid hot connect. 11/09/2000 sik.
		//	SETUP();
		//	CONNECT();

	
		CONNECT();
		SETUP();

		if(EventsIntroduced()){
			ENABLE_EVENT();
		} else {
			SkipState();
		}
	}

void SourceLoadEM::DISABLE_EVENTtoTedl()
	{
		AnalogResourceContext::DISABLE_EVENTtoTedl();
	}

void SourceLoadEM::ENABLE_EVENTtoTedl()
	{
		AnalogResourceContext::ENABLE_EVENTtoTedl();
	}


void SourceLoadEM::REMOVEtoTedl()
	{	
		AnalogResourceContext::REMOVEtoTedl();
	}

void	SourceLoadEM::FSMError(const std::string& t, const std::string& s)
	{
		RWCString err;
		err	+=	" SOURCE/LOAD:BAD State Transition attempt error: "+RWCString(t);
		err	+=	" in state "+RWCString(s);
		err	+=	" Device:"+getResource()->getName();
		
		throw TedlExecutionError(err);
	}


void SourceLoadEM::setResourceName( RWCString n )
	{
		getResource()->setName( n );
	}


// EVENTS for SourceLoad.
void
SourceLoadEM::APPLY		(AST * a)	{ setcurrent(a);APPLYevent		(); };
void
SourceLoadEM::ARM		(AST * a)	{ setcurrent(a);ARMevent		(); };
void
SourceLoadEM::CHANGE		(AST * a)	{ setcurrent(a);CHANGEevent		(); };
void
SourceLoadEM::CONNECT		(AST * a)	{ setcurrent(a);CONNECTevent		(); };
void
SourceLoadEM::COMPARE		(AST * a)	{ setcurrent(a);COMPAREevent		(); };
void
SourceLoadEM::DISABLE_EVENT	(AST * a)	{ setcurrent(a);DISABLE_EVENTevent	(); };
void
SourceLoadEM::DISCONNECT	(AST * a)	{ setcurrent(a);DISCONNECTevent		(); };
void
SourceLoadEM::_DISCONNECT	(AST * a)	{ setcurrent(a);DISCONNECTedevent	(); };
void
SourceLoadEM::ENABLE_EVENT	(AST * a)	{ setcurrent(a);ENABLE_EVENTevent	(); };
void
SourceLoadEM::MAXTIME		(AST * a)	{ setcurrent(a);MAXTIMEevent		(); };
void
SourceLoadEM::REMOVE		(AST * a)	{ setcurrent(a);REMOVEevent		(); };
void
SourceLoadEM::RESET		(AST * a)	{ setcurrent(a);RESETevent		(); };
void
SourceLoadEM::SETUP		(AST * a)	{ setcurrent(a);SETUPevent		(); };
void
SourceLoadEM::SkipState		(AST * a)	{ setcurrent(a);SkipStateevent		(); }
void
SourceLoadEM::Select		(AST * a)	{ setcurrent(a);Selectevent		(); }
void
SourceLoadEM::AsyncReset	(AST * a)	{ setcurrent(a);AsyncResetevent		(); }
void
SourceLoadEM::ResourceReset	(AST * a)	{ setcurrent(a);ResourceResetevent	(); }

void
SourceLoadEM::FETCH	(AST * a)
	{
		setcurrent(a);
		Error( "FETCH illegal for SOURCE/LOAD");
	}
void
SourceLoadEM::READ	(AST * a)
	{
		setcurrent(a);
		Error( "READ illegal for SOURCE/LOAD");
	}
void
SourceLoadEM::MONITOR	(AST * a)
	{
		setcurrent(a);
		Error( "MONITOR illegal for SOURCE/LOAD");
	}
void
SourceLoadEM::VERIFY	(AST * a)
	{
		setcurrent(a);
		Error( "VERIFY illegal for SOURCE/LOAD");
	}
void
SourceLoadEM::MEASURE	(AST * a)
	{
		setcurrent(a);
		Error( "MEASURE illegal for SOURCE/LOAD");
	}
void
SourceLoadEM::MEASUREMENT	(AST * a)
	{
		setcurrent(a);
		Error( "MEASUREMENT illegal for SOURCE/LOAD");
	}
		
void
SourceLoadEM::INITIATE	(AST * a)
	{
		setcurrent(a);
		Error( "INITIATE illegal for SOURCE/LOAD");
	}
		
int
SourceLoadEM::resetFSM()
	{
		return AnalogResourceContext::resetFSM();
	}
		
void
SourceLoadEM::Error(RWCString  e)
	{
		FSMError( e,this->GetState().StateName() );
	}
//------------------------------------------------------------------
Source:: Source( Resource * r ,Source * o):SourceLoadEM(r,o)
	{
	}

AnalogResourceContext * Source::clone( Resource * r )
	{
		return new Source(r,this);
	}

Load:: Load( Resource * r ,Load * o ):SourceLoadEM(r,o)
	{
	}

AnalogResourceContext * Load::clone( Resource * r )
	{
		return new Load(r,this);
	}

// Invalid commands
void SourceLoadEM::MEASUREtoTedl()	{assert(0);} // Not in a SOURCE/LOAD
void SourceLoadEM::MONITORtoTedl()	{assert(0);} // Not in a SOURCE/LOAD
void SourceLoadEM::VERIFYtoTedl()	{assert(0);} // Not in a SOURCE/LOAD
void SourceLoadEM::WAIT_FORtoTedl()	{assert(0);} // Not in a SOURCE/LOAD
void SourceLoadEM::ARMtoTedl()		{assert(0);} // Not in a SOURCE/LOAD
void SourceLoadEM::FETCHtoTedl()	{assert(0);} // Not in a SOURCE/LOAD
void SourceLoadEM::READtoTedl()		{assert(0);} // Not in a SOURCE/LOAD
void SourceLoadEM::InvalidatedResult()	{assert(0);} // Not in a SOURCE/LOAD
void SourceLoadEM::MeasuredtoTedl()	{assert(0);} // Not in a SOURCE/LOAD
void SourceLoadEM::COMPAREtoTedl()	{assert(0);} // Not in a SOURCE/LOAD
void SourceLoadEM::INITIATEtoTedl()	{assert(0);} // Not in a SOURCE/LOAD
