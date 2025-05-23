#ifndef DeviceReferencePortNode_H
#define DeviceReferencePortNode_H

class DeviceReferencePortNode : public DevicePortNode{
public:
	DeviceReferencePortNode (const NodeName & node );
	virtual int		admitConnect		(Vertex * w) ;
	virtual RWCString	theName			();
	virtual	void		commitEquivalence	(Vertex * w=0);
	virtual	int		isSource(Vertex * w=0);

};

#endif // DeviceReferencePortNode_H
