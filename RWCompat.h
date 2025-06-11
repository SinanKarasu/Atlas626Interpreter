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
#include <ostream>

typedef std::string RWCString;
typedef std::regex RWCRegexp;

// --- Placeholder Classes ---
class RWCollectable {};
class RWFile {};


// --- Legacy Rogue Wave compatibility shims ---

typedef long long Long;
//using RWBitVec = std::vector<bool>;

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



inline std::vector<bool> bitwise_not(const std::vector<bool>& bits) {
    std::vector<bool> result(bits.size());
    for (size_t i = 0; i < bits.size(); ++i) {
        result[i] = !bits[i];
    }
    return result;
}

#pragma once
#include <vector>
#include <algorithm>

class BitVec {
public:
    using Storage = std::vector<bool>;

    BitVec() = default;
    explicit BitVec(size_t n, bool val = false) : data(n, val) {}
    BitVec(const Storage& vec) : data(vec) {}
    BitVec(Storage&& vec) noexcept : data(std::move(vec)) {}

    size_t size() const { return data.size(); }
    void resize(size_t n, bool val = false) { data.resize(n, val); }

    bool operator[](size_t i) const { return data[i]; }
    //bool& operator[](size_t i) { return data[i]; }
    Storage::reference operator[](size_t i) { return data[i]; }


    const Storage& vec() const { return data; }
    Storage& vec() { return data; }

    // Unary NOT
    BitVec operator!() const {
        BitVec result(size());
        for (size_t i = 0; i < size(); ++i)
            result[i] = !data[i];
        return result;
    }

    // Bitwise AND
    BitVec operator&(const BitVec& other) const {
        size_t n = std::min(size(), other.size());
        BitVec result(n);
        for (size_t i = 0; i < n; ++i)
            result[i] = data[i] & other.data[i];
        return result;
    }

    BitVec& operator&=(const BitVec& other) {
        size_t n = std::min(size(), other.size());
        resize(n); // optional; ensures no out-of-bounds
        for (size_t i = 0; i < n; ++i)
            data[i] = data[i] & other.data[i];
        return *this;
    }

    // Bitwise OR
    BitVec operator|(const BitVec& other) const {
        size_t n = std::min(size(), other.size());
        BitVec result(n);
        for (size_t i = 0; i < n; ++i)
            result[i] = data[i] | other.data[i];
        return result;
    }

    BitVec& operator|=(const BitVec& other) {
        size_t n = std::min(size(), other.size());
        resize(n);
        for (size_t i = 0; i < n; ++i)
            data[i] = data[i] | other.data[i];
        return *this;
    }

    // Bitwise XOR
    BitVec operator^(const BitVec& other) const {
        size_t n = std::min(size(), other.size());
        BitVec result(n);
        for (size_t i = 0; i < n; ++i)
            result[i] = data[i] ^ other.data[i];
        return result;
    }

    BitVec& operator^=(const BitVec& other) {
        size_t n = std::min(size(), other.size());
        resize(n);
        for (size_t i = 0; i < n; ++i)
            data[i] = data[i] ^ other.data[i];
        return *this;
    }

private:
    Storage data;
};


using RWBitVec = BitVec;

#include <algorithm>
#include <cctype>
#include <string>

inline void toUpper(std::string& str) {
    std::transform(str.begin(), str.end(), str.begin(),
                   [](unsigned char c) { return std::toupper(c); });
}


// std::ostream& operator<<(std::ostream& os, const BitVec& bv) {
//     for (size_t i = 0; i < bv.size(); ++i) {
//         os.put(bv[i] ? '1' : '0');
//     }
//     return os;
// }






