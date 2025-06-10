#include	"Types.h"
#include	"Graph.h"
#include	"NodeName.h"
#include	"Vertex.h"
#include	"UutConnectorNode.h"

// const NodeType UutConnectorNode::m_NodeType=UutConnectorNodeType;

UutConnectorNode::UutConnectorNode(const NodeName & node )
		:Vertex ( node , UutConnectorNodeType )
		{
		}

int
UutConnectorNode::admitConnect(Vertex * w)
	{
		return 1;
	}

RWCString
UutConnectorNode::theName()
	{
		return "UUT:"+getName();
	}

void
UutConnectorNode::commitEquivalence(Vertex * w)
	{
		commit(this);
	}
int
UutConnectorNode::isSource(Vertex * w)
	{
		return 1;
	}

NodeType UutConnectorNode::theType(void)
	{
		return m_NodeType;
	}
