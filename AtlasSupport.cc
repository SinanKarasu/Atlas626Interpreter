
#include	<stdlib.h>
#include	"AtlasAST.h"
#include	"AtlasStd.h"
#include	"AtlasParser.h"
#include	"Dictionary.h"
#include	"Signal.h"

#include	"DebugEnv.h"

DeviceDictionary	VirtualDevices;

int trace_level;
int debug_statno;

static int ErrorFlag = 0;
static int StatementErrorFlag = 0;
static int ErrorCount = 0;
static int WarningCount = 0;

static int EnvironmentInitialized=0;

void
set_warning_flag()
	{
		WarningCount++ ;
	}

void
set_error_flag(int tedl=0)
	{
		if(!tedl){
			ErrorFlag = 1;
			StatementErrorFlag=1;
		}
		StatementErrorFlag=1;
		ErrorCount++ ;
	}
		
void
clear_error_flag()
	{
		ErrorFlag = 0;
		StatementErrorFlag=0;
		ErrorCount = 0;
	}

void
clear_statement_error_flag()
	{	
		StatementErrorFlag=0;
	}
	
int 
_error_()
	{	
		return ErrorFlag;
	}
int 
sane()
	{
		return StatementErrorFlag==0;
	}


RWCString
unquoted(RWCString & string)
	{
		RWCString a( string );
		RWCRegexp re( "'" );
		
		return (a(re) = "");
	}

int
verify( AST * t, AST * s )	// t=target, s=source
	{
		return
		(
			( t->getType() == s->getType() )
		||	( (t->getType() == DecimalNumberValue) && (s->getType() == IntegerNumberValue) )
		);
	}




void
Warning( RWCString E )
	{
		cerr << "Warning: " << E << endl;
		set_warning_flag();
	}


void
Warning( RWCString E, int l )
	{
		cerr << "Line:" << l << " ";
		Warning( E );
	}

void
Warning( RWCString E, ANTLRTokenPtr t )
	{
		if ( t == 0 ){
		
			Warning( E );
		}else{
			Warning( E + " " + t->getText() , t->getLine() );
		}
	}

void
Warning( RWCString E, AST * a )
	{		
		if ( a ){
			if(a->getToken()==0){
				Warning( E + " " + a->getName());
			} else {
				Warning( E , a->getToken());
			}
		}else{
			Warning( E );
		}
	}


void
TedlError( RWCString E )
	{
		cerr << "ERROR: " << E << endl;
		set_error_flag(1);
	}

void
TedlError( RWCString E, int l )
	{
		cerr << "Line:" << l << " ";
		TedlError( E );
	}
	
void
Error_Report( RWCString E )
	{
		cerr << "ERROR: " << E << endl;
		set_error_flag();
	}



void
Error_Report( RWCString E, int l )
	{
		cerr << "Line:" << l << " ";
		Error_Report( E );
	}

void
Error_Report( RWCString E, ANTLRTokenPtr t )
	{
		if ( t == 0 ){
		
			Error_Report( E );
		}else{
			Error_Report( E + " " + t->getText() , t->getLine() );
		}
	}

void
Error_Report( RWCString E, AST * a )
	{		
		if ( a ){
			if(a->getToken()==0){
				Error_Report( E + " " + a->getName());
			} else {
				Error_Report( E , a->getToken());
			}
		}else{
			Error_Report( E );
		}
	}





Scope *
AtlasParser::getScope( ANTLRTokenPtr t )
	{
		AST * x=0;
		
		if ( x=scope->findSymbolValue( t->getText()) ){
			return x->getScope();
		}else{
			Error_Report( RWCString( t->getText()) + " has no private Scope", t );
			return 0;
		}
	}

	
void
AtlasParser::init()
	{
        	ANTLRParser::init();
        	
        	
        	// take out the following and move into main.....
        	if(!EnvironmentInitialized){
        		EnvironmentInitialized	= 1;
			char *debug_flag_value = getenv( "DEBUG_FLAG" );
		
			if ( debug_flag_value && (*debug_flag_value >= '1') ){
				debug_flag = 1;
			} else {
				debug_flag = 0;
			}
		
			char *trace_level_value = getenv( "ATLAS_TRACE_LEVEL" );
		
			if ( trace_level_value && (*trace_level_value >= '1') ){
				trace_level = atoi(trace_level_value);
			} else {
				trace_level = 0;
			}
		
			char *debug_statno_value = getenv( "ATLAS_DEBUG_STATNO" );
		
			if ( debug_statno_value && (*debug_statno_value >= '0') ){
				debug_statno = atoi(debug_statno_value);
			} else {
				debug_statno = -1;
			}
		}

	}

	
void
AtlasParser::done()
	{
	}

LabelType
AtlasParser::getLabelType(const RWCString & name,Scope * s) const
	{
		AST * x=0;
		Scope * cscope;
		
		if ( s ) cscope=s; else  cscope=scope;
		if ( !cscope )return Undefined_Label;
		
		if ( x=cscope->findSymbolValue(name) ){

			return ((LabelAST *)x)->getLabelType();
		} else {
			if(!cscope->isParameterMode()){
				if(cscope=cscope->getPrev()){
					return getLabelType(name,cscope);
				}
			}
		}
		return Undefined_Label;
	}

LabelType
AtlasParser::getLabelTypeGlobal(const RWCString & name,Scope * s) const
	{
		AST * x=0;Scope * cscope;
		if(s)cscope=s;else cscope=scope;
		if(!cscope)return Undefined_Label;
		if(x=cscope->findSymbolValue(name)){
			return ((LabelAST *)x)->getLabelType();
		} else if(cscope=cscope->getPrev()){
			return getLabelTypeGlobal(name,cscope);
		}
		return Undefined_Label;
	}

AST *
AtlasParser::getTokenLabel(const ANTLRChar * name,const Scope * s) const
	{
		AST * x=0;
		const Scope *	cscope;
		
		if(s) cscope=s;else cscope=scope;
		if(!cscope) return 0;
		if(!(x=cscope->findSymbolValue(name))){
			if(!cscope->isParameterMode()){
				if(cscope=cscope->getPrev()){
					x=getTokenLabel(name,cscope);
				}
			}
		}
		return (AST *)x;
	}

AST *
AtlasParser::getTokenLabelGlobal( const ANTLRChar * name, const Scope * s ) const
	{
		AST * x=0;const Scope * cscope;
		if(s)cscope=s;else cscope=scope;
		if(cscope){
			if(!(x=cscope->findSymbolValue(name))){
				if(cscope=cscope->getPrev()){
					x=getTokenLabelGlobal(name,cscope);
				}
			}
		}
		return (AST *)x;
	}

AST *
AtlasParser::getEventIndicatorLabel( const ANTLRChar * name, const Scope * s ) const
	{
		AST * x;const Scope * cscope;
		if(s)cscope=s;else cscope=scope;
		if(!cscope)return 0;
		if(!(x=cscope->findEventIndicator(name))){
			if(cscope=cscope->getPrev()){
				x=getEventIndicatorLabel(name,cscope);
			}
		}
		return (AST *)x;
	}



AST *
AtlasParser::isType(ANTLRChar * name ,LabelType t)
	{
		return (getLabelType(name)==t)?getTokenLabel(name):0;
	}

AST *
AtlasParser::isEventIndicator(ANTLRChar * name )
	{
		return getEventIndicatorLabel(name);
	}

AST *
AtlasParser::isTypeGlobal(ANTLRChar * name ,LabelType t)
	{
		return (getLabelTypeGlobal(name)==t)?getTokenLabelGlobal(name):0;
	}

TheType
AtlasParser::getType( ANTLRChar * name )
	{
		AST *	label = getTokenLabel( name, 0 );
		
		if ( label ){
			return label->getType();
		}else{
			return UndefinedTypeValue;
		}
	}

TheType
AtlasParser::getGlobalType( ANTLRChar * name )
	{
		AST *	label = getTokenLabelGlobal( name, 0 );
		
		if ( label ){
			return label->getType();
		}else{
			return UndefinedTypeValue;
		}
	}


AST *
AtlasParser::insertFstatno( ANTLRTokenPtr t, Scope * s)
	{
		// parse the testno and stepno
		int	_bflag  = 0,
			_eflag  = 0,
			changed = 0,
			_test,
			_step;
		LineAction * lineAction=0;
		
		clear_statement_error_flag();
			
		RWCString	str=t->getText();
		Fstatno *	fstatno = 0;
		
		VerbLineNo = t->getLine(); // set the line number of verb in Parser
		
		if ( debug_flag ){
			cout << t->getLine() << " " <<
			RWCString( t->getText() ) << " " << guessing << endl;
		}
		switch ( str[0] ) { _eflag = 1; }
		
		if ( FlagStatement ) { _bflag=1; }
		
		if ( str(1,4) == "    " ){
			_test = -1;
		}else{
			_test = atoi( RWCString(str(1,4)) );		
		}
		
		if ( str(5,2) == "  " ){
			_step = -1;
		}else{
			_step = atoi( RWCString(str(5,2)) );		
		}
		
		if ( (_test ==-1) && (_step == -1) ){
		
			// nothing to insert, no statement number
			// see if there was B-statement in the previous statement
			if ( FlagStatement ){
				Warning( "B Flag Statement must precede a STATEMENT number", t );
			}
		
		}else if ( (_test == -1) || (_test == _prev_test) ){ // same ballpark
		
			if ( _step <= _prev_step ){
				// error , step no went backwards
				Warning( "Step number is out of order.", t );
				_prev_step = _step;
			}else{
				// good, so construct
				if ( _test != -1 ){
					_prev_test=_test;
				}
				_prev_step = _step;
				changed    = 1;
			};
			
		}else if( _test <  _prev_test ) {	// test no went backwards
			_prev_test=_test;
			_prev_step=_step;
			Warning( "TestNo field out of order (went backwards) ", t );
		}else{
			// larger test no. So construct.
			_prev_test = _test;
			_prev_step = _step;
			changed    = 1;
		}
		
		FlagStatement = 0;
		if ( changed ){
			fstatno = new Fstatno;
			fstatno->_testno = _prev_test;
			fstatno->_stepno = _prev_step;
			fstatno->_entry  = new LineAction(t,s,fstatno);
			lineAction=_prev_LineAction= fstatno->_entry;
			
			
			modulestatscope->insertKeyAndValue(_prev_test*100+_prev_step,fstatno);
			if(_bflag){
				insertTarget(_prev_test*100+_prev_step,fstatno->_entry);
			}

			VerbStatNo = _prev_test * 100 + _prev_step;
		} else {
			VerbStatNo=-1;
			lineAction= new LineAction(t,s,_prev_LineAction);
		}

		if(debug_env){
			lineInfoList.append(new LineInfo(VerbLineNo,lineAction));
		}
		
		return lineAction;
		
	};

int
Fstatno :: getLine() const
		{
			return _entry->getToken()->getLine();
		}
		
ostream&
operator << (ostream& output , Fstatno * n)
	{
		output	<< setw(4) << setfill('0') << n->_testno
			<< setw(2) << setfill('0') << n->_stepno
			;
		return output;
	}

void
AtlasParser::insertTarget(RWInteger sno,AST * a)
	{
		GoToStatementStack * tsstack=0;
		TargetStatement * ts=new TargetStatement(a,ContextLevel,ContextDepth);
		TargetStatement * us;// unresolved
		// insert it. Guaranteed to be unique, since statno only increases.
		GoToTargets.insertKeyAndValue(sno,ts);
		if(UnresolvedTargets.findValue(sno,tsstack)){
			while(!tsstack->isEmpty()){
				us=tsstack->pop();
				verifyGoToTarget(us,ts);// verify and set error flag
				us->_a->init(a);// do it anyway
			}
			delete tsstack; // future ones will see it from GoToTargets
			UnresolvedTargets.remove(sno);
		} 
	}

RWCString
stepNo(RWInteger s)
	{
		RWCString sn;
		for(int i=0;i<6;i++){
			sn=RWCString(char('0'+(s % 10)))+sn;
			s=s/10;
		}
		return sn;
	}

void
AtlasParser::ResolveTargets()
	{
		GoToStatementStack * tsstack=0;
		GoToDictionaryIterator git(UnresolvedTargets);
		TargetStatement * ts;// target from GoToTargets
		TargetStatement * us;// unresolved from UnresolvedTargets
		while(++git){
			tsstack=git.value();
			while(!tsstack->isEmpty()){
				us=tsstack->pop();
				if(GoToTargets.findValue(git.key(),ts)){
					// actually, this clause should NEVER get executed.
					// Since this routine is (usually) called at the end of
					// parse, for a program with good statement numbers,
					// insertTarget will resolve all the references by the
					// time we get here.
					verifyGoToTarget(us,ts); // verify and set error flag
					us->_a->init(ts->_a); // do it anyway
				} else {
					Error_Report("Can not find GO TO target "+stepNo(git.key()));
				}
			}
			delete tsstack;
			UnresolvedTargets.remove(git.key());
		}
		GoToTargets.clear(); // Remove (maybe NOT if we need it for debug...).
	}



void
AtlasParser::insertUnresolved(RWInteger sno,AST * a)
	{
		GoToStatementStack * tsstack=0;
		TargetStatement * ts=new TargetStatement(a,ContextLevel,ContextDepth);
		// insert it. Guaranteed to be unique, since statno only increases.
		if(UnresolvedTargets.findValue(sno,tsstack)){
			tsstack->push(ts);
		} else {
			tsstack=new GoToStatementStack();
			tsstack->push(ts);
			UnresolvedTargets.insertKeyAndValue(sno,tsstack);
		}
	}

void
AtlasParser::incrementContextDepth(AST * a, EntryType et)
	{
		ContextDepth++;
		if(ContextDepth>=MaxContextDepth){
			MaxContextDepth=ContextDepth;
			ContextLevel.reshape(ContextDepth+1);
			LevelEntry.reshape(ContextDepth+1);
			LevelType.reshape(ContextDepth+1);
			LevelSnum.reshape(ContextDepth+1);
		}
		ContextLevel[ContextDepth]=ContextLevel[ContextDepth]+1;
		LevelEntry[ContextDepth]=a;
		LevelType[ContextDepth]=et;
		LevelSnum[ContextDepth]=VerbStatNo;
	}		
	
void
AtlasParser::decrementContextDepth()
	{
		LevelEntry[ContextDepth] = 0;
		LevelType[ContextDepth]  = EntryUndefined;
		LevelSnum[ContextDepth]  = -1;
		ContextDepth--;
	}

void
AtlasParser::incrementContextLevel()
	{
		ContextLevel[ContextDepth] = ContextLevel[ContextDepth]+1;
	}



AST *
AtlasParser::verifyGoToTarget( TargetStatement * unresolved, TargetStatement * target )
	{
		for( int i=0; i<target->_ContextDepth; i++ ){
			if ( unresolved->_ContextLevel[i] != target->_ContextLevel[i] ){
				// oops mismatch
				Error_Report( "STATNO# is not in scope of GOTO from Line:" );
				return 0;
			} 
		}
		return target->_a;
	}

AST *
AtlasParser::verifyLeaveResumeTarget( AST * snum, EntryType lt )
	{
		const Fstatno * fstatno;
		AST * ret;
		
		for( int i=ContextDepth; i>=0; i-- ){
			if( snum ){
				// LEAVE/RESUME with target
				if ( LevelSnum[i] == snum->getInteger() ){
					if ( LevelType[i] == lt ){
						snum->init(LevelEntry[i]);
						return snum;
					}else{
						Error_Report( "Target Statament Number and LEAVE/RESUME type do not match.",
								snum );
						return 0;
					}
				}
			}else{
				// LEAVE/RESUME without target
				if ( LevelType[i]==lt ){
					ret = new StatementNumber();
					ret->init( LevelEntry[i] );
					return ret;
				}
			}
		}
		Error_Report( "No LEAVE/RESUME target statement found.", snum );				
		return 0;		
	}
	

void
ArgCheck( int curArg, AST * target, AST * source, int line_no )
	{
		TheType	tt, st;
		int	maxArgs;

		if ( !source ){		// should never get here
			return;
		}else if( !target ){	// should never get here
			return;
		}
		maxArgs = target->getInteger();
		if ( curArg > maxArgs ){
		
			Error_Report( "Too many arguments to function.", line_no );
		}else{
			IntegerNumber x(curArg);
			tt = target->getType( &x );
			st = source->getType();
			
			if ( tt == st ){
			}else if( (tt==DecimalNumberValue)&&(st==IntegerNumberValue) ){
			}else if( (tt==StringOfCharTypeValue)&&(st==CharTypeValue) ){
			}else if( (tt==StringOfBitTypeValue)&&(st==BitTypeValue) ){
			}else{
				Error_Report( "Argument is not compatible.", line_no );
				cerr << "Argument Number : " << curArg << endl;
			}
		}
	}


void
ResCheck( int curRes, AST * target, AST * source, int line_no )
	{
		TheType	tt, st;
		int	maxArgs,maxRess;
		
		if ( !source ){		// should never get here
			return;
		}else if( !target ){	// should never get here
			return;
		}
		maxArgs = target->getInteger(0);
		maxRess = target->getInteger(1);
		if ( curRes > maxRess ){
			Error_Report( "Too many RESULTS.", line_no );
		}else{
			IntegerNumber x( curRes + maxArgs ); // The results are after the Args...
			tt = target->getType( &x );
			st = source->getType();

			if ( tt == st ){
			}else if ( (tt==DecimalNumberValue) && (st==IntegerNumberValue) ){
			}else {
				Error_Report( "RESULT  is not compatible. ", line_no );
				cerr << "Result Number : " << curRes << endl;
			}
		}
	}

void
AtlasParser::setHI()
	{
		lHI->setInteger(0,1);
		setNOGO();
	}
void
AtlasParser::setLO()
	{
		lLO->setInteger(0,1);
		setNOGO();
	}
void
AtlasParser::setGO()
	{
		lHI->setInteger(0,0);
		lLO->setInteger(0,0);
		lGO->setInteger(0,1);
		lNOGO->setInteger(0,0);
	}

void
AtlasParser::setNOGO()
	{
		lGO->setInteger(0,0);
		lNOGO->setInteger(0,1);
	}


void
AtlasParser::syn(
			ANTLRAbstractToken	*	tok,
			ANTLRChar		*	egroup,
			SetWordType		*     	eset,
			ANTLRTokenType			etok,
			int 				k
		)
	{
		
		int line = ((AtlasToken *)LT(1))->getLine();
		int col =  ((AtlasToken *)LT(1))->col;

		syntaxErrCount++;

		cerr	<<	" line "	<< line 
			<<	" col "	<< col
			<<	" syntax error at " 
			<<	LT(1)->getText();

		if (!etok && !eset) {
			cerr <<  endl;
			return;
		}
		
		if (k == 1) {
			cerr <<  " missing ";
		} else {
			cerr	<< ";"
				<<	LT(1)->getText()
				<<   " not" ;
			if (set_deg(eset) > 1){
				cerr << " in";
			}
		}
		if (set_deg(eset) > 0) {
			edecode(eset);
		} else {
			cerr <<  token_tbl[etok];
		}

		if (strlen(egroup) > 0){
			cerr << " in " <<  egroup;
		}
		cerr << endl ;
		set_error_flag();
	}

void
AtlasParser::edecode(SetWordType *a)
{
	SetWordType *p = a;
	SetWordType *endp = &(p[bsetsize]);
	unsigned e = 0;

	if ( set_deg(a)>1 ){
		cerr << " {";
	}
	do {
		SetWordType t = *p;
		SetWordType *b = &(bitmask[0]);
		do {
			if ( t & *b ){
				cerr << " " << token_tbl[e];
			}
			e++;
		} while (++b < &(bitmask[sizeof(SetWordType)*8]));
	} while (++p < endp);
	if ( set_deg(a)>1 ){
		 cerr <<  " }";
	}
}





int
assignmentCompatible(AST * E1,AST * E2)
	{
		if(E1->data()->check(E2->data())){
			return 1;
		} else {
			return 0;
		}
	}

int
ArgsCheck(AST * E1,AST * E2)
	{

		TheType         tt, st;
		int             maxArgs;
		int result=1;
		IntegerNumber idx(0);
		if (!E1) {
			//should never get here
			return 0;
		} else if (!E2) {
			//should never get here
			return 0;
		}

		int             args1 = E1->getInteger(0);
		int             args2 = E2->getInteger(0);

		if (args1 != args2) {
			Error_Report("Argument counts do not match.", E1);
			return 0;
		} else if (args1>0){

			for (int i = 1; i <= args1; i++) {
				idx.setInteger(0,i);
				AST            *td = E1->data(&idx);
				AST            *sd = E2->data(&idx);
				if (!(td->check(sd))) {
					Error_Report("Argument is not compatible.", E1);
					cerr << "Argument Number : " << i << endl;
					result=0;
				}
			}
		}
		int		ress1=0;
		int		ress2=0;
		if(((LabelAST *)E1)->getLabelType()==Procedure_Label){
			ress1 = E1->getInteger(1);
			ress2 = E2->getInteger(1);
		}

		if (ress1 != ress2) {
			Error_Report("Result counts do not match.", E1);
			return 0;
		} else if(ress1>0) {
			for (int i = 1; i <= ress1; i++) {
				idx.setInteger(0,args1+i);
				AST            *td = E1->data(&idx);
				AST            *sd = E2->data(&idx);
				if (!(td->check(sd))) {
					Error_Report("Result is not compatible.", E1);
					cerr << "Result Number : " << i << endl;
					result=0;
				}
			}
		}
		return result;

	}





Scope *
AtlasParser::getScope()		{ return scope; }

void
AtlasParser::setScope(Scope * s)	{ scope = s; }

int
AtlasParser::nounAllowsDescriptors(const NounEntry * nounEntry)
	{
		if(! nounEntry ) {
			return 1;
		} else if(
				(nounEntry->noun=="EARTH")
			||	(nounEntry->noun=="COMMON")
			||	(nounEntry->noun=="SHORT")
			){
				return 0;
		} else {
			return 1;
		}
	}	

	
int
AtlasParser::getGO(){return lGO->getInteger();}
	
int
AtlasParser::getNOGO(){return lNOGO->getInteger();}
	
int
AtlasParser::getHI(){return lHI->getInteger();}

int
AtlasParser::getLO(){return lLO->getInteger();}
	
void
AtlasParser::insertEscape(AST * ei,AST * ep)
		{
			if(!allEscapes){
				allEscapes=new ASTList;
			}
			if(!allEscapes->contains(ei)){
				allEscapes->append(ei);
				ei->insert(ep);
			}
			return;
		}

void
AtlasParser::removeEscape(AST * ei)
		{
			if(!allEscapes){
			} else if(ei) {
				if(allEscapes->contains(ei)){
					allEscapes->remove(ei);
					ei->remove(0);
				}
			} else {
				ASTListIterator aeit(*allEscapes);
				while(++aeit){
					aeit.key()->remove(0);
				}
				allEscapes->clear();
			}
			return;
		}

void 
AtlasParser::tracein(const char *r)
	{
		tracefile << "enter rule " << r << endl;
	}

void
AtlasParser::traceout(const char *r)
	{
		tracefile << "exit rule " << r << endl;
	}

int
AtlasParser::isType(ANTLRTokenPtr ,ModifierEntry * m,const RWCString  a)
	{
		return (m->typeCode==a);
	}

int
AtlasParser::isType(ModifierEntry * m,const RWCString  a)
	{
		return (m->typeCode==a);
	}

int
AtlasParser::isType(ModifierEntry * m,const RWCString a,const RWCString  b)
	{
		return (m->typeCode==a||m->typeCode==b);
	}

int 
AtlasParser::isType(ModifierEntry * m,const RWCString a,const RWCString  b,const RWCString c)
	{
		return (m->typeCode==a||m->typeCode==b||m->typeCode==c);
	}

ModifierEntry *
AtlasParser::insertQuotedModifier(const RWCString quoted,NounEntry * nounEntry)
	{
		ModifierEntry * modifierEntry=new ModifierEntry(nounEntry);
		RWCString modifier=quoted;
		RWCRegexp re("'");
		modifier(re)="";
			
		modifierEntry->modifier=modifier;
		nounEntry->modifierDictionary.insertKeyAndValue(modifier,modifierEntry);
		QuantityList * quantityList=new QuantityList;
		quantityList->insert(new Quantity(quoted));
		modifierEntry->insertQuantityList(quantityList);
		return modifierEntry;
	}

//void insertModifiers(ModifierEntryList * modifierEntryList,NounEntry * nounEntry)
//	{
//		ModifierEntryListIterator melit(*modifierEntryList);
//		while(++melit){
//			ModifierEntry * me=melit.key();
//			QuantityList * quantityList=new QuantityList;
//			modifierEntry->insertQuantityList(quantityList); 
//			quantityList->insert(new Quantity("q"+me->modifier));
//			nounEntry->modifierDictionary.insertKeyAndValue(me->modifier,me);
//		};
//	}



TheType
AtlasParser::getModifierType(AST * re,AST * cm)
	{
		TheType	TypeOfModifier=UndefinedTypeValue;

		StringAST m("~MODIFIERS");
		if(re){
			AST *mods=re->data(&m);
			AST *mod=mods->data(cm);
			if(mod){
				TypeOfModifier=mod->getType();
			} else {
				TypeOfModifier=UndefinedTypeValue;
			}
		}
		return TypeOfModifier;
	}
			
void
AtlasParser::reset()
	{
        		FlagStatement		= 0;
        		
        		// Context info { BEGIN/END } { BLOCK/IF/DO/PROCEDURE/FUNCTION etc...}
        		ContextDepth		= 0;
        		MaxContextDepth		= 0;
        		
        		// Line/Default Line No for Error_Report/printout/debugger
			VerbLineNo		= 0;
			_prev_test		= 0;
			_prev_step		= 0;
			_prev_LineAction	= 0;
	}
	

Scope *
AtlasParser::getExternalScope() const
	{
       		return externalScope;
	}
Scope *
AtlasParser::getGlobalScope() const
	{
       		return globalScope;
	}

ModifierEntry *
AtlasParser::theModifierEntry(const RWCString modifier,NounEntry * nounEntry,RWCString & left)
	{
		return mnemonicsDB->theModifierEntry(modifier,nounEntry,left);
	}


NounEntry *
AtlasParser::theNounEntry(const ANTLRTokenPtr  nid,int & howmany)
{
	NounEntry * nounEntry=0;int j;
	RWCString noun=nid->getText();
	howmany=1;
	while(1){
		if(nounEntry=mnemonicsDB->theNounEntry(noun)){
			break;
		} else if(howmany>=8){	// arbitrary
			break;
		}
		noun+= RWCString(" ") + LT(++howmany)->getText();
	}
	
	if(nounEntry){
		return nounEntry;
	} else {
		howmany=0;
		return 0;
	}
}
