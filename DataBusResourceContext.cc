#include	"Resource.h"
#include	"TedlStd.h"
#include	"TedlParser.h"


DataBusResourceContext::DataBusResourceContext()
	:ResourceContextBASE()
	{
	}


void
DataBusResourceContext::toEVENTS(void (DataBusResourceContext::*c)(AST *))
	{
		AST *fr,*to;
		if(fr=m_Resource->getFromEvent()){
			if(c==&DataBusResourceContext::ENABLE_EXCHANGE){
				fr->init();
			}
			Resource * r=fr->getResource();
			(r->m_currentDataBusFSM->*c)(fr);
		}
		if(to=m_Resource->getToEvent()){
			if(c==&DataBusResourceContext::ENABLE_EXCHANGE){
				to->init();
			}
			Resource * r=to->getResource();
			(r->m_currentDataBusFSM->*c)(to);
		}
	}

	
void
DataBusResourceContext::preSetContext()
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


DataBusResourceContext *
DataBusResourceContext::clone(Resource *)
	{
		assert(0);
		return 0;
	}



int
DataBusResourceContext::resetFSM()
	{
		undoConnectRequest();
		return 1;
	}

