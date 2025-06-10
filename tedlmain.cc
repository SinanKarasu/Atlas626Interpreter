
#include	"AtlasAST.h"
#include	"TedlStd.h"
#include	"TedlParser.h"
#include	"AtlasDLG.h"
#include	"TedlLexer.h"


#include	"PBlackBox.h"
#include	"NestedParserBlackBox.h"


//Resource * currentResource;

ReverseMap * reverseMap;

void
TedlParser::init()
	{
		ANTLRParser::init();
	}

 class MultiFiles : public DLGFileInput
 {
 	MultiFiles(FILE * f):DLGFileInput(f){};
 };
 


class TedlBox:public NestedParserBlackBox<TedlLexer,TedlParser,AtlasToken> 
	{
		public:
		TedlBox(const std::string& f):NestedParserBlackBox<TedlLexer,TedlParser,AtlasToken>(f){}
	};


TedlBox * tedl_p=0;

int tedlmain(  RWCString model ,const RWCString & dbd )
{
	extern ASTBase		*	TEDLroot;
	extern TedlParser	*	TEDL;
	
	if(!TEDL){
		try{
			tedl_p = new TedlBox(model);
		}
		catch ( FileNotFound fnf) {
			std::cerr << " Tedl Model File " << fnf << " Not found" << std::endl;
			return -1;
		} 
		
		TEDL = tedl_p->parser();
		TEDL->tedl_initialization(&TEDLroot,dbd);
		reverseMap=new ReverseMap();
			
	} else {
		try{
			tedl_p->ResetFile(model);
		}
		catch ( FileNotFound fnf) {
			std::cerr << " Model File " << fnf << " Not found" << std::endl;
			return -1;
		} 
	}

	// should be nothing to execute from tedl.
	// so we make sure of that...
	// However later on if we put ATE initialization section,
	// then take out the following assert.
	// Actually this should not be done here, but in main, after all the
	// tedl files have been read in....
	assert(TEDLroot==0);
	TEDL->tedl(&TEDLroot,model);
	
	return 0;
}
