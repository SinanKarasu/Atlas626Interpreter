#include	"Types.h"
#include	"Resource.h"
#include	"Vertex.h"
#include	"VertexStack.h"
#include	"Graph.h"
#include	"DFSContext.h"


DFSContext::DFSContext(const int StartNode,const int TargetNode,const int _maxHops,const double _maxCost)
	:CURRENTStack(new VertexStack())
	{
		init( StartNode, TargetNode, _maxHops, _maxCost);
	}

DFSContext::DFSContext(Vertex * StartNode,Vertex * TargetNode,const int _maxHops,const double _maxCost)
	:CURRENTStack(new VertexStack())
	{
		init( StartNode, TargetNode, _maxHops, _maxCost);
	}

DFSContext::~DFSContext()
	{
		delete CURRENTStack;
	}

void 
DFSContext::init(const int StartNode,const int TargetNode,const int _maxHops,const double _maxCost)
	{
		Graph::G_DfsSearchStart=Graph::G_DfsSearch+1;
		count1=Graph::G_DfsSearchStart;
		count2=Graph::G_DfsSearchStart;
			
		v =Graph::Table[StartNode];
		w =Graph::Table[TargetNode];
		maxHops=_maxHops;
		maxCost=_maxCost;
		actualHops=0;
		searchMode=0;
	}

void 
DFSContext::init(Vertex * StartNode,Vertex * TargetNode,const int _maxHops,const double _maxCost)
	{
		Graph::G_DfsSearchStart=Graph::G_DfsSearch+1;
		count1=Graph::G_DfsSearchStart;
		count2=Graph::G_DfsSearchStart;
			
		v =StartNode;
		w =TargetNode;
		maxHops=_maxHops;
		maxCost=_maxCost;
		actualHops=0;
	}


void 
DFSContext::CURRENTStackPush(Vertex *x)
	{
		CURRENTStack->Push(x);
	}
int
DFSContext::CURRENTStackEmpty()
	{
		return CURRENTStack->Empty();
	}

Vertex *
DFSContext::CURRENTStackTop()
	{
		return CURRENTStack->Top();
	}

Vertex *
DFSContext::CURRENTStackPop()
	{
		return CURRENTStack->Pop();
	}

