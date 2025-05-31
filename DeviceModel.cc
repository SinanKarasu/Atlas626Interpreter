#include	"Types.h"
#include	"Resource.h"
#include	"DeviceModel.h"
#include	"Device.h"
#include	"Circuit.h"

Resource *
DeviceModel::clone(Resource * previous, const RWCString & newName)
	{
		return new DeviceModel(newName,this);
	}

Resource *
DeviceModel::instantiate(Resource * previous, const RWCString & newName)
	{
		return new Device(newName,this);
	}

DeviceModel::DeviceModel( const RWCString & name, const RWCString &  version)
	:Resource(0,name,version)
	{
	}


DeviceModel::DeviceModel(const RWCString & newName,Resource * source)
	:Resource(0,newName,source)
	{
		circuit()->Build(this,source);
	}
	
