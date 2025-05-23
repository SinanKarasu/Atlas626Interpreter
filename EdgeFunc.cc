#include	"Std.h"
#include	"Resource.h"
#include	"EdgeFunc.h"

EdgeFunc::EdgeFunc(int (*f)(Edge * e) )
	:m_f(f)
	{
	}
	
int
EdgeFunc::eval(Edge * e)
	{
		return (*m_f)(e);
	}

void
EdgeFunc::attach(int (*f)(Edge * e))
	{
		m_f=f;
	}
