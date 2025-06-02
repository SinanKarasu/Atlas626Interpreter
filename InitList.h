#pragma once

#include "Std.h"
#include "ResettableIterator.h"

//                       variable initialization support                        //



// sik weird, this causes an error, a conflict:  class InitList;

class InitData;                      // forward declaration
//class InitListIterator;              // forward declaration
using InitList = AppendableVector<InitData *>;  // now safe
//using InitListIterator = InitList::iterator;
using InitListIterator = ResettableIterator<InitList>;

class InitData
{
    public:
	InitData(AST * rep,AST * data,InitList * list);
	void reset();
	RWBoolean more();				
	AST * _rep;
	int _counter;
	AST * _data;
	InitList * _list;
	InitListIterator *_iterator;
};
// sik cleanup and rename the file to InitData.h


////class InitList : public RWTPtrSlist<InitData>
////{
////public:
////	InitList();
////};
////
////class InitListIterator:public  RWTPtrSlistIterator<InitData> 
////{
////public:
////	InitListIterator(InitList & il);
////};
////

