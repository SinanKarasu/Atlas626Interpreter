#ifndef Set_H
#define Set_H

class Set:public RWBitVec
{
public:

	Set();
	void operator=(int b);
		
	void set(int pos);
	void clear(int pos);
	int isSet(int pos);
};

#endif // Set_H

