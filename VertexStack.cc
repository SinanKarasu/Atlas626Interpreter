#include	"Types.h"
#include	"VertexStack.h"

VertexStack::VertexStack ()
		:RWTValOrderedVector<Vertex *> ()
	{
	}
	
RWBoolean
VertexStack::Empty()
	{
		return (length()==0);
	}
	
void
VertexStack::Push(Vertex * v)
	{
		append(v);
	}
	
Vertex * 
VertexStack::Top()
	{
		return last();
	}
	
Vertex *
VertexStack::Pop()
	{
		return removeLast();
	}
