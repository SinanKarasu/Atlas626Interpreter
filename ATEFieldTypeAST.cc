
#include	"AtlasStd.h"
#include	"ATEFieldTypeAST.h"
#include	"Visitors.h"

ATEFieldTypeAST::ATEFieldTypeAST( AST * a ){}
ATEFieldTypeAST::~ATEFieldTypeAST(){}

AST *	ATEFieldTypeAST::eval	( AST * a )
	{
		if ( a != 0 )
			return ASTdown()->eval( a );
		else
			return this;
	}

Long    ATEFieldTypeAST::getInteger( int indx ) const
	{
		return m_data;
	}
	
AST *	ATEFieldTypeAST::Accept	( ASTVisitor & v )	{ return v.VisitATEFieldTypeAST( this ); }

ATESyncField::ATESyncField( AST * a ){};

EventSlopeNegField::EventSlopeNegField( AST * a ){};
AST *	EventSlopeNegField::eval	( AST * a )
	{
		StringAST m("~NOUN");
		StringAST s("DECREASING");
		AST * mods=a->data(&m);
		if(mods && mods->data(&s)){
			m_data=1;
		} else {
			m_data=0;
		}
		return this;
	}

EventSlopePosField::EventSlopePosField( AST * a ){};
AST *	EventSlopePosField::eval	( AST * a )
	{
		StringAST m("~NOUN");
		StringAST s("INCREASING");
		AST * mods=a->data(&m);
		if(mods && mods->data(&s)){
			m_data=1;
		} else {
			m_data=0;
		}
		return this;
	}

EventVoltageField::EventVoltageField( AST * a ){};

StateField::StateField( AST * a ){};

EventPortField::EventPortField( AST * a ){};
PortIsField::PortIsField( AST * a ){};

AST *	PortIsField::eval	( AST * a )
	{
		Resource * r= a->getResource();
		AST * cnxField=r->getCnx();
		RWCString PinName=ASTdown()->getName();
		m_data=0;
		while(cnxField){
			if(PinName==cnxField->getName()){
				m_data=1;
				return this;
			}
			cnxField=cnxField->ASTright();
		}
		return this;
	}

ATECapability::ATECapability( ASTList * r )
	:m_resourceList(r)
	{
	}

AST *	ATECapability::check( AST * nounAST )
	{
		AST *		func = 0;
		//ASTListIterator	SignalFunction_it( *m_resourceList );
		
		for (auto it = m_resourceList->begin(); it!= m_resourceList->begin() && !func; ++it)		
			func = (*it)->check( nounAST );
		return  func;
	}
	
SwitchFunction::SwitchFunction( AST * a ){};
