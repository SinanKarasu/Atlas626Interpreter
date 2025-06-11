#include	"AtlasAST.h"
#include	"InitList.h"

//                       variable initialization support                        //

// Made STL...
// InitList::InitList()
// 	{
// 	}
// 
InitData::InitData(AST * rep,AST * data,InitList * list)
		:_rep(rep),_counter(0),_data(data),_list(list),_iterator(0)
		{
			if(_list){
				_iterator=new InitListIterator (*_list);
			}
		};
// 
// InitListIterator::InitListIterator(InitList & il)
// 	:RWTPtrSlistIterator<InitData>(il)
// 	{
// 	}
// 
void
InitData::reset()
	{
		_counter=0;
		if(_list){
			_iterator->reset();
		}
	}
// 
// RWBoolean
// InitData::more()
// 		{ 
// 			if(_counter < _rep->getInteger()) {
// 				return 1;
// 			} else if(_list) {
// 				if( (*_iterator)()){
// 					return 1;
// 				} else {
// 					return 0;
// 				}
// 			} else {
// 				return 0;
// 			}
// 		}
// 

