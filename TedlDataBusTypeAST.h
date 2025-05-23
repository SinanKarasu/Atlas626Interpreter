#ifndef TedlDataBusTypeAST_h
#define TedlDataBusTypeAST_h

#include	"AtlasAST.h"
#include	"InitList.h"
#include	"NounsModifiersDimensions.h"
#include	"TimerObject.h"


class	TedlDataBusTypeAST : public AST{
	public:
		TedlDataBusTypeAST(ANTLRTokenPtr	p=0,	TheType t=UndefinedTypeValue);
		TedlDataBusTypeAST(AST	 *	a,	TheType t=UndefinedTypeValue);
		TedlDataBusTypeAST(ANTLRTokenPtr  p,AST * a,TheType t=UndefinedTypeValue);
		virtual	~TedlDataBusTypeAST();
	
		virtual AST *	eval	( AST * a=0  );
		virtual AST *	assign	( AST * a    );
		virtual	AST *	data	( AST *  a = 0 );
		virtual	Long	compare	( AST *  o     ) const;
		virtual AST *	check	( AST * a=0 );
		virtual TheType	getType	( AST * a=0  ) const;
	protected:
		AST * m_AST;

	private:
		TheType	_storage;
};	

class	TedlTalker : public TedlDataBusTypeAST{
	public:
		TedlTalker ( ASTList *  equipList=0);
		virtual	AST *	data	( AST *  a = 0 );

	private:
		ASTList * m_equipList;
};

class	TedlListener : public TedlDataBusTypeAST{
	public:
		TedlListener ( ASTList *  equipList=0);
		virtual	AST *	data	( AST *  a = 0 );

	private:
		ASTList * m_equipList;
};


class	TedlDataBusDevice : public TedlDataBusTypeAST{
	public:
		TedlDataBusDevice ( ANTLRTokenPtr p=0);
		TedlDataBusDevice ( AST * a);

	private:
};

class	TedlDataBusDevices : public TedlDataBusTypeAST{
	public:
		TedlDataBusDevices ( ANTLRTokenPtr p=0);
		TedlDataBusDevices ( AST * a);
		virtual AST *	check	( AST * a=0 );

	private:
};


#endif // TedlDataBusTypeAST_h
