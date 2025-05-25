#ifndef AtlasAST_h
#define AtlasAST_h

#include	"Types.h"
#include	"ArrayObject.h"
#include	"Scope.h"
#include	"astream.h"
//#include	"limits.h"

#include <cfloat>

//#define NegativeInfinity  -DBL_MAX
//#define PositiveInfinity   DBL_MAX

const double NegativeInfinity = -DBL_MAX;
const double PositiveInfinity =  DBL_MAX;


class StatusWord{

public:
	StatusWord();
	int getWriteProtect		();
	int getReadWatch		();
	int getWriteWatch		();
	int getBreakPoint		();
	int getEvalutionExpression	();
	int getUninitialized	();

	void	setWriteProtect		(int i);
	void	setReadWatch		(int i);
	void	setWriteWatch		(int i);
	void	setBreakPoint		(int i);
	void	setEvalutionExpression	(int i);
	void	setUninitialized	(int i);
	
protected:
	unsigned	m_WriteProtect:1;
	unsigned	m_ReadWatch:1;
	unsigned	m_WriteWatch:1;
	unsigned	m_BreakPoint:1;
	unsigned	m_EvalutionExpression:1;
	unsigned	m_Uninitialized:1;
	

};
	


class Scope; // forward declaration
class ASTVisitor;
class Resource;
class DimensionEntry;
class AST : public ASTBase,public StatusWord {
public:
		AST();
		AST(ANTLRTokenPtr t);		//	Required by ANTLR
	virtual ~AST();

		AST	&	operator=( const AST& );	//	Assignment

	virtual	AST	*	ASTdown	() const ;
	virtual	AST	*	ASTright() const ;
	
	virtual	AST	*	assign	( AST * a = 0 );
	virtual AST	*	check	( AST * a = 0 );
	virtual	AST	*	eval	( AST * a = 0 );
	virtual	AST	*	init	( AST * a = 0 );
	virtual	AST	*	data	( AST * a = 0 );
	virtual AST	*	add	( AST * a = 0 );
	virtual AST	*	pred	( AST * a = 0 );
	virtual AST	*	succ	( AST * a = 0 );

	virtual AST	*	insert	( AST * a = 0 );
	virtual AST	*	remove	( AST * a = 0 );

	virtual AST	*	min	( AST * a = 0 );
	virtual AST	*	max	( AST * a = 0 );
	virtual	AST	*	clone	( Scope *s = 0 ) const;
	virtual	AST	*	label	( AST * a = 0 );
	
	virtual DimensionEntry * getDimension(AST * a=0) const;

	virtual void setDimension(AST * a=0);
	
	virtual RWCString  	getName	() const ;
	virtual void 		setName	( RWCString a );
	virtual ANTLRTokenPtr	getToken() const;

        virtual	Scope	*	getScope	() const ;
        virtual void		setScope	( Scope * scope );
	virtual TheType		getType		( AST *  a = 0 ) const ;
	virtual	Long		getInteger	( int indx = 0 ) const ;
	virtual	double		getDecimal	( int indx = 0 ) const ;
	virtual	void		setInteger	( int indx = 0,  Long   value = 0 );
	virtual	void		setDecimal	( int indx = 0,  double value = 0.0 );
	virtual void		print		( AST * a = 0  ) const;
	virtual AST	*	printEvaluation	( AST * a = 0  ) ;

	virtual	Long		compare	( AST *  o     ) const;
	virtual Long		count	( AST *  o     ) const;
	virtual Long		index	( AST *  o     ) const;
	virtual Long		length	( int indx = 0 ) const;



	virtual const RWCString *str() const;
	virtual const RWBitVec	*vec() const;

	virtual	void		preorder_action();
	virtual	void		preorder_before_action();
	virtual	void		preorder_after_action();

	virtual astream&	operator>>	( astream& s );
	virtual astream&	operator<<	( astream& s );
	virtual astream*	getStream	() const;

	virtual AST *	Accept	( ASTVisitor & );

	virtual Resource * getResource( );
	virtual void  setResource( Resource * resource );
	
	virtual	ArrayObject * 	array(AST * a=0);
	// REMOVE THIS FROM HEADER
	// virtual bool matches(const std::string& pattern) const {
	//     return false;
	// }
	
	virtual bool matches(const std::string& pattern) const;

	
protected:
	ANTLRTokenPtr token;
	void readEvent() const;
	void writeEvent() const;
	void routeIt(void (StatusWord::*c)(int)) const ;
	double ScaleDimension(const double x,const DimensionEntry* o,const DimensionEntry * n = 0) const;
};

class StringAST : public AST{
public:
	StringAST(RWCString s);
	virtual ~StringAST();
	
	virtual RWCString  	getName	() const ;

private:	
	RWCString _s;

};

#endif // AtlasAST_h
