#include	"Types.h"
#include	"Resource.h"
#include	"Device.h"
#include	"Circuit.h"

Resource *
Device::clone(Resource * previous,const RWCString & newName)
	{
		return new Device(newName,this);
	}


Device::Device(const RWCString & name,const RWCString &  version)
	:Resource(0,name,version)
	{
	}


Device::Device(const RWCString & newName,Resource * source)
	:Resource(0,newName,source)
	{
		circuit()->Build(this,source);
	}
	
Vertex *
Device::checkLoopThruConnections(Vertex * v,DFSContext & c)
	{
		//VertexDictionaryIterator vit(*m_nodeDictionary);
		for(const auto& vit: *m_nodeDictionary) {
		//while(++vit){
			Vertex * w=vit.second;
			if(c.v==0){
				c.init(w,w);
			} else {
				std::cout << "Checking Resource:" << theName();
				std::cout << " Vertex " << vit.second->theName() << std::endl;
			}
			if(!(w->DFSvisited())){
				if(w->getEquivalence()->getSourceCount()>0){
					if(w->getResource()==this){
						w->checkSourceLoop(v,c);
					}
				}
			}
			if(c.v->getResource()==this){
				std::cerr << " Yes " << std::endl;
			} else {
				std::cerr << " and No " << std::endl;
			}
		}
		return 0;
	}
