h37113
s 00002/00000/00126
d D 1.2 01/01/29 16:28:34 sinan 3 1
c check in before Mark xfer
e
s 00000/00000/00000
d R 1.2 97/08/07 10:32:48 Codemgr 2 1
c SunPro Code Manager data about conflicts, renames, etc...
c Name history : 1 0 AtlasFSM/smy.y
e
s 00126/00000/00000
d D 1.1 97/08/07 10:32:47 sinan 1 0
c Initial check in of some State Machine stuff
e
u
U
f e 0
t
T
I 1
%{
/* $Id: smy.y,v 1.6 1993/06/09 16:25:03 rmartin Exp $

/*-------------------------------------------------
/*Name
/*	sm.y -- Parse StateMap files to build C++ state classes.
/*
/*Description
/*	This is a YACC grammar for the specification of StateMaps which
/*	are converted into C++ programs.
/*
/*See Also
/*	sm.l
/*
/*Author(s)
/*	R. Martin
/*
/*Date/History
/*  30 May 93 0949 rm
/*  Change to "Context Derived" form.
/*
/*	27 Jun 91 0951 rm
/*
/**/


#include <string.h>
#include <stdio.h>
I 3
#include "sm.h"

E 3
char GfsmName[255];
char GcontextName[255];
char Gversion[256] = "No Version.";
%}

%start record
%union	{
		char s[255];
		}

%token <s> WORD
%token <s> C_VERSION
%token ERROR
%token C_FSMNAME
%token C_CONTEXT
%token C_HEADER
%token C_INITIAL

%% /* begin the rules */

record		:	declarators '{' stateBodies '}' 
					{
						GenStateMap();
					}
			;

declarators	:	
			| declarators declarator
			;

declarator	:	C_FSMNAME WORD
				{
					strcpy(GfsmName, $2);
				}
			|	C_CONTEXT WORD
				{
					strcpy(GcontextName, $2);
				}
			|   C_INITIAL WORD
			    {
			       SetInitialState($2); 
			    }
			|	C_HEADER WORD
				{
					PushHeader($2);
				}
			|	C_VERSION
				{
					strcpy(Gversion, $1);
					/* get rid of end of line */
					*(strchr(Gversion, '\n')) = 0;
				}
			;

stateBodies : 
			|	stateBodies stateBody
			;

stateBody 	:	state_definition transition_set
			;

state_definition : '(' WORD ')' ':' WORD {PushSuperSubState($2,$5);}
                 | '(' WORD ')' {PushSuperState($2);}
                 | WORD ':' WORD {PushSubState($1,$3);}
                 | WORD {PushState($1);}
                 ;

transition_set : transition_line
			| '{' transition_lines '}'
			;

transition_lines : 
			|	transition_lines transition_line
			;

transition_line : WORD WORD {PushTransitionLine($1,$2);} action_set
			|	'*'
			;

action_set 	:	action_line
			| 	'{' action_lines '}'
			;

action_lines :	
			|	action_lines action_line
			;

action_line	:	WORD
					{
						PushAction($1);
					}
			;

%%




E 1
