#ifndef AdapterConnectorNode_H
#define AdapterConnectorNode_H

class AdapterConnectorNode : public Vertex {
public:
	AdapterConnectorNode (const NodeName & node );
	virtual int		admitConnect		(Vertex * w) ;
	virtual RWCString	theName			()	;
	virtual	void		commitEquivalence	(Vertex * w=0);
	virtual	int		isSource(Vertex * w=0);
};

#endif // AdapterConnectorNode_H
