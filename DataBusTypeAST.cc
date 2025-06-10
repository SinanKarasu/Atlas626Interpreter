
#include	"AtlasStd.h"
#include        "AtlasParser.h"
#include	"DataBusTypeAST.h"
#include	"Visitors.h"

DataBusTypeAST::DataBusTypeAST( ANTLRTokenPtr	p ,TheType t):AST(p),_storage(t){}
DataBusTypeAST::DataBusTypeAST( AST	*	a ,TheType t):m_AST(a),_storage(t){}
DataBusTypeAST::DataBusTypeAST( ANTLRTokenPtr	p ,AST * a,TheType t):AST(p),m_AST(a),_storage(t){}
DataBusTypeAST::~DataBusTypeAST(){}

AST *
DataBusTypeAST::eval	( AST * a )
		{
			return ASTdown()->eval( a );
		}
AST *
DataBusTypeAST::assign	( AST * a )		{ return this; }

AST *
DataBusTypeAST::check( AST * a )			{ return ASTdown()->check( a ); }

Long
DataBusTypeAST::compare	( AST *  o     ) const	{ return ASTdown()->compare( o ); }

TheType
DataBusTypeAST::getType	( AST * a ) const
	{
		return ASTdown()->getType( a );
	}

AST *
DataBusTypeAST::Accept	( ASTVisitor & v )	{ return v.VisitDataBusTypeAST( this ); }

AST *
DataBusTypeAST::data(AST * a)			{ return ASTdown()->data( a ); }

TestEquipMonitor	::TestEquipMonitor		( ASTList * equipList )
				:DataBusTypeAST(0),m_equipList(equipList)	{};
ExchangeMonitor		::ExchangeMonitor		( ANTLRTokenPtr p )
				:DataBusTypeAST(p)	{};
TestEquipBusRole	::TestEquipBusRole		( ASTList *  mmsList )
				:DataBusTypeAST(0),m_mmsList(mmsList)	{};
				
				
AST *
TestEquipBusRole::data(AST * a)
	{
		AST * mod=0;
		if(a){
			m_mmsList->findValue(a->getName(),mod);
			return mod;
		} else {
			return this;
		}
	}

AST *
TestEquipBusRole::check( AST * a )	// we are the subset,a is the superset
	{
		AST			*subset = this,
					*supset = a;
		AST			*result = this;
		AST			*mod	= 0;
		//ASTListIterator		it( *m_mmsList );
		
		// for each MODIFIER in the m_mmsList,
		// make sure we are covered.
		for(const auto& it: *m_mmsList ) {
		//while ( ++it ){
			StringAST modifier(it->getName());
			if(mod=supset->data(&modifier)){
				if(  it->check( mod ) ){
					result = this;
				} else {
					return 0;
				}
			} else {	// oops , modifier not found!!!
				//Error_Report(" Modifier is not covered. ", it);
				return 0;
			}
			
		}
		return result;
	}
AST *
TestEquipBusRole::assign   ( AST * a )
	{
		m_mmsList->insert(a);
		return this;
	}


Long
TestEquipBusRole::compare( AST * o ) const	// we are the superset
	{
		AST	*subset = o;
		//ASTListIterator		it( *m_mmsList );
			
		long	result = -2;
		for(const auto& it: *m_mmsList) {
		//while ( ++it ){
			assert(0);
			
			switch(  subset->compare(it ) ){
				case   1:
						result = 1;
						break;
						
				case    0:	// equal modifiers
						if(result==-2){
							result=0;
						};
						break;
				case   -1:	// superset greater than subset
						if(result==-2){
							result=-1;
						};
				default:
					result = 0;	
			}
		}
		return result;
	}


void 
TestEquipBusRole::print(AST * a) const
	{

		//ASTListIterator		it( *m_mmsList );
		
		// for each BUS ROLE in the m_mmsList,
		for(const auto& it: *m_mmsList) {
		//while ( ++it ){
			it->print();
		}
	}


AST *
TestEquipBusRole::init( AST * a ){

		//ASTListIterator		it( *m_mmsList );
		
		// for each MODIFIER in the m_mmsList,
		for(const auto& it: *m_mmsList) {
		//while ( ++it ){
			it->init(a);
		}
		return this;
} 
				
				
Talker			::Talker		( ASTList * equipList )
				:DataBusTypeAST(0),m_equipList(equipList)	{};
AST *
Talker			::check( AST * a )
	{
		AST			*subset = this,
					*supset = a;
		AST			*result = this;
		AST			*mod	= 0;
		if(!m_equipList){
			return this;
		}
		//ASTListIterator		it( *m_equipList );
		
		// for each Name in the m_equipList,
		// make sure we are covered.
		for(const auto& it: *m_equipList) {
		//while ( ++it ){
			StringAST name(it->getName());
			if(!(mod=supset->data(&name))){
				return 0;
			}
			
		}
		return this;
	}


AST *
Talker			::data( AST * a )
	{
		AST * mod=0;
		if(!m_equipList){
			return 0;
		} else if(m_equipList->findValue(a->getName(),mod)){
			return mod;
		} else {
			return 0;
		}
	}


Listener		::Listener		( ASTList * equipList )
				:DataBusTypeAST(0),m_equipList(equipList)	{};
AST *
Listener	::check( AST * a )
	{
		AST			*subset = this,
					*supset = a;
		AST			*result = this;
		AST			*mod	= 0;
		if(!m_equipList){
			return this;
		}
		// ASTListIterator		it( *m_equipList );
		
		// for each Name in the m_equipList,
		// make sure we are covered.
		
		for(const auto& it: *m_equipList) {
		//while ( ++it ){
			StringAST name(it->getName());
			if(!(mod=supset->data(&name))){
				return 0;
			}
			
		}
		return this;
	}

AST *
Listener	::data( AST * a )
	{
		AST * mod=0;
		if(!m_equipList){
			return 0;
		} else if(m_equipList->findValue(a->getName(),mod)){
			return mod;
		} else {
			return 0;
		}
	}


DataBusDevice		::DataBusDevice		( ANTLRTokenPtr p )
				:DataBusTypeAST(p)	{};
DataBusDevice		::DataBusDevice		( AST * a )
				:DataBusTypeAST(a)	{};
BusRedundancyMode	::BusRedundancyMode	( ANTLRTokenPtr p )
				:DataBusTypeAST(p)	{};
RedundantBus		::RedundantBus		( ANTLRTokenPtr p )
				:DataBusTypeAST(p)	{};
AlternateBusTransmit	::AlternateBusTransmit	( ANTLRTokenPtr p )
				:DataBusTypeAST(p)	{};
DataBusDevices		::DataBusDevices	( AST * a )
				:DataBusTypeAST(a)	{};
DataBusDevices		::DataBusDevices	( ANTLRTokenPtr p )
				:DataBusTypeAST(p)	{};
AST *
DataBusDevices		::check( AST * a )
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



Command			::Command		( AST * a ):
				DataBusTypeAST(a)	{};
Data			::Data		( AST * a )
				:DataBusTypeAST(a)	{};
DataBusStatus		::DataBusStatus		( AST * a )
				:DataBusTypeAST(a)	{};
DataBusData		::DataBusData		( ANTLRTokenPtr p ,AST * a)
				:DataBusTypeAST(p,a)	{};
ExchangeModels		::ExchangeModels( ASTList * l )
				:DataBusTypeAST(0,0)
				,m_exchangeModelList(l)
				//,m_it(0)
				{};
AST *
ExchangeModels::data(AST * a)
	{
		AST * mod=0;
		m_exchangeModelList->findValue(a->getName(),mod);
		return mod;
	}

AST *
ExchangeModels::check( AST * a )	// we are the subset,a is the superset
	{
		AST			*subset = this,
					*supset = a;
		AST			*result = this;
		AST			*mod	= 0;
		init();		
		// for each MODIFIER in the m_exchangeModelList,
		// make sure we are covered.
		
		while ( (*m_it)!= (*m_exchangeModelList).end()){
			StringAST modifier((*(*m_it))->getName());
			if(mod=supset->data(&modifier)){
				if(  (*(*m_it))->check( mod ) ){
					result = this;
				} else {
					return 0;
				}
			} else {	// oops , Bus Mode!!!
				Error_Report(" BUS MODE is not covered. ", (*(*m_it)));
				return 0;
			}
			++m_it;
		}
		return result;
	}
AST *
ExchangeModels::assign   ( AST * a )
	{
		m_exchangeModelList->insert(a);
		return this;
	}


Long
ExchangeModels::compare( AST * o ) const	// we are the superset. o is the subset.
	{
		AST	*subset = o;

		//init();
					
		long	result = -2;
		//while ( ++(*m_it) ){
		while ( (*m_it)!= (*m_exchangeModelList).end()){

			assert(0);
			
			switch(  subset->compare((*(*m_it) )) ){
				case   1:
						result = 1;
						break;
						
				case    0:	// equal modifiers
						if(result==-2){
							result=0;
						};
						break;
				case   -1:	// superset greater than subset
						if(result==-2){
							result=-1;
						};
				default:
					result = 0;	
			}
		}
		return result;
	}


void
ExchangeModels::print(AST * a) const
	{

		// ASTListIterator		it( *m_exchangeModelList );
		
		// for each MODIFIER in the m_exchangeModelList,
		for(const auto& it: *m_exchangeModelList) {
		//while ( ++it ){
			it->print();
		}
	}


AST *
ExchangeModels::init( AST * a ){

// 		if(!m_it){
// 			m_it= (*m_exchangeModelList).begin() ; //new ASTListIterator( *m_exchangeModelList );
// 		}
// 		m_it->reset();
		// for each MODIFIER in the m_exchangeModelList,
		if(a){
			for(const auto& it: *m_exchangeModelList ) {
			//while ( ++(*m_it) ){
				it->init(a);
			}
		}
		return this;
} 

AST *
ExchangeModels::succ( AST * a ){

		if(m_it){
			
			if(++(*m_it) == (*m_exchangeModelList).end() ){
				return *(*m_it);
			} else {
				return 0;
			}
		} else {
			return 0;
		}
} 

ExchangeFrame 		::ExchangeFrame( AST * a)
				:DataBusTypeAST(a)
	{
	}

ExchangeSchedule	::ExchangeSchedule	( AST * a)
				:DataBusTypeAST(a)
	{
	}
	
ExchangeDetails		::ExchangeDetails	( AST * a)
				:DataBusTypeAST(a)
	{
	}


ExchangeContents	::ExchangeContents	( AST * a)
				:DataBusTypeAST(a)
	{
	}
RoleField		::RoleField	( AST * a)
				:DataBusTypeAST(a)
	{
	}
WatchDog		::WatchDog	( AST * a)
				:DataBusTypeAST(a)
	{
	}

TestEquipRoleName	::TestEquipRoleName( ANTLRTokenPtr p, ModifierEntry * me,AtlasParser * parser )
				:DataBusTypeAST(p),m_modifierEntry( me ),m_parser(parser)
	{
	}

RWCString
TestEquipRoleName::getName() const
	{ return m_modifierEntry->modifier; }

Long
TestEquipRoleName::compare( AST * o ) const	// o is the superset
	{
	
		StringAST modifier(getName());
		AST * what = o->data(&modifier);
		
		RWCString other=what->getName();
		
		if ( getName() < other ){
			return -1;
		}else{
			return ( getName() > other );
		}
	}

AST *
TestEquipRoleName::check( AST * a )
	{
		if ( (a) && compare( a ) == 0 ){
				return this;
		}
		return 0;
	}

AST *
TestEquipRoleName::data(AST * a)
	{
		return this;
	}

void
TestEquipRoleName::print(AST * a) const
	{
		std::cout << "Modifier Print>>" << getName() << std::endl;
	}
			
AST *
TestEquipRoleName::init(AST * a)
	{
		RWCString left;ModifierEntry * modifierEntry;
		NounEntry * nounEntry=((NounType *)a)->_nounEntry;
		modifierEntry=m_parser->theModifierEntry( getName(), nounEntry ,left );
		if(!modifierEntry){
			Error_Report("NOUN "+nounEntry->noun+" Does not support "
				+ m_modifierEntry->modifier,getToken());
		} else if(modifierEntry!=m_modifierEntry){
			std::cout << "Changed modifier " << std::endl;
			m_modifierEntry=modifierEntry;
		}
		return this;
	}


BusModeName		::BusModeName( ANTLRTokenPtr p, ModifierEntry * me,AtlasParser * parser )
				:DataBusTypeAST(p),m_modifierEntry( me ),m_parser(parser)
	{
	}

RWCString
BusModeName::getName() const
	{ return m_modifierEntry->modifier; }

Long
BusModeName::compare( AST * o ) const	// o is the superset
	{
	
		StringAST modifier(getName());
		AST * what = o->data(&modifier);
		
		RWCString other=what->getName();
		
		if ( getName() < other ){
			return -1;
		}else{
			return ( getName() > other );
		}
	}

AST *
BusModeName::check( AST * a )
	{
		if ( (a) && compare( a ) == 0 ){
				return ASTdown()->check(a->ASTdown());
		}
		return 0;
	}

AST *	BusModeName::data(AST * a)
	{
		return this;
	}

void  BusModeName::print(AST * a) const
	{
		std::cout << "Modifier Print>>" << getName() << std::endl;
	}
			
AST *	BusModeName::init(AST * a)
	{
		RWCString left;ModifierEntry * modifierEntry;
		NounEntry * nounEntry=((NounType *)a)->_nounEntry;
		modifierEntry=m_parser->theModifierEntry( getName(), nounEntry ,left );
		if(!modifierEntry){
			Error_Report("NOUN "+nounEntry->noun+" Does not support "
				+ m_modifierEntry->modifier,getToken());
		} else if(modifierEntry!=m_modifierEntry){
			std::cout << "Changed modifier " << std::endl;
			m_modifierEntry=modifierEntry;
		}
		return this;
	}

DataBusTransaction	::	DataBusTransaction	( AST * a)
		:DataBusTypeAST(a)
	{
	}
	
BusParameterName		::BusParameterName( ANTLRTokenPtr p, ModifierEntry * me,AtlasParser * parser )
				:DataBusTypeAST(p),m_modifierEntry( me ),m_parser(parser)
	{
	}

RWCString
BusParameterName::getName() const
	{ return m_modifierEntry->modifier; }

Long
BusParameterName::compare( AST * o ) const	// o is the superset
	{
	
		StringAST modifier(getName());
		AST * what = o->data(&modifier);
		
		RWCString other=what->getName();
		
		if ( getName() < other ){
			return -1;
		}else{
			return ( getName() > other );
		}
	}

AST *
BusParameterName::check( AST * a )
	{
		if ( (a) && compare( a ) == 0 ){
				return this;
		}
		return 0;
	}

AST *	BusParameterName::data(AST * a)
	{
		return this;
	}

void  BusParameterName::print(AST * a) const
	{
		std::cout << "Modifier Print>>" << getName() << std::endl;
	}
			
AST *	BusParameterName::init(AST * a)
	{
		RWCString left;ModifierEntry * modifierEntry;
		NounEntry * nounEntry=((NounType *)a)->_nounEntry;
		modifierEntry=m_parser->theModifierEntry( getName(), nounEntry ,left );
		if(!modifierEntry){
			Error_Report("NOUN "+nounEntry->noun+" Does not support "
				+ m_modifierEntry->modifier,getToken());
		} else if(modifierEntry!=m_modifierEntry){
			std::cout << "Changed modifier " << std::endl;
			m_modifierEntry=modifierEntry;
		}
		return this;
	}

BusParameter	::	BusParameter	( AST * a)
		:DataBusTypeAST(a)
	{
	}

BusParameters	::	BusParameters	( AST * a)
		:DataBusTypeAST(a)
	{
	}

ProtocolParameter	::	ProtocolParameter	( AST * a)
		:DataBusTypeAST(a)
	{
	}

ProtocolParameters	::	ProtocolParameters	( AST * a)
		:DataBusTypeAST(a)
	{
	}

EntireData	::	EntireData	( AST * a)
		:DataBusTypeAST(a)
	{
	}

DataPump	::	DataPump	( AST * a)
		:DataBusTypeAST(a)
	{
	}

ExchangeDefinition	::	ExchangeDefinition	( AST * a)
		:DataBusTypeAST(a)
	{
	}

AST *
ExchangeDefinition	::check( AST * a )	// we are the subset,a is the superset
	{
		AST			*subset = this,
					*supset = a;
		AST			*result = 0;
		AST			*mod	= 0;
		AST			*sup	= 0;
		
		// for each in the m_exchangeModelList,
		// make sure we are covered.
		
		supset->init();
		while(sup=supset->succ()){
		
			if(  result=ASTdown()->check( sup ) ){
				return result ;
			}
		}
		Error_Report(" BUS MODE is not covered. ", ASTdown());
		return 0;
	}

DefineExchangeFields	::	DefineExchangeFields	( AST * a)
		:DataBusTypeAST(a)
	{
	}

ExchangeDevices	::	ExchangeDevices	( AST * a)
		:DataBusTypeAST(a)
	{
	}

