#ifndef PointSourceNode_H
#define PointSourceNode_H


class PointSourceNode:public Vertex
{
public:
	PointSourceNode(const NodeName & node );
	virtual int		admitConnect		(Vertex * w);
	virtual RWCString 	theName			(); 
	virtual	void		commitEquivalence	(Vertex * w);
	virtual	int		isSource		(Vertex * w=0);
};

#endif // PointSourceNode_H
