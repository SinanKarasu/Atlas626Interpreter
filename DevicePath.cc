#include	"Types.h"
#include	"Graph.h"
#include	"NodeName.h"
#include	"DevicePath.h"

DevicePath::DevicePath(const NodeName & nodeName,PathNodeList * pnl,AST * pc)
	:Vertex(nodeName,DevicePathType)
	,m_path_characteristics(pc)
	,m_nodeName(nodeName)
	,m_pathNodeList(pnl)
	{
	}

int
DevicePath::admitConnect(Vertex * w)
	{
		return 1;
	}

RWCString 
DevicePath::theName() 
	{
		//sik return std::string("DevicePath:")+m_nodeName;
		return std::string("DevicePath:")+m_nodeName.getName();

	}


void
DevicePath::commitEquivalence(Vertex * w)
	{
		// this is a dummy right now. has to be fixed....
		if(w && (w!=this)){		// we don't commit, period....
			assert(equivalenceClass()==w->equivalenceClass());
			w->commitEquivalence(w);// pass it on....
		}
	}

int
DevicePath::isSource(Vertex * w)
	{
		return 0;
	}

// unsigned
// DevicePathHash(const RWCString& str) { return str.hash(); }

// DevicePathDictionary::DevicePathDictionary():RWTValHashDictionary<RWCString,DevicePath *>(DevicePathHash)
//         {resize(NbrBuckets);}

// DevicePathList::DevicePathList()
// 	:RWTValSlist<DevicePath *>()
// 	{
// 	}
	
// DevicePathListIterator::DevicePathListIterator( DevicePathList &d )
//         :RWTValSlistIterator<DevicePath *> (d)
//         {
//         }

// DevicePathDictionaryIterator::DevicePathDictionaryIterator(DevicePathDictionary &d)
// 	:RWTValHashDictionaryIterator< RWCString,DevicePath *> (d) 
// 	{
// 	}
