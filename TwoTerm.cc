#include	"Resource.h"
#include	"TAGContext.h"
#include	"Edge.h"
#include	"Vertex.h"
#include	"TwoTerm.h"

const EdgeType TwoTerm::m_theType=TwoTermType;

Set &
TwoTerm::theSet()
	{
		return m_energizer;
	}

TwoTerm::TwoTerm(Resource * r,Vertex * S,Vertex * D,Impedance z)
	:Edge(S,D,z)
	{
		m_Resource=r;
		m_state=0;
		m_energizer=1;
	}
	
RWCString TwoTerm::theName()
	{
		RWCString n="<";
		n+=Sorc->theName();
		n+="[???]";
		n+=Dest->theName();
		n+=">";
		return n;
	}

Edge *
TwoTerm::clone(Resource * r)
	{
		Vertex * s=r->AddNode(Sorc->getName(),Sorc->getNodeType());
		Vertex * d=r->AddNode(Dest->getName(),Dest->getNodeType());
		Edge * e=new TwoTerm(r,s,d,m_Cost);
		return e;
	}

int
TwoTerm::getState(){return 0;}

EdgeType TwoTerm::theType()
	{
		return m_theType;
	}
