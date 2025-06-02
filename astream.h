// AStreamModern.h — Modern replacement for legacy astream
#pragma once

#include <fstream>
#include <iomanip>
#include <iostream>
#include "RWCompat.h"

// Forward declarations if needed
//class RWBitVec;
//class AST;

class astream : public std::fstream {
public:
   astream() = default;

   astream(const char* filename, std::ios::openmode mode)
       : std::fstream(filename, mode) {}

   void bin() {
       binary = true;
   }

   void reset() {
       clear();  // reset stream state
       seekg(0);
       seekp(0);
   }

   void width(int& fw) {
       this->std::ios::width(fw);
   }

   int binary = 0;
   int field_width = 0;
   
//     friend astream& operator<<(astream& s, const RWBitVec& x);
//     friend astream& operator>>(astream& s, RWBitVec& x);
//     friend astream& operator<<(astream& s, AST* a);
//     friend astream& operator>>(astream& s, AST* a);
//     
//         // Accept manipulators like std::endl, std::hex, etc.
//     astream& operator<<(std::ostream& (*manip)(std::ostream&)) {
//         manip(*this); return *this;
//     }
// 
//     astream& operator<<(std::ios& (*manip)(std::ios&)) {
//         manip(*this); return *this;
//     }
// 
//     astream& operator<<(std::ios_base& (*manip)(std::ios_base&)) {
//         manip(*this); return *this;
//     }

};

// Legacy overloads (re-enable as needed)
astream& operator<<(astream& s, const RWBitVec& x);
astream& operator>>(astream& s, AST* a);
astream& operator<<(astream& s, AST* a);
astream& operator<<(astream& s, const RWBitVec& vec);
astream& operator>>(astream& s, RWBitVec& vec);



#include <vector>
#include <iostream>

// Forward declare AST
////class AST;
////
////using RWBitVec = std::vector<bool>;
////
////class astream {
////public:
////    std::ostream& out;
////    std::istream& in;
////
////    astream(std::ostream& o = std::cout, std::istream& i = std::cin)
////        : out(o), in(i) {}
////
////    template <typename T>
////    astream& operator<<(const T& value) {
////        out << value;
////        return *this;
////    }
////
////    template <typename T>
////    astream& operator>>(T& value) {
////        in >> value;
////        return *this;
////    }
////};
////

////#include <fstream>
////#include <string>
////#include <vector>
////
////using RWBitVec = std::vector<bool>; // legacy alias
////
////class AST; // forward declaration to avoid circular include
////
////class astream : public std::fstream {
////public:
////    using std::fstream::fstream; // inherit all constructors
////
////    // Overload for writing AST*
////    astream& operator<<(AST* a);
////    astream& operator>>(AST*& a);
////
////    // Overload for writing bit vectors
////    astream& operator<<(const RWBitVec& vec);
////    astream& operator>>(RWBitVec& vec);
////};
////
////// Explicit overloads
////astream& operator<<(astream& s, AST* a);
////astream& operator>>(astream& s, AST* a);
////
////astream& operator<<(astream& s, const RWBitVec& vec);
////astream& operator>>(astream& s, RWBitVec& vec);
////

// class astream : public std::fstream {
// public:
//     using std::fstream::operator<<;
//     using std::fstream::operator>>;
// 
//     using std::fstream::fstream; // inherit constructors
// 
//     // Your custom overloads
//     astream& operator<<(AST* a);
//     astream& operator>>(AST*& a);
// 
//     astream& operator<<(const RWBitVec& vec);
//     astream& operator>>(RWBitVec& vec);
//};


