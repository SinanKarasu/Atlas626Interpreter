h17995
s 00156/00159/00348
d D 1.2 01/01/29 16:28:33 sinan 3 1
c check in before Mark xfer
e
s 00000/00000/00000
d R 1.2 97/08/07 10:32:46 Codemgr 2 1
c SunPro Code Manager data about conflicts, renames, etc...
c Name history : 1 0 AtlasFSM/sm.c
e
s 00507/00000/00000
d D 1.1 97/08/07 10:32:45 sinan 1 0
c Initial check in of some State Machine stuff
e
u
U
f e 0
t
T
I 1
/* $Id: sm.c,v 1.8 1993/06/09 16:25:03 rmartin Exp $
/* ----------------------------------------------------------
/* Name
/*  sm.c
/*
/* Description
/*  This is the set of C functions called by the smc yacc parser.
/*  it writes the C++ program which controls the finite state
/*  machine.
/**/
/* Bugs
/*  This code is very ugly.  It has evloved over time.  It
/*  needs to be rewritten as a C++ program with typesafe objects
/*  and all.   (any takers?)
/**/

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <ctype.h>
I 3
#include <stdlib.h>
E 3

typedef enum {NO=0, YES=1} Bool;

extern char GfsmName[], GcontextName[], 
			Gversion[];

static char LinitialStateName[255];
static Bool LinitialStateSet = NO;


extern int lineNumber;


D 3
struct node
E 3
I 3
enum TheType {state, subState, superState, superSubState, 
	      transition, action};

struct Node
E 3
{
D 3
	struct node *link;
	enum {state, subState, superState, superSubState, 
	      transition, action} type;
E 3
I 3
	Node *link;
E 3
	char stateName[255];
I 3
	TheType type;
E 3
	char transitionName[255];
	char actionName[255];
	char superStateName[255];
D 3
	struct node* subStates;
	struct node* transitions;
	struct node* actions;
E 3
I 3
	Node* subStates;
	Node* transitions;
	Node* actions;
E 3
};
D 3
#define NONODE ((struct node*)0)
E 3
I 3
#define NONODE ((Node*)0)
E 3

struct nameNode
{
	struct nameNode *link;
D 3
	struct node *itsNode;
E 3
I 3
	Node *itsNode;
E 3
	char name[255];
};

D 3
struct node *stack = NULL;
struct node *actionList = NULL;
struct node *currentState = NULL;
struct node *currentTransition = 0;
struct node *currentAction = 0;
E 3
I 3
Node *stack = NULL;
Node *actionList = NULL;
Node *currentState = NULL;
Node *currentTransition = 0;
Node *currentAction = 0;
E 3

struct nameNode *tNameList = NULL;
struct nameNode *sNameList = NULL;
struct nameNode *hNameList = NULL;
FILE *cFile;
FILE *hFile;


D 3
struct node* FindState(theStateName)
char* theStateName;
E 3
I 3
Node* FindState(char *theStateName)
E 3
{
    struct nameNode* p;
D 3
    struct node* retval = 0;
E 3
I 3
    Node* retval = 0;
E 3
    for (p=sNameList; !retval && p; p=p->link)
    {
        if (strcmp(theStateName, p->name) == 0)
        {
            retval = p->itsNode;
        }
    }
    return retval;
}

D 3
main(ac,av)
int ac;
char **av;
E 3
I 3
int yyparse(void);

main(int ac,char ** av)
E 3
{
	lineNumber = 1;
	exit(yyparse());
}

D 3
yyerror(s)
char* s;
E 3
I 3
void yyerror(const char * s)
E 3
{
	printf("Line %d:%s\n",lineNumber,s);
}	

D 3
PushState(theStateName)
char* theStateName;
E 3
I 3

int yywrap()
E 3
{
D 3
	struct node *myNode = FindState(theStateName);
E 3
I 3
	return 1;
}	

void CheckNotSuperState(char * theStateName)
{
    Node* found = FindState(theStateName);
	if (found && 
            (found->type == superState || found->type == superSubState))
	{
	    printf("Line: %d. '%s' can't be a super state\n",
	            lineNumber, theStateName);
	    exit(1);
	}
}

void AddName(nameNode ** nameList, char * tName,Node * node)
//struct nameNode **nameList;
//char* tName;
//Node* node;
{
	struct nameNode *p;
	struct nameNode* found = 0;

	for (p=*nameList; p && !found; p=p->link)
	{
		if (strcmp(p->name, tName) == 0)
		{
			found = p;
		}
	}

	if (!found) /* didn't find it */
	{
		p = (struct nameNode *)malloc(sizeof(struct nameNode));
		p->link = *nameList;
		*nameList = p;
		strcpy(p->name, tName);
		p->itsNode = node;
	}
	else if (node != NONODE)
	{
	    if (found->itsNode == 0)
	    {
	        found->itsNode = node;
	    }
	    else if (found->itsNode != node)
	    {
	        printf("Line %d: '%s' redefined.\n", lineNumber, tName);
	        exit(1);
	    }
	}
}

void PushState(char * theStateName)
{
	Node *myNode = FindState(theStateName);
E 3
    CheckNotSuperState(theStateName);
    if (myNode == 0)
    {
D 3
		myNode = (struct node*) malloc(sizeof(struct node));
E 3
I 3
		myNode = (Node*) malloc(sizeof(Node));
E 3
		myNode->type = state;
		myNode->subStates = 0;
		myNode->transitions = 0;
		strcpy(myNode->stateName, theStateName);
		myNode->link = stack;
		stack = myNode;
		AddName(&sNameList, myNode->stateName, myNode);
	}
	currentState = myNode;
	currentTransition = 0;
}

D 3
PushSubState(theStateName, superStateName)
char* theStateName;
char* superStateName;
E 3
I 3
void CheckNotLeafState(char * theStateName)
E 3
{
D 3
	struct node *myNode = FindState(theStateName);
E 3
I 3
    Node* found = FindState(theStateName);
	if (found &&  
	   (found->type == state || found->type == subState))
	{
	    printf("Line: %d. '%s' can't be a target of a transition.\n",
	            lineNumber, theStateName);
	    exit(1);
	}
}

void PushSubState(char * theStateName,char *  superStateName)
{
	Node *myNode = FindState(theStateName);
E 3
	CheckNotSuperState(theStateName);
	CheckNotLeafState(superStateName);

	if (myNode == 0)
	{
D 3
		struct node *superNode;
		myNode = (struct node*) malloc(sizeof(struct node));
E 3
I 3
		Node *superNode;
		myNode = (Node*) malloc(sizeof(Node));
E 3
		myNode->subStates = 0;
		myNode->transitions = 0;
		superNode = FindState(superStateName);
		if (!superNode) 
		{
			printf("Line %d: '%s' not defined\n", 
				   lineNumber, superStateName);
			exit(1);
		}
		myNode->type = subState;
		strcpy(myNode->stateName, theStateName);
		strcpy(myNode->superStateName, superStateName);
		myNode->link = superNode->subStates;
		superNode->subStates = myNode;
		AddName(&sNameList, myNode->stateName, myNode);
	}
	currentState = myNode;
	currentTransition = 0;
}

D 3
PushSuperState(theStateName)
char* theStateName;
E 3
I 3
void PushSuperState(char *theStateName)
E 3
{
D 3
	struct node *myNode = FindState(theStateName);
E 3
I 3
	Node *myNode = FindState(theStateName);
E 3
    CheckNotLeafState(theStateName);
	if (myNode == 0)
	{
D 3
		myNode = (struct node*) malloc(sizeof(struct node));
E 3
I 3
		myNode = (Node*) malloc(sizeof(Node));
E 3
		myNode->subStates = 0;
		myNode->transitions = 0;
		myNode->type = superState;
		strcpy(myNode->stateName, theStateName);
		myNode->link = stack;
		stack = myNode;
		AddName(&sNameList, myNode->stateName, myNode);
	}
	currentState = myNode;
	currentTransition = 0;
}

D 3
PushSuperSubState(theStateName, superStateName)
char* theStateName;
char* superStateName;
E 3
I 3
void PushSuperSubState(char * theStateName, char * superStateName)
E 3
{
D 3
	struct node *myNode = FindState(theStateName);
E 3
I 3
	Node *myNode = FindState(theStateName);
E 3
	CheckNotLeafState(theStateName);
	CheckNotLeafState(superStateName);
	if (myNode == 0)
	{
D 3
		struct node *superNode;
		myNode = (struct node*) malloc(sizeof(struct node));
E 3
I 3
		Node *superNode;
		myNode = (Node*) malloc(sizeof(Node));
E 3
		myNode->subStates = 0;
		myNode->transitions = 0;
		myNode->type = superSubState;
		superNode = FindState(superStateName);
		if (!superNode) 
		{
			printf("Line %d: '%s' not defined\n", 
				   lineNumber, superStateName);
			exit(1);
		}
		strcpy(myNode->stateName, theStateName);
		strcpy(myNode->superStateName, superStateName);
		myNode->link = superNode->subStates;
		superNode->subStates = myNode;

		AddName(&sNameList, myNode->stateName, myNode);
	}
	currentState = myNode;
	currentTransition = 0;
}

D 3
SetInitialState(char* initialStateName)
E 3
I 3
void SetInitialState(char* initialStateName)
E 3
{
    strcpy(LinitialStateName, initialStateName);
    LinitialStateSet = YES;
    AddName(&sNameList, initialStateName, NONODE);
}

D 3
PushTransitionLine(trans, theStateName)
char *trans, *theStateName;
E 3
I 3
void PushTransitionLine(char * trans, char * theStateName)
E 3
{
D 3
	struct node *myNode = (struct node*) malloc(sizeof(struct node));
E 3
I 3
	Node *myNode = (Node*) malloc(sizeof(Node));
E 3
    if (currentState == 0)
    {
    	printf("Line %d: No current state for '%s'\n", trans);
    }
	myNode->actions = 0;
	myNode->type = transition;
	strcpy(myNode->transitionName, trans);
	strcpy(myNode->stateName, theStateName);
	myNode->link = currentState->transitions;
	currentState->transitions = myNode;

	AddName(&tNameList, trans, NONODE);
	CheckNotSuperState(myNode->stateName);
	AddName(&sNameList, myNode->stateName, NONODE);
	currentTransition = myNode;
	currentAction = 0;
}

D 3
PushHeader(header)
char *header;
E 3
I 3
void PushHeader(char * header)
E 3
{
	AddName(&hNameList, header, NONODE);
}

D 3
PushAction(actionName)
char* actionName;
E 3
I 3
void PushAction(char * actionName)
E 3
{
D 3
	struct node *myNode = (struct node*) malloc(sizeof(struct node));
E 3
I 3
	Node *myNode = (Node*) malloc(sizeof(Node));
E 3
	myNode->type = action;
	myNode->link = 0;
	strcpy(myNode->actionName, actionName);
	if (currentAction == 0)
	{
		currentTransition->actions = myNode;
	}
	else
	{
		currentAction->link = myNode;
	}
	currentAction = myNode;
}

D 3
AddName(nameList, tName, node)
struct nameNode **nameList;
char* tName;
struct node* node;
{
	struct nameNode *p;
	struct nameNode* found = 0;
E 3

D 3
	for (p=*nameList; p && !found; p=p->link)
E 3
I 3
void DumpStateHeaders(Node * theNode)
{
    Node* t;
	fprintf(hFile, "\n");
	if (theNode->type == state || theNode->type == superState)
E 3
	{
D 3
		if (strcmp(p->name, tName) == 0)
		{
			found = p;
		}
E 3
I 3
		fprintf(hFile, "class %s%sState : public %sState {\n",
			GfsmName, theNode->stateName,  GfsmName);
E 3
	}
D 3

	if (!found) /* didn't find it */
E 3
I 3
	else if (theNode->type == subState || theNode->type == superSubState)
E 3
	{
D 3
		p = (struct nameNode *)malloc(sizeof(struct nameNode));
		p->link = *nameList;
		*nameList = p;
		strcpy(p->name, tName);
		p->itsNode = node;
E 3
I 3
		fprintf(hFile, "class %s%sState : public %s%sState {\n",
			GfsmName, theNode->stateName,  
			GfsmName, theNode->superStateName);
E 3
	}
D 3
	else if (node != NONODE)
E 3
I 3
	fprintf(hFile, "public:\n");
	if (theNode->type == state || theNode->type == subState)
E 3
	{
D 3
	    if (found->itsNode == 0)
	    {
	        found->itsNode = node;
	    }
	    else if (found->itsNode != node)
	    {
	        printf("Line %d: '%s' redefined.\n", lineNumber, tName);
	        exit(1);
	    }
E 3
I 3
		fprintf(hFile, 
			"  virtual const char* StateName() const\n");
		fprintf(hFile, 
			"  {return(\"%s\");};\n", theNode->stateName);
E 3
	}
D 3
}
E 3

D 3
CheckNotSuperState(theStateName)
char* theStateName;
{
    struct node* found = FindState(theStateName);
	if (found && 
            (found->type == superState || found->type == superSubState))
E 3
I 3
	for (t = theNode->transitions; t; t=t->link)
E 3
	{
D 3
	    printf("Line: %d. '%s' can't be a super state\n",
	            lineNumber, theStateName);
	    exit(1);
E 3
I 3
		fprintf(hFile, "  virtual void %s(%s&);\n", 
			t->transitionName,GfsmName); 
E 3
	}
I 3
	fprintf(hFile, "};\n");

	if (theNode->subStates) DumpStateHeaders(theNode->subStates);
	if (theNode->link) DumpStateHeaders(theNode->link);
E 3
}

D 3
CheckNotLeafState(theStateName)
char* theStateName;
E 3
I 3
void DumpStateImplementations(Node * theNode)
E 3
{
D 3
    struct node* found = FindState(theStateName);
	if (found &&  
	   (found->type == state || found->type == subState))
	{
	    printf("Line: %d. '%s' can't be a target of a transition.\n",
	            lineNumber, theStateName);
	    exit(1);
	}
E 3
I 3
    Node* t = 0;
    for (t=theNode->transitions; t; t=t->link)
    {
        Node* a = 0;
		fprintf(cFile, "void %s%sState::%s(%s& s) {\n",
			GfsmName, theNode->stateName, t->transitionName, GfsmName);
		fprintf(cFile, "  s.SetState(%s::%sState);\n", 
						GfsmName, t->stateName);
		for (a=t->actions; a; a=a->link)
		{
			fprintf(cFile, "  s.%s();\n", a->actionName);
		}
		fprintf(cFile, "}\n");
    }
	if (theNode->subStates) DumpStateImplementations(theNode->subStates);
	if (theNode->link) DumpStateImplementations(theNode->link);
E 3
}

D 3
GenStateMap()
E 3
I 3

void GenStateMap()
E 3
{
	char cName[255];
	char hName[255];
D 3
	struct node *myNode;
E 3
I 3
	Node *myNode;
E 3
	struct nameNode *tName;
	struct nameNode *sName;
	struct nameNode *hdrName;
D 3
	struct node* reverseList = NULL;
E 3
I 3
	Node* reverseList = NULL;
E 3
	Bool inState = NO;
	Bool inTransition = NO;
	char currentState[255];
	char contextHeader[256];

	sprintf(cName, "%s.cc", GfsmName);
	sprintf(hName, "%s.h", GfsmName);
	/*cName[0] = tolower(cName[0]);*/
	/*hName[0] = tolower(hName[0]);*/

	cFile = fopen(cName, "w");
	if (cFile == NULL)
	{
		perror(cName);
		exit(1);
	}

	hFile = fopen(hName, "w");
	if (hFile == NULL)
	{
		perror(hName);
		exit(1);
	}

	fprintf(hFile, "#ifndef _H_%s\n#define _H_%s\n",
		GfsmName, GfsmName);

	fprintf(hFile, "#include <stddef.h>\n");

	for (hdrName=hNameList; hdrName; hdrName=hdrName->link)
	{
		fprintf(hFile, "#include \"%s\"\n", hdrName->name);
	}

    fprintf(hFile, "class %s;\n", GfsmName);
	fprintf(hFile, "\n");
	fprintf(hFile, "class %sState {\n", 
								GfsmName);
	fprintf(hFile, "public:\n");

	fprintf(hFile, "\n");
	fprintf(hFile, "  virtual const char* StateName() const = 0;\n");

	for (tName=tNameList; tName; tName=tName->link)
	{
		fprintf(hFile, "  virtual void %s(%s& s);\n", tName->name,
				GfsmName);
	}
		
	fprintf(hFile, "};\n");

    DumpStateHeaders(stack);

    fprintf(hFile, "class %s : public %s {\n", GfsmName, GcontextName);
    fprintf(hFile, "  public:\n");

	for (sName=sNameList; sName; sName=sName->link)
	{
	    if (sName->itsNode->type == state ||
	        sName->itsNode->type == subState)
	    {
			fprintf(hFile, "  static %s%sState %sState;\n",
				GfsmName, sName->name, sName->name);
		}
	}
    if (LinitialStateSet)
    {
		fprintf(hFile, "  %s();// default constructor\n", GfsmName);
	}

	for (tName=tNameList; tName; tName=tName->link)
	{
		fprintf(hFile, "  void %s() {itsState->%s(*this);}\n",
		               tName->name, tName->name);
	}

    fprintf(hFile, "  void SetState(%sState& theState) {itsState=&theState;}\n",
                   GfsmName);
    fprintf(hFile, "  %sState& GetState() const {return *itsState;};\n",
                    GfsmName);
	fprintf(hFile, "  private:\n");
	fprintf(hFile, "    %sState* itsState;\n", GfsmName);
	fprintf(hFile, "};\n");

	fprintf(hFile, "#endif\n");
	fclose(hFile);
	inState = NO;

	fprintf(cFile, "#include \"%s\"\n", hName);

	fprintf(cFile, "static char _versID[] = \"%s\";\n", Gversion);

	for (sName=sNameList; sName; sName=sName->link)
	{
	    if (sName->itsNode->type == state ||
	        sName->itsNode->type == subState)
	    {
		    fprintf(cFile, "%s%sState %s::%sState;\n", 
			    GfsmName, sName->name, GfsmName, sName->name);
		}
	}

	for (tName=tNameList; tName; tName=tName->link)
	{
		fprintf(cFile, "void %sState::%s(%s& s)\n", 
		        GfsmName, tName->name, GfsmName);
		fprintf(cFile, 
			"  {s.FSMError(\"%s\", s.GetState().StateName());}\n",
			tName->name);
	}

	DumpStateImplementations(stack);

	if (LinitialStateSet)
	{
	    fprintf(cFile, "%s::%s() : itsState(&%sState) {}\n", 
	            GfsmName, GfsmName, LinitialStateName);
	}
}

D 3
DumpStateHeaders(theNode)
struct node *theNode;
{
    struct node* t;
	fprintf(hFile, "\n");
	if (theNode->type == state || theNode->type == superState)
	{
		fprintf(hFile, "class %s%sState : public %sState {\n",
			GfsmName, theNode->stateName,  GfsmName);
	}
	else if (theNode->type == subState || theNode->type == superSubState)
	{
		fprintf(hFile, "class %s%sState : public %s%sState {\n",
			GfsmName, theNode->stateName,  
			GfsmName, theNode->superStateName);
	}
	fprintf(hFile, "public:\n");
	if (theNode->type == state || theNode->type == subState)
	{
		fprintf(hFile, 
			"  virtual const char* StateName() const\n");
		fprintf(hFile, 
			"  {return(\"%s\");};\n", theNode->stateName);
	}
E 3

D 3
	for (t = theNode->transitions; t; t=t->link)
	{
		fprintf(hFile, "  virtual void %s(%s&);\n", 
			t->transitionName,GfsmName); 
	}
	fprintf(hFile, "};\n");
E 3

D 3
	if (theNode->subStates) DumpStateHeaders(theNode->subStates);
	if (theNode->link) DumpStateHeaders(theNode->link);
}

DumpStateImplementations(theNode)
struct node* theNode;
{
    struct node* t = 0;
    for (t=theNode->transitions; t; t=t->link)
    {
        struct node* a = 0;
		fprintf(cFile, "void %s%sState::%s(%s& s) {\n",
			GfsmName, theNode->stateName, t->transitionName, GfsmName);
		fprintf(cFile, "  s.SetState(%s::%sState);\n", 
						GfsmName, t->stateName);
		for (a=t->actions; a; a=a->link)
		{
			fprintf(cFile, "  s.%s();\n", a->actionName);
		}
		fprintf(cFile, "}\n");
    }
	if (theNode->subStates) DumpStateImplementations(theNode->subStates);
	if (theNode->link) DumpStateImplementations(theNode->link);
}


E 3


E 1
