#include	"Resource.h"
#include	"TAGContext.h"
#include	"Edge.h"
#include	"Vertex.h"

#include	"ExecEnv.h"

extern ExecEnv execEnv;

#include<sys/types.h>
#include<time.h>

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>


double timer()
{
// 	double seconds,u_seconds;
// 	struct timeval tv;
// 
// 	gettimeofday(&tv,NULL);
// 
// 	//
// 	// seconds since midnight
// 	//
// 	seconds=tv.tv_sec%(60*60*24);
// 
// 	//
// 	// convert micro seconds to seconds
// 	//
// 	u_seconds=((float)tv.tv_usec*1e-6);
// 
// 	return seconds+u_seconds;


    using namespace std::chrono;
    // Get the current time point
	auto now = std::chrono::system_clock::now();
	// Get the duration since the epoch
	auto duration = now.time_since_epoch();

	// Convert the duration to milliseconds as a double
  	double milliseconds = std::chrono::duration<double, std::milli>(duration).count();
  	return milliseconds/1000.0;
	
}


RWCString  annotate_edge(Edge * e)
{
	RWCString	ret= e->theName() ;
	if(e->Jump()>0){
		ret+= " [Open] " ;
	} else {
		ret+= " >thru< " ;
	}
	return ret;
}


class PathData{
public:
	PathData(Impedance  cost,int pathFound)
		:m_Cost(cost),m_pathFound(pathFound)
		{
		}
	PathData():m_pathFound(0){}
	Impedance  m_Cost;
	int m_pathFound;
};

// Search with cost criteria
PathData
edgeImpedance(Edge * e,DFSContext & c)
{
	Vertex * v=e->Destination(AnyPathMode);
	// we are attempting a path from c.v to c.w
	if(v->committed(c.w)){	// committed to one of us?
		if(execEnv.rejReason()){
			debugtrace << " ACCEPTING (RESOURCE)Committed " << v->theName() << std::endl;
			debugtrace << " FOR DEVICE " << c.w->getResource()->theName() << std::endl;
			v->listCommitted() ;
		}
		return PathData(e->Cost(AnyPathMode),1);
	} else if(v->committed(c.v)){	// or committed to other of us?
		if(execEnv.rejReason()){
			debugtrace << " ACCEPTING (UUT)Committed " << v->theName() << std::endl;
			debugtrace << " FOR DEVICE " << c.v->getResource()->theName() << std::endl;
			v->listCommitted() ;
		}
		return PathData(e->Cost(AnyPathMode),1);
	//} else if(v->dynamicClass()==c.v->dynamicClass()){
	//	if(execEnv.rejReason()){
	//		debugtrace << " ACCEPTING (RESOURCE)W/Reservation " << v->theName() << std::endl;
	//		debugtrace << " FOR DEVICE " << c.v->getResource()->theName() << std::endl;
	//		v->listCommitted() ;
	//	}
	//	return e->Cost(AnyPathMode);
	} else if(v->dynamicClass()==c.w->dynamicClass()){
		if(execEnv.rejReason()){
			debugtrace << " ACCEPTING (RESOURCE)W/Reservation " << v->theName() << std::endl;
			debugtrace << " FOR DEVICE " << c.w->getResource()->theName() << std::endl;
			v->listCommitted() ;
		}
		return PathData(e->Cost(AnyPathMode),1);
	} else if(v->getNodeType()==SourceDevicePortNodeType){
		if(execEnv.edgeTrace()){
			debugtrace << " REJECTED SourcePort " << v->theName() << std::endl;
		}
		return PathData(e->Cost(AnyPathMode)*10.0,1);// prematurely ran into a Source
	} else if(v->getNodeType()==LoadDevicePortNodeType){ 
		if(execEnv.edgeTrace()){
			debugtrace << " REJECTED LoadPort " << v->theName() << std::endl;
		}
		return PathData(e->Cost(AnyPathMode)*20.0,1);
	} else if(v->getNodeType()==PointSourceNodeType){ 
		if(execEnv.edgeTrace()){
			debugtrace << " REJECTED PointSource " << v->theName() << std::endl;
		}
		return PathData(e->Cost(AnyPathMode)*10.0,1);
	} else if(v->committed()){	// Committed to somebody else?
		if(execEnv.rejReason()){
			debugtrace << " REJECTED Committed " << v->theName() << std::endl;
			v->listCommitted() ;
		}
		return PathData(e->Cost(AnyPathMode)*20.0,1);
	}
	return PathData(e->Cost(AnyPathMode),1);
}

PathData
DFSCost( Vertex * v , const Impedance currentCost, DFSContext & c, const int currentHops)
{
	Impedance minCost=c.maxCost+currentCost;
	//int minHops=c.maxHops;
	v->DFSNUM(++Graph::G_DfsSearch);
	v->setDist(currentCost);
	v->m_Jumps=currentHops;
	
	if(execEnv.nodeTrace()){
		std::cerr << " At the node " << v->theName() << std::endl;
		std::cerr << "	Current cost: "<<currentCost << std::endl;
	}
	
	if(currentCost>=c.maxCost){
		return PathData(c.maxCost+1.0,0);
	}

	if(v==c.w){	// TargetNode. We got it.....
		c.actualHops=currentHops;
		c.maxCost=currentCost;
		return PathData(currentCost,1);
	}

	int SourceConnectedComponent=( v->equivalenceClass()==c.v->equivalenceClass() );
	int TargetConnectedComponent=( v->equivalenceClass()==c.w->equivalenceClass() );

	if(SourceConnectedComponent){
		// we are on the "hard" connected component of start
		// so continue on
	} else if(TargetConnectedComponent){
		// we are on the "hard" connected component of target
		// so continue on
	} else if(v->getNodeType()==UutConnectorNodeType){
		// This is an unrelated UUT pin, don't pursue
		return PathData(c.maxCost+1.0,0);
	}

	Impedance cost,zCost;int tryIt;int hop;int pathFound=0;

	//EdgeListIterator P(*(v->Adj));
	for(const auto P: *(v->Adj)) {
	//while(++P){
		Edge * e=P;
		if(execEnv.edgeTrace()){
			std::cerr << " The Edge " << e->theName() << std::endl;
		}
		//cost=e->Cost(AnyPathMode);
		tryIt=1;
		hop = e->Hops();
		if( TargetConnectedComponent ) {
			cost=e->Cost(AnyPathMode);
			tryIt=(hop==0);	// we are in the vicinity, so don't jump non-wires
		} else {
			PathData zCost=edgeImpedance(e,c);
			cost=zCost.m_Cost;
			tryIt=zCost.m_pathFound;
		}
		Vertex * w=e->Destination(AnyPathMode);
		if(!(w->DFSvisited())){	// if this path have not been tried yet
			//if( TargetConnectedComponent ) {
				//tryIt=(hop==0);	// we are in the vicinity, so don't jump non-wires
				//cost=e->Cost(AnyPathMode);
			//} else {
				//PathData zCost=edgeImpedance(e,c);
				//tryIt=zCost.m_pathFound;
			//}
		} else if( (w->Dist() ) > (v->Dist()   + cost )) {	// found but thru a longer path
			tryIt=1;
		} else if( (w->m_Jumps) > (currentHops + hop  )) {
			tryIt=1;
		} else {
			tryIt=0;
		}
		
		if(tryIt){
			if(e->Destination(HardWiredMode)){
				PathData zCost=DFSCost(w,currentCost+cost,c,currentHops);
			
				if( (zCost.m_pathFound) && (zCost.m_Cost <=  minCost) ) {
					w->m_PrevEdge=e->m_other;
					minCost=zCost.m_Cost;
					pathFound=zCost.m_pathFound;
					if(execEnv.edgeTrace()){
						std::cerr << " Accepting edge " << e->theName() << std::endl;
					}
				}
			} else {
				// now we see if this edge is feasible.
				// first we get the current predictor

				Resource * er=e->getResource();
				Set cached=er->getPredictor(Graph::G_DfsSearch);
				er->m_predict &= e->theSet();

				// now check to see if this is a feasible edge.
				// example of a non-feasible edge would be 
				// trying to traverse the NC contact of a DPDT
				// relay AFTER traversing the NO contact.
				// You can't have your cake and eat it too.
				// (Actually I never understood why not! )

			
				if((er->m_predict.firstTrue())!=RW_NPOS){// it is feasible
					PathData zCost=DFSCost(w,currentCost+cost,c,currentHops+hop);
			
					if( (zCost.m_pathFound) && (zCost.m_Cost <=  minCost) ) {
						w->m_PrevEdge=e->m_other;
						minCost=zCost.m_Cost;
						pathFound=zCost.m_pathFound;
						if(execEnv.edgeTrace()){
							std::cerr << " Accepting edge " << e->theName() << std::endl;
						}
					}
				}
				er->m_predict=cached;	// Well , it wants it's money back. Give it BACK!
			}
		}		

	}
	return PathData(minCost,pathFound);
}


// Search with decision criteria
int nodeAllowed(Vertex * v,DFSContext & c)
{
	
	if(v->committed(c.w)){	// committed to one of us?
		if(execEnv.rejReason()){
			debugtrace << " ACCEPTING (RESOURCE)Committed " << v->theName() << std::endl;
			debugtrace << " FOR DEVICE " << c.w->getResource()->theName() << std::endl;
			v->listCommitted() ;
		}
		return 1;	// etc..
	} else if(v->committed(c.v)){	// or committed to other of us?
		if(execEnv.rejReason()){
			debugtrace << " ACCEPTING (UUT)Committed " << v->theName() << std::endl;
			debugtrace << " FOR DEVICE " << c.v->getResource()->theName() << std::endl;
			v->listCommitted() ;
		}
		return 1;	// etc..
	} else if(v->getNodeType()==SourceDevicePortNodeType){
		if(execEnv.edgeTrace()){
			debugtrace << " REJECTED SourcePort " << v->theName() << std::endl;
		}
		return 0;	// prematurely ran into a Source
	} else if(v->getNodeType()==LoadDevicePortNodeType){ 
		if(execEnv.edgeTrace()){
			debugtrace << " REJECTED LoadPort " << v->theName() << std::endl;
		}
		return 0;	// etc..
	} else if(v->getNodeType()==PointSourceNodeType){ 
		if(execEnv.edgeTrace()){
			debugtrace << " REJECTED PointSource " << v->theName() << std::endl;
		}
		return 0;	// etc..
	//} else if(v->dynamicClass()==c.v->dynamicClass()){
	//	if(execEnv.rejReason()){
	//		debugtrace << " ACCEPTING (RESOURCE)W/Reservation " << v->theName() << std::endl;
	//		debugtrace << " FOR DEVICE " << c.v->getResource()->theName() << std::endl;
	//		v->listCommitted() ;
	//	}
	//	return 1;	// etc..
	} else if(v->dynamicClass()==c.w->dynamicClass()){
		if(execEnv.rejReason()){
			debugtrace << " ACCEPTING (RESOURCE)W/Reservation " << v->theName() << std::endl;
			debugtrace << " FOR DEVICE " << c.w->getResource()->theName() << std::endl;
			v->listCommitted() ;
		}
		return 1;	// etc..
	
	} else if(v->committed()){	// Committed to somebody else?
		if(execEnv.rejReason()){
			debugtrace << " REJECTED Committed " << v->theName() << std::endl;
			v->listCommitted() ;
		}
		return 0;
	}
	return 1;
}

PathData
DFSDecision( Vertex * v , const Impedance currentCost, DFSContext & c, const int currentHops)
{
	int minHops=c.maxHops;
	v->DFSNUM(++Graph::G_DfsSearch);
	v->setDist(currentCost);
	v->m_Jumps=currentHops;
	
	
	if(execEnv.nodeTrace()){
		std::cerr << " At the node " << v->theName() << std::endl;
	}


	
	if(currentCost>=c.maxCost){
		return PathData(c.maxCost+1.0,0);
	}

	if(v==c.w){	// TargetNode. We got it.....
		c.actualHops=currentHops;
		c.maxCost=currentCost;
		return PathData(currentCost,1);
	}

	int SourceConnectedComponent=( v->equivalenceClass()==c.v->equivalenceClass() );
	int TargetConnectedComponent=( v->equivalenceClass()==c.w->equivalenceClass() );

	if(SourceConnectedComponent){
		// we are on the "hard" connected component of start
		// so continue on
	} else if(TargetConnectedComponent){
		// we are on the "hard" connected component of target
		// so continue on
	} else if(v->getNodeType()==UutConnectorNodeType){
		// This is an unrelated UUT pin, don't pursue
		return PathData(c.maxCost+1.0,0);
	}

	PathData minCost(c.maxCost+0.1,0);

	//EdgeListIterator P(*(v->Adj));
	Impedance cost;int tryIt;int hop;PathData zCost;
	for(const auto P: *(v->Adj)){
	//while(++P){
		Edge * e=P;
		if(execEnv.edgeTrace()){
			std::cerr << " The Edge " << e->theName() << std::endl;
		}
		cost=e->Cost(AnyPathMode);
		hop = e->Hops();
		Vertex * w=e->Destination(AnyPathMode);
		if(!(w->DFSvisited())){
			if( TargetConnectedComponent ) {
				tryIt=(hop==0);	// we are in the vicinity, so don't jump non-wires
			} else {
				tryIt=nodeAllowed(w,c);
			}
		} else if( (w->Dist() ) > (v->Dist()   + cost )) {
			tryIt=1;
		} else if( (w->m_Jumps) > (currentHops + hop  )) {
			tryIt=1;
		} else {
			tryIt=0;
		}
		
		if(tryIt){
			if(e->Destination(HardWiredMode)){
				zCost=DFSDecision(w,currentCost+cost,c,currentHops);
			
				if( (zCost.m_pathFound) && (zCost.m_Cost <=  minCost.m_Cost) ) {
					w->m_PrevEdge=e->m_other;
					minCost=zCost;
					if(execEnv.edgeTrace()){
						std::cerr << " Accepting edge " << e->theName() << std::endl;
					}
				}
			} else {
				// now we see if this edge is feasible.
				// first we get the current predictor

				Resource * er=e->getResource();

				Set cached=er->getPredictor(Graph::G_DfsSearch);
				er->m_predict &= e->theSet();

				// now check to see if this is a feasible edge.
				// example of a non-feasible edge would be 
				// trying to traverse the NC contact of a DPDT
				// relay AFTER traversing the NO contact.
				// You can't have your cake and eat it too.
				// (Actually I never understood why not! )

			

				if((er->m_predict.firstTrue())!=RW_NPOS){// it is feasible
					PathData zCost=DFSDecision(w,currentCost+cost,c,currentHops+hop);
			
					if( (zCost.m_pathFound) && (zCost.m_Cost <=  minCost.m_Cost) ) {
						w->m_PrevEdge=e->m_other;
						minCost=zCost;
						if(execEnv.edgeTrace()){
							std::cerr << " Accepting edge " << e->theName() << std::endl;
						}
					}
				}
				er->m_predict=cached;	// Well , it wants it's money back. Give it BACK!
			}
		}		

	}
	
	return minCost;
}

int 
Graph::FindPath( Vertex* StartNode ,Vertex* TargetNode , EdgeList & edgeList  )

{
	int currentHops=0;
	
	int maxHops=0;
	
	double maxCost=0.0;PathData found;
	
	Initialize( );	// This should be only done once

	ClearData();

	//DFSContext c(StartNode,TargetNode,4,0.040);	// 4 jumps or 400 wires
	DFSContext c(StartNode,TargetNode,maxHops,maxCost);

	double starttime=timer();
	
	if(execEnv.searchAlgorithm()==0){
		for(maxHops=2,maxCost=0.020; (maxHops<8) || (maxCost<0.2);maxHops+=2){
			maxCost*=2.0;
			c.init(StartNode,TargetNode,maxHops,maxCost);
			found=DFSDecision(c.v,0,c,0);
			if(found.m_Cost<=maxCost){
				break;
			}
		}
	} else {
		for(maxHops=2,maxCost=0.020; (maxHops<8) || (maxCost<0.2);maxHops+=2){
			maxCost*=2.0;
			c.init(StartNode,TargetNode,maxHops,maxCost);
			found=DFSCost(c.v,0,c,0);
			if( found.m_pathFound ){
				break;
			}
		}
	}

	
	double endtime=timer();
	
	if(double(found.m_Cost)>maxCost){
		std::cerr	<< " Searching for path from:" ;
		std::cerr	<< StartNode->theName() ;
		std::cerr	<< " To:";
		std::cerr	<< TargetNode->theName() ;
		//std::cerr	<< std::endl;
		
		std::cerr	<< " -- ";
			
		std::cerr << " PATH NOT Found" 	<< std::endl;
		std::cerr << " Elapsed Time " 	<< endtime-starttime << " Secs " << std::endl;
		std::cerr <<  std::endl;
		std::cerr << "Cost:" << found.m_Cost << " Max allowed:" << maxCost << std::endl;
		
		std::cerr << " ABORTING Search"	<< std::endl;
		return -1;
	}
	extern int s1,s2;
	if(execEnv.statistics()){

		debugtrace 	<< " Found after " << c.actualHops 	<< " Hops From: "
				<< StartNode->theName() 		<< " To: "
				<< TargetNode->theName()
				<< std::endl;
				
		debugtrace << " Elapsed Time " << endtime-starttime << " Secs " << std::endl;

	}
	
	Vertex * x=TargetNode;

	int printcount=0;
	while(x){
		if(x==StartNode){
			if(execEnv.showPath()){
				if(printcount==0){
					debugtrace << "// " ;
				}
				debugtrace << "..." ;
				debugtrace << x->theName();
				debugtrace << std::endl;
				printcount=0;
			}
			x=NULL;
		} else {
			Edge * e=x->m_PrevEdge;
			if(execEnv.showPath()){
				if(x==TargetNode){
					debugtrace << "// From " ;
					debugtrace << x->theName();
					//debugtrace << "..." ;
					printcount++;
				} else if(e->Hops() > 0){
					if(printcount==0){
						debugtrace << "// " ;
					}
					//if(e->Jump()>0){
						//debugtrace << "\tClose\t" ;
					//} else {
						//debugtrace << "\tthru\t" ;
					//}
					debugtrace << "..." ;
					debugtrace << e->theName() ;
					printcount++;
				}
			}
			edgeList.insert(e);
			x=e->Dest;
		}
		if(printcount>2){
			debugtrace << std::endl;
			printcount=0;
		}
	}

	
	return c.actualHops;
	
}


