
#ifndef Sensor_h
#define Sensor_h

#include	"Resource.h"
#include	"ResourceSensor.h"

class SensorEM : public ResourceSensor{
	public:
		SensorEM( Resource * ,AST * nm,SensorEM * o=0);
		
		virtual void SETUPtoTedl();
		virtual void CONNECTtoTedl();
		virtual void DISCONNECTtoTedl();
		virtual void CHANGEtoTedl();
		virtual void REMOVEtoTedl();
		virtual void MEASUREtoTedl();
		virtual void MONITORtoTedl();
		virtual void VERIFYtoTedl();
		virtual void WAIT_FORtoTedl();
		virtual void ARMtoTedl();
		virtual void FETCHtoTedl();
		virtual void READtoTedl();

		virtual void InvalidatedResult();
		virtual void MeasuredtoTedl();
		virtual void RESETtoTedl();

		virtual void APPLYtoTedl();
		virtual void ENABLE_EVENTtoTedl();
		virtual void DISABLE_EVENTtoTedl();
		virtual void INITIATEtoTedl();
		virtual void COMPAREtoTedl();
		//virtual void SkipStatetoTedl();

		virtual void FSMError(const std::string& t, const std::string& s);

		// EVENTS for Sensor.
		virtual void ARM		(AST * a=0);
		virtual void CHANGE		(AST * a=0);
		virtual void COMPARE		(AST * a=0);
		virtual void CONNECT		(AST * a=0);
		virtual void DISABLE_EVENT	(AST * a=0);
		virtual void DISCONNECT		(AST * a=0);
		virtual void _DISCONNECT	(AST * a=0);
		virtual void ENABLE_EVENT	(AST * a=0);
		virtual void FETCH		(AST * a=0);
		virtual void INITIATE		(AST * a=0);
		virtual void MAXTIME		(AST * a=0);
		virtual void MEASURE		(AST * a=0);
		virtual void MEASUREMENT	(AST * a=0);
		virtual void MONITOR		(AST * a=0);
		virtual void READ		(AST * a=0);
		virtual void REMOVE		(AST * a=0);
		virtual void RESET		(AST * a=0);
		virtual void SETUP		(AST * a=0);
		virtual void VERIFY		(AST * a=0);
		virtual void AsyncReset		(AST * a=0);
		virtual void ResourceReset	(AST * a=0);
		
			
		virtual void APPLY		(AST * a=0);


		virtual void SkipState		(AST * a=0);
		virtual void Select		(AST * a=0);

		virtual void setResourceName( RWCString );
		virtual int resetFSM();

	protected:
		void Error(RWCString  e);
		
};

class Sensor : public SensorEM{
	public:
		Sensor( Resource *,AST * nm,Sensor * o=0 );
		AnalogResourceContext * clone(Resource *);
};

#endif // Sensor_h
