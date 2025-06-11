
#include	"BuiltinFunctionAST.h"
#include	"BasicTypeAST.h"
#include	"Visitors.h"

#define FIX_THIS 0

BuiltinFunctionAST::BuiltinFunctionAST( ANTLRTokenPtr p ){}
BuiltinFunctionAST::~BuiltinFunctionAST(){}

AST *	BuiltinFunctionAST::eval	( AST * a )		{ assert(0); return 0; };
TheType	BuiltinFunctionAST::getType	( AST * a ) const	{ assert(0); return UndefinedTypeValue; };
AST *	BuiltinFunctionAST::Accept	( ASTVisitor & v )	{ return v.VisitBuiltinFunctionAST( this ); };

double	BuiltinFunctionAST::rad( double x ){ return ((x/45.0)*atan(1.0));}
double	BuiltinFunctionAST::deg( double x ){ return ((x*45.0)/atan(1.0));}

//--------------------------------------------------------------------
EOFFunction::EOFFunction():BuiltinFunctionAST(){ ast = new BooleanType; }

AST *	EOFFunction::eval( AST * a )
	{
		ast->setInteger(0,ASTdown()->getInteger(2));
		return ast;
	}

TheType	EOFFunction::getType	( AST * a ) const { return BooleanTypeValue; };

//--------------------------------------------------------------------
ODDFunction::ODDFunction():BuiltinFunctionAST(){ ast = new BooleanType; }

AST *	ODDFunction::eval( AST * a ){

		BooleanType temp( (ASTdown()->eval()->getInteger() % 2) == 1 );
		ast->assign( &temp );
	
		return ast;
	}

TheType	ODDFunction::getType	( AST * a ) const { return BooleanTypeValue; };

//--------------------Parity Even Functions---------------------------
PEFunction::PEFunction():BuiltinFunctionAST(){ ast = new BitType; }

AST *	PEFunction::eval( AST * a ){
		AST *			x = ASTdown()->eval();
		const RWBitVec *	y = x->vec();
		RWBoolean		result = (*y)[0];

		for ( int cnt=1; cnt < x->length(); ++cnt )
			result ^= (*y)[cnt];
	
		BitType temp( !result );
		ast->assign( &temp );

		return ast;
	}

TheType	PEFunction::getType	( AST * a ) const { return BitTypeValue; };

//---------------------Parity Odd Functions---------------------------
POFunction::POFunction():BuiltinFunctionAST(){ ast = new BitType; }

AST *	POFunction::eval( AST * a ){
		AST *			x = ASTdown()->eval();
		const RWBitVec *	y = x->vec();
		RWBoolean		result = (*y)[0];

		for ( int cnt=1; cnt < x->length(); ++cnt )
			result ^= (*y)[cnt];
		
		BitType temp( result );
		ast->assign( &temp );

		return ast;
	}

TheType	POFunction::getType	( AST * a ) const { return BitTypeValue; };

//--------------------------------------------------------------------
ORDFunction::ORDFunction():BuiltinFunctionAST(){ ast = new IntegerNumber; }

AST *	ORDFunction::eval( AST * a ){

		IntegerNumber temp( ASTdown()->eval()->getInteger() );
		ast->assign( &temp );
	
		return ast;
	}

TheType	ORDFunction::getType	( AST * a ) const { return IntegerNumberValue; };

//--------------------------------------------------------------------
LENFunction::LENFunction():BuiltinFunctionAST(){ ast = new IntegerNumber; }

AST *	LENFunction::eval( AST * a ){
	
		IntegerNumber temp( ASTdown()->eval()->length() );
		ast->assign( &temp );
	
		return ast;
	}

TheType	LENFunction::getType	( AST * a ) const { return IntegerNumberValue; };

//--------------------------------------------------------------------
LOCNFunction::LOCNFunction():BuiltinFunctionAST(){ ast = new IntegerNumber; }

AST *	LOCNFunction::eval( AST * a ){

		AST * x = ASTdown()->eval();			// 1st parameter.
		AST * y = ASTdown()->ASTright()->eval();	// 2nd parameter.
	
		IntegerNumber temp( x->index( y ) );
		ast->assign( &temp );

		return ast;
	}

TheType	LOCNFunction::getType	( AST * a ) const { return IntegerNumberValue; };

//--------------------------------------------------------------------
COUNTFunction::COUNTFunction():BuiltinFunctionAST(){ ast = new IntegerNumber; }

AST *	COUNTFunction::eval( AST * a ){

		AST * x		= ASTdown()->eval();			// 1st parameter.
		AST * y		= ASTdown()->ASTright()->eval();	// 2nd parameter.
		
		IntegerNumber temp( y->count( x ) );
		ast->assign( &temp );

		return ast;
	}

TheType	COUNTFunction::getType	( AST * a ) const { return IntegerNumberValue; };

//--------------------------------------------------------------------
SIZEFunction::SIZEFunction():BuiltinFunctionAST(){ ast = new IntegerNumber; }

AST *	SIZEFunction::eval( AST * a )
	{
		ast->setInteger(0,ASTdown()->getInteger(1));
		return ast;
	}

TheType	SIZEFunction::getType	( AST * a ) const { return IntegerNumberValue; };

//--------------------------------------------------------------------
SUCCFunction::SUCCFunction():BuiltinFunctionAST(){}

AST *	SUCCFunction::eval( AST * a )	{ return ASTdown()->eval()->succ(); }

TheType	SUCCFunction::getType	( AST * a ) const { return ASTdown()->getType(); };

//--------------------------------------------------------------------
PREDFunction::PREDFunction():BuiltinFunctionAST(){}

AST *	PREDFunction::eval( AST * a )	{ return ASTdown()->eval()->pred(); }

TheType	PREDFunction::getType	( AST * a ) const { return ASTdown()->getType(); };

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

DATEFunction::DATEFunction():BuiltinFunctionAST(){ ast = 0; }

AST* DATEFunction::eval(AST* ast) {
    using namespace std::chrono;

    // Get current time
    auto now = system_clock::now();
    auto secs = system_clock::to_time_t(now);
    auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;

    // Format time
    std::ostringstream oss;
    oss << std::put_time(std::localtime(&secs), "%y%m%d08/06/97M%S")
        << std::setw(3) << std::setfill('0') << ms.count();

    std::string result = oss.str();

    delete ast;
    // is it safe to pass address of stack object here? It is not saved, only contents are copied.
    ast = new StringOfCharType(&result);

    return ast;
}


TheType	DATEFunction::getType	( AST * a ) const { return StringOfCharTypeValue; };

//--------------------------------------------------------------------
CHARFunction::CHARFunction():BuiltinFunctionAST(){}

AST *	CHARFunction::eval( AST * a )	{ return FIX_THIS; }

TheType	CHARFunction::getType	( AST * a ) const { return CharTypeValue; };

//--------------------------------------------------------------------
BITSFunction::BITSFunction():BuiltinFunctionAST(){}

AST *	BITSFunction::eval( AST * a )	{ return FIX_THIS; }

TheType	BITSFunction::getType	( AST * a ) const { return StringOfBitTypeValue; };

//--------------------------------------------------------------------
COPYstocFunction::COPYstocFunction():BuiltinFunctionAST(){ast=0;}

AST *	COPYstocFunction::eval( AST * a )
	{
		AST * t=ASTdown();
		StringOfCharType * x=(StringOfCharType *)(t->eval());
		int len_x= x->length(); // dynamic length
		t=t->ASTright();int y=t->eval()->getInteger();
		t=t->ASTright();int z=t->eval()->getInteger();
		delete ast;
		if( len_x-y+1 < 1) {
			RWCString empty;
			ast=new StringOfCharType(&empty,0);
			return ast; // no copying occurs
		}
		if(z>len_x-y+1){
			z=len_x-y+1;
		}
		RWCString copy=x->str()->substr(y-1,z);
		ast=new StringOfCharType(&copy,z);
		return ast;
	}

TheType	COPYstocFunction::getType	( AST * a ) const { return ASTdown()->getType(); };

//--------------------------------------------------------------------
COPYstobFunction::COPYstobFunction():BuiltinFunctionAST(){ast=0;}

AST *	COPYstobFunction::eval( AST * a )
	{
		AST * t=ASTdown();
		StringOfBitType * x=(StringOfBitType *)(t->eval());
		int len_x= x->length(); // dynamic length
		t=t->ASTright();int y=t->eval()->getInteger();
		t=t->ASTright();int z=t->eval()->getInteger();
		delete ast;
		if( len_x-y+1 < 1) {
			RWBitVec empty;
			ast=new StringOfBitType(&empty,0);
			return ast; // no copying occurs
		}
		if(z>len_x-y+1){
			z=len_x-y+1;
		}
		RWBitVec copy(z);// length z
		int i;
		for(i=0;i<z;i++){
			copy[i]==(*x->vec())[y-1+z];
		}
		ast=new StringOfBitType(&copy,z);
		return ast;
	}

TheType	COPYstobFunction::getType	( AST * a ) const { return ASTdown()->getType(); };

//--------------------------------------------------------------------

int modulo(const int x,const int y){ int z=x % y; while(z<0)z+=y;return z; }

ROTATEFunction::ROTATEFunction():BuiltinFunctionAST(){ ast = 0; }

AST *	ROTATEFunction::eval( AST * a )
	{
		int i,j;
		AST * x=ASTdown()->eval();
		AST * y=ASTdown()->ASTright()->eval();
		int yint=y->getInteger();
		int n=x->length();
		const RWBitVec * xa=x->vec();
		delete ast;
		RWBitVec target(n);
		for(i=0,j=yint;i<n;i++,j++){
			target[i]=(*xa)[modulo(j,n)];
		}
		ast = new StringOfBitType(&target,n);
		return ast;
	}

TheType	ROTATEFunction::getType	( AST * a ) const { return StringOfBitTypeValue; };

//--------------------------------------------------------------------
SHIFTFunction::SHIFTFunction():BuiltinFunctionAST(){ast = 0;}

AST *	SHIFTFunction::eval( AST * a )
	{
		int i,j;
		AST * x=ASTdown()->eval();
		AST * y=ASTdown()->ASTright()->eval();
		int yint=y->getInteger();
		int n=x->length();
		const RWBitVec * xa=x->vec();
		delete ast;
		RWBitVec target(n);
		for(i=0,j=yint;i<n;i++,j++){
			if(j>=0 && j<n){
				target[i]=(*xa)[j];
			} else {
				target[i]=0;
			}
				
		}
		ast = new StringOfBitType(&target,n);
		return ast;
	}

TheType	SHIFTFunction::getType	( AST * a ) const { return StringOfBitTypeValue; };

//--------------------------------------------------------------------
A_SHIFTFunction::A_SHIFTFunction():BuiltinFunctionAST(){ast = 0;}

AST *	A_SHIFTFunction::eval( AST * a )
	{
		int i,j;
		AST * x=ASTdown()->eval();
		AST * y=ASTdown()->ASTright()->eval();
		int yint=y->getInteger();
		int n=x->length();
		const RWBitVec * xa=x->vec();
		RWBitVec target(n);
		int msb=(*xa)[n-1];
		delete ast;
		target[n-1]=msb;
		for(i=0,j=yint;i<n-1;i++,j++){
			if(j>=0 && j<n) {
				target[i]=(*xa)[j];
			} else if(j>=n ) {
				target[i]=msb;	// yint positive case
			} else {
				target[i]=0;	// yint negative case
			}
		}
		ast = new StringOfBitType(&target,n);
		return ast;
	}

TheType	A_SHIFTFunction::getType	( AST * a ) const { return StringOfBitTypeValue; };

//--------------------------------------------------------------------
DIGFunction::DIGFunction():BuiltinFunctionAST(){}

AST *	DIGFunction::eval( AST * a )	{ return FIX_THIS; }

TheType	DIGFunction::getType	( AST * a ) const { return StringOfBitTypeValue; };

//--------------------------------------------------------------------
DECFunction::DECFunction():BuiltinFunctionAST(){}

AST *	DECFunction::eval( AST * a )	{ return FIX_THIS; }

TheType	DECFunction::getType	( AST * a ) const { return DecimalNumberValue; };

//--------------------------------------------------------------------
INTFunction::INTFunction():BuiltinFunctionAST(){ ast = new IntegerNumber; }

AST *	INTFunction::eval( AST * a ){

		AST *	x = ASTdown()->eval();

		ast->setInteger( 0, int(x->getDecimal()) );
		return ast;
	}

TheType	INTFunction::getType	( AST * a ) const { return IntegerNumberValue; };

//--------------------------------------------------------------------
ROUNDFunction::ROUNDFunction():BuiltinFunctionAST(){ ast = new IntegerNumber; }

AST *	ROUNDFunction::eval( AST * a ){

		AST *	x  = ASTdown()->eval();
		double	xx = x->getDecimal();

		if ( xx > 0.0 ){
			xx += 0.5;
		}else{
			xx -= 0.5;
		}
		ast->setInteger( 0, Long(xx) );
		return ast;
	}

TheType	ROUNDFunction::getType	( AST * a ) const { return IntegerNumberValue; };

//--------------------------------------------------------------------
ABSFunction::ABSFunction():BuiltinFunctionAST(){ ast = 0; }

AST *	ABSFunction::eval( AST * a ){

		AST *	x = ASTdown()->eval();

		if ( x->getType() == IntegerNumberValue ){
	
			if ( ast == 0 ) ast = new IntegerNumber;
			ast->setInteger( 0, llabs( x->getInteger() ) );
		}else{
			if ( ast == 0 ) ast = new DecimalNumber;
			ast->setDecimal( 0, fabs( x->getDecimal() ) );
		}
		return ast;
	}

TheType	ABSFunction::getType	( AST * a ) const { return ASTdown()->getType(); };

//--------------------------------------------------------------------
SINFunction::SINFunction():BuiltinFunctionAST(){ ast = new DecimalNumber; }

AST *	SINFunction::eval( AST * a ){

		DecimalNumber tmp( sin( rad( ASTdown()->eval()->getDecimal() ) ) );
		ast->assign( &tmp );
		return ast;
	}

TheType	SINFunction::getType	( AST * a ) const { return DecimalNumberValue; };

//--------------------------------------------------------------------
COSFunction::COSFunction():BuiltinFunctionAST(){ ast = new DecimalNumber; }

AST *	COSFunction::eval( AST * a ){

		DecimalNumber tmp( cos( rad( ASTdown()->eval()->getDecimal() ) ) );
		ast->assign( &tmp );
		return ast;
	}

TheType	COSFunction::getType	( AST * a ) const { return DecimalNumberValue; };

//--------------------------------------------------------------------
TANFunction::TANFunction():BuiltinFunctionAST(){ ast = new DecimalNumber; }

AST *	TANFunction::eval( AST * a ){

		DecimalNumber tmp( tan( rad( ASTdown()->eval()->getDecimal() ) ) );
		ast->assign( &tmp );
		return ast;
	}

TheType	TANFunction::getType	( AST * a ) const { return DecimalNumberValue; };

//--------------------------------------------------------------------
LNFunction::LNFunction():BuiltinFunctionAST(){ ast = new DecimalNumber; }

AST *	LNFunction::eval( AST * a ){

		DecimalNumber tmp( log( ASTdown()->eval()->getDecimal() ) );
		ast->assign( &tmp );
		return ast;
	}

TheType	LNFunction::getType	( AST * a ) const { return DecimalNumberValue; };

//--------------------------------------------------------------------
LOGFunction::LOGFunction():BuiltinFunctionAST(){ ast = new DecimalNumber; }

AST *	LOGFunction::eval( AST * a ){

		DecimalNumber tmp( log10( ASTdown()->eval()->getDecimal() ) );
		ast->assign( &tmp );
		return ast;
	}

TheType	LOGFunction::getType	( AST * a ) const { return DecimalNumberValue; };

//--------------------------------------------------------------------
ALOGFunction::ALOGFunction():BuiltinFunctionAST(){ ast = new DecimalNumber; }

AST *	ALOGFunction::eval( AST * a ){

		DecimalNumber tmp( pow( 10.0, ASTdown()->eval()->getDecimal() ) );
		ast->assign( &tmp );
		return ast;
	}

TheType	ALOGFunction::getType	( AST * a ) const { return DecimalNumberValue; };

//--------------------------------------------------------------------
ATANFunction::ATANFunction():BuiltinFunctionAST(){ ast = new DecimalNumber; }

AST *	ATANFunction::eval( AST * a ){

		DecimalNumber tmp( deg( atan( ASTdown()->eval()->getDecimal() ) ) );
		ast->assign( &tmp );
		return ast;
	}

TheType	ATANFunction::getType	( AST * a ) const { return DecimalNumberValue; };

//--------------------------------------------------------------------
EXPFunction::EXPFunction():BuiltinFunctionAST(){ ast = new DecimalNumber; }

AST *	EXPFunction::eval( AST * a ){

		DecimalNumber tmp( exp( ASTdown()->eval()->getDecimal() ) );
		ast->assign( &tmp );
		return ast;
	}

TheType	EXPFunction::getType	( AST * a ) const { return DecimalNumberValue; };

//--------------------------------------------------------------------
SQRTFunction::SQRTFunction():BuiltinFunctionAST(){ ast = new DecimalNumber; }

AST *	SQRTFunction::eval( AST * a ){

		DecimalNumber tmp( sqrt( ASTdown()->eval()->getDecimal() ) );
		ast->assign( &tmp );
		return ast;
	}

TheType	SQRTFunction::getType	( AST * a ) const { return DecimalNumberValue; };

//--------------------------------------------------------------------
DELETEstocFunction::DELETEstocFunction():BuiltinFunctionAST(){}

AST *	DELETEstocFunction::eval( AST * a )
	{
		AST * t=ASTdown();
		StringOfCharType * s=(StringOfCharType *)(t->eval());
		int len_s= s->length(); // dynamic length
		t=t->ASTright();int p=t->eval()->getInteger();
		t=t->ASTright();int w=t->eval()->getInteger();
		delete ast;
		RWCString copy=(*s->str());
		if( (1<=p) && (p<=len_s)) {
			if(w>len_s-p+1)w=len_s-p+1;
		} else {
			p=len_s;	w=0;	// my interpretation. sik.
		}			
		copy.erase(p-1,w);
		return ast = new StringOfCharType(&copy,len_s-w);

	}

TheType	DELETEstocFunction::getType	( AST * a ) const { return ASTdown()->getType(); };

//--------------------------------------------------------------------
DELETEstobFunction::DELETEstobFunction():BuiltinFunctionAST(){}

AST *	DELETEstobFunction::eval( AST * a )
	{
		AST * t=ASTdown();
		StringOfBitType * s=(StringOfBitType *)(t->eval());
		int len_s= s->length(); // dynamic length
		t=t->ASTright();int p=t->eval()->getInteger();
		t=t->ASTright();int w=t->eval()->getInteger();
		delete ast;
		if( (1<=p) && (p<=len_s)) {
			if(w>len_s-p+1)w=len_s-p+1;
		} else {
			p=len_s;	w=0;	// my interpretation. sik.
		}
		RWBitVec copy(len_s-w);
		int i;
		for(i=0;i<len_s-w;i++){
			copy[i]=(*s->vec())[(i<p)?i:i+w];
		}
		return ast=new StringOfBitType(&copy,len_s-w);
				
	}

TheType	DELETEstobFunction::getType	( AST * a ) const { return ASTdown()->getType(); };


//--------------------------------------------------------------------
INSERTstocFunction::INSERTstocFunction():BuiltinFunctionAST(){}

AST *	INSERTstocFunction::eval( AST * a )
	{
		AST * x=ASTdown();
		StringOfCharType * s=(StringOfCharType *)(x->eval());
		x=x->ASTright();
		StringOfCharType * t=(StringOfCharType *)(x->eval());
		x=x->ASTright();
		int p=t->eval()->getInteger();
		int len_s= s->length(); // dynamic length
		int len_t= t->length(); // dynamic length
		delete ast;
		RWCString copy=(*t->str());
		if( (1<=p) && (p<=len_t)) {
			copy.insert(p,*s->str());
		} else {
			len_t=0;	// my interpretation. sik.
		}
		return ast = new StringOfCharType(&copy,len_s+len_t);
	}

TheType	INSERTstocFunction::getType	( AST * a ) const { return ASTdown()->getType(); };

//--------------------------------------------------------------------
INSERTstobFunction::INSERTstobFunction():BuiltinFunctionAST(){}

AST *	INSERTstobFunction::eval( AST * a )
	{
		AST * x=ASTdown();
		StringOfBitType * s=(StringOfBitType *)(x->eval());
		x=x->ASTright();
		StringOfBitType * t=(StringOfBitType *)(x->eval());
		x=x->ASTright();
		int p=t->eval()->getInteger();
		int len_s= s->length(); // dynamic length
		int len_t= t->length(); // dynamic length
		delete ast;
		if( (1<=p) && (p<=len_t)) {
		} else {
			len_s=0;	// my interpretation. sik.
		}
		RWBitVec copy(len_s+len_t);
		int i;
		for(i=0;i<len_s+len_t;i++){
			if(i<p){
				copy[i]=(*t->vec())[i];
			} else if(i<(p+len_s)){
				copy[i]=(*s->vec())[i-p];
			} else {
				copy[i]=(*t->vec())[i-(p+len_s)];
			}
		}
		
		
		return ast = new StringOfBitType(&copy,len_s+len_t);
	}

TheType	INSERTstobFunction::getType	( AST * a ) const { return ASTdown()->getType(); };



