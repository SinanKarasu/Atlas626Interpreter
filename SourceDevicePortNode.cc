#include	"Types.h"
#include	"Graph.h"
#include	"NodeName.h"
#include	"Vertex.h"
#include	"DevicePortNode.h"
#include	"SourceDevicePortNode.h"

// const NodeType SourceDevicePortNode::m_NodeType=SourceDevicePortNodeType;

SourceDevicePortNode::SourceDevicePortNode (const NodeName & node )
		:DevicePortNode ( node , SourceDevicePortNodeType )
		{
		}


int
SourceDevicePortNode::admitConnect(Vertex * w)
	{
	// What cehcks can we do here?
	// 1)	Check that the other guy is not another pin of this source
	// 2)	If the other guy is another source/load then we don't have
	// 	a cycle consisting of shorts/loads/sources
	//
		if(getResource()==w->getResource()){
			// attemp to short out a source
			return 0;
		} else {
			return 1;
		}
	}

RWCString
SourceDevicePortNode::theName()
	{
		return "(SRC)"+DevicePortNode::theName();
	}

void
SourceDevicePortNode::commitEquivalence(Vertex * w)
	{
		commit(this);
	}

int
SourceDevicePortNode::isSource(Vertex * w)
	{
		return 1;
	}

NodeType
SourceDevicePortNode::theType(void)
	{
		return m_NodeType;
	}
