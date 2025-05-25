#ifndef TedlDeviceAST_h
#define TedlDeviceAST_h

#include	"AtlasAST.h"
#include	"Resource.h"

//extern Resource * currentResource;

class TedlDeviceAST : public AST {
public:
	TedlDeviceAST(ANTLRTokenPtr p);
	virtual ~TedlDeviceAST();
};


class TedlDeviceDriver : public TedlDeviceAST {
public:
	TedlDeviceDriver(ANTLRTokenPtr p=NULL);
	virtual AST	*	eval(AST * a = 0 );
	virtual AST	*	init	( AST * a = 0 );

private:
	typedef union
		{
			int	Int;
			long	Long;
			double	Double;
			float	Float;
			char *	CharP;
			char	Char;
		}	CallArg;
		
	CallArg callArgs[16];
	void setArg(int argno,AST * source);
	void getArg(int argno,AST * source);
	void call_func();
	AST * m_data;
};


class TedlArgumentConstant : public TedlDeviceAST {
public:
	TedlArgumentConstant (ANTLRTokenPtr p=0);
	virtual RWCString  	getName	() const ;
	virtual AST *	eval	( AST * a = 0 );
	virtual	AST *	data	( AST * a = 0 );
	virtual AST *	assign	( AST * a    );
	virtual AST *	clone	( Scope * s=0 ) const;

private:
	//ModifierEntry	* m_modifierEntry;
	//NounEntry	* m_nounEntry;
};

class TedlArgumentLabel : public TedlDeviceAST {
public:
	TedlArgumentLabel (ANTLRTokenPtr p,int get,int put);
	virtual AST *	eval	( AST * a = 0 );
	virtual	AST *	data	( AST * a = 0 );
	virtual AST *	clone	( Scope * s=0 ) const;
	virtual AST *	assign	( AST * a    );
	virtual TheType	getType	( AST * a=0  ) const;
private:
	int m_get,m_put;
	AST * m_data;

};

class TedlExternalLabel : public TedlDeviceAST {
public:
	TedlExternalLabel (NounEntry * n,ANTLRTokenPtr p,ModifierEntry * m,int get,int put);
	virtual RWCString  	getName	() const ;
	virtual AST *	eval	( AST * a = 0 );
	virtual	AST *	data	( AST * a = 0 );
	virtual AST *	assign	( AST * a    );
	virtual AST *	clone	( Scope * s=0 ) const;
	virtual TheType	getType	( AST * a=0  ) const;

private:
	ModifierEntry	* m_modifierEntry;
	NounEntry	* m_nounEntry;
	int m_get,m_put;
	AST * m_data;
};

class  TedlProxy : public TedlDeviceAST {  // just a proxy for TedlArgumentLabel
public:
	TedlProxy(AST * label,int get,int put);
	virtual RWCString  	getName	() const ;
	virtual AST *		eval	( AST * a = 0 );
	virtual	AST *		data	( AST * a = 0 );
	virtual AST *	assign	( AST * a    );
private:
	int m_get,m_put;
	AST * m_data;
};


class ResourceContextAST : public AST
{
public:
	ResourceContextAST(Resource * r,const RWCString parameter);
	virtual AST *	eval(AST * a=0);
	virtual	AST *	data	( AST * a = 0 );
	virtual AST *	check	( AST * a=0 );
	virtual AST *	assign	( AST * a    );
	virtual void  setResource( Resource * resource );
	virtual Resource * getResource( );
	AST * currentParameters;
	Resource * m_resource;
	StringAST * m_parameter;
};

#endif // TedlDeviceAST_h
