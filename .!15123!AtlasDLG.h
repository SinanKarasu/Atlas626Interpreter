#ifndef AtlasDLG_h
#define AtlasDLG_h
/*
 * D L G L e x e r  C l a s s  D e f i n i t i o n
 *
 * Generated from: AtlasDLG.dlg
 *
 * 1989-2001 by  Will Cohen, Terence Parr, and Hank Dietz
 * Purdue University Electrical Engineering
 * DLG Version 1.33MR33
 */


#include "DLexerBase.h"



#include    "NestedTokenStream.h"

  

class AtlasDLG : public DLGLexerBase {
public:


typedef void (AtlasDLG::*Func)(void);

	  


NestedTokenStream           *nestedTokenStream;
const char *                filename;
virtual _ANTLRTokenPtr      getToken();

int sawFStatno ;   

virtual void newline()  { _line++;set_endcol(0); }   


//--------------------------------------------------------------------
enum	{MaxModeStack=10};
int	modeStack[MaxModeStack];
int 	stackDepth;
Func	funcStack[MaxModeStack];
int	homeState;
//--------------------------------------------------------------------
void	pushMode(int newMode,Func func)
{
  if(stackDepth == (MaxModeStack - 1) ) {
    panic("Mode stack overflow ");
  } else {
    modeStack[stackDepth] = automaton;	// getMode() in next version
    funcStack[stackDepth] = func;
    stackDepth++;
    mode(newMode);
  }
}
//--------------------------------------------------------------------
void	pushMode(int newMode)
{
  if(stackDepth == (MaxModeStack - 1) ) {
    panic("Mode stack overflow ");
  } else {
    modeStack[stackDepth] = automaton;	// getMode() in next version
    funcStack[stackDepth] = 0;
    stackDepth++;
    mode(newMode);
  }
}
//--------------------------------------------------------------------
void	popMode()
{
