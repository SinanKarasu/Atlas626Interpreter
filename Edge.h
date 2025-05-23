#ifndef Edge_H
#define Edge_H

#include	"DestinationMode.h"
#include	"TAGContext.h"
#include	"Association.h"
//class Vertex;
class Association;
//class TAGContext;
//class Impedance;

enum EdgeType
	{
		UndefinedEdgeType ,
		WireType ,
		TwoTermType ,
		ResistiveEdgeType ,
		CapacitiveEdgeType ,
		InductiveEdgeType ,
		ContactType
	};


class Edge
{
public:
	Vertex * Sorc;		// First vertex in edge
	Vertex * Dest;		// Second vertex in edge

	Edge(Vertex * S,Vertex * D, Impedance C  );
	
	virtual Edge *		clone		(Resource * resource)	=0	;
	virtual int		committed	(Association * r	=0)	;
	virtual int		onlycommitted	(Association * r)		;
	virtual void		commit		(Association * r)		;
	virtual void		uncommit	(Association * r)		;
	virtual void		listCommitted	(Association * r)		;
	virtual void		invalidateDynamicClass()			;
	virtual void		calculateDynamicClass()				;
	
	virtual	void		insertThyself(EdgeList * edgeList)		;

	virtual const	Impedance & 
				Cost	(DestinationMode mode=CurrentMode) 	;
	virtual	const int	Closed(const int closed=c_Und)			;
	virtual	Vertex *	Destination(const DestinationMode mode)		;
	virtual	void 		setCondition(const int pos)			;
	virtual	Set & 		theSet()	=0				;
	virtual	RWCString	theName()	=0				;
	void			setResource(Resource * resource)		;
	Resource *		getResource()	const				;
	virtual	int		Jump()						;
	virtual	int		Hops()						;
	virtual EdgeType 	theType()					;

	// added for circuit simulation
	virtual	double		getVoltage(void);
	virtual	double		getCurrent(void);
	virtual	double		getAdmittance(void);

public:
	int m_state;
	int m_forward;
	Edge * m_other;
	long m_search;

public:
	void 			setIsthmus(const int crit)			;
	void 			setLookAhead(const TAGContext c)		;
	int			getIsthmus(const int crit) const		;
	TAGContext &		getLookAhead()					;

protected:
	int m_isthmus;
	TAGContext m_lookAhead;
	Resource * m_Resource;
	const Impedance m_Cost;
	AssociationList * m_usingAssociationList;

	class Graph;
	friend Graph;
	
protected:
	double m_Current;
	
private:
	// Disable copy constructor and copy assignment
	Edge (const Edge &);
	const Edge & operator= ( const Edge & );
};

#endif // Edge_H
