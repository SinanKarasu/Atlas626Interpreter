#include	"Types.h"
#include	"Resource.h"
#include	"TPSHardware.h"
#include	"Circuit.h"

Resource *
TPSHardware::clone(Resource * previous,RWCString & newName)
	{
		return new TPSHardware(newName,this);
	}


TPSHardware::TPSHardware(RWCString & name,RWCString &  version)
	:Resource(0,name,version)
	{
	}


TPSHardware::TPSHardware(RWCString & newName,Resource * source)
	:Resource(0,newName,source)
	{
		circuit()->Build(this,source);
	}
	
