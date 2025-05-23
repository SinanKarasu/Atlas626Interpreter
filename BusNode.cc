#include	"Types.h"
#include	"Graph.h"
#include	"NodeName.h"
#include	"Vertex.h"
#include	"DevicePortNode.h"
#include	"BusNode.h"

BusNode::BusNode (const NodeName & node )
		:DevicePortNode ( node , BusNodeType )
		{
		}

int
BusNode::admitConnect(Vertex * w)
	{
		return 1;
	}

RWCString
BusNode::theName()
	{
		return "(BUS)"+DevicePortNode::theName();;
	}

void
BusNode::commitEquivalence(Vertex * w)
	{
		commit(this);
	}

int
BusNode::isSource(Vertex * w)
	{
		return 0;
	}
