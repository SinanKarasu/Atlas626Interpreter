#ifndef DebugEnv_h
#define DebugEnv_h

#include	"Std.h"
#include	"Types.h"

#include	<pthread.h>
#include	<semaphore.h>

//#include	"AtlasBox.h"

extern int debug_env;


class	LineInfo{
public:
	LineInfo(int lineNo, LineAction * lineAction);
	
	Scope * getScope();
	int             m_LineNo;
	LineAction     *m_LineAction;

};

typedef void (*CallBack)(class DebugInfo *);

class	DebugInfo{
public:
	DebugInfo(AST * x);
	AST		*m_ast;	// AST that owns this
	CallBack	m_ReadEvent;
	CallBack	m_WriteEvent;
	CallBack	m_BreakPoint;
	CallBack	m_Router;
	RWCString       m_info;
	StatusWord      m_StatusWord;
	EdgeList * m_EdgeList;
};

typedef AST * ASTPtr;

	
using DebugInfoDictionary = AppendableMap<ASTPtr ,DebugInfo * >;
////class	DebugInfoDictionary:public RWTValHashDictionary < ASTPtr ,DebugInfo * >{
////public:
////	DebugInfoDictionary();
////
////private:
////	enum {
////	NbrBuckets = RWDEFAULT_CAPACITY};
////	               
////};
////
////class	DebugInfoDictionaryIterator:public RWTValHashDictionaryIterator < ASTPtr ,DebugInfo * >{
////public:
////	DebugInfoDictionaryIterator(DebugInfoDictionary & d);
////};
////
class LineInfoList:public AppendableList<LineInfo*> {
public:
	bool findLineInfo(int lineNo, LineInfo*& out) {
	    out = nullptr;
	    for (LineInfo* info : *this) {
	        if (info->m_LineNo > lineNo) {
	            return true;
	        }
	        out = info;
	    }
	    return false;
	}
};

using LineInfoListIterator = LineInfoList::iterator;
////class	LineInfoList:public RWTValSlist < LineInfo * >{
////public:
////	//LineInfoList();
////	RWBoolean findValue(int lineNo, LineInfo *& );
////};

////class	LineInfoListIterator:public RWTValSlistIterator < LineInfo * >{
////public:
////	LineInfoListIterator(LineInfoList & d);
////};

extern LineInfoList lineInfoList;
//const int sendDebugInfo(AST * ,StatusWord s);

class DebugEnv{
public:
	DebugEnv(class AtlasBox * b);
	AST * setBreakPoint(int lineNo,int mode,CallBack f=0);
	AST * setReadWatch(int LineNo,RWCString var,CallBack f=0);
	AST * setWriteWatch(int LineNo,RWCString var,CallBack f=0);
	AST * singleStep(int mode,CallBack f=0);
	AST * continueRun(CallBack f=0);
	AST * evaluateExpression(int LineNo,RWCString ex);
	AST * assignExpression(int LineNo,RWCString ex);
	const int sendDebugInfo(const AST * ,StatusWord s);
	AST * blockProgress(AST * x,int force=0);
	AST * sendPath(EdgeList& edgeList);
	void  sendPath(CallBack f);
private:
	class AtlasBox * m_box;		
	DebugInfoDictionary debugInfoDictionary;
	void getEnv();
	void setEnv(int LineNo);
	void restoreEnv();
	Scope * m_saveScope;
	sem_t m_sem;
	pthread_mutex_t m_mutex;
	int m_single_step;
	AST * m_where;
	CallBack m_single_step_cb;
	CallBack m_path_cb;
};

#endif //DebugEnv_h
