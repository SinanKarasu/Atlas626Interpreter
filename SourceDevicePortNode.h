#ifndef SourceDevicePortNode_H
#define SourceDevicePortNode_H

class SourceDevicePortNode : public DevicePortNode{
public:
	SourceDevicePortNode (const NodeName & node );
	virtual int		admitConnect		(Vertex * w) ;
	virtual RWCString 	theName			() ; 
	virtual	void		commitEquivalence	(Vertex * w=0);
	virtual	int		isSource		(Vertex * w=0);
	virtual NodeType	theType(void);
private:
	static const NodeType m_NodeType = SourceDevicePortNodeType;
};

#endif // SourceDevicePortNode_H
