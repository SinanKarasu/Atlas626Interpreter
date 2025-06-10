#include	"TedlDeviceAST.h"
#include	"BasicTypeAST.h"

#include	<dlfcn.h>

#include	"ExceptionTypes.h"

#include	"ExecEnv.h"
extern ExecEnv execEnv;

ResourceContextAST::ResourceContextAST(Resource * r,const RWCString parameter)
	:currentParameters(0),m_resource(r)
	{
		m_parameter= new StringAST(parameter);
	}

void 
ResourceContextAST::setResource( Resource * resource )
	{
		if(resource!=m_resource){
			assert(0);
		}
		return;
	}
	
Resource *
ResourceContextAST::getResource( )
	{
		return m_resource;
	}

AST *
ResourceContextAST::data	( AST *  a )
	{
		if(a==this){
			assert(0);	// don't know what to do
			return 0;
		} else if(a){
			AST * label=0;
			if(a->getName()=="MAX-TIME"){
				return getResource()->getMaxTime();
			}
			m_resource->m_callArgEntries->findValue(a->getName(),label);
			if(label){
				return label->data();	// TedlArgumentLabel... no arguments
			}
			
			label=currentParameters->data(a);
			if(label){
				return label;
			}
			
			label=currentParameters->data(m_parameter);
			if(a->getName()=="~PARAMETER"){
				return label;
			} else if(label){
				return label->data(a);
			}
			
			Error_Report("INTERNAL parameter not found "+getName(),this);
			return 0;
		} else {	// just return what we know about....
			return currentParameters->data(m_parameter);
		}
	}


AST *
ResourceContextAST::check	( AST *  a )
	{
		return currentParameters->check(a);
	}

AST *
ResourceContextAST::eval	( AST *  a )
	{
		AST * where=data();
		return where->eval(this);
	}

AST *
ResourceContextAST::assign	( AST *  a )
	{
		return currentParameters->assign(a);
	}


		
TedlDeviceAST::TedlDeviceAST(ANTLRTokenPtr p):AST(p){;};
TedlDeviceAST::~TedlDeviceAST(){};


void
TedlDeviceDriver::setArg(int argno,AST * source)
	{
		CallArg * target=&callArgs[argno];
		switch (source->getType())	{
		case IntegerNumberValue:
		case EnumerationTypeValue:
		case BooleanTypeValue:
		case BitTypeValue:
			{
				DimensionEntry * oldDim=source->getDimension();
				int x=source->getInteger();
				if(oldDim&&(oldDim->scale!=0.0)){
					x=ScaleDimension(source->getInteger(),oldDim,0);
				}
				target->Int=x;
				break;
			}
		
		case DecimalNumberValue:
			{
				DimensionEntry * oldDim=source->getDimension();
				double x=source->getDecimal();
				if(oldDim&&(oldDim->scale!=0.0)){
					x=ScaleDimension(x,oldDim,0);
				}
				target->Double=x;
				break;
			}
		
		default:
			{
				assert(0);
				//target->CharP=new char(source->str()->length()+1);
				//strcpy(target->CharP,*(source->str()));	
			}
		}	
	}



void
TedlDeviceDriver::getArg(int argno,AST * target)
	{
		CallArg * source=&callArgs[argno];

		switch (target->getType()){
		case IntegerNumberValue:
		case EnumerationTypeValue:
		case BooleanTypeValue:
		case BitTypeValue:
			{
				DimensionEntry * newDim=target->getDimension();
				int x=source->Int;
				if(newDim&&(newDim->scale!=0.0)){
					x=ScaleDimension(x,0,newDim);
				}
				IntegerNumber integerNumber(x);
				target->assign(&integerNumber);
				break;
			}
		case DecimalNumberValue:
			{
				DimensionEntry * newDim=target->getDimension();
				double x=source->Double;
				if(newDim&&(newDim->scale!=0.0)){
					x=ScaleDimension(x,0,newDim);
				}
				DecimalNumber decimalNumber(x);
				target->assign(&decimalNumber);
				break;
			}
		default:
			{
				assert(0);
				//target->CharP=new char(source->str()->length()+1);
				//strcpy(target->CharP,*(source->str()));	
			}
		}	
	}

void
TedlDeviceDriver::call_func()
{
	extern void * deviceDriverLibraryHandle;

	typedef	void  (*Func)(
				void*,void*,void*,void*,
				void*,void*,void*,void*,
				void*,void*,void*,void*,
				void*,void*,void*,void*);


	Func func;
		

	func = (Func)dlsym(deviceDriverLibraryHandle,getName().c_str());
	if ( func == NULL ){
		throw TedlExecutionError(RWCString("dlsym: ")+RWCString(dlerror()));
	}

	(*(func))
			(&callArgs[ 0],&callArgs[ 1],&callArgs[ 2],&callArgs[ 3],
			 &callArgs[ 4],&callArgs[ 5],&callArgs[ 6],&callArgs[ 7],
			 &callArgs[ 8],&callArgs[ 9],&callArgs[10],&callArgs[11],
			 &callArgs[12],&callArgs[13],&callArgs[14],&callArgs[15]
			);
}

TedlDeviceDriver::TedlDeviceDriver(ANTLRTokenPtr p)
	:TedlDeviceAST(p)
	,m_data(0)
	{
	}

//#include "strstream.h"
#include <sstream>

AST *
TedlDeviceDriver::init    ( AST * a )
	{
		AST * x;

		x=ASTdown();
		std::ostringstream ss;
		ss << getName();
		int f=0;
		while(x){
			AST * source = x->data(a);

			if(f++){ss <<",";} else { ss << "(";}
			switch (source->getType())	{
				case IntegerNumberValue:
				case EnumerationTypeValue:
				case BooleanTypeValue:
				case BitTypeValue:
				{
					DimensionEntry * oldDim=source->getDimension();
					int x=source->getInteger();
					if(oldDim&&(oldDim->scale!=0.0)){
						x=ScaleDimension(source->getInteger(),oldDim,0);
					}
					ss << x ;
					break;
				}
		
				case DecimalNumberValue:
				{
					DimensionEntry * oldDim=source->getDimension();
					double x=source->getDecimal();
					if(oldDim&&(oldDim->scale!=0.0)){
						x=ScaleDimension(x,oldDim,0);
					}
					ss << x ;
					break;
				}
		
				default:
				{
					assert(0);
					//target->CharP=new char(source->str()->length()+1);
					//strcpy(target->CharP,*(source->str()));	
				}
			}	

			x=x->ASTright();
		}
		ss << ")" <<  std::ends;
		
		//cout << ss.str() << endl;
		
		return m_data=new StringAST(ss.str());
	}
	
AST *
TedlDeviceDriver::eval(AST * a)
	{
		AST * x;

		if(execEnv.deviceTrace()){
			debugtrace << " Calling " << getName() << std::endl ;
		}
		
		if(getName()=="btime"){
			int ijkl=1234;
		}

		if ( a == this ){	// I am supposed to arbitrate, don't know how.
		
			x = ASTright();	// pass it to right...
			return x->eval(x);
		}else{
			x=ASTdown();
			int argCount=0;
			while(x){
				int ijk;
				if(x->getName()=="MAX-TIME"){
					ijk=1;
				} else if (x->getName()=="max-time") {
					ijk=2;
				}
				AST * d = x->data(a);
				setArg(argCount++,d);
				a->getResource()->setNounParameterValue(x->getName(),d);
				x=x->ASTright();
			}
			if(execEnv.deviceTrace()){
				debugtrace << argCount << " Arguments " << std::endl;
			}
			call_func();
			x=ASTdown();
			argCount=0;
			while(x){
				x->data(a);	// redundant...
				getArg(argCount++,x);
				x=x->ASTright();
			}
		}
		return ASTright();	// sequential execution
	}


//---------------------------------------------------------------------------------------//
TedlArgumentLabel::TedlArgumentLabel(ANTLRTokenPtr p,int get,int put)
	:TedlDeviceAST(p),m_get(get),m_put(put),m_data(0)
	{
	}
	
TheType
TedlArgumentLabel::getType	( AST * a  ) const
	{
		return ASTdown()->getType(a);
	}
	
AST *
TedlArgumentLabel::eval(AST * a)
	{
		AST * x;

		if ( a == this ){	// I am supposed to arbitrate, don't know how.
			return ASTdown()->eval(ASTdown());
		}else{
			return ASTdown()->eval(a);
				
		}
	}


AST *
TedlArgumentLabel::data	( AST *  a )
	{
		if(a){
			return m_data=a->data(this);
		} else {
			return m_data=ASTdown();
		}
	}
	
AST *
TedlArgumentLabel::clone( Scope * s ) const
	{
		AST * t = new TedlArgumentLabel(getToken(),m_get,m_put);
		if(ASTdown()){
			t->setDown(ASTdown()->clone( s ));
		}
		return t;
	};

AST *
TedlArgumentLabel::assign( AST * a )
	{
		if(m_put){
			//return m_data->assign(a);
			return ASTdown()->assign(a);
		} else {
			return 0;
		}
	};

//---------------------------------------------------------------------------------------//
TedlArgumentConstant::TedlArgumentConstant(ANTLRTokenPtr p)
	:TedlDeviceAST(p)
	{
	}
	
RWCString  TedlArgumentConstant::getName() const {
		return ASTdown()->getName();
	}
	
AST * TedlArgumentConstant::eval(AST * a)
	{
		AST * x;

		if ( a == this ){	// I am supposed to arbitrate, don't know how.
		
			x = ASTright();	// pass it to right... does down know???
			return x->eval(x);
		}else{
			if ( x=ASTdown() ){
				// cout << " performing down action " << endl;
				x=x->eval(a);
				
				if ( x ){
					// cout << "got a request from below" << endl;
					return x;
				}
			} 
		}
		return ASTright();	// sequential execution
	}


AST *
TedlArgumentConstant::data	( AST *  a )
	{
		return ASTdown();
	}
	
AST *
TedlArgumentConstant::clone( Scope * s ) const
	{
		AST * t = new TedlArgumentConstant(getToken());
		t->setDown(ASTdown()->clone( s ));
		return t;
	};

AST *
TedlArgumentConstant::assign( AST * a )
	{
		// can't assign to constant.
		return 0;
	};

//---------------------------------------------------------------------------------------//
TedlExternalLabel::TedlExternalLabel(NounEntry * n,ANTLRTokenPtr p,ModifierEntry * m,int get,int put )
	:m_nounEntry(n),TedlDeviceAST(p),m_modifierEntry(m),m_get(get),m_put(put),m_data(0)
	{
	}
	
RWCString  TedlExternalLabel::getName() const {

	return m_modifierEntry->modifier;
}

TheType
TedlExternalLabel::getType	( AST * a  ) const
	{
		return m_data->getType(a);
	}
	
AST *
TedlExternalLabel::eval(AST * a)
	{
		AST * x;

		if ( a == this ){	// I am supposed to arbitrate, don't know how.
		
			x = ASTright();	// pass it to right... does down know???
			return x->eval(x);
		}else{
			if ( x=ASTdown() ){
				// cout << " performing down action " << endl;
				x=x->eval(a);
				
				if ( x ){
					// cout << "got a request from below" << endl;
					return x;
				}
			} 
		}
		return ASTright();	// sequential execution
	}


AST *
TedlExternalLabel::data	( AST *  a )
	{
		if(a){
			m_data=a->data(this);
			if(!m_data){
				Error_Report("TEDL:Modifier for External undefined:"+m_modifierEntry->modifier,this);
			}
		} else {
			m_data=0;
		}
		return m_data;
	}

AST *
TedlExternalLabel::assign( AST * a )
	{
		if(m_put){
			return m_data->assign(a);
		} else {
			return 0;
		}
	};


AST *
TedlExternalLabel::clone( Scope * s ) const
	{
		AST * t = new TedlExternalLabel(m_nounEntry,getToken(),m_modifierEntry,m_get,m_put);
		return t;
	};



TedlProxy::TedlProxy(AST * label,int get,int put)
	:TedlDeviceAST(0),m_get(get),m_put(put),m_data(0)
	{
		setDown(label);
	}

RWCString  TedlProxy::getName() const {

	return ASTdown()->getName();
}


AST *
TedlProxy::eval( AST * a )	// acts like a TedlArgumentLabel 
	{
		return 0;
	}

AST *
TedlProxy::data	( AST *  a )
	{
		m_data = a->data(this);
		if(!m_data){
			Error_Report("INTERNAL parameter not found "+getName(),this);
		}
		return m_data;
	}

AST *
TedlProxy::assign( AST * a )
	{
		if(m_put){
			return m_data->assign(a);
		} else {
			return 0;
		}
	};

