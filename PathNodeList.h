#pragma once

#include	"PathNode.h"

using PathNodeList = AppendableList<PathNode*>;
using PathNodeListIterator = PathNodeList::iterator;

////class PathNodeList : public RWTValSlist<PathNode *> 
////{
////public:
////	PathNodeList();
////};
////
////class PathNodeListIterator : public RWTValSlistIterator<PathNode *>
////{
////public:
////	PathNodeListIterator( PathNodeList &d );
////};
////
