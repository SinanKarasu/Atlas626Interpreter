// Dictionary.cc
#include "AST.h"           // or wherever AST is defined
//#include "ASTList.h"       // if ASTList is a distinct type
#include "Dictionary.h"

// All template methods should remain in the header.
// This file is only needed for non-template helper functions, if any.

// Currently no implementation required here.
// You can define custom logging, diagnostics, or non-templated helpers later.



bool ASTList::findValue(const std::string& name, AST*& result){
	for (auto var_it = begin(); var_it != end(); ++var_it) {
    	if ((*var_it)->getName() == name) {
       		result = *var_it;
        	return true;
		}
	}
    return false;
}
