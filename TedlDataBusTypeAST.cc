
#include	"AtlasStd.h"
#include        "AtlasParser.h"
#include	"TedlDataBusTypeAST.h"
#include	"Visitors.h"

TedlDataBusTypeAST::TedlDataBusTypeAST( ANTLRTokenPtr	p ,TheType t):AST(p),_storage(t){}
TedlDataBusTypeAST::TedlDataBusTypeAST( AST	*	a ,TheType t):m_AST(a),_storage(t){}
TedlDataBusTypeAST::TedlDataBusTypeAST( ANTLRTokenPtr	p ,AST * a,TheType t):AST(p),m_AST(a),_storage(t){}
TedlDataBusTypeAST::~TedlDataBusTypeAST(){}

AST *
TedlDataBusTypeAST::eval	( AST * a )
		{
			return ASTdown()->eval( a );
		}
AST *
TedlDataBusTypeAST::assign	( AST * a )		{ return this; }

AST *
TedlDataBusTypeAST::check( AST * a )			{ return ASTdown()->check( a ); }

Long
TedlDataBusTypeAST::compare	( AST *  o     ) const	{ return ASTdown()->compare( o ); }

TheType
TedlDataBusTypeAST::getType	( AST * a ) const
	{
		return ASTdown()->getType( a );
	}


AST *
TedlDataBusTypeAST::data(AST * a)			{ return ASTdown()->data( a ); }

				
TedlTalker			::TedlTalker		( ASTList * equipList )
				:TedlDataBusTypeAST(0),m_equipList(equipList)	{};
AST *
TedlTalker			::data( AST * a )
	{
		AST * mod=0;
		if(!m_equipList){
			return 0;
		} else {
			if(m_equipList->findValue("ANY",mod)){
				return this;
			} else {
				m_equipList->findValue(a->getName(),mod);
				return mod;
			}
		}
	}




TedlListener		::TedlListener		( ASTList * equipList )
				:TedlDataBusTypeAST(0),m_equipList(equipList)	{};
AST *
TedlListener		::data( AST * a )
	{
		AST * mod=0;
		if(!m_equipList){
			return 0;
		} else {
			if(m_equipList->findValue("ANY",mod)){
				return this;
			} else {
				m_equipList->findValue(a->getName(),mod);
				return mod;
			}
		}
	}



TedlDataBusDevices		::TedlDataBusDevices	( AST * a )
				:TedlDataBusTypeAST(a)	{};
TedlDataBusDevices		::TedlDataBusDevices	( ANTLRTokenPtr p )
				:TedlDataBusTypeAST(p)	{};
AST *
TedlDataBusDevices		::check( AST * a )
	{
		AST *	sub	=	ASTdown();
		AST *	sup	=	a->ASTdown();
		
		if(sub->check( sup )){
			sub=sub->ASTright();
			sup=sup->ASTright();
			if(sub->check( sup )){
				return this;
			}
		}
		return 0;
	}

