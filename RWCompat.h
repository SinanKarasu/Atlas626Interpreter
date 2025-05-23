// RWCompat.h
#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <forward_list>
#include <regex>

#define RW_NPOS std::string::npos
#define RWBoolean bool
#define RWInteger int
#define RWDEBUG(x)
#define RWBOUNDS_CHECK 0
#define RWDEFAULT_CAPACITY 0

typedef std::string RWCString;
typedef std::regex RWCRegexp;

template<typename T> using RWTPtrSlist = std::forward_list<std::shared_ptr<T>>;
//template<typename T> using RWTStack = std::stack<T>;

#include <stack>
template<typename T, typename Container = std::deque<T>>
using RWTStack = std::stack<T, Container>;


template<typename T> using RWTValVector = std::vector<T>;
template<typename T> using RWTValSlist = std::forward_list<T>;
template<typename K, typename V> using RWTValHashDictionary = std::unordered_map<K, V>;
template<typename T> using RWTValHashSet = std::unordered_set<T>;

template<typename K, typename V>
using RWTValHashDictionaryIterator = typename std::unordered_map<K, V>::iterator;

#include <vector>
template<typename T>
using RWTValOrderedVector = std::vector<T>;  // ordered => std::vector

#include <vector>
using RWBitVec = std::vector<bool>;

#include <vector>
template<typename T>
using RWTValOrderedVector = std::vector<T>;  // ordered => std::vector

#include <forward_list>
template<typename T>
using RWTValSlist = std::forward_list<T>;

template<typename T>
using RWTValSlistIterator = typename std::forward_list<T>::iterator;

template<typename T>
using RWTPtrSlist = std::forward_list<std::shared_ptr<T>>;

template<typename T>
using RWTPtrSlistIterator = typename std::forward_list<std::shared_ptr<T>>::iterator;


