
#include	"AtlasAST.h"
#include	"AtlasStd.h"
#include	"AtlasParser.h"
#include	"AtlasDLG.h"
#include	"Visitors.h"

#include	"AtlasBox.h"


#include	"DebugEnv.h"


AtlasBox * atlas_p=0;
DebugEnv * debugEnv=0;

AST *
atlasmain( RWCString atlas ,const RWCString & dbd,int mode )
{	
	ASTBase *	ATLASroot = 0;
	AST *		Root;
	AST *		PrevRoot=0;
	int		_error_();
	
	evalVisitor	go;
	
	if(atlas_p==0){
		try{
			atlas_p = new AtlasBox(atlas.c_str());
			debugEnv=new DebugEnv(atlas_p);
		}
		catch ( FileNotFound fnf) {
			std::cerr << " Atlas TPS File " << fnf << " Not found" << std::endl;
			return 0;
		} 
		//atlas_p->getLexer()->debugLexer(1);
		atlas_p->parser()->atlas_initialization( &ATLASroot,dbd );
		if(ATLASroot)assert(0);
	} else {
		try{
			atlas_p->ResetFile(atlas.c_str());
		}
		catch ( FileNotFound fnf) {
			std::cerr << " Atlas File " << fnf << "Not found" << std::endl;
			return 0;
		} 
	}

	if(mode==0){
		atlas_p->parser()->atlas_program_structure( &ATLASroot ,atlas);
	} else if(mode==1){
		atlas_p->parser()->atlas_module_structure( &ATLASroot ,atlas);
	}
	
	atlas_p->parser()->done();

	Root=(AST *)ATLASroot;

	
	//Root->preorder();
	//if ( ! _error_() ) go.Execute( Root, 0 );
	if ( ! _error_() ) {
		return Root;
	} else {
		return 0;
	}

//	AST * x=debugEnv->evaluateExpression(12,RWCString("'d'"));

//////// debugger example ///////////////////////////////////////////////////
//	atlas_p->ResetString("'X'='X'+2$",AtlasDLG::StatementMode);
//	ATLASroot=0;
//	LineInfo * x;
//	lineInfoList.findValue(4,x);
//	atlas_p->parser()->setScope(x->getScope());
//	atlas_p->parser()->integer_assignment_expression(&ATLASroot);
	
//	Root=(AST *)ATLASroot;
//	Root->eval();
//	cout << " And the result is!!! " <<  Root->getInteger() << endl;
/////////////////////////////////////////////////////////////////////////////

	return 0;
}
