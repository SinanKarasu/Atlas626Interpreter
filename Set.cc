#include	"Types.h"
#include	"Set.h"

Set::Set()
	:RWBitVec(16)
	{
	}

void
Set::operator=(int b)
		{
			for(int i=0;i<length();i++){
				if(b){
					setBit(i);
				} else {
					clearBit(i);
				}
			}
		}
		
void 
Set::set(int pos)
		{	
			if(length()<=pos){
				resize(pos+1);
			}
			setBit(pos);
		}
void 
Set::clear(int pos)
		{
			if(length()<=pos){
				resize(pos+1);
			}
			clearBit(pos);
		}
int 
Set::isSet(int pos)
		{
			if(pos<0){
				return 0;
			} else if(length()<=pos){
				return 0;
			} else {
				return testBit(pos);
			}
		}
