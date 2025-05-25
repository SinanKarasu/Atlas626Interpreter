#ifndef PathNodeList_H
#define PathNodeList_H

#include	"PathNode.h"

class PathNodeList : public RWTValSlist<PathNode *> 
{
public:
	PathNodeList();
};

class PathNodeListIterator : public RWTValSlistIterator<PathNode *>
{
public:
	PathNodeListIterator( PathNodeList &d );
};


#endif // PathNodeList_H
