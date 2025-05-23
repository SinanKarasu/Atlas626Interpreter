#ifndef VertexStack_H
#define VertexStack_H

class Vertex;

class VertexStack : public RWTValOrderedVector<Vertex *> {
public:
	VertexStack ();
	
	RWBoolean Empty();
	void Push(Vertex * v);
	Vertex * Top();
	Vertex * Pop();
};


#endif // VertexStack_H
