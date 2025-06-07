#ifndef ArrayObject_H
#define ArrayObject_H


class ArrayObject:public std::vector<AST *> {
public:
	ArrayObject( Long size=1 );
	ArrayObject * clone( class Scope * s=0 ) const ;
	virtual ~ArrayObject();
#ifdef LAZY_ARRAY
	AST *& operator[](size_t i);

private:
	// do not allow....
	AST *& operator[](size_t i) const;
#endif




};

#endif // ArrayObject_H
