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
			<< *arg1		<<	" , "
			<< *arg2		<<	" , "
			<< int((*arg3)*1000)	<<	" ) "
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
			<< *arg1		<<	" , "
			<< *arg2		<<	" , "
			<< int((*arg3)*1000)	<<	" ) "
			<< endl;
	}
}
//-----------------------------------------------------------------------
extern "C" {

void ctrsu( int * arg1, double * arg2, int* arg3, int * arg4)
	{
		//cout	<< "call ctrsu ( "
		//	<< *arg1	<<	" , "
		//	<< *arg2	<<	" , "
		//	<< *arg3	<<	" ) "
		//	<< endl;
		
		if(*arg3==0){
			if((*arg4==0)){
				cout << " CTR_A_LEVEL="	<<	0.0	<< endl;
				cout << " CTR_A_SLOPE="	<<	1	<< endl;
				cout << " gosub ~counter_Freq_A_Setup " << endl;
			} else {
				cout << " CTR_B_LEVEL="	<<	0.0	<< endl;
				cout << " CTR_B_SLOPE="	<<	1	<< endl;
				cout << " gosub ~counter_Freq_B_Setup "	<< endl;
			}
		} else if(*arg4==0){
			cout << " CTR_A_LEVEL="	<<	*arg2	<< endl;
			cout << " CTR_A_SLOPE="	<<	*arg3	<< endl;
		} else {
			cout << " CTR_B_LEVEL="	<<	*arg2	<< endl;
			cout << " CTR_B_SLOPE="	<<	*arg3	<< endl;
		}
	}
}
//-----------------------------------------------------------------------
extern "C" {

void ctrrm( int * arg1, double * arg2)
	{
		cout	<< "call ctrrm ( "
			<< *arg1	<<	" , "
			<< "M[1]"	<<	" ) "
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
		//cout	<< "call enable ( "
		//	<< *arg1	<<	" , "
		//	<< *arg2	<<	" , "
		//	<< *arg3	<<	" ) "
		//	<< endl;
		if(*arg3==1){
			cout << " gosub ~counter_Setup " << endl;
		}
	}
}

//-----------------------------------------------------------------------
extern "C" {

void matsw( int * arg1,int *arg2,int *arg3,int *arg4)
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

void muxsw( int * arg1,int *arg2,int *arg3,int *arg4)
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

void modsw( int * arg1,int *arg2,int *arg3,int *arg4)
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

		cout	<< "call wtkls( "
			<< *arg1	<<	" , "
			<< 1		<<	" , "
			<< 0		<<	" ) "
			<< endl;
			
		cout	<< "call wtek ( "
			<< *arg1	<<	" , "
			<< 0		<<	" , "
			<< 0		<<	" , "
			<< *arg3	<<	" , "
			<< *arg2	<<	" * 2.88  , "
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

//-----------------------------------------------------------------------
extern "C" {
void tdhor( int * arg1,double * arg2, int * arg3, int * arg4, int * arg5)
	{
		cout	<< "call tdhor ( "
			<< *arg1	<<	" , "
			<< *arg2	<<	" ,"
			<< *arg3	<<	" , "
			<< *arg4	<<	" , "
			<< *arg5	<<	" ) "
			<< endl;
	}
}
//-----------------------------------------------------------------------
extern "C" {
void tdver( int * arg1,double * arg2,int * arg3,int * arg4,int * arg5,int * arg6,int * arg7)
	{
		cout	<< "call tdver ( "
			<< *arg1	<<	" , "
			<< *arg2	<<	" , "
			<< *arg3	<<	" , "
			<< *arg4	<<	" , "
			<< *arg5	<<	" , "
			<< *arg6	<<	" , "
			<< *arg7	<<	" ) "
			<< endl;
	}
}
//-----------------------------------------------------------------------
extern "C" {
void btime(int * arg1,double * arg2)
	{
		cout	<< "call btime ( "
			<< *arg1	<<	" , "
			<< *arg2	<<	" ) "
			<< endl;
	}
}

//-----------------------------------------------------------------------
extern "C" {
void tdfac()
	{
		cout	<< "call tdfac "
			<< endl;
	}
}

//-----------------------------------------------------------------------
extern "C" {
void tdmea( 	int *arg1,int *arg2,int *arg3,int *arg4,int * arg5,int * arg6,int * arg7,
		double *arg8,int *arg9)
	{
		cout	<< "call tdmea ( "
			<< *arg1	<<	" , "
			<< *arg2	<<	" ,"
			<< *arg3	<<	" , "
			<< *arg4	<<	" , "
			<< *arg5	<<	" , "
			<< *arg6	<<	" , "
			<< *arg7	<<	" , "
			<< *arg8	<<	" , "
			<< "M[1]"	<<	" , "
			<< "Uread"	<<	" ) "
			<< endl;
	}
}
