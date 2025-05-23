#include	"Types.h"
#include	"Graph.h"
#include	"NodeName.h"
#include	"Vertex.h"
#include	"DevicePortNode.h"
#include	"DeviceReferencePortNode.h"

DeviceReferencePortNode::DeviceReferencePortNode (const NodeName & node )
		:DevicePortNode ( node , DeviceReferencePortNodeType )
		{
		}

int
DeviceReferencePortNode::admitConnect(Vertex * w)
	{
	// might consider giving this more authority. E.g.
	// we may ask all we own ( all the pins we know that we 
	// are connected to ) if they admitConnect(w).
			return 1;
	}

RWCString
DeviceReferencePortNode::theName()
	{
		return "(SNS)"+DevicePortNode::theName();
	}

void
DeviceReferencePortNode::commitEquivalence(Vertex * w)
	{
		commit(this);
	}
int
DeviceReferencePortNode::isSource(Vertex * w)
	{
		return 0;
	}
