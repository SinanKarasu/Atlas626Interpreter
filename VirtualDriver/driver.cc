#include	<iostream>
#include	<string.h>

#include	<dlfcn.h>
extern void* 	library_handle;

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

typedef void (*Func_dmmsu)(short*,short*,short*,short*);

void dmmsu( int * arg1, int * arg2, int * arg3,int * arg4)
	{
		Func_dmmsu sub_dmmsu;

		short unit=*arg1;
		short func=*arg2;
		short range=*arg3;
		short filtr=*arg4;

		sub_dmmsu=(Func_dmmsu)dlsym(library_handle,"dmmsu_");
		sub_dmmsu(&unit,&func,&range,&filtr);
	}
}

//-----------------------------------------------------------------------
extern "C" {

typedef void (*Func_dmmmu)(short*,float*,short*);

void dmmmu( int * arg1, double * arg2, int * arg3)
	{
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
	}
}

extern "C" {

typedef void (*Func_dcvsh)(short*,float*,float*);

void dcvhirange( int * arg1, double * arg2, double * arg3)
	{
		Func_dcvsh dcvsh;
		float current_limit=*arg3;
		short unit=*arg1;
		float volts=*arg2;

		dcvsh=(Func_dcvsh)dlsym(library_handle,"dcv_");
		dcvsh(&unit,&volts,&current_limit);
	}
}

//-----------------------------------------------------------------------
extern "C" {

typedef void (*Func_dcvsl)(short*,float*,float*);

void dcvlorange( int * arg1, double * arg2, double * arg3)
	{
		Func_dcvsl dcvsl;
		float current_limit=*arg3;
		short unit=*arg1;
		float volts=*arg2;

		dcvsl=(Func_dcvsl)dlsym(library_handle,"dcv_");
		dcvsl(&unit,&volts,&current_limit);
	}
}
//-----------------------------------------------------------------------
extern "C" {

typedef void (*Func_uctrg)(short*,short*,short*,float*,short*,short*,short*,short*,short*);

void ctrsu( int *piUnit, float* pfTlev, int*)
	{
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
	}
}
//-----------------------------------------------------------------------
extern "C" {

typedef void (*Func_ctrrm)(float*,float*);

void ctrrm( int *, double *pdRval, int * arg3)
	{
		float fmode=1;
		float rval;
		Func_ctrrm sub_ctrrm=(Func_ctrrm)dlsym(library_handle,"ctrrm_");
		sub_ctrrm(&fmode,&rval);
		*pdRval=rval;
	}
}
//-----------------------------------------------------------------------
extern "C" {

typedef void (*Func_ucfun)(short*,short*,short*,short*);
typedef void (*Func_dasci)(short*,short*,char*);
typedef void (*Func_timct)(short*);

void ctrenab( int *)
	{
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
	}
}

//-----------------------------------------------------------------------
extern "C" {

typedef void (*Func_matsw)(short*,short*,short*,short*);

void matswitch( int * piOpenClose,int *,int *piBus,int *piRelay)
	{
		short smode=*piOpenClose;
		short sunit=1;
		short sslot=*piBus;
		short srelay=*piRelay;
		
		Func_matsw matsw=(Func_matsw)dlsym(library_handle,"matsw_");
		matsw(&smode,&sunit,&sslot,&srelay);
	}
}

//-----------------------------------------------------------------------
extern "C" {

typedef void (*Func_mutsw)(short*,short*,short*,short*);

void muxsw( int *piOpenClose,int*,int *piSlot,int *piRelay)
	{
		short smode=*piOpenClose;
		short sunit=1;
		short sslot=*piSlot;
		short srelay=*piRelay;
		
		Func_muxsw muxsw=(Func_matsw)dlsym(library_handle,"muxsw_");
		muxsw(&smode,&sunit,&sslot,&srelay);
	}
}
