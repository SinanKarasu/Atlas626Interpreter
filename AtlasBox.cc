#include	"AtlasAST.h"
#include	"AtlasStd.h"
#include	"AtlasParser.h"
#include	"AtlasDLG.h"
//#include	"Visitors.h"

#include	"AtlasBox.h"

AtlasBox::AtlasBox(const char * f)
	:NestedParserBlackBox<AtlasLexer,AtlasParser,AtlasToken>(f)
	{
	}
