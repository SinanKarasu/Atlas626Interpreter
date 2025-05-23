#include	"Types.h"
//#include	"Graph.h"
#include	"EdgeList.h"
#include	"Impedance.h"
#include	"Set.h"
#include	"Edge.h"


EdgeList::EdgeList()
	:RWTValSlist<Edge *>()
	{
	}

int
EdgeList::contains(Edge  * e)
	{
		Edge * x=e;
		Edge *y=e->m_other;
		
		return  (RWTValSlist<Edge *>::contains(x) || RWTValSlist<Edge *>::contains(y));
	}
void  
EdgeList::print(Association * r)
	{
		EdgeListIterator elit(*this);
		while(++elit){
 			Edge * e=elit.key();
			cout << "//-Edge " << e->theName() << endl;
			if(r){
				cout << " in Use by " ;
			} else {
				cout << " is free? " ;
			}
			e->listCommitted(r);
			cout << endl;
		}
	}

EdgeListIterator::EdgeListIterator( EdgeList &d )
        :RWTValSlistIterator<Edge *> (d)
        {
        }
