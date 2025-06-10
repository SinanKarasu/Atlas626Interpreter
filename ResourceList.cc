#include	"Types.h"
#include	"Resource.h"
#include	"ResourceList.h"


// ResourceList::ResourceList()
// 	:RWTValSlist<Resource *>()
// 	{
// 	}

// RWBoolean
// ResourceList::findValue(RWCString key,Resource *& value)
// 	{
// 		ResourceListIterator rlit(*this);
// 		while(++rlit){
// 			if(rlit.key()->getName()==key){
// 				value=rlit.key();
// 				return TRUE;
// 			}
// 		}
// 		return FALSE;
// 	}



// ResourceListIterator::ResourceListIterator( ResourceList &d )
//         :RWTValSlistIterator<Resource *> (d)
// 	{
// 	}

ResourceListStack::ResourceListStack():m_instantiated(0){}

ResourceList *
ResourceListStack::getOne()
	{
		if(empty()){
			m_instantiated++;
			return new ResourceList;
		} else {
			auto ret = top(); pop();
			return ret;
		}
	}
