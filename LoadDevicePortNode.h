#ifndef LoadDevicePortNode_H
#define LoadDevicePortNode_H

class LoadDevicePortNode : public DevicePortNode
{
public:
	LoadDevicePortNode (const NodeName & node );
	virtual int		admitConnect		(Vertex * w) ;
	virtual RWCString	theName			();
	virtual	void		commitEquivalence	(Vertex * w=0);
	virtual	int		isSource		(Vertex * w=0);

};


#endif // LoadDevicePortNode_H
