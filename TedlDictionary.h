#pragma once
//                       symbol table support                        //

#include <string>

class DeviceEquivalence{
public:
	DeviceEquivalence(std::string name, std::string capability="-");
	operator RWCString()	const;
	
	std::string getCapabilityName()	const;
	std::string getName()		const;
	
	
protected:	
	std::string	m_Name;
	std::string	m_CapabilityName;	
	
};


class TedlSymbolDictionary : public RWTValHashDictionary<RWCString,DeviceEquivalence * > {
public:
	TedlSymbolDictionary();
	
private:
   enum { NbrBuckets = RWDEFAULT_CAPACITY };
};

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

