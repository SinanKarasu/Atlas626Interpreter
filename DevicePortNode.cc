#include	"Types.h"
#include	"Graph.h"
#include	"NodeName.h"
#include	"DevicePortNode.h"

DevicePortNode::DevicePortNode (const NodeName & node , NodeType nodeType )
		:Vertex ( node , nodeType )
		{
		}

int
DevicePortNode::admitConnect(Vertex * w)
	{
		if(dynamicClass()==w->dynamicClass()){
			return 1;
		} else {
			return 0;
		}
	}

RWCString
DevicePortNode::theName()
	{
		return Vertex::theName();
	}
