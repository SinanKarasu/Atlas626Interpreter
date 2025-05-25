#ifndef TedlDictionary_h
#define TedlDictionary_h

//                       symbol table support                        //


class DeviceEquivalence{
public:
	DeviceEquivalence(RWCString name, RWCString capability="-");
	operator RWCString()	const;
	
	RWCString getCapabilityName()	const;
	RWCString getName()		const;
	
	
protected:	
	RWCString	m_Name;
	RWCString	m_CapabilityName;	
	
};


class TedlSymbolDictionary : public RWTValHashDictionary<RWCString,DeviceEquivalence * > {
public:
	TedlSymbolDictionary();
	
private:
   enum { NbrBuckets = RWDEFAULT_CAPACITY };
};

class Resource;

class ResourceDictionary : public RWTValHashDictionary<RWCString,Resource *> {
public:
	ResourceDictionary();
	
private:
   enum { NbrBuckets = RWDEFAULT_CAPACITY };
};


class ResourceDictionaryIterator : public RWTValHashDictionaryIterator<RWCString,Resource *>{
public:
	ResourceDictionaryIterator( ResourceDictionary &d );
};


#endif //Dictionary_h
