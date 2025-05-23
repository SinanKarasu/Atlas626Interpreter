
#include	"SignalOperatorAST.h"
#include	"Visitors.h"

SignalOperatorAST::SignalOperatorAST( ANTLRTokenPtr p )
	:AST(p),m_ast(0)
	{
	}

SignalOperatorAST::~SignalOperatorAST() { ; };
	
AST *	SignalOperatorAST::eval		( AST * a )		{ return this; };
AST *	SignalOperatorAST::assign	( AST * a )		{ return this; };
AST *	SignalOperatorAST::Accept	( ASTVisitor & v )	{ return v.VisitSignalOperatorAST( this ); }
AST *	SignalOperatorAST::data(AST * a)			{ return ASTdown()->data( a ); }

TheType	SignalOperatorAST::getType( AST * a ) const {

	return ASTdown()->getType( a );
}

Long	SignalOperatorAST::getInteger(int indx) const
	{
		if(m_ast){
			return m_ast->getInteger(indx);
		} else {
			return ASTdown()->getInteger(indx);
		}
	}
	
double	SignalOperatorAST::getDecimal(int indx) const
	{
		if(m_ast){
			return m_ast->getDecimal(indx);
		} else {
			return ASTdown()->getDecimal(indx);
		}
	}
//------------------------------------------------------------------------------------
ConstraintRange::ConstraintRange( ANTLRTokenPtr p ):SignalOperatorAST(p){};
AST *	ConstraintRange::min		( AST * a )
	{
		AST * x=ASTdown()->eval(a);
		return x->min(a);
	}

AST *	ConstraintRange::max		( AST * a )
	{
		AST * x=ASTdown()->ASTright()->eval(a);
		return x->max(a);
	}

AST *	ConstraintRange::check( AST * a )
	{
		if ( max(a)->compare( a->max(this) ) > 0 ){
				return 0;
		}
		if ( min(a)->compare( a->min(this) ) < 0 ){
				return 0;
		}
		return this;
	}

//------------------------------------------------------------------------------------
SelectLimitMax::SelectLimitMax( ANTLRTokenPtr p )
	:SignalOperatorAST(p)
	,m_ninf(new DecimalNumber(NegativeInfinity))
	{
	}

AST *	SelectLimitMax::min		( AST * a )
	{
		if(a==this){	// I don't do arbitrations, pass it down
			return ASTdown()->min(ASTdown());
		} else if(a) {
			if(a->min()->compare(ASTdown()->min())>=0){
				return a->min();	// it is OK....
			} else {
				return a->max();
			}
		} else {
			return ASTdown()->min();
		}
	}

AST *	SelectLimitMax::max		( AST * a )
	{
		if(a==this){	// I don't do arbitrations, pass it down
			return ASTdown()->max(ASTdown());
		} else if(a) {
			if(a->max()->compare(ASTdown()->max())<=0){
				return a->max();	// it is OK....
			} else {
				return a->min();
			}
		} else {
			return m_ninf;
		}
	}

AST *	SelectLimitMax::check( AST * a )
	{
		if(a==this){	// I don't do arbitrations, pass it down
			return ASTdown()->check(ASTdown());
		} else if(a) {
			if ( max(a)->compare( a->max() ) > 0 ){
					return 0;
			}
			if ( m_ninf->compare( a->min() ) == 0){
			}else if ( min(a)->compare( a->min() ) < 0 ){
					return 0;
			}
			return this;
		} else {
			return ASTdown()->check();
		}
	}
//------------------------------------------------------------------------------------
SelectLimitMin::SelectLimitMin( ANTLRTokenPtr p )
	:SignalOperatorAST(p)
	,m_pinf(new DecimalNumber(PositiveInfinity))
	{
	}

AST *	SelectLimitMin::min		( AST * a )
	{
		if(a==this){	// I don't do arbitrations, pass it down
			return ASTdown()->min(ASTdown());
		} else if(a) {
			if(a->min()->compare(ASTdown()->min())<=0){
				return a->min();	// it is OK....
			} else {
				return a->max();
			}
		} else {
			return ASTdown()->min();
		}
	}

AST *	SelectLimitMin::max		( AST * a )
	{
		if(a==this){	// I don't do arbitrations, pass it down
			return ASTdown()->max(ASTdown());
		} else if(a) {
			if(a->max()->compare(ASTdown()->max())>=0){
				return a->max();	// it is OK....
			} else {
				return a->min();
			}
		} else {
			return ASTdown()->max();
		}
	}

AST *	SelectLimitMin::check( AST * a )
	{
		if(a==this){	// I don't do arbitrations, pass it down
			return ASTdown()->check(ASTdown());
		} else if(a) {
			if ( min(a)->compare( a->min() ) > 0 ){
					return 0;
			}
			if ( m_pinf->compare( a->max() ) == 0){
			}else if ( max(a)->compare( a->max() ) < 0 ){
					return 0;
			}
			return this;
		} else {
			return ASTdown()->check();
		}
	}

//------------------------------------------------------------------------------------


ByQuantization::ByQuantization( ANTLRTokenPtr p ):SignalOperatorAST(p)
	{
	}
AST *	ByQuantization::min		( AST * a )
	{
		return ASTdown()->min(a);
	}

AST *	ByQuantization::max		( AST * a )
	{
		return ASTdown()->max(a);
	}

AST *	ByQuantization::check( AST * a )
	{
		if(!ASTdown()->check(a)){
			return 0;
		}
		// FIX_ME
		// We have to figure out what else checks needed here...
		return this;
	}

//------------------------------------------------------------------------------------

ConstraintMax::ConstraintMax( ANTLRTokenPtr p )
	:SignalOperatorAST(p),m_min(new DecimalNumber(NegativeInfinity))
	{
	}

AST *	ConstraintMax::min		( AST * a )
	{
		return m_min;
	}

AST *	ConstraintMax::max		( AST * a )
	{
		AST * x=ASTdown()->eval(a);
		return x->max(a);
	}

AST *	ConstraintMax::check( AST * a )
	{
		if ( max(a)->compare( a->max(this) ) > 0 ){
				return 0;
		}
		return this;
	}

//------------------------------------------------------------------------------------


ConstraintMin::ConstraintMin( ANTLRTokenPtr p )
	:SignalOperatorAST(p),m_max(new DecimalNumber(PositiveInfinity))
	{
	}
AST *	ConstraintMin::min		( AST * a )
	{
		AST * x=ASTdown()->eval(a);
		return x->min(a);
	}

AST *	ConstraintMin::max		( AST * a )
	{
		return m_max;
	}


AST *	ConstraintMin::check( AST * a )
	{
		if ( min(a)->compare( a->min(this) ) < 0 ){
				return 0;
		}
		return this;
	}

//------------------------------------------------------------------------------------

ConstraintPercent::ConstraintPercent( ANTLRTokenPtr p ):SignalOperatorAST(p)
	{
	}

AST *	ConstraintPercent::min		( AST * a )
	{
		return 0;
	}

AST *	ConstraintPercent::max		( AST * a )
	{
		return 0;
	}

AST *	ConstraintPercent::check( AST * a )
	{
		if ( max(a)->compare( a->max(this) ) > 0 ){
				return 0;
		}
		if ( min(a)->compare( a->min(this) ) < 0 ){
				return 0;
		}
		return 0; // FIX_ME
	}


//------------------------------------------------------------------------------------

ConstraintArray::ConstraintArray( ANTLRTokenPtr p ):SignalOperatorAST(p)
	{
	}

AST *	ConstraintArray::min		( AST * a )
	{
		IntegerNumber ind;
		if(a->min()->compare(a->max())!=0){	// make sure we have
			return 0;			// just a number
		}
		for(int i=1;i<=ASTdown()->length();i++){
			ind.setInteger(0,i);
			AST * loc=ASTdown()->eval(&ind);
			if ( loc->min()->compare( a->min() ) == 0 ){	// or max.. 
				return loc;
			}
		}
		return 0;
	}

AST *	ConstraintArray::max		( AST * a )
	{
		IntegerNumber ind;
		if(a->min()->compare(a->max())!=0){	// make sure we have
			return 0;			// just a number
		}
		for(int i=1;i<=ASTdown()->length();i++){
			ind.setInteger(0,i);
			AST * loc=ASTdown()->eval(&ind);
			if ( loc->min()->compare( a->min() ) == 0 ){	// or max.. 
				return loc;
			}
		}
		return 0;
	}


AST *	ConstraintArray::check( AST * a )
	{
		IntegerNumber ind;
		for(int i=1;i<=ASTdown()->length();i++){
			ind.setInteger(0,i);
			AST * loc=ASTdown()->eval(&ind);
			if ( loc->max(a)->compare( a->max(loc) ) > 0 ){
				return 0;
			}
			if ( loc->min(a)->compare( a->min(loc) ) < 0 ){
				return 0;
			}
		}
		return this;
	}

//------------------------------------------------------------------------------------


RealErrlim::RealErrlim( ANTLRTokenPtr p ):SignalOperatorAST(p){};
AST *	RealErrlim::min		( AST * a )
	{
		AST * value=ASTdown();
		AST * error=value->ASTright();
		
		if(error){	// if real_error field exists
			return error->min(value->min());
		} else {	// just ask down
			return value->min();
		}
		
	}

AST *	RealErrlim::max		( AST * a )
	{
		AST * value=ASTdown();
		AST * error=value->ASTright();
		
		if(error){	// if real_error field exists
			return error->min(value->max());
		} else {	// just ask down
			return value->max();
		}
	}


AST *	RealErrlim::check( AST * a )
	{
		if ( max(a)->compare( a->max(this) ) > 0 ){
				return 0;
		}
		if ( min(a)->compare( a->min(this) ) < 0 ){
				return 0;
		}
		return this;
	}

//------------------------------------------------------------------------------------


ANDErrLmt::ANDErrLmt( ANTLRTokenPtr p )
	:SignalOperatorAST(p)
	,m_nom(new DecimalNumber(0.0))
	,m_max(new DecimalNumber(0.0))
	,m_min(new DecimalNumber(0.0))
	{
	}
AST *	ANDErrLmt::eval		( AST * a )
	{
		AST * x=ASTdown()->eval(a);
		double nom=x->getDecimal(0);
		double nom_max=x->max()->getDecimal(0);
		double nom_min=x->min()->getDecimal(0);

		AST * y=ASTdown()->ASTright()->eval(a);
		double ppc=y->max()->getDecimal(0);
		double npc=y->min()->getDecimal(0);

		m_nom->setDecimal(0,nom);
		m_max->setDecimal(0,nom_max+nom*ppc);
		m_min->setDecimal(0,nom_min+nom*npc);
		return this;
	}
AST *	ANDErrLmt::min		( AST * a )
	{
		return m_min;
	}
AST *	ANDErrLmt::max		( AST * a )
	{
		return m_max;
	}

//------------------------------------------------------------------------------------

ORErrLmt::ORErrLmt( ANTLRTokenPtr p )
	:SignalOperatorAST(p)
	,m_nom(new DecimalNumber(0.0))
	,m_max(new DecimalNumber(0.0))
	,m_min(new DecimalNumber(0.0))
	{
	}
AST *	ORErrLmt::eval		( AST * a )
	{
		AST * x=ASTdown()->eval(a);
		double dnom=x->getDecimal(0);
		double dmax=x->max()->getDecimal(0);
		double dmin=x->min()->getDecimal(0);

		AST * y=ASTdown()->ASTright()->eval(a);
		double pmax=(1.0+y->max()->getDecimal())*dnom;
		double pmin=(1.0+y->min()->getDecimal())*dnom;
		m_nom->setDecimal(0,dnom);
		m_max->setDecimal(0,(dmax<pmax)?dmax:pmax);
		m_min->setDecimal(0,(dmin>pmin)?dmin:pmin);
		return this;
	}
AST *	ORErrLmt::min		( AST * a )
	{
		return 0;
	}

AST *	ORErrLmt::max		( AST * a )
	{
		return 0;
	}

//------------------------------------------------------------------------------------

ULOperator::ULOperator( ANTLRTokenPtr p )
	:SignalOperatorAST(p)
	{
		m_ast = new BooleanType;
	}

AST *
ULOperator::eval( AST * a ){

	AST *	x = ASTdown()->eval();
	AST *	y = ASTdown()->ASTright()->eval();

	int z = x->compare(y);
	z = (z==0) || (z==-1);
	m_ast->setInteger( 0, z );
	return m_ast;
}

astream&
ULOperator::operator<<( astream &s )
	{
		AST * ds=ASTdown();
		AST * ul=ds->ASTright();
		AST * nm=ul->ASTright();
		if(nm){
			s << nm ;
			s << " ";
		}
		s << "UL " ;
		s << ul;
		return s;
	}

//------------------------------------------------------------------------------------

LLOperator::LLOperator( ANTLRTokenPtr p )
	:SignalOperatorAST(p)
	{
		m_ast = new BooleanType;
	}

AST *
LLOperator::eval( AST * a ){

	AST *	x = ASTdown()->eval();
	AST *	y = ASTdown()->ASTright()->eval();

	int z = x->compare(y);
	z = (z==0) || (z==1);
	m_ast->setInteger( 0, z );
	return m_ast;
}

astream&
LLOperator::operator<<( astream &s )
	{
		AST * ds=ASTdown();
		AST * ll=ds->ASTright();
		AST * nm=ll->ASTright();
		if(nm){
			s << nm ;
			s << " ";
		}
		s << "LL " ;
		s << ll;
		s << " ";
		return s;
	}
//------------------------------------------------------------------------------------

NOMOperator::NOMOperator( ANTLRTokenPtr p )
	:SignalOperatorAST(p)
	{
		m_ast = NULL;
	}

AST *
NOMOperator::eval( AST * a ){

	m_ast = ASTdown()->eval();
	return m_ast;
}

astream&
NOMOperator::operator<<( astream &s )
	{
		s << "NOM " ;
		s << ASTdown() ;
		return s;
	}


//------------------------------------------------------------------------------------

WITHINOperator::WITHINOperator( ANTLRTokenPtr p )
	:SignalOperatorAST(p)
	{
		m_ast =  new BooleanType;
	}

AST *
WITHINOperator::eval( AST * a ){

	AST *	x = ASTdown()->eval();
	AST *	y = ASTdown()->ASTright()->eval();


	m_ast->setInteger( x->getInteger() | y->getInteger() );
		
	return m_ast;
}

astream&
WITHINOperator::operator<<( astream &s )
	{
		AST * ul=ASTdown();
		AST * ll=ul->ASTright();
		s<< ll ;
		s<< " ";
		s<< ul;
		return s;
	}

