
#include	"BasicTypeAST.h"
#include	"LabelAST.h"

Long
BitStrCompOp( const RWBitVec *x, const RWBitVec *y )
	{
		// The RWBitVec semantics are the same as ATLAS String Of Bit
		// So we extend smaller to the larger size and return it.
		
		const RWBitVec *xx = x;
		const RWBitVec *yy = y;
		RWBitVec * z = 0;
		
		if ( x->length() > y->length() ){
		
			z = new RWBitVec( *y );
			z->resize( x->length() );
			yy = z;	
			
		}else if ( x->length() < y->length() ){
		
			z = new RWBitVec( *x );
			z->resize( y->length() );
			xx = z;
		}
		
		// Here we use the builtin methods. 
		// If these cause a performance hit, they should
		// be converted to loop-bitwise compare. Sinan
		
		int x1 = (((*xx)^(*yy))&(*xx)).firstTrue();
		int y1 = (((*xx)^(*yy))&(*yy)).firstTrue();
		
		delete z;
		
		if ( x1 <  y1 ) return -1;
		if ( x1 == y1 ) return  0;
		if ( x1  > y1 ) return  1;
	}

class Scope; // forward declaration

//------------------------------------------------------------------------------------------//
BasicTypeAST::BasicTypeAST( ANTLRTokenPtr p, TheType s)
	:_storage( s )
	,AST( p )
	{
	}

BasicTypeAST::~BasicTypeAST(){}
	
AST *	BasicTypeAST::eval	( AST * a )		{ return this; }
AST *	BasicTypeAST::assign	( AST * a )		{ return this; }
TheType	BasicTypeAST::getType	( AST * a ) const	{ return _storage; }
AST *	BasicTypeAST::Accept	( ASTVisitor & v )	{ return v.VisitBasicTypeAST( this ); }
AST *	BasicTypeAST::data	( AST *  a )		{ return this;; };
AST *
BasicTypeAST::check	( AST * a )
	{
		if(verify(this,a)){
			return this;
		} else if(verify(a,this)){
			return this;
		} else {
			return 0;
		}
	}

//------------------------------------------------------------------------------------------//
DecimalNumber::DecimalNumber( ANTLRTokenPtr p ,int sign):BasicTypeAST( p, DecimalNumberValue )
	{
		_data = 0.0;
		if ( p == 0 ){
			setUninitialized(1);
		}else{
			char * s, *e;
			_data = strtod( s=p->getText(), &e ) * sign ;
			if(s==e){
				setUninitialized(1);	// a variable;
			} else {
				//setWriteProtect(1);	// a constant;
			}
		}
	}
		
DecimalNumber::DecimalNumber(double data):BasicTypeAST(0, DecimalNumberValue ),_data(data)
	{
	}

DecimalNumber::DecimalNumber(AST *a)
	:BasicTypeAST(0, DecimalNumberValue )
	,_data(a->getDecimal())
	{
	}

astream&
DecimalNumber::operator>>( astream& s )
	{
		s >> _data;
		return s;
	}

astream&
DecimalNumber::operator<<( astream& s )
	{
		s << _data;
		return s;
	}

AST *
DecimalNumber::eval( AST * a )
	{
		return this;
	}

AST *
DecimalNumber::init( AST * a )
	{
		if ( a ){
			AST * x = a->eval( a );
			if ( x ){
				if ( verify(this,x) ){
					assign( x );
				}else{
					Error_Report( "Data is not compatible with DECIMAL", a );
				}
			}
		}
		return this;
	}


AST *
DecimalNumber::assign( AST * a )
	{
		setDecimal(0, a->getDecimal(0));
		return this;
	};

AST *
DecimalNumber::add( AST * a )
	{
		setDecimal(0,_data + a->getDecimal(0));
		return this;
	};

AST *
DecimalNumber::clone( Scope * s ) const
	{
		DecimalNumber * t = new DecimalNumber(_data);
		return t;
	};

Long
DecimalNumber::getInteger(int indx) const
	{
		readEvent();
		return _data;
	}

void
DecimalNumber::setInteger(int indx,Long value)
	{
		writeEvent();
		_data = value;
	}

double
DecimalNumber::getDecimal(int indx) const
	{
		readEvent();
		return _data;
	}
	
void
DecimalNumber::setDecimal(int indx,double value)
	{
		writeEvent();
		_data = value;
	}

Long
DecimalNumber::compare( AST * o ) const
	{
		double other = o->getDecimal(0);
		double me=getDecimal();
		if ( me < other ){
			return -1;
		}else{
			return ( me > other );
		}
	};

//------------------------------------------------------------------------------------------//
IntegerNumber::IntegerNumber(ANTLRTokenPtr p ,int sign):BasicTypeAST(p, IntegerNumberValue )
	{
		_data = 0;
		if ( p == 0 ){
			setUninitialized(1);
		}else{
			char * s, *e;
			_data = strtoll( s=p->getText(), &e, 10 ) * sign ;
			if(s==e){
				setUninitialized(1);	// a variable;
			} else {
				//setWriteProtect(1);	// a constant;
			}
		}
	};

IntegerNumber::IntegerNumber(Long data):BasicTypeAST(0, IntegerNumberValue ),_data(data)
	{
	}

AST *
IntegerNumber::clone( Scope * s ) const
	{
		IntegerNumber *	t = new IntegerNumber(_data);
		return t;
	};

astream&
IntegerNumber::operator>>( astream& s )
	{
		s >> _data;
		return s;
	}
	
astream&
IntegerNumber::operator<<( astream& s )
	{
		s << _data;
		return s;
	}
			
AST *
IntegerNumber::eval( AST * a )
	{
		return this;
	}

AST * 
IntegerNumber::init( AST * a )
	{
		if ( a ){
			AST * x = a->eval(a);
			
			if ( x ){
				x = x->eval();
				if ( verify(this,x) ){
					assign( x );
				}else{
					Error_Report( "Data is not compatible with INTEGER", x );
				}
			}
		}
		return this;
	}
	
AST *
IntegerNumber::assign( AST * a )
	{
		setInteger(0, a->getInteger());
		return this;
	}

AST *
IntegerNumber::add( AST * a )
	{
		setInteger(0,_data + a->getInteger());
		return this;
	}

Long
IntegerNumber::getInteger( int indx ) const
	{
		readEvent();
		return _data;
	}

void
IntegerNumber::setInteger( int indx, Long value )
	{
		writeEvent();
		_data = value;
	}

double
IntegerNumber::getDecimal( int indx ) const
	{
		readEvent();
		return _data;
	}
void
IntegerNumber::setDecimal( int indx, double value )
	{
		writeEvent();
		_data = value;
	}

Long
IntegerNumber::compare( AST * o ) const
	{
		Long other 	= o->getDecimal(0);
		Long me		= getInteger(0);
		if ( me < other ){
			return -1;
		}else{
			return ( me > other );
		}
	}



//------------------------------------------------------------------------------------------//
EnumerationType::EnumerationType(ANTLRTokenPtr p, Long pos)
		:BasicTypeAST(p, EnumerationTypeValue),_pos(pos)
	{
		if( p!=0 ) {
			_data = new RWCString( p->getText() );
			//setWriteProtect(1);
		} else {
			setUninitialized(1);
			_data = new RWCString;
		}
	}

astream&
EnumerationType::operator>>( astream& s )
	{
		s >> *_data;
		return s;
	}
	
astream&
EnumerationType::operator<<( astream& s )
	{
		s << *_data;
		return s;
	}
			
AST *
EnumerationType::eval( AST * a )
	{
		return this;
	}

AST *
EnumerationType::init( AST * a )
	{
		if ( a )
		{
			AST * x = a->eval(a);
			
			if ( x ){
				x = x->eval();
				if ( verify(this,x) ){
					assign( x );
				}else{
					Error_Report( "Data is not compatible with Enumeration ", x );
				}
			}
		}
		return this;
	}
	
AST *
EnumerationType::clone( Scope * s ) const
	{
		AST* x;
		
		x = new EnumerationType( getToken(), _pos );
		x->setDown( ASTdown() );
		return x;
	}

AST *
EnumerationType::add( AST * a )
	{
		EnumerationsType * x = (EnumerationsType *)ASTdown();
		Long y     = ( a? a->getInteger() : 1 );
		Long last  = ( x? x->_data->entries()-1 : 0 );
		Long first = 0;

		if ( y > 0  &&  _pos < last ){
		
			return assign( succ() );
			
		}else if ( y < 0  &&  _pos > first ){
		
			return assign( pred() );
		}
		
		return 0;
	}

AST *
EnumerationType::succ( AST * a )
	{
		EnumerationsType * x = (EnumerationsType *)ASTdown();
		Long pos = getInteger();

		if ( x )
		{
			if ( pos < (x->_data->entries() - 1) ){
				return x->_data->at( pos+1 );
			}else{
				Error_Report( "Attempting Successor of last element ", this );
			}
		}
		return 0;
	}
	
AST *
EnumerationType::pred( AST * a )
	{
		EnumerationsType * x = (EnumerationsType *)ASTdown();
		Long pos = getInteger();

		if ( x )
		{
			if ( pos > 0 ){
				return x->_data->at( pos-1 );
			}else{
				Error_Report( "Attempting Predecessor of 1st element ", this );
			}
		}
		return 0;
	}

Long
EnumerationType::getInteger(int indx) const
	{
		readEvent();
		return _pos;
	}

void
EnumerationType::setInteger( int indx, Long value )
	{
		writeEvent();
		_pos = value;
	}

Long
EnumerationType::compare( AST * o ) const
	{
		Long other = o->getInteger();
		
		if ( _pos <  other ){
			return -1;
		}else{
			return ( _pos > other );
		};
	}

AST *
EnumerationType::assign( AST * a )
	{		
		if ( !check( a ) ){
		
			Error_Report( "Not assignment compatible ", a );
		}else{
		
			*_data = *(((EnumerationType *)a)->_data);
			setInteger(0, a->getInteger());
		}
		return this;
	}

//------------------------------------------------------------------------------------------//	
EnumerationsType::EnumerationsType(ASTList * list)
				:BasicTypeAST(0,EnumerationsTypeValue), _data(list)
	{
	}

astream&
EnumerationsType::operator>>( astream& s )
	{
		ASTListIterator iterate(*_data);
		
		for( int i=0; iterate(); ++i )
			s >> (*_data)[i];
			
		return s;
	}

astream&
EnumerationsType::operator<<( astream& s )
	{
		ASTListIterator iterate(*_data);
		
		for( int i=0; iterate(); ++i )
			s << (*_data)[i];
			
		return s;
	}
			
AST *
EnumerationsType::clone( Scope * s ) const
	{
		AST *	x = new EnumerationsType(_data);
		return x;
	}
	
AST *
EnumerationsType::eval( AST * a )
	{
		return this;
	}

AST *
EnumerationsType::init( AST * a )
	{
		ASTListIterator it(*_data);
		
		if ( a ){
			for ( int i=0; ++it; ++i ){
				(*_data)[i]->init(a);
			}
		}
		return this;
	}
	


AST *
EnumerationsType::check( AST * a )
	{		
		if ( a ){
			EnumerationsType* x = (EnumerationsType*)a->check();
		
			if ( x->_data == _data ){
				return this;
			}else{
				Error_Report( "Enumeration Type Mismatch ", a );
				return 0;
			}
		}else{
			return this;
		}
	}
	
AST *
EnumerationsType::assign( AST * a )
	{
		*_data = *((EnumerationsType *)a)->_data;
		return this;
	}
	
Long
EnumerationsType::compare( AST * o ) const
	{
		cerr << " EnumerationsType::compare(AST*) [Don't call me yet !!] " << endl;
		assert(0);
		return 0;
	}


//------------------------------------------------------------------------------------------//
ConnectionType::ConnectionType(ANTLRTokenPtr p)
	:BasicTypeAST(p,ConnectionTypeValue )
	{
		if(p!=0){
			_data=new RWCString(p->getText());
			//setWriteProtect(1);
		} else {
			_data=new RWCString;
			setUninitialized(1);
		}
		
	}

AST *
ConnectionType::clone( Scope * s ) const
	{
		AST *	x = new ConnectionType(getToken());
		
		x->setDown( ASTdown());
		return x;
	}
	
AST *
ConnectionType::eval( AST * a )
	{
		return this;
	}

AST *
ConnectionType::init( AST * a )
	{
		if ( a ){

			AST *	x = a->eval(a);
			if ( x ){
				x = x->eval();
				if ( verify(this,x) ){
					assign(x);
				}else{
					Error_Report( "Data is not compatible with Connection ", a );
				}
			}
		}
		return this;
	}


Long
ConnectionType::compare( AST * o ) const
	{
		StringOfCharType temp(_data);
		return - (o->compare(&temp));
	}

astream&
ConnectionType::operator>>( astream& s )
	{
		s >> *_data;
		return s;
	}
	
astream&
ConnectionType::operator<<( astream& s )
	{
		s << *_data;
		return s;
	}

AST *
ConnectionType::assign( AST * a )
	{
		writeEvent();
		if ( ASTdown() ){
		
			// we got a Connections down there, so check
			if ( ! check( a ) ){
			
				Error_Report( "Not assignment compatible ", a );
			}else{
				*_data = *(((ConnectionType *)a)->_data);
			}
		}else{
			// No down node, anything is fair game in CONNECTION Assignments
			*_data = *(((ConnectionType *)a)->_data);
		}		
		return this;
	}


RWCString
ConnectionType::getName() const
	{
		readEvent();
		return *_data;
	}



//------------------------------------------------------------------------------------------//
ConnectionsType::ConnectionsType(SymbolDictionary * list)
	:BasicTypeAST(0, ConnectionsTypeValue ),_data(list)
	{
	}

	
AST *
ConnectionsType ::clone( Scope * s ) const
	{
		AST *	x = new ConnectionsType(_data);
		return	x;
	};

AST *
ConnectionsType::eval( AST * a )
	{
		return this;
	}

Long	ConnectionsType::compare( AST * o ) const
	{
		cerr<< " ConnectionsType::compare(AST*) [Don't call me yet !!] " << endl;
		assert(0);
		return 0;
	}

astream&
ConnectionsType::operator>>( astream& s )
	{
		AST *			x;
		SymbolDictionaryIterator	it( *_data );
		RWCString			name;
		
		while ( it() ){
		
			x = (AST *)it.value();
			s >> name;
			x->setName( name );
		}
		return s;
	}
	
astream&
ConnectionsType::operator<<( astream& s )
	{
		AST *			x;
		SymbolDictionaryIterator	it( *_data );
		
		while ( it() ){
		
			x = (AST *)it.value();
			s << x->getName();
		}
			
		return s;
	}

AST *
ConnectionsType::init( AST * a )
	{
		AST * x;
		SymbolDictionaryIterator it(*_data);

		if ( a ){
			for( int i=0; ++it ; ++i ){
				x=(AST *)it.value();
				x->init(a);
			}
		}
		return this;
	}

AST *
ConnectionsType::check( AST * a )
	{
		if ( a ){
			AST * x=0;
			if (_data->findValue(a->getName() ,x )){
				return this;	// note that we only flag if it is OK or not...
			}else{
				Error_Report( "Connection is not Assignment compatible ", a );
				return 0;
			}
		}else{
			return this;
		}
	}
	
AST *
ConnectionsType::assign( AST * a )
	{
		*_data=*((ConnectionsType *)a)->_data;
		return this;
	};

//------------------------------------------------------------------------------------------//
TerminalType::TerminalType(ANTLRTokenPtr p):BasicTypeAST(p, TerminalTypeValue )
	{
	}

AST *
TerminalType::clone( Scope * s ) const
	{
		AST *	t = new TerminalType;
		
		((TerminalType *)t)->_data = new RWCString(*_data);
		return t;
	}

astream&
TerminalType::operator>>( astream& s )
	{
		s >> *_data;
		return s;
	}

astream&
TerminalType::operator<<( astream& s )
	{
		s << *_data;
		return s;
	}

AST *
TerminalType::eval( AST * a ){return this;};

Long
TerminalType::compare( AST * o ) const
	{
		return this->_data->compareTo( *(RWCString *)((o->eval())->data()) );
	}

AST *
TerminalType::assign( AST * a )
	{
		*_data = *((TerminalType *)a)->_data;
		return this;
	}

//------------------------------------------------------------------------------------------//
CharType::CharType(ANTLRTokenPtr p):BasicTypeAST(p, CharTypeValue )
	{
		if(p!=0){
			const char * c=p->getText();
			if(strlen(c)>=2 && c[1]=='\'' ){
				RWCString s = p->getText();
				s.remove(0,2);			// remove leading "C'"
				s.remove(s.length()-1,1);	// remove trailing "'"
				_data = new RWCString( s );
				//setWriteProtect(1);
			} else {
				_data=new RWCString;
				setUninitialized(1);
			}
		} else {
			_data=new RWCString;
			setUninitialized(1);
		}
	}

CharType::CharType(const RWCString & c):BasicTypeAST(0, CharTypeValue )
	{
		_data = new RWCString(c);
		//setWriteProtect(1);
	}

AST *
CharType::clone( Scope * s ) const
	{
		AST *	t = new CharType( *_data );
		return	t;
	}

AST *
CharType::assign( AST * a )
	{
		*_data = *((CharType *)a)->_data;
		return this;
	}

astream&
CharType::operator>>( astream& s )
	{
		s >> *_data;
		return s;
	}

astream&
CharType::operator<<( astream& s )
	{
		s << *_data;
		return s;
	}
	
AST *
CharType::eval( AST * a )
	{
		return this;
	}

AST *
CharType::add( AST * a )
	{
		writeEvent();
		return assign( succ() );
	}

AST *
CharType::succ( AST * a )
	{
		readEvent();
		int	 my_data   = (*_data)[0] + 1;
		char	new_data[] = { my_data, 0 };

		return new CharType( RWCString(new_data) );
	}
	
AST *
CharType::pred( AST * a )
	{
		readEvent();
		int	 my_data   = (*_data)[0] - 1;
		char	new_data[] = { my_data, 0 };

		return new CharType( RWCString(new_data) );
	}

Long
CharType::getInteger(int indx) const
	{
		readEvent();
		Long	result = (*_data)[0];
		
		return result;
	}


Long
CharType::compare( AST * o ) const
	{
		CharType * other = (CharType*)o;
		
		if ( str()->operator[](0) < o->str()->operator[](0) ){
			return -1;
		}else{
			return ( str()->operator[](0) > o->str()->operator[](0) );
		}
	}

const RWCString *
CharType::str() const
	{
		return _data;
	}



AST *
CharType::init( AST * a )
	{
		if ( a ){
			AST * x = a->eval( a );
			if ( x ){
				if ( verify(this,x) ){
					assign( x );
				}else{
					Error_Report( "Data is not compatible with BOOLEAN", a );
				}
			}
		}
		return this;
	}
	
//------------------------------------------------------------------------------------------//
CharClassType::CharClassType(ANTLRTokenPtr p):BasicTypeAST(p, CharClassTypeValue )
	{
	}
	
	
AST *
CharClassType ::clone( Scope * s ) const
	{
		AST *	t = new CharClassType;
		return	t;
	}

AST *
CharClassType::eval( AST * a )
	{
		return this;
	}
	
Long
CharClassType::compare( AST * o ) const
	{
		cerr<< " CharClassType::compare(AST*) [Don't call me yet !!] " << endl;
		assert(0);return 0;
	}

void convertSpecial(RWCString & s){
				s(RWCRegexp("\\NUL\\")	)= RWCString(char(0x00));
				s(RWCRegexp("\\SOH\\")	)= RWCString(char(0x01));
				s(RWCRegexp("\\STX\\")	)= RWCString(char(0x02));
				s(RWCRegexp("\\ETX\\")	)= RWCString(char(0x03));
				s(RWCRegexp("\\EOT\\")	)= RWCString(char(0x04));
				s(RWCRegexp("\\ENQ\\")	)= RWCString(char(0x05));
				s(RWCRegexp("\\ACQ\\")	)= RWCString(char(0x06));
				s(RWCRegexp("\\BEL\\")	)= RWCString(char(0x07));
				s(RWCRegexp("\\BS\\")	)= RWCString(char(0x08));
				s(RWCRegexp("\\HT\\")	)= RWCString(char(0x09));
				s(RWCRegexp("\\LF\\")	)= RWCString(char(0x0A));
				s(RWCRegexp("\\VT\\")	)= RWCString(char(0x0B));
				s(RWCRegexp("\\FF\\")	)= RWCString(char(0x0C));
				s(RWCRegexp("\\CR\\")	)= RWCString(char(0x0D));
				s(RWCRegexp("\\SO\\")	)= RWCString(char(0x0E));
				s(RWCRegexp("\\SI\\")	)= RWCString(char(0x0F));
				s(RWCRegexp("\\DLE\\")	)= RWCString(char(0x10));
				s(RWCRegexp("\\DC1\\")	)= RWCString(char(0x11));
				s(RWCRegexp("\\DC2\\")	)= RWCString(char(0x12));
				s(RWCRegexp("\\DC3\\")	)= RWCString(char(0x13));
				s(RWCRegexp("\\DC4\\")	)= RWCString(char(0x14));
				s(RWCRegexp("\\NAK\\")	)= RWCString(char(0x15));
				s(RWCRegexp("\\SYN\\")	)= RWCString(char(0x16));
				s(RWCRegexp("\\ETB\\")	)= RWCString(char(0x17));
				s(RWCRegexp("\\CAN\\")	)= RWCString(char(0x18));
				s(RWCRegexp("\\EM\\")	)= RWCString(char(0x19));
				s(RWCRegexp("\\SUB\\")	)= RWCString(char(0x1A));
				s(RWCRegexp("\\ESC\\")	)= RWCString(char(0x1B));
				s(RWCRegexp("\\FS\\")	)= RWCString(char(0x1C));
				s(RWCRegexp("\\GS\\")	)= RWCString(char(0x1D));
				s(RWCRegexp("\\RS\\")	)= RWCString(char(0x1E));
				s(RWCRegexp("\\US\\")	)= RWCString(char(0x1F));
				s(RWCRegexp("\\SP\\")	)= RWCString(char(0x20));
				s(RWCRegexp("\\DEL\\")	)= RWCString(char(0x7F));
				//return s;
	}


//------------------------------------------------------------------------------------------//
StringOfCharType::StringOfCharType(ANTLRTokenPtr p,int maxlen):BasicTypeAST(p, StringOfCharTypeValue )
	{
		RWCString  s;
		
		_dyn_length = 0;
		
		if ( maxlen ){
			_max_length = maxlen;
		}else{
			if ( p != 0 ){
				s = p->getText();
				s.remove( 0,2 );		// remove leading "C'"
				s.remove( s.length()-1, 1 );	// remove trailing "'"
				// now filter the rest according to
				//	\NUL\	->	x00
				//	\SOH\	->	x01
				//	\STX\	->	x02
				//	\ETX\	->	x03
				//	\EOT\	->	x04
				//	\ENQ\	->	x05
				//	\ACQ\	->	x06
				//	\BEL\	->	x07
				//	\BS\	->	x08
				//	\HT\	->	x09
				//	\LF\	->	x0A
				//	\VT\	->	x0B
				//	\FF\	->	x0C
				//	\CR\	->	x0D
				//	\SO\	->	x0E
				//	\SI\	->	x0F
				//	\DLE\	->	x10
				//	\DC1\	->	x11
				//	\DC2\	->	x12
				//	\DC3\	->	x13
				//	\DC4\	->	x14
				//	\NAK\	->	x15
				//	\SYN\	->	x16
				//	\ETB\	->	x17
				//	\CAN\	->	x18
				//	\EM\	->	x19
				//	\SUB\	->	x1A
				//	\ESC\	->	x1B
				//	\FS\	->	x1C
				//	\GS\	->	x1D
				//	\RS\	->	x1E
				//	\US\	->	x1F
				//	\SP\	->	x20
				//	\DEL\	->	x7F
				convertSpecial(s);
				_dyn_length = s.length();
				_max_length = _dyn_length;
			}else{
				_max_length = 0;
			}
		}
		_data = new ArrayObject( _max_length );
		_assign( &s );
		_str = new RWCString;
	};

StringOfCharType::StringOfCharType(const RWCString * str,int maxlen):BasicTypeAST(0, StringOfCharTypeValue )
	{
		_dyn_length = str->length();
		if(maxlen){
			_max_length = maxlen;
		} else {
			_max_length = _dyn_length;
			//setWriteProtect(1);
		}
		
		_data = new ArrayObject( _max_length );
		_assign( str );
		_str = new RWCString;
	}

StringOfCharType::~StringOfCharType()
	{
		delete _data;
		delete _str;
	}

astream&
StringOfCharType::operator>>( astream& s )
	{
		RWCString x;
	
		s >> x;
		StringOfCharType y( &x );
		assign( &y );

		return s;
	}

astream&
StringOfCharType::operator<<( astream& s )
	{
		s << str()->data();
		return s;
	}
	

AST *
StringOfCharType::clone( Scope * s ) const
	{
		const RWCString *s1 = str();
		AST		*t  = new StringOfCharType( s1, _max_length );
		
		return t;
	}
	
AST *
StringOfCharType::eval( AST * a )
	{
		if(a==0){
			return this;
		} else if(a==this){ // Someone wants to know type of _data[]
			return (*_data)[0]; // any one will do. We know 0 is always there
		} else {
			return (*_data)[a->getInteger()-1];
		}
	}

AST *
StringOfCharType::init( AST * a )
	{
		if ( a ){
			AST * x = a->eval( a );
			if ( x ){
				if ( verify(this,x) ){
					assign( x );
				}else{
					Error_Report( "Data is not compatible with STRING() OF CHAR", a );
				}
			}
		}
		return this;
	}


AST *
StringOfCharType::assign( AST * a )
	{
		writeEvent();

		const RWCString & source_str = *a->str();
		
		if ( a->length() > _max_length ){
			Error_Report( "RHS String is too long", a );
			_dyn_length = _max_length;
		}else
			_dyn_length = a->length();
		
		for( int i=0; i < _max_length; i++ ){
			
			if ( i <  _dyn_length ){
				*(((CharType *)(*_data)[i]))->_data = source_str[i];
			}else{
				*(((CharType *)(*_data)[i]))->_data = "";
			}
		}
		
		return this;
	}

	
Long
StringOfCharType::compare( AST * o ) const
	{
		return str()->compareTo( *(o->eval()->str()) );
	}

Long
StringOfCharType::count( AST * o ) const{

	const RWCString *	ystr   = str();
	const RWCString *	xchar  = o->str();
	Long			cnt = 0;

	for ( int pos = 0; pos < ystr->length(); ++pos ){

		if ( ystr->operator[](pos) == xchar->operator[](0) ){

			++cnt;
		}
	}

	return cnt;
}

Long
StringOfCharType::index( AST * o ) const
	{
		const RWCString *	xstr = str();
		const RWCString *	ystr = o->str();
		Long			indx = ystr->index( *xstr );
		
		if ( indx != RW_NPOS ){
			++indx;
			return indx;
		}else{
			return 0;
		}
	}
		
Long
StringOfCharType::length(int indx) const
	{
		if ( indx )	return _max_length;
		else		return _dyn_length;
	}


const RWCString *
StringOfCharType::str() const
	{
		readEvent();
		*_str = "";
		
		for ( int i=0; i < _dyn_length; i++ ){
			*_str += *((*_data)[i]->str());
		}
		return _str;
	}


void
StringOfCharType::_assign(const RWCString *s)
	{
		// private member, no writeEvent() here...
		if ( s->length() > _max_length ){
			assert(0);
		}

		for( int i=0; i<_max_length; i++ ){
		
			if ( i<_dyn_length ){
				(*_data)[i] = new CharType((*s)[i]);
			}else{
				(*_data)[i] = new CharType("\0");
			}
		}
	}
	
ArrayObject *
StringOfCharType::array(AST * len)
	{
		if(len){
			_data->reshape(len->getInteger());
			_dyn_length=_max_length=len->getInteger();
		}
		return _data;
	}

//------------------------------------------------------------------------------------------//
DigClassType::DigClassType(ANTLRTokenPtr p):BasicTypeAST(p, DigClassTypeValue )
	{
	}
	
AST *
DigClassType ::clone( Scope * s ) const
	{
		AST *	t = new DigClassType;
		return	t;
	}
	
AST *
DigClassType::eval( AST * a )
	{
		return this;
	}
	
		
Long
DigClassType::compare( AST * o ) const
	{
		cerr<< " DigClassType::compare(AST*) [Don't call me yet !!] " << endl;
		assert(0);
		return 0;
	}

astream&
DigClassType::operator>>( astream& s )
	{
		s >> *_data;
		return s;
	}

astream&
DigClassType::operator<<( astream& s )
	{
		s << *_data;
		return s;
	}

//------------------------------------------------------------------------------------------//
BooleanType::BooleanType(ANTLRTokenPtr p):BasicTypeAST(p, BooleanTypeValue ),m_data(0),m_indicator(0)
	{
	}

BooleanType::BooleanType(int data):BasicTypeAST(0, BooleanTypeValue ),m_data(data),m_indicator(0)
	{
		//setWriteProtect(1);
	}
	
AST *
BooleanType::assign( AST * a )
	{
		setInteger(0,a->getInteger(0));
		return this;
	}

AST *
BooleanType::insert( AST * a )
	{
		if(!m_indicator){
			m_indicator=a;
			return this;
		} else {
			return 0;
		}
	}

AST *
BooleanType::remove( AST * a )
	{
		if(!m_indicator){
			return 0;
		} else if(m_indicator==a){
			m_indicator=0;
			return this;
		} else {
			m_indicator=0;
			return 0;
		}
	}

AST *
BooleanType ::clone( Scope * s ) const
	{
		AST *	t = new BooleanType( m_data );
		return	t;
	}

astream&
BooleanType::operator>>( astream& s )
	{
		RWCString	temp;
		
		s >> temp;
		
		if ( !strcmp( temp, "TRUE" ) )

			m_data = 1;
		else
			m_data = 0;

		return s;
	}

astream&
BooleanType::operator<<( astream& s )
	{
		if ( m_data )
			s << "TRUE";
		else
			s << "FALSE";
			
		return s;
	}

AST *
BooleanType::eval( AST * a )
	{
		return this;
	}
	
Long
BooleanType::compare( AST * o ) const
	{
		return m_data -(o->getInteger());
	}

Long
BooleanType::getInteger(int indx) const
	{
		readEvent();
		return m_data;
	}


void
BooleanType::setInteger( int indx, Long value )
	{
		writeEvent();
		if(m_indicator&&((!m_data)&&(value))){
			m_indicator->eval(m_indicator);
		}
		m_data = value;
	}


double
BooleanType::getDecimal(int indx) const
	{
		readEvent();
		return m_data;
	}
	
void
BooleanType::setDecimal( int indx, double value )
	{
		writeEvent();
		setInteger(value);
	}


AST *
BooleanType::init( AST * a )
	{
		if ( a ){
			AST * x = a->eval( a );
			if ( x ){
				if ( verify(this,x) ){
					assign( x );
				}else{
					Error_Report( "Data is not compatible with BOOLEAN", a );
				}
			}
		}
		return this;
	}

//------------------------------------------------------------------------------------------//
BitType::BitType(ANTLRTokenPtr p):BasicTypeAST(p, BitTypeValue )
	{
		if ( p != 0 ){
		
			RWCString s = p->getText();
			if ( s[0] == 'B' ){
				_data = new RWBitVec( 1, s[2] == '1' );
			}else{
				_data = new RWBitVec( 1, s[3] == '1' );	// Hex or Octal Format
			}
			//setWriteProtect(1);
		}else{
			_data = new RWBitVec( 1, 0 );
		}
	}

//------------------------------------------------------------------------------------------//
BitType::BitType(int a):BasicTypeAST(0, BitTypeValue )
	{
		_data= new RWBitVec(1,a);
	}
	
AST *
BitType ::clone( Scope * s ) const
	{
		AST * t = new BitType;

		t->setInteger( 0, getInteger() );
		return t;
	}

astream&
BitType::operator>>( astream& s )
	{
		s >> *_data;
		return s;
	}

astream&
BitType::operator<<( astream& s )
	{
		s << getInteger();
		return s;
	}
	
AST * BitType::eval( AST * a )
	{
		return this;
	}

Long
BitType::compare( AST * o ) const
	{
		return BitStrCompOp( this->_data, o->vec() );
	}

Long
BitType::length(int indx) const
	{
		return 1;
	}

AST *	BitType::assign( AST * a )
	{	
		*_data = *a->vec();
		return this;
	}

const
RWBitVec *
BitType::vec() const			{ return _data; };

Long
BitType::getInteger( int indx ) const
	{
		readEvent();
		return (*_data)[0];
	}

void
BitType::setInteger( int indx, Long value )
	{
		writeEvent();
		(*_data)[0] = value;
	}

//------------------------------------------------------------------------------------------//
StringOfBitType::StringOfBitType(ANTLRTokenPtr p, int maxlen):BasicTypeAST(p, StringOfBitTypeValue ) {

	int	bits_per_digit,
		leading;

	_max_length = _dyn_length = maxlen;

	if ( maxlen == 0 ){
		if ( p!=0 ){
			RWCString  binary_number = RWCString( p->getText() );

			switch ( binary_number[0] ) {
				case 'X':
					if ( binary_number[1] == '\'' ){
						leading = 4;		// Default # of bits left digit.
						binary_number.remove( 0, 2 );	// remove H'
					}else{
						leading = binary_number[1] - '0';
						binary_number.remove( 0, 3 );	// remove H?'
					}
					bits_per_digit = 4;		
					break;
					
				case 'O':
					if ( binary_number[1] == '\'' ){
						leading = 3;
						binary_number.remove( 0, 2 );	// remove O'
					}else{
						leading = binary_number[1] - '0';
						binary_number.remove( 0, 3 );	// remove O?'
					}
					bits_per_digit = 3;		
					break;

				case 'B':
					leading = 1;
					binary_number.remove( 0, 2 );		// remove B'
					bits_per_digit = 1;		
					break;
			}

			binary_number.remove( binary_number.length()-1, 1 );	// remove the trailing '
			
			int digits     = binary_number.length();
			int last_digit = digits - 1;
			
			_dyn_length = _max_length = leading + bits_per_digit * (digits-1);
			_vec = new RWBitVec( _max_length );
			
			
			for( int pos = 0; pos < digits; ++pos ){
			
				int lsb   = digits - pos - 1;
				int digit = (binary_number[ lsb ] < 'A') ?
					    (binary_number[ lsb ] - '0') : 
					    (binary_number[ lsb ] - 'A' + 10);
				int start = pos*bits_per_digit;
				
				for (	int bit = start;
					bit < ( pos < last_digit ? start + bits_per_digit : start + leading );
					bit++ ){
				
					(*_vec)[bit] = digit & 1;
					digit = digit >> 1;
				}
			}
		}
		//setWriteProtect(1);
	}else{
		_max_length = maxlen;
		_dyn_length = 0;
		_vec = new RWBitVec( _max_length, 0 );
	}
	_data = new ArrayObject( _max_length );
	_assign( _vec );
}
	
	
StringOfBitType::StringOfBitType(const RWBitVec * str, int dynlen):BasicTypeAST(0, StringOfBitTypeValue )
	{

		_max_length = str->length();
		_dyn_length = dynlen;

		_data = new ArrayObject( _max_length );
		_assign( str );
		_vec = new RWBitVec( *str );
	}

StringOfBitType::~StringOfBitType()
	{
		delete _data;
		delete _vec;
	}

astream&
StringOfBitType::operator>>( astream& s )
	{
		s >> *_vec;
		return s;
	}

astream&
StringOfBitType::operator<<( astream& s )
	{
		s << vec();

		return s;
	}


AST *
StringOfBitType::clone( Scope * s ) const
	{

		return new StringOfBitType( vec(), _dyn_length );
	}
		
AST *
StringOfBitType::eval( AST * a ) 
	{

		if ( a == 0 ){
			return this;
		} else if ( a == this ){
			return (*_data)[0]; // any one will do. We know 0 is always there
		} else {
			return (*_data)[a->getInteger()-1];
		}
	return this;
	}

	
Long
StringOfBitType::compare( AST * o ) const{

	return BitStrCompOp( vec(), o->vec() );
}

Long
StringOfBitType::index( AST * o ) const{

	const RWBitVec *	xvec = vec();
	const RWBitVec *	yvec = o->vec();
	RWBoolean	match = FALSE;
	int		length_diff = yvec->length() - xvec->length();
	int		pos = 0;

	for ( pos = 0; (pos <= length_diff) && (match == FALSE); ++pos ){

		match = TRUE;
		for ( int bit = 0; bit < xvec->length(); ++bit ){

			if ( xvec->operator[](bit) != yvec->operator[](bit + pos) ){

				match = FALSE;
				break;
			}
		}
	}

	if ( match ){
		return pos ;
	}else{
		return 0 ;
	}
}

Long	StringOfBitType::count( AST * o ) const{

	const RWBitVec *	yvec = vec();
	const RWBitVec *	xbit  = o->vec();
	Long			cnt = 0;

	for ( int pos = 0; pos < yvec->length(); ++pos ){

		if ( yvec->operator[](pos) == xbit->operator[](0) ){

			++cnt;
		}
	}

	return cnt;
}


Long	StringOfBitType::length( int indx ) const
	{
		if ( indx )	return _max_length;
		else		return _dyn_length;
	}

AST *	StringOfBitType::init( AST * a )
	{
		if ( a ){
			AST * x = a->eval( a );
			if ( x ){
				if ( verify(this,x) ){
					assign( x );
				}else{
					Error_Report( "Data is not compatible with STRING() OF BIT", a );
				}
			}
		}
		return this;
	}

AST * StringOfBitType::assign( AST * a )
{
	_dyn_length = a->length(0);
	_assign( a->vec() );
	
	return this;
}


const RWBitVec * StringOfBitType::vec() const
	{
		int i;
		for(i=0;i<_dyn_length;i++){
			(*_vec)[i]= (*(*_data)[i]->vec())[0];
		}
		for(i=_dyn_length;i<_max_length;i++){
			(*_vec)[i]= (*(*_data)[i]->vec())[0];
		}
		return _vec;
	}


void  StringOfBitType::_assign( const RWBitVec *s )
{
	if ( _dyn_length > _max_length ) assert(0);

	for ( int i=0; i < _max_length; i++ ){
	
		if ( i < _dyn_length ){
			(*_data)[i] = new BitType((*s)[i]);
		}else{
			(*_data)[i] = new BitType(0);
		}
	}
}

ArrayObject *
StringOfBitType::array(AST * len)
	{
		if(len){
			_data->reshape(len->getInteger());
		}
		return _data;
	}

//------------------------------------------------------------------------------------------//

PreDeclaredEnumerationType::PreDeclaredEnumerationType(ANTLRTokenPtr p)
		:BasicTypeAST(p, PreDeclaredEnumerationTypeValue )
		{
		}
		
AST * PreDeclaredEnumerationType ::clone( Scope * s ) const
	{
		AST *	t = new PreDeclaredEnumerationType;
		return	t;
	}
			
AST * PreDeclaredEnumerationType::eval( AST * a )
	{
		return this;
	}

Long PreDeclaredEnumerationType::compare( AST * o ) const
	{
		cerr<< " PreDeclaredEnumerationType::compare(AST*) [Don't call me yet !!] " << endl;
		assert(0);return 0;
	}

//------------------------------------------------------------------------------------------//
#ifdef LAZY_ARRAY
ArrayType::ArrayType( AST * element, Long lo, Long hi )

		:BasicTypeAST( 0, ArrayTypeValue ),
		_lo( lo ),
		_hi( hi )
	{
		_data = new ArrayObject( _hi-_lo+1 );
		
		if ( element ){
			
			(*_data)[0] = element->clone();
		}
	};
#else 
ArrayType::ArrayType( AST * element, Long lo, Long hi )

		:BasicTypeAST( 0, ArrayTypeValue ),
		_lo( lo ),
		_hi( hi )
	{
		_data = new ArrayObject( _hi-_lo+1 );
		
		if ( element ){
			for ( Long i = 0; i <= (_hi-_lo); i++ ){
			
				(*_data)[i] = element->clone();
			}
		}
	};

#endif
ArrayType::ArrayType( RWTValOrderedVector<AST*> &l )

		:BasicTypeAST( 0, ArrayTypeValue ),
		_lo(1),
		_hi( l.length() )
	{
		_data = new ArrayObject( l.length() );
		
		for ( Long i = 0; i <= (_hi-_lo); i++ )  (*_data)[i] = l[i];
	};


AST *	ArrayType::assign( AST * a )
	{
		ArrayObject & from_data = *(((ArrayType *)a)->_data);

		if ( from_data.length() == _data->length() ) {
			for ( int element = 0;  element <= (_hi-_lo);  ++element ){
				if(!(*_data)[element] ->assign( from_data[element] )){
					return 0;
				}
			}
		} else {
			Error_Report( "Array Types are not assignment compatible (different lengths)", this );
			return 0;
		}
	};
	
	
AST *	ArrayType::clone( Scope * s ) const
	{
		AST *	t = new ArrayType( (*_data)[0], _lo, _hi );
		ArrayObject & data = *(((ArrayType *)t)->_data);
		
		t->assign( (AST *)this );
		
		return	t;
	};
				

AST *	ArrayType::eval( AST * a )
	{
		if ( a == 0 ){
		
			return this;
		
		}else if ( a == this ){
						// special tag for the down element type
			return (*_data)[0];	// 0 is as good as any...
		}else{
			return (*_data)[ a->getInteger() - _lo ];
		}
	}

AST *	ArrayType::init( AST * a )
	{
		AST *	t;
		
		for ( Long i = 0; i <= (_hi-_lo); i++ ){
		
			if ( t = (*_data)[i] ){
			
				t->init( a );
			//}else{
			//	(*_data)[i] = a->clone();
			}
		}
		return this;
	}
AST *	ArrayType::insert( AST * a )
	{
		AST *	t;
		
		// being called from parser just
		// for setting the type.
		if ( t = (*_data)[0] ){
			
				t->init( a );
		}else{
			(*_data)[0] = a;
		}
		return this;
	}

Long	ArrayType::compare( AST * o ) const
	{
		cerr << " ArrayType::compare(AST*) [Don't call me yet !!] " << endl;
		assert( 0 );
		return 0;
	}

Long	ArrayType::length( int indx ) const
	{
		return _data->length();
	}
AST *
ArrayType::check( AST * a )
	{
		if(a&&verify(this,a)){
			ArrayObject & from_data = *(((ArrayType *)a)->_data);

			if ( from_data.length() == _data->length() ){
				return (*_data)[0] ->check( from_data[0] );// note that we only flag if it is OK or not..
			} else {
				Error_Report( "Array Types are not assignment compatible (different lengths)", this );
			}
		}
		return 0;
	}

//------------------------------------------------------------------------------------------//
ArraySliceType::ArraySliceType( AST * lo, AST * hi, AST * by )

		:BasicTypeAST( 0, ArraySliceTypeValue ),
		_lo(lo), _hi(hi), _by(by)
	{};

astream&	ArraySliceType::operator>>( astream& s )
	{
		AST *	element;
		
		Long	lo = _lo->eval()->getInteger();
		Long	hi = _hi->eval()->getInteger();
		Long	by = ( _by ? _by->eval()->getInteger() : 1 );
			
		IntegerNumber	pos;
		
		for ( int i = lo; i <= hi; i = i + by ){
		
			pos.setInteger( 0, i );
			element = ASTdown()->eval( &pos );
			s >> element;
		}

		return s;
	}

astream&	ArraySliceType::operator<<( astream& s )
	{
		AST *	element;
		
		Long	lo = _lo->eval()->getInteger();
		Long	hi = _hi->eval()->getInteger();
		Long	by = ( _by ? _by->eval()->getInteger() : 1 );
		
		IntegerNumber	pos;
		
		for ( int i = lo; i <= hi; i = i + by ){
		
			pos.setInteger( 0, i );
			element = ASTdown()->eval( &pos );
			s << element;
		}
		return s;
	}
				
AST *	ArraySliceType::eval( AST * a )
	{
		AST *	array = ASTdown()->eval();
		
		Long	lo = _lo->eval()->getInteger();
		Long	by = ( _by ? _by->eval()->getInteger() : 1 );
					
		if ( a == 0 ){
			return this;
		}else if ( a == this ){
								// get wherever the array is
			return array->eval( array );		// and just get any element...
		}else{
			IntegerNumber tmp( (a->eval()->getInteger() - 1) * by + lo );
			return array->eval( &tmp );
		}
	};
	
Long	ArraySliceType::length( int indx ) const
	{
		Long	lo = _lo->eval()->getInteger();
		Long	hi = _hi->eval()->getInteger();
		Long	by = ( _by ? _by->eval()->getInteger() : 1 );
		Long	size = 0;
		
		for ( int i = 0; i <= (hi-lo); i = i+by )
			++size;

		return size;
	}

//------------------------------------------------------------------------------------------//	
ArrayElementsType::ArrayElementsType( RWTValOrderedVector<AST*> &l )

		:BasicTypeAST( 0, ArrayElementsTypeValue )
	{
		_data = new ArrayObject( l.length() );
		
		for ( Long i = 0; i < l.length(); i++ )  (*_data)[i] = l[i];
	}

astream&	ArrayElementsType::operator>>( astream& s )
	{
		AST *	element;
				
		for ( int i = 0; i < _data->length(); ++i ){
		
			int pos = (*_data)[i] ->eval()->getInteger();
			IntegerNumber tmp( pos );
			element = ASTdown()->eval( &tmp );
			s >> element;
		}
		return s;
	}

astream&	ArrayElementsType::operator<<( astream& s )
	{
		AST *	element;
				
		for ( int i = 0; i < _data->length(); ++i ){
		
			int pos = (*_data)[i] ->eval()->getInteger();
			IntegerNumber tmp( pos );
			element = ASTdown()->eval( &tmp );
			s << element;
		}
		return s;
	}
				
AST *	ArrayElementsType::eval( AST * a )
	{
		AST *	array = ASTdown()->eval();
							
		if ( a == 0 ){
			return this;
		}else if ( a == this ){
								// get wherever the array is
			return array->eval( array );		// and just get any element...
		}else{
			int	passed_position  = a->eval()->getInteger() - 1;
			int	element          = (*_data)[passed_position] ->eval()->getInteger();
			IntegerNumber tmp( element );
			return array->eval( &tmp );
		}
	}

//------------------------------------------------------------------------------------------//
TextType::TextType( ANTLRTokenPtr p )
	
		:BasicTypeAST( p, TextTypeValue )
	{};
		
AST *	TextType::clone( Scope * s ) const
	{
		AST *	t = new TextType;
		return	t;
	};
	
AST *	TextType::eval( AST * a ){return this;};

//------------------------------------------------------------------------------------------//
FileType::FileType( ANTLRTokenPtr p )

		:BasicTypeAST( p, FileTypeValue ),
		_enabled( 0 ),
		_file( 0 )
	{}

FileType::FileType( astream * file )

		:BasicTypeAST( 0, FileTypeValue ),
		_enabled( 0 ),
		_file( file )
	{}
	
AST *	FileType::clone( Scope * s ) const
	{
		AST *	t = new FileType( getToken() );
		return	t->assign( (AST*)this );
	}

AST *	FileType::assign( AST * a )
	{
		_file    = a->getStream();
		_enabled = a->getInteger();
		
		return this;
	}
		
AST *
FileType::eval( AST * a )
	{
		return this;
	}

Long
FileType::getInteger( int indx ) const
	{
		readEvent();
		switch (indx){
		case 0:
			return _enabled;
		case 1:
			{
				if(_file){
					long here =	_file->tellg();
							_file->seekg(0,ios::end);
					long size =	_file->tellg();
							_file->seekg(here);
					return size;
				}
				return 0;
			}
		case 2:
			return _file->eof();
		default:
			assert(0);
			return 0;
		}
	}

void
FileType::setInteger( int indx, Long value )
	{
		writeEvent();
		_enabled = value;
	}

astream&
FileType::operator<<( astream& s )
	{
		return *_file;
	}

astream&
FileType::operator>>( astream& s )
	{
		return *_file;
	}
	
astream *
FileType::getStream() const
	{
		return _file;
	}
	
//------------------------------------------------------------------------------------------//
RecordType::RecordType( Scope * scope, int id )

		:BasicTypeAST( NULL, RecordTypeValue )
	
	{
		m_scope = scope;
		
		if ( id == 0 )
			m_record_id = _get_record_id();
		else
			m_record_id = id;
	}

AST *
RecordType::init( AST * a )
	{
		ASTList * _list=getScope()->getASTList();
		ASTListIterator	fields( *_list );
		
		for ( int i=0; ++fields; ++i )
			(*_list)[i]->init(a);
			
		return this;
	}

AST *
RecordType::clone( Scope * s ) const
	{
		AST *	element;
		Scope *		new_scope = new RecordScope( (s ? s :getScope()->getPrev()) );
		//ASTList *	new_list  = new ASTList;
		ASTList *	new_list  = new_scope->getASTList();
		ASTList *	_list=m_scope->getASTList();

		ASTListIterator	iterate( *(_list) );
		
		for ( int i=0;  iterate();  ++i ){
		
			element = (AST *)( (*_list)[i]->clone() );
			//new_list->insert( element );
			new_scope->insertSymbolValue( element );
		}
		
		return ( new RecordType( new_scope, m_record_id ) );
	}

AST *
RecordType::assign( AST * a )
	{
		ASTList *	_list=m_scope->getASTList();
		ASTListIterator	myiter( *_list );
		
		ASTList &	other = *(a->getScope()->getASTList());
		ASTListIterator a_iter( other );
		
		for ( int i=0; (++myiter && ++a_iter); ++i )
			(*_list)[i]->assign( other[i]->eval() );
			
		return this;
	}

Scope *
RecordType::getScope() const
	{
		return m_scope;
	}

Long
RecordType::getInteger( int indx ) const 
	{
		return m_record_id;
	}

AST *
RecordType::eval( AST * a ) {return this;}

int
RecordType::_get_record_id()	{ static int id = 0; return ++id; }

AST *
RecordType::check( AST * a )
	{
		if(a&&verify(this,a)){
			ASTList *	_list=m_scope->getASTList();
			ASTListIterator	myiter( *_list );
		
			ASTList &	other = *(a->getScope()->getASTList());
			ASTListIterator a_iter( other );
			if(_list->entries() != other.entries()){
				Error_Report("Records have different # of elements",a);
				return 0;
			}
			for ( int i=0; (++myiter && ++a_iter); ++i ){
				if(!(*_list)[i]->check( other[i]->eval() )){
					return 0;
				}
			}
			return this;
		} else {
			Error_Report("Not RECORD compatible",a);
			return 0;
		}
	}

//------------------------------------------------------------------------------------------//
StatementNumber::StatementNumber( ANTLRTokenPtr p )

		:BasicTypeAST( p, StatementNumberValue )
	{
		if ( p == 0 ){
			_data=0;
		}else{
			_data = strtoul( p->getText(), NULL, 10 );
		}
	}

AST *
StatementNumber::eval( AST * a )
	{
		return _target;
	}

Long
StatementNumber::getInteger( int indx ) const
	{
		return _data;
	}

AST *
StatementNumber::assign( AST * a )
	{
		if ( a ) _target = a;
		return this;
	}

AST *
StatementNumber::init( AST * a )
	{
		if ( a ) _target = a;
		return this;
	}

