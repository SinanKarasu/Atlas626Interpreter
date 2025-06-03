
#include	"LabelAST.h"
#include	"Visitors.h"


LabelAST::LabelAST( ANTLRTokenPtr p, LabelType t, Scope * scope )
	:AST(p)
	,m_labelType(t)
	,m_Scope(scope)
	{
	}
	
LabelAST::~LabelAST(){
			//delete mytoken(token);
			delete m_Scope;
		}
AST *	LabelAST::data( AST * a )	{ return ASTdown()->data(a); }
AST *	LabelAST::label( AST * a  )	{ return this; }


int
LabelAST::ArgsCheck(AST * E1,AST * E2)
	{

		TheType         tt, st;
		int             maxArgs;
		int result=1;
		IntegerNumber idx(0);
		if (!E1) {
			//should never get here
			return 0;
		} else if (!E2) {
			//should never get here
			return 0;
		}

		int             args1 = E1->getInteger(0);
		int             args2 = E2->getInteger(0);

		if (args1 != args2) {
			Error_Report("Argument counts do not match.", E1);
			return 0;
		} else if (args1>0){

			for (int i = 1; i <= args1; i++) {
				idx.setInteger(0,i);
				AST            *td = E1->data(&idx);
				AST            *sd = E2->data(&idx);
				if (!(td->check(sd))) {
					Error_Report("Argument is not compatible.", E1);
					std::cerr << "Argument Number : " << i << std::endl;
					result=0;
				}
			}
		}
		int		ress1=0;
		int		ress2=0;
		if(((LabelAST *)E1)->getLabelType()==Procedure_Label){
			ress1 = E1->getInteger(1);
			ress2 = E2->getInteger(1);
		}

		if (ress1 != ress2) {
			Error_Report("Result counts do not match.", E1);
			return 0;
		} else if(ress1>0) {
			for (int i = 1; i <= ress1; i++) {
				idx.setInteger(0,args1+i);
				AST            *td = E1->data(&idx);
				AST            *sd = E2->data(&idx);
				if (!(td->check(sd))) {
					Error_Report("Result is not compatible.", E1);
					std::cerr << "Result Number : " << i << std::endl;
					result=0;
				}
			}
		}
		return result;

	}




AST *	LabelAST::eval( AST * a )
{
	AST *	what = ASTdown();	// see who is down there
	
	if ( what ){
		if ( a == this ){	// info about whom we represent. Ask
			what = what->eval(what);
		}else{			// just pass it down. Who knows what the intent is..
			what = what->eval(a);
		}
	}else if ( what = ASTright() ){	// if no one is down there go to the right....
		assert(0); // if does not work bring back the following
		//what = what->eval(a);	// I think this should be an error....
	}
	return what;
}

LabelType LabelAST::getLabelType() const		{ return m_labelType; }
Scope * LabelAST::getScope	() const		{ return m_Scope; }
TheType	LabelAST::getType	(AST * a) const	
		{
			if(ASTdown()){
				return ASTdown()->getType(a);
			} else {
				return UndefinedTypeValue;
			}
		}

AST *	LabelAST::Accept	( ASTVisitor & v )	{ return v.VisitLabelAST( this ); }
RWCString	LabelAST::getName() const {return token->getText();}

astream&	LabelAST::operator<<( astream &s )
	{
		s << getName() ;
		return s;
	}

ProgramNameLabel::ProgramNameLabel(ANTLRTokenPtr p,Scope * scope):LabelAST(p,Program_Name_Label,scope){};

AST *	ProgramNameLabel::eval( AST * a )
	{
		AST * where;
	
		if(ASTdown()){
		
			printf( " ProgramNameLabel should not have a child.... \n" );
			assert(0);
		}else if( where = ASTright() ){
		
			where = where->eval(a);
			return where;
		}
		return 0;
	}

ModuleNameLabel::ModuleNameLabel(ANTLRTokenPtr p,Scope * scope):LabelAST(p,Module_Name_Label,scope){};
NonAtlasModuleLabel::NonAtlasModuleLabel(ANTLRTokenPtr p,Scope * scope):LabelAST(p,NonAtlasModule_Name_Label,scope){};
BlockNameLabel::BlockNameLabel(ANTLRTokenPtr p,Scope * scope):LabelAST(p,Block_Name_Label,scope){};
ParameterLabel::ParameterLabel(ANTLRTokenPtr p,Scope * scope):LabelAST(p,Parameter_Label,scope){};

AST *	ParameterLabel::eval(AST * a)
	{
		if(a==this){
			return ASTdown()->eval(ASTdown());
		} else {
			return ASTdown()->eval(a);
		}
	}
LabelType ParameterLabel::getLabelType() const
	{
		return ((LabelAST *)ASTdown())->getLabelType();
	}

DrawingLabel  :: DrawingLabel	( ANTLRTokenPtr p, Scope * scope ) :LabelAST(p,Drawing_Label,scope){};
SignalLabel   :: SignalLabel	( ANTLRTokenPtr p, Scope * scope ) :LabelAST(p,Signal_Label,scope){};
ProcedureLabel:: ProcedureLabel	( ANTLRTokenPtr p, AtlasParser * ap, Scope * scope )
			:LabelAST(p,Procedure_Label,scope),_p(ap){};
	
void ProcedureLabel::setInteger(int indx,Long value)
	{
		if(indx==0){
			paramcount=value;
		} else if(indx=1){
			resultcount=value;
		}
	}
	
Long ProcedureLabel::getInteger(int indx) const
	{
		if(indx==0){
			return paramcount;
		} else if(indx=1){
			return resultcount;
		}
	}


AST *
ProcedureLabel::check(AST * a)
	{
		if(a){
			return (ArgsCheck(this,a)?this:0);
		} else {
			return ASTdown()->check(a);
		}
	}
	
AST * ProcedureLabel::eval(AST * a)
	{
		evalVisitor	go;
		
		AST * ap, * p, * r , * s , * ar;
		AST * pp;	// Parameters pointer
		AST * rr;	// Results pointer
		AST * ss;	// Saves pointer
		AST * aP;	// Actual Parameters pointer
		AST * aR;	// Actual Results pointer
		
		int i;

//sik		if ( a == this ){	// This is the insidious job of LeaveAction.
//sik			return this;	// LeaveAction will return to LineAction
//sik		}			// LineAction will pass it on to Execute

		pp = ASTdown()->ASTright();				// Parameters pointer
		rr = ASTdown()->ASTdown()->ASTright();			// Results pointer
		ss = ASTdown()->ASTdown()->ASTdown()->ASTright();	// Saves pointer
		aP = a->ASTright();					// Actual Parameters pointer
		aR = a->ASTdown()->ASTright();				// Actual Results pointer

		p=pp; ap=aP;
		for ( i=1; i<=paramcount; i++ )
		{
			p->eval()->assign( ap->eval() );	// eval and copy the args
			p  =  p->ASTright();
			ap = ap->ASTright();
		};

		r=rr; s=ss; ar=aR;
		for ( i=1; i<=resultcount; i++ )
		{
			s->setDown(r->ASTdown()->ASTdown());	// save the location pointer (VariableLable._down
			r->ASTdown()->setDown(ar);		// replace it with the passed address
			s  =  s->ASTright();
			ar = ar->ASTright();
		};
		
		Scope * scope = _p->getScope();
		{
			m_Scope->setCallChain(scope);	// Tell our scope where we came from...
		}
		_p->setScope( m_Scope );				// switch scope to the function
		go.Execute( ASTright(), this );			//eval the actual code
	
		r=rr; s=ss;
		for ( i=1; i<=resultcount; i++ )
		{
			r->ASTdown()->setDown( s->ASTdown() );	// restore the local addresses
			s = s->ASTright();
			r = r->ASTright();
		};
		
		{
			m_Scope->setCallChain(0);	// Tell our scope we are done...
		}
		
		_p->setScope( scope );		// restore the scope
		return 0;
	}


TheType	ProcedureLabel::getType(AST * a) const
	{
		// if a==0 then return the function return type
		// else return the type of i'th argument

		AST *	result=ASTdown();
		if(a){
			int argno=a->getInteger();
			if(argno<=paramcount){
				result=result->ASTright();
				for(int i=1;i<argno;i++){
					result=result->ASTright();
				}
			} else {
				result=result->ASTdown()->ASTright();
				for(int i=1;i< argno - paramcount;i++){
					result=result->ASTright();
				}
			}
		} else {
			result=0;
		}
		return result->getType();
	}

AST *
ProcedureLabel::data(AST * a)
	{
		// if a==0 then return the function return type
		// else return the type of i'th argument

		AST *	result=ASTdown();
		if(a){
			int argno=a->getInteger();
			if(argno<=paramcount){
				result=result->ASTright();
				for(int i=1;i<argno;i++){
					result=result->ASTright();
				}
			} else {
				result=result->ASTdown()->ASTright();
				for(int i=1;i< argno - paramcount;i++){
					result=result->ASTright();
				}
			}
		} else {
			result=0;
		}
		return result;
	}


FunctionLabel::FunctionLabel(ANTLRTokenPtr p,AtlasParser * ap,Scope * scope)
			:LabelAST(p,Function_Label,scope),_p(ap)
	{
	}
	
void
FunctionLabel::setInteger( int indx, Long value )
	{
		paramcount=value;
	}

AST *
FunctionLabel::check(AST * a)
	{
		if(a){
			return (ArgsCheck(this,a)?this:0);
		} else {
			return ASTdown()->check(a);
		}
	}


AST *
FunctionLabel::eval(AST * a)
	{
		evalVisitor	go;
				
		AST *	it_a = a;
		AST *	it_p = ASTdown();
		AST *	result;
		Scope *	scope;
		
		for( int i=1;  i <= paramcount;  i++ ){

			it_p->eval()->assign( it_a->eval() );	//eval and copy the args
			it_p = it_p->ASTright();
			it_a = it_a->ASTright();
		};
		
		scope = _p->getScope();
		{
			m_Scope->setCallChain(scope);	// Tell our scope where we came from...
		}

		_p->setScope( m_Scope );			// switch scope to the function

		
		go.Execute( ASTright(), this );			//eval the actual code
		
		result = _p->getTokenLabel( token->getText() );	// func name is the result var.
		result = result->eval( a );

		{
			m_Scope->setCallChain(0);	// Tell our scope we are done...
		}
		
		_p->setScope( scope );			// restore the scope
		
		return result;
	}

Long
FunctionLabel::getInteger(int indx) const
	{
                return paramcount; // transparent node
        };

TheType
FunctionLabel::getType(AST * a) const
	{
		// if a==0 then return the function return type
		// else return the type of i'th argument

		AST *	result;
		if(a){
			result=ASTdown();
			if(a->getInteger()<=paramcount){
				for(int i=1;i<a->getInteger();i++){
					result=result->ASTright();
				}
			} else {
				return UndefinedTypeValue;
			}
		} else {
			result=_p->getTokenLabel(token->getText(),m_Scope);
		}
		return result->getType();
	}

AST *
FunctionLabel::data(AST * a)
	{
		// if a==0 then return the function return type
		// else return the type of i'th argument

		AST *	result=ASTdown();
		if(a){
			int argno=a->getInteger();
			if(argno<=paramcount){
				for(int i=1;i<argno;i++){
					result=result->ASTright();
				}
			} else {
				result=0;
			}
		} else {
			result=_p->getTokenLabel(token->getText(),m_Scope);
		}
		return result;
	}


//------------------------------------------------------------------------

EventLabel::EventLabel(ANTLRTokenPtr p,Scope * scope)
	:LabelAST(p,Event_Label,scope),m_monitor(0)
	{

		extern TedlSymbolDictionary monitorEquivalence;
		extern TedlSymbolDictionary deviceEquivalence;
		if(monitorEquivalence.findValue(getName(),m_monitor)){
		} else if(deviceEquivalence.findValue(getName(),m_monitor)){
		} else {
			RWCString raw=unquoted(getName());
			m_monitor=new DeviceEquivalence(raw);
		}
	};

EventIntervalLabel::EventIntervalLabel(ANTLRTokenPtr p,Scope * scope):LabelAST(p,Event_Interval_Label,scope){};

EventIndicatorLabel::EventIndicatorLabel(ANTLRTokenPtr p,Scope * scope):LabelAST(p,Event_Indicator_Label,scope){};

ExchangeLabel::ExchangeLabel(ANTLRTokenPtr p,Scope * scope):LabelAST(p,Exchange_Label,scope){};

ProtocolLabel::ProtocolLabel(ANTLRTokenPtr p,Scope * scope):LabelAST(p,Protocol_Label,scope){};

AST *
ProtocolLabel::check(AST * a)
	{
		if(a){// Don't know what it is about but honor it....
			return ASTdown()->check(a);
		} else {
			StringAST dev(getName());
			ASTdown()->init(&dev);
			return ASTdown()->check();
		}
	}

DeviceIdentifierLabel::DeviceIdentifierLabel(ANTLRTokenPtr p,Scope * scope):LabelAST(p,Device_Identifier_Label,scope){};

ConfigurationLabel::ConfigurationLabel(ANTLRTokenPtr p,Scope * scope):LabelAST(p,Configuration_Label,scope){};

ExchangeConfigurationLabel::ExchangeConfigurationLabel(ANTLRTokenPtr p,Scope * scope):LabelAST(p,Exchange_Configuration_Label,scope){};

DigitalSourceLabel::DigitalSourceLabel(ANTLRTokenPtr p,Scope * scope):LabelAST(p,Digital_Source_Label,scope){};

DigitalSensorLabel::DigitalSensorLabel(ANTLRTokenPtr p,Scope * scope):LabelAST(p,Digital_Sensor_Label,scope){};

TimerLabel::TimerLabel(ANTLRTokenPtr p,Scope * scope):LabelAST(p,Timer_Label,scope){};

RequirementLabel::RequirementLabel(ANTLRTokenPtr p,Scope * scope):LabelAST(p,Requirement_Label,scope)
	{
		//extern TedlSymbolDictionary deviceEquivalence;
		//if(deviceEquivalence.findValue(getName(),dev_name)){
		//} else {
		//	dev_name=new DeviceEquivalence(unquoted(getName()));
		//}
	};

AST *
RequirementLabel::check(AST * a)
	{
		if(a){// Don't know what it is about but honor it....
			return ASTdown()->check(a);
		} else {
			StringAST dev(getName());
			ASTdown()->init(&dev);
			return ASTdown()->check();
		}
	}

ConstantIdentifierLabel::ConstantIdentifierLabel(ANTLRTokenPtr p,Scope * scope):LabelAST(p,Constant_Identifier_Label,scope){};

TypeIdentifierLabel::TypeIdentifierLabel(ANTLRTokenPtr p,Scope * scope)
	:LabelAST(p,Type_Identifier_Label,scope)
	{
	};
AST *	TypeIdentifierLabel::eval(AST *a)
	{
		return ASTdown()->eval();
	}

VariableIdentifierLabel::VariableIdentifierLabel(ANTLRTokenPtr p,Scope * scope):LabelAST(p,Variable_Identifier_Label,scope){};
AST *	VariableIdentifierLabel::eval( AST * a )
	{
		if ( a == this ){
			return ASTdown()->eval(ASTdown());
		}else{
			return ASTdown()->eval(a);
		}
	}
AST *	VariableIdentifierLabel::clone( Scope * s ) const
	{
		AST * x = new VariableIdentifierLabel( getToken() );
		
		x->setDown( ASTdown()->clone( s ) );
		
		return x;
	}

Proxy::Proxy(ANTLRTokenPtr p,Scope * scope):LabelAST(p,Variable_Identifier_Label,scope){};

AST *	Proxy::eval( AST * a )	// acts like a VariableIdentifierLabel 
	{
		if(ASTdown()){
			if ( a == this ){
				return ASTdown()->eval( ASTdown() );
			}else{
				return ASTdown()->eval( a );
			}
		} else {
			return 0;
		}
	}
LabelType Proxy::getLabelType() const
	{
		return ((LabelAST *)ASTdown())->getLabelType();
	}

AST *
Proxy::label	( AST * a  )
	{
		if(ASTdown()){
			return ASTdown()->label();
		} else {
			return 0;
		}
	}

astream& 
Proxy::operator <<	( astream& s )
	{
		s << ASTdown();
		return s;
	}



FileLabel::FileLabel( ANTLRTokenPtr p, Scope * scope ):LabelAST( p, File_Label, scope ){};

AST *	FileLabel::eval(AST * a)
	{
		if ( a == this ){
			return	ASTdown()->eval( ASTdown() );
		}else{
			astream * file = new astream;

			if ( getName() == "stdout" ){
			
				file->attach( 1 );			// attach cout or stdout.
				
			}else if ( getName() == "stdin" ){
			
				file->attach( 0 );			// attach cin or stdin.
			
			}else if ( getName() == "stderr" ){
			
				file->attach( 2 );			// attach cerr or stderr.
			}
			FileType	tmp( file );
			ASTdown()->assign( &tmp );
			ASTdown()->setInteger( 0, 1 );			// Enable it.
			return ASTdown();
		}
	}

EnumerationElementLabel::EnumerationElementLabel(ANTLRTokenPtr p,Scope * scope):LabelAST(p,Enumeration_Element_Label,scope){};

AST * EnumerationElementLabel::eval(AST * a)  {
		if(a==this){
			return ASTdown()->eval(ASTdown());
		} else {
			return ASTdown()->eval(a);
		}
}


ConnectionTypeLabel::ConnectionTypeLabel(ANTLRTokenPtr p,Scope * scope):LabelAST(p,Connection_Type_Label,scope){};

AST * ConnectionTypeLabel::eval(AST * a)  {
		if(a==this){
			return ASTdown()->eval(ASTdown());
		} else {
			return ASTdown()->eval(a);
		}
}

BusSpecificationLabel::BusSpecificationLabel(ANTLRTokenPtr p,Scope * scope):LabelAST(p,Bus_Specification_Label,scope){};

ModifierNameLabel::ModifierNameLabel(ANTLRTokenPtr p,Scope * scope):LabelAST(p,Modifier_Name_Label,scope){};

DimNameLabel::DimNameLabel(ANTLRTokenPtr p,Scope * scope):LabelAST(p,Dim_Name_Label,scope){};

PinDescriptorNameLabel::PinDescriptorNameLabel(ANTLRTokenPtr p,Scope * scope):LabelAST(p,Pin_Descriptor_Name_Label,scope){};

TerminalIdentifierLabel::TerminalIdentifierLabel(ANTLRTokenPtr p,Scope * scope):LabelAST(p,Terminal_Identifier_Label,scope){};
RWCString  TerminalIdentifierLabel::getName() const {return RWCString(RWCString("*")+token->getText());}


ProtocolParameterNameLabel::ProtocolParameterNameLabel(ANTLRTokenPtr p,Scope * scope):LabelAST(p,Protocol_Parameter_Name_Label,scope){};

BusParameterNameLabel::BusParameterNameLabel(ANTLRTokenPtr p,Scope * scope):LabelAST(p,Bus_Parameter_Name_Label,scope){};

BusModeNameLabel::BusModeNameLabel(ANTLRTokenPtr p,Scope * scope):LabelAST(p,Bus_Mode_Name_Label,scope){};

TestEquipRoleNameLabel::TestEquipRoleNameLabel(ANTLRTokenPtr p,Scope * scope):LabelAST(p,Test_Equip_Role_Name_Label,scope){};

