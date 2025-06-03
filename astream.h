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
   
void attach(int fd) {
	switch (fd) {
		case 0: this->std::ios::rdbuf(std::cin.rdbuf()); break;
		case 1: this->std::ios::rdbuf(std::cout.rdbuf()); break;
		case 2: this->std::ios::rdbuf(std::cerr.rdbuf()); break;
		default:
			throw std::runtime_error("astream::attach only supports 0, 1, 2 for stdin/stdout/stderr");
	}
}

// using std::fstream::operator<<;
// using std::fstream::operator>>;
// using std::ostream::operator<<;


    // Handle const char* unambiguously
    astream& operator<<(const char* s) {
        (*static_cast<std::ostream*>(this)) << s;
        return *this;
    }
    
    
    // Handle stream manipulators like std::endl
	astream& operator<<(std::ostream& (*manip)(std::ostream&)) {
	    (*static_cast<std::ostream*>(this)) << manip;
	    return *this;
	}

        // Templated forwarding operators
    template <typename T>
    astream& operator<<(const T& val) {
        (*static_cast<std::ostream*>(this)) << val;
        return *this;
    }

    template <typename T>
    astream& operator>>(T& val) {
        (*static_cast<std::istream*>(this)) >> val;
        return *this;
    }


};

// Legacy overloads (re-enable as needed)
astream& operator<<(astream& s, const RWBitVec& x);
astream& operator>>(astream& s, AST* a);
astream& operator<<(astream& s, AST* a);
astream& operator<<(astream& s, const RWBitVec& vec);
astream& operator>>(astream& s, RWBitVec& vec);



