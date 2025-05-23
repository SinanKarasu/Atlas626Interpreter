#ifndef EdgeFunc_H
#define EdgeFunc_H

//typedef  int  EdgeFunc (Edge * v);

class EdgeFunc{
public:
	EdgeFunc	(int (*f)(Edge * e) )		;
	int 		eval		(Edge * e)	;
	void		attach(int (*f)(Edge * e))	;

protected:
	int (*m_f)(Edge * e);
};

#endif // EdgeFunc_H
