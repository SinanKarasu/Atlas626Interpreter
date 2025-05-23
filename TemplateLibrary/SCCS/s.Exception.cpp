h24614
s 00013/00000/00000
d D 1.1 01/01/29 16:26:32 sinan 1 0
c date and time created 01/01/29 16:26:32 by sinan
e
u
U
f e 0
t
T
I 1
#include "Exception.h"
#include <stdlib.h>
#include <iostream>

void
EXCEPTION( int Condition, const char *ErrMsg )
{
    if( Condition )
    {
        cerr << "Exception: " << ErrMsg << endl;
        abort( );
    }
}
E 1
