#ifndef ResourceList_H
#define ResourceList_H


// class ResourceList : public RWTValSlist<Resource *>{
// public:
// 	ResourceList();
// 	RWBoolean findValue(RWCString key,Resource *& value);
// 	
// };

using ResourceList = AppendableList<Resource *>;
class ResourceListIterator : public RWTValSlistIterator<Resource *>{
public:
	ResourceListIterator( ResourceList &d );
};


class ResourceListStack : public RWTStack< ResourceList * , RWTValOrderedVector < ResourceList * > >{
public:
	ResourceListStack();
	ResourceList * getOne();
private:
	int m_instantiated;
};



#endif // ResourceList_H
