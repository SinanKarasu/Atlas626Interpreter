
#include	"VirtualResourceAST.h"
#include	"Sensor.h"
#include	"SourceLoad.h"
#include	"TedlDefinitions.h"
#include	"TedlParser.h"
#include	"SignalTypeAST.h"
#include	"Visitors.h"
#include	"Dictionary.h"

extern TedlParser *	TEDL;
extern DeviceDictionary	VirtualDevices;

VirtualResourceAST::VirtualResourceAST( Resource *  resource )
	:m_resource(resource)
	{
	}

Resource *
VirtualResourceAST::getResource( )
	{
		return m_resource;
	}


void 
VirtualResourceAST::setResource( Resource * resource )
	{
		m_resource=resource;
	}
		

RWCString
VirtualResourceAST::realDevice(AST * label)
	{
		extern TedlSymbolDictionary deviceEquivalence;
		//RWCString unquoted(RWCString & string);
		RWCString name,equiv_name;
		DeviceEquivalence * equiv_dev=0;
		if(label){
			name=label->getName();
		} 
		if(deviceEquivalence.findValue(name,equiv_dev)){
			return equiv_dev->getName();
		} else {
			return unquoted(getName());
		}
	}

void
VirtualResourceAST::toFSM(void (AnalogResourceContext::*c)(AST *))
	{
		AST * useResource=0;
		if(m_resource){
			if(m_resource->m_currentAnalogFSM){
				(m_resource->m_currentAnalogFSM->*c)(this);
			}
		}
	}


AST *
VirtualResourceAST::initialize()
	{
		AST * sigChar;
		toFSM(&AnalogResourceContext::Select);
		if(m_resource){
			AnalogResourceContext * save=m_resource->m_currentAnalogFSM;
			if(sigChar=m_resource->RequirementsCheck(this)){
				if(save!=m_resource->m_currentAnalogFSM){
					int ijk=123;
				}
				m_resource->m_currentAnalogFSM->m_transitionSigChar=sigChar;
			}
			return sigChar;
		} else {
			return 0;
		}
	}


VirtualResourceAST::VirtualResourceAST( ANTLRTokenPtr p )
	:ResourceAST(p)
	,m_resource(0)
	{
	}

VirtualResourceAST::~VirtualResourceAST(){};
	
AST *
VirtualResourceAST::eval	( AST * a )		{ return this; };

AST *
VirtualResourceAST::data	( AST *  a )
	{
		AST * where=ASTdown();
		if(a==this){
			return this; // Someone is looking for a virtual resource
		} else if(a){
			if(a->getName()=="~NOUN"){
				return where;
			}
			if(a->getName()=="~MEASURED"){
				return where->ASTdown();
			}
			where=where->ASTright();
			if(a->getName()=="~MODIFIERS"){
				return where;
			
			}
			where=where->ASTright();
			if(a->getName()=="~CNX"){
				return where;
			}
			return 0;
		} else {
			return where;
		}
		return where;
	}
AST *
VirtualResourceAST::assign	( AST * a )		{ return this; };

TheType
VirtualResourceAST::getType	( AST * a ) const	{ return m_storage; };

RWCString
VirtualResourceAST::getName	() const		{ return m_name; };

void
VirtualResourceAST::setName	( RWCString a )
	{
		extern TedlSymbolDictionary deviceEquivalence;
		m_name = a;
		if(deviceEquivalence.findValue(m_name,m_DeviceEquivalence)){
			m_resource = TEDL->getDevice( m_DeviceEquivalence->getName());
			if(m_resource){
				Resource * sub_device=m_resource->getDevice( m_DeviceEquivalence->getCapabilityName());
				m_resource=sub_device?sub_device:m_resource;
			}
		} else if (TEDL!=NULL) {	// if we have a TEDL
			m_DeviceEquivalence=new DeviceEquivalence(m_name);
			m_resource = TEDL->getDevice( m_name );
		} else {
			m_resource=NULL;
		}
		VirtualDevices.insertKeyAndValue( m_name, this );
	}
		
		
AST *
VirtualResourceAST::init( AST * a )
	{
		if ( a ){
			setName(a->getName());
			return this;
		} else {
			return VirtualResourceAST::check();
		}
	}

AST *
VirtualResourceAST::check( AST * a )
	{
		if(a){
			if(checkSubsume(ASTdown()->ASTright(),a)){
				return this;
			} else {
				return 0;
			}
		} else {
			return initialize();
		}
	}


AST *
VirtualResourceAST::Accept	( ASTVisitor & v )	{ return v.VisitVirtualResourceAST( this ); }


//-----------------------------------------------------------------------------


VirtualAnalogEvent	::VirtualAnalogEvent	( ANTLRTokenPtr p ){};

//-----------------------------------------------------------------------------

VirtualEventMonitor	::VirtualEventMonitor	( ANTLRTokenPtr p ):VirtualResourceAST(p){};

AST *
VirtualEventMonitor::eval( AST * a )
	{
		if ( a != 0 ){
			return this;
		}else{
			return 0;
		}
	}

AST *
VirtualEventMonitor::check( AST * a )
	{
		return VirtualResourceAST::check(a);
	}


AST *
VirtualEventMonitor::clone	( Scope * s ) const
	{
		AST * c= new VirtualEventMonitor(getToken());
		c->setDown(ASTdown());
		return c;
	}

//-----------------------------------------------------------------------------

VirtualTimer::VirtualTimer		( ANTLRTokenPtr p ):VirtualResourceAST(p)
	{
		the_time = new DecimalNumber(0.0);
	}

TheType
VirtualTimer::getType	( AST * a  ) const
	{
		return the_time->getType();
	}


AST *
VirtualTimer::clone	( Scope * s ) const
	{
		AST * c=  new VirtualTimer(getToken());
		c->setDown(ASTdown());
		return c;
	}


AST *
VirtualTimer::init	( AST * a ) 
	{
		m_timer.reset();
		return eval();
	}


AST *
VirtualTimer::data	( AST * a )
	{
		return this;
	}

AST *
VirtualTimer::eval	( AST * a )
	{
		the_time->setDecimal(0,m_timer.seconds());
		return this;
	}

double
VirtualTimer::getDecimal( int indx ) const
	{
		the_time->setDecimal(0,m_timer.seconds());
		return the_time->getDecimal();
	}

AST *
VirtualTimer::add	( AST * a )
	{
		double at_time=a->getDecimal();
		m_timer.schedule(0,at_time,0.0,a,a);
		return this;
	}
AST *
VirtualTimer::check( AST * a )
	{
		//return VirtualResourceAST::check(a);
		return this;
	}

//-----------------------------------------------------------------------------

VirtualSensor::VirtualSensor( ANTLRTokenPtr p ):VirtualResourceAST(p)
	{
	};

AST *
VirtualSensor::eval( AST * a )
	{
		if ( a != 0 ){
			return this;
		}else{
			return 0;
		}
	}

AST *
VirtualSensor::check( AST * a )
	{
		return VirtualResourceAST::check(a);
	}

AST *
VirtualSensor::clone	( Scope * s ) const
	{
		AST * c= new VirtualSensor(getToken());	
		c->setDown(ASTdown());
		return c;
	}

//-----------------------------------------------------------------------------
	
VirtualSource::VirtualSource( ANTLRTokenPtr p ):VirtualResourceAST(p)
	{		
	}

AST *
VirtualSource::clone	( Scope * s ) const
	{
		AST * c= new VirtualSource(getToken());	
		c->setDown(ASTdown());
		return c;
	}


AST *
VirtualSource::eval( AST * a )
	{
		if ( a != 0 ){
			return this;
		}else{
			return 0;
		}
	}

AST *
VirtualSource::check( AST * a )
	{
		return VirtualResourceAST::check(a);
	}

//-----------------------------------------------------------------------------

VirtualLoad::VirtualLoad( ANTLRTokenPtr p ):VirtualResourceAST(p){};


AST *
VirtualLoad::clone	( Scope * s ) const
	{
		AST * c= new VirtualLoad(getToken());	
		c->setDown(ASTdown());
		return c;
	}

AST *
VirtualLoad::eval( AST * a )
	{
		if ( a != 0 ){
			return this;
		}else{
			return 0;
		}
	}

AST *
VirtualLoad::check( AST * a )
	{
		return VirtualResourceAST::check(a);
	}


//------------------------------------------------------------------------------//
//		Usage Classes							//
//------------------------------------------------------------------------------//

UseResource::UseResource( AST * label )
		:m_label(label)
	{
	}
	
AST *
UseResource::check( AST * a )
	{	AST * sup=0;	AST * sub = this;
		if(a==0){
			// we check to see if we are subsumed by the REQUIRE statement
			// VirtualSensor m_label ( i.e. USING label).
			sup = m_label->eval(m_label);// send a message to get the data...
			sub = ASTdown();
			if( (sup) &&checkSubsume(sub,sup->ASTdown())){
				m_resource=sup->getResource();
				return this;
			}
		} else {
			// we are being asked by the TedlDevice to see if a 
			// given SIG_CHAR is good enough for us.
			sup = a;
			sub = ASTdown()->ASTright();
			if( (sup) &&checkSubsume(sub,sup->ASTdown())){
				//m_resource=sup->getResource();
				return this;
			}
		}

		return 0;
	}
	
AST *
UseResource::eval( AST * a )
	{
		return this;
	}


//-----------------------------------------------------------------------------

UseSource::UseSource( AST * label ):UseResource(label){}

	
AST *
UseSource::check( AST * a )
	{
		return UseResource::check(a);
	}
	
AST *
UseSource::eval( AST * a )
	{
		return UseResource::eval(a);
	}

//-----------------------------------------------------------------------------
	
UseSensor::UseSensor( AST * label )
	:UseResource(label)
	{
	}
	
AST *
UseSensor::check( AST * a )
	{	
		return UseResource::check(a);
	}
	
AST *
UseSensor::eval( AST * a )
	{
		return UseResource::eval(a);
	}


//-----------------------------------------------------------------------------

UseEventMonitor::UseEventMonitor( AST * label )
	:UseResource(label),m_FSM(0),m_sigChar(0)
	{
	}

AST *
UseEventMonitor::check( AST * a )
	{	
		return UseResource::check(a);
	}
	
AST *
UseEventMonitor::eval( AST * a )
	{
		return UseResource::eval(a);
	}
AST *
UseEventMonitor::init( AST * a )
	{
		if(initialize()){
			m_FSM	=m_resource->m_currentAnalogFSM;
			m_sigChar=m_FSM->m_transitionSigChar;
			return m_sigChar;
		} else {
			m_FSM=0;
			m_sigChar=0;
			return 0;
		}	
	}

//-----------------------------------------------------------------------------
	
UseLoad::UseLoad( AST * label ):UseResource(label){}

AST *
UseLoad::check( AST * a )
	{
		return UseResource::check(a);
	}
	
AST *
UseLoad::eval( AST * a )
	{
		return UseResource::eval(a);
	}

