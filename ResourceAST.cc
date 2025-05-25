#include	"AtlasAST.h"
#include	"ResourceAST.h"


ResourceAST::ResourceAST(ANTLRTokenPtr p):AST(p){}

int ResourceAST::checkSubsume(AST * subSet,AST * supSet)
	{

		AST * rck = supSet;	// ->ASTdown();
		AST *  ck = subSet;
		int field=0;
		while(ck) {
			field++;
			if(!(ck->check(rck))){ 
				// Field is not covered by requirement
				// Annotate...
				return 0;
			}
			rck = rck->ASTright();
			 ck =  ck->ASTright();
			if(field >=2) break;
		}
		

		return 1;
	}
	

