#ifndef ActionAST_h
#define ActionAST_h



#include	"InitList.h"
#include	"BasicTypeAST.h"

class ActionAST : public AST {
	public:
		ActionAST(ANTLRTokenPtr p);
		virtual ~ActionAST();

		virtual AST *	Accept	( ASTVisitor & );
	
		AST * ast;	
};


class LineAction : public ActionAST {
	public:
		LineAction( ANTLRTokenPtr p, Scope * scope,Fstatno * fstatno);
		LineAction( ANTLRTokenPtr p, Scope * scope,LineAction * prevfstatno);
		virtual AST * eval(AST * a=0);
		virtual	Scope	*	getScope	() const ;
        	virtual void		setScope	( Scope * scope );

	virtual astream& operator<<( astream& s );

	private:
		Scope		*	m_scope;
		Fstatno		*	m_fstatno;
		LineAction	*	m_prevfstatno;
		int m_delta;
		void trace(int flag=0);
		int getLine() const ;
};

class BeginBlockAction : public ActionAST {
	public:
		BeginBlockAction( ANTLRTokenPtr p);
		virtual AST * eval(AST * a=0);
};


class EndBlockAction : public ActionAST {
	public:
		EndBlockAction( ANTLRTokenPtr p);
		virtual AST * eval(AST * a=0);
};


class IndexAction : public ActionAST {
	public:
		IndexAction( ANTLRTokenPtr p=NULL );

		virtual AST *	eval	( AST * a=0 );
		virtual TheType	getType	( AST * a=0 ) const;
		virtual astream&	operator<<( astream& s );
	protected:
		AST * m_index;
};

class ExecuteProgramAction : public ActionAST {
	public:
		ExecuteProgramAction( ANTLRTokenPtr p,Scope * scope,AtlasParser * parser );
		virtual AST *	eval		( AST * a=0 );
	private:
		Scope		*	m_scope;
		AtlasParser	*	m_parser;
};

class ExecuteFunctionAction : public ActionAST {
	public:
		ExecuteFunctionAction( ANTLRTokenPtr p=NULL );

		virtual AST *	eval		( AST * a=0 );
		virtual void	setInteger	( int indx=0,Long value=0 );
	private:
		int argcount;
};

class ExecuteProcedureAction : public ActionAST {
	public:
		ExecuteProcedureAction( ANTLRTokenPtr p=NULL );

		virtual AST *	eval		( AST * a=0 );
		virtual void	setInteger	( int indx=0,Long value=0 );
	private:
		int argcount;
};

class ExecuteAction : public ActionAST {
	public:
		ExecuteAction( ANTLRTokenPtr p=NULL );
		virtual AST *	eval		( AST * a=0 );
};

class DataInputAction : public ActionAST {
	public:
		DataInputAction( ANTLRTokenPtr p=NULL );
		virtual AST * eval(AST * a=0);
};

class TextInputAction : public ActionAST {
	public:
		TextInputAction( ANTLRTokenPtr p=NULL );
		virtual AST * eval(AST * a=0);
};

class FileDataInputAction : public ActionAST {
	public:
		FileDataInputAction( ANTLRTokenPtr p=NULL );
		virtual AST * eval( AST * a=0 );
};

class FileTextInputAction : public ActionAST {
	public:
		FileTextInputAction( ANTLRTokenPtr p=NULL );
		virtual AST * eval( AST * a=0 );
};


class DataOutputAction : public ActionAST {
	public:
		DataOutputAction( ANTLRTokenPtr p=NULL );
		virtual AST * eval(AST * a=0);
};

class TextOutputAction : public ActionAST {
	public:
		TextOutputAction( ANTLRTokenPtr p=NULL );
		virtual AST * eval(AST * a=0);
};

class FilePositionAction : public ActionAST {
	public:
		FilePositionAction( ANTLRTokenPtr p=NULL );
		virtual AST * eval( AST * a=0 );
};

class FileDataOutputAction : public ActionAST {
	public:
		FileDataOutputAction( ANTLRTokenPtr p=NULL );
		virtual AST * eval( AST * a=0 );
};

class FileTextOutputAction : public ActionAST {
	public:
		FileTextOutputAction( ANTLRTokenPtr p=NULL );
		virtual AST * eval( AST * a=0 );
};


class HexFormatAction : public ActionAST {
	public:
		HexFormatAction( ANTLRTokenPtr p=NULL );
		
		virtual AST * eval( AST * a=0 );
		
		virtual astream& operator>>( astream& s );
		virtual astream& operator<<( astream& s );
};

class ExpFormatAction : public ActionAST {
	public:
		ExpFormatAction( ANTLRTokenPtr p=NULL );
		
		virtual AST * eval( AST * a=0 );
		
		virtual astream& operator>>( astream& s );
		virtual astream& operator<<( astream& s );
};

class BinFormatAction : public ActionAST {
	public:
		BinFormatAction( ANTLRTokenPtr p=NULL );
		
		virtual AST * eval( AST * a=0 );
		
		virtual astream& operator>>( astream& s );
		virtual astream& operator<<( astream& s );
};

class OctFormatAction : public ActionAST {
	public:
		OctFormatAction( ANTLRTokenPtr p=NULL );
		
		virtual AST * eval( AST * a=0 );
		
		virtual astream& operator>>( astream& s );
		virtual astream& operator<<( astream& s );
};

class NoFormatAction : public ActionAST {
	public:
		NoFormatAction( ANTLRTokenPtr p=NULL );
		
		virtual AST * eval( AST * a=0 );
		
		virtual astream& operator>>( astream& s );
		virtual astream& operator<<( astream& s );
};

class IntegerToDecimalAction : public ActionAST {
	public:
		IntegerToDecimalAction( ANTLRTokenPtr p=NULL );
		virtual AST * eval( AST * a=0 );
};

class AssignAction : public ActionAST {
	public:
		AssignAction( ANTLRTokenPtr p=NULL );
		virtual AST * eval( AST * a=0 );
};

class EnableFileAction : public ActionAST {
	public:
		EnableFileAction( ANTLRTokenPtr p=NULL, int mode=0, int create=0 );
		virtual AST * eval( AST * a=0 );
	private:
		int	_mode;
		int	_create;
};

class DisableFileAction : public ActionAST {
	public:
		DisableFileAction( ANTLRTokenPtr p=NULL );
		virtual AST * eval( AST * a=0 );
};

class CreateFileAction : public ActionAST {
	public:
		CreateFileAction( ANTLRTokenPtr p=NULL );
		virtual AST * eval( AST * a=0 );
};

class DeleteFileAction : public ActionAST {
	public:
		DeleteFileAction( ANTLRTokenPtr p=NULL );
		virtual AST * eval( AST * a=0 );
};

class GoToAction : public ActionAST {
	public:
		GoToAction( ANTLRTokenPtr p=NULL, Scope * s=NULL );
		virtual AST * eval(AST * a=0);
	protected:
		Scope * _scope;
};

class LeaveAction : public ActionAST {
	public:
		LeaveAction( ANTLRTokenPtr p=NULL, Scope * s=NULL );
		virtual AST * eval(AST * a=0);
	protected:
		Scope * _scope;
};

class ResumeForAction : public ActionAST {
	public:
		ResumeForAction( ANTLRTokenPtr p=NULL, Scope * s=NULL );
		virtual AST * eval(AST * a=0);
	protected:
		Scope * _scope;
};


class TargetAction : public ActionAST {
	public:
		TargetAction( ANTLRTokenPtr p=NULL );
		TargetAction( AST * a );
		
		virtual AST * eval( AST * a=0 );
	protected:
		AST * _target;
};


class ForThenEntryAction : public ActionAST {
	public:
		ForThenEntryAction( ANTLRTokenPtr p=NULL );
		// ForThenEntryAction( AST * a );
		
		virtual AST * eval(AST * a=0);
	protected:
		AST * _target;
};

class ArrayIteratorAction : public ActionAST {
	public:
		ArrayIteratorAction( ANTLRTokenPtr p=NULL );
		
		virtual AST * eval(AST * a=0);
	protected:
		AST * _current;
};

class ScalarIteratorAction : public ActionAST {
	public:
		ScalarIteratorAction( ANTLRTokenPtr p=NULL );
		// ScalarIteratorAction( AST * a );
		
		virtual AST * eval(AST * a=0);
	protected:
		AST * _from;
		AST * _current;
		AST * _by;
		int _first_time;
};

class ForCompareAction : public ActionAST {
	public:
		ForCompareAction( ANTLRTokenPtr p=NULL );
		// ForCompareAction( AST * a );
		
		virtual AST * eval(AST * a=0);
	protected:
		AST * _loopvar;
		AST * _by;
		AST * _to;
		int _dir;
		AST * _result;
};


class VarInitAction : public ActionAST {
	public:
		VarInitAction(ASTList * Label_List,InitList *init_List);
		virtual AST * eval(AST * a=0);
	protected:
		ASTList  * m_label_list;
		InitList * m_init_list;
		InitListIterator *m_init_list_iterator;
		RWTStack<InitData *,RWTValOrderedVector<InitData *> > m_stack;
		InitData * top();
		InitData * push(InitData * x) ;
		InitData * pop() ;
		void reset() ;
};


class ExternalFunctionAction : public ActionAST {
	public:
		ExternalFunctionAction( ANTLRTokenPtr p=NULL );
		ExternalFunctionAction( AST * a );
	
		virtual AST * eval( AST * a=0 );
	protected:
		AST * _target;
};

class HiAction : public ActionAST {
	public:
		HiAction (class AtlasParser * p);
		virtual AST * eval( AST * a=0 );
		virtual astream& operator<<( astream& s );
	private:
		AtlasParser * m_parser;		
		

};

class LoAction : public ActionAST {
	public:
		LoAction (class AtlasParser * p);
		virtual AST * eval( AST * a=0 );
		virtual astream& operator<<( astream& s );
	private:
		AtlasParser * m_parser;		
		

};

class GoAction : public ActionAST {
	public:
		GoAction (class AtlasParser * p);
		virtual AST * eval( AST * a=0 );
		virtual astream& operator<<( astream& s );
	private:
		AtlasParser * m_parser;		
		

};

class NoGoAction : public ActionAST {
	public:
		NoGoAction (class AtlasParser * p);
		virtual AST * eval( AST * a=0 );
		virtual astream& operator<<( astream& s );
	private:
		AtlasParser * m_parser;		
		

};

class CompareAction : public ActionAST {
	public:
		CompareAction (class AtlasParser * p);
		virtual AST * eval( AST * a=0 );
		virtual	Scope	*	getScope	() const ;
		virtual astream&	operator<<( astream& s );
	private:
		AtlasParser * m_parser;		
		

};

#endif // ActionAST_h
