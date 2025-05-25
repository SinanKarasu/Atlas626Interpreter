#ifndef CapacitiveEdge_H
#define CapacitiveEdge_H

#include	"TwoTerm.h"

class CapacitiveEdge : public TwoTerm {
public:
	CapacitiveEdge (Resource * r,Vertex * S,Vertex * D,Impedance z=Impedance(0.0001));
	virtual Edge * clone(Resource * resource);
	virtual int getState();
	virtual Set & theSet();
	virtual RWCString theName();
	virtual EdgeType theType(void);
private:
	// there should be only one....fix this....
	Set m_energizer;
	static const EdgeType m_theType;
};


#endif // CapacitiveEdge_H
