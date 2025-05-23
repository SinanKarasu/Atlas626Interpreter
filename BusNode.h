#ifndef BusNode_H
#define BusNode_H


class BusNode : public DevicePortNode{
public:
	BusNode (const NodeName & node );
	virtual int		admitConnect		(Vertex * w) ;
	virtual RWCString	theName			();
	virtual	void		commitEquivalence	(Vertex * w=0);
	virtual	int		isSource		(Vertex * w=0);

};

#endif // BusNode_H
