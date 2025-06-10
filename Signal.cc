#define _POSIX_PTHREAD_SEMANTICS
#include <pthread.h>
#include "Signal.h"
#include "assert.h"


SignalWatcher::SignalWatcher(int SigNo, AST * ast)
:m_ast(ast),m_SigNo(SigNo)
{
	extern pthread_attr_t  attr;
	int             err;
	pthread_t       tid;
	sigset_t        s;
	sigset_t        *o=NULL;
	sigemptyset(&s);
	sigaddset(&s,SigNo);
	// bug ?? pthread_sigmask(SIG_BLOCK, &s, o);	/* Take no interrupts */
	m_active=1;
	assert(sem_init(&watcher_sem, 0, 0)==0);
	assert(pthread_create(&tid, &attr, signalWatcherFunc, (void *) this)==0);
}


void *
SignalWatcher::sigwaiter(void *arg)
{
	SignalWatcher *  master=(SignalWatcher *)arg;
	int SigNo=master->m_SigNo;
	int             sig, err;
	sigset_t        s;
	sigset_t        *o=NULL;

	sigfillset(&s);
	// bug ?? pthread_sigmask(SIG_BLOCK, &s,o);	/* Take no interrupts */

	sigemptyset(&s);
	sigaddset(&s, SigNo);

	while (master->active()) {
		assert(sigwait(&s, &sig)==0);
		assert(sig==SigNo);
		master->tickle();
	}
	return 0;
}


int 
SignalWatcher::Sem_Wait()
	{
		while (sem_wait(&watcher_sem) != 0) {}
		return 0;
	}


void SignalWatcher::tickle()
	{
		assert(sem_post(&watcher_sem)==0);
	}

void SignalWatcher::kill()
	{
		m_active=0;
		// This line is Solaris specific.
		// sigsend((idtype_t)P_MYID,0,m_SigNo);
		// replace with this instead.
		sem_post(&watcher_sem);  // safe and portable
	}

int SignalWatcher::active()
	{
		return m_active;
	}

void SignalWatcher::wakeup()
	{
		m_ast->eval(m_ast);
	}

void	*
SignalWatcher::signalWatcherFunc( void * arg )
	{
		SignalWatcher * master=(SignalWatcher *)arg;
		
		extern pthread_attr_t  attr;
		int             err;
		pthread_t       tid;

		int result = pthread_create(&tid, &attr, sigwaiter, (void *) arg);
		while(master->active()){
			master->Sem_Wait();
			master->wakeup();
		}
		return 0;
	}

#ifdef NotDefSignal
#include <unistd.h>
 pthread_attr_t	attr;


class PthreadAttributes
{
public:	
	PthreadAttributes()
		{
			pthread_attr_init( &attr );
			pthread_attr_setscope( &attr, PTHREAD_SCOPE_SYSTEM );
			pthread_attr_setdetachstate(  &attr, PTHREAD_CREATE_DETACHED );
		}
};

static PthreadAttributes pthreadAttributes;	// instantiate to initialize


int main()
	{
	AST * x=new AST;
	SignalWatcher sw(SIGINT,x);
	sleep(10);	
	
	}
#endif