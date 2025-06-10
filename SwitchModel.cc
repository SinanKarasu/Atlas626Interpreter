#include	"AppendCompat.h"
#include	"SwitchModel.h"
#include	"TedlDeviceAST.h"
#include	"TAGContext.h"
#include	"Contact.h"
#include	"Vertex.h"
#include	"NodeName.h"
#include	"Circuit.h"

// SwitchModel -------------------------------------------------------

SwitchModel::SwitchModel(Resource * previous,const std::string & name)
	:Resource(previous,name,RWCString(""))
	,m_SwitchState		(c_Und)
	,m_DefaultState		(c_Und)
	,m_UndoCommitted	(c_Und)
	{
		m_ResourceContextAST=new ResourceContextAST(this,"~SWITCH");
	}

SwitchModel::SwitchModel(Resource * previous,const std::string & newName, SwitchModel * source)
	:Resource(previous,newName,source)
	,m_ConnectVector	( source->m_ConnectVector )
	,m_SwitchState		( source->m_SwitchState   )
	,m_DefaultState		( source->m_DefaultState  )
	,m_AllStates		( source->m_AllStates     )
	,m_UndoCommitted	( source->m_UndoCommitted )
	{
		m_ResourceContextAST=new ResourceContextAST(this,"~SWITCH");
		setState(source->getState());

		// Here is an important point. 
		// Build must be called from the most derived class that
		// overrides the AddEdge method. Also make sure that we
		// do not multiply call it....
		// circuit()->Build(this,source);

	}

int
SwitchModel::getState(){return m_SwitchState;}

Resource *
SwitchModel::clone(Resource * previous,const std::string & newName)
	{
			return new SwitchModel(previous,newName,this);
	}

void
SwitchModel::AddEdge(const const std::string & from,const const std::string & to,Edge * contact)
	{
		addContact(contact->m_state,from,to);
	}

int
SwitchModel::setState(int state ,ReverseMap * rm)
	{
		m_SwitchState=state;
		invalidateDynamicClass();
		if(rm){
			//EdgeListIterator clit(m_ContactList);
			for(const auto& clit: m_ContactList) {
			//while(++clit){
				Edge * e=clit;
				rm->processEdge(e);
				//rm->processNode(e->Sorc);
				//rm->processNode(e->Dest);
			}
		}
		return m_SwitchState;
	}


void
SwitchModel::addContact(int state,const const std::string & from, const const std::string & to,AST * control)
	{
		Edge * edge1 = 0 ;
		Edge * edge2 = 0 ;
 		Vertex * sorc=AddNode(NodeName(from,this),SwitchContactNodeType);
		Vertex * dest=AddNode(NodeName(to,this),SwitchContactNodeType);
		
		// search to see if we have this contact already
		//EdgeListIterator P(m_ContactList);
	
		// BEGIN... this is a consistency check and should be disabled eventually.....
		for(const auto& P: m_ContactList) {
			//while(++P){
			Edge * e=P;
			assert(!((e->Sorc->m_Rank==sorc->m_Rank) && (e->Dest->m_Rank==dest->m_Rank)));
			assert(!((e->Sorc->m_Rank==dest->m_Rank) && (e->Dest->m_Rank==sorc->m_Rank)));
		}
		// END... this is a consistency check and should be disabled eventually.....

		m_Circuit->AddEdge( sorc , dest , edge1=new Contact(this,state,sorc,dest));
		m_Circuit->AddEdge( dest , sorc , edge2=new Contact(this,state,dest,sorc));
		edge1->m_forward=1;
		edge2->m_forward=0;
		edge1->m_other=edge2;
		edge2->m_other=edge1;

		edge1->setCondition(state);
		edge2->setCondition(state);
		m_ContactList.append(edge1);
		m_ContactList.append(edge2);
	}


void
SwitchModel::addContacts	(	int state,int dstate,
					StringVector & FromList,
					StringVector & ToList,
					AST * control
				)
	{
		Edge * edge;
		for(int i=0;i<FromList.size();i++){
			addContact(state,FromList[i],ToList[i]);
		}
		m_AllStates.set(state);
		if(dstate>=0)m_DefaultState=dstate;
		if( (FromList.size()==0) && m_DefaultState==c_Und ){
			m_DefaultState=state;
		}
		m_SwitchState=m_DefaultState;
		m_UndoCommitted=m_SwitchState;
		m_ConnectVector.append(control);
	}

int
SwitchModel::connectToState(int state,Association * usingAssociation,EdgeList & edgeList)
	{
		if(state!=m_SwitchState){
			m_UndoCommitted=m_SwitchState;
			setState(state);
			m_ConnectVector[state]->eval(m_ResourceContextAST);
			invalidateDynamicClass();
			calculateDynamicClass();
			return 1;
		} else {
			return 0;
		}
	}

int
SwitchModel::connSetToState(Set & state,Association * usingAssociation,EdgeList & 	edgeList)
	{
		assert(0);
		return 0;
	}

int
SwitchModel::disconnToState(Association * usingAssociation,Association & edgeList,int state)
	{
		uncommitEdges(usingAssociation,edgeList);
		uncommit(usingAssociation);
		if(committed()){
			return 0;
		} else if(state==m_SwitchState){
			return 0;
		} else {
			setState(state);
			m_ConnectVector[state]->eval(m_ResourceContextAST);
			invalidateDynamicClass();
			calculateDynamicClass();
			return 1;
		}
	}

int
SwitchModel::resetResource(int softOrHard)
	{
		m_UndoCommitted=m_SwitchState;
		setState(m_DefaultState);
		if(softOrHard){
			m_ConnectVector[m_DefaultState]->eval(m_ResourceContextAST);
		}
		invalidateDynamicClass();
		calculateDynamicClass();
		//EdgeListIterator clit(m_ContactList);
		for(const auto& clit: m_ContactList) {
		//while(++clit){
			Edge * e=clit;
			e->uncommit(0);
		}
		return Resource::resetResource(softOrHard);
	}

int
SwitchModel::calculateDisconnect(int state)
	{
		// Calculate a disconnect state
		int tstate=c_Und;	
		Set target;
		target=0;
		target.set(m_SwitchState); // make it the set of current state;
		
		
		target ^= m_AllStates;	// Set of all the other states (plus invalid ones)
		target &= m_AllStates;	// Constrain to the valid ones.
		
		// Now target contains all the valid disconnect states.
		
		if(m_UndoCommitted==m_SwitchState){
			tstate=m_SwitchState;
		} else if ( target.isSet(m_DefaultState) ){
			tstate=m_DefaultState;
		} else {
			std::cerr << "PANIC.. Can not find a state to disconnect to...." << std::endl;
			assert(target.isSet(m_DefaultState));
		}
		
		if(state!=c_Und){
			if(tstate!=state){
				// Error, I don't know what is going on...
				// but the disconnect state we found is different than requested.
				// Somebody is computing wrong....
				if(!m_AllStates.isSet(state)){
					std::cerr << "PANIC.. No such state to disconnect to...." << std::endl;
					assert(m_AllStates.isSet(state));
				}
				tstate=state;
			}
		}
		return tstate;
	}


int
SwitchModel::uncommitEdges(Association * usingAssociation,Association & edgeList)
	{
		int status=1;
		//EdgeListIterator clit(m_ContactList);
		for(const auto& clit: m_ContactList) {
		//while(++clit){
			Edge * e=clit;
			if(edgeList.contains(e)){	// 7-25-1999. Appears that not checked agains m_other....
				e->uncommit(usingAssociation);
			}		
			status = (!e->committed(usingAssociation)) && status ;
		}
		return status;
	}

int
SwitchModel::commitEdges(int state,Association * usingAssociation,EdgeList & edgeList)
	{
		int status=1;
		//EdgeListIterator clit(m_ContactList);
		for(const auto& clit: m_ContactList) {
		//while(++clit){
			Edge * e =clit;
			if(edgeList.contains(e)){
				e->uncommit(usingAssociation);	// remove usingAssociation first
				if(e->committed()){	// anybody else ???
					// somebody else is usingAssociation this contact.
					// check to see if it is OK to go to the new state.
					// assert(state==m_SwitchState);
					status = (state==m_SwitchState) && status ;
				}
				// All is kosher for this edge and we need it
				e->commit(usingAssociation);
			}
		}
		return status;
	}

void
SwitchModel::invalidateDynamicClass	()
	{
		Vertex::V_DFSDynamicSearch++;
		//EdgeListIterator clit(m_ContactList);
		for(const auto& clit: m_ContactList) {
		//while(++clit){
			Edge * e=clit;
			//e->Source->invalidateDynamicClass();
			//e->Dest->invalidateDynamicClass();
			e->invalidateDynamicClass();
		}
	}
void
SwitchModel::calculateDynamicClass	()
	{
		//EdgeListIterator clit(m_ContactList);
		for(const auto& clit: m_ContactList) {
		//while(++clit){
			Edge * e=clit;
			e->calculateDynamicClass();
		}
	}


//static int pcnt=0;

void
SwitchModel::createReverseMap(ReverseMap * rm){
	int states=m_ConnectVector.size();
	for(int state=0;state<states;state++){
		//pcnt++;
		//if(pcnt==1207){
		//	pcnt++;
		//}
		AST * x= m_ConnectVector[state]->init(m_ResourceContextAST);
		if(x){
			//cout << " map state "<< state << ":" << x->getName() << endl;
			rm->insertState( x->getName() , this , state );
		}
	}
	Resource::createReverseMap(rm);	// it is turtles all the way down
}

MatrixSwitch::MatrixSwitch ( Resource * previous, const std::string & name )
	:SwitchModel(previous,name)
	{
	}
	
MatrixSwitch::MatrixSwitch ( Resource * previous, const std::string & newName, MatrixSwitch * source )
	:SwitchModel(previous,newName,source)
	{
		circuit()->Build(this,source);
	}

Resource *
MatrixSwitch::clone(Resource * previous,const std::string & newName)
	{
		return new MatrixSwitch(previous,newName,this);
	}

void
MatrixSwitch::addContacts	(	int state,int dstate,
					StringVector & FromList,
					StringVector & ToList,
					AST * control
				)
	{
		assert(0);	// We use bunch og GangSwitches for Matrix...
	}

int
MatrixSwitch::connectToState(int state,Association * usingAssociation,EdgeList & edgeList)
	{
		// This is illegal.
		assert(0);
		return 0;
	}

int
MatrixSwitch::disconnToState(Association * usingAssociation,Association & edgeList,int state)
	{
		// This is illegal.
		assert(0);
		return 0;
	}


int
MatrixSwitch::TellMe(Set & energizer)
	{
		assert(0);
		return 0;
	}

GangSwitch::GangSwitch ( Resource * previous, const std::string & name )
	:SwitchModel(previous,name)
	{
	}

GangSwitch::GangSwitch ( Resource * previous, const std::string & newName, GangSwitch * source )
	:SwitchModel(previous,newName,source)
	{
		circuit()->Build(this,source);
	}


Resource *
GangSwitch::clone(Resource * previous,const std::string & newName)
	{
		return new GangSwitch(previous,newName,this);
	}

void
GangSwitch::addContacts	(	int state,int dstate,
				StringVector & FromList,
				StringVector & ToList,
				AST * control
			)
	{
		SwitchModel::addContacts(state,dstate,FromList,ToList,control);
	}

int
GangSwitch::connectToState(int state,Association * usingAssociation,EdgeList & edgeList)
	{

		int status=1;
		//uncommit(usingAssociation);	// 12/17/2000 see if fixes... sik...
		status=commitEdges(state,usingAssociation,edgeList);
		if(status==1){
			status = SwitchModel::connectToState(state,usingAssociation,edgeList);
		}
		commit(usingAssociation);	// 12/17/2000 .. sik
		return status;
	}

int
GangSwitch::disconnToState(Association * usingAssociation,Association & edgeList,int state)
	{
		
		int tstate=calculateDisconnect(state);
		return SwitchModel::disconnToState(usingAssociation,edgeList,tstate);

	}

int
GangSwitch::TellMe(Set & energizer)
	{
		return energizer.isSet(m_SwitchState);
	}

MultiplexSwitch::MultiplexSwitch ( Resource * previous, const std::string & name )
	:SwitchModel(previous,name)
	{
	}
	
MultiplexSwitch::MultiplexSwitch ( Resource * previous, const std::string & newName, MultiplexSwitch * source )
	:SwitchModel(previous,newName,source)
	{
		circuit()->Build(this,source);
	}

Resource *
MultiplexSwitch::clone(Resource * previous,const std::string & newName)
		{
			return new MultiplexSwitch(previous,newName,this);
		}

void
MultiplexSwitch::addContacts(	int state,int dstate,
				StringVector & FromList,
				StringVector & ToList,
				AST * control
			)
	{
		SwitchModel::addContacts(state,dstate,FromList,ToList,control);
	}

int
MultiplexSwitch::TellMe(Set & energizer)
	{
		return energizer.isSet(m_SwitchState);
	}

int
MultiplexSwitch::connectToState(int state,Association * usingAssociation,EdgeList & edgeList)
	{
		int status=1;
		//uncommit(usingAssociation);	// remove usingAssociation first
		status=commitEdges(state,usingAssociation,edgeList);
		if(status==1){
			status=SwitchModel::connectToState(state,usingAssociation,edgeList);
		}
		commit(usingAssociation);
		return status;
	}

int MultiplexSwitch::disconnToState(Association * usingAssociation,Association & edgeList,int state)
	{
		int tstate=calculateDisconnect(state);
		return SwitchModel::disconnToState(usingAssociation,edgeList,tstate);
	}

SpstSwitch::SpstSwitch ( Resource * previous, const std::string & name )
	:SwitchModel(previous,name)
	{
	}
	
SpstSwitch::SpstSwitch ( Resource * previous, const std::string & newName, SpstSwitch * source )
	:SwitchModel(previous,newName,source)
	{
		circuit()->Build(this,source);
	}

Resource *
SpstSwitch::clone(Resource * previous,const std::string & newName)
	{
		return new SpstSwitch(previous,newName,this);
	}

void
SpstSwitch::addContacts	(	int state,int dstate,
				StringVector & FromList,
				StringVector & ToList,
				AST * control
			)
	{
		SwitchModel::addContacts(state,dstate,FromList,ToList,control);
	}

int
SpstSwitch::connectToState(int state,Association * usingAssociation,EdgeList & edgeList)
	{
		//uncommit(usingAssociation);
		int status=commitEdges(state,usingAssociation,edgeList);
		if(status){
			status = SwitchModel::connectToState(state,usingAssociation,edgeList);
		}
		commit(usingAssociation);
		return status;
	}

int
SpstSwitch::disconnToState(Association * usingAssociation,Association & edgeList,int state)
	{
		int tstate=calculateDisconnect(state);
		return SwitchModel::disconnToState(usingAssociation,edgeList,tstate);
	}

int
SpstSwitch::TellMe(Set & energizer)
	{
		return energizer.isSet(m_SwitchState);
	}
