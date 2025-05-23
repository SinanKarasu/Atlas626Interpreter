#include	"Resource.h"
#include	"TAGContext.h"
#include	"Edge.h"
#include	"Vertex.h"
#include	"CapacitiveEdge.h"

const EdgeType CapacitiveEdge::m_theType=CapacitiveEdgeType;

Set &
CapacitiveEdge::theSet()
	{
		return m_energizer;
	}

CapacitiveEdge::CapacitiveEdge(Resource * r,Vertex * S,Vertex * D,Impedance z)
	:TwoTerm(r,S,D,z)
	{
	}
	
RWCString CapacitiveEdge::theName()
	{
		RWCString n="<";
		n+=Sorc->theName();
		n+="[-C-]";
		n+=Dest->theName();
		n+=">";
		return n;
	}

Edge *
CapacitiveEdge::clone(Resource * r)
	{
		Vertex * s=r->AddNode(Sorc->getName(),Sorc->getNodeType());
		Vertex * d=r->AddNode(Dest->getName(),Dest->getNodeType());
		Edge * e=new CapacitiveEdge(r,s,d,m_Cost);
		return e;
	}

int
CapacitiveEdge::getState(){return 0;}

EdgeType CapacitiveEdge::theType()
	{
		return m_theType;
	}
