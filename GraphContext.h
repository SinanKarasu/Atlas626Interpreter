#ifndef GraphContext_H
#define GraphContext_H


class Vertex;

class GraphContext
{
public:
	static long const NullVertex;	//=-1;
	static long const Undefined;		//=-1;

	static long G_CurrentSearch;
	static long G_DfsSearch;
	static long G_DfsSearchStart;
	static long G_Initialized;



	static RWTValOrderedVector< Vertex * > Table;	// The table array

	static int NumVertices;				// Current # vertices created
};


#endif // GraphContext_H
