
#include	"AtlasStd.h"
#include	"AtlasParser.h"
#include	"ExceptionTypes.h"
#include	"DebugEnv.h"
#include	"ExecEnv.h"

extern ExecEnv execEnv;


ActionAST::ActionAST(ANTLRTokenPtr p):AST(p){};
ActionAST::~ActionAST(){};

AST *	ActionAST::Accept	( ASTVisitor & v )	{ return v.VisitActionAST( this ); }

LineAction::LineAction( ANTLRTokenPtr p, Scope * scope, Fstatno	* fstatno )
	:ActionAST(p),m_scope(scope),m_fstatno(fstatno),m_prevfstatno(0){m_prevfstatno=this;}

LineAction::LineAction( ANTLRTokenPtr p, Scope * scope, LineAction	* prevfno )
	:ActionAST(p),m_scope(scope),m_fstatno(0),m_prevfstatno(prevfno)
		{m_fstatno=m_prevfstatno->m_fstatno;}

Scope	*
LineAction::getScope	() const
	{
		return m_scope;
	}

void
LineAction::setScope	( Scope * scope )
        {
        	// only called during parse....
        	m_scope=scope;	// needed by (this)::trace()
        }

AST *	LineAction::eval( AST * a )
	{
		AST * x;
		trace(0);
		extern DebugEnv * debugEnv;
		debugEnv->blockProgress(this,getBreakPoint());
		extern astream sout;
		
		if ( a == this ){	// I am supposed to arbitrate, don't know how.
		
			x = ASTright();	// pass it to right...
			return x->eval(x);
		}else{
			if ( x=ASTdown() ){
				try{
					x=x->eval(a);
					if ( x ){
						return x;
					}
				} catch ( TedlExecutionError eerr ) {
					sout << eerr << " At statement: " ;
					sout << m_scope << std::endl;
				}
			} 
		}
		return ASTright();	// sequential execution
	}

int LineAction::getLine() const {return getToken()->getLine();}


astream&	LineAction::operator<<( astream &s )
	{
		s <<  m_scope ;
		return s;
	}

void LineAction::trace(int level)
	{
		if(m_prevfstatno==this){
			m_delta=0;
		} else {
			m_delta=getLine()-m_prevfstatno->getLine();
		}
		m_scope->setCallChain(m_scope);
		m_scope->setCurrentLine(getLine());
		m_scope->setDeltaLine(m_delta);
		m_scope->setCurrentFstatno(m_fstatno);
		if(execEnv.execTrace()){
		
			std::cerr << "@:" <<  m_scope << std::endl;
		}
		if((debug_statno==(m_fstatno->_testno*100+m_fstatno->_stepno))&&(m_delta==0)){
			int ijk=m_delta;
		}
	
	}


BeginBlockAction::BeginBlockAction( ANTLRTokenPtr p)
	:ActionAST(p)
	{}

AST *	BeginBlockAction::eval( AST * a )
	{
		AST * x;
		//if(execEnv.execTrace()){
			debugtrace	<<	"// BEGIN BLOCK: "
					<<	getName()
					<<	std::endl;
		//}
		x=ASTright();	// Point to TargetAction
		if(a==this){	// let TargetAction arbitrate
			return x->eval(x);
		}
		// Otherwise skip over TargetAction
		return x->ASTright();	// sequential execution
	}

EndBlockAction::EndBlockAction( ANTLRTokenPtr p)
	:ActionAST(p){}

AST *	EndBlockAction::eval( AST * a )
	{
		AST * x;
		//if(execEnv.execTrace()){
			debugtrace	<<	"// END BLOCK: "
					<<	getName()
					<<	std::endl;
		//}
				
		return ASTright();	// sequential execution
	}

IndexAction::IndexAction( ANTLRTokenPtr p ):ActionAST(p){ m_index = new IntegerNumber; };

AST *	IndexAction::eval( AST * a )
	{
		AST *	x;
		
		if ( a == this ){
			x = ASTdown()->eval( ASTdown() );	// special message to array object to return the first location
			x = x->eval( x );
		}else{
			m_index->setInteger( 0, this->ASTdown()->ASTright()->eval()->getInteger() );
			x = ASTdown()->eval( m_index );
			
			if ( a ) x = x->eval( a->eval() );
		}
		return x;
	}


TheType
IndexAction::getType( AST * a ) const
	{
		AST *	x = ASTdown()->eval( ASTdown() );// special message to array object to return the first location
		return	x->getType();
	}

astream&
IndexAction::operator<<( astream& s )
	{
		AST * l=ASTdown();
		AST * i=ASTdown()->ASTright();

		s << l << "[" << i << "]";
		
		return s;
	}


// ExecuteFunction
// The AST Tree looks as follows:
//
//  ExecuteFunction
//        |
//        v
//      Proxy -> Arg1 -> ..... Argn
//        |
//        v
//      FunctionLabel -> {Code....}
//        |
//        v
//       [Arg 1] -> .... [Argn]
//      
   
ExecuteProgramAction::ExecuteProgramAction( ANTLRTokenPtr p,Scope * scope,AtlasParser * parser )
	:ActionAST(p)
	,m_scope(scope)
	,m_parser(parser)
	{}

AST *	ExecuteProgramAction::eval( AST * a )
	{
		// first set up the arguments by passing them to the func.
		// note that they are attached to the Proxy.
		// Function will evaluate and return the result.
		
		m_parser->setScope(m_scope);
		return 0;
	};


ExecuteFunctionAction::ExecuteFunctionAction( ANTLRTokenPtr p ):ActionAST(p){};


AST *	ExecuteFunctionAction::eval( AST * a )
	{
		// first set up the arguments by passing them to the func.
		// note that they are attached to the Proxy.
		// Function will evaluate and return the result.
		
		return ASTdown()->eval( ASTdown()->ASTright() ); 	
	};

	
void	ExecuteFunctionAction::setInteger( int indx, Long value )
	{
		if ( ASTdown()->getInteger() != value ){
		
			Error_Report( " Argument Count Mismatch ", this );
		}
		argcount = value;
	}



// ExecuteProcedure
// The AST Tree looks as follows:
//
//  ExecuteProcedure
//        |
//        v
//      Proxy -> Proxy -> Arg1 -> ..... Argn
//        |       |
//        |      Proxy -> Res1  -> ..... Resm
//        |
//        |
//        v
//      ProcedureLabel -> {Code....}
//        |
//        v
//       Proxy  ->[Arg 1] -> .... [Argn]
//        | 
//       Proxy  -> Res1 -> ..... Resm
//        |
//       Proxy  -> Sav1 -> ..... Savm
//      

ExecuteProcedureAction::ExecuteProcedureAction( ANTLRTokenPtr p ):ActionAST(p){};

AST *	ExecuteProcedureAction::eval( AST * a )
	{
		// first set up the arguments by passing them to the func.
		// note that they are attached to the Proxy.
		// Function will evaluate and return the result.
		
		ASTdown()->eval( ASTdown()->ASTright() );
		return ASTright();
	};


void	ExecuteProcedureAction::setInteger( int indx, Long value )
	{
		argcount = value;
	}


ExecuteAction::ExecuteAction( ANTLRTokenPtr p ):ActionAST(p){};

AST *	ExecuteAction::eval( AST * a )
	{
		evalVisitor	go;
		
		go.Execute( ASTright(), 0 );
		return 0;
	}

TextInputAction::TextInputAction( ANTLRTokenPtr p ):ActionAST(p){};



AST *	TextInputAction::eval( AST * ifileAST )
	{
		AST *	argAST = ASTdown();
		AST *	fmtAST = ASTdown()->ASTright();
	
		astream& s = *(ifileAST->getStream());
		
		if ( ifileAST->getInteger() )	// if enabled
			if ( fmtAST != 0 )
				s >> fmtAST->eval() >> argAST->eval();
			else
				s >> argAST->eval();
		else
			Error_Report( "Attempting to read from file that is not enabled." );

		return ASTright();
	}

DataInputAction::DataInputAction( ANTLRTokenPtr p ):ActionAST(p){};

AST *	DataInputAction::eval( AST * ifileAST )
	{
		AST *	argAST = ASTdown();
		AST *	fmtAST = ASTdown()->ASTright();
	
		astream& s = *(ifileAST->getStream());
		
		if ( ifileAST->getInteger() )	// if enabled
			if ( fmtAST != 0 )
				s >> fmtAST->eval() >> argAST->eval();
			else
				s >> argAST->eval();
		else
			Error_Report( "Attempting to read from file that is not enabled." );

		return ASTright();
	}




DataOutputAction::DataOutputAction( ANTLRTokenPtr p ):ActionAST(p){};

AST *	DataOutputAction::eval( AST * ofileAST )
	{
		AST *	argAST = ASTdown();
		AST *	fmtAST = ASTdown()->ASTright();
	
		astream& s = *(ofileAST->getStream());
		
		if ( ofileAST->getInteger() )	// if enabled
			if ( fmtAST != 0 )
				s << fmtAST->eval() << argAST->eval();
			else
				s << argAST->eval();
		else
			Error_Report( "Attempting to write to file that is not enabled." );

		return ASTright();
	}


TextOutputAction::TextOutputAction( ANTLRTokenPtr p ):ActionAST(p){};

AST *	TextOutputAction::eval( AST * ofileAST )
	{
		AST *	argAST = ASTdown();
		AST *	fmtAST = ASTdown()->ASTright();
	
		astream& s = *(ofileAST->getStream());
		
		if ( ofileAST->getInteger() )	// if enabled
			if ( fmtAST != 0 )
				s << fmtAST->eval() << argAST->eval();
			else
				s << argAST->eval();
		else
			Error_Report( "Attempting to write to file that is not enabled." );

		return ASTright();
	}

FilePositionAction::FilePositionAction( ANTLRTokenPtr p ):ActionAST(p){};

AST *	FilePositionAction::eval( AST * a )
	{
		AST *	file = ASTdown()->eval();
		int	pos  = ASTdown()->ASTright()->eval()->getInteger();

		astream& s = *(file->getStream());
		
		s.seekp( pos );

		return file;
	}

FileDataOutputAction::FileDataOutputAction( ANTLRTokenPtr p ):ActionAST(p){};

AST *	FileDataOutputAction::eval( AST * a )
	{
		AST *	where = ASTdown()->ASTright()->eval();
		AST *	x     = ASTdown();
		
		astream& s = *(where->getStream());

		while ( x )
			x = x->eval(where);

		s << std::endl;
		
		return 0;
	}

FileTextOutputAction::FileTextOutputAction( ANTLRTokenPtr p ):ActionAST(p){};

AST *	FileTextOutputAction::eval( AST * a )
	{
		AST *	where = ASTdown()->ASTright()->eval();
		AST *	x     = ASTdown();
		
		astream& s = *(where->getStream());
	
		while ( x )
			x = x->eval(where);

		s << std::endl;
		
		return 0;
	}

//-
FileDataInputAction::FileDataInputAction( ANTLRTokenPtr p ):ActionAST(p){};

AST *	FileDataInputAction::eval( AST * a )
	{
		AST *	where = ASTdown()->ASTright()->eval();
		AST *	x     = ASTdown();
		
		astream& s = *(where->getStream());

		while ( x )
			x = x->eval(where);

		////s << std::endl;
		
		return 0;
	}

FileTextInputAction::FileTextInputAction( ANTLRTokenPtr p ):ActionAST(p){};

AST *	FileTextInputAction::eval( AST * a )
	{
		AST *	where = ASTdown()->ASTright()->eval();
		AST *	x     = ASTdown();
		
		astream& s = *(where->getStream());
	
		while ( x )
			x = x->eval(where);

		////s << std::endl;
		
		return 0;
	}


//---

HexFormatAction::HexFormatAction( ANTLRTokenPtr p ):ActionAST(p){};
		
AST *		HexFormatAction::eval( AST * a ) { return this; };

astream&	HexFormatAction::operator>>( astream& s )
	{
		s >> std::hex;
		return s;
	}
	
astream&	HexFormatAction::operator<<( astream& s )
	{
		int	field_width = ASTdown()->eval()->getInteger();
			
		s.width( field_width );
		
		s << std::hex;
		return s;
	}
	

ExpFormatAction::ExpFormatAction( ANTLRTokenPtr p ):ActionAST(p){};
		
AST *		ExpFormatAction::eval( AST * a ) { return this; };

astream&	ExpFormatAction::operator>>( astream& s )
	{
		s >> std::dec;
		return s;
	}
	
astream&	ExpFormatAction::operator<<( astream& s )
	{
		int	field_width = ASTdown()->eval()->getInteger();
		int	precision   = ASTdown()->ASTright()->eval()->getInteger();
			
		s.width( field_width );
		s.precision( precision );
		
		s << std::dec;
		return s;
	}


BinFormatAction::BinFormatAction( ANTLRTokenPtr p ):ActionAST(p){};
		
AST *		BinFormatAction::eval( AST * a ) { return this; };

astream&	BinFormatAction::operator>>( astream& s )
	{
		return s;
	}
	
astream&	BinFormatAction::operator<<( astream& s )
	{
		int	field_width = ASTdown()->eval()->getInteger();
		
		s.width( field_width );
		
		s.bin();
		return s;
	}


OctFormatAction::OctFormatAction( ANTLRTokenPtr p ):ActionAST(p){};
		
AST *		OctFormatAction::eval( AST * a ) { return this; };

astream&	OctFormatAction::operator>>( astream& s )
	{
		s >> std::oct;
		return s;
	}
	
astream&	OctFormatAction::operator<<( astream& s )
	{
		int	field_width = ASTdown()->eval()->getInteger();
			
		s.width( field_width );
		
		s << std::oct;
		return s;
	}


NoFormatAction::NoFormatAction( ANTLRTokenPtr p ):ActionAST(p){};
		
AST *		NoFormatAction::eval( AST * a ) { return this; };

astream&	NoFormatAction::operator>>( astream& s )
	{
		return s;
	}
	
astream&	NoFormatAction::operator<<( astream& s )
	{
		return s;
	}


IntegerToDecimalAction::IntegerToDecimalAction( ANTLRTokenPtr p ):ActionAST(p){ ast = new DecimalNumber; };

AST *	IntegerToDecimalAction::eval( AST * a )
	{
		ast->setDecimal( 0, ASTdown()->getInteger() );
		return ast;
	}


AssignAction::AssignAction( ANTLRTokenPtr p ):ActionAST(p){};

AST *	AssignAction::eval( AST * a )
	{
		AST *	source = ASTdown()->eval();
		AST *	target = ASTdown()->ASTright()->eval();

		target->assign(source);
		return 0;
	}


EnableFileAction::EnableFileAction( ANTLRTokenPtr p, int mode, int create ):ActionAST(p),_mode(mode),_create(create){};

AST *	EnableFileAction::eval( AST * a )
	{
		AST *	file = ASTdown()->eval();
		AST *	name = ASTdown()->ASTright()->eval();
		astream *	new_file = 0;
		
		switch ( _mode )
		{
			case  1:	// read only
				new_file = new astream( name->str()->data(), std::ios::in /*| ios::nocreate */);
				break;
				
			case  2:	// write only
				if ( _create )
					new_file = new astream( name->str()->data(), std::ios::out );
				else
					new_file = new astream( name->str()->data(), std::ios::out /*| ios::nocreate */ );

				break;
		
			case  3:	// read write
				if ( _create )
					new_file = new astream( name->str()->data(), std::ios::out | std::ios::in );
				else
					new_file = new astream( name->str()->data(), std::ios::out | std::ios::in /*| ios::nocreate */ );

				break;
		}
		
		if ( !new_file )
			Error_Report( "Enable File Access Failed" );
		
		FileType tmp( new_file );
		file->assign( &tmp );
		file->setInteger( 0, 1 );		// Set the _enabled flag to true.
		return ASTright();
	}


DisableFileAction::DisableFileAction( ANTLRTokenPtr p ):ActionAST(p){};

AST *	DisableFileAction::eval( AST * a )
	{	
		AST *	file = ASTdown()->eval();
		
		file->getStream()->close();
		file->setInteger( 0, 0 );		// Set the _enabled flag to false.
		return ASTright();
	}


CreateFileAction::CreateFileAction( ANTLRTokenPtr p ):ActionAST(p){};

AST *	CreateFileAction::eval( AST * a )
	{
		const char *	filename = ASTdown()->eval()->str()->data();
		
		astream newfile( filename, std::ios::trunc );
		newfile.close();
		
		if ( ASTdown()->ASTright() != 0 )
			ASTdown()->ASTright()->eval()->setInteger( 0, std::ios::failbit );
		
		return ASTright();
	}


DeleteFileAction::DeleteFileAction( ANTLRTokenPtr p ):ActionAST(p){};

AST *	DeleteFileAction::eval( AST * a )
	{
		const char *	filename = ASTdown()->eval()->str()->data();
		
		if ( ASTdown()->ASTright() != 0 )
			ASTdown()->ASTright()->eval()->setInteger( 0, std::ios::failbit );

		return ASTright();
	}


GoToAction::GoToAction( ANTLRTokenPtr p, Scope * s ):ActionAST(p), _scope(s){};

AST *	GoToAction::eval( AST * a )
	{
		return ASTdown()->eval();
	}


LeaveAction::LeaveAction( ANTLRTokenPtr p, Scope * s ):ActionAST(p), _scope(s){};

AST *	LeaveAction::eval(AST * a)
	{
		AST *	x = ASTdown()->eval();
		return	x->ASTright()->eval();
	}


ResumeForAction::ResumeForAction( ANTLRTokenPtr p, Scope * s ):ActionAST(p), _scope(s){};

AST *	ResumeForAction::eval( AST * a )
	{
		
		AST *	x = ASTdown();		// StatementNumber
		x = x->eval();			// ForThenEntryAction
		
		//x = x->ASTright();	// TargetAction(#efs);
		//x = x->ASTright();	// First executable;
		return x;
	}


TargetAction::TargetAction( ANTLRTokenPtr p ):ActionAST(p), _target(0){};
TargetAction::TargetAction( AST * a ):ActionAST(0), _target(a){};
		
AST *	TargetAction::eval( AST * a )	{ return _target; };


ForThenEntryAction::ForThenEntryAction( ANTLRTokenPtr p ):ActionAST(p){};

AST *	ForThenEntryAction::eval(AST * a)
	{
		AST * LoopDecision;
			
		if ( a == this ){	// initialize and start

			ASTdown()->ASTright()->eval(); // initialize the iterator
			LoopDecision = ASTdown()->eval();
		}else{
			LoopDecision = ASTdown()->eval( ASTdown() );
		}
		
		if ( LoopDecision->getInteger() ){
			return ASTright()->ASTright();	// Loop
		}else{
			return ASTright();		// Exit
		}
	}


ArrayIteratorAction::ArrayIteratorAction( ANTLRTokenPtr p ):ActionAST(p){ _current = new IntegerNumber; };

AST *	ArrayIteratorAction::eval(AST * a)
	{
		AST *	result = 0;
		
		if ( a == 0 ){

			ASTdown()->eval();
			
			for ( int i = 1; i <= ASTdown()->length(); i++ ){
			
				_current->setInteger( 0, i);
				ASTdown()->eval( _current )->eval();	// They must be expressions
			}

			_current->setInteger( 0, 1 );
			
		}else if ( a == this ){			// we arbitrate
		
			if ( _current->getInteger() <= int(ASTdown()->length()) ){

				result = ASTdown()->eval( _current );
				_current->setInteger( 0, _current->getInteger() + 1 );
			}	
		}else{
			assert(0);
		}
		
		return result;
	}


ScalarIteratorAction::ScalarIteratorAction( ANTLRTokenPtr p ):ActionAST(p){};

AST *	ScalarIteratorAction::eval(AST * a)
	{
		if ( a == 0 ){
			_from = ASTdown()->eval();	// from value
			_current = _from->clone();
			
			if ( _by = ASTdown()->ASTright() ){
				_by = _by->eval();
			}else{
				_by = new IntegerNumber(1);
			}

			_first_time = 1;
			return _by;

		}else if ( a == this ){		// we arbitrate
		
			if( _first_time ){
				_first_time = 0;
			}else{
				_current = _current->add( _by );
			}

			return _current;
		}else{
			assert(0);
		}
	}


ForCompareAction::ForCompareAction( ANTLRTokenPtr p ):ActionAST(p){ _result = new BooleanType; };

AST *	ForCompareAction::eval( AST * a )
	{
		if ( a == 0 )
		{
			_result->setInteger( 0, 1 );		// Initial result = continue looping.
			_loopvar = ASTdown()->eval();		// get the location of loop variable
			
			if ( _to = ASTdown()->ASTright() ){
				_to = _to->eval(0);		// get the final value
			}
			_by = ASTright()->eval();		// and the incremtnt value
			
			if ( _by ){				// if there is an increment value find + or -
				if ( _by->getDecimal(0) >= 0.0 ){
					_dir =  1;
				}else{
					_dir = -1;
				}
			}else{
				_dir = 1;			// if not specified assume +
			}
		}

		AST * newval = ASTright()->eval( ASTright() );	// get the iterated value
		if ( newval ){
			_loopvar->assign( newval );		// iterator returned something
		
			if ( _to )
			{					// note that _to==NULL -> ArrayIterator doing the job
				if ( _dir == 1 ){

					_result->setInteger( 0, _loopvar->compare( _to ) <= 0 );
				}else if ( _dir == -1 ){
					_result->setInteger( 0, _loopvar->compare( _to ) >= 0 );

				}else{
					_result->setInteger( 0, 0 );	// exit.
				}
			}			
		}else{
			_result->setInteger( 0, 0 );		// exit.
		}

		return _result;					
	}

VarInitAction::VarInitAction(ASTList *Label_List,InitList *init_List)
		:ActionAST(0),m_label_list(Label_List),m_init_list(init_List)
	{
		m_init_list_iterator=new InitListIterator(*init_List);
	}

	
AST *	VarInitAction::eval( AST * a )
	{
		AST * _data=0;
		
		if ( a==0 ){
			reset();					// reset the "variables to initialize" iterator , and clear the stack
			
			//ASTListIterator it( m_label_list->begin() );		// now iterate over ALL the variables to be initialized.
			//while( ++it ){					// we call the variable and pass it US.
			for(auto it = m_label_list->begin() ; it!= m_label_list->end(); ++it ){					// we call the variable and pass it US.
				(*it)->init( this );			// and it will call us with non-zero
			}
			
			
			if(_data=eval(this)){	// should be no more data
				Error_Report( "Too many Variable Initializers", _data );
			} else {
				return ASTright();
			}

		}else{
			while(1){		// variable is calling us
				if ( m_stack.empty() ){
////					if ( ++(*m_init_list_iterator) ){ // get the next list entry
////						push( m_init_list_iterator->key() );
////						top()->reset();
////					} else {
////						_data=0 ; break;					// no more
////					}
// 					if (++(*m_init_list_iterator) != m_init_list->end())
					if (++(*m_init_list_iterator) != m_init_list->end()) {
					    push(*(*m_init_list_iterator));
					    top()->reset();
					} else {
					    _data = nullptr;
					    break;
					}

				
				} else if (top()->_counter < (top()->_rep)->getInteger() ) {	
					if ( (top())->_list ){					// we have a list
//						if ( ++(*(top()->_iterator)) ){			// more "_rep" _data or _list in the list.
//						if ( ++(*(top()->_iterator) != top()->_list->end() ){			// more "_rep" _data or _list in the list.
						if ( ++(*top()->_iterator) != top()->_list->end()) {
//							push( (top()->_iterator->key()) );// defer decision
							push( **(top()->_iterator) );// defer decision
							top()->reset();
						} else {
							top()->_counter++;			// this one is done . defer decision
							top()->_iterator->reset();
						}
					} else {
						top()->_counter++;			// remember? we zeroed this
						_data = top()->_data;break;
					}
				} else {					// no more "_rep"eated data here.
						pop();
				}
			}
			return _data;
		}
		
		return 0;
	}

InitData *
VarInitAction::top()
	{
		return m_stack.top();
	}

InitData *
VarInitAction::push(InitData * x)
	{
		m_stack.push(x);return x;
}

InitData *
VarInitAction::pop()
	{
		if (m_stack.empty()) return nullptr;
		auto x = m_stack.top();
		m_stack.pop();
		return x;
	}
	
void
VarInitAction::reset()
	{
		m_init_list_iterator->reset();
		while (!m_stack.empty()) m_stack.pop();
}

HiAction::HiAction(AtlasParser * p):ActionAST(0),m_parser(p){}
AST *	HiAction::eval( AST * a )
		{
			AST * result=ASTdown()->eval();
			if(!(result->getInteger()==1)){
				m_parser->setHI();
			}
			return result;
		}

astream&	HiAction::operator<<( astream &s )
	{
		s << ASTdown();
		if(m_parser->getHI()){
			s <<  "(FAIL) " ;
		}
		return s;
	}


LoAction::LoAction(AtlasParser * p):ActionAST(0),m_parser(p){}
AST *	LoAction::eval( AST * a )
		{
			AST * result=ASTdown()->eval();
			if(!(result->getInteger()==1)){
				m_parser->setLO();
			}
			return result;
		}

astream&	LoAction::operator<<( astream &s )
	{
		s << ASTdown();
		if(m_parser->getLO()){
			s <<  "(FAIL) " ;
		}
		return s;
	}


GoAction::GoAction(AtlasParser * p):ActionAST(0),m_parser(p){}
AST *	GoAction::eval( AST * a )
		{
			AST * result=ASTdown()->eval();
			if(!(result->getInteger()==1)){
				m_parser->setGO();
			}
			return result;
		}

astream&	GoAction::operator<<( astream &s )
	{
		s << ASTdown();
		if(m_parser->getGO()){
			s <<  "GO " ;
		}
		return s;
	}


NoGoAction::NoGoAction(AtlasParser * p):ActionAST(0),m_parser(p){}
AST *	NoGoAction::eval( AST * a )
		{
			AST * result=ASTdown()->eval();
			if(!(result->getInteger()==1)){
				m_parser->setNOGO();
			}
			return result;
		}

astream&	NoGoAction::operator<<( astream &s )
	{
		s << ASTdown();
		if(m_parser->getNOGO()){
			s <<  "(NOGO) " ;
		} else {
			s <<  "(GO) " ;
		}
		return s;
	}


CompareAction::CompareAction(AtlasParser * p):ActionAST(0),m_parser(p){}
AST *	CompareAction::eval( AST * a )
		{
			m_parser->setGO();	
			return ActionAST::eval(a);
		}
Scope	*
CompareAction::getScope	() const
	{
		return m_parser->getScope();
	}

astream&	CompareAction::operator<<( astream& s )
	{
		AST * c=ASTdown();
		AST * l=ASTdown()->ASTright();

		s << l;
		s << "=";
		s << l->eval();
		s << ", ";
		s << c;
		
		return s;
	}
