#pragma once

#include <string>
#include <unordered_map>
#include <list>
#include <algorithm>
#include "AtlasAST.h"
//#include "ResourceAST.h"

#include "AppendCompat.h"

// // --- Modern Dictionary Template ---
// template <typename Key, typename Value>
// class Dictionary : public std::unordered_map<Key, Value> {
// public:
//     using std::unordered_map<Key, Value>::unordered_map;
// 
//     void insertKeyAndValue(const Key& k, const Value& v) {
//         this->operator[](k) = v;
//     }
// 
//     bool insertIfAbsent(const Key& k, const Value& v) {
//         return this->emplace(k, v).second;
//     }
// };
// 
// template <typename Key, typename Value>
// class DictionaryIterator {
// public:
//     using iterator = typename std::unordered_map<Key, Value>::iterator;
// 
//     DictionaryIterator(std::unordered_map<Key, Value>& dict)
//         : it(dict.begin()), end(dict.end()) {}
// 
//     bool operator()() {
//         if (it == end) return false;
//         ++it;
//         return it != end;
//     }
// 
//     Key key() const { return it->first; }
//     Value value() const { return it->second; }
// 
// private:
//     iterator it;
//     iterator end;
// };

#include <vector>

class GoToStatementStack {
public:
    using value_type = TargetStatement*;

    void push(value_type stmt) {
        stack_.push_back(stmt);
    }

    value_type pop() {
        if (stack_.empty()) return nullptr;
        value_type val = stack_.back();
        stack_.pop_back();
        return val;
    }

    value_type top() const {
        return stack_.empty() ? nullptr : stack_.back();
    }

    bool empty() const {
        return stack_.empty();
    }

    size_t size() const {
        return stack_.size();
    }

    void clear() {
        stack_.clear();
    }

private:
    std::vector<value_type> stack_;
};


using SymbolDictionary = Dictionary<std::string, AST*>;
using SymbolDictionaryIterator = DictionaryIterator<std::string, AST*>;
using FstatnoDictionary = Dictionary<int, Fstatno*>;
using GoToDictionary = Dictionary<int, GoToStatementStack*>;
using GoToDictionaryIterator = DictionaryIterator<int, GoToStatementStack*>;
using EntryDictionary = Dictionary<int, TargetStatement*>;

class ResourceAST;
using DeviceDictionary = Dictionary<std::string, ResourceAST*>;
using DeviceDictionaryIterator = DictionaryIterator<std::string, ResourceAST*>;
class ReverseMapEntry;

using ReverseMapDictionary = Dictionary<std::string, ReverseMapEntry*>;
using ReverseMapDictionaryIterator = DictionaryIterator<std::string, ReverseMapEntry*>;

////class ASTList {
////private:
////    std::list<AST*> items;
////
////public:
////    void insert(AST* ast) { items.push_back(ast); }
////
////    bool findValue(const std::string& key, AST*& result); // Not implemented yet
////
////    using iterator = std::list<AST*>::iterator;
////
////    iterator begin() { return items.begin(); }
////    iterator end()   { return items.end();   }
////};
////
class ASTListIterator {
public:
    using iterator = std::list<AST*>::iterator;

    ASTListIterator(ASTList& list)
        : current(list.begin()), end(list.end()) {}

    AST* key() const {
        return (current != end) ? *current : nullptr;
    }

    ASTListIterator& operator++() {
        if (current != end) ++current;
        return *this;
    }

    bool hasMore() const {
        return current != end;
    }

    explicit operator bool() const {
        return current != end;
    }
    
    bool atEnd() const { return current == end; }

private:
    iterator current;
    iterator end;
};

class StringSet : public std::unordered_set<std::string> {
public:
    StringSet() = default;
};

class ErrorLimit {
public:
    ErrorLimit();
    virtual int compare(ErrorLimit&);

private:
    double m_percentage;
    std::string m_nounModifier;
    double m_min;
    double m_max;
};

class Capability {
public:
    Capability(AST* a = nullptr);
    virtual int compare(Capability&);
    virtual void require();
    virtual bool required();
    virtual void setMax(double);
    virtual void setMin(double);
    virtual void setNoun(std::string);
    virtual void setModifier(std::string);
    virtual void setAST(AST* a);
    virtual AST* getAST();

    friend Capability;

private:
    bool m_required;
    std::string m_noun;
    std::string m_nounModifier;
    std::string m_command;
    double m_min;
    double m_max;
    double m_by;
    bool m_limit;
    AST* m_ast;
};

class CapabilityList : public std::list<Capability*> {
public:
    bool findValue(const std::string& key, AST*& result); // Stub
};

class CapabilityListIterator {
public:
    using iterator = std::list<Capability*>::iterator;

    CapabilityListIterator(CapabilityList& l)
        : current(l.begin()), end(l.end()) {}

    Capability* next() {
        if (current == end) return nullptr;
        return *current++;
    }

private:
    iterator current;
    iterator end;
};

////using ModifierDictionary = Dictionary<std::string, AST*>;
////using ModifierDictionaryIterator = DictionaryIterator<std::string, AST*>;
////
////using DimensionDictionary = Dictionary<std::string, AST*>;
////using DimensionDictionaryIterator = DictionaryIterator<std::string, AST*>;
////

class ReverseMapEntry;
