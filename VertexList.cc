#include	"Types.h"
#include	"VertexList.h"

VertexList::VertexList()
	:RWTValSlist<Vertex *>()
	{
	}

VertexListIterator::VertexListIterator( VertexList &d )
	:RWTValSlistIterator<Vertex *> (d)
	{
	}


VertexListStack::VertexListStack():m_instantiated(0){}

VertexList *
VertexListStack::getOne()
	{
		if(isEmpty()){
			m_instantiated++;
			return new VertexList;
		} else {
			return pop();
		}
	}
