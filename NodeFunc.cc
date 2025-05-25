#include	"Std.h"
#include	"Resource.h"
#include	"NodeFunc.h"

NodeFunc::NodeFunc(int (*f)(Vertex * v) )
	:m_f(f)
	{
	}
	
int
NodeFunc::eval(Vertex * v)
	{
		return (*m_f)(v);
	}

void
NodeFunc::attach(int (*f)(Vertex * v))
	{
		m_f=f;
	}
