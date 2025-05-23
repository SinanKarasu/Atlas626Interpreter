#include	"Resource.h"
#include	"TAGContext.h"
#include	"Edge.h"
#include	"Vertex.h"

#include	"Graph.h"
#include	"BFS.h"

#include	"Circuit.h"



int connectedComponent(Vertex * v){
	cout << " ^^^^^^^^^^^^^^^ connectedComponent:" << v->theName() << endl ;
	return 0;
}


int doKirchoff (Vertex * v){
	EdgeListIterator P(*(v->Adj));
	while (++P) {
		Edge * y = P.key();
		cout << " ^^^^^^^^^^^^^^^ doKirchoff:" << y->theName() << endl ;
	}
	return 0;
}


int isWired (Edge * e){
	//cout << " ^^^^^^^^^^^^^^^ isWired " << endl ;
	return e->Closed();
}

int isAdmittance (Edge * e)
{
	//cout << " ^^^^^^^^^^^^^^^ isAdmittance " << endl ;
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

