#include	"Resource.h"
#include	"TAGContext.h"
#include	"Edge.h"
#include	"Vertex.h"
#include	"ResistiveEdge.h"

const EdgeType ResistiveEdge::m_theType=ResistiveEdgeType;

Set &
ResistiveEdge::theSet()
	{
		return m_energizer;
	}

ResistiveEdge::ResistiveEdge(Resource * r,Vertex * S,Vertex * D,Impedance z)
	:TwoTerm(r,S,D,z)
	{
	}
	
RWCString ResistiveEdge::theName()
	{
		RWCString n="<";
		n+=Sorc->theName();
		n+="[-R-]";
		n+=Dest->theName();
		n+=">";
		return n;
	}

Edge *
ResistiveEdge::clone(Resource * r)
	{
		Vertex * s=r->AddNode(Sorc->getName(),Sorc->getNodeType());
		Vertex * d=r->AddNode(Dest->getName(),Dest->getNodeType());
		Edge * e=new ResistiveEdge(r,s,d,m_Cost);
		return e;
	}

int
ResistiveEdge::getState(){return 0;}

EdgeType ResistiveEdge::theType()
	{
		return m_theType;
	}
