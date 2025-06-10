#ifndef SensorDevicePortNode_H
#define SensorDevicePortNode_H

class SensorDevicePortNode : public DevicePortNode{
public:
	SensorDevicePortNode (const NodeName & node );
	virtual int		admitConnect		(Vertex * w);
	virtual RWCString	theName			(); 
	virtual	void		commitEquivalence	(Vertex * w=0);
	virtual	int		isSource		(Vertex * w=0);
	virtual NodeType        theType(void);
private:
	static const NodeType m_NodeType = SensorDevicePortNodeType;

};

#endif // SensorDevicePortNode_H
