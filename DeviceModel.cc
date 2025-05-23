#include	"Types.h"
#include	"Resource.h"
#include	"DeviceModel.h"
#include	"Device.h"
#include	"Circuit.h"

Resource *
DeviceModel::clone(Resource * previous,RWCString & newName)
	{
		return new DeviceModel(newName,this);
	}

Resource *
DeviceModel::instantiate(Resource * previous,RWCString & newName)
	{
		return new Device(newName,this);
	}

DeviceModel::DeviceModel(RWCString & name,RWCString &  version)
	:Resource(0,name,version)
	{
	}


DeviceModel::DeviceModel(RWCString & newName,Resource * source)
	:Resource(0,newName,source)
	{
		circuit()->Build(this,source);
	}
	
