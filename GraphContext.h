#ifndef GraphContext_H
#define GraphContext_H


class Vertex;

class GraphContext
{
public:
	static inline long const NullVertex = -1;	//=-1;
	static inline long const Undefined = -1;		//=-1;

	static inline long G_CurrentSearch;
	static inline long G_DfsSearch;
	static inline long G_DfsSearchStart;
	static inline long G_Initialized;



	//static inline RWTValOrderedVector< Vertex * > Table;	// The table array
	
	static inline AppendableVector<Vertex*> Table;
	static inline int NumVertices;				// Current # vertices created
};


#endif // GraphContext_H
