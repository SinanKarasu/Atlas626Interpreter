#ifndef Contact_H
#define Contact_H

#include "Edge.h"

class Contact : public Edge{
public:
	Contact	(
			Resource * resource,	int state,	Vertex * S,Vertex * D,
			Impedance cz=Impedance(0.010),		Impedance oz=Impedance(c_Inf)
		);

	virtual Edge * clone(Resource * resource);
	virtual Vertex * Destination(const DestinationMode mode);
	virtual const int Closed(const int closed=c_Und);
	virtual void setCondition(const int pos);
	virtual Set & theSet();
	virtual int Jump() ;
	virtual int Hops() ;
	virtual const Impedance & Cost(DestinationMode mode=CurrentMode) ;
	virtual RWCString theName();

	virtual	void		insertThyself(EdgeList * edgeList)		;

	virtual void	commit(Association * r);
	virtual void	uncommit(Association * r);
	virtual void	invalidateDynamicClass()			;
	virtual void	calculateDynamicClass();
	virtual EdgeType theType(void);

private:
	Set m_energizer;
	const Impedance m_Open;
	static const EdgeType m_theType;
};

#endif // Contact_H
