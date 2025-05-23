h04840
s 00039/00001/00222
d D 1.4 01/01/29 16:25:03 sinan 5 4
c ls
e
s 00062/00064/00161
d D 1.3 99/10/20 10:23:44 sinan 4 3
c changes
e
s 00032/00010/00193
d D 1.2 99/08/09 14:11:42 sinan 3 1
c Aug 9
e
s 00000/00000/00000
d R 1.2 99/07/21 22:48:50 Codemgr 2 1
c SunPro Code Manager data about conflicts, renames, etc...
c Name history : 1 0 VirtualDriver/simDriver.cc
e
s 00203/00000/00000
d D 1.1 99/07/21 22:48:49 sinan 1 0
c date and time created 99/07/21 22:48:49 by sinan
e
u
U
f e 0
t
T
I 1
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
D 4
		cout	<< "dmmsu called "
			<< *arg1
			<< " "
			<< *arg2
			<< " "
			<< *arg3
			<< " "
			<< *arg4
E 4
I 4
		cout	<< "call dmmsu ( "
			<< *arg1	<<	" , "
			<< *arg2	<<	" , "
			<< *arg3	<<	" , "
			<< *arg4	<<	" ) "
E 4
			<< endl;		
	}
}

//-----------------------------------------------------------------------
extern "C" {

void dmmmu( int * arg1, double * arg2, int * arg3)
	{
D 4
		cout	<< "dmmmu called "
			<< *arg1
			<< " "
			<< *arg2
			<< " "
			<< *arg3
			<< " "
E 4
I 4
		cout	<< "call dmmmu ( "
			<< *arg1	<<	" , "
			<< "M[1]"	<<	" , "
			<< *arg3	<<	" ) "
E 4
			<< endl;
		*arg2=1.234;		
	}
}

I 5
//-----------------------------------------------------------------------
E 5
extern "C" {

void dcvhirange( int * arg1, double * arg2, double * arg3)
	{
D 4
		cout	<< "dcvhirange called "
			<< *arg1
			<< " "
			<< *arg2
			<< " "
			<< *arg3
			<< " "
E 4
I 4
		cout	<< "call dcvsh ( "
			<< *arg1	<<	" , "
			<< *arg2	<<	" , "
			<< *arg3	<<	" ) "
E 4
			<< endl;
	}
}

//-----------------------------------------------------------------------
extern "C" {

I 5
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

E 5
void dcvlorange( int * arg1, double * arg2, double * arg3)
	{
D 4
		cout	<< "dcvlorange called "
			<< *arg1
			<< " "
			<< *arg2
			<< " "
			<< *arg3
			<< " "
E 4
I 4
		cout	<< "call dcvsl ( "
			<< *arg1	<<	" , "
			<< *arg2	<<	" , "
			<< *arg3	<<	" ) "
E 4
			<< endl;
	}
}
//-----------------------------------------------------------------------
extern "C" {

void ctrsu( int * arg1, int* arg2, int * arg3)
	{
D 4
		cout	<< "ctrsu called "
			<< *arg1
			<< " "
			<< *arg2
			<< " "
			<< *arg3
			<< " "
E 4
I 4
		cout	<< "call ctrsu ( "
			<< *arg1	<<	" , "
			<< *arg2	<<	" , "
			<< *arg3	<<	" ) "
E 4
			<< endl;
	}
}
//-----------------------------------------------------------------------
extern "C" {

void ctrrm( int * arg1, double * arg2, int * arg3)
	{
D 4
		cout	<< "ctrrm called "
			<< *arg1
			<< " "
			<< *arg2
			<< " "
			<< *arg3
			<< " "
E 4
I 4
		cout	<< "call ctrrm ( "
			<< *arg1	<<	" , "
			<< "M[1]"	<<	" , "
			<< *arg3	<<	" ) "
E 4
			<< endl;
		*arg2=0.12;
	}
}
//-----------------------------------------------------------------------
extern "C" {

void ctrenab( int * arg1)
	{
D 4
		cout	<< "ctrenab called "
			<< *arg1
			<< " "
E 4
I 4
		cout	<< "call ctrenab ( "
			<< *arg1	<<	" ) "
E 4
			<< endl;
	}
}
I 5
//-----------------------------------------------------------------------
extern "C" {
E 5

I 5
void enable( int * arg1,double * arg2, int * arg3)
	{
		cout	<< "call enable ( "
			<< *arg1	<<	" , "
			<< *arg2	<<	" , "
			<< *arg3	<<	" ) "
			<< endl;
	}
}

E 5
//-----------------------------------------------------------------------
extern "C" {

void matswitch( int * arg1,int *arg2,int *arg3,int *arg4)
	{
D 3
		cout	<< "matswitch called "
E 3
I 3
D 4
		cout	<< "matsw called "
E 3
			<< *arg1
			<< " "
			<< *arg2
			<< " "
			<< *arg3
			<< " "
			<< *arg4
			<< " "
E 4
I 4
		cout	<< "call matsw ( "
			<< *arg1	<<	" , "
			<< *arg2	<<	" , "
			<< *arg3	<<	" , "
			<< *arg4	<<	" ) "
E 4
			<< endl;
	}
}

//-----------------------------------------------------------------------
extern "C" {

void muxswitch( int * arg1,int *arg2,int *arg3,int *arg4)
	{
D 4
		cout	<< "muxsw called "
			<< *arg1
			<< " "
			<< *arg2
			<< " "
			<< *arg3
			<< " "
			<< *arg4
			<< " "
E 4
I 4
		cout	<< "call muxsw ( "
			<< *arg1	<<	" , "
			<< *arg2	<<	" , "
			<< *arg3	<<	" , "
			<< *arg4	<<	" ) "
E 4
			<< endl;
	}
}

//-----------------------------------------------------------------------
extern "C" {

void modswitch( int * arg1,int *arg2,int *arg3,int *arg4)
	{
D 3
		cout	<< "muxsw called "
			<< *arg1
			<< " "
			<< *arg2
			<< " "
			<< *arg3
			<< " "
			<< *arg4
			<< " "
E 3
I 3
D 5
		cout	<< "call modsw ("
E 5
I 5
		cout	<< "call modsw ( "
E 5
			<< *arg1	<<	" , "
			<< *arg2	<<	" , "
			<< *arg3	<<	" , "
			<< *arg4	<<	" ) "
E 3
			<< endl;
	}
I 3
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
I 4
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
E 4
E 3
}
E 1
