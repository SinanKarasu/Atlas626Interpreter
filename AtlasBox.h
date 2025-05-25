#ifndef AtlasBox_h
#define AtlasBox_h

#include	"PBlackBox.h"
#include	"AtlasLexer.h"

#include	"NestedParserBlackBox.h"


class AtlasBox:public NestedParserBlackBox<AtlasLexer,AtlasParser,AtlasToken> {
public:
	AtlasBox(const char * f);
};



#endif // AtlasBox_h
