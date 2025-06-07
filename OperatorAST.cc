
#include	"OperatorAST.h"
#include	"BasicTypeAST.h"
#include	"Visitors.h"

OperatorAST::OperatorAST( ANTLRTokenPtr p ):AST( p ){ ; }
OperatorAST::~OperatorAST(){ delete ast; }

TheType	OperatorAST::getType( AST * a ) const {

	return ASTdown()->getType( a );
}

AST * OperatorAST::eval( AST * a ){ return ast->eval(); }

Long	OperatorAST::getInteger(int indx) const
	{
		return ast->getInteger(indx);
	}
	
double	OperatorAST::getDecimal(int indx) const
	{
		return ast->getDecimal(indx);
	}

AST *	OperatorAST::Accept( ASTVisitor & v )	{ return v.VisitOperatorAST( this ); }

astream&
OperatorAST::operator<<( astream& s )
	{
		AST * d;
		s << getName();
		if(d=ASTdown()->ASTright()){
			s << " ";
			s << d;
		}
		return s;
	}

DIVOperator::DIVOperator( ANTLRTokenPtr p ):OperatorAST( p ){ ast = new IntegerNumber;}

AST * DIVOperator::eval( AST * a ){

	AST *	x = ASTdown()->eval();
	AST *	y = ASTdown()->ASTright()->eval();

	if ( (x->getType() == IntegerNumberValue) && (y->getType() == IntegerNumberValue) ){
	
		ast->setInteger( 0, int(x->getInteger() / y->getInteger()) );
	}else{

		Error_Report( "Incompatible arguments" );
		assert( 0 );
		ast = 0;
	}
	return ast;
}

SubtractOperator::SubtractOperator( ANTLRTokenPtr p ):OperatorAST( p ){ ast = 0;}
AST * SubtractOperator::eval( AST * a ){

	AST *	x = ASTdown()->eval();
	AST *	y = ASTdown()->ASTright()->eval();

	if ( (x->getType() == IntegerNumberValue) && (y->getType() == IntegerNumberValue) ){
	
		if ( ast == 0 ) ast = new IntegerNumber;
		ast->setInteger( 0, x->getInteger() - y->getInteger() );
	}else{
		if ( ast == 0 ) ast = new DecimalNumber;
		ast->setDecimal( 0, x->getDecimal() - y->getDecimal() );
	}
	return ast;
}

TimesOperator::TimesOperator( ANTLRTokenPtr p ):OperatorAST( p ){ ast = 0; }
AST * TimesOperator::eval( AST * a ){

	AST *	x = ASTdown()->eval();
	AST *	y = ASTdown()->ASTright()->eval();

	if ( (x->getType() == IntegerNumberValue) && (y->getType() == IntegerNumberValue) ){
	
		if ( ast == 0 ) ast = new IntegerNumber;
		ast->setInteger( 0, x->getInteger() * y->getInteger() );
	}else{
		if ( ast == 0 ) ast = new DecimalNumber;
		ast->setDecimal( 0, x->getDecimal() * y->getDecimal() );
	}
	return ast;
}

//
AddOperator::AddOperator( ANTLRTokenPtr p ):OperatorAST( p ){ ast = 0; }
AST * AddOperator::eval( AST * a ){

	AST *	x = ASTdown()->eval();
	AST *	y = ASTdown()->ASTright()->eval();

	if ( (x->getType() == IntegerNumberValue) && (y->getType() == IntegerNumberValue) ){
	
		if ( ast == 0 ) ast = new IntegerNumber;
		ast->setInteger( 0, x->getInteger() + y->getInteger() );
	}else{
		if ( ast == 0 ) ast = new DecimalNumber;
		ast->setDecimal( 0, x->getDecimal() + y->getDecimal() );
	}
	return ast;
}

ConcatOperator::ConcatOperator( ANTLRTokenPtr p ):OperatorAST( p ){ ast = 0; }
	
AST * ConcatOperator::eval( AST * a ){

	AST *	x = ASTdown()->eval();
	AST *	y = ASTdown()->ASTright()->eval();

	RWCString z = *x->str() + *y->str();
	
	if ( ast != 0 ) delete ast;
	ast = new StringOfCharType( &z );
	return ast;
}


DivideOperator::DivideOperator( ANTLRTokenPtr p ):OperatorAST( p ){ ast = new DecimalNumber; }
AST * DivideOperator::eval( AST * a ){  // result is always decimal

	AST *	x = ASTdown()->eval();
	AST *	y = ASTdown()->ASTright()->eval();

	ast->setDecimal( 0, x->getDecimal() / y->getDecimal() );
	return ast;
}

EQOperator::EQOperator( ANTLRTokenPtr p ):OperatorAST( p ){ ast = new BooleanType; }
AST * EQOperator::eval( AST * a ){

	AST *	x = ASTdown()->eval();
	AST *	y = ASTdown()->ASTright()->eval();

	ast->setInteger( 0, int( x->compare(y) == 0 ) );
	return ast;
}

ExpoOperator::ExpoOperator( ANTLRTokenPtr p ):OperatorAST( p ){ ast = 0; }
AST * ExpoOperator::eval( AST * a ){

	AST *	x = ASTdown()->eval();
	AST *	y = ASTdown()->ASTright()->eval();

	DecimalNumber temp(pow( x->getDecimal(), y->getDecimal() ) );
	if(ast==NULL){
		ast=x->clone();
	}
	ast->assign(&temp);

	return ast;
}

FunctionCallOperator::FunctionCallOperator( ANTLRTokenPtr p ):OperatorAST( p ){;}
AST * FunctionCallOperator::eval( AST * a )
	{
		return NULL;
	}

GEOperator::GEOperator( ANTLRTokenPtr p ):OperatorAST( p ){ ast = new BooleanType; }
AST * GEOperator::eval( AST * a ){

	AST *	x = ASTdown()->eval();
	AST *	y = ASTdown()->ASTright()->eval();

	int z = x->compare(y);
	z = (z==0) || (z==1);
	ast->setInteger( 0, z );
	return ast;
}

GTOperator::GTOperator( ANTLRTokenPtr p ):OperatorAST( p ){ ast = new BooleanType; }
AST * GTOperator::eval( AST * a ){

	AST *	x = ASTdown()->eval();
	AST *	y = ASTdown()->ASTright()->eval();

	int z = x->compare(y);
	z = (z==1);
	ast->setInteger( 0, z );
	return ast;
}

IdentityOperator::IdentityOperator( ANTLRTokenPtr p ):OperatorAST( p ){;}
AST * IdentityOperator::eval( AST * a ){
	ast = ASTdown()->clone();
	return ast;
}

LEOperator::LEOperator( ANTLRTokenPtr p ):OperatorAST( p ){ ast = new BooleanType; }
AST * LEOperator::eval( AST * a ){

	AST *	x = ASTdown()->eval();
	AST *	y = ASTdown()->ASTright()->eval();

	int z = x->compare(y);
	z = (z==0) || (z==-1);
	ast->setInteger( 0, z );
	return ast;
}

LTOperator::LTOperator( ANTLRTokenPtr p ):OperatorAST( p ){ ast = new BooleanType; }
AST * LTOperator::eval( AST * a ){

	AST *	x = ASTdown()->eval();
	AST *	y = ASTdown()->ASTright()->eval();

	int z = x->compare(y);
	z = (z==-1);
	ast->setInteger( 0, z );
	return ast;
}

MODOperator::MODOperator( ANTLRTokenPtr p ):OperatorAST( p ){ ast = new IntegerNumber; }
AST * MODOperator::eval( AST * a ){

	AST *	x = ASTdown()->eval();
	AST *	y = ASTdown()->ASTright()->eval();
	
	ast->setInteger( 0, Long( x->getInteger() % y->getInteger() ) );
	return ast;
}

NEOperator::NEOperator( ANTLRTokenPtr p ):OperatorAST( p ){ ast = new BooleanType;}
AST * NEOperator::eval( AST * a ){

	AST *	x = ASTdown()->eval();
	AST *	y = ASTdown()->ASTright()->eval();

	int z = x->compare(y);
	z = (z!=0);
	ast->setInteger( 0, z );
	return ast;
}

NegativeOperator::NegativeOperator( ANTLRTokenPtr p ):OperatorAST( p ){ ast = 0; }
AST * NegativeOperator::eval( AST * a ){

	AST *	x = ASTdown()->eval();
	
	if ( x->getType() == IntegerNumberValue ){
	
		if ( ast == 0 ) ast = new IntegerNumber;
		ast->setInteger( 0, -1 * x->getInteger() );
	}else{
		if ( ast == 0 ) ast = new DecimalNumber;
		ast->setDecimal( 0, -1.0 * x->getDecimal() );
	}
	return ast;
}


astream&
NegativeOperator::operator<<( astream& s )
	{
		s << ast;
		return s;
	}

NOTOperator::NOTOperator( ANTLRTokenPtr p ):OperatorAST( p ){ ast = 0; }
AST * NOTOperator::eval( AST * a ){

	AST *	x = ASTdown()->eval();

	switch ( x->getType() ){
	
		case  BooleanTypeValue :
		
			if ( ast == 0 ) ast = new BooleanType;
			ast->setInteger( 0, ! x->getInteger() );
			break;
			
		case  BitTypeValue :
		
			if ( ast == 0 ) ast = new BitType;
			ast->setInteger( 0, ! x->getInteger() );
			break;
			
		case  StringOfBitTypeValue :
		
			if ( ast != 0 ) delete ast;
			//auto tmp = bitwise_not(*x->vec());
			RWBitVec tmp = ( ! *x->vec() );
			ast = new StringOfBitType( &tmp );
			break;
	}
	
	return ast;
}


ANDOperator::ANDOperator( ANTLRTokenPtr p ):OperatorAST( p ){ ast = 0; }
AST * ANDOperator::eval( AST * a ){

	AST *	x = ASTdown()->eval();
	AST *	y = ASTdown()->ASTright()->eval();

	if ( (x->getType() == BooleanTypeValue) && (y->getType() == BooleanTypeValue) ){

		if ( ast == 0 ) ast = new BooleanType;
		ast->setInteger( x->getInteger() & y->getInteger() );
		
	}else if ( (x->getType() == BitTypeValue) && (y->getType() == BitTypeValue) ){
	
		if ( ast == 0 ) ast = new BitType;
		ast->setInteger( (*x->vec() & *y->vec())[0] );
		
	}else if ( (x->getType() == StringOfBitTypeValue) && (y->getType() == StringOfBitTypeValue) ){
	
		const RWBitVec *	xx = x->vec();
		const RWBitVec *	yy = x->vec();
		RWBitVec		z;
		
		int	diff = xx->size() - yy->size();
		
		if ( diff < 0 ){
		
			z = *xx;
			z.resize( yy->size() );
			z &= *yy;
			
		}else if ( diff > 0 ){

			z = *yy;
			z.resize( xx->size() );
			z &= *xx;
			
		}else{
			z = *xx & *yy;
		}

		if ( ast != 0 ) delete ast;
		ast = new StringOfBitType( &z );
	}else{
		Error_Report( "Operands not the same type.", this );
	}

	return ast;
}


OROperator::OROperator( ANTLRTokenPtr p ):OperatorAST( p ){ ast = 0;}

AST * OROperator::eval( AST * a ){

	AST *	x = ASTdown()->eval();
	AST *	y = ASTdown()->ASTright()->eval();

	if ( (x->getType() == BooleanTypeValue) && (y->getType() == BooleanTypeValue) ){

		if ( ast == 0 ) ast = new BooleanType;
		ast->setInteger( x->getInteger() | y->getInteger() );
		
	}else if ( (x->getType() == BitTypeValue) && (y->getType() == BitTypeValue) ){
	
		if ( ast == 0 ) ast = new BitType;
		ast->setInteger( (*x->vec() | *y->vec())[0] );
		
	}else if ( (x->getType() == StringOfBitTypeValue) && (y->getType() == StringOfBitTypeValue) ){
	
		const RWBitVec *	xx = x->vec();
		const RWBitVec *	yy = x->vec();
		RWBitVec		z;
		
		int	diff = xx->size() - yy->size();
		
		if ( diff < 0 ){
		
			z = *xx;
			z.resize( yy->size() );
			z |= *yy;
			
		}else if ( diff > 0 ){

			z = *yy;
			z.resize( xx->size() );
			z |= *xx;
			
		}else{
			z = *xx | *yy;
		}

		if ( ast != 0 ) delete ast;
		ast = new StringOfBitType( &z );
	}else{
		Error_Report( "Operands not the same type.", this );
	}

	return ast;
}


XOROperator::XOROperator( ANTLRTokenPtr p ):OperatorAST( p ){ ast = 0; }

AST * XOROperator::eval( AST * a ){

	AST *	x = ASTdown()->eval();
	AST *	y = ASTdown()->ASTright()->eval();

	if ( (x->getType() == BooleanTypeValue) && (y->getType() == BooleanTypeValue) ){

		if ( ast == 0 ) ast = new BooleanType;
		ast->setInteger( x->getInteger() ^ y->getInteger() );
		
	}else if ( (x->getType() == BitTypeValue) && (y->getType() == BitTypeValue) ){
	
		if ( ast == 0 ) ast = new BitType;
		ast->setInteger( (*x->vec() ^ *y->vec())[0] );
		
	}else if ( (x->getType() == StringOfBitTypeValue) && (y->getType() == StringOfBitTypeValue) ){
	
		const RWBitVec *	xx = x->vec();
		const RWBitVec *	yy = x->vec();
		RWBitVec		z;
		
		int	diff = xx->size() - yy->size();
		
		if ( diff < 0 ){
		
			z = *xx;
			z.resize( yy->size() );
			z ^= *yy;
			
		}else if ( diff > 0 ){

			z = *yy;
			z.resize( xx->size() );
			z ^= *xx;
			
		}else{
			z = *xx ^ *yy;
		}

		if ( ast != 0 ) delete ast;
		ast = new StringOfBitType( &z );
	}else{
		Error_Report( "Operands not the same type.", this );
	}

	return ast;
}


