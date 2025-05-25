#include	"Types.h"
#include	"Resource.h"
#include	"Device.h"
#include	"Circuit.h"

Resource *
Device::clone(Resource * previous,RWCString & newName)
	{
		return new Device(newName,this);
	}


Device::Device(RWCString & name,RWCString &  version)
	:Resource(0,name,version)
	{
	}


Device::Device(RWCString & newName,Resource * source)
	:Resource(0,newName,source)
	{
		circuit()->Build(this,source);
	}
	
Vertex *
Device::checkLoopThruConnections(Vertex * v,DFSContext & c)
	{
		VertexDictionaryIterator vit(*m_nodeDictionary);
		while(++vit){
			Vertex * w=vit.value();
			if(c.v==0){
				c.init(w,w);
			} else {
				cout << "Checking Resource:" << theName();
				cout << " Vertex " << vit.value()->theName() << endl;
			}
			if(!(w->DFSvisited())){
				if(w->getEquivalence()->getSourceCount()>0){
					if(w->getResource()==this){
						w->checkSourceLoop(v,c);
					}
				}
			}
			if(c.v->getResource()==this){
				cerr << " Yes " << endl;
			} else {
				cerr << " and No " << endl;
			}
		}
		return 0;
	}
