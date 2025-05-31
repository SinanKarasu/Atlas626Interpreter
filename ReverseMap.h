#ifndef ReverseMap_h
#define ReverseMap_h


class ReverseMapEntry{

public:
		ReverseMapEntry(const RWCString & key,Resource * resource,const int state);

Resource *		getResource();
RWCString	&	getKey();
int			getState();
void			setChild(ReverseMapEntry * rme);
ReverseMapEntry	*	getChild();

protected:
	RWCString	m_key;
	Resource *	m_Resource;
	int		m_state;
	ReverseMapEntry	*	m_child;

};


// chat typedef	(*ReverseMapCallBack)(class Vertex *, VertexList & vl);
typedef void (*ReverseMapCallBack)(Vertex*, VertexList&);

using ReverseMapDictionary = Dictionary<std::string, ReverseMapEntry*>;
using ReverseMapDictionaryIterator = DictionaryIterator<std::string, ReverseMapEntry*>;


class ReverseMap {
public:
	ReverseMap();
	ReverseMapEntry * insertState( const RWCString & key, Resource * resourc , int state ); 
	ReverseMapEntry * setResourceState(const RWCString & key);
	int	getActiveUUTNodes(StringVector &){return 0;};
	int	getActiveATENodes(StringVector &){return 0;};
	int	getConnectedUUTNodes(const RWCString & ATENode , StringVector & ){return 0;};
	int	getConnectedATENodes(const RWCString & UUTNode , StringVector & ){return 0;};

	int	setNodeNodeListCallBack(ReverseMapCallBack f=0);
	void	processEdge(Edge * e);
	void	reportUUTNodeConnections(Vertex * v);
	int	getUUTNodes(Vertex * v);
protected:
	ReverseMapDictionary	m_ReverseMapDictionary;
	ReverseMapCallBack	m_NodeNodeList;
	VertexDictionary	m_manipulatedVertices;
	VertexDictionary	m_processedVertices;

};




#endif // ReverseMap_h
