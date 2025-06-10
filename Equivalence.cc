#include	"Resource.h"
#include	"Equivalence.h"
#include	"ResourceList.h"
#include	"Vertex.h"
#include	"VertexList.h"
#include	"BFS.h"

ResourceListStack allResourceLists;
VertexListStack allVertexLists;

//long Equivalence::G_EqSearchNum=0;


int  G_Equivalence_insantiated=0;
////////////////////////////////////////////////////////////
//
//	x		y
//	-----		-----
//	m_fwd	-->	y
//	x	<--	m_back
//
//	in other words we have a doubly linked circular list
//
/////////////////////////////////////////////////////////////


Equivalence::Equivalence(int rank)
	:m_Rank			(rank)
	,m_usingDeviceNodes	(0)
	,m_SourceCount		(0)
	{
		// an empty  list is indicated
		// by a self referencing list
		init();
		G_Equivalence_insantiated++;
	}


void
Equivalence::init()
{
	m_fwd=m_back=this;
}

Equivalence *
Equivalence::linkDynamic(Equivalence  * y)
	{
		// assume x and y the Equivalences to be linked.
		// we swap
		// x->m_fwd <==> (y->m_fwd)->m_back
		// y->m_fwd <==> (x->m_fwd)->m_back
		//if(!m_fwd){
		//	m_fwd=m_back=this;
		//}
		Equivalence * t,*x=this;
		
		//swap( (y->m_fwd)->m_back , x->m_fwd );
		t=(y->m_fwd)->m_back;	(y->m_fwd)->m_back=x->m_fwd;	x->m_fwd=t;

		//swap( (x->m_fwd)->m_back , y->m_fwd );
		t=(x->m_fwd)->m_back;	(x->m_fwd)->m_back=y->m_fwd;	y->m_fwd=t;

		return this;
	}


Equivalence *
Equivalence::unlinkDynamic()
	{
		//Equivalence * vEq=0;
		//if(v){
		//	vEq=v->equivalenceClass();
		//}

		Equivalence * t=m_fwd;
		while((t!=this)){
			Equivalence * e=t;
			t=t->m_fwd;
			e->init();
		}
		init();
		return this;
	}

int
Equivalence::committed(Vertex * v,Equivalence * last)
	{
		if(m_usingDeviceNodes){
			if(v==0){
				return 1;
			} else if(m_usingDeviceNodes->contains(v)){
				return 1;
			} else {
				if( last && m_fwd != last ){
					return m_fwd->committed(v,last);
				}
			}
		} else if( last && m_fwd != last ){
			return m_fwd->committed(v,last);
		}
		return 0;
	}



int
Equivalence::onlycommitted(Vertex * v,Equivalence * last)
	{
		assert(v);
		if(m_usingDeviceNodes){
			if(committed(v)&&(m_usingDeviceNodes->size()==1)){
				if( last && m_fwd != last ){
					return (m_fwd->committed(0,last)==0); // search for any commitment
				} else {
					return 1;
				}
			}
		} else if( last && m_fwd != last ){
			return m_fwd->onlycommitted(v,last);
		}
		return 0;
	}

void
Equivalence::commit(Vertex * v)
	{
		// commit only does static equivalence class committing.
		// as a matter of fact it should only be called from
		// tedl, and not from atlas......
		// We should put a disable switch in it....
		assert(v);
		if(!m_usingDeviceNodes){
			m_usingDeviceNodes=allVertexLists.getOne();
		}
		if(!(m_usingDeviceNodes->contains(v))){
			if(v->isSource()){
				m_SourceCount++;
			}
			m_usingDeviceNodes->append(v);
		}
	}
	
int
Equivalence::getSourceCount(Resource * r)
	{
		return m_SourceCount;
	}

void
Equivalence::listCommitted(Vertex * v,Equivalence * last)
	{
		if(m_usingDeviceNodes){
			//VertexListIterator vlit(*m_usingDeviceNodes);
			for(const auto& vlit: *m_usingDeviceNodes) {
			//while(++vlit){
				std::cout << "//--" << vlit->theName();
				if(v && v==vlit){
					std::cout << "<--- Requesting Vertex!! ";
				}
				std::cout << std::endl;
			}
		}
		if( last && m_fwd != last ){
			m_fwd->listCommitted(v,last);
		}
	}

//EquivalenceDictionary::EquivalenceDictionary():RWTValHashDictionary<RWInteger,Equivalence *>(IntegerHash)
//	{resize(NbrBuckets);}

//Equivalence *
//EquivalenceDictionary::getEquivalence(int rank)
//	{
//		Equivalence * e=0;
//		findValue(rank,e);
//		if(!e){
//			insertKeyAndValue(rank,e=new Equivalence(rank));
//		}
//		return e;
//	}


//void
//EquivalenceDictionary::linkDynamic(Edge * e)
//	{
//		Equivalence * veq=getEquivalence(e->Sorc->m_Rank);
//		Equivalence * weq=getEquivalence(e->Dest->m_Rank);
//		veq->linkDynamic(weq);
//		weq->linkDynamic(veq);// redundant and not needed...
//	}

//void
//EquivalenceDictionary::unlinkDynamic(Vertex * v)
//	{
//		Equivalence * veq=getEquivalence(v->m_Rank);
//		veq->unlinkDynamic();
//	}
//
