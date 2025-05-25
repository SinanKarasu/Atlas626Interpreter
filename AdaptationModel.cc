#include	"Types.h"
#include	"Resource.h"
#include	"AdaptationModel.h"
#include	"Circuit.h"

Resource *
AdaptationModel::clone(Resource * previous,RWCString & newName)
	{
		return new AdaptationModel(newName,this);
	}


AdaptationModel::AdaptationModel(RWCString & name,RWCString &  version)
	:Resource(0,name,version)
	{
	}


AdaptationModel::AdaptationModel(RWCString & newName,Resource * source)
	:Resource(0,newName,source)
	{
		circuit()->Build(this,source);
	}
	
