
#include	"Types.h"
#include	"Resource.h"
#include	"Graph.h"
#include	"NodeName.h"
#include	"Vertex.h"
#include	"TAGContext.h"
#include	"Edge.h"
#include	"Equivalence.h"


int	Vertex::V_DFSDynamicSearch = 1 ;

Vertex::Vertex(const NodeName & name,NodeType nodeType)
		:m_NodeName		( name  )
		,m_search		( c_Und )
		,m_CurrentNum		( c_Und )
		,m_DfsNum		( c_Und )
		,m_BfsNum		( c_Und )
		,m_DfsDyn		( c_Und )
		,m_CompNum		( c_Und )
		,m_ConstantParent	( c_Und )
		,m_DynamicParent	( c_Und )
		,m_LowPt		( c_Und )
		,m_Visited		( c_Und )
		,m_Jumps		( c_Und )
		,m_nodeType		( nodeType )
		,m_PrevEdge		(0)
		,m_Equivalence		(0)
		{
		}

Vertex::operator	int()		const	{ return m_Rank;}
	
RWCString
Vertex::getName()	const	{ return m_NodeName.getName(); }

Resource * 	
Vertex::getResource()	const	{ return m_NodeName.getResource(); }

RWCString
Vertex::theName()
	{
		return m_NodeName.theName();
	}

Vertex *
Vertex::equivalenceVertex()
	{
		return Graph::vertex(equivalenceClass());
	}

void
Vertex::insertEdge(Edge * edge)
	{
		edge->insertThyself(Adj);
	}

int
Vertex::DFSNUM(const int num)
	{
		if(num!=c_Und)m_DfsNum=num;
		return m_DfsNum;
	}

int
Vertex::COMPNUM(const int num)
	{
		if(num!=c_Und)m_CompNum=num;
		return m_CompNum;
	}

int
Vertex::LOWPT(const int num)
	{
		if(num!=c_Und)m_LowPt=num;
		return m_LowPt;
	}

int
Vertex::CURRENT(const int num)
	{
		if(num!=c_Und)m_CurrentNum=num;
		return m_CurrentNum;
	}
		

int
Vertex::admitConnect(Vertex * w)
		{
			return 1;
		}

NodeType
Vertex::getNodeType()
	{
		return m_nodeType;
	}

const Impedance & Vertex::Dist()
	{
		if(m_search < m_Graph->G_CurrentSearch){
			m_Dist	=m_Graph->Inf();		// set to infinity
			m_Prev = Graph::NullVertex;
			Scratch = 0;
			m_search=m_Graph->G_CurrentSearch;	// lock it in place
		}
		return m_Dist; 
	}

void Vertex::setDist(const Impedance & Dist)
	{
		if(m_search < m_Graph->G_CurrentSearch){
			m_Prev = Graph::NullVertex;
			Scratch = 0;
			m_search=m_Graph->G_CurrentSearch;	// lock it in place
		}
		m_Dist=Dist;
	}

Equivalence *
Vertex::getEquivalence()
	{
		if(equivalenceClass()==m_Rank){
			if(!m_Equivalence){
				m_Equivalence=new Equivalence(m_Rank);
			}
			return m_Equivalence;
		} else {
			return equivalenceVertex()->getEquivalence();
		}
	}

int
Vertex::setEquivalenceClass	(const int parent)
	{		
			m_ConstantParent=parent;
			//EdgeListIterator P(*(Adj));
			int low=m_Rank;
			for(const auto& P: *(Adj)){
			//while( ++P ){
				Edge * e=P;Vertex * Dest;
				if( Dest=e->Destination(HardWiredMode )){
					if( e->Dest->m_ConstantParent==c_Und ){
						int x=e->Dest->setEquivalenceClass(m_ConstantParent);
						low=(x<low)?x:low;
					}
				}
			}
			if(m_ConstantParent==m_Rank){
				m_ConstantParent=low;
				Graph::vertex(m_ConstantParent)->m_ConstantParent=m_ConstantParent;
			}
			return low;	
	}


int
Vertex::equivalenceClass()
	{
		if( m_ConstantParent==c_Und ){
			m_ConstantParent=setEquivalenceClass(m_ConstantParent=m_Rank);
		}
		if( m_ConstantParent==m_Rank ){
			return m_Rank;
		}
		Vertex * parent = Graph::vertex(m_ConstantParent);
		return m_ConstantParent = parent->equivalenceClass();
	}

int
Vertex::setDynamicClass(const int parent)
	{
			m_DfsDyn = c_Und-1;
			m_DynamicParent=parent;
			//EdgeListIterator P(*(Adj));
			int low=m_Rank;
			for(const auto& P: *(Adj)){
			//while( ++P ){
				Edge * e=P;
				if( e->Destination(CurrentMode )){
					if( e->Dest->m_DfsDyn<c_Und ){
						// looped back, ignore....
					} else if( e->Dest->m_DfsDyn<V_DFSDynamicSearch ){
						// haven't visited yet....
						linkDynamic(e);
						int x=e->Dest->setDynamicClass(m_DynamicParent);
						low=(x<low)?x:low;
					} else {
						low=(e->Dest->m_Rank<low)?e->Dest->m_Rank:low;
					}
				}
			}
			//if(m_DynamicParent==m_Rank){
				m_DynamicParent=low;
				//Graph::vertex(m_DynamicParent)->m_DynamicParent=m_DynamicParent;
			//}
			m_DfsDyn = V_DFSDynamicSearch;
			return low;	
	}
	


int
Vertex::dynamicClass()
	{
		if( m_DfsDyn < V_DFSDynamicSearch ){
			m_DynamicParent=setDynamicClass(m_DynamicParent=m_Rank);
		}
		if(m_DynamicParent==m_Rank ){
			return m_Rank;
		}
		Vertex * parent = Graph::vertex(m_DynamicParent);
		return m_DynamicParent = parent->dynamicClass();

	}


int
Vertex::visited(const int v)
	{
		if( v==1 ){
			m_Visited = m_Graph->G_CurrentSearch;
		}
		return m_Visited>=m_Graph->G_CurrentSearch;
	}
int
Vertex::isSource(Vertex * w)
	{
		assert(0);
		return 0;
	}

int
Vertex::DFSvisited()
	{
		return m_DfsNum>=Graph::G_DfsSearchStart;
	}

int
Vertex::committed(Vertex * v)
	{
		Equivalence * eq=getEquivalence();
		return eq->committed(v,eq);
	}

int
Vertex::onlycommitted(Vertex * v)
	{
		Equivalence * eq=getEquivalence();
		return eq->onlycommitted(v,eq);
	}

void
Vertex::listCommitted(Vertex * v)
	{
		Equivalence * eq=getEquivalence();
		eq->listCommitted(v,eq);
	}

void
Vertex::commit(Vertex * v)
	{
		getEquivalence()->commit(v);
	}

void
Vertex::uncommit(Vertex * v)
	{
		assert(0);
		//m_Graph->getEquivalence(equivalenceClass())->uncommit(v);
	}

Vertex *
Vertex::linkDynamic(Edge * e)
	{
		m_Graph->linkDynamic(e);
		return this;
	}
	
Vertex *
Vertex::unlinkDynamic()
	{
		m_Graph->unlinkDynamic(this);
		return this;
	}

Vertex *
Vertex::checkSourceLoops(Vertex * v)
	{
		DFSContext c(this,this);
		return checkSourceLoop(v,c);
	}


Vertex *
Vertex::checkSourceLoop(Vertex * o,DFSContext & c)
	{
		Vertex * loopedThru=this;
		std::cout << "Checking At Vertex " << theName() << std::endl;
		DFSNUM(++Graph::G_DfsSearch);
		if(isSource()){
			getResource()->checkLoopThruConnections(this,c);
		}
		//EdgeListIterator P(*(Adj));
		for(const auto& P: *(Adj)){
		//while(++P){
			Edge * e=P;
			Vertex * w;
			if(w=e->Destination(HardWiredMode)){
				if(!(w->DFSvisited())){
					Vertex * looped=0;
					looped=w->checkSourceLoop(this,c);
					if(looped->m_DfsNum<loopedThru->m_DfsNum){
						loopedThru = looped;
					}
				} else if(w==o){
					// nothing to do, this is how we got here..
				} else if(w->getEquivalence()==getEquivalence()){
					std::cerr	<<	" Oh oh ! trouble at"
						<<	w->theName()
						<<	" seen from "
						<<	theName()
						<<	std::endl;
				}
			}
		}
		if(loopedThru==this){
			return o;
		} else {
			return loopedThru;
		}
	
	}

NodeType 
Vertex::theType(void)
	{
		assert(0);
		return UndefinedNodeType;
	}

double
Vertex::computeVoltage()
	{
		//EdgeListIterator P(*(Adj));
		double sum_vy=0.0;double sum_y=0.0;
		for(const auto& P: *(Adj)){
		//while(++P){
			Edge * e=P;
			Vertex * w;
			if(w=e->Destination(AnyPathMode)){
				double y_i=e->getAdmittance();
				double v_i=w->getVoltage();
				sum_vy	+=	v_i*y_i	;
				sum_y	+=	y_i	;
			}
		}
		if(sum_y!=0.0){
			m_Voltage=sum_vy/sum_y;
		} else {	// we are not connected anywhere
			//	m_voltage	=	m_Voltage;
		}
		
		return m_Voltage;
				
	}

double
Vertex::getVoltage()
	{
		return m_Voltage;
	}

//double
//Vertex::getAdmittance()
//	{
//	
//	}
