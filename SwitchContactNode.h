#ifndef SwitchContactNode_H
#define SwitchContactNode_H

class SwitchContactNode : public Vertex
{
public:
	SwitchContactNode (const NodeName & node );
	virtual int		admitConnect		(Vertex * w) ;
	virtual RWCString 	theName			()	;
	virtual	void		commitEquivalence	(Vertex * w=0);
	virtual	int		isSource		(Vertex * w=0);
	virtual NodeType	theType(void);
private:
	static const NodeType m_NodeType = SwitchContactNodeType;
};


#endif // SwitchContactNode_H
