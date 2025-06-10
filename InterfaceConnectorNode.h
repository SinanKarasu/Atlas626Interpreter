#ifndef InterfaceConnectorNode_H
#define InterfaceConnectorNode_H

class InterfaceConnectorNode : public Vertex
{
public:
	InterfaceConnectorNode (const NodeName & node );
	virtual int		admitConnect		(Vertex * w) ;
	virtual RWCString	theName			()	;
	virtual	void		commitEquivalence	(Vertex * w=0);
	virtual	int		isSource(Vertex * w=0);
	virtual NodeType	theType(void);
private:
	static const NodeType m_NodeType = InterfaceConnectorNodeType;
};

#endif // InterfaceConnectorNode_H
