
#include	"ATEResourceAST.h"
#include	"Visitors.h"
#include 	"Resource.h"

ATEResourceAST::ATEResourceAST( ANTLRTokenPtr p ){};
ATEResourceAST::~ATEResourceAST(){};
	
AST *
ATEResourceAST::eval	( AST * a )		{ return this; };

AST *
ATEResourceAST::assign	( AST * a )		{ return this; };

AST *
ATEResourceAST::check	( AST * a )		{ return ASTdown()->check( a ); }

AST *
ATEResourceAST::Accept	( ASTVisitor & v )	{ return v.VisitATEResourceAST( this ); }

AST *
ATEResourceAST::data(AST * a)
	{
		return ASTdown()?ASTdown()->data( a ):0;
	}

RWCString
ATEResourceAST::getName() const
	{
		return ASTdown()->getName();
	}
AST *
ATEResourceAST::setBase(AST * a,AnalogResourceContext * b)
	{
		Resource * r=a->getResource();
		if(r->m_currentAnalogFSM){
			if(r->m_currentAnalogFSM->m_Original!=b){
				delete r->m_currentAnalogFSM;
				r->m_currentAnalogFSM=0;
			}
		}
		if(!r->m_currentAnalogFSM){
			r->m_currentAnalogFSM=b->clone(r);
		}
		return this;
	}

AST *
ATEResourceAST::setBase(AST * a,DataBusResourceContext * b)
	{
		Resource * r=a->getResource();
		if(r->m_currentDataBusFSM){
			if(r->m_currentDataBusFSM->m_Original!=b){
				delete r->m_currentDataBusFSM;
				r->m_currentDataBusFSM=0;
			}
		}
		if(!r->m_currentDataBusFSM){
			r->m_currentDataBusFSM=b->clone(r);
		}
		return this;
	}

ATEDeviceFunction::ATEDeviceFunction( AST * stateMachine,ASTList * _SigCharsList )
	:m_StateMachine(stateMachine),m_SigCharsList(_SigCharsList)
	{
	}

AST *
ATEDeviceFunction::data(AST * a)
	{
		if(!a){
			return this;
		} else {
			AST * mod=0;
			m_SigCharsList->findValue(a->getName(),mod);
			return mod;
		}
	}

AST *
ATEDeviceFunction::check	( AST * a )
	{
		AST			*subset =a  ,
					*supset ;
		AST			*result = this;
		//ASTListIterator		it( *m_SigCharsList );
		
		// for each SIG_CHAR in the m_SigCharsList,
		// find the first coverage
		
		for (const auto &it : *m_SigCharsList) {
			if(  a->check( supset=it ) ){
					return it;	// found one
			}
		}
		Error_Report(" UNABLE to satisfy requirement for :", a);
		return 0;
	}


AST *
ATEDeviceFunction::eval(AST * a)
	{
		if(!a){
			return this;
		} else {
			if(m_StateMachine){
				m_StateMachine->eval(a);
				return ASTdown();
			} else {
				ATEResourceAST::eval(a);
			}
		}
	}


CnxList::CnxList( ASTList * cnxList ):m_cnxList(cnxList){};

SigChar::SigChar( ANTLRTokenPtr p ){};
AST * SigChar::data(AST * a)
	{
		return ASTdown()->data(a);
	}

ControlBlock::ControlBlock( ANTLRTokenPtr p ){};

ATESource::ATESource( AnalogResourceContext * p )
	:m_AnalogResourceContext(p) {};

AST *
ATESource::eval	( AST * a )
	{
		setBase(a,m_AnalogResourceContext);
		return this;
	}

ATESensor::ATESensor( AnalogResourceContext * p )
	:m_AnalogResourceContext(p) {};

AST *
ATESensor::eval	( AST * a )
	{
		setBase(a,m_AnalogResourceContext);
		return this;
	}

ATEEventMonitor::ATEEventMonitor( AnalogResourceContext * p )
	:m_AnalogResourceContext(p) {};

AST *
ATEEventMonitor::eval	( AST * a )
	{
		setBase(a,m_AnalogResourceContext);
		return this;
	}

ATELoad::ATELoad( AnalogResourceContext * p )
	:m_AnalogResourceContext(p) {};

AST *
ATELoad::eval	( AST * a )
	{
		setBase(a,m_AnalogResourceContext);
		return this;
	}

ATETimer::ATETimer( AnalogResourceContext * p )
	:m_AnalogResourceContext(p) {};

AST *
ATETimer::eval	( AST * a )
	{
		setBase(a,m_AnalogResourceContext);
		return this;
	}

ATEBusProtocol::ATEBusProtocol( DataBusResourceContext * p ,const RWCString spec)
	:m_DataBusResourceContext(p)
	,m_spec(spec)
	{
	}

AST *
ATEBusProtocol::eval	( AST * a )
	{
		setBase(a,m_DataBusResourceContext);
		return this;
	}

