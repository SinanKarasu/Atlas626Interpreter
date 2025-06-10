
#include	"AtlasAST.h"

#include	"TimerObject.h"


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


TimeSpec::TimeSpec(){}
TimeSpec::TimeSpec(const timeval & t){tv_sec=t.tv_sec;tv_nsec=t.tv_usec*1000;}
TimeSpec::TimeSpec(const long _sec,const long _nsec){tv_sec=_sec;tv_nsec=_nsec;}
	
TimeSpec  
TimeSpec::operator +(const double  &rhs)
	{
		const	long	nanosecs = 1000000000L;
		long            sec = rhs;
		long            nsec = (rhs - sec) * 1e9;

		while ((tv_nsec + nsec) >= nanosecs) {
			nsec -= nanosecs;
			sec++;
		}

		return TimeSpec(tv_sec + sec, tv_nsec + nsec);
	}
		
TimeSpec 
TimeSpec::operator =(const timeval & rhs)
	{
		return TimeSpec(rhs.tv_sec, rhs.tv_usec * 1000);
	}
	
TimeSpec::operator timespec()
	{
		timespec        t;

		t.tv_sec = tv_sec;
		t.tv_nsec = tv_nsec;
		return t;
	}

arg_struct::arg_struct()
	{
		at_time = 0.0;
		period = 0.0;
		what = 0;
		a = 0;
		back_c = new pthread_cond_t;
		pthread_cond_init(back_c,NULL);
		gang_c = 0;
		m = new pthread_mutex_t;
		pthread_mutex_init(m,NULL);
		sp = 0;
		external = 0;
	}

int
TimerObject::periodic(arg_struct * args){ return (args->period > 0.0); }
void	*
TimerObject::sched_start_func( void * arg )
	{
		arg_struct	* args = (arg_struct*)arg;

		//arg_struct	  local=*args;
		TimeSpec  now;
		clock_gettime(CLOCK_REALTIME,&now);
		if(args->back_c){			// does it want a tickle back?
			pthread_mutex_lock(args->m);	// we are here
			pthread_cond_signal(args->back_c);	// safe and sound
			if(args->gang_c){
				pthread_cond_wait(args->gang_c,args->m);// wait for condition broadcast
			}
			pthread_mutex_unlock(args->m);
		}
			
			
		
		wait(args->at_time);

		TimeSpec  start,next;
		clock_gettime(CLOCK_REALTIME,&start);
		next=now;
		pthread_cond_t wc;pthread_cond_init(&wc,NULL);
		pthread_mutex_t wm;pthread_mutex_init(&wm,NULL);
		pthread_mutex_lock(&wm);
		//if(args->back_c){				// condition signal back
			pthread_mutex_lock(args->m);		// that we made it back
			pthread_cond_signal(args->back_c);	// safe and sound
			pthread_mutex_unlock(args->m);		// AND in time
		//}
		do{
			if ( args->what ) (args->what)->eval(args->a);	// run the AST action
			//if ( args->gang_c){
			//	// do nothing...
			//} else if(periodic(args)){
			//} else {
			//}
			next=next+args->period;
			pthread_cond_timedwait(&wc,&wm,&next);
		} while(periodic(args));
			
		if(args->sp){
			sem_post(args->sp);
		}
		
		if(args->external==0){
			delete args;
		}
		
		return 0;
	}

TimerObject::TimerObject()
	{
		reset();
		arg=0;
	}

void
TimerObject::reset()
	{
		clock_gettime(CLOCK_REALTIME,&_time);
	}
long
TimerObject::read()	const
	{
		TimeSpec  now;
		clock_gettime(CLOCK_REALTIME,&now);						
		return ((now.tv_sec-_time.tv_sec)*100000+(now.tv_nsec-_time.tv_nsec)/1000);
	}
						
double
TimerObject::seconds() const	{ return read()/1.0e6;	}
		
void
TimerObject::schedule( arg_struct * as , const double at_time,const double period, AST * what, AST * a )
	{
		int             result;

		if (as) {
			arg = as;
		} else {
			arg = new arg_struct;

			arg->period = period;
			arg->at_time = at_time;
			arg->what = what;
			arg->a = a;
			arg->external = 0;
		}

		pthread_t       tid;

		pthread_mutex_lock(arg->m);
		
		result = pthread_create(&tid, &attr, sched_start_func, (void *) arg);

		
		if (!result) {
			pthread_cond_wait(arg->back_c,arg->m);
		}
		pthread_mutex_unlock(arg->m);
	}
			
void
TimerObject::cancel()
	{
		arg->period = 0.0;
	}

//////----------------------------------------------------------------------------------------------------------------
void	*
TimerObject::timed_start_func( void * arg )
	{
		arg_struct	* args = (arg_struct*)arg;

		arg_struct	  local=*args;
		
		(local.what)->eval(local.a);		// run the AST action
		pthread_mutex_lock(local.m);		// 
		pthread_cond_signal(local.back_c);	// I'm  baaaaack! safe and sound
		pthread_mutex_unlock(local.m);		// AND in time maybe ?
		
		return 0;
	}
		

int
TimerObject::execute_timed( double max_time , AST * what, AST * a,AST * fail )
	{
		arg = new arg_struct;
		int             result;

		pthread_t       tid;

		//pthread_cond_t  c;
		//pthread_mutex_t m;

		TimeSpec        to = _time + max_time;

		arg->what = what;
		arg->a = a;
		//arg->back_c = &c;
		//arg->m = &m;

		pthread_mutex_lock(arg->m);
		result = pthread_create(&tid, &attr, timed_start_func, (void *) arg);
		int             err = pthread_cond_timedwait(arg->back_c, arg->m, &to);


		if (err == ETIMEDOUT) {
			/* timeout, do something */
			pthread_cancel(tid);
			if (fail) {
				fail->eval();
			}
			return ETIMEDOUT;
		} else {
			return 0;
		}

	}
			
			
void
TimerObject::wait(double deltime)
	{
		timespec        rqtp;

		if (deltime > 0.0) {
			rqtp.tv_sec = deltime;
			rqtp.tv_nsec = (deltime - rqtp.tv_sec) * 1000000000;

			nanosleep(&rqtp, 0);
		}
	}
