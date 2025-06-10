#ifndef ExecEnv_h
#define ExecEnv_h

class ExecEnv;

typedef void (ExecEnv::*p)(char *s, char *t);

typedef struct Opt {
	const	char           *option;
	int             arg;
	p          process;
	const	char 	           *descr;
} ;


class ExecEnv
{
public:
	ExecEnv(/*AtlasBox * b*/);
	
	int ProcessArgs (int argc, char **argv );

	void	help();

	int	nodeTrace	(int newLevel=-1);
	int	edgeTrace	(int newLevel=-1);
	int	execTrace	(int newLevel=-1);
	int	rejReason	(int newLevel=-1);
	int	showPath	(int newLevel=-1);
	int	statistics	(int newLevel=-1);
	int	relayFeasible	(int newLevel=-1);
	int	deviceTrace	(int newLevel=-1);
	int	pathFeasible	(int newLevel=-1);
	int	dumpCircuit	(int newLevel=-1);
	int	searchAlgorithm	(int newLevel=-1);
	int	codeGen		(int newLevel=-1);



	char *	dbDirectory();
	char *	cfFile();
	
public:	
	friend struct Opt;
	 void	pTNS (char *s, char *t);
	int ProcessArgs (int argc, char **argv ,Opt * options);

	 void	pTES (char *s, char *t);

	 void	pTEX (char *s, char *t);

	 void	pDBD (char *s, char *t);
	 void	pSAL (char *s, char *t);
	 
	 void	pFIL	(char *s, char *t);
	 void	pREJ	(char *s, char *t);
	 void	pSHP	(char *s, char *t);
	 void	pSTAT	(char *s, char *t);
	 void	pRFS	(char *s, char *t);
	 void	pTDV	(char *s, char *t);
	 void	pPF	(char *s, char *t);
	 void	pDUMP	(char *s, char *t);
	 void	pCGEN	(char *s, char *t);


//private:
	int	setter(int newLevel,int & oldLevel);
	int		m_edgeTrace		;
	int		m_nodeTrace		;
	int		m_execTrace		;
	int		m_rejReason		;
	int		m_showPath		;
	int		m_statistics		;
	int		m_relayFeasible		;
	int		m_deviceTrace		;
	int		m_pathFeasible		;
	int		m_dumpCircuit		;
	int		m_codeGen		;

	int		m_searchAlgorithm	;

	char	*	m_dbDirectory		;
	char	*	m_cfFile		;
	
};

#endif //ExecEnv_h
