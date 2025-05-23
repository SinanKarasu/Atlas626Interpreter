#ifndef VerbAST_h
#define VerbAST_h

#include	"AtlasAST.h"

class VerbAST : public AST {
public:
        VerbAST(ANTLRTokenPtr p=0);
        virtual ~VerbAST();
	virtual AST *	eval	( AST * a=0 );
	virtual AST *	Accept	( ASTVisitor & );
};


class CalculateVerb : public VerbAST {
public:
        CalculateVerb(ANTLRTokenPtr p=0);
};



class IfThenVerb : public VerbAST {
public:
        IfThenVerb(ANTLRTokenPtr p=0);
	virtual AST * eval(AST * a=0);
};

class ElseIfThenVerb : public VerbAST {
public:
        ElseIfThenVerb(ANTLRTokenPtr p=0);
	virtual AST * eval(AST * a=0);
};

class ElseVerb : public VerbAST {
public:
        ElseVerb(ANTLRTokenPtr p=0);
	virtual AST * eval(AST * a=0);
};

class EndIfVerb : public VerbAST {
public:
        EndIfVerb(ANTLRTokenPtr p=0);
	virtual AST * eval(AST * a=0);
};

class WhileThenVerb : public VerbAST {
public:
        WhileThenVerb(ANTLRTokenPtr p=0);
	virtual AST * eval(AST * a=0);
};


class EndWhileVerb : public VerbAST {
public:
        EndWhileVerb(ANTLRTokenPtr p=0);
	virtual AST * eval(AST * a=0);
};

class ForThenVerb : public VerbAST {
public:
        ForThenVerb(ANTLRTokenPtr p=0);
	virtual AST * eval(AST * a=0);
};

class EndForVerb : public VerbAST {
public:
        EndForVerb(ANTLRTokenPtr p=0);
	virtual AST * eval(AST * a=0);
};

class RequireSignalVerb : public VerbAST {
public:
        RequireSignalVerb(ANTLRTokenPtr p=0);
	virtual AST * eval(AST * a=0);
};

class CompareVerb : public VerbAST {
public:
        CompareVerb(Scope * s);
	virtual AST * eval(AST * a=0);
private:
	Scope * m_scope;
};


class ReadTimerVerb : public VerbAST {
public:
        ReadTimerVerb(ANTLRTokenPtr p=0);
	virtual AST * eval(AST * a=0);
};

class ResetTimerVerb : public VerbAST {
public:
        ResetTimerVerb(ANTLRTokenPtr p=0);
        ResetTimerVerb(ASTList * allTimers);
	virtual AST * eval(AST * a=0);
private:
	ASTList * m_timers;
};


#endif // VerbAST_h
