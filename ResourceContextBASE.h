
#ifndef ResourceContextBASE_h
#define ResourceContextBASE_h

#include <iostream>
#include <stdlib.h>

class ResourceContextBASE;
class Vertex;
class ConnectRequest;

//#include	"Resource.h"
#include	"AtlasAST.h"
#include	"Graph.h"

class ResourceContextAST;

class ResourceContextBASE
{ 
	public:
		ResourceContextBASE();
		
		virtual void setResourceName( RWCString )=0;
	
		virtual RWCString theName();

		void setResource( Resource * r );

		Resource * getResource();

		
		virtual int resetFSM();

		AST * m_noun_modifier;
		ResourceContextBASE * m_Original;
		
		AST * m_transitionSigChar;
		ResourceContextAST *	getResourceContextAST();

protected:
		//virtual	int	VerifyPath(Vertex   * Source , Vertex   * Dest );
		virtual	int	find_path(Vertex * uutv, Vertex * portv);
		virtual	int	find_path(const RWCString & uutpin, Vertex * portv);
		virtual	int	find_path(const RWCString & uutpin, const RWCString & port);

		virtual	int	disconnect_path(Vertex * uutv, Vertex * portv);
		virtual	int	disconnect_path(const RWCString & uutpin, Vertex * portv);
		virtual	int	disconnect_path(const RWCString & uutpin, const RWCString & port);
			int	findPathState(Vertex * uutv, Vertex * portv);
			int	findPathState(const RWCString & uutpin, const RWCString port,int expect);		
			int	allocateResource();
			int	unallocateResource();
		ConnectRequest * undoConnectRequest(Vertex * portv=0, Vertex * uutv=0);
		AST	*	doControlAction(const RWCString & todo);
		AST	*	doPrepControl(const RWCString & todo);
		AST	*	doPrepCapability(const RWCString & todo);
		

		//EdgeList	m_EdgeList;

		AST * m_modifierList;
		AST * m_cnxList;
		AST * m_controlBlock;
		AST * m_prepCapability;
		AST * m_prepControl;

		int m_Unallocated;

		ConnectRequests	m_ConnectRequests;

		void	setResourceContextAST(ResourceContextAST * ast);
		void setcurrent(AST * a);
		int  EventsIntroduced();
		virtual void preSetContext();
		void toEVENTS(void (ResourceContextBASE::*c)(AST *));
		void sayhi(const char * s);
		Resource *	m_Resource;
		int SimulateCircuit(const RWCString & port);
private:
		ResourceContextAST * m_ResourceContextAST;
		// Disable copy constructor and copy assignment
		ResourceContextBASE (const ResourceContextBASE &);
		const ResourceContextBASE & operator= ( const ResourceContextBASE & );

};

#endif // ResourceContextBASE_h
