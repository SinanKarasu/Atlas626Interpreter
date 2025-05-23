h48154
s 00000/00001/00217
d D 1.4 98/07/20 08:38:04 mas4189 6 4
i 5
c Accepted child's version in workspace "/home/mas4189/Atlas".
c 
e
s 00112/00080/00105
d D 1.2.1.1 98/07/20 08:35:40 mas4189 5 3
c added 182A subroutine calls
e
s 00019/00001/00184
d D 1.3 98/07/16 15:33:32 sinan 4 3
c Check in before big weekend cleanup
e
s 00144/00001/00041
d D 1.2 98/07/09 08:57:40 sinan 3 1
c 
e
s 00000/00000/00000
d R 1.2 97/10/27 14:07:23 Codemgr 2 1
c SunPro Code Manager data about conflicts, renames, etc...
c Name history : 1 0 VirtualDriver/driver.cc
e
s 00042/00000/00000
d D 1.1 97/10/27 14:07:22 frank 1 0
c checkin new
c 
e
u
U
f e 0
t
T
I 1
#include	<iostream>
#include	<string.h>

I 5
#include	<dlfcn.h>
extern void* 	library_handle;

E 5
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

D 3

E 3
I 3
//-----------------------------------------------------------------------
E 3
extern "C" {

void VirtualInstrumentDriver( int argc, char * argv[] )
	{
		VirtualInterface	x( argc, argv );
		
		x.print();
	}
}
I 3

//-----------------------------------------------------------------------
extern "C" {

I 5
typedef void (*Func_dmmsu)(short*,short*,short*,short*);

E 5
void dmmsu( int * arg1, int * arg2, int * arg3,int * arg4)
	{
D 5
		cout	<< "dmmsu called "
			<< *arg1
			<< " "
			<< *arg2
			<< " "
			<< *arg3
			<< " "
			<< *arg4
			<< endl;		
E 5
I 5
		Func_dmmsu sub_dmmsu;

		short unit=*arg1;
		short func=*arg2;
		short range=*arg3;
		short filtr=*arg4;

		sub_dmmsu=(Func_dmmsu)dlsym(library_handle,"dmmsu_");
		sub_dmmsu(&unit,&func,&range,&filtr);
E 5
	}
}

//-----------------------------------------------------------------------
extern "C" {

I 5
typedef void (*Func_dmmmu)(short*,float*,short*);

E 5
void dmmmu( int * arg1, double * arg2, int * arg3)
	{
D 5
		cout	<< "dmmmu called "
			<< *arg1
			<< " "
			<< *arg2
			<< " "
			<< *arg3
			<< " "
			<< endl;
		*arg2=1.234;		
E 5
I 5
		Func_dmmmu sub_dmmmu;

		
		short unit=*arg1;
		short num=*arg3;
		float* buf=new float[num];

		sub_dmmmu=(Func_dmmmu)dlsym(library_handle,"dmmmu_");
		sub_dmmmu(&unit,buf,&num);
		for(int i=0;i<num;i++){
			arg2[i]=buf[i];
		}
		delete[] buf;
E 5
	}
}

extern "C" {

I 5
typedef void (*Func_dcvsh)(short*,float*,float*);

E 5
void dcvhirange( int * arg1, double * arg2, double * arg3)
	{
D 5
		cout	<< "dcvhirange called "
			<< *arg1
			<< " "
			<< *arg2
			<< " "
			<< *arg3
			<< " "
			<< endl;
E 5
I 5
		Func_dcvsh dcvsh;
		float current_limit=*arg3;
		short unit=*arg1;
		float volts=*arg2;

		dcvsh=(Func_dcvsh)dlsym(library_handle,"dcv_");
		dcvsh(&unit,&volts,&current_limit);
E 5
	}
}

//-----------------------------------------------------------------------
extern "C" {

I 5
typedef void (*Func_dcvsl)(short*,float*,float*);

E 5
void dcvlorange( int * arg1, double * arg2, double * arg3)
	{
D 5
		cout	<< "dcvlorange called "
			<< *arg1
			<< " "
			<< *arg2
			<< " "
			<< *arg3
			<< " "
			<< endl;
E 5
I 5
		Func_dcvsl dcvsl;
		float current_limit=*arg3;
		short unit=*arg1;
		float volts=*arg2;

		dcvsl=(Func_dcvsl)dlsym(library_handle,"dcv_");
		dcvsl(&unit,&volts,&current_limit);
E 5
	}
}
//-----------------------------------------------------------------------
extern "C" {

D 5
void ctrsu( int * arg1, int* arg2, int * arg3)
E 5
I 5
typedef void (*Func_uctrg)(short*,short*,short*,float*,short*,short*,short*,short*,short*);

void ctrsu( int *piUnit, float* pfTlev, int*)
E 5
	{
D 5
		cout	<< "ctrsu called "
			<< *arg1
			<< " "
			<< *arg2
			<< " "
			<< *arg3
			<< " "
			<< endl;
E 5
I 5
		Func_uctrg uctrg;
		short sunit=1;
		short schan=*piUnit;
		short smode=1;
		float ftlev=*pfTlev;
		short sauto=0;
		short scoup=0;
		short sfltr=0;
		short sattn=1;
		short simp=0;

		uctrg=(Func_uctrg)dlsym(library_handle,"uctrg_");
		uctrg(&sunit,&schan,&smode,&ftlev,&sauto,&scoup,&sfltr,&sattn,&simp);
E 5
	}
}
//-----------------------------------------------------------------------
extern "C" {

D 5
void ctrrm( int * arg1, double * arg2, int * arg3)
E 5
I 5
typedef void (*Func_ctrrm)(float*,float*);

void ctrrm( int *, double *pdRval, int * arg3)
E 5
	{
D 5
		cout	<< "ctrrm called "
			<< *arg1
			<< " "
			<< *arg2
			<< " "
			<< *arg3
			<< " "
			<< endl;
		*arg2=0.12;
E 5
I 5
		float fmode=1;
		float rval;
		Func_ctrrm sub_ctrrm=(Func_ctrrm)dlsym(library_handle,"ctrrm_");
		sub_ctrrm(&fmode,&rval);
		*pdRval=rval;
E 5
	}
}
//-----------------------------------------------------------------------
extern "C" {

D 5
void ctrenab( int * arg1)
E 5
I 5
typedef void (*Func_ucfun)(short*,short*,short*,short*);
typedef void (*Func_dasci)(short*,short*,char*);
typedef void (*Func_timct)(short*);

void ctrenab( int *)
E 5
	{
D 5
		cout	<< "ctrenab called "
			<< *arg1
			<< " "
			<< endl;
E 5
I 5
		short stime=5.5+5;		
		Func_timct timct=(Func_timct)dlsym(library_handle,"timct_");
		timct(&stime);

		Func_ucfun ucfun;
		short sunit=1;
		short sfunc=2;
		short sres=2;
		short smult=5.5+3;
		ucfun=(Func_ucfun)dlsym(library_handle,"ucfun_");
		ucfun(&sunit,&sfunc,&sres,&smult);

		short slu=46;
		short slen=3;
		Func_dasci dasci=(Func_dasci)dlsym(library_handle,"dasci_");
		dasci(&slu,&slen,"WA1");
E 5
	}
}

//-----------------------------------------------------------------------
extern "C" {

D 5
void matswitch( int * arg1,int *arg2,int *arg3,int *arg4)
E 5
I 5
typedef void (*Func_matsw)(short*,short*,short*,short*);

void matswitch( int * piOpenClose,int *,int *piBus,int *piRelay)
E 5
	{
D 5
		cout	<< "matswitch called "
			<< *arg1
			<< " "
			<< *arg2
			<< " "
			<< *arg3
			<< " "
			<< *arg4
			<< " "
			<< endl;
E 5
I 5
		short smode=*piOpenClose;
		short sunit=1;
		short sslot=*piBus;
		short srelay=*piRelay;
		
		Func_matsw matsw=(Func_matsw)dlsym(library_handle,"matsw_");
		matsw(&smode,&sunit,&sslot,&srelay);
E 5
	}
}

//-----------------------------------------------------------------------
extern "C" {

D 4
D 5
void muxsw( int * arg1,int *arg2,int *arg3,int *arg4)
E 5
I 5
typedef void (*Func_mutsw)(short*,short*,short*,short*);

void muxsw( int *piOpenClose,int*,int *piSlot,int *piRelay)
E 5
E 4
I 4
D 6
void muxswitch( int * arg1,int *arg2,int *arg3,int *arg4)
E 6
E 4
	{
D 5
		cout	<< "muxsw called "
I 4
			<< *arg1
			<< " "
			<< *arg2
			<< " "
			<< *arg3
			<< " "
			<< *arg4
			<< " "
			<< endl;
	}
}

//-----------------------------------------------------------------------
extern "C" {

void modswitch( int * arg1,int *arg2,int *arg3,int *arg4)
	{
		cout	<< "muxsw called "
E 4
			<< *arg1
			<< " "
			<< *arg2
			<< " "
			<< *arg3
			<< " "
			<< *arg4
			<< " "
			<< endl;
E 5
I 5
		short smode=*piOpenClose;
		short sunit=1;
		short sslot=*piSlot;
		short srelay=*piRelay;
		
		Func_muxsw muxsw=(Func_matsw)dlsym(library_handle,"muxsw_");
		muxsw(&smode,&sunit,&sslot,&srelay);
E 5
	}
}
E 3
E 1
