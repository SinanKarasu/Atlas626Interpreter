#ifndef EdgeList_H
#define EdgeList_H

class Edge;

class EdgeList : public RWTValSlist<Edge *>
{
public:
	EdgeList();
	int contains(Edge * e);
	void print(class Association * r=0);
};

class EdgeListIterator : public RWTValSlistIterator<Edge *>
{
public:
	EdgeListIterator( EdgeList &d );
};

#endif // EdgeList_H
