#include	"Resource.h"
#include	"TAGContext.h"
#include	"Edge.h"
#include	"Vertex.h"
#include	"Wire.h"

const EdgeType Wire::m_theType=WireType;

Set &
Wire::theSet()
	{
		return m_energizer;
	}

Wire::Wire(Resource * r,Vertex * S,Vertex * D,Impedance z)
	:Edge(S,D,z)
	{
		m_Resource=r;
		m_state=0;
		m_energizer=1;
	}
	
RWCString Wire::theName()
	{
		RWCString n="<";
		n+=Sorc->theName();
		n+="[---]";
		n+=Dest->theName();
		n+=">";
		return n;
	}

Edge *
Wire::clone(Resource * r)
	{
		Vertex * s=r->AddNode(Sorc->getName(),Sorc->getNodeType());
		Vertex * d=r->AddNode(Dest->getName(),Dest->getNodeType());
		Edge * e=new Wire(r,s,d,m_Cost);
		return e;
	}

int
Wire::getState(){return 0;}

EdgeType Wire::theType()
	{
		return m_theType;
	}
