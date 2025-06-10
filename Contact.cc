#include	"Types.h"
#include	"Graph.h"
#include	"NodeName.h"
#include	"Vertex.h"
#include	"TAGContext.h"
#include	"Contact.h"
#include	"Resource.h"

const EdgeType Contact::m_theType=ContactType;

Contact::Contact(Resource * r,int state,Vertex * S,Vertex * D,Impedance cz,Impedance oz)
	:Edge(S,D,cz),m_Open(oz)
	{
		m_Resource=r;
		m_state=state;
	}

Set &
Contact::theSet()
	{
		return m_energizer;
	}
	
RWCString Contact::theName()
{
	
	RWCString n="<";
	n+=m_Resource->theName()+":"+Sorc->getName();
	if(Closed()){
		n+="[o-o]";
	} else {
		n+="[o/o]";
	}
	n+=m_Resource->theName()+":"+Dest->getName();
	n+=">";
	return n;
}

Edge *
Contact::clone(Resource * r)
	{
		Vertex * s=r->AddNode(Sorc->getName(),Sorc->getNodeType());
		Vertex * d=r->AddNode(Dest->getName(),Dest->getNodeType());
		Contact * c= new Contact(r,m_state,s,d,m_Cost,m_Open);
		c->m_energizer=m_energizer;
		return c;
	}

Vertex *
Contact::Destination(const DestinationMode mode)
	{
		Vertex * val=0;
		switch	(mode){
		case	HardWiredMode	:
					{
						val = 0;
						break; 
					}
		case	CurrentMode	:
					{	if(Closed()){
							val = Dest;
						} else {
							val = 0;
						}
						break; 
					}
		case	AnyPathMode	:
					{
						val = Dest;
						break; 
					}
		Default			:
					{
						val = 0;
					}
		}
		return val;
	}

void
Contact::insertThyself(EdgeList * edgeList)
	{
		edgeList->push_front(this);
//		edgeList->append(this);
	}

const Impedance &
Contact::Cost(DestinationMode mode)
	{
		switch	(mode){
		case	HardWiredMode	:
					{
						return m_Open;
					}
		case	CurrentMode	:
					{
						if(Closed()){
							return m_Cost;
						} else {
							return m_Open;
						}
					}
		case	AnyPathMode	:
					{
						return m_Cost;
					}
		Default			:
					{
						assert(0);
					}
		}
	}


int const
Contact::Closed(const int check)
	{
		if(check==c_Und){
			return m_Resource->TellMe( m_energizer );
		} else {
			return m_energizer.isSet(check);
		}
	} 

int
Contact::Jump()
	{
		if(Closed()){
			return 0;
		} else {
			return 1;
		}
	}

int
Contact::Hops()
	{
		return 1;
	}

void
Contact::setCondition(const int pos)
	{
		m_energizer.set(pos);
	}

void
Contact::commit(Association * r)
	{
		Edge::commit(r);
		calculateDynamicClass();
	}
void
Contact::uncommit(Association * r)
	{
		Edge::uncommit(r);
		calculateDynamicClass();
	}

void
Contact::calculateDynamicClass()
	{
		invalidateDynamicClass();
	
	}
void
Contact::invalidateDynamicClass()
	{
		Sorc->unlinkDynamic();	
		Dest->unlinkDynamic();	
	}

EdgeType
Contact::theType()
	{
		return m_theType;
	}
