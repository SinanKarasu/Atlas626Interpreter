#include	"Types.h"
#include	"Resource.h"
#include	"ConfigurationModel.h"
#include	"Circuit.h"

Resource *
ConfigurationModel::clone(Resource * previous,RWCString & newName)
	{
		return new ConfigurationModel(newName,this);
	}


ConfigurationModel::ConfigurationModel(RWCString & name,RWCString &  version)
	:Resource(0,name,version)
	{
	}


ConfigurationModel::ConfigurationModel(RWCString & newName,Resource * source)
	:Resource(0,newName,source)
	{
		circuit()->Build(this,source);
	}
	
