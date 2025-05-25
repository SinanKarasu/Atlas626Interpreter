#ifndef Wire_H
#define Wire_H

class Wire : public Edge{
public:
	Wire(Resource * r,Vertex * S,Vertex * D,Impedance z=Impedance(0.0001));
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


#endif // Wire_H
