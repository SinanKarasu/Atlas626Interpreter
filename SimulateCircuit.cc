#include	"Resource.h"
#include	"TAGContext.h"
#include	"Edge.h"
#include	"Vertex.h"

#include	"Graph.h"
#include	"BFS.h"

#include	"Circuit.h"



int connectedComponent(Vertex * v){
	std::cout << " ^^^^^^^^^^^^^^^ connectedComponent:" << v->theName() << std::endl ;
	return 0;
}


int doKirchoff (Vertex * v){
	//EdgeListIterator P(*(v->Adj));
	for(const auto& P: *(v->Adj)) {
	//while (++P) {
		Edge * y = P;
		std::cout << " ^^^^^^^^^^^^^^^ doKirchoff:" << y->theName() << std::endl ;
	}
	return 0;
}


int isWired (Edge * e){
	//std::cout << " ^^^^^^^^^^^^^^^ isWired " << std::endl ;
	return e->Closed();
}

int isAdmittance (Edge * e)
{
	//std::cout << " ^^^^^^^^^^^^^^^ isAdmittance " << std::endl ;
	return 1;
}


int 
Circuit::SimulateCircuit( Vertex * StartNode)
{
	NodeFunc	connComp	(connectedComponent);
	NodeFunc	Kirchoff	(doKirchoff);
	EdgeFunc	wired		(isWired);
	EdgeFunc	admittance	(isAdmittance);
	
	BFS bfsSearch( &connComp , &Kirchoff , &wired , &admittance );
	
	
	bfsSearch.evalDouble(StartNode);
	
	return 1;
}

