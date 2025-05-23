#ifndef Vertex_H
#define Vertex_H

#include	"NodeName.h"
#include	"DFSContext.h"


class Vertex{
public:
	Vertex(const NodeName & name,NodeType nodeType);
	operator	int()		const	;
	RWCString	getName()	const	;
	Resource * 	getResource()	const	;

	const Impedance & Dist();
	void setDist(const Impedance & z);

	virtual RWCString  theName() ;

	Equivalence * getEquivalence();

	int equivalenceClass();
	Vertex * equivalenceVertex();
	int dynamicClass();
	
	virtual void	commitEquivalence(Vertex * w=0)=0;

	int DFSNUM(const int num=c_Und);

	int COMPNUM(const int num=c_Und);

	int LOWPT(const int num=c_Und);

	int CURRENT(const int num=c_Und);
		
	int visited(const int v=c_Und);

	int DFSvisited(); 

	virtual int admitConnect(Vertex * w);

	NodeType getNodeType();
			
	virtual int committed(Vertex * v=0);
	virtual int onlycommitted(Vertex * v);
	virtual void listCommitted(Vertex * v=0);

	virtual	int	isSource(Vertex * w=0);

	void	insertEdge(Edge * edge);

	Vertex * unlinkDynamic();

	Vertex * linkDynamic(Edge * e);
	
	Vertex *	checkSourceLoops(Vertex * v);
	Vertex *	checkSourceLoop(Vertex * o,DFSContext & c);
	static int V_DFSDynamicSearch;

	Equivalence * m_Equivalence;

protected:
	virtual void commit(Vertex * v);
	virtual void uncommit(Vertex * v);
	
	int	m_DfsDyn;

public:
	Impedance	m_Cost;
	int		m_Jumps;
	int m_LowPt;
	int m_CompNum;
	int m_CurrentNum;
	
	// members for minimum spanning tree searches...
	int m_mstParent;

	int m_BfsNum;

	int m_Prev;		// Previous vertex on shortest path
	int Scratch;		// Extra variable for use in algorithm
	Graph * m_Graph;
	int m_Rank;		// Unique Vertex number at time of creation

	Edge * m_PrevEdge;
	NodeName m_NodeName;	// Real name
	EdgeList *Adj;		// Adjacent vertices

	// added for circuit simulation
	virtual	double	computeVoltage();
	virtual	double	getVoltage();
//protected:
//	virtual	double	getAdmittance(void);	// sum of node edge admittances
	
protected:
	int setEquivalenceClass		(const int parent);
	int setDynamicClass		(const int parent);
	
protected:
	Impedance m_Dist;		// Cost (after running algorithm)
	int m_DfsNum;
	virtual NodeType theType(void);

protected:	// see later if should be separate class...
	double m_Voltage;
	double m_Time;
	
private:
	long m_Visited;
	long m_search;	// only used to initialize.
	NodeType m_nodeType;
	//ResourceList * m_usingResource;
	int m_ConstantParent;
	int m_DynamicParent;
	friend void debug_it();
	// Disable copy constructor and copy assignment
	Vertex (const Vertex &);
	const Vertex & operator= ( const Vertex & );
	
};


#endif // Vertex_H
