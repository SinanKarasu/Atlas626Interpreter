#include	"Types.h"
#include	"AtlasAST.h"
#include	"ArrayObject.h"

ArrayObject::ArrayObject(Long size)
	:RWTValVector<AST *>(size,(AST *)0)
	{
		//for(int i=0;i<size;i++){
		//	(*this)[i]=0;
		//}
	}
ArrayObject::~ArrayObject()
	{
		for(int i=0;i<size();i++){
			delete (*this)[i];
		}
	}

#ifdef LAZY_ARRAY
ArrayObject *
ArrayObject::clone( Scope * s ) const
	{
		ArrayObject *	array = new ArrayObject(this->size());
		
		AST *	y = (*this)[0];
		if(y){
			y = y->clone(s);
		}
		(*array)[0] = y;
		return array;
	}

AST  *&
ArrayObject::operator [](size_t i)
	{
		AST * a;
		//if((i>=m_size) && m_dynamic){
		//	reshape(m_size=i+1);
		//}
		if((RWTValVector<AST *>::operator[](i))==0){
			if(a=RWTValVector<AST *>::operator[](0)){
				a=a->clone();
			}
			RWTValVector<AST *>::operator[](i)=a;
		}
		return	RWTValVector<AST *>::operator[](i);
	}


AST *&
ArrayObject::operator [](size_t i) const
	{
		assert(0);
		return RWTValVector<AST *>::operator[](0);
	}
	
	
#else 
ArrayObject *
ArrayObject::clone( Scope * s ) const
	{
		ArrayObject *	array = new ArrayObject(this->size());
		
		for ( int i=0; i < this->size(); i++ )
		{
			AST *	y = ((*this)[i])->clone();
			(*array)[i] = y;
		}
		return array;
	}
#endif
