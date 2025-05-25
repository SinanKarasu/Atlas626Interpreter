#ifndef ResistiveEdge_H
#define ResistiveEdge_H

#include	"TwoTerm.h"

class ResistiveEdge : public TwoTerm {
public:
	ResistiveEdge (Resource * r,Vertex * S,Vertex * D,Impedance z=Impedance(0.0001));
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


#endif // ResistiveEdge_H
