#ifndef InitList_H
#define InitList_H


#include "Std.h"


//                       variable initialization support                        //



class InitList;

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
	class InitListIterator *_iterator;
};

class InitList : public RWTPtrSlist<InitData>
{
public:
	InitList();
};

class InitListIterator:public  RWTPtrSlistIterator<InitData> 
{
public:
	InitListIterator(InitList & il);
};



#endif //InitList_H
