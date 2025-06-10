#include	"Types.h"
#include	"Graph.h"
#include	"NodeName.h"
#include	"Vertex.h"
#include	"SwitchContactNode.h"

// const NodeType SwitchContactNode::m_NodeType=SwitchContactNodeType;

SwitchContactNode::SwitchContactNode (const NodeName & node )
		:Vertex( node , SwitchContactNodeType )
		{
		}


int
SwitchContactNode::admitConnect(Vertex * w)
	{
		return 1;
	}

RWCString
SwitchContactNode::theName()
	{
		return Vertex::theName();
	}

void
SwitchContactNode::commitEquivalence(Vertex * w)
	{
		if(w && (w!=this)){		// we don't commit, period....
			assert(equivalenceClass()==w->equivalenceClass());
			w->commitEquivalence(w);// pass it on....
		}
	}
int
SwitchContactNode::isSource(Vertex * w)
	{
		return 0;
	}

NodeType
SwitchContactNode::theType(void)
	{
		return m_NodeType;
	}
