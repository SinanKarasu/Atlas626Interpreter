
#include	<fstream>
#include	"NounsModifiersDimensions.h"
//#include	"Globals.h"
#include	"TedlDictionary.h"
#include	"Visitors.h"

#include	<dlfcn.h>
#include	<libgen.h>
#include	"astream.h"
#include	"ExecEnv.h"

TedlSymbolDictionary	deviceEquivalence;

TedlSymbolDictionary	monitorEquivalence;

ExecEnv execEnv;

std::ofstream debugtrace;
astream sout;
class TedlParser;
TedlParser *	TEDL=0;
ASTBase		*	TEDLroot = 0;

extern int G_Equivalence_insantiated;

class TpsContext {
public:

	RWCString	AteConfiguration;
	StringList	AdapterConfiguration;
	StringList	AtlasModules;
	RWCString	UutPartNo;
	RWCString	AtlasTps;
	RWCString	DeviceDriver;

};


RWCString getConfStr(std::ifstream & conf){
	RWCString x;
	if(!conf.eof()){
		conf >> x;
		return x;
	} else {
		return "";
	}
}


RWCString getQuotedConfStr(std::ifstream & conf){
	RWCString x;
	char ch;
	
	while(!conf.eof() && conf.peek()==' ')conf.get(ch);
	if(!conf.eof() && conf.peek()=='\''){
		conf.get(ch);x+=ch;
		while(!conf.eof() && conf.get(ch)){
			x+=ch;
			if(ch=='\''){
				//cout << "GOT " << x << endl;
				return x;
			}
		}
	} else {
		return "";
	}
	
}

int skipStr(std::ifstream & conf,const RWCString & skip){
	RWCString x;
	if(!conf.eof()){
		if(getConfStr(conf)==skip){
			return 1;
		} else {
			return 0;
		}
	} else {
		return 0;
	}
}

int ParseTpsContext(RWCString contextFile,TpsContext & tpsContext)
{
	std::ifstream conf(contextFile);

	if(!conf){
		std::cerr << "File:" << contextFile << " Not found " << std::endl;
		exit(1);
	}
	
	int LineNo=1;
	RWCString token;
	
	while((token=getConfStr(conf))!="") {
		if(token=="ATE"){
			if(getConfStr(conf)=="CONFIGURATION"){
				skipStr(conf,":");
				tpsContext.AteConfiguration=getConfStr(conf);
			} else {
				std::cerr << "ATE CONFIGURATION expected" << std::endl;
				exit(1);
			}
		} else if(token=="ADAPTER"){
			if(getConfStr(conf)=="CONFIGURATION"){
				skipStr(conf,":");
				tpsContext.AdapterConfiguration.push_back(getConfStr(conf));
			} else {
				std::cerr << "ADAPTER CONFIGURATION expected" << std::endl;
				exit(1);
			}
		} else if(token=="DRIVER"){
			if(getConfStr(conf)=="LIBRARY"){
				skipStr(conf,":");
				tpsContext.DeviceDriver=getConfStr(conf);
			} else {
				std::cerr << "DRIVER LIBRARY expected" << std::endl;
				exit(1);
			}
		} else if(token=="UUT"){
			if(getConfStr(conf)=="P/N"){
				skipStr(conf,":");
				tpsContext.UutPartNo=getConfStr(conf);
			} else {
				std::cerr << "UUT P/N expected" << std::endl;
				exit(1);
			}
		} else if(token=="ATLAS"){
			RWCString name;
			if((name=getConfStr(conf))=="TPS"){
				skipStr(conf,":");
				tpsContext.AtlasTps=getConfStr(conf);
			} else if(name=="MODULE"){
				skipStr(conf,":");
				tpsContext.AtlasModules.push_back(getConfStr(conf));
			} else {
				std::cerr << "ATLAS TPS or MODULE expected" << std::endl;
				exit(1);
			}
		} else if(token=="DEVICE"){
			RWCString virt_dev=getQuotedConfStr(conf);
			skipStr(conf,"=");
			RWCString real_dev=getConfStr(conf);
			
			deviceEquivalence[virt_dev] = new DeviceEquivalence(real_dev);
		} else if(token=="EVENT"){
			skipStr(conf,"MONITOR");
			RWCString virt_mon=getQuotedConfStr(conf);
			skipStr(conf,"=");
			RWCString real_dev=getConfStr(conf);
			RWCString real_cap=getConfStr(conf);
			deviceEquivalence[virt_mon] = new DeviceEquivalence(real_dev,real_cap);
			monitorEquivalence[virt_mon] = new DeviceEquivalence(real_dev,real_cap);
		}
	}
	return 1;
}


int tedlmain(  RWCString model ,const RWCString & dbd);

AST *
atlasmain( RWCString  v ,const RWCString & dbd,int mode);

void * deviceDriverLibraryHandle=0;

int main( int argc, char* argv[] )
{

	evalVisitor	go;

	TpsContext tpsContext;
	
	if ( argc <= 1 ) {
		execEnv.help();
		return 0 ;
	}
	execEnv.ProcessArgs(argc-1, &(argv[1]));
	
	std::string dbd;

	if(execEnv.dbDirectory()){
		dbd=RWCString(execEnv.dbDirectory());
		dbd=dbd+"/";
	} else {
		std::string pathcopy(argv[0]);
		//dbd=RWCString(dirname(pathcopy));
		dbd = pathcopy.substr(pathcopy.find_last_of("/\\") + 1);

		if(dbd.length()==0){
			dbd=dbd+".";
		}
		dbd=dbd+"/";
		//free(pathcopy);
	}

	//debugtrace.open("debugtrace");

    debugtrace.open("debug.log");
		
    sout.open("output.log");
	
	ParseTpsContext(execEnv.cfFile(),tpsContext);

	if(tpsContext.DeviceDriver !=""){
		deviceDriverLibraryHandle=dlopen(tpsContext.DeviceDriver.c_str(), RTLD_LAZY );
		if(deviceDriverLibraryHandle==0){
			std::cerr << "Device Library Error " << dlerror() << std::endl;
		}
	}
	

	if(tpsContext.AteConfiguration !="NONE"){
		tedlmain(  tpsContext.AteConfiguration ,dbd);
	}
	
	
	//1
	
	for (const auto& module : tpsContext.AdapterConfiguration) {
        if (module != "NONE") {
            tedlmain(module, dbd);
        }
    }
////	StringListIterator slit( tpsContext.AdapterConfiguration );
////	
////	while(++slit){
////		if(slit.key()!="NONE"){
////			tedlmain(  slit.key(),dbd );
////		}
////	}	
	//

// 2
	for (const auto& module : tpsContext.AtlasModules) {
        if (module != "NONE") {
            atlasmain(module, dbd, 1);
        }
    }
////	StringListIterator mlit( tpsContext.AtlasModules);
////	
////	while(++mlit){
////		if(mlit.key()!="NONE"){
////			atlasmain(  mlit.key(),dbd ,1);
////		}
////	}		
////	//

	AST *Root=atlasmain( tpsContext.AtlasTps ,dbd,0 );
	if ( Root ){
		go.Execute( Root, 0 );
		// report:cerr << "Equivalences:" << G_Equivalence_insantiated << endl;
		return 0;
	} else {
		std::cerr << " ***************aborting due to errors*****" << std::endl;
		return 1;
	}
	
	//ModifierDictionaryIterator mit( *modifierDictionary);
	//while(++mit){
	//	//delete mit.key();
	//	delete mit.value();
	//}
	
}


