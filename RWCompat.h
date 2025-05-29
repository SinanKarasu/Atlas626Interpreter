// RWCompat.h — Compatibility shims for legacy Rogue Wave classes

#pragma once

#include <string>
#include <vector>
#include <deque>
#include <list>
#include <unordered_map>
#include <unordered_set>
#include <forward_list>
#include <regex>
#include <memory>
#include <stack>
#include <fstream> // RWFile placeholder

// --- Type macros ---
#define RW_NPOS std::string::npos
#define RWBoolean bool
#define RWInteger int
#define RWDEBUG(x)    // no-op
#define RWBOUNDS_CHECK 0
#define RWDEFAULT_CAPACITY 0

// --- RW-style string and regex ---
typedef std::string RWCString;
typedef std::regex RWCRegexp;

// --- Containers ---
template<typename T>
using RWTValVector = std::vector<T>;

template<typename T>
using RWTValSlist = std::list<T>;

template<typename K, typename V>
using RWTValHashDictionary = std::unordered_map<K, V>;

template<typename T>
using RWTValHashSet = std::unordered_set<T>;

template<typename T>
using RWTPtrSlist = std::forward_list<std::shared_ptr<T>>;

// --- Iterators ---
template<typename T>
using RWTValSlistIterator = typename std::list<T>::iterator;

template<typename T>
using RWTPtrSlistIterator = typename RWTPtrSlist<T>::iterator;

template<typename K, typename V>
using RWTValHashDictionaryIterator = typename RWTValHashDictionary<K, V>::iterator;

// --- Ordered vector and stack ---
template<typename T>
using RWTValOrderedVector = std::vector<T>;

template<typename T, typename Container = std::deque<T>>
using RWTStack = std::stack<T, Container>;

// --- Bit vector ---
using RWBitVec = std::vector<bool>;

// --- Placeholder types ---
class RWCollectable {
    // Used as a base class in the Rogue Wave container hierarchy.
    // You can extend this later to support virtual functions like clone(), isEqual(), etc.
};

class RWFile {
    // Placeholder for file streams; use std::ifstream / std::ofstream instead.
    // This can be replaced with a class adapter if needed.
};


