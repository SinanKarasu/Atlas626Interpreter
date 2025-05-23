#include	"Types.h"
#include	"Resource.h"
#include	"DeviceModels.h"
#include	"Circuit.h"

Resource *
DeviceModels::clone(Resource * previous,RWCString & newName)
	{
		return new DeviceModels(previous,this);
	}


DeviceModels::DeviceModels( Resource * previous)
	:Resource(previous,RWCString(""),RWCString(""))
	{
	}

DeviceModels::DeviceModels(Resource * previous,Resource * source)
	:Resource(previous,RWCString(""),source)
	{
		circuit()->Build(this,source);
	}
