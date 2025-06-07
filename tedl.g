<<
	#include	"ATEResourceAST.h"
	#include	"DevicePath.h"
	#include	"VertexDictionary.h"
	#include	"DeviceModel.h"
	#include	"Device.h"
	#include	"CompositeDevice.h"
	#include	"ConfigurationModels.h"
	#include	"DeviceModels.h"
	#include	"AdaptationModels.h"
	#include	"ConfigurationModel.h"
	#include	"AdaptationModel.h"
	#include	"TPSHardware.h"

>>


class TedlParser{
<<

public:

//#include	"theEntries.h"
	
	
	Resource *	insertDeviceModel(Resource *);
	Resource *	getDeviceModel(AST *);
	Resource *	getDevice(AST *);		// called from ATLAS
	Resource *	getDevice(const std::string&);		// called from ATLAS
		
	Resource *	CurrentModel;
	Resource *	CurrentResource;
	Resource *	AdaptationModelsRoot;
	Resource *	ConfigurationModelsRoot;
	Resource *	TPSHardwareRoot;
	Resource *	DeviceModelsRoot;

	ResourceDictionary	deviceModelsDictionary;
	ResourceDictionary	realDevicesDictionary;
	ResourceDictionary *	CurrentResources;
	
	VertexDictionary		InterfaceConnectors;
	VertexDictionary		AdapterConnectors;
	VertexDictionary		UutConnectors;
	DevicePathDictionary		DevicePaths;
	
	ResourceList			activeResources;
	
	RWCString			m_modelfile;
	
	int removeAll(int softHard=0);
	
// Error reporting and Line info facilities

	virtual void syn(
				ANTLRAbstractToken	*	tok,
				ANTLRChar		*	egroup,
				SetWordType		*     	eset,
				ANTLRTokenType			etok,
				int 				k
			);
	virtual void edecode(SetWordType *a);

int ErrorFlag;

virtual void	init();

void set_error_flag();

void clear_error_flag();
        
int _error_();

void    Error_Report( RWCString E );

void    Error_Report( RWCString E, int l );

void    Error_Report( RWCString E, ANTLRTokenPtr t );

void    Error_Report( RWCString E, AST * a );

const RWCString& getLastError(void)const;
RWCString m_last_error;

private:

MnemonicsDB * mnemonicsDB;

NounEntry *
theNounEntry(const ANTLRTokenPtr  nid,int & howmany);


enum ProcessorMode { NoMode , DeviceModelMode , ConfigurationModelMode , AdaptationModelMode };

ProcessorMode processorMode ;

ModifierEntry * argModifier(NounEntry * n, ANTLRTokenPtr t );

>>

tedl_initialization![const RWCString & dbd]:
	<<

		mnemonicsDB= new MnemonicsDB(dbd+"Noun.db",dbd+"Dimension.db");
		//nounDictionary		= LoadAnalogNounDB("Noun.db",modifierDictionary);
		//quantityDictionary	= LoadDimensionDB("Dimension.db",allDimensions);
		TPSHardwareRoot		= new	TPSHardware		(RWCString(""),RWCString(""));
		AdaptationModelsRoot	= new	AdaptationModels	(TPSHardwareRoot);
		ConfigurationModelsRoot	= new	ConfigurationModels	(TPSHardwareRoot);
		DeviceModelsRoot	= new	DeviceModels		(TPSHardwareRoot);

		Vertex * v;
		v=ConfigurationModelsRoot->AddNode( "EARTH" , PointSourceNodeType );
		AdapterConnectors.insertKeyAndValue( "EARTH",v);

		v=ConfigurationModelsRoot->AddNode( "COMMON" , PointSourceNodeType );
		AdapterConnectors.insertKeyAndValue( "COMMON",v);	
	>>
	;
//A.1.9 Formal Syntax Definition (FSD)
// SYNTAX-RULES
tedl![const RWCString & modelfile]:    // l.0
	(
		<<m_modelfile=modelfile;>>
		(device_model)*
		(configuration_model)* 
		(adaptation_model)*
		Eof
   	)
	;
  
adaptation_model!:    // 2 . 0
	<< Resource * r; >>
	adaptation_model_header
	ate_identification
	uut_identification
	(
		device_identification
	) *
	(
		composite_device_identification
	) *
	( path )+
	{port_aliases}
	adaptation_model_end
	;
  
adaptation_model_header!:    // 2 .l
	ADAPTATION_MODEL
	<<
		CurrentResource  = AdaptationModelsRoot;
		CurrentResources = &realDevicesDictionary;
		processorMode    = AdaptationModelMode ;
	>>
		name
		VERSION  version_label stmt_term 
	;
	
ate_identification!:    // 2 .2
	ATE  name stmt_term
	;
	
uut_identification!:    // 2 .3
	UUT  name 
		VERSION  (  version_label (Fd version_label)* |  ALL   ) 
		stmt_term
	;
		
device_identification!:    // 2.4
	device_using_device_model
	stmt_term
	;

device_using_device_model!:
	<<
		Resource * resourceSave  = CurrentResource;
		Resource * resource = 0;
	>>
	DEVICE  local:name  USING  DEVICE_MODEL  external:name
		<<
			resource = getDeviceModel( #external );
			if ( !resource ){
				Error_Report( "Model is not defined: ", #external );
			}else if (sane()){
				if(CurrentResources->contains(#local->getName())){
					Error_Report( "DEVICE is already defined: ", #local );
				} else {
					resource = resource->instantiate( CurrentResource, #local->getName() );
					if( !resource ){
						Error_Report( "Can not instantiate "
							+	#local->getName()
							+	" from "
							+ 	#external->getName()
							,	#external
						);
					}else{
						CurrentResources->insertKeyAndValue( #local->getName(), resource );
						CurrentResource = resource;
					}
				}
			} else {
				Error_Report( "DEVICE is skipped: ", #local );
			}
				
			delete #local; delete #external;
		>>
	{
		port_assign
	}
	control_interface
	<<	extern ReverseMap * reverseMap;
		CurrentResource->createReverseMap(reverseMap);
		CurrentResource = resourceSave;
	>>
	;	
	<< CurrentResource = resourceSave; >>







port_assign!:    // 2 .4.l
		
	(
		(
		PORT  to:device_port_name  IS  from:device_model_port_name
			
			(	port:name
				<<
					Resource * r;
					RWCString From(#from->getName());RWCString To(#to->getName());
					RWCString Port(#port->getName());
					if( CurrentResources->findValue( From,r ) ){ // alias
						if(!CurrentResource->aliasPort( To, r->vertex(Port) )){
							Error_Report( "ALIAS error: ", #port );
						}
					}
					delete #port;
				>>

			|	()
				<<
					RWCString From(#from->getName());RWCString To(#to->getName());
					if(!CurrentResource->aliasPort( To, CurrentResource->vertex(From) )){
						Error_Report( "ALIAS error: ", #to );
					}
				>>
			)
			<< delete #to; delete #from; >>
		)+
	|	 PORT  RENAME_PREFACE  rp:name
			<<
				CurrentResource->RenamePreface(#rp->getName());
				delete #rp;
			>>
	)
	;
  
control_interface!:    // 2.4.2
	CONTROL_INTERFACE  NONE
	|	( cont_interface_info ) +
	|	( cont_parameter_info ) *
	;
  
cont_interface_info!:    // 2.4.2A
	<<
		DimensionEntry *	dimensionEntry = 0;
	>>
	CONTROL_INTERFACE  bi:bus_info  {  va:name  }
	
		{
			TIME_OUT  dn:decimal_number  td:time_dim[dimensionEntry]
			<< #( #dn, #td );>>
		}
		ADDRESS  a1:address	<< #( #va, #a1 ); >>	// Trick 
			(
				Fd an:address	<< ( 0, #a1, #an ); >>
			)*
		<<
			CurrentResource->LinkControl( CurrentResource, #bi, #va?#va:#a1, #dn );
		>>
	; 


cont_parameter_info!:    // Extension
	<<
		DimensionEntry *	dimensionEntry = 0;
	>>
	CONTROL_PARAMETER  { AtSign  model:name } parameter:name
		{
			TIME_OUT  dn:decimal_number  td:time_dim[dimensionEntry]
			//<< #( #dn, #td ); >>
		}
		VALUE  value:address
		<<
			CurrentResource->LinkControl( CurrentResource, #model,#parameter, #value );
			delete #model;delete #parameter;delete #value;
		>>
	; 

time_dim![DimensionEntry *& dimensionEntry]:	// sik, included from atlas
	<< DimensionDictionary * dimensionDictionary = mnemonicsDB->theDimensionDictionary( "Time" ); >>
	<< dimensionEntry = mnemonicsDB->theDimensionEntry( LT(1)->getText(), dimensionDictionary ) >>?
		[ Error_Report( "Time dimension expected:", LT(1) ); ]
	
	eid:EID		<< #0 = new DimensionType( $eid, dimensionEntry ); >>
	;

bus_info!:    // 2.4.2B
	<<
		DimensionEntry * dimensionEntry=0;
	>>
	(
		bus_name
	|	GPIB {  DELAY  decimal_number  time_dim[dimensionEntry] }
	|	IEEE_488p1
	|	IEEE_488p2
	|	RS232
	|	ETHERNET
	|	(
			VXI1p2
		|	VXI1p3
		|	VXI1p4
		)
		
   		(
   			MESSAGE_BASED
   		|	REGISTER_BASED
   		)
   	)
	;
  
composite_device_identification! :    // 2 . 5
	<<
		Resource * resource = 0;
		Resource * resourceSave	=	CurrentResource;
		ResourceDictionary * dictionarySave	=	CurrentResources;
	>>
	COMPOSITE_DEVICE  local:name USING DEVICE_MODEL  external:name 
		<<
			resource = getDeviceModel( #external );
			if ( !resource ){
				Error_Report( "Model is not defined: ", #external );
			}else{
				if(CurrentResources->contains(#local->getName())){
					Error_Report( "DEVICE is already defined : ", #local );
				}
					
				resource = new CompositeDevice(#local->getName(),resource );
				if( !resource ){
						Error_Report( "Can not instantiate "
							+	#local->getName()
							+	" from "
							+ 	#external->getName()
							,	#external
						);
				}else{
					CurrentResources->insertKeyAndValue( #local->getName(), resource );
					CurrentResource = resource;
					CurrentResources=resource->getResourceDictionary();
				}
			}
			delete #local; delete #external;
		>>
	NEED_DEVICE (device_using_device_model)+
	//( define_wire )*
	NEED_PATH  ( define_path )+
	ASSIGN port_assign stmt_term
	<< CurrentResource = resourceSave;CurrentResources = dictionarySave; >>
	;
	<< CurrentResource = resourceSave;CurrentResources = dictionarySave; >>
  
define_path!:    // 2 . 5. 1
	<< Vertex * vfrom=0, * vto=0; PathNodeList * pathNodeList=0; >>
	(
		PATH  { pn:name } CONNECTS
			connection_point_info>[vfrom]
		<<
			pathNodeList=new PathNodeList;
			pathNodeList->append(new PathNode(CurrentResource,vfrom));
		>>
		( {sc:switch_command }
			TO  define_path_info>[vto]
			<< pathNodeList->append(new PathNode(CurrentResource,vto,#sc)); >>
		)+
		{ pc:path_characteristics }
		<<
			RWCString dpn=#pn?#pn->getName():RWCString("");
			DevicePath * dp=new DevicePath(NodeName(dpn,CurrentResource),pathNodeList,#pc);
			dp = CurrentResource->addPath(dp);
			if(#pn){
				DevicePath * dp=0;
				if(!DevicePaths.findValue(#pn->getName(),dp)){
					DevicePaths.insertKeyAndValue(#pn->getName(),dp);
				} else {
					Error_Report("DUPLICATE PATH Definition",#pn);
				}
				delete #pn;
			}
		>>
	)
	|	define_wire
	|	define_resistor
	;

define_wire!:    // 2 . 5. 1
	<< Vertex * vfrom=0, * vto=0; Edge * e;>>
	(
		WIRE	connection_point_info>[vfrom]
		(	TO  define_path_info>[vto]
				<<
					if(vfrom && vto){
						CurrentResource->addWire(vfrom,vto);
					}
				>>
		)+
	)
	;
	
define_resistor!:    // 2 . 5. 1
	<< 	Vertex * vfrom=0, * vto=0; Edge * e;
		int howmany=0;
		NounEntry	* nounEntry;
		ModifierEntry	* modifierEntry;
		DimensionEntry	* dimensionEntry;
	>>
	(
		TWOTERM
			connection_point_info>[vfrom]
		(	TO  define_path_info>[vto]
			
				<<
					nounEntry=mnemonicsDB->theNounEntry("IMPEDANCE");
				>>
				noun_modifier[nounEntry]>[modifierEntry]
					dn:dimensioned_decimal_number[modifierEntry,dimensionEntry]
				<<
					if(vfrom && vto){
						CurrentResource->addTwoTerm(vfrom,vto,#dn);
					}
				>>
		)
	)
	;
	
connection_point_info!>[Vertex * v]:
	<< Resource * r=0; DevicePath * dp=0;>>
	(
		PATH
			(	pf:name
				<<
					if(!DevicePaths.findValue(#pf->getName(),dp)){
						Error_Report(" PATH not found ",#pf);
					} else {
						$v=dp;
					}
					delete #pf;
				>>
			)
	|	DEVICE 
			(	dn:name  dpn:device_port_name
				<<
					if( !CurrentResources->findValue( #dn->getName(), r ) ){
						Error_Report(" DEVICE not found ",#dn);
					}else if( ! ( $v = r->node( #dpn->getName() ) ) ){
						Error_Report(" DEVICE PORT not found:"+#dn->getName(),#dpn);
					}
					delete #dn;delete #dpn;			
				>>
			)
        |       iC:INTERFACE_CONNECTOR
                        (       ic:device_port_name
                                <<
                                        if( !InterfaceConnectors.findValue( #ic->getName(), $v )){
                                                if( AdapterConnectors.findValue( #ic->getName(), $v )){
                                                        Error_Report("Node is ADAPTER_CONNECTOR :",#ic);
                                                }
                                                $v = CurrentResource->AddNode( #ic->getName() , InterfaceConnectorNodeType );
                                                InterfaceConnectors.insertKeyAndValue( #ic->getName(), $v );
                                        }
                                        delete #ic;
                                >>
                        )
	|	aC:ADAPTER_CONNECTOR 
			(	ac:device_port_name
				<<
					if( !AdapterConnectors.findValue( #ac->getName(), $v )){
                                                if( InterfaceConnectors.findValue( #ac->getName(), $v )){
                                                        Error_Report("Node is INTERFACE_CONNECTOR :",#ac);
                                                }
						$v = CurrentResource->AddNode( #ac->getName() , AdapterConnectorNodeType );
						AdapterConnectors.insertKeyAndValue( #ac->getName(), $v );
					}
					delete #ic;
				>>
			)
	|	uC:UUT_CONNECTOR
			(	uc:device_port_name
				<<
					if( processorMode == AdaptationModelMode ){
						if( !UutConnectors.findValue( #uc->getName(), $v )){
							if(!($v = CurrentResource->AddNode( #uc->getName() , UutConnectorNodeType ))){
								Error_Report("Inconsistent Node specification:",#uc);
							} else {
								UutConnectors.insertKeyAndValue( #uc->getName(), $v );
							}
						}
					}else{
						Error_Report( "UUT_CONNECTOR is not allowed here:", $uC );
					}
					delete #uc;
				>>
			)
	)
	;

define_path_info!>[Vertex * v]:    // 2. 5. 1A
	(
		connection_point_info>[$v]
	|	earth:EARTH
		<<
			if( !AdapterConnectors.findValue( "EARTH", $v ) ){
				Error_Report( "EARTH has not been defined ", $earth );
			}
		>>
	|	common:COMMON 
		<<
			if( !AdapterConnectors.findValue( "COMMON", $v )){
				Error_Report( "COMMON ihas not been defined ", $common );
			}
		>>
	)
	;
  
switch_command :    // 2. 5. 1. 1
	Open_Quote Characters Semi_Colon  Characters Close_Quote
	;
   
  
path_characteristics!:    // 2 . 5.2
	<<
		ModifierEntry	* modifierEntry	=0;
		DimensionEntry	* dimensionEntry=0;
	>>
	 nm1:noun_modifier[0]>[modifierEntry]  pci1:path_char_info[modifierEntry,dimensionEntry]
	 	<< #0=#(#nm1,#pci1); >>
	 	( 
	 		Fd nmn:noun_modifier[0]>[modifierEntry]  pcin:path_char_info[modifierEntry,dimensionEntry]
	 		<< #0=#(0,#0,#(#nmn,#pcin)); >>
	 	)* 
	;
  
path_char_info![ModifierEntry * modifierEntry,DimensionEntry * & dimensionEntry] :    // 2.5.2A
		rc:real_constraint[0,modifierEntry]>[dimensionEntry] << #0=#rc; >>
		{
			re:errlim[0,$dimensionEntry]
			<<
				#0 = #( new RealErrlim, #0, #re );
			>>
		} 
		//<<
		//	#0 = #( #nm, #0 );
		//>>
	;
  
path! :    // 2 . 6
	define_path stmt_term 
	;
wire! :
	define_wire stmt_term 
	;

  
port_aliases! :    // 2 .7
	PORT_ALIASES  
   		port_alias
	stmt_term 
	;

port_alias! :		// sik added
	{ name  }  PORT  name  IS  name   device_port_name
		{ Fd port_alias}
	;
	
adaptation_model_end! :    // 2 . 8
	END  ADAPTATION_MODEL  name stmt_term
		<<
			processorMode = NoMode ;
		>>
	;
   
configuration_model! :    // 3. 0
	<< Resource * r; >>
	configuration_model_header
		(
			device_identification
		)+ 
		(
			composite_device_identification
		)*
		( path )*
	configuration_model_end 
	;
  
configuration_model_header! :    // 3. 1
	CONFIGURATION_MODEL
	<<
		CurrentResource  =  ConfigurationModelsRoot;
		CurrentResources = &realDevicesDictionary;
		processorMode    =  ConfigurationModelMode ;
	>>
	
	  n:name  VERSION  vl:version_label stmt_term
		<<
			CurrentResource = new Resource( CurrentResource, #n->getName(), #vl->getName() );
			delete #n; delete #vl;
		>>
	;
  
configuration_model_end! :    // 3 .2
	END  CONFIGURATION_MODEL  name stmt_term
		<<
			CurrentResource = CurrentResource->getPrev();
			processorMode = NoMode ;
		>>
	;
  
device_model! :    // 4 . 0
	device_model_header
	( extend  )*
	( capability_block )*
	device_model_end
	;
   
device_model_header! :    // 4.1
	DEVICE_MODEL
	<<
		CurrentResources = &deviceModelsDictionary;
		processorMode    = DeviceModelMode ;
	>>
	n:name  VERSION  vl:version_label stmt_term
		<<
			CurrentModel    = insertDeviceModel( new DeviceModel( #n->getName(), #vl->getName() ) );
			CurrentResource = CurrentModel;
			delete #n; delete #vl;
		>>
	;

extend !:    // 4.2
	EXTEND 
		(
			NOUN		nn:name
			
		|	MODIFIER	mn:name
		|	DIMENSION	dm:name
		|	CONNECTIONS	cn:name
		)   stmt_term
	;

capability_block! :	// 4.3
	<<
		ASTList		* capabilityList 		=0;
		ASTList		* 	prepCapability 		= 0;
		ASTList		*	prepControl 		= 0;
		NounEntry	*	nounEntry		= 0;
		int			prepCapabilityUsed	= 0;		
		int			prepControlUsed		= 0;
		AST	*cont	= 0,	*pcap	= 0,	*pcon	= 0;
	>>
	CAPABILITY
	<<	
		capabilityList	=	new ASTList;
		prepCapability	=	new ASTList;
		prepControl	=	new ASTList;
		cont		=	new ATECapability( capabilityList );
		pcap		=	new ATECapability( prepCapability );
		pcon		=	new ATECapability( prepControl    );
	>>
	
	{	n:name
	  	<<
	  		CurrentResource->addNamedCapability(#n->getName(), cont );
	  		delete #n;
	  	>>
	}
	(	
		(
			<<
				if(prepCapabilityUsed){
					prepCapability	=	new ASTList;
					pcap=new ATECapability( prepCapability );
					prepCapabilityUsed=0;
				}
				if(prepControlUsed){
					prepControl	=	new ASTList;
					pcon=new ATECapability( prepControl    );
					prepControlUsed=0;
				}
			>>
					
			fst:function_stmt[nounEntry,prepCapability,prepControl]
				<<
					capabilityList->insert( #fst );
				>>
			{	cap:resource_preparation_capability
					<<
						prepCapabilityUsed=1;
						prepCapability->insert( #cap );
					>>
			}
			{	con:resource_preparation_control[nounEntry]
					<<
						prepControlUsed=1;
						prepControl->insert( #con );
					>>
			}
		)+
	)
	END  CAPABILITY stmt_term
		<<
				if(!prepCapabilityUsed){
					delete pcap;
					delete prepCapability;
				}
				if(!prepControlUsed){
					delete pcon;
					delete prepControl;
				}
		>>
	;
  
function_stmt![NounEntry	*& nounEntry,ASTList * pcap,ASTList * pcon ]:	// 4.3.1
	<<
		//NounEntry 	* nounEntry	=0;
		SwitchModel	* sm=0; int num	=0;
		ASTList		* sigCharsList	=0;
		ASTList		* prepCon	=0;
		ASTList		* prepCap	=0;
		AST		* n		=0;
		AST		* sig_char	=0;
		AST		* stateMachine	=0;
		NodeType	nodeType	=UndefinedNodeType;
		int		matrix		=0;
	>>
	fnc:FUNCTION 
	(
		swi:switch_info>[sm,num,matrix]
		<<
			if(sane()){	
				CurrentResource = CurrentModel->AddResource( sm ) ;	
			}
		>>
		cdi:conn_disconn_info[num,matrix]		<< #0 = #( new SwitchFunction , #cdi ); >>
	|
		essi:event_stim_sens_info>[nounEntry,nodeType,stateMachine]
		<<	sigCharsList = new ASTList;>>
		(
			sci:sig_char_info[nounEntry,nodeType]	
			<< sigCharsList->insert(#sci); >>
			{ rpcon:resource_preparation_control[nounEntry] }
			<<#(0,#sci,#(new Proxy,#rpcon));>>

		)+
		<<
			#0 = #(	0
				,#essi
				,new ATEDeviceFunction (stateMachine,sigCharsList)
				,new ATEDeviceFunction (stateMachine,pcon)
				,new ATEDeviceFunction (stateMachine,pcap)
			);
			CurrentResource->addNounCapability(#essi->getName(), #essi );
		>>
	|
		bpi:bus_protocol_info>[nodeType,stateMachine]
		<<	sigCharsList = new ASTList;	>>
		( dd:databus_definition		<< 	sigCharsList->insert(#dd);	>> )+
		<<
			#0 = #( 0 , #bpi , new ATEDeviceFunction (stateMachine,sigCharsList));
			CurrentResource->addNounCapability(#bpi->getName(), #bpi );
		>>
	)
	stmt_term
	;

// additions for DATA BUS

bus_protocol_info!>[NodeType nodeType,AST * stateMachine] :
	BUSdPROTOCOL	SPEC spec:ID Fd
			<<
				AST * name=new StringAST($spec->getText());
				$stateMachine=CurrentResource->createBusProtocol(name) ;
				$nodeType= BusNodeType ;
				#0=new StringAST($spec->getText());
			>>
	;
	
databus_definition!:                             // fsd-- 6.11A
	<<
		NounEntry 	* nounEntry=0;
		NodeType	nodeType=BusNodeType;
	>>
	bip:bus_installation_parameters Fd
	<<
		#0=#bip;
	>>
	pac:ports_and_control[nounEntry,nodeType]
	<< #0  = #( 0, #0,#pac); >>
	;
  
  
 
bus_installation_parameters!:                    // fsd-- 6.11C
	<< ASTList * csdList = 0;ASTList * emList=0; >>
	tebr:test_equip_bus_role 
	{
		Fd TESTdEQUIPdMONITOR
		<< csdList=new ASTList; >>
		csd1:command_status_data
			<< csdList->insert(#csd1); >>
			(
				Fd csdn:command_status_data
				<< csdList->insert(#csdn); >>
			)*
	}
	<< emList=new ASTList; >>
	( Fd em:exchange_model << emList->insert(#em); >> )+
	{ Fd ebp:establish_bus_parameters }
	{ Fd epp:establish_protocol_parameters }
	<<
		#0=	#(0,	#(#tebr,			new TestEquipMonitor(csdList)));
		#0=	#(0,#0,	#(new ExchangeModels(emList)));
		#0=	#(0,#0,	#(new BusParameters,		#ebp));
		#0=	#(0,#0,	#(new ProtocolParameters,	#epp));
	>>
	;

test_equip_bus_role!:                            // fsd-- 6.11D
	<< ASTList * mmsList=0; >>
	TESTdEQUIPdROLE
	mms1:monitor_master_slave
			<< (mmsList=new ASTList)->insert(#mms1); >>
	(
		Fd mmsn:monitor_master_slave
			<< mmsList->insert(#mmsn); >>
	)*
	<< #0=new TestEquipBusRole(mmsList); >>
	;

command_status_data:
	(
		COMMAND
	|	STATUS
	|	DATA
	)
	;
monitor_master_slave:
		test_equip_role_name
	;    

test_equip_role_name!:                           // fsd-- 15.10.15
	<< RWCString left;ModifierEntry * modifierEntry=0; >>
	<< modifierEntry=mnemonicsDB->theModifierEntry(LT(1)->getText(),"~TEST-EQUIP-ROLE",left)>>?
		[Error_Report(RWCString("Not a TEST-EQUIP-ROLE Name :"),LT(1));]
		m:ModifierCLASS
	<<
		if(!(left=="")){ Error_Report("TEST-EQUIP-ROLE ",$m);}
		#0 = new TestEquipRoleName($m,modifierEntry);
	>>
	;

exchange_model!:                                 // fsd-- 6.11E
	<< ASTList * talkers=0;ASTList * listeners=0; >>
	BUSdMODE bmn:bus_mode_name
	{	Fd TALKER
			<< talkers=new ASTList; >>
			t:establish_device
			<< talkers->append(#t); >>
	} 
	{	Fd LISTENER
			<<listeners=new ASTList;>>
			(	l:establish_device
				<< listeners->append(#l); >>
			)+
	}
	<<
		#0=#(new TedlDataBusDevices,new TedlTalker(talkers),new TedlListener(listeners));
		#0=#(#bmn,#0);
	>>
	;
  
   

establish_device!:				// fsd-- 6.11F
	(
		ee : name
			<<#0=#(new DeviceIdentifierLabel(#ee->getToken()),#(new Proxy,#ee));>>
	|	teq:TESTdEQUIP
			<<#0=new DeviceIdentifierLabel($teq);>>
	|	uut:UUT  
			<<#0=new DeviceIdentifierLabel($uut);>>
	)	
	;
  
  
establish_bus_parameters:                       // fsd-- 6.11G
	BUSdPARAMETER 
	establish_bus_parameter (Fd establish_bus_parameter)*
	;
        
establish_bus_parameter:
	{ fetch_update_spec }
	bus_parameter_range 
	;
       
fetch_update_spec:                              // fsd-- 6.11I
	( 
		UPDATABLEdFETCHABLE
	|	FETCHABLE
	|	UPDATABLE  
	)
	(  
		PROTOCOLdEXCHANGE
	|	PROTOCOL
	|	EXCHANGE    
	)
	;
 

bus_parameter_range:                           // fsd-- 6.11H
	<<
		ModifierEntry * modifierEntry=0;
	>>
	bpn:bus_parameter_name>[modifierEntry]
	{ bpvr:bus_parameter_value_range[modifierEntry] }
	<<#0=#(#bpn,#bpvr);>>
	; 

bus_parameter_name!>[ModifierEntry * modifierEntry]:                             // fsd-- 15.10.17
	<< RWCString left;>>
	<< $modifierEntry=mnemonicsDB->theModifierEntry(LT(1)->getText(),"~BUS-PARAMETER",left)>>?
		[Error_Report(RWCString("Not a BUS-PARAMETER Name :"),LT(1));]
		m:ModifierCLASS
	<<
		if(!(left=="")){ Error_Report("BAD BUS-PARAMETER ",$m);}
		#0 = new BusParameterName($m,$modifierEntry);
	>>
	;
 
  
bus_parameter_value_range![ModifierEntry * &modifierEntry]:                      // fsd-- 6.11J
	<<
		DimensionEntry * dimensionEntry=0;
	>>
	rc:real_constraint[0,modifierEntry]>[dimensionEntry]
		<<
			#0 = #rc;
		>>
	;

//	Bogus.. Examples allow negative numbers.....
//dimensioned_constant[ModifierEntry * &modifierEntry,DimensionEntry * & dimensionEntry]:                            // fsd-- 6.11K
//		decimal_constant dim[modifierEntry,dimensionEntry]
//	;
 
 
establish_protocol_parameters:                   // fsd-- 6.11L
	PROTOCOLdPARAMETER
		establish_protocol_parameter (Fd establish_protocol_parameter)*
	;
  
establish_protocol_parameter:
	{ fetch_update_spec }
	protocol_parameter_types 
	; 
        
  
protocol_parameter_types:                        // fsd-- 6.11M
	( 
		COMMANDdWORD                 |
		STATUSdWORD                  |
		DATAdWORD  
	)
	( 
		PULSEdCODE                     |
		PARITY                         |
		LENGTH                         |
		SYNC                           |
		TdR 
	) 
	|
	( 
		WORDdCOUNT 
	) 
	;


bus_mode_name!:                                  // fsd-- 15.10.16
	<< RWCString left;ModifierEntry * modifierEntry=0; >>
	<< modifierEntry=mnemonicsDB->theModifierEntry(LT(1)->getText(),"~BUS-MODE",left)>>?
		[Error_Report(RWCString("Not a BUS-MODE Name :"),LT(1));]
		m:ModifierCLASS
	<<
		if(!(left=="")){ Error_Report("BAD BUS-MODE ",$m);}
		#0 = new BusModeName($m,modifierEntry);
	>>
	;

// end of DATA BUS devices

switch_info!>[SwitchModel * sm,int num,int matrix] :    // 4.3.1A
	SWITCH
	(
		switch_type>[$sm,$matrix] << $num=1; >>   {switch_width>[$num]}
	|	<<
			$sm=new MultiplexSwitch	( CurrentModel , RWCString("") );
			$num=1;
		>>
	)
	;
	
switch_type!>[SwitchModel * sm,int matrix]:
		MATRIX		<< $sm=new MatrixSwitch		( CurrentModel , RWCString("") ); $matrix=1; >>
	|	GANG		<< $sm=new GangSwitch		( CurrentModel , RWCString("") ); >>
	|	MULTIPLEX	<< $sm=new MultiplexSwitch	( CurrentModel , RWCString("") ); >>
	|	SPST		<< $sm=new SpstSwitch		( CurrentModel , RWCString("") ); >>
	;

switch_width!>[int num]:
		Zz1_WIRE	<< $num=1;>>
	|	Zz2_WIRE	<< $num=2;>>
	|	Zz3_WIRE	<< $num=3;>>
	|	Zz4_WIRE	<< $num=4;>>
	;
	
conn_disconn_info![int num,int matrix]:    // 4,3.1B
	(	<<StringVector FromList, ToList;int state=0,dstate=-1;Resource * relay;>>

		(
			CONNECT_PORT	fnm1:device_port_name << FromList.push_back( #fnm1->getName() );delete #fnm1;>>
			 	( Fd  	fnmn:device_port_name << FromList.push_back( #fnmn->getName() );delete #fnmn;>> )*
			
			TO_PORT		tnm1:device_port_name << ToList.push_back( #tnm1->getName() );delete #tnm1;>>
				( Fd	tnmn:device_port_name << ToList.push_back( #tnmn->getName() );delete #tnmn;>> )*
			
			{
				dc:DEFAULT
				<< 
					if(dstate>=0){
						Error_Report( "DEFAULT was already defined.", $dc );
					} else {
						dstate=state;
					}
				>>
				
			} cntrl:control_sequence[0]
			<<
				if(matrix){
					relay = CurrentResource->AddResource( new GangSwitch(CurrentResource,RWCString("")));
				} else {
					relay = CurrentResource;
				}
				if ( FromList.size() == ToList.size() ){
					relay->addContacts(state++,dstate,FromList, ToList, #cntrl );
				}else{
					Error_Report( "FROM & TO Nodes are not equinumerous in CONNECT", #cntrl );
				}
				FromList.clear();ToList.clear();
			>>
			{
				DISCONNECT_COMMAND
				{
					dd:DEFAULT
						<< 
							if(dstate>=0){
								Error_Report( "DEFAULT was already defined.", $dd );
							} else {
								dstate=state;
							}
						>>
				}
				dcntrl:control_sequence[0]  
				<<
					if(dstate==-1){
						dstate=state;
					}
					relay->addContacts(state++,dstate, FromList, ToList, #dcntrl );
					state=0;dstate=-1;
				>>
			}

		//|	CONNECT_ALGORITHM  Open_Quote connect_algorithm   Close_Quote
		//|	DISCONNECT_CHANGE  Open_Quote connect_mnemonic    Close_Quote  
		//	TO		   Open_Quote disconnect_mnemonic Close_Quote 
		)+
	)
	;
  
event_stim_sens_info!>[NounEntry * nounEntry,NodeType nodeType,AST * stateMachine] :    // 4.3. lC
	<<
		ModifierEntry	*	modifierEntry	= 0 ;
		NounEntry	*	nounEntry	= 0 ;
	>>
		(
			(
				sns:SENSOR		LP snm:noun_modifier[nounEntry]>[modifierEntry] RP
						<<
							$stateMachine=CurrentResource->createSensor(#snm) ;
							$nodeType= SensorDevicePortNodeType ;
						>>
			|	evm:EVENT_MONITOR	LP enm:noun_modifier[nounEntry]>[modifierEntry] RP
						<<
							$stateMachine=CurrentResource->createEventMonitor(#enm) ;
							$nodeType= SensorDevicePortNodeType ;
						>>
			|	src:SOURCE	
						<<
							$stateMachine=CurrentResource->createSource();
							$nodeType= SourceDevicePortNodeType ;
						>>
			|	lod:LOAD
						<<
							$stateMachine=CurrentResource->createLoad();
							$nodeType= LoadDevicePortNodeType ;
						>>
			|	tmr:TIMER
						<<
							$stateMachine=CurrentResource->createTimer();
							$nodeType= UndefinedNodeType ;
						>>
			)
			n:NOUN  ne:noun[nounEntry]>[$nounEntry]
				<<
					#0 = #(#ne ,new ModifierType($n,modifierEntry));
				>>
		)?	// we force double scan, and pick up the nounEntry first time thru
		|
		( STIM | RESP | STIM_RESP )  DIGITAL_TEST
		;
  
sig_char_info![NounEntry * nounEntry,NodeType & nodeType]:    // 4.3. lD
	<<
		ASTList * cnxList=0;
		ASTList * sigChars=0;
		AST	* sigs=0;
	>>
	
	sig:SIG_CHAR	<< sigChars=new ASTList; >>
		{sc:signal_characteristics[nounEntry,sigChars]}
		<<
			sigs=new ModifierListType( sigChars );
			CurrentResource->insertCapability( new Capability( sigs) );
		>>
		pac:ports_and_control[nounEntry,nodeType]
		<< #0  = #( new Proxy($sig) , sigs,#pac); >>
	;
	
	
ports_and_control![NounEntry * nounEntry,NodeType & nodeType]:
	<< 
		ASTList * cnxList=0;
	>>
	{	
		AT << cnxList = new ASTList; >>
			(
				n:device_port_name
				<<
					NodeType nt;
////					int l=#n->getName().length();
////					if((l>4) && (#n->getName().index("_REF")==(l-4))){
////						nt = DeviceReferencePortNodeType ;
////					} else {
////						nt = nodeType;
////					}
						std::string name = #n->getName();
						if (ends_with(name, "_REF")) {
						    nt = DeviceReferencePortNodeType;
						} else {
						    nt = nodeType;
						}
					CurrentResource->AddNode( #n->getName() , nt );
					cnxList->insert( new ConnectionType( #n->getToken() ) );
					delete #n;
				>>
			)+
	}

	<< #0  = new CnxList( cnxList ) ; >>
	
	{
		cb:control_block[nounEntry]
	}
	<< #0  = #(0,#0,#(new ControlBlock , #cb ) ); >>
	;

signal_characteristics![NounEntry * nounEntry,ASTList *& sigChars]:    // 4.3.1.1
	<< AST * sig=0; >>
	(
			sigi:sig_info[nounEntry]	<<	#( sig=new SigChar, #sigi );	>>
		|
			sync:sync_info[nounEntry]	<<	#( sig=new SigChar, #sync );	>>
	)
	<<
		sigChars->insert(sig);
	>>
	{ Fd signal_characteristics[nounEntry,sigChars] }
	;

sig_info![NounEntry * nounEntry] : // 4.3.1.1A
	<<
		ModifierEntry	* modifierEntry = 0;
		DimensionEntry	* dimensionEntry = 0;
	>>
	nm:noun_modifier[nounEntry]>[modifierEntry]  
	{
		LIMIT
			(
				MAX
					<< #0=new SelectLimitMax; >>
			|	MIN
					<< #0=new SelectLimitMin; >>
			|
				<<
					if(LA(1)==MIN){
						#0=new SelectLimitMin;
					} else {
						#0=new SelectLimitMax;
					}
				>>
			)
	}
	rc:real_constraint[nounEntry,modifierEntry]>[dimensionEntry]	<< #0=#(#0,#rc); >>
		{
			re:errlim[nounEntry,dimensionEntry]
			<<
				#0 = #(0,#0, #(new RealErrlim, #re ));
			>>
		} 
		<<
			#0 = #( #nm, #0 );
		>>
	;
	
real_constraint![NounEntry * nounEntry, ModifierEntry * modifierEntry]>[DimensionEntry * dimensionEntry]:
	(
		MAX  maxdn:dimensioned_decimal_number[modifierEntry,$dimensionEntry]
			<<
				#0 = #( new ConstraintMax, #maxdn );
			>>
				
	|	MIN  mindn:dimensioned_decimal_number[modifierEntry,$dimensionEntry]
			<<
				#0 = #( new ConstraintMin, #mindn );
			>>

	|	ddnr:dimensioned_decimal_number_range[modifierEntry,$dimensionEntry]
			<< #0 = #ddnr; >>
		{
			BY bydn:dimensioned_decimal_number[modifierEntry,$dimensionEntry]
			<<
				#0 = #( new ByQuantization, #0, #bydn );
			>>
		}
	)
	;
  
sync_info![NounEntry * nounEntry] :    // 4. 3. 1. 1.B
	<<
		ModifierEntry	* modifierEntry	= 0;
		DimensionEntry	* dimensionEntry= 0;
	>>
	sync:SYNC 
	(
		nm:noun_modifier[nounEntry]>[modifierEntry]
			(	gt:GTcomp	<< #0=#(new GTOperator($gt)); >>
			|	lt:LTcomp	<< #0=#(new LTOperator($lt)); >>
			)  
			ddn:dimensioned_decimal_number_range[modifierEntry,dimensionEntry]
				<< #0=#(#0,#nm,#ddn); >>
			
			(	NEG_SLOPE
				<< #0=#(new EventSlopeNegField,#0); >>
			|	POS_SLOPE
				<< #0=#(new EventSlopePosField,#0); >>
			)
	|	(
			WORD_TRANSITION
		|	BIT_TRANSITION
		)
	)  TO  n:name	<< #0=#(new ATESyncField,#0,#n); >>
	;
  
control_block![NounEntry * nounEntry]:    // 4 . 3. 1.2
		<< ASTList * actions ; >>
		CONTROL
			<< actions = new ASTList; >>
			cbi:control_block_info[ actions , nounEntry ]	<< #0=new ATEDeviceFunction(0,actions); >>
	;
	
control_block_info![ASTList * actions,NounEntry * nounEntry]:
	<<
		ModifierEntry	* modifierEntry	= 0;
		DimensionEntry	* dimensionEntry= 0;
		AST * action;
	>>
		{ sv : signal_verb }
		{ nm : noun_modifier[0]>[modifierEntry] } 
		(
			cs : control_sequence[ nounEntry ]	<< #(#sv,#cs,#nm); >>
		|
			ib : ifthen_block[ nounEntry ]	<< #(#sv,#ib,#nm); >>
		)
		<< actions->insert( #sv); >>
		(
			Fd cbi : control_block_info[ actions , nounEntry ]
			<< #0=#cbi; >>
		|
			<< #0=new ATEDeviceFunction(0,actions); >>
		)
	;

  
ifthen_block![NounEntry * nounEntry]:    // 4. 3. 1. 2 . 1
	<< AST * v=0 ; AST  * b=0 ; >>
	IF  c:condition  THEN
		<< #0=#(v=new IfTedl,#c) ; >>
	(	cs:control_sequence[nounEntry]	<< b=#cs; >>
	|	tb:ifthen_block[nounEntry]	<< b=#tb; >>
	)
	(
		ELSE_IF  cn:condition THEN
		<< v=new SkipTedl;#(0,#0,new TargetTedl(v),b,v,#(new ElseIfTedl,#cn)); >>
		(	csn:control_sequence[nounEntry]		<<b=#csn;>>
		|	tbn:ifthen_block[nounEntry]		<<b=#tbn;>>
		)
	)*
	{ ELSE
		<< v=new SkipTedl; #(0,#0,new TargetTedl(v),b,v,new ElseTedl); >>
		(
			csnn:control_sequence[nounEntry]	<<b=#csnn;>>
		|	tbnn:ifthen_block[nounEntry]		<<b=#tbnn;>>
		)
	}
	END IF
	<<
		v=new SkipTedl;#(0,#0,new TargetTedl(v),b,v,new EndIfTedl);
		#0=#(new ExecuteTedl,#0);
	>>
	;
  
condition!:    // 4 . 3. 1. 2 .2
	(
		pt:PORT  IS  pn:name
		<< #0 = #( new PortIsField, #pn ); >>
	|	st:STATE  IS  s:state
		<< #0 = #( new StateField,#s ); >>
		
	|	ep:EVENT_PORT  IS  epn:name
		<< #0 = #( new EventPortField, #epn ); >>
		
	|	EVENT_SLOPE_POS
		<< #0 = new EventSlopePosField ; >>
		
	|	EVENT_SLOPE_NEG
		<< #0 = new EventSlopeNegField; >>
		
	|	eve:EVENT_VOLTAGE_EQ  dn:decimal_number	//	Appears to be totally bogus.
		<< #0 = #( new EventVoltageField, #dn ); >>
		
	|	nmee:noun_modifier_evaluation_expression
		<<  #0=#nmee; >>
//	|	rmee:resource_modifier_evaluation_expression
//		<<  #0=#rmee; >>
	)
	{
		 ao:and_or  c:condition
		 << #0=#(#ao,#0,#c); >>
	}
	;
  
and_or!:
	( andOp:AND << #0= new ANDOperator($andOp); >> | orOp:OR << #0= new OROperator($orOp); >> )
	;
	
noun_modifier_evaluation_expression!:    // 4 . 3. 1.2 .3
	<<
		ModifierEntry	* modifierEntry	= 0;
		DimensionEntry	* dimensionEntry= 0;
	>>
	nm:noun_modifier[0]>[modifierEntry]	co:comparison_operator
		(
			md:modifier_descriptor	<< #0 = #( #co, #nm, #md ); >>
		|	dn:decimal_number	<< #0 = #( #co, #nm, #dn ); >>
		)
	;
  
resource_modifier_evaluation_expression!:    // 4. 3. 1. 2 .4
	rm:resource_modifier co:comparison_operator
	(
			rmd:resource_modifier_descriptor	<< #0 = #( #co, #rm, #rmd ); >>
		|	dn:decimal_number			<< #0 = #( #co, #rm, #dn  ); >>
	)
	;
	
comparison_operator!:
		eq:EQcomp	<< #0 = #( new EQOperator($eq) ); >>
	|	ne:NEcomp	<< #0 = #( new NEOperator($ne) ); >>
	|	gt:GTcomp	<< #0 = #( new GTOperator($gt) ); >>
	| 	lt:LTcomp	<< #0 = #( new LTOperator($lt) ); >>
	|	ge:GEcomp	<< #0 = #( new GEOperator($ge) ); >>
	|	le:LEcomp	<< #0 = #( new LEOperator($le) ); >>
	;

  
state:    // 4 .3 . 1.2 . 5
		UNALLOCATED
	|	SET
	|	CONNECTED
	|	PREPARED
	|	ARMED
	|	READY
	|	APPLIED
	|	MEASURED
	;
  
control_sequence![NounEntry * nounEntry]:    // 4.3 . 1. 3
		ac : ascii_command  { IF_ERROR  { name } Open_Quote message Close_Quote }
	|	dc : device_command[nounEntry] { IF_ERROR  { name } Open_Quote message Close_Quote } // sik extension
		<<
			#0=#dc;
		>>
	;
  
ascii_command!:    // 4 .3 . 1. 3. 1
	Open_Quote command_characters
	{
		(Left_Angle_Bracket  command_characters Right_Angle_Bracket {Fd}) 
   	|	command_characters
	}  Close_Quote
	;
	// above period included for real values, omitted for integers


device_command[NounEntry * nounEntry]: // sik extension
			// This rule will accept calls such as
			// @modsw<1><1><slot><relay>;
			// @matsw<func><box>,slot><relay>;
			// All spec's calls will be executed, to enforce the condition.
	Open_Bracket!
		(
			func_call[nounEntry]
		)*
		Close_Bracket!
		<<
			#0 = #( new ExecuteTedl,#0 );
		>>
	;

func_call[NounEntry * nounEntry]:
		AtSign! id:EID!
			<< int get=0; >>
			(
				call_arg[nounEntry]
			)* Semi_Colon!
		<<
			#0 = #( new TedlDeviceDriver($id),#0 );
		>>
	;

call_arg![NounEntry * nounEntry] : // sik
	<< RWCString argtype="I";RWCString argusage="SRM";int get=1,put=1; >>
	{Left_Angle_Bracket<<get=1;>>}
	(
		dn:decimal_number
			<<	#0=#(new TedlArgumentConstant,#dn);	>>
	|	{	LP type:EID << argtype=$type->getText(); >>
				{COLON usage:EID << argusage=$usage->getText(); >>}
			RP
		}
		id:EID
			<<
				ModifierEntry * modifierEntry=argModifier(nounEntry,$id);
				// = new TedlArgumentLabel(nounEntry,$id,modifierEntry);
				RWCString param=$id->getText();
				AST * tedlArgumentLabel=CurrentResource->theCallArg
							(
								nounEntry	,	$id,
								param		,	modifierEntry,
								argtype		,	argusage,
								get		,	put
							);
				#0=tedlArgumentLabel;
			>>
	)
	{Right_Angle_Bracket << put=1; >>}
	;

resource_preparation_capability!:    // 4 . 3. 2
	<<
		ModifierEntry * modifierEntry = 0;
		DimensionEntry * dimensionEntry = 0;
	>>
	PREP  resource_modifier
		{
			resource_modifier_descriptor
				(
					Fd resource_modifier_descriptor
				)*
		}
		{ 
			dimensioned_decimal_number_range[modifierEntry,dimensionEntry]
		}
		{ 
			AT (name)+
		} 
	;



//	READ_RESOURCE not implemented...
resource_preparation_control![NounEntry *& nounEntry]:    // 4. 3 .3
	<< ASTList * actions ; >>
	SETUP_RESOURCE
		<< actions = new ASTList; >>
		cbi:control_block_info[ actions , nounEntry ]	<< #0=new ATEDeviceFunction(0,actions); >>
	;

//resource_preparation_control_info![NountEntry *& nounEntry]:    // 4. 3 .3
//	(
//		resource_modifier  {resource_modifier_descriptor }
//		(
//			cs : control_sequence[nounEntry]
//		|	ib : ifthen_block[nounEntry]
//		)
//	|	ifthen_block[nounEntry] 
//	)
//	;

device_model_end!:    // 4 .4
	END  DEVICE_MODEL  name stmt_term
		<<
			CurrentModel=0;
			processorMode = NoMode ;
		>>
	;

stmt_term!:    // 5. 0   --- consumes surrounding white space
	Statement_Terminator
	<< clear_statement_error_flag(); >>
	; 

dimensioned_decimal_number![ModifierEntry * modifierEntry,DimensionEntry * & dimensionEntry] :	    // 5.l
	dn:decimal_number d:dim[modifierEntry,dimensionEntry]
		<< #0 = #( new DimensionedNumberType( modifierEntry ), #dn, #d ); >>
	;

unsigned_decimal_number!:	// 5.2
		udecimal:Unsigned_Decimal_Number	<< #0 = new DecimalNumber($udecimal); >> 
	|	uin:unsigned_integer_number		<< #0 = #uin; >> 
	;

decimal_number!:	// 5.2
		MINUSop
			(
				udn:Unsigned_Decimal_Number
				<< #0 = new DecimalNumber($udn,-1); >>
			|	uin:Unsigned_Integer_Number
				<< #0 = new IntegerNumber($uin,-1); >>
			)
	|	{PLUSop}  udnp:unsigned_decimal_number	<< #0 = #udnp; >>
	;   

//sign!:
//		PLUSop	<< #0 = new IdentityOperator; >>
//	|	MINUSop	<< #0 = new NegativeOperator; >>
//	;

dimensioned_decimal_number_range![ModifierEntry *& modifierEntry,DimensionEntry * & dimensionEntry] :	// 5.3

	RANGE	fr:dimensioned_decimal_number[modifierEntry,dimensionEntry]
	TO	to:dimensioned_decimal_number[modifierEntry,dimensionEntry]
		<<
			#0 = #( new ConstraintRange, #fr, #to );
		>>
	;

errlim![NounEntry * nounEntry,DimensionEntry * &dimensionEntry]:    // 5.4
	<<
		ModifierEntry * modifierEntry = 0;
	>>
	PLUSMINUSop pc:unsigned_decimal_number Percent
	<<
		#0 = #( new ConstraintPercent, #pc, #pc );
	>>
	{
		noun_modifier[nounEntry]>[modifierEntry]
			dimensioned_decimal_number_range[modifierEntry,dimensionEntry]
	}
	;
  
signal_verb!: //5 . 5
		setup		:SETUP			<<#0=new TedlSignalSetupVerb		($setup)	;>>
	|	connect		:CONNECT		<<#0=new TedlSignalConnectVerb		($connect)	;>>
	|	disconnect	:DISCONNECT		<<#0=new TedlSignalDisconnectVerb	($disconnect)	;>>
	|	arm		:ARM			<<#0=new TedlSignalArmVerb		($arm)		;>>
	|	fetch		:FETCH			<<#0=new TedlSignalFetchVerb		($fetch)	;>>
	|	change		:CHANGE			<<#0=new TedlSignalChangeVerb		($change)	;>>
	|	enable		:ENABLE EVENT		<<#0=new TedlSignalEnableEventVerb	($enable)	;>>
	|	disable		:DISABLE EVENT		<<#0=new TedlSignalDisableEventVerb	($disable)	;>>
	|	reset		:RESET			<<#0=new TedlSignalResetVerb		($reset)	;>>
	|	stimulate	:STIMULATE		<<#0=new TedlSignalStimulateVerb	($stimulate)	;>>
	|	sense		:SENSE			<<#0=new TedlSignalSenseVerb		($sense)	;>>
	|	prove		:PROVE			<<#0=new TedlSignalProveVerb		($prove)	;>>

	|	exchange	:DO Fd EXCHANGE		<<#0=new TedlExchangeDoVerb		($exchange)	;>>
	|	enablex		:ENABLE Fd EXCHANGE	<<#0=new TedlExchangeEnableVerb		($enablex)	;>>
	|	disablex	:DISABLE Fd EXCHANGE	<<#0=new TedlExchangeDisableVerb	($disablex)	;>>
	|	updatex		:UPDATE Fd EXCHANGE	<<#0=new TedlExchangeUpdateVerb		($updatex)	;>>
	|	fetchx		:FETCH Fd EXCHANGE	<<#0=new TedlExchangeFetchVerb		($fetchx)	;>>
	;


modifier_descriptor_pulse_class:                // ATLAS fsd-- 6.19.3
	( 
			RZ
		|	NRZ
		|	BIP
		|	MIP
		|	HDB unsigned_integer_number 
	)
	;
  
modifier_descriptor!:    // 5. 6
	( 
		modifier_descriptor_pulse_class
	//|	modifier_descriptor_name 
	)
	;
	
//zero_one_Q:
//	(0 | 1 | Q )+ { OR zero_one_Q}
//	;
  
dim![ ModifierEntry * modifierEntry, DimensionEntry * & dimensionEntry ]:	// 5.7
  		()
  			<< dimensionEntry = mnemonicsDB->theDimensionEntry( LT(1)->getText(), modifierEntry ) >>?
  			[ Error_Report( "Tedl Unrecognized Dimension:", LT(1) ); ]
  		did:DimensionCLASS
  			<< #0 = new DimensionType( $did, dimensionEntry ); >>
	;
  

noun_modifier![NounEntry * nounEntry]>[ModifierEntry * modifierEntry]:     // 5.8
		<< RWCString last;ANTLRTokenPtr p; >>
		modifier[nounEntry,last]>[$modifierEntry,p]	//{measured_characteristic_mnemonic_sfx}
		<<
			#0=new TedlCharacteristicMnemonicType(p,$modifierEntry,last);
			if(last!=""){
				Error_Report("Unrecognized modifier suffix:"+last,p);
			}
		>>
		//measured_characteristic_mnemonic[nounEntry]>[$modifierEntry] //+ suffixes
	;


modifier![NounEntry * nounEntry,RWCString & left]>[ModifierEntry * modifierEntry,ANTLRTokenPtr p]:  // From ATLAS
	() << $modifierEntry = mnemonicsDB->theModifierEntry( LT(1)->getText(), nounEntry ,left ) >>?
	[ Error_Report( "Tedl Unrecognized Noun Modifier :", LT(1) ); ]
	id:EID
	<< $p= $id; >>
	;

  
noun![NounEntry *& nounEntry]>[NounEntry * nounEntry] :
		<< int howmany=0;int count=0; >>
		<< nounEntry=$nounEntry=theNounEntry( LT(1),howmany )>>?
		[ Error_Report( "Tedl Noun expected:", LT(1) ); ]
		( nc:NounCLASS << count ++; >> )+
		<<
			if(howmany!=count){
				Error_Report( "Tedl Noun is ambiguous at:", $nc );
			}
			#0 = new NounType( $nc , $nounEntry );
		>>
		//	(
		//		(Dash no
		//	|	( noun)?
		//	|	()
		//	)
	;
  
  
version_label:    // 6. 0
	EID			//( letter | digit ) ( letter | digit | _ )*
	;
  
bus_name:    // 6. 1
	{ AtSign! } name;
  
name:    // 6.2
	EID			//letter ( letter | digit | _ | d ) *
	;
  
address:    // 6.3
		EID
	|	integer_number
	;

device_port_name:    // 6.4
	NameCLASS
	;
  
resource_modifier: 		// 6. 5
	name			// an implementation unique modifier used to describe instrument
	;			// capabilities that can not be accessed at the ATLAS level
				// (e.g. AC_COUPLING, FILTERING, etc.)
  
resource_modifier_descriptor:	// 6. 6
	name			// a descriptor used with a resource_modifier to further
	;			// define instrument
				// capabilities that can not be accessed at the ATLAS level
				// (e.g. ON. OFF, LOG, LINEAR, etc.)
  
connect_mnemonic:    // 6.7
	Characters;
  
disconnect_mnemonic:    // 6.8
	Characters;
  
substitution_source:    // 6.9
		name
	|	decimal_number
	;	//	source of value to be extracted from a test program or TEDL model
		//	i. e. noun_modifier, resource_modifier,
		//	resource_modifier_descriptor or logical_location
  
command_characters:    // 6. 10
		Characters
	|	AtSign!	EID
	;
  
connect_algorithm!:    // 6. 11
	(
		Characters
	|	Left_Angle_Bracket
	|	Right_Angle_Bracket
	|	EID
	)+
	
 	// formula which derives switch connect
	;


device_model_port_name:    // 6. 12
	NameCLASS
	;
  
message:    // 6.13

		(Characters | Left_Angle_Bracket | Right_Angle_Bracket | EID) +
	;
  
unsigned_integer_number!:	// 6. 1 8
	uin:Unsigned_Integer_Number
	<< #0=new IntegerNumber($uin);>>
	;

integer_number!:		// sik extension
		PLUSop	pin:Unsigned_Integer_Number << #0=new IntegerNumber($pin);>>
	|	MINUSop	nin:Unsigned_Integer_Number << #0=new IntegerNumber($nin,-1);>>
	|		uin:Unsigned_Integer_Number << #0=new IntegerNumber($uin);>>
	;
	
	 
//   FINIS

}





