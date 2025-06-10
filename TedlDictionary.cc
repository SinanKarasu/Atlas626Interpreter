#include "Resource.h"
#include "TedlDictionary.h"
#include "ResourceList.h"

// unsigned TedlLabelHash(const RWCString& str) { return str.hash(); }


DeviceEquivalence::DeviceEquivalence(RWCString name, RWCString capability)
	:m_Name(name)
	,m_CapabilityName(capability)
	{
	}



DeviceEquivalence::operator RWCString	()	const
	{
		return getName();
	}


// RWCString
// DeviceEquivalence::getCapabilityName()	const
// 	{
// 		return m_CapabilityName;
// 	}
// 
// RWCString
// DeviceEquivalence::getName()	const
// 	{
// 		return m_Name;
// 	}

// TedlSymbolDictionary::TedlSymbolDictionary()
// 	:RWTValHashDictionary<RWCString ,DeviceEquivalence * >(TedlLabelHash)
// 	{
// 		resize(NbrBuckets);
// 	}
// 
// ResourceDictionary::ResourceDictionary()
// 	:RWTValHashDictionary<RWCString,Resource *>(TedlLabelHash)
// 	{
// 		resize(NbrBuckets);
// 	}
// 
// ResourceDictionaryIterator::ResourceDictionaryIterator( ResourceDictionary &d )
// 	:RWTValHashDictionaryIterator<RWCString,Resource *> (d)
// 	{
// 	}

