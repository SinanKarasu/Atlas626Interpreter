#include "astream.h"

// // Stub or actual implementation
// astream& operator<<(astream& s, const RWBitVec& x) {
//     s << "<RWBitVec output not yet implemented>";
//     return s;
// }
// 
// astream& operator>>(astream& s, AST* a) {
//     // Implement deserialization if needed
//     s >> std::ws; // skip whitespace
//     return s;
// }
// 
// astream& operator<<(astream& s, AST* a) {
//     // Implement serialization if needed
//     s << "<AST output not yet implemented>";
//     return s;
// }
// 
// 
// astream& operator<<(astream& s, const RWBitVec& vec) {
//     for (bool b : vec) {
//         s << (b ? '1' : '0');
//     }
//     return s;
// }
// 
// //If you want to deserialize:
// ///astream& operator>>(astream& s, RWBitVec& vec) {
// ///    vec.clear();
// ///    char c;
// ///    while (s >> c) {
// ///        if (c == '1') vec.push_back(true);
// ///        else if (c == '0') vec.push_back(false);
// ///        else break; // Stop on invalid char
// ///    }
// ///    return s;
// ///}
// ///

#include "astream.h"
#include "AST.h" // Make sure AST is defined

// I/O for AST*
astream& operator<<(astream& s, AST* a) {
    // Simplified dummy output — real logic may vary
    s << "<AST>";
    return s;
}

astream& operator>>(astream& s, AST* a) {
    // Not implemented — likely unused or replace with deserialization later
    return s;
}

// Output RWBitVec as 0/1 sequence
astream& operator<<(astream& s, const RWBitVec& vec) {
    for (bool b : vec) {
        s << (b ? '1' : '0');
    }
    return s;
}

// Input RWBitVec from 0/1 sequence
astream& operator>>(astream& s, RWBitVec& vec) {
    vec.clear();
    char c;
    while (s.in >> std::noskipws >> c) {
        if (c == '0') vec.push_back(false);
        else if (c == '1') vec.push_back(true);
        else break;
    }
    return s;
}
