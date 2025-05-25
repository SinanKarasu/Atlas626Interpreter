#ifndef TwoTerm_H
#define TwoTerm_H

class TwoTerm : public Edge{
public:
	virtual Edge * clone(Resource * resource);
	virtual int getState();
	virtual Set & theSet();
	virtual RWCString theName();
	virtual EdgeType theType(void);
protected:
	TwoTerm (Resource * r,Vertex * S,Vertex * D,Impedance z=Impedance(0.0001));

private:
	// there should be only one....fix this....
	Set m_energizer;
	static const EdgeType m_theType;
};


#endif // TwoTerm_H
