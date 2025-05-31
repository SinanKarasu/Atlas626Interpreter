#include	"Std.h"
#include	"Resource.h"
#include	"BasicTypeAST.h"
#include	"SignalTypeAST.h"
#include	"ATEResourceAST.h"
#include	"TedlDeviceAST.h"

#include	"Sensor.h"
#include	"SourceLoad.h"
#include	"EventMonitor.h"
#include	"DataBus.h"
#include	"VertexDictionary.h"
#include	"ResourceList.h"
#include	"Circuit.h"

extern AssociationListStack theAssociationListStack;

AST *
makeTypedData ( RWCString & type)
	{
		if(		type=="R"){
			return new DecimalNumber(0.0);
		} else if(	type=="I"){
			return new IntegerNumber(0);
		//} else if(	type=="SB"){
		//	return new StringOfBitType(0);
		//} else if(	type=="SC"){
		//	return new StringOfCharType(0);
		//} else if(	type=="RA"){
		//	return new IntegerNumber(0);
		//} else if(	type=="IA"){
		//	return new IntegerNumber(0);
		//} else if(	type=="AB"){
		//	return new IntegerNumber(0);
		//} else if(	type=="AC"){
		//	return new IntegerNumber(0);
		//} else if(	type=="MD"){
		//	return new IntegerNumber(0);
		//} else if(	type=="MO"){
		//	return new IntegerNumber(0);
		//} else if(	type=="SS"){
		//	return new IntegerNumber(0);
		} else {
			assert(0);
		}
		return 0;
		
			
	}

// Resource     -------------------------------------------------------
// Creates a blank slate. If it is not the first one in the hierarchy, it simply 
// uses the previous ones dictionary (.i.e. the top level dictionaries)
Resource::Resource( Resource * previous, const RWCString & name, const RWCString & version )
	:m_ResourceDictionary(0)
	,m_previous(previous)
	,m_callArgEntries(0)
	,m_nodeDictionary(0)
	,m_Circuit(0)
	,m_usingAssociationList(0)
	,d_printFlag(0)
	{
		if( m_previous ){
			m_nodeDictionary = m_previous->m_nodeDictionary;
			m_callArgEntries = m_previous->m_callArgEntries;
		}else{
			m_nodeDictionary = new VertexDictionary;
			m_callArgEntries = new SymbolDictionary;

		}
		m_name    = name;
		m_version = version;
		m_Circuit = new Circuit( Impedance( c_Inf, c_Inf ) );
		m_states  = c_Und;
		
		m_DfsNum	= 0;
		
		m_currentAnalogFSM		= 0;
		m_currentDataBusFSM		= 0;

		m_ResourceDictionary		= 0;
		m_devicePathList		= 0;
		resetResource();
	}

// Creates a clone from source. For each resource in the source->m_ResourceList
// the resource is cloned with "this" as previous. CloneResource in turn calls
// clone(...) method for the derived class, which again calls this constructor.
// (i.e. it is turtles all the way down...)
// This creates all the Graph structures that reside "under" this node.
// However, then the vertices/edges defined at this level must be defined. Which is
// done by calling the Graph clone constructor which also clones the stateList.
//   
Resource::Resource( Resource * previous, const RWCString & newName, Resource * source )
	:m_ResourceDictionary(0)
	,m_previous(previous)
	,m_callArgEntries(0)
	,m_nodeDictionary(0)
	,m_Circuit(0)
	,m_usingAssociationList(0)
	,d_printFlag(0)
	{
		ResourceListIterator	rlit( source->m_ResourceList );
		Resource *		turtle; // it is turtles all the way down
		CapabilityListIterator	clit( *(source->getCapabilities()) );


		m_DfsNum	= 0;
		
		if( m_previous ){
			m_nodeDictionary = m_previous->m_nodeDictionary;
			m_callArgEntries = m_previous->m_callArgEntries;
		}else{
			m_nodeDictionary = new VertexDictionary;
			m_callArgEntries = new SymbolDictionary;

		}
		
		SymbolDictionaryIterator sit(*(source->m_callArgEntries));
		while(++sit){
			m_callArgEntries->insertKeyAndValue(sit.key(),sit.value()->clone());
		}
		
		if( newName != "" ){	// we are renaming
			m_name = newName;
		}else{			// use the old name
			m_name = source->m_name;
		}
		m_version = source->m_version;
		// Make the stateList
		m_states  = c_Und;
		m_Circuit = new Circuit(Impedance(c_Inf,c_Inf));
		
		while( ++rlit ){
			turtle = rlit.key();
			turtle = turtle->clone( this, turtle->m_name );
			AddResource( turtle );
		}
		
		
		while( ++clit ){
			insertCapability( clit.key() );
		}
		
		m_currentAnalogFSM		= 0;
		m_currentDataBusFSM		= 0;
		
		m_capabilityNamedEntries= source->m_capabilityNamedEntries;
		m_capabilityNounEntries	= source->m_capabilityNounEntries;			
		m_devicePathList	= source->m_devicePathList;

		resetResource();

	}

Resource::~Resource()
	{
		assert(0);
	}

Resource *
Resource::clone	( Resource * previous, RWCString & newName )
	{
		assert(0);return new Resource( previous, newName, this );
	}

Resource *
Resource::instantiate	( Resource * previous, const RWCString & newName )
	{
		return clone(previous, newName);
	}

void
Resource::setName( RWCString n )
	{
		m_name = n;
	}

ResourceList &
Resource::resourceList()
	{
		return  m_ResourceList;
	}

Circuit *
Resource::circuit() const
	{
		return m_Circuit;
	}

int
Resource::getState()
	{
		return 0;
	}

void
Resource::addContacts	(	int state,int dstate,
				StringVector & FromList,
				StringVector & ToList,
				AST * control
			)
	{
		assert(0);
	}
	
Resource *
Resource::AddResource(Resource * resource)
	{
		if(resource){
			m_ResourceList.insert(resource);
			return resource;
		} else {
			return 0;
		}
	}


Set
Resource::getPredictor(int dfsNum)
	{
		if(m_DfsNum < Graph::G_DfsSearchStart){
			m_predict=1;
		}
		m_DfsNum=dfsNum;
		return m_predict;
	}

int
Resource::TellMe(Set & energizer)
	{
		return 1;
	}

Resource *
Resource::getDevice(RWCString & dev)
	{
		return 0;
	}


Resource *
Resource::getPrev()
	{
		return m_previous;
	}

Resource *
Resource::renamePort( RWCString & from, RWCString & to )
	{
		Vertex * x;
		
		if( nodes()->findValue( from, x ) ){
		
			nodes()->remove( from );
			nodes()->insertKeyAndValue( to, x );
			return this;
		}else{
			return 0;
		}
	}

Vertex *
Resource::vertex(const RWCString & name)
	{
		Vertex * x;
		
		if( nodes()->findValue( name, x ) ){ 
			return x;
		}else{
			return 0;
		}
	}

int
Resource::printRequested()
	{
		//if(theName()=="SLOT13_K9"){
		//	return true;
		//} else {
		//	return false;
		//}
		return d_printFlag;
	}

void
Resource::printCommitted(RWCString pre)
	{
			cerr << "printCommitted " << pre << endl;
			if(!m_usingAssociationList){
				cerr << theName() << "Not committed" << endl;
			} else {
				cerr << theName() << "is committed to:" << endl ;
				AssociationListIterator alit(*m_usingAssociationList);
				while(++alit){
					cerr << "//-->>>>>>>"+alit.key()->theName() << endl;
				}
			}
	}

int
Resource::committed(Association * r)
	{
		if(printRequested()){
			printCommitted("in Commited");
		}
			
		if(!m_usingAssociationList){
			return 0;
		} else if(r==0){
			return ( m_usingAssociationList->entries() > 0 );
		} else if(m_usingAssociationList->contains(r)){
			return 1;
		} else {
			return 0;
		}
	}

void
Resource::commit(Association * r)
	{
		assert(r);
		if(printRequested()){
			printCommitted("Before Commit");
		}
		if(!m_usingAssociationList){
			m_usingAssociationList=theAssociationListStack.getOne();
			m_usingAssociationList->append(r);
		} else if(!m_usingAssociationList->contains(r)){
			m_usingAssociationList->append(r);
		}
		if(printRequested()){
			printCommitted("After Commit");
		}
	}
void
Resource::uncommit(Association * r)
	{
		assert(r);
		if(printRequested()){
			printCommitted("Before UnCommit");
		}
		if(m_usingAssociationList){
			if(m_usingAssociationList->contains(r)){
				m_usingAssociationList->remove(r);
				//if(m_usingAssociationList->entries()==0){
				//	theAssociationListStack.push(m_usingAssociationList);
				//	m_usingAssociationList=0;
				//}
			}
		}
		if(printRequested()){
			printCommitted("After UnCommit");
		}
	}


Vertex *
Resource::aliasPort( RWCString & port,Vertex * v)
	{
		if(vertex(port)){
			Error_Report("PORT already exists, can not be aliased "+port);
			return 0;
		} else if(v) {
			nodes()->insertKeyAndValue( port, v );
			return v;
		} else {
			Error_Report("NODE to be ALIASed does not exist ");
			return 0;
		}
	}

void
DebugDumpNodeDictionary(Resource * turtle )
	{
		VertexDictionaryIterator ndit( *( turtle->nodes() ) );
		
		cout << "Dumping:" << turtle->getName() << endl;
		while( ++ndit ){

			cout	<< " Node :" 
				<< ndit.key() 
				<< endl;
		}
	}


Vertex *
Resource::node(const RWCString & name)
	{
		Vertex * x;
		
		if( nodes()->findValue( name, x ) ){
			return x;
		}else{
			DebugDumpNodeDictionary( this );
			return 0;
		}
	}

Resource *
Resource::RenamePreface( RWCString & preface )
	{
		VertexDictionaryIterator ndit( *(nodes()) );
		
		while( ++ndit ){
			Vertex * v= ndit.value();
			nodes()->remove( ndit.key() );
			nodes()->insertKeyAndValue( preface+ndit.key(), v );
		}
		return this;
	}

Resource *
Resource::LinkControl( Resource * r, AST * model, AST * parameter, AST * value)
	{
		AST * label=0;
		m_callArgEntries->findValue(parameter->getName(),label);
		if(label){
			//cerr << label->getName()<< " WAS " << label->getDecimal() << endl;
			label->assign(value);
			//cerr << label->getName()<< " NOW " << label->getDecimal() << endl;
			return this;
		} else {
			Error_Report(parameter->getName() + " is not a known PARAMETER ",parameter);
		}
		return 0;
			
	}

RWCString
Resource::getName() const
	{
		return m_name;
	}

Capability *
Resource::findCapability( Capability * c )
	{
 		int	cap = 0;
 		CapabilityListIterator	ATECapability_it( m_capabilityList );
		
		while( cap != -1 ){
		
			if ( ++ATECapability_it ){
				cap = ATECapability_it.key() ->compare( *c );
			}else{
				return 0;
			}
		}
		return ATECapability_it.key();
	}

#include	"NodeName.h"
#include	"Vertex.h"
#include	"UutConnectorNode.h"
#include	"PointSourceNode.h"
#include	"InterfaceConnectorNode.h"
#include	"AdapterConnectorNode.h"
#include	"DevicePortNode.h"
#include	"SourceDevicePortNode.h"
#include	"SensorDevicePortNode.h"
#include	"LoadDevicePortNode.h"
#include	"DeviceReferencePortNode.h"
#include	"SwitchContactNode.h"
#include	"BusNode.h"
#include	"DevicePath.h"

Vertex *
Resource::AddNode(const NodeName & node , const NodeType nodeType )
	{
		Vertex * v=0;
		
		if( nodes()->findValue( node.getName(), v ) ){
			if(v->getNodeType()==nodeType){
				return v;
			} else {
				Error_Report("Attempt to redefine the node type for:"+node);
				return 0;
			}
			//assert(v->getNodeType()==nodeType);	// consistency check
		}else{
				
			switch(nodeType){
			case UutConnectorNodeType :
				{
					v = new UutConnectorNode ( node );
					break;
				}
			case PointSourceNodeType :
				{
					v = new PointSourceNode ( node );
					break;
				}
			case InterfaceConnectorNodeType :
				{
					v = new InterfaceConnectorNode ( node );
					break;
				}
			case AdapterConnectorNodeType :
				{
					v = new AdapterConnectorNode ( node );
					break;
				}
			case SourceDevicePortNodeType :
				{
					v = new SourceDevicePortNode ( node );
					break;
				}
			case SensorDevicePortNodeType :
				{
					v = new SensorDevicePortNode ( node );
					break;
				}
			case LoadDevicePortNodeType :
				{
					v = new LoadDevicePortNode ( node );
					break;
				}
			case DeviceReferencePortNodeType :
				{
					v = new DeviceReferencePortNode ( node );
					break;
				}
			case SwitchContactNodeType :
				{
					v = new SwitchContactNode ( node );
					break;
				}
			case BusNodeType :
				{
					v = new BusNode ( node );
					break;
				}
				
			default :
					assert(0);//new Vertex( node , nodeType );
					break;
			}
			nodes()->insertKeyAndValue( node.getName(), v );
			return  m_Circuit->AddNode( v );
		}
	}

Vertex *
Resource::AddNode(const RWCString & node ,const NodeType nodeType )
	{
		return AddNode( NodeName( node, this ) , nodeType );
	}
	

DevicePath *
Resource::addPath( DevicePath *dp )
	{
		if(!m_devicePathList){
			m_devicePathList=new DevicePathList;
		}
		
		m_devicePathList->append(dp);
		return dp;
	}

int Resource::setState(int, ReverseMap * ){return 0;}

AST *
Resource::RequirementsCheck( AST * a ){
	// Here we are passed a Virtual(Source/Sensor/???)
	// and asked to see if we can cover those requirements.

	ASTList  *	capabilities;
	//StringAST noun("~NOUN");
	AST *	matchNoun=0;
	
	// First we check to see if we got a NOUN coverage for the req...
	// AST * reqNoun = a->data(&noun);
	AST * reqNoun = a->data();


	// find the required capability by NOUN
	
	m_capabilityNounEntries.findValue(reqNoun->getName(),matchNoun);
	
		
	
	if(!matchNoun){
		if(m_ResourceDictionary){	// see if any subdevices cover..
			ResourceDictionaryIterator rdit(*m_ResourceDictionary);
			while(++rdit){
				AST * x;
				if(x=rdit.value()->RequirementsCheck(a)){
					return x;
				}
			}	
		}	
		//Error_Report(a->getName() + " does not cover ",a);
		return 0;
	} 
	// If we have NOUN coverage, then we try to match the characteristics...
	// down  is Measured Char Mnem..
	// right is the ATEDeviceFunction... Tell it to iterate
	AST * ateDeviceFunction = matchNoun->ASTright();
	AST *x = ateDeviceFunction->check(a) ;
	// don't forget that we might have gotten here recursively,
	// thus just in case tell the caller that we are the matched resource.
	if(x){
		a->setResource(this);				// update the Resource
		ateDeviceFunction->eval(a);	// set the State Machine
	}
	return x;
}

#include	"TAGContext.h"
#include	"Edge.h"
#include	"Wire.h"

Edge *
Resource::addWire(Vertex * source, Vertex * dest  )
	{
		Edge * edge1,* edge2;
		
		m_Circuit->AddEdge( source , dest , edge1=new Wire(this,source,dest));
		edge1->m_forward=1;
		m_Circuit->AddEdge( dest , source , edge2=new Wire(this,dest,source));
		edge2->m_forward=0;
		edge1->m_other=edge2;
		edge2->m_other=edge1;
		return edge1;
	}

#include	"ResistiveEdge.h"
#include	"CapacitiveEdge.h"
#include	"InductiveEdge.h"

Edge *
Resource::addTwoTerm(Vertex * source, Vertex * dest , AST * ast )
	{
		Edge * edge1,* edge2;

		DimensionEntry * oldDim=ast->getDimension();
		double x=ast->getDecimal();
		//if(oldDim&&(oldDim->scale!=0.0)){
		//		x=ast->ScaleDimension(x,oldDim,0);
		//}
		
		if((oldDim->quantity=="Impedance")||(oldDim->quantity=="Impedance_Real")){
			edge1=new ResistiveEdge(this,source,dest,x);
			edge2=new ResistiveEdge(this,dest,source,x);
		} else if(oldDim->quantity=="Inductance"){
			edge1=new InductiveEdge(this,source,dest,x);
			edge2=new InductiveEdge(this,dest,source,x);
		} else if(oldDim->quantity=="Capacitance"){
			edge1=new CapacitiveEdge(this,source,dest,x);
			edge2=new CapacitiveEdge(this,dest,source,x);
		} else {
			assert(0);
		}

		
		m_Circuit->AddEdge( source , dest , edge1);
		edge1->m_forward=1;
		m_Circuit->AddEdge( dest , source , edge2);
		edge2->m_forward=0;
		edge1->m_other=edge2;
		edge2->m_other=edge1;
		return edge1;
	}

void
Resource::insertContacts(int state,int pos,Edge * edge1,Edge * edge2)
	{
		assert(0);
	}
			

void
Resource::AddEdge(const RWCString & from,const RWCString & to,Edge * edge)
	{
		cout << " Say eh!!! what path is this ?? " << endl;
		assert(0);
	}

AST *
Resource:: createSensor(AST * nm)
	{
		AST * ast=new ATESensor(m_currentAnalogFSM = new Sensor( this ,nm) ) ;
		return ast;
	}

AST *
Resource:: createEventMonitor(AST * nm)
	{
		AST * ast=new ATEEventMonitor(m_currentAnalogFSM = new EventMonitor( this , nm ) ) ;
		return ast;
	}
	
AST *
Resource:: createSource()
	{
		AST * ast=new ATESource(m_currentAnalogFSM = new Source( this ) ) ;
		return ast;
	}
	
AST *
Resource:: createLoad()
	{
		AST * ast=new ATELoad(m_currentAnalogFSM = new Load( this ) ) ;
		return ast;
	}

AST *
Resource:: createTimer()
	{
		AST * ast=new ATETimer(m_currentAnalogFSM=0);
		return ast;
	}

AST *
Resource:: createBusProtocol(AST * name)
	{
		AST * ast=new ATEBusProtocol(m_currentDataBusFSM=new DataBus(this,name),name->getName());
		return ast;
	}

AST *
Resource:: theCallArg	(
					NounEntry * n,		ANTLRTokenPtr p,
					RWCString & param,	ModifierEntry * m,
					RWCString & type,	RWCString & usage,
					int get,		int put
				)
	{
		AST * label=0;
		if(!m){ // we do not keep the MODIFIERS. They come from ATLAS
			m_callArgEntries->findValue(param,label);
			if(label){
				return new TedlProxy(label,get,put);
			} else {
				label=new TedlArgumentLabel(p,get,put);
				AST * data = makeTypedData(type);
				label->setDown(data);
				m_callArgEntries->insertKeyAndValue(param,label);
				return label;
			}
		} else {
			return new TedlExternalLabel(n,p,m,get,put);
		}
	}

RWCString
Resource::theName()
	{
		RWCString n="";Resource * r=this;
		do{
			n=r->getName()+n;
		}while(r=r->getPrev());
		return n;
	}

int
Resource::resetResource(int softOrHard)
	{
		if(softOrHard==1){
			if(m_currentAnalogFSM){
				m_currentAnalogFSM->AsyncReset();
				m_currentAnalogFSM->REMOVE();
			}
			if(m_currentDataBusFSM){
				m_currentDataBusFSM->DISABLE_EXCHANGE();
			}
		}
		introduceFromEvent(0);
		introduceToEvent(0);
		introduceCnx(0);
		introduceMaxTime(0);
		introduceFromGate(0);
		introduceToGate(0);
		assert(!committed());

		// simulation...
		m_Voltage			= 0.0 ;

		return 0;
	}

int
Resource::resetResources(int softOrHard)
	{
		resetResource(softOrHard);
		ResourceListIterator	rlit( m_ResourceList );
		while( ++rlit ){
			Resource * turtle = rlit.key();
			turtle->resetResources(softOrHard);
		}
		return 0;
	}


CapabilityList	*
Resource::getCapabilities	()
	{
		return &m_capabilityList;
	}

void
Resource::insertCapability( Capability * c )
	{
		m_capabilityList.insert(c);
	}

void
Resource::addNamedCapability	( RWCString n,AST * c )
	{
		m_capabilityNamedEntries.insertKeyAndValue(n,c);
	}

AST *
Resource::getNamedCapability	( RWCString n)
	{
		AST * c=0;
		m_capabilityNamedEntries.findValue(n,c);
		return c;
	}

void
Resource::addNounCapability	( RWCString n,AST * c )
	{
		m_capabilityNounEntries.insertKeyAndValue(n,c);
	}
		

int
Resource::FindPath(Vertex   * Source,Vertex   * Dest , EdgeList & edgeList )
	{
		return m_Circuit->FindPath(Source,Dest,edgeList );
	}

int
Resource::connectToState(int state,Association * usingAssociation,EdgeList & 	edgeList)
	{
		assert(0);
		return 0;
	}

int
Resource::connSetToState(Set & state,Association * usingAssociation,EdgeList & 	edgeList)
	{
		assert(0);
		return 0;
	}

int
Resource::disconnToState(Association *	usingAssociation,Association & 	edgeList,int state)
	{
		assert(0);
		return 0;
	}


VertexDictionary *
Resource::nodes()
	{
		return m_nodeDictionary ;
	}


ResourceDictionary *
Resource::getResourceDictionary()
	{
		return 0;
	}


void
Resource::introduceFromEvent	(AST * e)
	{ m_FromEvent	=e;}
void
Resource::introduceToEvent	(AST * e)
	{ m_ToEvent	=e;}
void
Resource::introduceCnx		(AST * e)	
	{ m_Cnx		=e;}

void
Resource::introduceMaxTime	(AST * e)	
	{ m_MaxTime	=e;}

void
Resource::introduceFromGate	(AST * e)
	{ m_FromGate	=e;}
void
Resource::introduceToGate	(AST * e)
	{ m_ToGate	=e;}

AST*
Resource::getFromEvent		(AST * e)
	{ return m_FromEvent;}
AST*
Resource::getToEvent		(AST * e)
	{ return m_ToEvent;}
AST*
Resource::getCnx		(AST * e)
	{ return m_Cnx;}

AST*
Resource::getMaxTime		(AST * e)
	{ return m_MaxTime;}

AST*
Resource::getFromGate		(AST * e)
	{ return m_FromGate;}
AST*
Resource::getToGate		(AST * e)
	{ return m_ToGate;}

	

Vertex *
Resource::checkLoopThruConnections(Vertex * v,DFSContext & c)
	{
		return 0;
	}


void
Resource::createReverseMap(ReverseMap * rm){
		ResourceListIterator	rlit( m_ResourceList );
		while( ++rlit ){
			Resource * turtle = rlit.key();
			turtle->createReverseMap(rm);
		}	
}

int
Resource::SimulateCircuit(Vertex * v){
	return circuit()->SimulateCircuit(v);
}


int
Resource::setNounParameterValue(RWCString n,AST * d){
	// note: here we must make a test to see if
	// m_currentAnalogFSM is defined.
	// If it is , then we must defer to it. Otherwise we 
	// set the value here. Right now , we just set it. This is the
	// first day of this code.
	
	if(n=="VOLTAGE"){
		cout << " setting voltage (was) " << m_Voltage << endl ;
		m_Voltage = d->getDecimal();
		cout << " set voltage = " << m_Voltage  << endl ;
	} else {
		cout << " Not setting value: " << n << endl;
	}
	return 0;
}
