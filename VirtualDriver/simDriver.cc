#include	<iostream>
#include	<string.h>

class	VirtualInterface
{
	public:
		VirtualInterface( int argc, char * argv[] )
		{
			_argc = argc;
						
			for ( int cnt=0; cnt < argc; ++cnt ){
			
				_argv[cnt] = new char( strlen(argv[cnt])+1 );
				strcpy( _argv[cnt], argv[cnt] );
			}
		};
		
		void	print()
			{
				cout << "Call to Virtual Instrument Driver." << endl;
				
				for ( int cnt=0; cnt < _argc; ++cnt )
					cout <<  _argv[cnt] << " ";
				
				cout << endl;
			};
	
	private:
		int	_argc;
		char *	_argv[100];
};

//-----------------------------------------------------------------------
extern "C" {

void VirtualInstrumentDriver( int argc, char * argv[] )
	{
		VirtualInterface	x( argc, argv );
		
		x.print();
	}
}

//-----------------------------------------------------------------------
extern "C" {

void dmmsu( int * arg1, int * arg2, int * arg3,int * arg4)
	{
		cout	<< "call dmmsu ( "
			<< *arg1	<<	" , "
			<< *arg2	<<	" , "
			<< *arg3	<<	" , "
			<< *arg4	<<	" ) "
			<< endl;		
	}
}

//-----------------------------------------------------------------------
extern "C" {

void dmmmu( int * arg1, double * arg2, int * arg3)
	{
		cout	<< "call dmmmu ( "
			<< *arg1	<<	" , "
			<< "M[1]"	<<	" , "
			<< *arg3	<<	" ) "
			<< endl;
		*arg2=1.234;		
	}
}

//-----------------------------------------------------------------------
extern "C" {

void dcvhirange( int * arg1, double * arg2, double * arg3)
	{
		cout	<< "call dcvsh ( "
			<< *arg1	<<	" , "
			<< *arg2	<<	" , "
			<< *arg3	<<	" ) "
			<< endl;
	}
}

//-----------------------------------------------------------------------
extern "C" {

void dcv6002lo( int * arg1, double * arg2, double * arg3)
	{
		cout	<< "call dcv6002lo ( "
			<< *arg1	<<	" , "
			<< *arg2	<<	" , "
			<< *arg3	<<	" ) "
			<< endl;
	}
}
//-----------------------------------------------------------------------
extern "C" {

void dcv6002hi( int * arg1, double * arg2, double * arg3)
	{
		cout	<< "call dcv6002hi ( "
			<< *arg1	<<	" , "
			<< *arg2	<<	" , "
			<< *arg3	<<	" ) "
			<< endl;
	}
}

//-----------------------------------------------------------------------
extern "C" {

void dcvlorange( int * arg1, double * arg2, double * arg3)
	{
		cout	<< "call dcvsl ( "
			<< *arg1	<<	" , "
			<< *arg2	<<	" , "
			<< *arg3	<<	" ) "
			<< endl;
	}
}
//-----------------------------------------------------------------------
extern "C" {

void ctrsu( int * arg1, int* arg2, int * arg3)
	{
		cout	<< "call ctrsu ( "
			<< *arg1	<<	" , "
			<< *arg2	<<	" , "
			<< *arg3	<<	" ) "
			<< endl;
	}
}
//-----------------------------------------------------------------------
extern "C" {

void ctrrm( int * arg1, double * arg2, int * arg3)
	{
		cout	<< "call ctrrm ( "
			<< *arg1	<<	" , "
			<< "M[1]"	<<	" , "
			<< *arg3	<<	" ) "
			<< endl;
		*arg2=0.12;
	}
}
//-----------------------------------------------------------------------
extern "C" {

void ctrenab( int * arg1)
	{
		cout	<< "call ctrenab ( "
			<< *arg1	<<	" ) "
			<< endl;
	}
}
//-----------------------------------------------------------------------
extern "C" {

void enable( int * arg1,double * arg2, int * arg3)
	{
		cout	<< "call enable ( "
			<< *arg1	<<	" , "
			<< *arg2	<<	" , "
			<< *arg3	<<	" ) "
			<< endl;
	}
}

//-----------------------------------------------------------------------
extern "C" {

void matswitch( int * arg1,int *arg2,int *arg3,int *arg4)
	{
		cout	<< "call matsw ( "
			<< *arg1	<<	" , "
			<< *arg2	<<	" , "
			<< *arg3	<<	" , "
			<< *arg4	<<	" ) "
			<< endl;
	}
}

//-----------------------------------------------------------------------
extern "C" {

void muxswitch( int * arg1,int *arg2,int *arg3,int *arg4)
	{
		cout	<< "call muxsw ( "
			<< *arg1	<<	" , "
			<< *arg2	<<	" , "
			<< *arg3	<<	" , "
			<< *arg4	<<	" ) "
			<< endl;
	}
}

//-----------------------------------------------------------------------
extern "C" {

void modswitch( int * arg1,int *arg2,int *arg3,int *arg4)
	{
		cout	<< "call modsw ( "
			<< *arg1	<<	" , "
			<< *arg2	<<	" , "
			<< *arg3	<<	" , "
			<< *arg4	<<	" ) "
			<< endl;
	}
}

//-----------------------------------------------------------------------
extern "C" {
void elgar( int * arg1,int * arg2, double * arg3, double * arg4)
	{
		cout	<< "call elgar ( "
			<< *arg1	<<	" , "
			<< *arg2	<<	" , "
			<< *arg3	<<	" , "
			<< *arg4	<<	" ) "
			<< endl;
	}
}

//-----------------------------------------------------------------------
extern "C" {
void wavetekSine( int * arg1,double * arg2, double * arg3, double * arg4)
	{
		cout	<< "call wavetekSine ( "
			<< *arg1	<<	" , "
			<< *arg2	<<	" , "
			<< *arg3	<<	" , "
			<< *arg4	<<	" ) "
			<< endl;
	}
}


//-----------------------------------------------------------------------
extern "C" {
void oscSetup( int * arg1,int * arg2, int * arg3, int * arg4)
	{
		cout	<< "call oscSetup ( "
			<< *arg1	<<	" , "
			<< *arg2	<<	" , "
			<< *arg3	<<	" , "
			<< *arg4	<<	" ) "
			<< endl;
	}
}
//-----------------------------------------------------------------------
extern "C" {
void oscRead( int * arg1,double * arg2, int * arg3 )
	{
		cout	<< "call oscRead ( "
			<< *arg1	<<	" , "
			<< "M[1]"	<<	" , "
			<< *arg3	<<	" ) "
			<< endl;
	}
}
