
#include	"Std.h"
#include	"Resource.h"
#include	"BFS.h"
#include	"TemplateLibrary/Queue.h"


BFS::BFS(NodeFunc * process, EdgeFunc * criteria)
	:m_initialize(0)
	,m_process(process)
	,m_criteria_1(criteria)
	,m_criteria_2(0)
	{
	}

int
BFS::evalSingle(Vertex * v)
{
	Vertex *x ;
	Edge * y;
	Queue<Vertex *> BFSQueue;
	int bfs_Search=Graph::G_CurrentSearch+1;

	v->m_BfsNum = bfs_Search;
	BFSQueue.Enqueue(v);
	while (!BFSQueue.IsEmpty()) {
		x = BFSQueue.GetFront();
		m_process->eval(x);
		BFSQueue.Dequeue();
		EdgeListIterator P(*(x->Adj));

		while (++P) {
			y = P.key();
			if (y->Dest->m_BfsNum < bfs_Search){
				if (m_criteria_1->eval(y) ) {
					y->Dest->m_BfsNum = bfs_Search;
					BFSQueue.Enqueue(y->Dest);
				}
			}
		}
	} // while que is not empty
	return 0;
}


BFS::BFS(NodeFunc * initialize ,NodeFunc * process ,EdgeFunc * criteria_1 ,EdgeFunc * criteria_2 )
	:m_initialize(initialize)
	,m_process(process)
	,m_criteria_1(criteria_1)
	,m_criteria_2(criteria_2)
	{
	}

int
BFS::evalDouble(Vertex * v)
{
	Vertex         *x;
	Edge         *y;
	int bfs_Search=Graph::G_CurrentSearch+1;
	
	Queue<Vertex *> BFSQueue;
	Queue<Vertex *> SUBQueue;

	v->m_BfsNum = bfs_Search;
	SUBQueue.Enqueue(v);
	while (!SUBQueue.IsEmpty()) {
		x = SUBQueue.GetFront();
		SUBQueue.Dequeue();
		m_initialize->eval (x);
		BFSQueue.Enqueue(x);
		while (!BFSQueue.IsEmpty()) {
			x = BFSQueue.GetFront();
			BFSQueue.Dequeue();
			m_process->eval(x);
			EdgeListIterator P(*(x->Adj));

			while (++P) {
				y = P.key();
				if (y->Dest->m_BfsNum < bfs_Search) {
					if (m_criteria_1->eval(y)) {
						y->Dest->m_BfsNum = bfs_Search;
						BFSQueue.Enqueue(y->Dest);
					} else if (m_criteria_2->eval (y)) {
						y->Dest->m_BfsNum = bfs_Search;
						SUBQueue.Enqueue(y->Dest);
					}
				}
			}	/* while */
		}		/* while BFSQueue is not empty */
	}			/* while SUBQueue is not empty */
	return 0;
}
