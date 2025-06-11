#include	"Types.h"
#include	"VertexList.h"

// VertexList::VertexList()
// 	:RWTValSlist<Vertex *>()
// 	{
// 	}
// 
// VertexListIterator::VertexListIterator( VertexList &d )
// 	:RWTValSlistIterator<Vertex *> (d)
// 	{
// 	}
// 
// 
VertexListStack::VertexListStack()
	:RWTStack< VertexList * , RWTValOrderedVector < VertexList * > >()
,m_instantiated(0){}

VertexList *
VertexListStack::getOne()
	{
		if(empty()){
			m_instantiated++;
			return new VertexList;
		} else {
			auto theLastOne = top(); pop(); 
			return theLastOne;
		}
	}
