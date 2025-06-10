#ifndef VertexList_H
#define VertexList_H

class Vertex;

// class VertexList : public RWTValSlist<Vertex *> {
// public:
// 	VertexList();
// };

using VertexList = AppendableList<Vertex *>;

// class VertexListIterator : public RWTValSlistIterator<Vertex *>{
// 	public: VertexListIterator( VertexList &d );
// };


class VertexListStack : public RWTStack< VertexList * , RWTValOrderedVector < VertexList * > >{
public:
	VertexListStack();
	VertexList * getOne();
private:
	int m_instantiated;
};


#endif // VertexList_H
