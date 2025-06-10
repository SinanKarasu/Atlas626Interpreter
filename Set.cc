#include	"Types.h"
#include	"Set.h"

////sik??  Set::Set()
////sik??  	:RWBitVec(16)
////sik??  	{
////sik??  	}
////sik??  
////sik??  void
////sik??  Set::operator=(int b)
////sik??  		{
////sik??  			for(int i=0;i<length();i++){
////sik??  				if(b){
////sik??  					setBit(i);
////sik??  				} else {
////sik??  					clearBit(i);
////sik??  				}
////sik??  			}
////sik??  		}
////sik??  		
////sik??  void 
////sik??  Set::set(int pos)
////sik??  		{	
////sik??  			if(length()<=pos){
////sik??  				resize(pos+1);
////sik??  			}
////sik??  			setBit(pos);
////sik??  		}
////sik??  void 
////sik??  Set::clear(int pos)
////sik??  		{
////sik??  			if(length()<=pos){
////sik??  				resize(pos+1);
////sik??  			}
////sik??  			clearBit(pos);
////sik??  		}
////sik??  int 
////sik??  Set::isSet(int pos)
////sik??  		{
////sik??  			if(pos<0){
////sik??  				return 0;
////sik??  			} else if(length()<=pos){
////sik??  				return 0;
////sik??  			} else {
////sik??  				return testBit(pos);
////sik??  			}
////sik??  		}


Set::Set() : BitVec(16) {}  // or any default size you prefer

void Set::operator=(int b) {
    resize(size()); // ensure size consistency
    for (size_t i = 0; i < size(); ++i) {
        (*this)[i] = static_cast<bool>(b);
    }
}

void Set::set(int pos) {
    if (size() <= static_cast<size_t>(pos)) resize(pos + 1);
    (*this)[pos] = true;
}

void Set::clear(int pos) {
    if (size() <= static_cast<size_t>(pos)) resize(pos + 1);
    (*this)[pos] = false;
}

int Set::isSet(int pos) {
    if (pos < 0 || static_cast<size_t>(pos) >= size()) return 0;
    return (*this)[pos] ? 1 : 0;
}
