#include	"Types.h"
#include	"Graph.h"
#include	"NodeName.h"
#include	"Vertex.h"
#include	"DevicePortNode.h"
#include	"SensorDevicePortNode.h"

//const NodeType SensorDevicePortNode::m_NodeType=SensorDevicePortNodeType;

SensorDevicePortNode::SensorDevicePortNode (const NodeName & node )
		:DevicePortNode ( node , SensorDevicePortNodeType )
		{
		}

int
SensorDevicePortNode::admitConnect(Vertex * w)
	{
	// might consider giving this more authority. E.g.
	// we may ask all we own ( all the pins we know that we 
	// are connected to ) if they admitConnect(w).
			return 1;
	}

RWCString
SensorDevicePortNode::theName()
	{
		return "(SNS)"+DevicePortNode::theName();
	}

void
SensorDevicePortNode::commitEquivalence(Vertex * w)
	{
		commit(this);
	}

int
SensorDevicePortNode::isSource(Vertex * w)
	{
		return 0;
	}

NodeType SensorDevicePortNode::theType(void)
	{
		return m_NodeType;
	}	
