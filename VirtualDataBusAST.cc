
#include	"VirtualDataBusAST.h"
#include	"Sensor.h"
#include	"SourceLoad.h"
#include	"TedlDefinitions.h"
#include	"TedlParser.h"
#include	"SignalTypeAST.h"
#include	"Visitors.h"
#include	"Dictionary.h"

extern TedlParser *	TEDL;
extern DeviceDictionary	VirtualDevices;

VirtualDataBusAST::VirtualDataBusAST( Resource *  resource )
	:m_resource(resource)
	{
	}

RWCString
VirtualDataBusAST::realDevice(AST * label)
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

Resource *
VirtualDataBusAST::getResource( )
	{
		return m_resource;
	}

void 
VirtualDataBusAST::setResource( Resource * resource )
	{
		m_resource=resource;
	}

void
VirtualDataBusAST::toFSM(void (DataBusResourceContext::*c)(AST *))
	{
		AST * useResource=0;
		if(m_resource){
			if(m_resource->m_currentDataBusFSM){
				(m_resource->m_currentDataBusFSM->*c)(this);
			}
		}
	}

AST *
VirtualDataBusAST::initialize()
	{
		AST * sigChar;
		//toFSM(DataBusResourceContext::Select);
		if(m_resource){
			DataBusResourceContext * save=m_resource->m_currentDataBusFSM;
			if(sigChar=m_resource->RequirementsCheck(this)){
				if(save!=m_resource->m_currentDataBusFSM){
					int ijk=123;
				}
				m_resource->m_currentDataBusFSM->m_transitionSigChar=sigChar;
			}
			return sigChar;
		} else {
			return 0;
		}
	}


VirtualDataBusAST::VirtualDataBusAST( ANTLRTokenPtr p )
	:ResourceAST(p)
	,m_resource(0)
	{
	}

VirtualDataBusAST::~VirtualDataBusAST(){};
	
AST *		VirtualDataBusAST::eval	( AST * a )		{ return this; };
AST *		VirtualDataBusAST::data	( AST *  a )
	{
		AST * where=ASTdown();
		if(a==this){
			return this; // Someone is looking for a virtual resource
		} else if(a){
			if(a->getName()=="~SPEC"){
				return where;
			}
			where=where->ASTright();
			if(a->getName()=="~INSTALLATION"){
				return where->data();
			
			}
			if(a->getName()=="~MODELS"){
				if(where){
					where=where->data();
					return where->ASTright();
				} else {
					return 0;
				}
			}
			where=where->ASTright();
			if(a->getName()=="~REDUNDANCY"){
				return where->data();
			}
			where=where->ASTright();
			if(a->getName()=="~CNX"){
				return where;
			}
			return 0;
		} else {
			return where;
		}
	}

AST *
VirtualDataBusAST::assign	( AST * a )		{ return this; };

TheType
VirtualDataBusAST::getType	( AST * a ) const	{ return m_storage; };

RWCString
VirtualDataBusAST::getName	() const		{ return m_name; };

void
VirtualDataBusAST::setName	( RWCString a )
	{
		extern TedlSymbolDictionary deviceEquivalence;
		m_name = a;
		if(deviceEquivalence.findValue(m_name,m_DeviceEquivalence)){
			m_resource = TEDL->getDevice( m_DeviceEquivalence->getName());
			if(m_resource){
				Resource * sub_device=m_resource->getDevice( m_DeviceEquivalence->getCapabilityName());
				m_resource=sub_device?sub_device:m_resource;
			}
		} else  if (TEDL!=NULL) {	// if we have a TEDL
			m_DeviceEquivalence=new DeviceEquivalence(m_name);
			m_resource = TEDL->getDevice( m_name );
		} else {
			m_resource=NULL;
		}
		VirtualDevices.insertKeyAndValue( m_name, this );
	}
		
		
AST *
VirtualDataBusAST::init( AST * a )
	{
		if ( a ){
			setName(a->getName());
			return this;
		} else {
			return VirtualDataBusAST::check();
		}
	}

AST *
VirtualDataBusAST::check( AST * supSet )
	{
		if(supSet){
			AST * subSet=ASTdown()->ASTright();
			if(checkSubsume(subSet->data(),supSet)){
				return this;
			} else {
				return 0;
			}
		} else {
			return initialize();
		}
	}


AST *
VirtualDataBusAST::Accept	( ASTVisitor & v )	{ return v.VisitVirtualDataBusAST( this ); }

//-----------------------------------------------------------------------------
VirtualProtocol::VirtualProtocol( ANTLRTokenPtr p )
	:VirtualDataBusAST(p)
	{
	}

AST *
VirtualProtocol::eval( AST * a )
	{
		if ( a != 0 ){
			return this;
		}else{
			return 0;
		}
	}

AST *
VirtualProtocol::check( AST * a )
	{
		return VirtualDataBusAST::check(a);
	}

AST *
VirtualProtocol::clone	( Scope * s ) const
	{
		return new VirtualProtocol;	
	}

//-----------------------------------------------------------------------------

VirtualExchange::VirtualExchange( ANTLRTokenPtr p )
	:VirtualDataBusAST(p)
	{
	};

AST *
VirtualExchange::eval( AST * a )
	{
		if ( a != 0 ){
			return this;
		}else{
			return 0;
		}
	}


AST *
VirtualExchange::check( AST * a )
	{
		if(a){
			return 0;
		} else {
			StringAST models("~MODELS");
			AST * ast=ASTdown()->ASTright()->data(&models);
			ASTdown()->check(ast);
		}
	}

AST *
VirtualExchange::clone	( Scope * s ) const
	{
		return new VirtualExchange;	
	}


//-----------------------------------------------------------------------------

VirtualExchangeConfiguration::VirtualExchangeConfiguration( ANTLRTokenPtr p )
	:VirtualDataBusAST(p)
	{
	};

AST *
VirtualExchangeConfiguration::eval( AST * a )
	{
		if ( a != 0 ){
			return this;
		}else{
			return 0;
		}
	}

AST *
VirtualExchangeConfiguration::check( AST * a )
	{
		return VirtualDataBusAST::check(a);
	}

AST *
VirtualExchangeConfiguration::clone	( Scope * s ) const
	{
		return new VirtualExchangeConfiguration;	
	}

AST *
VirtualExchangeConfiguration::data	( AST *  a )
	{
		AST * where=ASTdown();AST * protocol;
		if(a==this){
			return this; // Someone is for the whole taco
		} else if(a){
			while(where){
				if(a->getName()==where->getName()){
					break;
				}
				where=where->ASTright();
			}
		}
		return where;
	}

//------------------------------------------------------------------------------//
//		Usage Classes							//
//------------------------------------------------------------------------------//
	
UseDataBus::UseDataBus( AST * label )
		:m_label(label)
	{
	}

AST *
UseDataBus::check( AST * a )
	{	
		AST * sup = m_label->eval(m_label);// send a message to get the data...
		// we check to see if we are subsumed
		// by the VirtualSensor m_label.
		if( (sup) &&checkSubsume(ASTdown(),sup->ASTdown())){
			m_resource=sup->getResource();
			return this;
		} else {
			return 0;
		}
	}

AST *
UseDataBus::eval( AST * a )
	{	
		return this;
	}

//-----------------------------------------------------------------------------
	
UseProtocol::UseProtocol( AST * label )
		:UseDataBus(label)
	{
		//m_name=m_label->getName();
	}
	
AST *
UseProtocol::check( AST * a )
	{
		return UseDataBus::check(a);
	}
	
AST *
UseProtocol::eval( AST * a )
	{
		return this;
	}

//-----------------------------------------------------------------------------
	
UseExchangeConfiguration::UseExchangeConfiguration( AST * label )
		:UseDataBus(label)
	{
		//m_name=m_label->getName();
	}
	
AST *
UseExchangeConfiguration::check( AST * a )
	{	
		return UseDataBus::check(a);
	}
	
AST *
UseExchangeConfiguration::eval( AST * a )
	{
		return this;
	}

//-----------------------------------------------------------------------------
	
UseExchange::UseExchange( AST * label )
		:UseDataBus(label)
	{
		//m_name=m_label->getName();
	}
	
AST *
UseExchange::check( AST * a )
	{	
		return UseDataBus::check(a);
	}
	
AST *
UseExchange::eval( AST * a )
	{
		return this;
	}
