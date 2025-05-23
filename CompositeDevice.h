#ifndef CompositeDevice_H
#define CompositeDevice_H

#include	"Device.h"


class CompositeDevice : public Device{
public:
	CompositeDevice( RWCString & newName, Resource * source   );
	
	virtual Resource * clone(Resource * previous,RWCString & newName);

	virtual ResourceDictionary * getResourceDictionary();

	virtual Resource * getDevice(RWCString & dev);

private:
	// Disable copy/assignment
        CompositeDevice (const CompositeDevice &);
        const CompositeDevice & operator= ( const CompositeDevice & );
};


#endif	// CompositeDevice_H
