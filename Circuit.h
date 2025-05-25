#ifndef Circuit_H
#define Circuit_H


class Circuit : virtual public Graph{
public:
	Circuit(Impedance Inf,Resource * resource=0);
		
	Circuit(Resource *r, Circuit * c);
	virtual ~Circuit();

	virtual	int	SimulateCircuit( Vertex * StartNode);

protected:
	Resource * m_Resource;
};


#endif	// Circuit_H
