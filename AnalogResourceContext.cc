#include	"AnalogResourceContext.h"
#include	"Resource.h"
#include	"TedlDeviceAST.h"
#include	"ExecEnv.h"

#include	"CodeGenVisitor.h"

extern ExecEnv	execEnv;

AnalogResourceContext::AnalogResourceContext()
			:ResourceContextBASE()
			{
			}

AnalogResourceContext *
AnalogResourceContext::clone(Resource *){assert(0);return 0;}

void
AnalogResourceContext::toEVENTS(void (AnalogResourceContext::*c)(AST *))
	{
		AST *fr,*to;
		if(fr=m_Resource->getFromEvent()){
			if(c==&AnalogResourceContext::SETUP){
				fr->init();
			}
			Resource * r=fr->getResource();
			(r->m_currentAnalogFSM->*c)(fr);
		}
		if(to=m_Resource->getToEvent()){
			if(c==&AnalogResourceContext::SETUP){
				to->init();
			}
			Resource * r=to->getResource();
			(r->m_currentAnalogFSM->*c)(to);
		}
	}

	
void
AnalogResourceContext::preSetContext()
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
		AST * cnxAction = getResourceContextAST()->currentParameters->data(&cnx);

		// Now ask it to introduce itself
		if(cnxAction)cnxAction->eval(getResourceContextAST());
	}





int
AnalogResourceContext::resetFSM()
	{
		undoConnectRequest();
		return 1;
	}



void
AnalogResourceContext::SETUPtoTedl()
	{
		sayhi("SETUPtoTedl");

		preSetContext();

		doPrepControl("SETUP");

		toEVENTS(&AnalogResourceContext::SETUP);

		doControlAction("SETUP");

		allocateResource();
	}


void
AnalogResourceContext::CONNECTtoTedl()
	{

		StringList devPortList;
	
		sayhi("CONNECTtoTedl");
		
		preSetContext();

		AST * pin_desc	 = getResource()->getCnx();
		while(pin_desc){
			RWCString dev_port_name  = pin_desc->getName();
			devPortList.append(dev_port_name);
			AST * uut_pin = pin_desc->ASTdown();
			//if(dev_port_name=="CNX"){
			//	if(uut_pin){
			//		RWCString dev_port_name = (uut_pin->eval())->getName();
			//		uut_pin=uut_pin->ASTright();
			//	} else {
			//		assert(0);
			//	}
			//}
			while(uut_pin){
				RWCString uut_pin_name = (uut_pin->eval())->getName();
				
				find_path(uut_pin_name,dev_port_name);
				
				SimulateCircuit(dev_port_name);

				uut_pin=uut_pin->ASTright();
			}
			pin_desc = pin_desc->ASTright();
		}

		pin_desc	 = getResource()->getCnx();
		while(pin_desc){
			AST * uut_pin = pin_desc->ASTdown();
			while(uut_pin){
				RWCString uut_pin_name = (uut_pin->eval())->getName();
				RWCString dev_port_name  = pin_desc->getName();

				int finalState =  findPathState(uut_pin_name,dev_port_name,1);
				uut_pin=uut_pin->ASTright();
			}
			pin_desc = pin_desc->ASTright();
		}

// 		StringListIterator frlit( devPortList );
// 		StringListIterator tolit( devPortList );
		for(const auto frlit: devPortList) {
		//while(++frlit ){
			for(const auto tolit: devPortList) {
			//while(++tolit){
				if(frlit.find("REF_")==0){
					// don't consider "REF_..." for now
				} else if(tolit.find("REF_")==0){
					// don't consider "REF_..." for now
				} else if(frlit != tolit ){
					Vertex	* portv1=m_Resource->node(frlit);
					Vertex	* portv2=m_Resource->node(tolit);
					if(findPathState(portv1, portv2)!=0){
						std::cerr	<<	" PATH is SHORTED.FROM:"
							<<	portv1->getName()
							<<	"  TO:"
							<<	portv2->getName()
							<<	std::endl;
					}
				}
			}
		}
		
		doPrepControl("CONNECT");

		toEVENTS(&AnalogResourceContext::CONNECT);

		doControlAction("CONNECT");

	}
	
void
AnalogResourceContext::DISCONNECTtoTedl()
	{
		sayhi("DISCONNECTtoTedl");
	
		preSetContext();

		doPrepControl("DISCONNECT");

		toEVENTS(&AnalogResourceContext::DISCONNECT);

		AST * pin_desc;
		pin_desc	= getResource()->getCnx();
		while(pin_desc){
			AST * uut_pin = pin_desc->ASTdown();
			while(uut_pin){
				RWCString uut_pin_name = (uut_pin->eval())->getName();
				RWCString dev_port_name  = pin_desc->getName();

				disconnect_path(uut_pin_name,dev_port_name);
				
				uut_pin=uut_pin->ASTright();
			}
			pin_desc = pin_desc->ASTright();
		}

		pin_desc	= getResource()->getCnx();
		while(pin_desc){
			AST * uut_pin = pin_desc->ASTdown();
			while(uut_pin){
				RWCString uut_pin_name = (uut_pin->eval())->getName();
				RWCString dev_port_name  = pin_desc->getName();
				int finalState =  findPathState(uut_pin_name,dev_port_name,0);
				uut_pin=uut_pin->ASTright();
			}
			pin_desc = pin_desc->ASTright();
		}

		doControlAction("DISCONNECT");

		if(m_ConnectRequests.cleanAll()){
			if(execEnv.deviceTrace()){
				std::cerr << "Device " << theName() << " Disconnected " << std::endl;
			}
			_DISCONNECT();
		} else {
			if(execEnv.deviceTrace()){
				std::cerr << "Device " << theName() << std::endl;
				m_ConnectRequests.print();
			}
		}
	}

void
AnalogResourceContext::DISABLE_EVENTtoTedl()
	{
		sayhi("DISABLE_EVENTtoTedl");
	
		preSetContext();

		doPrepControl("DISABLE");

		toEVENTS(&AnalogResourceContext::DISABLE_EVENT);

		doControlAction("DISABLE");
	}

void
AnalogResourceContext::ENABLE_EVENTtoTedl()
	{
		sayhi("ENABLE_EVENTtoTedl");
	
		preSetContext();

		doPrepControl("ENABLE");

		toEVENTS(&AnalogResourceContext::ENABLE_EVENT);

		doControlAction("ENABLE");
	}


void
AnalogResourceContext::CHANGEtoTedl()
	{
		sayhi("CHANGEtoTedl");
	
		preSetContext();

		doPrepControl("CHANGE");

		toEVENTS(&AnalogResourceContext::CHANGE);

		doControlAction("CHANGE");

	}

void
AnalogResourceContext::RESETtoTedl()
	{
		sayhi("RESETtoTedl");
	
		preSetContext();

		doPrepControl("RESET");

		toEVENTS(&AnalogResourceContext::RESET);

		doControlAction("RESET");

	
	}

void
AnalogResourceContext::REMOVEtoTedl()
	{
		sayhi("REMOVEtoTedl");
		toEVENTS(&AnalogResourceContext::AsyncReset);	// drop them into a holding state
		DISABLE_EVENT();
		DISCONNECT();
		//_DISCONNECT();
		RESET();
	}


void
AnalogResourceContext::MEASUREtoTedl()
	{
		sayhi("MEASUREtoTedl");
		SETUP();
		CONNECT();
		ARM();
		MEASUREMENT(); // For debug, this actually gets triggerd by TEDL
		FETCH();
		DISCONNECT();
		RESET();
	}

void
AnalogResourceContext::MONITORtoTedl()
	{
		sayhi("MONITORtoTedl");
		SETUP();
		CONNECT();
		//FIX ENABLE_MANUAL_INTERVENTION();
		//FIX while (MANUAL_INTERVENTION EQ FALSE READ, OUTPUT);
		//FIX DISABLE_MANUAL_INTERVENTION();
		DISCONNECT();
		RESET();
	}

void
AnalogResourceContext::VERIFYtoTedl()
	{
		sayhi("VERIFYtoTedl");
		SETUP();
		CONNECT();
		ARM();
		if(EventsIntroduced()){
			ENABLE_EVENT();
			SkipState();	// need a solution.....
			FETCH();
			DISABLE_EVENT();
		} else {
			SkipState();	// makes state 4 and 5 equivalent
			SkipState();	// makes state 4 and 5 equivalent
			FETCH();
		}
		COMPARE();
		DISABLE_EVENT();
		DISCONNECT();
		RESET();
	}

void
AnalogResourceContext::COMPAREtoTedl()
	{
		extern astream sout;
		sayhi("COMPAREtoTedl");
		AST * sensorAST=	getResourceContextAST()->currentParameters;
		if(sensorAST){
			AST * compAST=sensorAST->ASTright();
			if(compAST){
				compAST->eval();
				StringAST cnx("~CNX");
				sout << compAST->getScope() ;
				sout << "COMPARE";
				//sout << "(";
				sout << sensorAST->ASTdown();
				//sout << ")";
				sout << ",";
				sout << compAST ;
				
				AST * c=sensorAST->data(&cnx);
				if(c){
					sout << c;
				}
				
				sout << std::endl;
				//CodeGenVisitor codeGenVisitor(sout);
				//codeGenVisitor.Execute(compAST);
			}
		}
	}


void
AnalogResourceContext::ARMtoTedl()
	{

		sayhi("ARMtoTedl");
	
		preSetContext();

		doPrepControl("ARM");

		toEVENTS(&AnalogResourceContext::ARM);

		doControlAction("ARM");
	
	}

void
AnalogResourceContext::SkipStatetoTedl()
	{

		sayhi("SkipStatetoTedl");
	
		preSetContext();
	
		toEVENTS(&AnalogResourceContext::SkipState);
	}

void
AnalogResourceContext::AsyncResettoTedl()
	{
		sayhi("AsyncResettoTedl");
		toEVENTS(&AnalogResourceContext::AsyncReset);	// drop them into a holding state
	}

void
AnalogResourceContext::ResourceResettoTedl()
	{
		sayhi("ResourceResettoTedl");

		resetFSM();
		unallocateResource();

		toEVENTS(&AnalogResourceContext::ResourceReset);	// clear them up also
	}

