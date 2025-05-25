#include	"Types.h"
#include	"Resource.h"
#include	"CompositeDevice.h"

CompositeDevice::CompositeDevice( RWCString & newName, Resource * source   )
		:Device(newName, source   )
	{
		m_ResourceDictionary=new ResourceDictionary;
	}
	
Resource * 
CompositeDevice::clone(Resource * previous,RWCString & newName)
	{
		assert(0);return new Device(newName,this);
	}

ResourceDictionary * 
CompositeDevice::getResourceDictionary()
	{
		return  m_ResourceDictionary;
	}

Resource * 
CompositeDevice::getDevice(RWCString & dev)
	{
		Resource * r=0;
		m_ResourceDictionary->findValue(dev,r);
		return r;
	}
