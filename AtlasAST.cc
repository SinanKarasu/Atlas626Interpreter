
#include	"AtlasAST.h"
#include	"AtlasStd.h"
#include	"NounsModifiersDimensions.h"

#include	<typeinfo>



static int indent = 0;

StatusWord::StatusWord()
	:m_WriteProtect(0)
	,m_ReadWatch(0)
	,m_WriteWatch(0)
	,m_BreakPoint(0)
	,m_EvalutionExpression(0)
	,m_Uninitialized(0)
	{
	}

//const int  sendDebugInfo( const AST * ,StatusWord s);


#include	"AtlasParser.h"
#include	"DebugEnv.h"

extern DebugEnv * debugEnv;

int
StatusWord::getWriteWatch		()
	{	
		return m_WriteWatch;
	}

int
StatusWord::getBreakPoint		()
	{
		return m_BreakPoint;
	}

int
StatusWord::getEvalutionExpression	()
	{
		return m_EvalutionExpression;
	}
int
StatusWord::getUninitialized		()
	{
		return m_EvalutionExpression;
	}

void
StatusWord::setWriteProtect		(int i)
	{
		m_WriteProtect		=	i?1:0	;
	}

void
StatusWord::setReadWatch		(int i)
	{
		m_ReadWatch		=	i?1:0	;
	}

void
StatusWord::setWriteWatch		(int i)
	{
		m_WriteWatch		=	i?1:0	;
	}

void
StatusWord::setBreakPoint		(int i)
	{
		m_BreakPoint		=	i?1:0	;
	}

void
StatusWord::setEvalutionExpression	(int i)
	{
		m_EvalutionExpression	=	i?1:0	;
	}
	
void
StatusWord::setUninitialized		(int i)
	{
		m_Uninitialized		=	i?1:0	;
	}

void
AST::routeIt(void (StatusWord::*c)(int) ) const
	{
		if(debugEnv){
			StatusWord sw;
			(sw.*c)(1);
			assert(debugEnv->sendDebugInfo(this,sw));
		}
	}


void
AST::readEvent() const
	{
	}
void
AST::writeEvent() const
	{
		if(m_WriteProtect){
			routeIt(&StatusWord::setWriteProtect);
		} else if(m_WriteWatch){
			routeIt(&StatusWord::setWriteWatch);
		}
			
	}

bool AST::matches(const std::string& pattern) const {
    // Dummy logic for now — customize later
    return false;
}


int
StatusWord::getWriteProtect()
	{
		return m_WriteProtect;
	}

int
StatusWord::getReadWatch		()
	{
		return m_ReadWatch;
	}
	
	
AST::AST():ASTBase()	{}
	
AST::AST( ANTLRTokenPtr t ):ASTBase(),token(t)	{}

AST::~AST(){

}

AST	*
AST::label	( AST * a  )
	{
		return ASTdown()?ASTdown()->label():0;
	}


DimensionEntry *
AST::getDimension(AST * a) const
	{
		return ASTdown()?ASTdown()->getDimension():0;
	}

void
AST::setDimension(AST * a)
	{
		if(ASTdown())ASTdown()->setDimension(a);
	}


RWCString 
AST::getName() const
	{
		return token->getText();
	}


void  AST::setName(RWCString a){

	token->setText((char *)(a.c_str()));
}

void  AST::print(AST * a) const {

	std::cout << "Default Print>>" << getName() << std::endl;
}

AST * AST::printEvaluation(AST * a) {

	std::cout << "Default Print>>" << getName() << std::endl;
	return this;
}


AST * AST::check( AST * a ){

	AST *	what = ASTdown();

	if ( what ){
		if(a==this){
			return what->check(what);
		} else {
			return what->check( a );
		}
	}

	return 0;	// Nothing to check here!
}


AST * AST::init( AST * a ){

	AST * what = ASTdown();
		
	if ( what ){
		return what->init(a);
	}

	return 0;
} 


AST * AST::assign( AST * a ){

	AST * what = ASTdown();
	
	if ( what ){
		return what->assign(a);
	}

	return 0;
} 


ANTLRTokenPtr	AST::getToken() const{

	return token;
}


// The structure of the program is as such:
//  AST -> AST -> AST -> AST is the sequential lines in the program.
//   |
//   v 
//  AST(*)
//   |
//   v 
//  AST(**) -> AST(***) -> AST(***) -> AST(***)
//   |
//   v 
//  AST(****)
// 

//  AST is specialized "line" action.
//  AST(*) is specialized "Verb" action which is called the ???Verb().
//  AST(**) is specialized "Clause" action.

AST * AST::eval( AST * a ){

	AST	*what  = ASTdown(),
		*where = ASTright();
	
	if ( what ){
	
		what = what->eval( a );		// first we excecute the rules here
	}
	
	if ( where ){
		what = where->eval( a );
	}
	
	return what;
}

AST * AST::max( AST * a ){ return this; }
AST * AST::min( AST * a ){ return this; }


AST * AST::ASTdown() const{

	if ( this == _down ){
		Error_Report( "Down LOOP Detected" );
		assert( 0 );
	}else{
		return (AST *)_down;
	}
}
			
			
AST * AST::ASTright() const{

	if ( this == _right ){
		return 0;
	}else{
		return (AST *)_right;
	}
}
			
			
Scope *
AST::getScope() const
	{

		if ( ASTdown() ){
			Error_Report( "ASTdown() node in AST::getScope()" );
			assert(0);
		}else{
			return ASTright()->getScope(); // transparent node
		}
	}

void
AST::setScope( Scope * scope )
	{
		if ( ASTdown() )
			ASTdown()->setScope( scope );
	}
		

TheType
AST::getType( AST * a ) const {

	if ( ASTdown() ){
		return ASTdown()->getType(a); // transparent node
	}else{
		return UndefinedTypeValue;
	}
}


Long	AST::getInteger( int indx ) const {

	if ( ASTdown() ){
		return ASTdown()->getInteger(indx); // transparent node
	}
}

double	AST::getDecimal( int indx ) const {

	if ( ASTdown() ){
		return ASTdown()->getDecimal( indx ); // transparent node
	}
}


void AST::setInteger( int indx, Long value ){

	if ( ASTdown() ){
		ASTdown()->setInteger( indx, value ); // transparent node
	}
}

void AST::setDecimal( int indx, double value ){

	if ( ASTdown() ){
		ASTdown()->setDecimal( indx, value ); // transparent node
	}
}

AST *	AST::data	( AST *  a )		{ assert( 0 ); return 0; };
AST *	AST::add	( AST *  a )		{ assert( 0 ); return 0; };
AST *	AST::clone	( Scope *s ) const	{ assert( 0 ); return 0; };
AST *	AST::pred	( AST *  a )		{ assert( 0 ); return 0; };
AST *	AST::succ	( AST *  a )		{ assert( 0 ); return 0; };
AST *	AST::insert	( AST *  a )		{ assert( 0 ); return 0; };
AST *	AST::remove	( AST *  a )		{ assert( 0 ); return 0; };
Long	AST::compare	( AST *  o ) const	{ assert( 0 ); return 0; };
Long	AST::count	( AST *  o ) const	{ assert( 0 ); return 0; };
Long	AST::index	( AST *  o ) const	{ assert( 0 ); return 0; };
Long	AST::length	( int indx ) const	{ assert( 0 ); return 0; };

const RWCString * AST::str() const	{ assert( 0 ); return 0; };
const RWBitVec	* AST::vec() const	{ assert( 0 ); return 0; };

void AST::preorder_action()
	{
		std::cerr << typeid( *this ).name() << ", ";
	}
void AST::preorder_before_action()	// Going DOWN.
	{
		std::cerr << std::endl;
		
		for( int spacing=0; spacing < indent; ++spacing ){
		
			std::cerr << "    ";
		}
		
		std::cerr << "   (";
		++indent;
	}	
void AST::preorder_after_action()	// Coming UP.
	{
		std::cerr << std::endl;
		--indent;

		for( int spacing=0; spacing < indent; ++spacing ){
		
			std::cerr << "    ";
		}

		std::cerr << "   )";
	}

astream&	AST::operator>>( astream &s )
	{
		assert(0);
		return s;
	}
	
	
astream&	AST::operator<<( astream &s )
	{
		assert(0);
		return s;
	}

astream*	AST::getStream	() const	{ assert( 0 ); return 0; };



ArrayObject *
AST::array(AST * a)
	{
		return ASTdown()->array(a);
	}


AST *
AST::Accept		( ASTVisitor &	v	)	{ assert(0); return 0; };

Resource *
AST::getResource( )
	{
		if(ASTdown()){
			return ASTdown()->getResource();
		} else {
			assert(0);
			return 0;
		}
	}

void 
AST::setResource( Resource * resource )
	{
		assert(0);
	}

double
AST::ScaleDimension(const double x,const DimensionEntry * oldDim,const DimensionEntry * newDim) const
	{
		// e.g. x = 1 REV
		// convert to RAD
		// 1 * 360.0 / 57.29577951
		//double oldScale=oldDim->scale

		if( newDim && oldDim){
			assert((oldDim->scale!=0.0)&&(newDim->scale!=0.0));
			return x*oldDim->scale/newDim->scale;
		} else if(oldDim){
			assert(oldDim->scale!=0.0);
			return x*oldDim->scale;	// to base dimension (e.g. SEC, V, A)
		} else if(newDim){
			assert(newDim->scale!=0.0);
			return x/newDim->scale;	// from base dimension (e.g. SEC, V, A)
		}
		assert( oldDim || newDim);
	
	}


StringAST::StringAST(RWCString s):_s(s){}
StringAST::~StringAST(){}

RWCString
StringAST::getName	() const {return _s;}


////bool ASTList::findValue(const std::string& key, AST*& result) {
////        for (AST* item : list->items) {
////            if (item->matches(key)) {
////                result = item;
////                return true;
////            }
////        }
////        return false;
////    };
////
