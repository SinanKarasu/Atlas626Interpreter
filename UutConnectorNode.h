#ifndef UutConnectorNode_H
#define UutConnectorNode_H
class UutConnectorNode:public Vertex{
public:
	UutConnectorNode(const NodeName & node );
	virtual int		admitConnect		(Vertex * w) ;
	virtual RWCString 	theName			(); 
	virtual	void		commitEquivalence	(Vertex * w=0);
	virtual	int		isSource		(Vertex * w=0);
	virtual NodeType	theType(void);
private:
	static const NodeType m_NodeType=UutConnectorNodeType; // some random number.
};

#endif // UutConnectorNode_H
