#include	"Types.h"
#include	"Graph.h"
#include	"NodeName.h"
#include	"Vertex.h"
#include	"PointSourceNode.h"

PointSourceNode::PointSourceNode(const NodeName & node )
		:Vertex ( node , PointSourceNodeType )
		{
		}

int
PointSourceNode::admitConnect(Vertex * w)
	{
		return 1;
	}

RWCString
PointSourceNode::theName()
	{
		return "PNT:"+getName();
	}

void
PointSourceNode::commitEquivalence(Vertex * w)
	{
		commit(this);
	}

int
PointSourceNode::isSource(Vertex * w)
	{
		return 1;
	}
