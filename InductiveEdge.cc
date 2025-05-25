#include	"Resource.h"
#include	"TAGContext.h"
#include	"Edge.h"
#include	"Vertex.h"
#include	"InductiveEdge.h"

const EdgeType InductiveEdge::m_theType=InductiveEdgeType;

Set &
InductiveEdge::theSet()
	{
		return m_energizer;
	}

InductiveEdge::InductiveEdge(Resource * r,Vertex * S,Vertex * D,Impedance z)
	:TwoTerm(r,S,D,z)
	{
	}
	
RWCString InductiveEdge::theName()
	{
		RWCString n="<";
		n+=Sorc->theName();
		n+="[-L-]";
		n+=Dest->theName();
		n+=">";
		return n;
	}

Edge *
InductiveEdge::clone(Resource * r)
	{
		Vertex * s=r->AddNode(Sorc->getName(),Sorc->getNodeType());
		Vertex * d=r->AddNode(Dest->getName(),Dest->getNodeType());
		Edge * e=new InductiveEdge(r,s,d,m_Cost);
		return e;
	}

int
InductiveEdge::getState(){return 0;}

EdgeType InductiveEdge::theType()
	{
		return m_theType;
	}
