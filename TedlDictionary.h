#pragma once
//                       symbol table support                        //

#include "Dictionary.h"
#include <string>

class DeviceEquivalence{
public:
	DeviceEquivalence(std::string name, std::string capability="-");
	operator RWCString()	const;
	
////	std::string getCapabilityName()	const;
////	std::string getName()		const;
	
	const std::string& getCapabilityName() const { return m_CapabilityName; }
    const std::string& getName() const { return m_Name; }

	
protected:	
	std::string	m_Name;
	std::string	m_CapabilityName;	
	
};

using TedlSymbolDictionary = AppendableMap<std::string, DeviceEquivalence*>;
// class TedlSymbolDictionary : public RWTValHashDictionary<RWCString,DeviceEquivalence * > {
// public:
// 	TedlSymbolDictionary();
// 	
// private:
//    enum { NbrBuckets = RWDEFAULT_CAPACITY };
//};

class Resource;

using ResourceDictionary = AppendableMap<std::string, Resource*>;
// class ResourceDictionary : public RWTValHashDictionary<RWCString,Resource *> {
// public:
// 	ResourceDictionary();
// 	
// private:
//    enum { NbrBuckets = RWDEFAULT_CAPACITY };
// };

using ResourceDictionaryIterator = ResourceDictionary::iterator;

// class ResourceDictionaryIterator : public RWTValHashDictionaryIterator<RWCString,Resource *>{
// public:
// 	ResourceDictionaryIterator( ResourceDictionary &d );
// };

