#pragma once

#include <vector>
#include <list>
#include <set>
#include <algorithm>
#include <regex>

// Lightweight Rogue Wave compatibility shims for append, insert, contains, isEmpty

// Append support (already covered by vector::push_back, but for stylistic uniformity)
template<typename T>
struct AppendableVector : public std::vector<T> {
    using std::vector<T>::vector; // inherit constructors
    using std::vector<T>::insert; // expose base class overloads

    void append(const T& item) {
        this->push_back(item);
    }

    void insert(size_t index, const T& item) {
        this->std::vector<T>::insert(this->begin() + index, item);
    }

    bool contains(const T& item) const {
        return std::find(this->begin(), this->end(), item) != this->end();
    }

    bool isEmpty() const {
        return this->empty();
    }
};


using ASTVector = AppendableVector<AST*>;

// AppendableList - equivalent to RWTPtrSlist
// Uses std::list as a stand-in for pointer-based containers
////template<typename T>
////struct AppendableList : public std::list<T> {
////    using std::list<T>::list; // inherit constructors
////
////    void append(const T& item) {
////        this->push_back(item);
////    }
////
////    void insert(const T& item) {
////        this->push_back(item); // RW-style insert maps to push_back
////    }
////
////    bool isEmpty() const {
////        return this->empty();
////    }
////};

template<typename T>
struct AppendableList : public std::list<T> {
    using std::list<T>::list; // inherit constructors

    void append(const T& item) {
        this->push_back(item);
    }
    
    void insert(const T& item) {
        this->push_back(item); // RW-style insert maps to push_back
    }


    bool isEmpty() const {
        return this->empty();
    }
};


// Set with contains()
template<typename T>
struct AppendableSet : public std::set<T> {
    using std::set<T>::set;

    bool contains(const T& item) const {
        return this->find(item) != this->end();
    }

    bool isEmpty() const {
        return this->empty();
    }
};

// Utility free function for isEmpty()
template<typename Container>
bool isEmpty(const Container& c) {
    return c.empty();
}

// For string-specific cases (e.g., StringVector -> AppendableVector<std::string>)
using StringVector = AppendableVector<std::string>;

// Example usage for AST* and similar legacy types
class AST;  // forward declaration
using ASTList = AppendableList<AST*>;
class InitData;
using InitList = AppendableVector<InitData *>;
using ContactList = AppendableVector<class Edge*>;
//using PathNodeList = AppendableVector<class PathNode*>;

// Add more aliases as needed


using VarNamesList = AppendableList<ANTLRTokenPtr>;
using VarNamesListIterator = VarNamesList::iterator;



