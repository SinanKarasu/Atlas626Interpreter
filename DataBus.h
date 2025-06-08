
#ifndef DataBus_h
#define DataBus_h

#include	"Resource.h"
#include	"ResourceDataBus.h"

class DataBusEM : public ResourceDataBus
{
	public:
		DataBusEM( Resource * ,AST * nm,DataBusEM * o=0);
		
		virtual void ENABLE_EXCHANGEtoTedl();
		virtual void CONNECT_EXCHANGEtoTedl();
		virtual void DO_EXCHANGEtoTedl();
		virtual void DISABLE_EXCHANGEtoTedl();
		virtual void DISCONNECT_EXCHANGEtoTedl();
		virtual void UPDATE_EXCHANGEtoTedl();
		virtual void FETCH_EXCHANGEtoTedl();
		virtual void do_HOLDtoTedl();
		virtual void do_PROCEEDtoTedl();


		virtual void InvalidatedResult();


		virtual void FSMError(const std::string& t, const std::string& s);

		// EVENTS for DataBus.
		virtual void ENABLE_EXCHANGE	(AST * a=0);
		virtual void CONNECT_EXCHANGE	(AST * a=0);
		virtual void DO_EXCHANGE	(AST * a=0);
		virtual void DISABLE_EXCHANGE	(AST * a=0);
		virtual void DISCONNECT_EXCHANGE(AST * a=0);
		virtual void UPDATE_EXCHANGE	(AST * a=0);
		virtual void FETCH_EXCHANGE	(AST * a=0);


		virtual void setResourceName( RWCString );
		virtual int resetFSM();

	protected:
		void Error(RWCString  e);
		
};

class DataBus : public DataBusEM
{
	public:
		DataBus( Resource *,AST * nm,DataBus * o=0 );
		DataBusResourceContext * clone(Resource *);
};

#endif // DataBus_h
