
#include	"VerbAST.h"
#include	"Visitors.h"
#include	"ExceptionTypes.h"

	VerbAST::VerbAST(ANTLRTokenPtr p):AST(p){;};
	VerbAST::~VerbAST(){};
AST *	VerbAST::eval(AST * a)
		{
			if(ASTdown()){
				ASTdown()->eval();
				if(ASTdown()->ASTright()){
					ASTdown()->ASTright()->eval();
				}
			}
			return 0;
		}

AST *	VerbAST::Accept	( ASTVisitor & v )	{ return v.VisitVerbAST( this ); }

	CalculateVerb::CalculateVerb(ANTLRTokenPtr p):VerbAST(p){;};

        IfThenVerb::IfThenVerb(ANTLRTokenPtr p):VerbAST(p){;};
AST *	IfThenVerb::eval(AST * a)
		{
				
			AST * target=ASTright()->eval();
			if(a==this){	// someone asked us to skip
					// to the end (LEAVE IF ?)
				return target->eval();	// means don't execute ELSE clause
			} else {
				AST * cond=ASTdown()->eval();
				if(cond->getInteger(0)){
					return ASTright()->ASTright();// execute block
				} else {
					return target->eval(target);// ask else
				}
			}
		}
			
        ElseIfThenVerb::ElseIfThenVerb(ANTLRTokenPtr p):VerbAST(p){;};
AST *	ElseIfThenVerb::eval(AST * a)
		{
			AST * target=ASTright()->eval();
			if(a==this){// we are being asked
				AST * cond=ASTdown()->eval();
				if(cond->getInteger(0)){ //execute
					return ASTright()->ASTright();
				} else { // ask next for eval
					return target->eval(target);
				}
			} else {// prev if executed
				return target;
			}
		}

        ElseVerb::ElseVerb(ANTLRTokenPtr p):VerbAST(p){;};
AST *	ElseVerb::eval(AST * a)
		{
			if(a==this){
				return ASTright()->ASTright();
			} else {
				return ASTright()->eval();
			}
		}


        EndIfVerb::EndIfVerb(ANTLRTokenPtr p):VerbAST(p){;};
AST *	EndIfVerb::eval(AST * a)
		{
			return ASTright();
		}
  
        WhileThenVerb::WhileThenVerb(ANTLRTokenPtr p):VerbAST(p){;};
AST *	WhileThenVerb::eval(AST * a)
		{	AST *cond,*target;
				
			cond=ASTdown()->eval();
			target=ASTright()->eval();
			if(a==this){	// someone asked us to skip
					// to the end (LEAVE WHILE ?)
				return target->eval();
			} else if(cond->getInteger(0)){
				return ASTright()->ASTright();// execute block
			} else {
				return target->eval();//  EndWhile
			}
		}
	
        EndWhileVerb::EndWhileVerb(ANTLRTokenPtr p):VerbAST(p){;};
AST *	EndWhileVerb::eval(AST * a)
		{
			return ASTright();
		}
 

//  !no ForThenVerb Loop is constructed Thusly:
//
//  !no ForThenVerb                 ( ast=0       --> execute loop )
//                              ( ast=this    --> return ASTright()->ASTright()
//  !no ForThenEntryAction  
//				( ast=this    --> init and return) 
//				( ast=0       --> evaluate and return target)
//  !no TargetAction (EndForVerb)
//  !no    body
//  !no TargetAction (ForThenEntryAction)
//  !no EndForVerb 


// ForThenVerb Loop is constructed Thusly:
//
//  ForThenVerb                 ( ast==0       --> evaluate and execute loop )
//                              ( ast==this    --> terminate loop )
//                              ( ast!=this    --> next iteration of loop )
//  TargetAction (EndForVerb)
//     body
//  TargetAction (ForThenEntryAction)
//  EndForVerb 



// ForThenVerb Loop is constructed Thusly:
//
//  ForThenVerb                 ( ast==0       --> evaluate and execute loop )
//                              ( ast==this    --> terminate loop )
//                              ( ast!=this    --> next iteration of loop )
//  TargetAction (EndForVerb)
//     body
//  TargetAction (ForThenEntryAction)
//  EndForVerb 

        ForThenVerb::ForThenVerb(ANTLRTokenPtr p):VerbAST(p){;};
AST *	ForThenVerb::eval(AST * a)
		{
			if(a==0){
				return ASTright()->eval(ASTright());
			} else {
				return ASTright()->ASTright(); // we are out of here
			}
		}
			

        EndForVerb::EndForVerb(ANTLRTokenPtr p):VerbAST(p){;};
AST *	EndForVerb::eval(AST * a)
		{
			return ASTright();
		}
 
        RequireSignalVerb::RequireSignalVerb(ANTLRTokenPtr p):VerbAST(p){;};
AST *	RequireSignalVerb::eval(AST * a)
		{
			return VerbAST::eval(a);

		}

CompareVerb::CompareVerb( Scope * s ):VerbAST(0),m_scope(s)
        {
        }
       
AST *
CompareVerb::eval(AST * a)
		{
			VerbAST::eval(a);

			extern astream sout;

			sout << m_scope ;
			sout << "COMPARE";
			sout << ",";
			sout << ASTdown() ;
			sout << std::endl;

			//throw PrintEvaluationRequest("COMPARE",ASTdown());
			return 0;
		}


        ReadTimerVerb::ReadTimerVerb(ANTLRTokenPtr p):VerbAST(p){;};
AST *	ReadTimerVerb::eval(AST * a)
		{
			ASTdown()->ASTright()->assign(ASTdown());
			return 0;
		}


        ResetTimerVerb::ResetTimerVerb(ANTLRTokenPtr p)
        	:VerbAST(p),m_timers(0){;};
        ResetTimerVerb::ResetTimerVerb(ASTList * allTimers)
        	:VerbAST(0),m_timers(allTimers){;};
AST *	ResetTimerVerb::eval(AST * a)
		{
			if(m_timers){
				//ASTListIterator timit(*m_timers);
				for (const auto &timit : *m_timers) {
				//while(++timit){
					timit->init();
				}
			} else {
				AST * t=ASTdown();
				while(t){
					t->init();
					t=t->ASTright();
				}
			}
			return 0;
		}


//--- DataBus---------------------------------------------//
