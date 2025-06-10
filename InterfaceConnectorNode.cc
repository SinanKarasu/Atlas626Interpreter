#include	"Types.h"
#include	"Graph.h"
#include	"NodeName.h"
#include	"Vertex.h"
#include	"InterfaceConnectorNode.h"

// const NodeType InterfaceConnectorNode::m_NodeType=InterfaceConnectorNodeType;

InterfaceConnectorNode::InterfaceConnectorNode (const NodeName & node )
		:Vertex ( node , InterfaceConnectorNodeType )
		{
		}

int
InterfaceConnectorNode::admitConnect(Vertex * w)
	{
			return 1;
	}

RWCString
InterfaceConnectorNode::theName()
	{
		return "ATE:"+getName();
	}
void
InterfaceConnectorNode::commitEquivalence(Vertex * w)
	{
		if(w && (w!=this)){		// we don't commit, period....
			assert(equivalenceClass()==w->equivalenceClass());
			w->commitEquivalence(w);// pass it on....
		}
	}
int
InterfaceConnectorNode::isSource(Vertex * w)
	{
		return 0;
	}

NodeType
InterfaceConnectorNode::theType(void)
	{
		return m_NodeType;
	}
