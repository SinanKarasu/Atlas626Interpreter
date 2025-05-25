#include	"Types.h"
#include	"Resource.h"
#include	"NodeName.h"

NodeName::NodeName(RWCString name,Resource * resource)
	:m_name(name),m_Resource(resource)
	{
	}

NodeName::operator const RWCString	() const
	{
		return m_name;
	}
	
Resource *
NodeName::getResource() const
	{
		return m_Resource;
	}

void 
NodeName::setResource(Resource * resource)
	{
		m_Resource=resource;
	}

const RWCString &
NodeName::getName() const
	{
		return m_name;
	}

const RWCString 
NodeName::theName() 
	{
		return m_Resource->theName() + "-" + getName();
	}
