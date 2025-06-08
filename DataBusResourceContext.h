
#ifndef DataBusResourceContext_h
#define DataBusResourceContext_h

#include <iostream>
#include <stdlib.h>
#include <string>

class DataBusResourceContext;
class Vertex;
class ConnectRequest;

#include	"ResourceContextBASE.h"
#include	"AtlasAST.h"
#include	"Graph.h"

class DataBusResourceContext: public ResourceContextBASE
{ 
	public:
		DataBusResourceContext();

		virtual void ENABLE_EXCHANGEtoTedl()=0 ;
		virtual void CONNECT_EXCHANGEtoTedl()=0 ;
		virtual void DO_EXCHANGEtoTedl()=0 ;
		virtual void DISABLE_EXCHANGEtoTedl()=0 ;
		virtual void DISCONNECT_EXCHANGEtoTedl()=0 ;
		virtual void UPDATE_EXCHANGEtoTedl()=0 ;
		virtual void FETCH_EXCHANGEtoTedl()=0 ;

		virtual void do_HOLDtoTedl()=0 ;
		virtual void do_PROCEEDtoTedl()=0 ;

		virtual void InvalidatedResult()=0 ;
		//virtual void SkipStatetoTedl()=0 ;
	
		virtual void FSMError(const std::string& t, const std::string& s)=0;
				
		// Corresponding Events for Action Functions.
		virtual void ENABLE_EXCHANGE(AST * a=0)=0;
		virtual void CONNECT_EXCHANGE(AST * a=0)=0;
		virtual void DO_EXCHANGE(AST * a=0)=0;
		virtual void DISABLE_EXCHANGE(AST * a=0)=0;
		virtual void DISCONNECT_EXCHANGE(AST * a=0)=0;
		virtual void UPDATE_EXCHANGE(AST * a=0)=0;
		virtual void FETCH_EXCHANGE(AST * a=0)=0;

		//virtual void SkipState(AST * a=0)=0;
		//virtual void Select(AST * a=0)=0;
		
		virtual void setResourceName( RWCString )=0;
		virtual void preSetContext();
	
		virtual int resetFSM();

		virtual DataBusResourceContext * clone(Resource *);

		AST * m_noun_modifier;
		DataBusResourceContext * m_Original;
		
		AST * m_transitionSigChar;
protected:
		void toEVENTS(void (DataBusResourceContext::*c)(AST *));


};

#endif // DataBusResourceContext_h
