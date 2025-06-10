#include	"Types.h"
#include	"VertexStack.h"

VertexStack::VertexStack ()
		:RWTValOrderedVector<Vertex *> ()
	{
	}
	
RWBoolean
VertexStack::Empty()
	{
		return empty();
	}
	
void
VertexStack::Push(Vertex * v)
	{
		push_back(v);
	}
	
Vertex * 
VertexStack::Top()
	{
		return back();
	}
	
Vertex *
VertexStack::Pop()
	{
		auto theLastOne = Top(); pop_back(); // pople always complain that the non-native speakers of English tend to skip the definitive article the 'the'.
										// So here it is.
		return theLastOne;
	}
