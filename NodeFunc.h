#ifndef NodeFunc_H
#define NodeFunc_H

//typedef  int  NodeFunc (Vertex * v);

class NodeFunc{
public:
	NodeFunc	(int (*f)(Vertex * v) )		;
	int		eval(Vertex * v)		;
	void		attach(int (*f)(Vertex * v))	;

protected:
	int (*m_f)(Vertex * v);
};

#endif // NodeFunc_H

