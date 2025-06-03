#ifndef AtlasDefinitions_h
#define AtlasDefinitions_h

#include	"Types.h"
#include	"AtlasStd.h"
#include	"AtlasToken.h"

#include	"NounsModifiersDimensions.h"
#include	"MnemonicsDB.h"

#include	"TedlDictionary.h"

#include	"AST.h"
#include	"InitList.h"

#include	"Dictionary.h"

#include	"ASTVector.h"


class AtlasParser;	// Forward declaration for compiling AtlasParser.cpp
class TedlParser;


#include	"AtlasTokens.h"
#include	"Scope.h"

#include	"SwitchModel.h"

void ArgCheck(int curc,AST * target, AST * source,int line_no);
void ResCheck(int curc,AST * target, AST * source,int line_no);

std::string unquoted(const std::string & name);

#include	"TedlDefinitions.h"
#include	"DevicePath.h"

#endif // AtlasDefinitions_h
