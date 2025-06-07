// AppendCompat.h — Modern STL-Compatible Container Extensions
#pragma once

#include <vector>
#include <list>
#include <set>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <string>

// --- Appendable Vector ---
template<typename T>
struct AppendableVector : public std::vector<T> {
    using std::vector<T>::vector;
    using std::vector<T>::insert;

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

// --- Appendable List ---
template<typename T>
struct AppendableList : public std::list<T> {
    using std::list<T>::list;

    void append(const T& item) {
        this->push_back(item);
    }

    void insert(const T& item) {
        this->push_back(item);
    }

    bool contains(const T& value) const {
        return std::find(this->begin(), this->end(), value) != this->end();
    }

    bool findValue(const T& key, T& result) const {
        auto it = std::find(this->begin(), this->end(), key);
        if (it != this->end()) {
            result = *it;
            return true;
        }
        return false;
    }

    bool isEmpty() const {
        return this->empty();
    }
};

// --- Appendable Set ---
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

// --- Unified AppendableMap Template (Modern Dictionary Replacement) ---

template<typename Key, typename Value>
class AppendableMap : public std::unordered_map<Key, Value> {
public:
    using std::unordered_map<Key, Value>::unordered_map;

    // Insert or overwrite
    void insertKeyAndValue(const Key& k, const Value& v) {
        (*this)[k] = v;
    }

    // Insert only if key is not present
    bool insertIfAbsent(const Key& k, const Value& v) {
        return this->emplace(k, v).second;
    }

    // Check if key exists
    bool contains(const Key& k) const {
        return this->find(k) != this->end();
    }

    // Try to get the value associated with a key
    bool findValue(const Key& k, Value& result) const {
        auto it = this->find(k);
        if (it != this->end()) {
            result = it->second;
            return true;
        }
        return false;
    }

    // Check if map is empty
    bool isEmpty() const {
        return this->empty();
    }
};


// --- Convenience Aliases ---
using StringSet = AppendableSet<std::string>;
using StringVector = AppendableVector<std::string>;
//sik move it to AST
// using SymbolDictionary = AppendableMap<std::string, AST*>;
// using SymbolDictionaryIterator = SymbolDictionary::iterator;
//


////using FstatnoDictionary = AppendableMap<int, Fstatno*>;
////using FstatnoDictionaryIterator = FstatnoDictionary::iterator;

////using GoToDictionary = AppendableMap<int, GoToStatementStack*>;
////using GoToDictionaryIterator = GoToDictionary::iterator;

////using EntryDictionary = AppendableMap<int, TargetStatement*>;
////using EntryDictionaryIterator = EntryDictionary::iterator;



