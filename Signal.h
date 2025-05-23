#ifndef Signal_h
#define Signal_h

#include <signal.h>
#include <stdio.h>
#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>

#ifdef NotDefSignal

// this is just a test class... So that we can test this with a main without all
// The Atlas structures...
class AST{
	public:
		void eval(AST * a)
			{
				cerr << " Got the signal " << endl;
			}
};
#else 
#include "AtlasAST.h"
#endif

class           SignalWatcher {
public:
	SignalWatcher(int SigNo,AST * ast);
private:
	sem_t watcher_sem;
	int m_SigNo;
	int m_active;
	AST * m_ast;
	static void	*	sigwaiter(void *arg);
	int			Sem_Wait();
	void			tickle();
	void			wakeup();
	void			kill();
	int			active();
	static void    *	signalWatcherFunc(void *arg);
};

#endif
