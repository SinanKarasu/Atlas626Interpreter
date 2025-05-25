#ifndef PathNode_H
#define PathNode_H

#include	"Vertex.h"

class PathNode : public Vertex
{
public:
	PathNode (Resource * r,Vertex * v,AST * cmd=0);
	virtual int		admitConnect		(Vertex * w) ;
	virtual RWCString	theName			()	;
	virtual	void		commitEquivalence	(Vertex * w=0);
	virtual	int		isSource		(Vertex * w=0);

protected:
	Vertex * m_shadow;
	AST * m_cmd;
};


#endif	// PathNode_H
