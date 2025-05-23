#include	"Types.h"
#include	"Graph.h"
#include	"NodeName.h"
#include	"Vertex.h"
#include	"PathNode.h"


PathNode::PathNode (Resource * r,Vertex * v,AST * cmd)
		:Vertex(NodeName(v->getName()+"_PathNode",r),PathNodeType)
		,m_shadow( v )
		,m_cmd(cmd)
		{
		}

int
PathNode::admitConnect(Vertex * w)
	{
		return 1;
	}

RWCString
PathNode::theName()
	{
		return "PathNode:"+getName();
	}


void
PathNode::commitEquivalence(Vertex * w)
	{
		// this is a dummy right now. has to be fixed....
		if(w && (w!=this)){		// we don't commit, period....
			assert(equivalenceClass()==w->equivalenceClass());
			w->commitEquivalence(w);// pass it on....
		}
	}

int
PathNode::isSource(Vertex * w)
	{
		return 0;
	}
