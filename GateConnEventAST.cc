
#include	"AtlasStd.h"
#include	"GateConnEventAST.h"
#include	"Visitors.h"
#include	"Resource.h"

GateConnEventAST::GateConnEventAST( ANTLRTokenPtr p )
	:AST(p)
	{
	}

GateConnEventAST::~GateConnEventAST(){}
	

AST *	GateConnEventAST::eval	( AST * a )
	{
		AST * x=ASTdown();
		while(x){
			x->eval( (a==this)?x:a );
			x=x->ASTright();
		}
		return this;
	}
	
AST *	GateConnEventAST::assign	( AST * a )
	{
		return this;
	}

AST *	GateConnEventAST::check ( AST * a )
	{
		//ASTdown()->check();
		//ASTdown()->ASTright()->check();
		//return 0;
		return this; // FIX_ME
	}	

AST *	GateConnEventAST::Accept	( ASTVisitor & v )
	{
		return v.VisitGateConnEventAST( this );
	}

astream&
GateConnEventAST::operator<<	( astream& s )
	{
		if(ASTdown()){
			s << std::endl;
			s << "\tCNX:";
			s << ASTdown();
		}
		return s;
	}


PinDescriptor::PinDescriptor( ANTLRTokenPtr p )
	:GateConnEventAST(p)
	{
	}

PinDescriptor::PinDescriptor( AST * p , const RWCString pre)
	:m_ast(p),m_prefix(pre)
	{
	}

AST *	PinDescriptor::eval	( AST * a )
	{
		
		return m_ast?m_ast->eval( a ):this;
	}
RWCString      PinDescriptor::getName() const
	{
		return m_prefix + (m_ast?(m_ast->getName()):RWCString("CNX"));
	}

astream&
PinDescriptor::operator<<	( astream& s )
	{
		s << getName();
		if(ASTdown()){
			s << " ";
			s << ASTdown();
			//s << endl;
		}
		if(ASTright()){
			s << " ";
			s << ASTright();
		}
		return s;
	}

PreDefinedPinDescriptor::PreDefinedPinDescriptor( ANTLRTokenPtr p )
	:GateConnEventAST(p)
	{
	}

PreDefinedPinDescriptor::PreDefinedPinDescriptor( AST * p )
	:m_ast(p)
	{
	}	

AST *	PreDefinedPinDescriptor::eval	( AST * a )
	{
			return this;
	}

ConnDescriptor::ConnDescriptor( ANTLRTokenPtr p )
	:GateConnEventAST(p)
	{
	}

AST *	ConnDescriptor::eval	( AST * a )
	{
			return ASTdown()->eval( a );
	}

RWCString      ConnDescriptor::getName() const
	{
		return ASTdown()->getName();
	}

astream&
ConnDescriptor::operator<<	( astream& s )
	{
		//s << getName();
		if(ASTdown()){
			s << " ";
			s << ASTdown()->data();
			//s << endl;
		}
		if(ASTright()){
			s << " ";
			s << ASTright();
		}
		return s;
	}

ReferenceConn::ReferenceConn( ANTLRTokenPtr p )
	:GateConnEventAST(p)
	{
	}
AST *	ReferenceConn::eval	( AST * a )
	{
			return ASTdown()->eval( a );
	}

ConnectSetRef::ConnectSetRef( ANTLRTokenPtr p )
	:GateConnEventAST(p)
	{
	}

AST *	ConnectSetRef::eval	( AST * a )
	{
			return ASTdown()->eval( a );
	}


GateField::GateField( ANTLRTokenPtr p )
	:GateConnEventAST(p)
	{
	}

AST *	GateField::eval	( AST * a )
	{
			if((a==this) || (a==0) ){
				return GateConnEventAST::eval( a );
			} else {
				Resource * r=a->getResource();
				if(r){
					r->introduceFromGate(ASTdown());
					if(ASTdown()->ASTright()){
						r->introduceToGate(ASTdown()->ASTright());
					}
				} else {
					return 0;
				}
			}
			return this;
	}

astream&
GateField::operator<<	( astream& s )
	{
		return s;
	}

EventField::EventField( ANTLRTokenPtr p )
	:GateConnEventAST(p)
	{
	}

AST *	EventField::eval	( AST * a )
	{

			if((a==this) || (a==0) ){
				return GateConnEventAST::eval( a );
			} else {
				Resource * r=a->getResource();
				if(r){
					r->introduceFromEvent(ASTdown());
					if(ASTdown()->ASTright()){
						r->introduceToEvent(ASTdown()->ASTright());
					}
				} else {
					return 0;
				}
			}
			return this;
	}

astream&
EventField::operator<<	( astream& s )
	{
		return s;
	}

CnxField::CnxField( ANTLRTokenPtr p )
	:GateConnEventAST(p)
	{
	}

AST *	CnxField::eval	( AST * a )
	{
			if((a==this) || (a==0) ){
				return GateConnEventAST::eval( a );
			} else {
				Resource * r=a->getResource();
				if(r){
					r->introduceCnx(ASTdown());
				} else {
					return 0;
				}
			}
			return this;
	}

astream&
CnxField::operator<<	( astream& s )
	{
		if(ASTdown()){
			s << ASTdown();
		}
		if(ASTright()){
			s << ASTright();
		}
		return s;
	}

MaxTime::MaxTime( ANTLRTokenPtr p )
	:GateConnEventAST(p)
	{
	}

AST *	MaxTime::eval	( AST * a )
	{
			if((a==this) || (a==0) ){
				return GateConnEventAST::eval( a );
			} else {
				Resource * r=a->getResource();
				if(r){
					r->introduceMaxTime(ASTdown());
				} else {
					return 0;
				}
			}
			return this;
	}


astream&
MaxTime::operator<<	( astream& s )
	{
		return s;
	}
