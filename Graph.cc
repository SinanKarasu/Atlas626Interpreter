#include	"Resource.h"
#include	"Equivalence.h"
#include	"TAGContext.h"
#include	"Edge.h"
#include	"EdgeList.h"
#include	"Wire.h"
#include	"TwoTerm.h"
#include	"Contact.h"
#include	"Vertex.h"
#include	"VertexList.h"
#include	"Circuit.h"
#include	"NodeName.h"

#include	"ExecEnv.h"
extern ExecEnv execEnv;

AssociationListStack theAssociationListStack;


//EquivalenceDictionary Graph::equivalenceDictionary;

unsigned IntegerHash(const RWInteger& i) { return i ; }


/////////////////////////////////////////////////////////////////////////////

// Clone method

void Graph::Build(Resource * r, Resource  * source )
{
	Graph * g=source->circuit();
	
	//VertexListIterator vit(g->m_VertexList);
	for(const auto vit: g->m_VertexList) {
	//while(++vit){
		Vertex * vertex = vit;
		r->AddNode(vertex->getName(),vertex->getNodeType());	// This is necessary for isolated nodes..
									// Such as DVM HI etc....
		//EdgeListIterator P(*(vertex->Adj));
		for(const auto P: *(vertex->Adj)) {
		//while( ++P){
			Edge * e=P;
			if(e->getResource()==source){
				Resource * rSource	=e->Sorc->getResource();
				Resource * rDest	=e->Dest->getResource();
				if(rSource==source && rDest==source){
					if(e->m_forward){
						// if Source AND Dest are on this device
						// then we will see this edge twice.
						// so, only add the forward one.
						RWCString fName=e->Sorc->getName();
						RWCString tName=e->Dest->getName();
						r->AddEdge(fName,tName,e);
					}
				} else {
					RWCString fName=e->Sorc->getName();
					RWCString tName=e->Dest->getName();
					r->AddEdge(fName,tName,e);
				}
			}
		}
	}

}


void
Graph::AddEdge(	Vertex * Source,Vertex * Dest,Edge * edge)
{
	AddInternalEdge( Source , Dest , edge );
}


// Assume that Result is newly created.
// Put it into the proper place in Table 
// and set up adj. list.....

Vertex * 
Graph::AddNode( Vertex * Result )
{

	Result->m_Rank = NumVertices;
	Result->m_Graph=this;
	Result->Adj= new EdgeList;
		
	Table.append( Result );		// insert at NumVertices for the global table
	m_VertexList.insert(Result);	// and into the Graph local list
	return Table[NumVertices++];

}

// Add the edge ( Source, Dest, Cost ) to the graph
// Source and Dest are internal vertex numbers


void
Graph::AddInternalEdge(Vertex * Source,Vertex * Dest,Edge * edge)
{
	//EdgeList * P = Table[ Source->m_Rank ]->Adj;
	edge->Sorc=Source;
	edge->Dest=Dest;
	Source->insertEdge(edge);
	//P->insert( edge );
}


// Driver routine to handle unreachables and print total cost
// It calls recursive routine to print shortest path to
// DestNode after a hortest path algorithm has run

void
Graph::PrintPath( int DestNode ) const
{
	if( Table[ DestNode ]->Dist() == Infinity ) {
		std::cout << Table[ DestNode ]->getName() << " is unreachable";
	} else {
		PrintPathRec( DestNode );
		std::cout << " (cost is " << Table[ DestNode ]->Dist() << ")";
	}
	std::cout << std::endl;
}


// Recursive routine to print shortest path to DestNode
// after running shortest path algorithm

void
Graph::PrintPathRec( int DestNode ) const
{
	if( Table[ DestNode ]->m_Prev != NullVertex ) {
		PrintPathRec( Table[ DestNode ]->m_Prev );
		std::cout << " to ";
	}
	std::cout << Table[ DestNode ]->getName();
}


        
Graph::Graph( Impedance Inf):Infinity( Inf )
{
}

Graph::~Graph( )
{
	for( int i = 0; i < NumVertices; i++ ){
		delete Table[ i ];
	}
}


Vertex *
Graph::vertex(int rank)
	{
		return Table[rank];
	}

const Impedance &
Graph::Inf(){return Infinity;}

void
Graph::unlinkDynamic(Edge * e)
	{
		e->Sorc->unlinkDynamic();
		e->Dest->unlinkDynamic();
	}
		
void
Graph::unlinkDynamic(Vertex * v)
	{
		//equivalenceDictionary.unlinkDynamic(v);
		Equivalence * veq=v->getEquivalence();
		veq->unlinkDynamic();

	}
		

void
Graph::linkDynamic(Edge * e)
	{
		Equivalence * veq=e->Sorc->getEquivalence();
		Equivalence * weq=e->Dest->getEquivalence();
		veq->linkDynamic(weq);
		weq->linkDynamic(veq);// redundant and not needed...

	}


void
AllWires(Vertex * v ,DFSContext & c)
{
	std::cout	<< "\t" << "(" << v->m_Rank << "," << v->equivalenceClass() << ")"
		<< v->theName() << std::endl;
	v->DFSNUM(c.count1=++Graph::G_DfsSearch);
	//EdgeListIterator P(*(v->Adj));
	for(const auto P : *(v->Adj)){
	//while(++P){
		Vertex * w;
		if(w=P->Destination(HardWiredMode)){
			assert(v->equivalenceClass()==w->equivalenceClass());
			if(!(w->DFSvisited())){
				AllWires(w,c);
			}
		}
	}
}


void Graph::DumpTable()
{
	int i;
	
	for( i = 0; i < Graph::numVertices(); i++ ){
		//EdgeListIterator P(*(Graph::vertex( i )->Adj));
		std::cout	<< i
			<< "("
			<< Graph::vertex( i )->equivalenceClass()
			<< ","
			<< Graph::vertex( i )->dynamicClass()
			<< ")"
			<< ":" << Graph::vertex( i )->theName()
			<< " >>>" ;
		int count=0;
		for(const auto P: *(Graph::vertex( i )->Adj)) {
		//while(++P ){
			Edge * e=P;
			count++;
			std::cout << e->theName();
			if(count % 5 == 0){
				std::cout << std::endl;
				std::cout << "          ";
			}
		}
		std::cout << std::endl;
	}
	
	
	ClearData( );

	DFSContext c(0,0,0);
	std::cout << " WIRE CONNECTIVITY " << std::endl;
	for( i = 0; i < NumVertices; i++ ) {
		Vertex * W=Table[ i ];
		if( !W->DFSvisited() ){
				std::cout << "(" << W->m_Rank << "," << W->equivalenceClass() << ")" << W->theName() << std::endl;
				AllWires(W,c);	
		}
	}
}


// Initialize the graph table prior to running
// any shortest path algorithm

void
Graph::ClearData( )
{
	// Ok, the way this works is that each node checks it's saved
	// search number against the G_CurrentSearch. If it is smaller, 
	// they clear themselves first time they are accessed after this call.
	// This saves us iterating thru thousands of nodes every time....

	G_CurrentSearch++;
}




void
EquivClasses(Vertex * v ,DFSContext & c)
{
	// This routine sets the Hard Connected Components
	v->DFSNUM(c.count1=++Graph::G_DfsSearch);
	v->LOWPT(v->DFSNUM());
	//EdgeListIterator P(*(v->Adj));
	for(const auto P: *(v->Adj)) {
	//while(++P){
		Vertex * w;
		if(w=P->Destination(HardWiredMode)){	// Is this a hard wire?
			if(!(w->DFSvisited())){
				EquivClasses(w,c);
				w->CURRENT(1);
				c.CURRENTStackPush(w);
				if(w->LOWPT()<v->LOWPT()){
					v->LOWPT(w->LOWPT());
				}
				if((w->DFSNUM() < v->DFSNUM()) && w->CURRENT()){
					while(!c.CURRENTStackEmpty()){
						if(c.CURRENTStackTop()->DFSNUM() >= v->DFSNUM()){
							c.CURRENTStackPop()->CURRENT(0);
						} else {
							break;
						}
					}
				}
			}
		}
	}
	v->COMPNUM(++c.count2);
}


void
AllResources(Vertex * v ,DFSContext & c)
{
	v->DFSNUM(c.count1=++Graph::G_DfsSearch);
	v->commitEquivalence();
	//EdgeListIterator P(*(v->Adj));
	for(const auto P: *(v->Adj)) {
	//while(++P){
		Vertex * w;
		if(w=P->Destination(HardWiredMode)){
			if(!(w->DFSvisited())){
				AllResources(w,c);
			}
		}
	}
}


//
void
checkResource(Resource * r)
{

}
// This is a consistency routine..It checks for the consistency of 
// equivalence classes.
void
AllEq(Vertex * v ,DFSContext & c)
{
	v->DFSNUM(c.count1=++Graph::G_DfsSearch);
	//EdgeListIterator P(*(v->Adj));
	for(const auto P: *(v->Adj)) {
	//while(++P){
		Vertex * w;
		if(w=P->Destination(HardWiredMode)){
			assert(v->equivalenceClass()==w->equivalenceClass());
			if(!(w->DFSvisited())){
				AllEq(w,c);
			}
		}
	}
}


int
Graph::numVertices(){return NumVertices;}


void
Graph::Initialize( )
{
	int V, W;

	if(!G_Initialized){

		
		ClearData( );


		DFSContext c(0,0,0);
	
		for( V = 0; V < NumVertices; V++ ) {
			Vertex * W=Table[ V ];
			if( !W->DFSvisited() ){
				EquivClasses(W,c);	
			}
		}
		
		ClearData( );
		
		DFSContext t(0,0,0);	// Needed to reset the search
	
		TAGContext lookAhead;
		
		for( V = 0; V < NumVertices; V++ ) {
			Vertex * W=Table[ V ];
			if( !W->DFSvisited() ){
				TagCritical(W,0,lookAhead);
			}
		}

		ClearData( );

		c.init(0,0,0);

		for( V = 0; V < NumVertices; V++ ) {
			Vertex * W=Table[ V ];
			if( !W->DFSvisited() ){
				AllResources(W,c);
			}
		}
		
		ClearData( );

		c.init(0,0,0);

		for( V = 0; V < NumVertices; V++ ) {
			Vertex * W=Table[ V ];
			if( !W->DFSvisited() ){
				AllEq(W,c);
			}
		}
		
		
		// now traverse thru all the entries in the resourceList and
		// make sure that no Source cycles exist.....
		// Here is a little exposition on the subject..

////#define _CHECKSOURCES_
#ifdef _CHECKSOURCES_
		
		ClearData( );
		c.init(0,0,0);

		for( V = 0; V < NumVertices; V++ ) {
			Vertex * W=Table[ V ];
			if( !W->DFSvisited() ){
				if(W->isSource()){
					int s=W->getEquivalence()->getSourceCount();
					if(s>0){
						std::cout << "Checking " << W->theName() << std::endl;
						if(W->theName()=="ATE:PP-T34"){
							int ijkl=1234;
						}
						Vertex * v=W->checkSourceLoops(W);
						if(v->equivalenceClass()==W->equivalenceClass()){
							cerr	<< "Problem: "
								<< W->theName()
								<< " Source LOOPS to:"
								<< v->theName()
								<< std::endl ;
						}
					}
				}
			}
		}
				
#endif		
		if(execEnv.dumpCircuit()){
			ClearData( );DumpTable( );
		}
		

		G_Initialized=1;
	}
}




int Graph::TagCritical(Vertex * here, Edge * to_here , TAGContext & lookAhead )
	{
		int now,min,nl;TAGContext localLookAhead(here,0);
					
      		here->DFSNUM(++G_DfsSearch);

      		nl=now=min=here->DFSNUM();
      		
		//EdgeListIterator P(*(here->Adj));
		for(const auto P: *(here->Adj)) {
		//while (++P) {
			Vertex * there;
			if (there=P->Destination(CurrentMode)){	//traversable path ?
				if( !there->DFSvisited()){	// not yet visited
					nl=TagCritical(there,P->m_other,localLookAhead);//see if we loop back
					P->setIsthmus(nl>now);
					P->setLookAhead(localLookAhead);
					if (nl<min) {min=nl;}		//remember lowest node seen
				} else if (P==to_here) {
					//this is how we came here, so ignore. Allows multiple wires.
				} else {	// Already visited so must be a cycle
					P->setIsthmus(0);
					TAGContext temp(P->Dest);
					P->setLookAhead(temp);
					if(there->DFSNUM() < min){
						min=there->DFSNUM();
					}
				}
			}
		}
		if(to_here){to_here->setIsthmus(min>=now);}
		lookAhead.tagUpdate(localLookAhead);
		return min;
	}          

