#include        "AtlasAST.h"
#include        "AtlasStd.h"
#include        "AtlasParser.h"
#include        "Dictionary.h"
#include        "Signal.h"

#include	"DebugEnv.h"
#include	"AtlasDLG.h"
#include	"AtlasBox.h"

LineInfoList	lineInfoList;

//DebugInfoDictionary debugInfoDictionary;

int debug_env=1;

unsigned
ASTPtrHash( const ASTPtr &  p ) { return (unsigned long)(p); }

//DebugInfoDictionary::DebugInfoDictionary()
//	:RWTValHashDictionary<ASTPtr, DebugInfo * > (ASTPtrHash)
//	{
//	}

////DebugInfoDictionaryIterator::DebugInfoDictionaryIterator(DebugInfoDictionary & d)
////	:RWTValHashDictionaryIterator < ASTPtr , DebugInfo * >(d)
////	{
////	}

const int
DebugEnv::sendDebugInfo( const AST  *   a,StatusWord s)
	{
		DebugInfo * debugInfo=0;
		if(debugInfoDictionary.findValue((AST *)a,debugInfo)){
			debugInfo->m_StatusWord=s;
			if(debugInfo->m_Router){
				(*(debugInfo->m_Router))(debugInfo);
			}
			return 1;
		} else {
			return 0;
		}	
	
	}


LineInfo::LineInfo(int lineNo, LineAction * lineAction)
	:m_LineNo(lineNo)
	,m_LineAction(lineAction)
	{
	}

Scope *
LineInfo::getScope()
	{
		if(m_LineAction){
			return m_LineAction->getScope();
		} else {
			return 0;
		}
	}

////unsigned
////ASTPtrHash( const AST * & p ) { return unsigned(p); }
////
////DebugInfoDictionary :: DebugInfoDictionary()
////	:RWTValHashDictionary<AST *,DebugInfo *> (ASTPtrHash)
////        {resize(NbrBuckets);}

////RWBoolean
////LineInfoList :: findValue( int lineNo, LineInfo *& value)
////	{
////		value=0;
////		LineInfoListIterator lilit(*this);
////		while(++lilit){
////			if(lilit.key()->m_LineNo > lineNo){
////				return TRUE;
////			}
////			value=lilit.key();
////		}
////		return FALSE;
////
////	}
////
////LineInfoListIterator::LineInfoListIterator(LineInfoList & d)
////	:RWTValSlistIterator < LineInfo * > (d)
////	{
////	}
////

DebugEnv::DebugEnv(AtlasBox * b)
	:m_box(b)
	,m_single_step(0)
	,m_single_step_cb(0)
	,m_path_cb(0)
	,m_where(0)
	{
	}

AST *
DebugEnv::blockProgress(AST * x,int force)
	{
		if(force){
			pthread_mutex_lock(&m_mutex);
			sem_init(&m_sem,0,0);
			pthread_mutex_unlock(&m_mutex);
		}
		m_where=x;
		if(force || m_single_step){
			pthread_mutex_lock(&m_mutex);
			pthread_mutex_unlock(&m_mutex);
			DebugInfo * debugInfo=0;
			if(debugInfoDictionary.findValue(x,debugInfo)){
				if(debugInfo->m_BreakPoint){
					(*(debugInfo->m_BreakPoint))(debugInfo);
				}
			} else if(m_single_step_cb){
				DebugInfo debugInfo(x);
				(*m_single_step_cb)(&debugInfo);
			}
			sem_wait(&m_sem);
			return x;
		} else {
			return 0;
		}
	}

AST *
DebugEnv::setBreakPoint(int lineNo,int mode,CallBack f)
	{
		LineInfo * lineInfo=0;
		lineInfoList.findLineInfo( lineNo, lineInfo);
		if(lineInfo){
			pthread_mutex_lock(&m_mutex);
			lineInfo->m_LineAction->setBreakPoint(mode);
			DebugInfo * debugInfo=0;
			if(!debugInfoDictionary.findValue(lineInfo->m_LineAction,debugInfo)){
				debugInfo=new DebugInfo(lineInfo->m_LineAction);
				debugInfoDictionary.insertKeyAndValue(lineInfo->m_LineAction,debugInfo);
			} 
			debugInfo->m_BreakPoint=f;
			pthread_mutex_unlock(&m_mutex);
			return lineInfo->m_LineAction;
		} else {
			return 0;
		}
	}

AST *
DebugEnv::setReadWatch(int LineNo,RWCString var,CallBack f)
	{
		return 0;
	}


AST *
DebugEnv::setWriteWatch(int LineNo,RWCString var,CallBack f)
	{
		return 0;
	}

AST *
DebugEnv::singleStep(int mode,CallBack f)
	{
		AST * ret=m_where;
		pthread_mutex_lock(&m_mutex);
		m_single_step=mode;
		m_single_step_cb=f;
		sem_post(&m_sem);
		pthread_mutex_unlock(&m_mutex);
		return ret;
	}


void
DebugEnv::sendPath(CallBack f)
	{
		m_path_cb=f;
	}

AST *
DebugEnv::sendPath(EdgeList & edgeList)
	{
		EdgeList lEdgeList;
		if(m_path_cb){
			//EdgeListIterator elit(edgeList);
			for(const auto& elit: edgeList) {
			//while(++elit){
				lEdgeList.append(elit);
			}
			DebugInfo debugInfo(m_where);
			debugInfo.m_EdgeList=&lEdgeList;
			(*m_path_cb)(&debugInfo);
		}
		AST * ret=m_where;
		return ret;
	}



AST *
DebugEnv::continueRun(CallBack f)
	{
		return 0;
	}


AST *
DebugEnv::evaluateExpression(int LineNo,RWCString ex)
	{
		ASTBase *	ATLASroot = 0;
		getEnv();
		setEnv(LineNo);
		m_box->ResetString(ex,AtlasDLG::StatementMode);
		m_box->parser()->debug_evaluate(&ATLASroot);
		restoreEnv();
		return (AST *)ATLASroot;
	}


AST *
DebugEnv::assignExpression(int LineNo,RWCString ex)
	{
		ASTBase *	ATLASroot = 0;
		getEnv();
		setEnv(LineNo);
		m_box->ResetString(ex,AtlasDLG::StatementMode);
		m_box->parser()->assignment_expression(&ATLASroot);
		restoreEnv();
		return (AST *)ATLASroot;
	}

void
DebugEnv::getEnv()
	{
		m_saveScope=m_box->parser()->getScope();
	}
	
void
DebugEnv::setEnv(int LineNo)
	{
		LineInfo * x=0;
		lineInfoList.findLineInfo(LineNo,x);
		if(x){
			m_box->parser()->setScope(x->getScope());
		}
	}
	
void
DebugEnv::restoreEnv()
	{
		if(m_saveScope){
			m_box->parser()->setScope(m_saveScope);
		}
	}

DebugInfo::DebugInfo(AST * x)
	:m_ast(x)
	,m_ReadEvent(0)
	,m_WriteEvent(0)
	,m_BreakPoint(0)
	,m_Router(0)
	{
	}
