#include	"ExecEnv.h"
#include	<iostream>
#include	"stdlib.h"
#include	"ctype.h"
#include	"string.h"


Opt options[] =	{
		{"-tns"		, 0,&ExecEnv::pTNS	, "Turn on node search trace"},
		{"-tes"		, 0,&ExecEnv::pTES	, "Turn on edge search trace"},
		{"-tex"		, 0,&ExecEnv::pTEX	, "Trace  execution line no"},
		{"-dbd"		, 1,&ExecEnv::pDBD	, "Specify noun/dimension database directory"},
		{"-sal"		, 1,&ExecEnv::pSAL	, "Specify search algorithm"},
		{"-rej"		, 0,&ExecEnv::pREJ	, "Annotate rejection reasons for path search"},
		{"-shp"		, 0,&ExecEnv::pSHP	, "Show actual path"},
		{"-stat"	, 0,&ExecEnv::pSTAT	, "some timing statistics"},
		{"-rfs"		, 0,&ExecEnv::pRFS	, "Relay feasibility analysis"},
		{"-tdv"		, 0,&ExecEnv::pTDV	, "device call trace"},
		{"-pf"		, 0,&ExecEnv::pPF	, "path feasibility/conflict"},
		{"-dump"	, 0,&ExecEnv::pDUMP	, "dump circuit(BIG)"},
		{"-cgen"	, 0,&ExecEnv::pCGEN	, "code gen"},
		{"*"		, 0,&ExecEnv::pFIL	, "file"},
		{NULL, 0, NULL}
	 	};

int require(int expr,const char * err)
	{
		if( !(expr) ){
			std::cerr << *err << std::endl;
			return 0;
		}
		return 1;
	}
	
void
ExecEnv::help( )
{
	Opt *p	=	options;
	std::cerr	<< "atlas [options] file"	<<	std::endl;
	while ( *(p->option) != '*' )
	{
		std::cerr	<<	"	"		;
		std::cerr	<< 	p->option		;
		std::cerr	<<	"	"		;
		std::cerr	<<	((p->arg)?"___":"   ")	;
		std::cerr	<<	"	"		;
		std::cerr	<<	p->descr		;
		std::cerr	<<	std::endl			;
		p++;
	}
}

ExecEnv::ExecEnv()
	:	m_edgeTrace		(0)
	,	m_nodeTrace		(0)
	,	m_execTrace		(0)
	,	m_rejReason		(0)
	,	m_showPath		(0)
	,	m_statistics		(0)
	,	m_relayFeasible		(0)
	,	m_deviceTrace		(0)
	,	m_pathFeasible		(0)
	,	m_dumpCircuit		(0)
	,	m_codeGen		(0)
	,	m_searchAlgorithm	(0)
	,	m_dbDirectory		(0)
	,	m_cfFile		(0)
	{
	}	


int 
	ci_strequ (const char *a, const char *b)
	{
		for (; *a != 0 && *b != 0; a++, b++) {
			if (toupper (*a) != toupper (*b))
				return 0;
		}
		return (*a == *b);
	}
int
ExecEnv::ProcessArgs (int argc, char **argv)
	{
		return ProcessArgs (argc, argv, options);
	}
int
ExecEnv::ProcessArgs (int argc, char **argv, Opt * options)
	{
		Opt            *p;
		while (argc-- > 0) {
			p	=	options;
			while (p->option != NULL) {
				if (strcmp (p->option, "*") == 0 ||
						ci_strequ (p->option, *argv) == 1) {
					if (p->arg) {
						if (argc-- > 0) {
							(this->*(p->process)) (*argv, *(argv + 1));
							argv++;
						} else {
							std::cerr 	<<	"error: required argument for option "
								<<	*argv
								<<	" omitted"
								<<	std::endl;
							return (0);
						};
					} else
						(this->*(p->process)) (*argv,NULL);
					break;
				}
				p++;
			}
			argv++;
		}
		return 1;
	}

int giveMe(int theDefault,const char *t)
	{
		if(t){
			return atoi(t);
		} else {
			return theDefault;
		}
	}


//	getters	----------------------------------------
int
ExecEnv::nodeTrace(int newLevel)
	{
		return setter(newLevel,m_nodeTrace);
	}

int
ExecEnv::edgeTrace(int newLevel)
	{
		return setter(newLevel,m_edgeTrace);
	}

int
ExecEnv::execTrace(int newLevel)
	{
		return setter(newLevel,m_execTrace);
	}

int
ExecEnv::rejReason(int newLevel)
	{
		return setter(newLevel,m_rejReason);
	}

int
ExecEnv::showPath(int newLevel)
	{
		return setter(newLevel,m_showPath);
	}

int
ExecEnv::statistics(int newLevel)
	{
		return setter(newLevel,m_statistics);
	}

int
ExecEnv::relayFeasible(int newLevel)
	{
		return setter(newLevel,m_relayFeasible);
	}

int
ExecEnv::deviceTrace(int newLevel)
	{
		return setter(newLevel,m_deviceTrace);
	}

int
ExecEnv::pathFeasible(int newLevel)
	{
		return setter(newLevel,m_pathFeasible);
	}

int
ExecEnv::dumpCircuit(int newLevel)
	{
		return setter(newLevel,m_dumpCircuit);
	}
	
int
ExecEnv::codeGen(int newLevel)
	{
		return setter(newLevel,m_codeGen);
	}

int
ExecEnv::searchAlgorithm(int newLevel)
	{
		return setter(newLevel,m_searchAlgorithm);
	}

char *
ExecEnv::dbDirectory()
	{
		return m_dbDirectory;
	}
char *
ExecEnv::cfFile()
	{
		return m_cfFile;
	}

int
ExecEnv::setter(int newLevel,int & oldLevel)
	{
		if(newLevel!=-1){
			oldLevel=newLevel;
		}
		return oldLevel;
	}


//	setters	----------------------------------------
void
ExecEnv::pTNS (char *s, char *t)
	{
		m_nodeTrace	=	giveMe(1,t);
	}

void
ExecEnv::pTES (char *s, char *t)
	{
		m_edgeTrace	=	giveMe(1,t);
	}

void
ExecEnv::pTEX (char *s, char *t)
	{
		m_execTrace	=	giveMe(1,t);
	}

void
ExecEnv::pDBD (char *s, char *t)
	{
		m_dbDirectory	=	t;
	}

void
ExecEnv::pSAL (char *s, char *t)
	{
		m_searchAlgorithm	=	giveMe(1,t);
	}

void
ExecEnv::pFIL (char *s, char *t)
	{
		m_cfFile	=	s;
	}

void
ExecEnv::pREJ (char *s, char *t)
	{
		m_rejReason	=	giveMe(1,t);
	}

void
ExecEnv::pSHP (char *s, char *t)
	{
		m_showPath	=	giveMe(1,t);
	}

void
ExecEnv::pSTAT(char *s, char *t)
	{
		m_statistics	=	giveMe(1,t);
	}


void
ExecEnv::pRFS (char *s, char *t)
	{
		m_relayFeasible	=	giveMe(1,t);
	}
void
ExecEnv::pTDV (char *s, char *t)
	{
		m_deviceTrace	=	giveMe(1,t);
	}
void
ExecEnv::pPF (char *s, char *t)
	{
		m_pathFeasible	=	giveMe(1,t);
	}

void
ExecEnv::pDUMP (char *s, char *t)
	{
		m_dumpCircuit	=	giveMe(1,t);
	}

void
ExecEnv::pCGEN (char *s, char *t)
	{
		m_codeGen	=	giveMe(1,t);
	}
