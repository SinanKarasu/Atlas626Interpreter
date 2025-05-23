h56346
s 00001/00002/00087
d D 1.4 01/01/29 16:37:17 sinan 5 4
c warning removal
e
s 00002/00001/00087
d D 1.3 01/01/29 16:36:34 sinan 4 3
c clean up
e
s 00001/00000/00087
d D 1.2 01/01/29 16:28:34 sinan 3 1
c check in before Mark xfer
e
s 00000/00000/00000
d R 1.2 97/08/07 10:32:46 Codemgr 2 1
c SunPro Code Manager data about conflicts, renames, etc...
c Name history : 1 0 AtlasFSM/sml.l
e
s 00087/00000/00000
d D 1.1 97/08/07 10:32:45 sinan 1 0
c Initial check in of some State Machine stuff
e
u
U
f e 0
t
T
I 1
%{
/* $Id: sml.l,v 1.5 1993/06/09 16:25:03 rmartin Exp $
/*--------------------------------------------------------
/*Name
/*	sm.l -- Lexical functions for parsing StateMap files..
/*
/*Description
/*	This LEX file specifies the lexical analysis necessary for the
/*	tokenization of Statemap specification files.
/*
/*See Also
/*	sm.y
/*
/*Author(s)
/*	R. Martin
/*
/*Date/History
/*  30 May 93 0942
/*    Change to "Context Derived" form.
/*
/*	27 Jun 91 0936
/**/


#include "smy.h"
I 3
#include "string.h"
E 3

extern YYSTYPE yylval;
int lineNumber;

%}
D 5

D 4
%% /* start the lexical rules */
E 4
I 4
%%
E 5
/* start the lexical rules */
I 5
%%
E 5
E 4

FSMNAME			|
fsmname 		|
FSMName 		|
FsmName 		{
				return(C_FSMNAME);
				}

CONTEXT			|
context			|
Context			{
				return(C_CONTEXT);
				}

INITIAL         |
Initial         |
initial         {
                return(C_INITIAL);
                }

HEADER			|
Header			|
header			{
				return(C_HEADER);
				}

Version.*\n		|
version.*\n		|
VERSION.*\n		{
				strcpy(yylval.s, yytext);
				lineNumber++;
				return(C_VERSION);
				}

[{}\*():]		{
				return(yytext[0]);
				}

[A-Za-z][A-Za-z0-9._]*	{
				strcpy(yylval.s, yytext);
				return(WORD);
				}

\/\/.*\n		|
\n				{
				lineNumber++;
				}

\/\*.*\*\/		|
\t				|
" "				;

%%


E 1
