#ifndef Graph_H
#define Graph_H

#include	"Std.h"

//#include	"Queue.h"
//#include	"Heap.h"

//#include	"Pair.h"


#include	"Impedance.h"
#include	"Set.h"
//#include	"Vertex.h"
#include	"VertexList.h"
#include	"Edge.h"
#include	"EdgeList.h"
#include	"Equivalence.h"

extern std::ofstream debugtrace;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//class Resource;
//class Vertex;
//class Edge;
//class EdgeList;
//class Equivalence;
//class EquivalenceDictionary;
//class TAGContext;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

unsigned IntegerHash(const RWInteger& i);

enum NodeType
	{
			UndefinedNodeType
		,	UutConnectorNodeType
		,	InterfaceConnectorNodeType
		,	SourceDevicePortNodeType
		,	AdapterConnectorNodeType
		,	PointSourceNodeType
		,	SensorDevicePortNodeType
		,	LoadDevicePortNodeType
		,	DeviceReferencePortNodeType
		,	PathNodeType
		,	DevicePathType
		,	SwitchContactNodeType
		,	BusNodeType
	};




// Basic item in the adjacency list




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "Association.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Graph;


class Graph:public	GraphContext{
public:
	Graph ( Impedance Inf );

	static Vertex * vertex(int rank);

	void Build ( Resource * r ,  Resource * source );
	virtual ~Graph( );

	void AddEdge( Vertex * Source,Vertex * Dest,Edge * edge );
	const Impedance & Inf();
	Vertex * AddNode( Vertex* vertex );
	int FindPath( Vertex* StartNode,Vertex* TargetNode , EdgeList & edgeList );
	//int FindPathDFS ( int StartNode,int TargetNode , EdgeList & edgeList );
	//int FindPathDFS1( int StartNode,int TargetNode , EdgeList & edgeList );
	//int FindPathBFS( int StartNode,int TargetNode , EdgeList & edgeList );
	void Initialize();
	int TagCritical	(Vertex * here,Edge * to_here,TAGContext & lookAhead );
	static int numVertices();

	void linkDynamic(Edge * e);
	void unlinkDynamic(Edge * e);
	void unlinkDynamic(Vertex * v);


	void ClearData( );      // Initialize the table


private:
	// Disable copy constructor and copy assignment
	Graph(const Graph &);
	const Graph & operator= ( const Graph & );

	const Impedance Infinity;			// For initialization

	// Add an edge given internal numbers of its vertices
	void AddInternalEdge(
				Vertex * Source,
				Vertex * Dest,
				Edge * edge
			);


	VertexList  m_VertexList;

	void PrintPathRec( int DestNode ) const;
	void PrintPath( int DestNode ) const;
	
	void DumpTable();
};


#endif // Graph_H
