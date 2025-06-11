#pragma once
#include <iostream>
#include <iomanip>
#include "Std.h"

#include "ASTBase.h"
// #include	"ATokPtr.h"
// #include	"PBlackBox.h"

class astream : public std::fstream {
    friend std::fstream &operator>>(std::fstream &, astream &);
    friend std::fstream &operator<<(std::fstream &, const astream &);

  public:
    astream();
    astream(const char *a, int x);

    void bin();
    void reset();

    void width(int fw);
    void attach(int fd) {
        switch (fd) {
            case 0:
                this->std::ios::rdbuf(std::cin.rdbuf());
                break;
            case 1:
                this->std::ios::rdbuf(std::cout.rdbuf());
                break;
            case 2:
                this->std::ios::rdbuf(std::cerr.rdbuf());
                break;
            default:
                throw std::runtime_error("astream::attach only supports 0, 1, 2 for stdin/stdout/stderr");
        }
    };

    int binary;
    int field_width;
};

astream &operator<<(astream &s, const BitVec *x);

inline astream &operator<<(astream &s, const BitVec &x) {
    return s << &x;
}

astream &operator>>(astream &s, AST *a);
// astream		&operator<<( astream &s, AST *a );

// Legacy overloads (re-enable as needed)
// astream& operator<<(astream& s, const RWBitVec& x);
// astream& operator>>(astream& s, AST* a);
// astream& operator<<(astream& s, AST* a);
// astream& operator<<(astream& s, const RWBitVec& vec);
astream &operator>>(astream &s, BitVec &vec);
