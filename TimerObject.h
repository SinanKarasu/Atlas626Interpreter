#ifndef TimerObject_h
#define TimerObject_h


#include	<pthread.h>
#include	<time.h>
#include	<iostream>
#include	<errno.h>
#include	<semaphore.h>





struct TimeSpec : public timespec{
	TimeSpec();
	TimeSpec(const timeval & t);
	TimeSpec(const long _sec,const long _nsec);
	TimeSpec  operator +(const double  &rhs);
	TimeSpec  operator =(const timeval & rhs);
	
	operator timespec();
};

struct	arg_struct{
	arg_struct();
	double 			at_time;
	double 			period;
	AST		*	what;
	AST		*	a;
	pthread_cond_t	*	back_c;
	pthread_cond_t	*	gang_c;
	pthread_mutex_t	*	m;
	sem_t		*	sp;
	int			external;
};


class	TimerObject {
	private:
		TimeSpec	_time;
		arg_struct	*arg ;
	
	static int periodic(arg_struct * args);
		
	static void	*sched_start_func( void * arg );
	static void	*timed_start_func( void * arg );

	public:
		TimerObject();

		void	reset();
		long	read()	const;

						
		double	seconds() const;
		
		void	schedule( arg_struct * as , const double at_time,const double period, AST * what, AST * a=0 );

		void cancel();

		int	execute_timed( double max_time , AST * what, AST * a=0,AST * fail=0 );

			
		static	void wait(double deltime);


};


#endif
