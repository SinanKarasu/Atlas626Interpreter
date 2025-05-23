#ifndef DevicePortNode_H
#define DevicePortNode_H

#include	"Vertex.h"

class DevicePortNode : public Vertex
{
public:
	virtual RWCString	theName()	; 

protected:
	DevicePortNode (const NodeName & node , NodeType nodeType );
	virtual int admitConnect(Vertex * w) ;
};

#endif // DevicePortNode_H
