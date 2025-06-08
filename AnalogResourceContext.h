
#ifndef AnalogResourceContext_H
#define AnalogResourceContext_H

#include <iostream>
#include <stdlib.h>
#include <string>


#include	"ResourceContextBASE.h"

class AnalogResourceContext: public ResourceContextBASE { 
	public:
		AnalogResourceContext();
		virtual void SETUPtoTedl	();
		virtual void APPLYtoTedl	()=0;
		virtual void CONNECTtoTedl	();
		virtual void DISCONNECTtoTedl	();
		virtual void CHANGEtoTedl	();
		virtual void REMOVEtoTedl	();
		virtual void ENABLE_EVENTtoTedl	();
		virtual void DISABLE_EVENTtoTedl();
		virtual void MEASUREtoTedl	();
		virtual void MONITORtoTedl	();
		virtual void VERIFYtoTedl	();
		virtual void WAIT_FORtoTedl	()=0;
		virtual void ARMtoTedl		();
		virtual void FETCHtoTedl	()=0;
		virtual void READtoTedl		()=0;
		virtual void INITIATEtoTedl	()=0;

		virtual void InvalidatedResult	()=0;
		virtual void MeasuredtoTedl	()=0;
		virtual void COMPAREtoTedl	()=0;
		virtual void RESETtoTedl	()=0;

		virtual void SkipStatetoTedl();
		virtual void AsyncResettoTedl();
		virtual void ResourceResettoTedl();
	
		virtual void FSMError(const std::string& t, const std::string& s)=0;
				
		// Corresponding Events for Action Functions.
		virtual void APPLY		(AST * a=0)=0;
		virtual void ARM		(AST * a=0)=0;
		virtual void CHANGE		(AST * a=0)=0;
		virtual void COMPARE		(AST * a=0)=0;
		virtual void CONNECT		(AST * a=0)=0;
		virtual void DISABLE_EVENT	(AST * a=0)=0;
		virtual void DISCONNECT		(AST * a=0)=0;
		virtual void _DISCONNECT	(AST * a=0)=0;
		virtual void ENABLE_EVENT	(AST * a=0)=0;
		virtual void FETCH		(AST * a=0)=0;
		virtual void INITIATE		(AST * a=0)=0;
		virtual void MAXTIME		(AST * a=0)=0;
		virtual void MEASURE		(AST * a=0)=0;
		virtual void MEASUREMENT	(AST * a=0)=0;
		virtual void MONITOR		(AST * a=0)=0;
		virtual void READ		(AST * a=0)=0;
		virtual void REMOVE		(AST * a=0)=0;
		virtual void RESET		(AST * a=0)=0;
		virtual void SETUP		(AST * a=0)=0;
		virtual void VERIFY		(AST * a=0)=0;
		virtual void SkipState		(AST * a=0)=0;
		virtual void Select		(AST * a=0)=0;
		virtual void AsyncReset		(AST * a=0)=0;
		virtual void ResourceReset	(AST * a=0)=0;
		
		virtual void setResourceName( RWCString )=0;
		virtual void preSetContext();
	
		virtual int resetFSM();

		virtual AnalogResourceContext * clone(Resource *);

public:
		AnalogResourceContext * m_Original;
		
		AST * m_transitionSigChar;
protected:
		void toEVENTS(void (AnalogResourceContext::*c)(AST *));
protected:
		AST * m_noun_modifier;


};

#endif // AnalogResourceContext_H
