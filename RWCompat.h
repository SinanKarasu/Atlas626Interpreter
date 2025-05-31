// RWCompat.h — Minimal Legacy Compatibility Header
#pragma once

// --- Macros ---
#define RW_NPOS std::string::npos
#define RWBoolean bool
#define RWInteger int
#define RWDEBUG(x)
#define RWBOUNDS_CHECK 0
#define RWDEFAULT_CAPACITY 0

// --- Types ---
#include <string>
#include <regex>
#include <vector>
#include <list>
#include <deque>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <forward_list>
#include <memory>

typedef std::string RWCString;
typedef std::regex RWCRegexp;

// --- Placeholder Classes ---
class RWCollectable {};
class RWFile {};


// --- Legacy Rogue Wave compatibility shims ---

typedef long long Long;
using RWBitVec = std::vector<bool>;

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

template<typename T>
using RWTValSlistIterator = typename std::list<T>::iterator;

template<typename T>
using RWTPtrSlistIterator = typename RWTPtrSlist<T>::iterator;

template<typename K, typename V>
using RWTValHashDictionaryIterator = typename RWTValHashDictionary<K, V>::iterator;

template<typename T>
using RWTValOrderedVector = std::vector<T>;

template<typename T, typename Container = std::deque<T>>
using RWTStack = std::stack<T, Container>;

inline bool ends_with(const std::string& str, const std::string& suffix) {
    return str.size() >= suffix.size() &&
           str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}




