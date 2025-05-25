#include	"AtlasAST.h"

TargetStatement::TargetStatement( AST * a, RWTValVector<RWInteger> & cl, RWInteger cd )
				:_a(a), _ContextLevel(cl), _ContextDepth(cd) {};
		
Fstatno::Fstatno():_testno(0),_stepno(0){};

