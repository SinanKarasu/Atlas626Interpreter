#include	"Types.h"
#include	"Resource.h"
#include	"TPSHardware.h"
#include	"Circuit.h"

Resource *
TPSHardware::clone(Resource * previous,const RWCString & newName)
	{
		return new TPSHardware(newName,this);
	}


TPSHardware::TPSHardware(const RWCString & name,const RWCString &  version)
	:Resource(0,name,version)
	{
	}


TPSHardware::TPSHardware(const RWCString & newName,Resource * source)
	:Resource(0,newName,source)
	{
		circuit()->Build(this,source);
	}
	
