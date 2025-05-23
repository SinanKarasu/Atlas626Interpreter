#include	"Types.h"
#include	"Graph.h"
#include	"NodeName.h"
#include	"Vertex.h"
#include	"AdapterConnectorNode.h"

AdapterConnectorNode::AdapterConnectorNode (const NodeName & node )
		:Vertex ( node , AdapterConnectorNodeType )
		{
		}

int
AdapterConnectorNode::admitConnect(Vertex * w)
	{
			return 1;
	}

RWCString 
AdapterConnectorNode::theName() 
	{
		return "ATE:"+getName();
	}
void
AdapterConnectorNode::commitEquivalence(Vertex * w)
	{
		if(w && (w!=this)){		// we don't commit, period....
			assert(equivalenceClass()==w->equivalenceClass());
			w->commitEquivalence(w);// pass it on....
		}
	}
int
AdapterConnectorNode::isSource(Vertex * w)
	{
		return 0;
	}
