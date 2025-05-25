#ifndef BasicTypeAST_h
#define BasicTypeAST_h

#include	"AtlasAST.h"

#include	"ArrayObject.h"

#include	"InitList.h"

class StringOfBitType;

class BasicTypeAST:public AST {
	public:
		BasicTypeAST(ANTLRTokenPtr p=0, TheType s=BasicTypeValue);
		virtual	~BasicTypeAST();
	
		virtual AST *	eval	( AST * a=0  );
		virtual AST *	assign	( AST * a    );
		virtual TheType	getType	( AST * a=0  ) const;
		virtual	AST *	data	( AST * a = 0 );
		virtual AST *	Accept	( ASTVisitor & );
		virtual AST *  	check	( AST   * a=0 );

	private:
		TheType	_storage;	
};	


class DecimalNumber: public BasicTypeAST {
	public:
		DecimalNumber( ANTLRTokenPtr p=0 ,int sign=1);
		DecimalNumber( AST *a ); // used for integer to decimal conv.
		DecimalNumber( double data );
	
		virtual AST *	eval	( AST   * a=0 );
		virtual AST *	init	( AST   * a=0 );
		virtual AST *	assign	( AST   * a   );
		virtual AST *	clone	( Scope * s=0 ) const;
		virtual AST *	add	( AST   * a   );
		virtual	Long	compare	( AST   * o   ) const;
		virtual Long	getInteger( int indx=0 ) const;
		virtual void	setInteger( int indx=0, Long value=0 );
		virtual double	getDecimal( int indx=0 ) const;
		virtual void	setDecimal( int indx=0, double value=0.0 );

		virtual astream&	operator>>( astream& s );
		virtual astream&	operator<<( astream& s );
		
	private:
		double _data;
};

class IntegerNumber: public BasicTypeAST {
	public:
		IntegerNumber(ANTLRTokenPtr p=0 ,int sign=1);
		IntegerNumber(Long data);
	
		virtual AST *	eval	( AST   * a=0 );
		virtual AST *	init	( AST   * a=0 );
		virtual AST *	assign	( AST   * a   );
		virtual AST *	clone	( Scope * s=0 ) const;
		virtual AST *	add	( AST   * a   );
	
		virtual Long	getInteger( int indx=0 ) const;
		virtual void	setInteger( int indx=0, Long value=0 );
		virtual double	getDecimal( int indx=0 ) const;
		virtual void	setDecimal( int indx=0, double value=0.0 );
	
		virtual	Long	compare(AST * o) const;
		
		virtual astream&	operator>>( astream& s );
		virtual astream&	operator<<( astream& s );

	private:
		Long _data;
};


class EnumerationType: public BasicTypeAST{
	public:
		EnumerationType(ANTLRTokenPtr p=0, Long pos=0);
	
		virtual AST *	eval	( AST   * a=0 );
		virtual AST *	init	( AST   * a=0 );
		virtual AST *	assign	( AST   * a   );
		virtual AST *	clone	( Scope * s=0 ) const;
		virtual AST *	add	( AST   * a=0 );
		virtual AST *	succ	( AST   * a=0 );
		virtual AST *	pred	( AST   * a=0 );
		virtual	Long	compare	( AST   * o   ) const;
		virtual Long	getInteger(int indx=0) const;
		virtual void	setInteger( int indx=0, Long value=0 );
		
		virtual astream&	operator>>( astream& s );
		virtual astream&	operator<<( astream& s );

	private:
		friend class EnumerationsType;
		RWCString * _data;
		Long        _pos;
};

class EnumerationsType : public BasicTypeAST{
	public:
		EnumerationsType( ASTList * data );
	
		virtual AST *	eval	( AST   * a=0 );
		virtual AST *	init	( AST   * a=0 );
		virtual AST *  	check	( AST   * a=0 );
		virtual AST *	assign	( AST   * a   );
		virtual AST *	clone	( Scope * s=0 ) const;
		virtual	Long	compare	( AST   * o   ) const;
		
		virtual astream&	operator>>( astream& s );
		virtual astream&	operator<<( astream& s );

	private:
		friend class EnumerationType;
		ASTList* _data;
};

class ConnectionType: public BasicTypeAST{
	public:
		ConnectionType(ANTLRTokenPtr p=0);
	
		virtual RWCString  	getName	() const ;
		virtual AST *	eval	( AST   * a=0 );
		virtual AST *	init	( AST   * a=0 );
		virtual AST *	assign	( AST   * a   );
		virtual AST *	clone	( Scope * s=0 ) const;
		virtual	Long	compare	( AST   * o   ) const;

		virtual astream&	operator>>( astream& s );
		virtual astream&	operator<<( astream& s );

	private:
		friend class ConnectionsType;
		RWCString* _data;
};

class ConnectionsType: public BasicTypeAST{
	public:
		ConnectionsType(SymbolDictionary * list);
		
		virtual AST *	eval	( AST   * a=0 );
		virtual AST *	init	( AST   * a=0 );
		virtual AST *	check	( AST   * a=0 );
		virtual AST *	assign	( AST   * a   );
		virtual AST *	clone	( Scope * s=0 ) const;
		virtual	Long	compare	( AST   * o   ) const;
	
		virtual astream&	operator>>( astream& s );
		virtual astream&	operator<<( astream& s );

	private:
		SymbolDictionary * _data;
};

class TerminalType: public BasicTypeAST{
	public:
		TerminalType(ANTLRTokenPtr p=0);
	
		virtual AST *	eval	( AST   * a=0 );
		virtual AST *	assign	( AST   * a   );
		virtual AST *	clone	( Scope * s=0 ) const;
		virtual	Long	compare	( AST   * o   ) const;

		virtual astream&	operator>>( astream& s );
		virtual astream&	operator<<( astream& s );

	private:
		RWCString * _data;
};


class CharType: public BasicTypeAST{
	public:
		CharType(ANTLRTokenPtr p=0);
		CharType(const RWCString & c);
	
		virtual AST *	eval	( AST *   a=0 );
		virtual AST *	clone	( Scope * s=0 ) const;
		virtual AST *	assign	( AST *   a   );
		virtual AST *	add	( AST *   a=0 );
		virtual AST *	succ	( AST *   a=0 );
		virtual AST *	pred	( AST *   a=0 );
		virtual	Long	compare	( AST *   o   ) const;
		virtual AST *	init	( AST *   a=0 );
	
		virtual Long	getInteger(int indx=0) const;
		virtual const RWCString * str() const;
		
		virtual astream&	operator>>( astream& s );
		virtual astream&	operator<<( astream& s );

	private:
		void _assign(const RWCString *s);
		RWCString * _data;
		friend class StringOfCharType;
};

class CharClassType: public BasicTypeAST{
	public:
		CharClassType( ANTLRTokenPtr p = 0 );
	
		virtual AST *	eval	( AST   * a = 0 );
		virtual AST *	clone	( Scope * s = 0 ) const;
		virtual	Long	compare	( AST   * o     ) const;
};

class StringOfCharType: public BasicTypeAST{
	public:
		StringOfCharType( ANTLRTokenPtr    p=0, int len=0 );
		StringOfCharType( const RWCString *str, int len=0 );
		virtual ~StringOfCharType();
		virtual AST *	eval	( AST   *  a = 0 );
		virtual AST *	assign	( AST   *  a     );
		virtual AST *	clone	( Scope *  s = 0 ) const;
		virtual AST *	init	( AST   *  a = 0 );
	
		virtual	Long	compare	( AST   *  o     ) const;
		virtual Long	count	( AST   *  o     ) const;
		virtual Long	index	( AST   *  o     ) const;
		virtual Long	length	( int indx = 0   ) const;
	
		virtual const RWCString * str() const;
		virtual	ArrayObject * 	array(AST * a=0);

		virtual astream&	operator>>( astream& s );
		virtual astream&	operator<<( astream& s );

	private:
		ArrayObject *	_data;
		RWCString *	_str;
		int		_max_length;
		int		_dyn_length;
		void		_assign( const RWCString *s );
};

class DigClassType: public BasicTypeAST{
	public:
		DigClassType(ANTLRTokenPtr p=0);
	
		virtual AST *	eval	( AST   * a=0 );
		virtual AST *	clone	( Scope * s=0 ) const;
		virtual	Long	compare	( AST   * o   ) const;

		virtual astream&	operator>>( astream& s );
		virtual astream&	operator<<( astream& s );

	private:
		RWCString * _data;
};

class BooleanType: public BasicTypeAST{
	public:
		BooleanType(ANTLRTokenPtr p=0);
		BooleanType(int data);
	
		virtual AST *	eval	( AST   * a=0 );
		virtual AST *	clone	( Scope * s=0 ) const;
		virtual AST *	assign	( AST   * a   );
		virtual	Long	compare	( AST   * o   ) const;
		virtual AST *	init	( AST   * a=0 );
		virtual AST *	insert	( AST   * a   );
		virtual AST *	remove	( AST   * a   );
	
		virtual Long	getInteger( int indx=0 ) const;
		virtual void	setInteger( int indx=0, Long value=0 );
		virtual double	getDecimal( int indx=0 ) const;
		virtual void	setDecimal( int indx=0, double value=0.0 );
	
		virtual astream&	operator>>( astream& s );
		virtual astream&	operator<<( astream& s );

	private:
		int m_data;
		AST * m_indicator;
};

class BitType: public BasicTypeAST{
	public:
		BitType(ANTLRTokenPtr p=0);
		BitType(int a);
	
		virtual AST *	eval	( AST   * a=0 );
		virtual AST *	clone	( Scope * s=0 ) const;
		virtual AST *	assign	( AST   * a   );
		virtual	Long	compare	( AST   * o   ) const;
		virtual Long	length	( int  indx=0 ) const;

		virtual const RWBitVec * vec() const;
		virtual Long	getInteger( int indx=0 ) const;
		virtual void	setInteger( int indx=0, Long value=0 );
	
		virtual astream&	operator>>( astream& s );
		virtual astream&	operator<<( astream& s );

	private:
		RWBitVec *_data;
};

class StringOfBitType: public BasicTypeAST{
	public:
		StringOfBitType(ANTLRTokenPtr p=0,int len=0);
		StringOfBitType(const RWBitVec * data=0,int len=0);
		virtual ~StringOfBitType();

		virtual AST *	eval	( AST   *  a = 0 );
		virtual AST *	clone	( Scope *  a = 0 ) const;
		virtual AST *	assign	( AST   *  a     );
		virtual AST *	init	( AST   * a=0 );
	
		virtual	Long	compare	( AST   *  o     ) const;
		virtual Long	count	( AST   *  o     ) const;
		virtual Long	index	( AST   *  o     ) const;
		virtual Long	length	( int indx = 0   ) const;
	
		virtual const RWBitVec * vec() const;

		virtual astream&	operator>>( astream& s );
		virtual astream&	operator<<( astream& s );
		virtual	ArrayObject * 	array(AST * a=0);

	private:
		ArrayObject *	_data;
		RWBitVec *	_vec;
		int		_max_length;
		int		_dyn_length;
		void		_assign( const RWBitVec *s );
};


class PreDeclaredEnumerationType: public BasicTypeAST{
	public:
		PreDeclaredEnumerationType(ANTLRTokenPtr p=0);
	
		virtual AST *	eval	( AST   * a=0 );
		virtual AST *	clone	( Scope * a=0 ) const;
		virtual	Long	compare	( AST   * o   ) const;
};

class ArrayType: public BasicTypeAST{
	public:
		ArrayType( AST * element, Long  lo, Long  hi );
		ArrayType( RWTValOrderedVector<AST *> &l );
	
		virtual AST *	assign	( AST   * a    );
		virtual AST *	eval	( AST   * a=0  );
		virtual AST *	init	( AST   * a=0  );
		virtual AST *	insert	( AST   * a=0  );
		virtual AST *	clone	( Scope * s=0  ) const;
		virtual	Long	compare	( AST   * o    ) const;
		virtual Long	length	( int  indx=0  ) const;
		virtual AST *  	check	( AST   * a=0 );

	private:
		Long	_lo;
		Long	_hi;
		ArrayObject *	_data;
};


class ArraySliceType: public BasicTypeAST{
	public:
		ArraySliceType( AST * lo, AST * hi, AST * by );

		virtual AST *	eval	( AST * a=0 );
		virtual Long	length	( int indx=0 ) const;

		virtual astream& operator>>( astream& s );
		virtual astream& operator<<( astream& s );

	private:
		AST *	_by;
		AST *	_lo;
		AST *	_hi;
};

class ArrayElementsType: public BasicTypeAST{
	public:
		ArrayElementsType( RWTValOrderedVector<AST *> &l );

		virtual AST *	eval	( AST * a=0 );

		virtual astream& operator>>( astream& s );
		virtual astream& operator<<( astream& s );

	private:
		ArrayObject * _data;
};

class TextType: public BasicTypeAST{
	public:
		TextType( ANTLRTokenPtr p=0 );
	
		virtual AST *	eval	( AST   * a=0 );
		virtual AST *	clone	( Scope * s=0 ) const;
};

class FileType: public BasicTypeAST{
	public:
		FileType( ANTLRTokenPtr p );
		FileType( astream* file );
	
		virtual AST *	assign	( AST   * a=0 );
		virtual AST *	eval	( AST   * a=0 );
		virtual AST *	clone	( Scope * s=0 ) const;
		
		virtual astream& operator>>( astream& s );
		virtual astream& operator<<( astream& s );
		virtual astream* getStream() const;
		
		virtual Long	getInteger( int indx=0 ) const;
		virtual void	setInteger( int indx=0, Long value=0 );

	private:
		astream *	_file;
		
		int		_enabled;
};

class RecordType: public BasicTypeAST{
	public:
		RecordType( Scope *s, int id = 0 );
	
		virtual AST *	eval	( AST   *  a=0 );
		virtual AST *	assign	( AST   *  a=0 );
		virtual AST *	clone	( Scope *  s=0 ) const;
		virtual AST *	init	( AST   *  a=0 );
		virtual Long	getInteger( int indx=0 ) const;
	        virtual	Scope *	getScope() const ;
		virtual AST *  	check	( AST   * a=0 );

	private:
		Scope *		m_scope;
		int		m_record_id;
		static int	_get_record_id();
};


class StatementNumber: public BasicTypeAST{
	public:
		StatementNumber(ANTLRTokenPtr p=0);
	
		virtual AST *	eval	( AST * a=0 );
		virtual AST *	assign	( AST * a=0 );
		virtual AST *	init	( AST * a=0 );
	
		virtual Long	getInteger( int indx=0 ) const;

	private:
		Long  _data;
		AST * _target;
};


#endif // BasicTypeAST_h
