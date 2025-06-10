#include	"Resource.h"
//#include	"ReverseMap.h"


ReverseMapEntry::ReverseMapEntry(const RWCString & key,Resource * r,const int s)
	:m_key(key)
	,m_Resource(r)
	,m_state(s)
	,m_child(0)
	{
	}
	
Resource *
ReverseMapEntry::getResource()	{return	m_Resource;}
RWCString &
ReverseMapEntry::getKey()	{return m_key;}
int	
ReverseMapEntry::getState()	{return m_state;}

void
ReverseMapEntry::setChild(ReverseMapEntry * rme)
	{
		if(m_child){
			m_child->setChild(rme);
		} else {
			m_child=rme;
		}
	}

ReverseMapEntry *	
ReverseMapEntry::getChild()
	{
		return m_child;
	}

ReverseMap::ReverseMap()
	:m_NodeNodeList(0)
	{
	}


int
ReverseMap::setNodeNodeListCallBack(ReverseMapCallBack f)
	{
		m_NodeNodeList = f ;
		return 0;
	}


ReverseMapEntry *
ReverseMap::insertState( const RWCString & key, Resource * r , int state )
	{
		ReverseMapEntry * rme=new ReverseMapEntry(key,r,state);
		ReverseMapEntry * x=0;
		if(m_ReverseMapDictionary.findValue(key,x)){
			x->setChild(rme);
		} else {
			m_ReverseMapDictionary.insertKeyAndValue(key,rme);
		}
		return rme;
	}


void
DFSGet(Vertex * v,Set & nt,VertexList & vl)
	{
		v->DFSNUM(++Graph::G_DfsSearch);
		if(nt.isSet(v->getNodeType())){
			vl.append(v);
		}
		//EdgeListIterator P(*(v->Adj));
		for(const auto& P: *(v->Adj)) {
		//while(++P){
			Edge * e=P;
			Vertex * w=e->Destination(CurrentMode);
			if(w && (!(w->DFSvisited()))){
				DFSGet(w,nt,vl);
			}
		}
		
	}


void
ReverseMap::reportUUTNodeConnections(Vertex * v)
	{
		Set nt;
		nt.set( SourceDevicePortNodeType );
		nt.set( SensorDevicePortNodeType );
		nt.set( PointSourceNodeType );
		nt.set( LoadDevicePortNodeType );

		Graph::G_DfsSearchStart=Graph::G_DfsSearch+1;
		VertexList ateNodes;
		DFSGet(v,nt,ateNodes);
		if(ateNodes.size()>0){
			(*m_NodeNodeList)(v,ateNodes);
		}
	}


int
ReverseMap::getUUTNodes(Vertex * v)
	{

		VertexList uutNodes;
		Set nt;
		nt.set(UutConnectorNodeType);

		Graph::G_DfsSearchStart=Graph::G_DfsSearch+1;
		DFSGet(v,nt,uutNodes);
		//VertexListIterator vlit(uutNodes);
		int count=0;
		for(const auto& vlit: uutNodes) {
		//while(++vlit){
			count++;
			reportUUTNodeConnections(vlit);
		}
		return count;
	}
ReverseMapEntry *
ReverseMap::setResourceState(const RWCString & key)
	{
		ReverseMapEntry * rme=0;
		m_ReverseMapDictionary.findValue(key,rme);
		ReverseMapEntry *x=rme;
		while(rme){
		
			rme->getResource()->setState(rme->getState());

			// now we have to process the affected
			// Resource and all on it's ports, and
			// report it as such...
			rme=rme->getChild();
		}
		if(m_NodeNodeList){
			//VertexDictionaryIterator mvlit(m_manipulatedVertices);
			for(const auto& mvlit: m_manipulatedVertices) {
			//while(++mvlit){
				Vertex * v=0;
				m_manipulatedVertices.findValue(mvlit.first,v);
				if(v){
					getUUTNodes(v);
				}
				m_processedVertices.insertKeyAndValue(mvlit.first,v);
			}
			m_manipulatedVertices.clear();
		}
		return x;
	}
void
ReverseMap::processEdge(Edge * e)
	{
		if(m_NodeNodeList){
			m_manipulatedVertices.insertKeyAndValue(e->Sorc->getName(),e->Sorc);
			m_manipulatedVertices.insertKeyAndValue(e->Dest->getName(),e->Dest);
		}
	}
