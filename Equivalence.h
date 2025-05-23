#ifndef Equivalence_H
#define Equivalence_H

#include	"ResourceList.h"
#include	"VertexList.h"
#include	"EdgeList.h"

class Equivalence{
public:
	Equivalence(int rank);
	Equivalence * linkDynamic(Equivalence * vEq);
	Equivalence * unlinkDynamic();

	//Equivalence * linkDynamic(Edge * e);
	//Equivalence * unlinkDynamic(Edge * e);


	virtual int committed(Vertex * v=0, Equivalence * last=0);
	virtual int onlycommitted(Vertex * v, Equivalence * last=0);
	virtual void listCommitted(Vertex * v, Equivalence * last=0);
	virtual void commit(Vertex * v);

	int getSourceCount(Resource * r=0);
	
	//int dynamicEquivalence();
	//int dynamicClass();

private:
	int getDynamicClass(int current);
	ResourceList * m_usingResources;
	VertexList * m_usingDeviceNodes;
	//EdgeList * m_linkingEdges;
	int m_Rank;
	int m_SourceCount;
	Equivalence * m_fwd;
	Equivalence * m_back;
	//long	m_EqSearchNum;
	//static long G_EqSearchNum;
	void init();
};

//class EquivalenceDictionary : public RWTValHashDictionary<RWInteger,Equivalence *>{
//	public:
//		EquivalenceDictionary();
//		//blockInserts();
//		Equivalence * getEquivalence(int rank);
//		//void linkDynamic(Vertex * v,Vertex * w); 
//		//void unlinkDynamic(Vertex * v,Vertex * w); 
//
//		void linkDynamic(Edge * e); 
//		void unlinkDynamic(Vertex * e); 
//
//	private:
//		enum { NbrBuckets = RWDEFAULT_CAPACITY };
//};



#endif // Equivalence_H
