#include	"Types.h"
#include	"Graph.h"
#include	"TAGContext.h"

#include	"Edge.h"

#include	"Vertex.h"
#include <iostream>

extern AssociationListStack theAssociationListStack;

Edge::Edge(Vertex * S,Vertex * D, Impedance C  )
	:Sorc		( S )
	,Dest		( D )
	,m_Cost		( C )
	,m_search	( 0 )
	,m_forward	( 0 )
	,m_isthmus	( 0 )
	,m_other	( 0 )
	,m_usingAssociationList(0)
	{
	}

void
Edge::invalidateDynamicClass()
	{
	}			;
void
Edge::calculateDynamicClass()
	{
	}			;

const int
Edge::Closed(const int closed)
	{
		return 1;
	}

void
Edge::setCondition(const int pos)
	{
	}

void
Edge::insertThyself(EdgeList * edgeList)
	{
		edgeList->insert(this);
	}

void
Edge::setResource(Resource * resource)
	{
		m_Resource=resource;
	}

Resource *
Edge::getResource()	const
	{
		return m_Resource;
	}

int
Edge::Jump()
	{
		return 0;
	}

int
Edge::Hops()
	{
		return 0;
	}


void
Edge::setIsthmus(const int crit)
	{
		m_isthmus=crit;
	}
	
void
Edge::setLookAhead(const TAGContext c)
	{
		m_lookAhead=c;
	}

int
Edge::getIsthmus(const int crit) const
	{
		return m_isthmus;
	}

TAGContext &
Edge::getLookAhead()
	{
		return m_lookAhead;
	}

const Impedance &
Edge::Cost(DestinationMode mode)
	{
		return m_Cost;			
	}
 
Vertex *
Edge::Destination(const DestinationMode mode)
	{
		return Dest ;
	}


int
Edge::committed(Association * r)
	{
		if(!m_usingAssociationList){
			if(m_other->m_usingAssociationList){
				return m_other->committed(r);
			}
			return 0;
		} else if(r==0){
			return (m_usingAssociationList->size()>0);
		} else if(m_usingAssociationList->contains(r)){
			return 1;
		} else {
			return 0;
		}
	}

int
Edge::onlycommitted(Association * r)
{
		assert(r!=0);
		if(!m_usingAssociationList){
			if(m_other->m_usingAssociationList){
				return m_other->onlycommitted(r);
			}
			return 1;
		} else {
			//AssociationListIterator rcblit(*m_usingAssociationList);
			//while(++rcblit){
			for(const auto& rcblit: *m_usingAssociationList) {
				if(r==rcblit){
				} else if(r->getResource() !=rcblit->getResource()) {
						return 0;
				} else if(r->getVertex() !=rcblit->getVertex()) {
						return 0;
				}
			}
			return 1;	// note: no-commits at all wil return 1. It is OK.
		}
}

void
Edge::commit(Association * r)
	{
		assert(r);
		if(!m_usingAssociationList){
			if(m_other->m_usingAssociationList){
				m_other->commit(r);
			} else {
				m_usingAssociationList=theAssociationListStack.getOne();
				m_usingAssociationList->append(r);
			}
		} else if(!m_usingAssociationList->contains(r)){
			m_usingAssociationList->append(r);
		}
	}
void
Edge::uncommit(Association * r)
	{
		assert(r);
		if(m_usingAssociationList){
			if(m_usingAssociationList->contains(r)){
				m_usingAssociationList->remove(r);
				//if(m_usingAssociationList->entries()==0){
				//	theAssociationListStack.push(m_usingAssociationList);
				//	m_usingAssociationList=0;
				//}
			}
		} else if(m_other->m_usingAssociationList){
			m_other->uncommit(r);
		}
	}

void
Edge::listCommitted(Association * r)
	{
		if(m_usingAssociationList){
			//AssociationListIterator rcblit(*m_usingAssociationList);
			for(const auto& rcblit: *m_usingAssociationList) {
			//while(++rcblit){
				std::cout << "//--" << rcblit->theName();
				if(r && r==rcblit){
					std::cout << "<--- Requesting Association !! ";
				}
				std::cout << std::endl;
			}
		} else if(m_other->m_usingAssociationList){
			m_other->listCommitted(r);
		}
	}
	

EdgeType
Edge::theType(void)
	{
		assert(0);
		return UndefinedEdgeType;
	}

double
Edge::getCurrent()
	{
		return getVoltage()/getAdmittance();
	}

double
Edge::getVoltage()
	{
		return (Dest->getVoltage() - Sorc->getVoltage());
	}
	
double
Edge::getAdmittance()
	{
		return 1.0/m_Cost;
	}
	
