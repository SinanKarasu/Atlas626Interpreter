// Dictionary.h — Legacy name retained for compatibility
#pragma once

#include "ATokPtr.h"
#include "RWCompat.h"
#include "AppendCompat.h"

// using ASTList = AppendableList<AST*>;
// using ASTListIterator = ASTList::iterator;
class AST;
using ASTVector = AppendableVector<AST*>;
using ASTVectorIterator = ASTVector::iterator;

using VarNamesList = AppendableList<ANTLRTokenPtr>;

////class ASTList: public AppendableList<AST*> {
////	bool findValue(const std::string& name, AST*& result){
////		for (auto var_it = begin(); var_it != end(); ++var_it) {
////
////		//for (const auto& item : list) {
////    		if ((*var_it)->getName() == name) {
////       		 	result = *var_it;
////            	return true;
////			}
////		}
////    	return false;
////	}
////};

class ASTList: public AppendableList<AST*> {
public:
	bool findValue(const std::string& name, AST*& result);
	
	
	AST* at(size_t index) const {
		if (index >= this->size()) throw std::out_of_range("ASTList index out of range");
		auto it = this->begin();
		std::advance(it, index);
 		return *it;
    }
    
};

using SymbolDictionary = AppendableMap<std::string, AST*>;
using SymbolDictionaryIterator = SymbolDictionary::iterator;

using ASTListIterator = ASTList::iterator;
	