#include	"Types.h"
#include	"Resource.h"
#include	"ConfigurationModels.h"
#include	"Circuit.h"

Resource *
ConfigurationModels::clone(Resource * previous,RWCString & newName)
	{
		return new ConfigurationModels(previous,this);
	}


ConfigurationModels::ConfigurationModels( Resource * previous)
	:Resource(previous,RWCString(""),RWCString(""))
	{
	}

ConfigurationModels::ConfigurationModels(Resource * previous,Resource * source)
	:Resource(previous,RWCString(""),source)
	{
		circuit()->Build(this,source);
	}
