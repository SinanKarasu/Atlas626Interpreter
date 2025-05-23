#ifndef SwitchModel_h
#define SwitchModel_h

#include	"Resource.h"
#include	"ASTVector.h"
#include	"StringVector.h"

class SwitchModel : public Resource{
public:
	SwitchModel( Resource * previous, RWCString & name );
	SwitchModel( Resource * previous, RWCString & newName, SwitchModel * source );

	virtual Resource * clone(Resource * previous,RWCString & newName);

	virtual void AddEdge(const RWCString & from,const RWCString & to,Edge * edge);
	virtual int setState(int state, ReverseMap * rm=0);

	virtual int getState();

	virtual void addContacts(	int state,int dstate,
					StringVector & FromList,
					StringVector & ToList,
					AST * control=0
				);
	virtual int	connectToState(int   state,Association * usingAssociation,EdgeList & edgeList);
	virtual int	connSetToState(Set & state,Association * usingAssociation,EdgeList & edgeList);
	virtual int	disconnToState(Association * usingAssociation,Association & edgeList,int state=c_Und);
	virtual	int 	resetResource(int softOrHard=0);
	virtual	void	createReverseMap(ReverseMap * rm);
protected:
	ASTVector m_ConnectVector;
	int m_SwitchState; // current state of switch/relay [ deen,ener1,ener2,closed, etc....]
	int m_UndoCommitted;
	EdgeList m_ContactList;
	int m_DefaultState;
	Set m_AllStates;
	int calculateDisconnect(int state);
	int uncommitEdges(Association * usingAssociation,Association & edgeList);
	int commitEdges  (int state,Association * usingAssociation,EdgeList & edgeList);
	void invalidateDynamicClass	();
	void calculateDynamicClass	();
private:
	// Disable copy/assignment
        SwitchModel (const SwitchModel &);
        const SwitchModel & operator= ( const SwitchModel & );

	class ResourceContextAST * m_ResourceContextAST;
	
	void	addContact	( int, const RWCString &, const RWCString &, AST * control=0 );

};


class MatrixSwitch : public SwitchModel {
public:
	MatrixSwitch ( Resource * previous, RWCString & name );
	MatrixSwitch ( Resource * previous, RWCString & newName, MatrixSwitch * source );
	virtual Resource * clone(Resource * previous,RWCString & newName);
	virtual void addContacts(	int state,int dstate,
					StringVector & FromList,
					StringVector & ToList,
					AST * control=0
				);
	virtual int TellMe(Set & energizer);
	virtual int	connectToState(int state,Association * usingAssociation,EdgeList & edgeList);
	virtual int	disconnToState(Association * usingAssociation,Association & edgeList,int state=c_Und);

private:
};


class GangSwitch : public SwitchModel {
public:
	GangSwitch ( Resource * previous, RWCString & name );
	GangSwitch ( Resource * previous, RWCString & newName, GangSwitch * source );
	virtual Resource * clone(Resource * previous,RWCString & newName);
	virtual void addContacts(	int state,int dstate,
					StringVector & FromList,
					StringVector & ToList,
					AST * control=0
				);
	virtual int TellMe(Set & energizer);
	virtual int	connectToState(int state,Association * usingAssociation,EdgeList & edgeList);
	virtual int	disconnToState(Association * usingAssociation,Association & edgeList,int state=c_Und);

private:
};

class MultiplexSwitch : public SwitchModel {
public:
	MultiplexSwitch ( Resource * previous, RWCString & name );
	MultiplexSwitch ( Resource * previous, RWCString & newName, MultiplexSwitch * source );
	virtual Resource * clone(Resource * previous,RWCString & newName);
	virtual void addContacts(	int state,int dstate,
					StringVector & FromList,
					StringVector & ToList,
					AST * control=0
				);
	virtual int TellMe(Set & energizer);
	virtual int	connectToState(int state,Association * usingAssociation,EdgeList & edgeList);
	virtual int	disconnToState(Association * usingAssociation,Association & edgeList,int state=c_Und);

private:
};

class SpstSwitch : public SwitchModel {
public:
	SpstSwitch ( Resource * previous, RWCString & name );
	SpstSwitch ( Resource * previous, RWCString & newName, SpstSwitch * source );
	virtual Resource * clone(Resource * previous,RWCString & newName);
	virtual void addContacts(	int state,int dstate,
					StringVector & FromList,
					StringVector & ToList,
					AST * control=0
				);
	virtual int TellMe(Set & energizer);
	virtual int	connectToState(int state,Association * usingAssociation,EdgeList & edgeList);
	virtual int	disconnToState(Association * usingAssociation,Association & edgeList,int state=c_Und);

private:
};


#endif	// SwitchModel_h
