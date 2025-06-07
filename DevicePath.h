#ifndef DevicePath_H
#define DevicePath_H

#include	"Vertex.h"
#include	"PathNodeList.h"

class DevicePath:public Vertex {
public:
	DevicePath(const NodeName & nodeName,PathNodeList * pnl,AST * pc=0);
	virtual int		admitConnect		(Vertex * w) ;
	virtual RWCString	theName			()	;
	virtual	void		commitEquivalence	(Vertex * w=0);
	virtual	int		isSource(Vertex * w=0);
private:
	const NodeName & m_nodeName;
	PathNodeList * m_pathNodeList;
	AST * m_path_characteristics;
};

using DevicePathList = AppendableList<DevicePath *>;
// class DevicePathList : public RWTValSlist<DevicePath *> {
// public:
// 	DevicePathList();
//};

// class DevicePathListIterator : public RWTValSlistIterator<DevicePath *>{
// 	public: DevicePathListIterator( DevicePathList &d );
//};

using DevicePathDictionary = AppendableMap< std::string, DevicePath*>;
using DevicePathDictionaryIterator = DevicePathDictionary::iterator;

////class DevicePathDictionary : public RWTValHashDictionary< RWCString,DevicePath *> {
////	public:
////		DevicePathDictionary();
////
////	private:
////		enum { NbrBuckets = RWDEFAULT_CAPACITY };
////};
////
////class DevicePathDictionaryIterator : public RWTValHashDictionaryIterator< RWCString,DevicePath *>{
////        public: DevicePathDictionaryIterator(DevicePathDictionary &d);
////};
////


#endif	// DevicePath_H
