#include	"TedlCompAST.h"

	TedlCompAST::TedlCompAST (AST * a ){}
	TedlCompAST::~TedlCompAST(){}

AST *	TedlCompAST::eval	( AST * a  ){assert(0);return 0;}

        IfTedl::IfTedl(AST * a):TedlCompAST(a){;};
        IfTedl::~IfTedl(){};
AST *	IfTedl::eval(AST * a)
		{
				
			AST * target=ASTright()->eval();// points to Skip
			AST * cond=ASTdown()->eval(a);
			if(cond->getInteger(0)){
				return ASTright()->ASTright();// execute block
			} else {
				return target->ASTright();// ask Else....
			}
		}
			
        ElseIfTedl::ElseIfTedl(AST * a):TedlCompAST(a){;};
        ElseIfTedl::~ElseIfTedl(){};
AST *	ElseIfTedl::eval(AST * a)
		{
			AST * target=ASTright()->eval();// points to Skip
			if(a==0){	//we are getting out
				return target;
			} else {
				AST * cond=ASTdown()->eval(a);
				if(cond->getInteger(0)){ //execute
					return ASTright()->ASTright();
				} else { // ask next for eval
					return target->ASTright();
				}
			}
		}

        ElseTedl::ElseTedl(AST * a):TedlCompAST(a){;};
        ElseTedl::~ElseTedl(){};
AST *	ElseTedl::eval(AST * a)
		{
			AST * target=ASTright()->eval();// points to Skip
			if(a==0){
				return target;
			} else {
				return ASTright()->ASTright();
			}
		}


        EndIfTedl::EndIfTedl(AST * a):TedlCompAST(a){;};
        EndIfTedl::~EndIfTedl(){};
AST *	EndIfTedl::eval(AST * a)
		{
			return ASTright();
		}
  
        SkipTedl::SkipTedl(AST * a):TedlCompAST(a){;};
        SkipTedl::~SkipTedl(){};
AST *	SkipTedl::eval(AST * a)
		{
			return ASTright()->eval(0);
		}

        TargetTedl::TargetTedl(AST * a):TedlCompAST(a),m_target(a){;};
        TargetTedl::~TargetTedl(){};
AST *	TargetTedl::eval(AST * a)
		{
			return m_target;
		}

        ExecuteTedl::ExecuteTedl(AST * a):TedlCompAST(a){;};
        ExecuteTedl::~ExecuteTedl(){};
AST *	ExecuteTedl::eval(AST * a)
		{
			AST *	root = ASTdown();
		
			while ( root  &&  root != 0 )
				root = root->eval( a );
			return 0;
		}
		
		


