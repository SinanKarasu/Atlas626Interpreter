#ifndef TedlCompAST_h
#define TedlCompAST_h

#include	"AtlasAST.h"

class TedlCompAST : public AST {
    public:
	TedlCompAST (AST * a = 0);
	virtual	~TedlCompAST ();
	virtual AST *	eval	( AST * a=0  );
};	


class IfTedl : public TedlCompAST {
    public:
	IfTedl (AST * a = 0);
	virtual	~IfTedl ();
	virtual AST *	eval	( AST * a=0  );
};	

class ElseIfTedl : public TedlCompAST {
    public:
	ElseIfTedl (AST * a = 0);
	virtual	~ElseIfTedl ();
	virtual AST *	eval	( AST * a=0  );
};	

class ElseTedl : public TedlCompAST {
    public:
	ElseTedl (AST * a = 0);
	virtual	~ElseTedl ();
	virtual AST *	eval	( AST * a=0  );
};	

class EndIfTedl : public TedlCompAST {
    public:
	EndIfTedl (AST * a = 0);
	virtual	~EndIfTedl ();
	virtual AST *	eval	( AST * a=0  );
};	

class SkipTedl : public TedlCompAST {
    public:
	SkipTedl (AST * a = 0);
	virtual	~SkipTedl ();
	virtual AST *	eval	( AST * a=0  );
};	

class TargetTedl : public TedlCompAST {
    public:
	TargetTedl (AST * a = 0);
	virtual	~TargetTedl ();
	virtual AST *	eval	( AST * a=0  );
    private:
    AST * m_target;
};	

class ExecuteTedl : public TedlCompAST {
    public:
	ExecuteTedl (AST * a = 0);
	virtual	~ExecuteTedl ();
	virtual AST *	eval	( AST * a=0  );
};	

#endif // TedlCompAST_h
