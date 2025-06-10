#include	"Types.h"
//#include	"Graph.h"
#include	"EdgeList.h"
#include	"Impedance.h"
#include	"Set.h"
#include	"Edge.h"
#include <iostream>

EdgeList::EdgeList()
	{
	}

// int
// EdgeList::contains(Edge  * e)
// 	{
// 		Edge * x=e;
// 		Edge *y=e->m_other;
// 		
// 		return  (RWTValSlist<Edge *>::contains(x) || RWTValSlist<Edge *>::contains(y));
// 	}
void  
EdgeList::print(Association * r)
	{
		//EdgeListIterator elit(*this);
		//while(++elit){
		for(const auto& elit: *this) {
 			Edge * e=elit;
			std::cout << "//-Edge " << e->theName() << std::endl;
			if(r){
				std::cout << " in Use by " ;
			} else {
				std::cout << " is free? " ;
			}
			e->listCommitted(r);
			std::cout << std::endl;
		}
	}

// EdgeListIterator::EdgeListIterator( EdgeList &d )
//         :RWTValSlistIterator<Edge *> (d)
//         {
//         }
