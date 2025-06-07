
#include	"AtlasStd.h"
#include	"EventTypeAST.h"
#include        "AtlasParser.h"
#include	"Visitors.h"



EventTypeAST::EventTypeAST(TheType t ,AST * server1,AST * server2)
		:_storage(t)
		,m_s1(server1)
		,m_s2(server2)
		,m_initialized(0)
		,m_enabled(0)
	{
	}

void
EventTypeAST::setInteger( int indx, Long value )
	{
		if(value){
			if(!m_initialized){
				if(m_s1)m_s1->insert(this);
				if(m_s2)m_s2->insert(this);
			}
		}
		m_enabled=value;
	}

Long
EventTypeAST::getInteger( int indx ) const
	{
		return m_enabled;
	}

AST *
EventTypeAST::data(AST * a)
	{
		return ASTdown()->eval();;
	}

AST *
EventTypeAST::eval(AST * a)
	{
		if(a==this){	// we are being tickled by a server
			if(m_enabled)broadcast(a);
			return this;
		} else {
			assert(0);
			return this;
		}
	}


void
EventTypeAST::print(AST * a) const
	{
		std::cout << " EventTypeAST >>" << getName() << std::endl;
	}


void
EventTypeAST::add_client(AST * a)
	{
		if(!m_clients){
			m_clients=new ASTList;
		}
		//ASTListIterator	clit(*m_clients);
		//while(++clit){
		//	if(clit.key()==a){
		//		return;	// already there
		//	}
		//}
		if(!m_clients->contains(a)){
			m_clients->insert(a);
		}
	}

AST *
EventTypeAST::insert	( AST   * a   )
	{
		add_client(a);
		return a;
	}


AST *
EventTypeAST::remove	( AST   * a   )
	{
		if(m_clients&& (m_clients->contains(a))){
			m_clients->remove(a);
			return a;
		} else {
			return 0;
		}
	}


void
EventTypeAST::broadcast(AST * a)
	{
		if(m_clients){
			//ASTListIterator	clit(*m_clients);
		for (const auto &clit : *m_clients) {
				if(a==clit){
					assert(0);
				}
				clit->eval(clit);
			}
		}
	}

TheType	EventTypeAST::getType	( AST * a ) const
	{
		return _storage;
	}



AnalogEventType::AnalogEventType( AST * server1)
		:EventTypeAST(AnalogEventTypeValue,server1)
	{
	}



AST *
AnalogEventType::data(AST * a)
	{
		return ASTdown()->eval();;
	}

AST *
AnalogEventType::eval(AST * a)
	{
		return m_s1->eval(a);
	}

Resource *
AnalogEventType::getResource()
	{
		return m_s1->getResource();
	}



void
AnalogEventType::print(AST * a) const
	{
		std::cout << " AnalogEventType >>" << getName() << std::endl;
	}

EventIntervalType::EventIntervalType( AST * server1,AST * server2 )
	:EventTypeAST(EventIntervalTypeValue,server1,server2)
	{
		fromEvent=0;
		toEvent=0;
	}



AST *
EventIntervalType::data(AST * a)
	{
		return ASTdown()->eval();;
	}

AST *
EventIntervalType::eval(AST * a)
	{
		if( (!fromEvent) && (!toEvent) ){
			m_s1->insert(this);
		} else if( (fromEvent) && (!toEvent ) ){
			broadcast(this);
			m_s2->insert(this);
		} else {
			broadcast(this);
		}
		return this;
	}


void
EventIntervalType::print(AST * a) const
	{
		std::cout << " EventIntervalType >>" << getName() << std::endl;
	}

EventIndicatorType::EventIndicatorType( AST * server , AST * label)
		:EventTypeAST(EventIndicatorTypeValue,server ),m_label(label)
	{
		setInteger(0,0);
	}



AST *
EventIndicatorType::data(AST * a)
	{
		return ASTdown()->eval();
	}


AST *
EventIndicatorType::eval(AST * a)
	{
		if(a==this){
			if(m_enabled){
				m_label->setInteger(0,1);
				broadcast(this);
			}
		}
		return this;		
	}


void
EventIndicatorType::print(AST * a) const
	{
		std::cout << " EventIndicatorType >>" << ASTdown()->getName() << std::endl;
	}




TimeBasedEventType::TimeBasedEventType(AST * server )
	:EventTypeAST(TimeBasedEventTypeValue,server )
	{

	}

AST *
TimeBasedEventType::data(AST * a)
	{
		return ASTdown()->eval();
	}
	
AST *
TimeBasedEventType::eval(AST * a)
	{
		if(a==this){
			EventTypeAST::eval(this);
		} else {
			m_s1->insert(this);
		}
		return this;
	}


void
TimeBasedEventType::print(AST * a) const
	{
		std::cout << " TimeBasedEventType >>" << getName() << std::endl;
	}


OneShotTimerType::OneShotTimerType()
	:EventTypeAST(OneShotTimerTypeValue )
	{
		

	}
	
AST *
OneShotTimerType::data(AST * a)
	{
		return ASTdown()->eval();
	}

AST *
OneShotTimerType::eval(AST * a)
	{
		if(a==this){
			EventTypeAST::eval(this);
			setInteger(0,0);	// disable ourselves
		} else {
			double secs=ASTdown()->getDecimal(0);
			setInteger(0,1);	// enable ourselves
			m_timer.schedule(0,secs,0.0,this,this);
		}
		return this;
	}


void
OneShotTimerType::print(AST * a) const
	{
		std::cout << " OneShotTimerType >>" << getName() << std::endl;
	}


PeriodicTimerType::PeriodicTimerType()
	:EventTypeAST(PeriodicTimerTypeValue )
	{
		

	}
	
AST *
PeriodicTimerType::data(AST * a)
	{
		return ASTdown()->eval();
	}

AST *
PeriodicTimerType::eval(AST * a)
	{
		if(a==this){
			EventTypeAST::eval(this);
			//setInteger(0,0);	// disable ourselves
		} else {
			double secs=ASTdown()->getDecimal(0);
			setInteger(0,1);	// enable ourselves
			m_timer.schedule(0,0.0,secs,this,this);
		}
		return this;
	}


void
PeriodicTimerType::print(AST * a) const
	{
		std::cout << " PeriodicTimerType >>" << getName() << std::endl;
	}



PeriodicEventCounterType :: PeriodicEventCounterType ( AST * server,AST * final )
		:EventTypeAST(EventCounterTypeValue,server)
		,m_final(final)
		,m_count(0)
	{
	}





AST *
PeriodicEventCounterType ::eval(AST * a)
	{
		if( a!=this ){
			m_s1->insert(this);
			m_count=0;
		} else if( a==this ){
			m_count++;
			if(m_final){
				if(m_count>=m_final->getInteger()){
					m_count=0;
					broadcast(this);
				}
			} else {
				m_count=0;
				broadcast(this);
			}
		}
		return this;
	}

// Note that OnceEventCounterType can actually be derived from PeriodicEventCounterType
// but later.....

OnceEventCounterType :: OnceEventCounterType ( AST * server,AST * final )
	:EventTypeAST(EventCounterTypeValue,server)
	,m_final(final)
	,m_count(0)
	{
	}





AST *
OnceEventCounterType::eval(AST * a)
	{
		if( a!=this ){
			m_s1->insert(this);
			m_count=0;
		} else if( a==this ){
			m_count++;
			if(m_final){
				if(m_count>=m_final->getInteger()){
					m_count=0;
					broadcast(this);
					setInteger(0,0);
				}
			} else {
				m_count=0;
				broadcast(this);
				setInteger(0,0);
			}
		}
		return this;
	}

