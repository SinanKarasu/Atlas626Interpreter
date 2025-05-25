#include        "AtlasAST.h"
#include        "TedlStd.h"
#include        "TedlParser.h"
#include        "AtlasDLG.h"
#include        "TedlLexer.h"


_ANTLRTokenPtr AtlasDLG::getToken() {
  AtlasToken         *result;
  result=(AtlasToken *)DLGLexerBase::getToken();
  result->col=DLGLexerBase::begcol();
  result->dlgLexer=this;
  return result;
}
