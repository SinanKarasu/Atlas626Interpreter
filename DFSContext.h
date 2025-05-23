#ifndef DFSContext_H
#define DFSContext_H

class Vertex;

class DFSContext {
public:
	DFSContext(const int StartNode,const int TargetNode,const int _maxHops=10,const double _maxCost=0.050);
	DFSContext(Vertex *  StartNode,Vertex *  TargetNode,const int _maxHops=10,const double _maxCost=0.050);
	virtual ~DFSContext();
	void init(const int StartNode,const int TargetNode,const int _maxHops=10,const double _maxCost=0.050);
	void init(Vertex *  StartNode,Vertex *  TargetNode,const int _maxHops=10,const double _maxCost=0.050);
	
	int accept(Vertex * x);

	void		CURRENTStackPush(Vertex *w);
	int		CURRENTStackEmpty();
	Vertex *	CURRENTStackTop();
	Vertex *	CURRENTStackPop();

	int count1;
	int count2;
	int maxHops;
	int actualHops;
	Impedance maxCost;
	Vertex * v ;
	Vertex * w ;
	int searchMode;
	
protected:
	class VertexStack * CURRENTStack;

};

#endif // DFSContext_H
