#ifndef BFS_H
#define BFS_H

#include	"NodeFunc.h"
#include	"EdgeFunc.h"

class BFS {
public:

	BFS	(
			NodeFunc	*	process		,
			EdgeFunc	*	criteria
		);

	BFS	(
			NodeFunc	*	initialize	,
			NodeFunc	*	process		,
			EdgeFunc	*	criteria_1	,
			EdgeFunc	*	criteria_2
		);

	int	evalSingle(Vertex * v);

	int	evalDouble(Vertex * v);

protected:
	NodeFunc	*	m_initialize	;
	NodeFunc	*	m_process	;
	EdgeFunc	*	m_criteria_1	;
	EdgeFunc	*	m_criteria_2	;
	
};

#endif // BFS_H
