#include	"Types.h"
#include	"Graph.h"
#include	"NodeName.h"
#include	"Vertex.h"
#include	"DevicePortNode.h"
#include	"LoadDevicePortNode.h"

LoadDevicePortNode::LoadDevicePortNode (const NodeName & node )
		:DevicePortNode ( node , LoadDevicePortNodeType )
		{
		}

int
LoadDevicePortNode::admitConnect(Vertex * w)
	{
		if(getResource()==w->getResource()){
			// attemp to short out a load
			return 0;
		} else {
			return 1;
		}
	}

RWCString
LoadDevicePortNode::theName()
	{
		return "(LOD)"+DevicePortNode::theName();
	}

void
LoadDevicePortNode::commitEquivalence(Vertex * w)
	{
		commit(this);
	}
int
LoadDevicePortNode::isSource(Vertex * w)
	{
		return 1;
	}
