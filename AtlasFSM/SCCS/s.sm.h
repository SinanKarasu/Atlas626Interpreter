h47537
s 00013/00000/00000
d D 1.1 01/01/29 16:31:23 sinan 1 0
c date and time created 01/01/29 16:31:23 by sinan
e
u
U
f e 0
t
T
I 1
#ifndef sm_h
#define sm_h

void GenStateMap();
void SetInitialState(char* initialStateName);
void PushHeader(char * header);
void PushSuperSubState(char * theStateName, char * superStateName);
void PushSuperState(char *theStateName);
void PushSubState(char * theStateName,char *  superStateName);
void PushState(char * theStateName);
void PushTransitionLine(char * trans, char * theStateName);
void PushAction(char * actionName);
#endif // sm_h
E 1
