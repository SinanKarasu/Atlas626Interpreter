#include	"Types.h"
#include	"Resource.h"
#include	"AdaptationModels.h"
#include	"Circuit.h"

Resource *
AdaptationModels::clone(Resource * previous,RWCString & newName)
	{
		return new AdaptationModels(previous,this);
	}


AdaptationModels::AdaptationModels( Resource * previous)
	:Resource(previous,RWCString(""),RWCString(""))
	{
	}

AdaptationModels::AdaptationModels(Resource * previous,Resource * source)
	:Resource(previous,RWCString(""),source)
	{
		circuit()->Build(this,source);
	}
