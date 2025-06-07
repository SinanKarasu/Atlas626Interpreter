#include	"Resource.h"
#include	"TedlStd.h"
#include	"TedlParser.h"

#include	"TAGContext.h"
#include	"Edge.h"
#include	"VertexDictionary.h"
#include	"ResourceList.h"

#include	"ExecEnv.h"


extern ExecEnv	execEnv;

ResourceContextBASE::ResourceContextBASE()
	:m_Resource(0)
	,m_noun_modifier(0)
	,m_transitionSigChar(0)
	,m_Unallocated(1)
	,m_Original(0)
	,m_ResourceContextAST(0)
	{
	}

void
ResourceContextBASE::setResource( Resource * r )
	{
		m_Resource = r;
		m_ConnectRequests.init(r);
	}

Resource *
ResourceContextBASE::getResource()
	{
		return m_Resource ;
	}


RWCString
ResourceContextBASE::theName()
	{
		return m_Resource->theName();
	}


void
ResourceContextBASE::toEVENTS(void (ResourceContextBASE::*c)(AST *))
	{
		// should never be called
		assert(0);
	}


void
ResourceContextBASE::setcurrent(AST * a)
	{
		if(a){
			m_ResourceContextAST->currentParameters=a->eval(a);
		}
	}
	
void
ResourceContextBASE::preSetContext()
	{
		AST * sig_char		= m_transitionSigChar->ASTdown();
		m_modifierList		= sig_char;
		sig_char		= sig_char->ASTright();
		m_cnxList		= sig_char;
		sig_char		= sig_char->ASTright();
		m_controlBlock		= sig_char;

		sig_char		= m_transitionSigChar->ASTright();
		if(sig_char){
			m_prepControl		= sig_char->ASTdown();
		} else {
			m_prepControl		= 0;
		}
		//sig_char		= sig_char->ASTright();
		//m_prepCapability	= sig_char;
	
		// Get the gate_conn_or_event_field from ATLAS
		StringAST cnx("~CNX");
		AST * cnxAction = m_ResourceContextAST->currentParameters->data(&cnx);

		// Now ask it to introduce itself
		if(cnxAction)cnxAction->eval(m_ResourceContextAST);
	}
int
ResourceContextBASE::EventsIntroduced()
	{
		if(m_Resource->getFromEvent()){
			return 1;
		} else {
			return 0;
		}
	}




int
ResourceContextBASE::find_path(Vertex * uutv, Vertex * portv)
	{

		EdgeList edgeList;

		m_Resource->FindPath( uutv , portv , edgeList );
					
			
		//VerifyPath(uutv,portv);
		
		
		{// First set all the bits in the feasible union
			//EdgeListIterator P(edgeList);
			for(const auto P: edgeList) {
			//while (++P){
				Edge * e=P;
				Resource * r=e->getResource();
				r->m_predict=1;
				if(execEnv.relayFeasible()){
					debugtrace << e->theName() << r->m_predict << std::endl ;
				}
			}
		}

		{// Next AND the edge states with the feasible union
			//EdgeListIterator P(edgeList);
			for(const auto P: edgeList) {
			//while (++P){
				Edge * e=P;
				Resource * r=e->getResource();
				r->m_predict &= e->theSet();
				if(execEnv.relayFeasible()){
					debugtrace << e->theName() << r->m_predict << std::endl ;
				}
			}
		}

		{	// At this point if the feasible union of a resource
			// referenced in the edgeList is zero, then there is a conflict
			// in the required state for the resource and we can't use it.
			// This is an error condition.
			// However, since Search now does the same test recursively
			// and ensures that any path found is feasible, this
			// becomes just a redundant confirmation.
			Association * cr = m_ConnectRequests.getOne(portv)->getOne(uutv);
			//EdgeListIterator P(edgeList);
			for(const auto P: edgeList) {
			//while (++P){
				Edge * e=P;
				Resource * r=e->getResource();
				if(e->Sorc->equivalenceClass()==e->Dest->equivalenceClass()){
					if		(e->Sorc->equivalenceClass()==uutv->equivalenceClass()){
						//	just a wire on uut side. Do not commit those.
					} else if	(e->Sorc->equivalenceClass()==portv->equivalenceClass()){
						//	just a wire on device side. Do not commit those.
					} else {
						// a wire..
						e->commit(cr);
						m_ConnectRequests.insertEdge(portv,uutv,e);
					}
				} else {
					if(e->Closed()){
						// lock it anyway...
						r->connectToState(r->getState(),cr,edgeList);
					} else {
						int p;
						if((p=r->m_predict.firstTrue())!=RW_NPOS){
							// So there is way to close it.Just do it then.
							if(execEnv.deviceTrace()){
								debugtrace << " calling DEVICE " << r->theName() << std::endl;
							}
							r->connectToState(p,cr,edgeList);
						} else {
							std::cerr << "No feasible set for relay:" << r->theName() << std::endl;
							assert(0);
						}
					}
					e->commit(cr);
					m_ConnectRequests.insertEdge(portv,uutv,e);
					if(e->committed(cr)) {
						if((!e->onlycommitted(cr))){
							std::cerr 	<< "//--"	<< theName()
								<< ":Contact "	<< e->theName()
								<<" also belongs to :" << std::endl;
							e->listCommitted(cr);
						}
					}
				}
			}
		}
		
		edgeList.clear();
		
		return 1+0;
	}


int
ResourceContextBASE::find_path(const RWCString & uutpin, Vertex * portv)
	{
		extern TedlParser	*	TEDL;
		Vertex		* uutv  = 0;
		if ( TEDL==NULL){
			return 0;
		} else if ( (TEDL->UutConnectors).findValue(uutpin,uutv) ){
			return find_path(uutv,portv);
		} else if ( (TEDL->AdapterConnectors).findValue( uutpin, uutv ) ){
			return find_path(uutv,portv);
		} else if ( (TEDL->InterfaceConnectors).findValue( uutpin, uutv ) ){
			return find_path(uutv,portv);
		} else {
			std::cerr	<< " CNX PIN " << uutpin 
				<< " Does not exist " << std::endl;
			return 0;
		}
	
	}
	
int
ResourceContextBASE::find_path(const RWCString & uutpin,const RWCString & port)
	{
		Vertex			* portv = 0;
		
		if(!( portv=m_Resource->node(port) )){
			std::cerr 	<< " DEVICE "	<< m_Resource->getName() 
				<< " PORT "	<< port
				<< " Does not exist " << std::endl;
			return 0;
		} else {
			return find_path(uutpin,portv);
		}
		
	}

int
ResourceContextBASE::findPathState(Vertex * uutv, Vertex * portv)
	{

		//uutv->setDynamicClass();
		//portv->setDynamicClass();
		if(uutv->equivalenceClass()==portv->equivalenceClass()){
			return -1;
		} else if(uutv->dynamicClass()==portv->dynamicClass()){
			return 1;
		} else {
			return 0;
		}
	}

int
ResourceContextBASE::findPathState(const RWCString & uutpin, const RWCString port,int expect)
	{
		extern TedlParser	*	TEDL;
		Vertex * portv;
		portv=m_Resource->node(port);
		getResource()->getName();
		Vertex	* uutv  = 0;
		int finalState;
		return 0;	// sik fix this.....
		if ( TEDL==NULL){
			return expect;	// no TEDL, nothing to do.....
		} else if ( (TEDL->UutConnectors).findValue(uutpin,uutv) ){
			finalState = findPathState(uutv,portv);
		} else if ( (TEDL->InterfaceConnectors).findValue(uutpin, uutv ) ){
			finalState = findPathState(uutv,portv);
		} else if ( (TEDL->AdapterConnectors).findValue(uutpin, uutv ) ){
			finalState = findPathState(uutv,portv);
		};
		if(finalState==expect){
		} else if(finalState==-1){
			if(execEnv.pathFeasible()){
				std::cerr 	<< " Pin: " << uutpin
					<< " and: " << getResource()->getName()
					<< " pin: " << port
					<< " are hard wired. No connect/disconnect possible."
					<< std::endl ;
				}
		} else if(finalState==1){
			std::cerr 	<< " Pin: " << uutpin
				<< " and: " << getResource()->getName()
				<< " pin: " << port
				<< " could not be disconnected."
				<< std::endl ;
		} else if(finalState==0){
			std::cerr 	<< " Pin: " << uutpin
				<< " and: " << getResource()->getName()
				<< " pin: " << port
				<< " could not be connected."
				<< std::endl ;
		} else {
			std::cerr << " Internal error " << std::endl;
			assert(0);
		}
			
	}

ConnectRequest *
ResourceContextBASE::undoConnectRequest(Vertex * portv, Vertex * uutv)
	{
		//ConnectRequest * portC=m_ConnectRequests.getOne(portv);
		//ConnectRequestsIterator portA (m_ConnectRequests);
		for(const auto portA: m_ConnectRequests){ 
		//while(++portA){
			//ConnectRequestIterator uutA (*portA.key());
			for(const auto uutA:*portA){ 
			//while( ++uutA ){
				//AssociationIterator ait(*uutA.key());
				for(const auto ait:*uutA){ 
				//while(++ait){
					Edge * e=ait;
					Resource * r=e->getResource();
					if(e->Sorc->equivalenceClass()==e->Dest->equivalenceClass()){
						// a wire.. 
						e->uncommit(uutA);
					} else {
						r->disconnToState(uutA,*(uutA));
					}
				}
			}
		}
		m_ConnectRequests.cleanAll();
		return 0;
	}


int
ResourceContextBASE::resetFSM()
	{
		undoConnectRequest();
		return 1;
	}

int
ResourceContextBASE::disconnect_path(Vertex * uutv, Vertex * portv)
	{
		if(execEnv.showPath()){
			debugtrace << "// Disconnecting " << uutv->theName() << "..." <<  portv->theName() << std::endl;
		}
		ConnectRequest * portC=m_ConnectRequests.getOne(portv);
		if(portC){
			Association * uutA=portC->getOne(uutv);
			if(uutA){
				//AssociationIterator uit(*uutA);
				for(const auto uit: *uutA){
				//while( ++uit ){
					Edge * e=uit;
					Resource * r=e->getResource();
					if(e->Sorc->equivalenceClass()==e->Dest->equivalenceClass()){
						// a wire.. 
						e->uncommit(uutA);
					} else {
						if(r->disconnToState(uutA,*uutA)) {
							if(execEnv.pathFeasible()){
								debugtrace << "Disconnected all :" <<  r->getName() << std::endl;
							}
						} else {
							if(execEnv.pathFeasible()){
								debugtrace << "Some were not Disconnected :" << r->getName() << std::endl;
							}
						}
					}
				}
				portC->clean(uutA);
			} else {
				std::cerr << " PANIC , No such Association !!! " << std::endl;
				assert(0);
			}
		} else {
			std::cerr << " PANIC , No such ConnectRequest !!! " << std::endl;
			assert(0);
		}
		
		return 1+0;	
	
	}
	
int
ResourceContextBASE::disconnect_path(const RWCString & uutpin, Vertex * portv)
	{
		extern TedlParser	*	TEDL;
		Vertex		* uutv  = 0;
		if ( TEDL==NULL){
			return 0;
		} else if( (TEDL->UutConnectors).findValue(uutpin,uutv) ){
			return disconnect_path(uutv,portv);
		} else if ( (TEDL->InterfaceConnectors).findValue( uutpin, uutv ) ){
			return disconnect_path(uutv,portv);
		} else if ( (TEDL->AdapterConnectors).findValue( uutpin, uutv ) ){
			return disconnect_path(uutv,portv);
		} else {
			std::cerr	<< " CNX PIN " << uutpin 
				<< " Does not exist " << std::endl;
			return 0;
		}
	
	}
	
int
ResourceContextBASE::disconnect_path(const RWCString & uutpin,const RWCString & port)
	{
		Vertex			* portv = 0;
		
		if(!( portv=m_Resource->node(port) )){
			std::cerr 	<< " DEVICE "	<< m_Resource->getName() 
				<< " PORT "	<< port
				<< " Does not exist " << std::endl;
			return 0;
		} else {
			return disconnect_path(uutpin,portv);
		}
		
	}


AST *
ResourceContextBASE::doControlAction(const RWCString & todo)
	{
		StringAST action(todo);
		AST * controlAction	=	m_controlBlock->data(&action);
		if(controlAction){
			controlAction->eval(m_ResourceContextAST);
		}
		return controlAction;
	
	}

AST *
ResourceContextBASE::doPrepControl(const RWCString & todo)
	{
		StringAST action(todo);
		if(m_prepControl){
			AST * prepControlAction	=	m_prepControl->data(&action);
			if(prepControlAction){
				prepControlAction->eval(m_ResourceContextAST);
			}
			return prepControlAction;
		} else {
			return 0;
		}
	
	}

AST *
ResourceContextBASE::doPrepCapability(const RWCString & todo)
	{
		StringAST action(todo);
		AST * prepCapabilityAction	=	m_prepCapability->data(&action);
		if(prepCapabilityAction){
			prepCapabilityAction->eval(m_ResourceContextAST);
		}
		return prepCapabilityAction;
	
	}

int
ResourceContextBASE::allocateResource()
	{
		extern TedlParser	*	TEDL;
		if ( TEDL==NULL){
			return 0;
		} else if((TEDL->activeResources).contains(m_Resource)){
			assert(m_Unallocated==1);
			return 1;
		} else {
			(TEDL->activeResources).append(m_Resource);
			 m_Unallocated=0;
			 return 0;
		}
	}
	
int
ResourceContextBASE::unallocateResource()
	{
		extern TedlParser	*	TEDL;
		if ( TEDL==NULL){
			return 0;
		} else {
			m_Resource->resetResource(0);
			if((TEDL->activeResources).contains(m_Resource)){
				(TEDL->activeResources).remove(m_Resource);
				m_Unallocated=1;
				return 1;
			} else {
				assert(m_Unallocated==1);
				return 0;
			}
		}
	}

		
void
ResourceContextBASE::sayhi(const char * s)
	{
		if(execEnv.deviceTrace()){
			debugtrace << "In " << s << "Device : " << m_Resource->getName() << std::endl;
		}
	}


void
ResourceContextBASE::setResourceContextAST(ResourceContextAST * ast)
	{
		if(m_ResourceContextAST){
			delete m_ResourceContextAST;
			m_ResourceContextAST=0;
		}
		m_ResourceContextAST=ast;
	}

ResourceContextAST *
ResourceContextBASE::getResourceContextAST()
	{
		return	m_ResourceContextAST;
	}

int
ResourceContextBASE::SimulateCircuit(const RWCString & port)
	{
		Vertex			* portv = 0;
		
		if(!( portv=m_Resource->node(port) )){
			std::cerr 	<< " DEVICE "	<< m_Resource->getName() 
				<< " PORT "	<< port
				<< " Does not exist " << std::endl;
			return 0;
		} else {
			return m_Resource->SimulateCircuit(portv );
		}
		
	}
