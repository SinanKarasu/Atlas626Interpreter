#ifndef Set_H
#define Set_H

class Set : public BitVec {
  public:
    Set();
    void operator=(int b);

    void set(int pos);
    void clear(int pos);
    int  isSet(int pos);

    int firstTrue() {
        for (size_t i = 0; i < size(); ++i) {
            if (isSet(i)) {
                return static_cast<int>(i); // Return the index of the first set bit
            }
        }
        return std::string::npos; // Return -1 if no set bit is found
    }
};

// #include <ostream>
// 
// inline std::ostream& operator<<(std::ostream& os, const BitVec& bv) {
//     bv.print(os);  // assuming BitVec has a method like this
//     return os;
// }

// This one works albeit with strings.
// std::ostream& operator<<(std::ostream& os, const BitVec& vec) {
// //  os << "[";
//   for (size_t i = 0; i < vec.size(); ++i) {
// //    os << (vec[i] ? "true" : "false");
//     os << (vec[i] ? "1" : "0");
//     if (i < vec.size() - 1) {
//       os << ", ";
//     }
//   }
// //  os << "]";
//   return os;
//}


//friend 
inline std::ostream& operator<<(std::ostream& os, const BitVec& bv) {
    for (size_t i = 0; i < bv.size(); ++i) {
        os.put(bv[i] ? '1' : '0');
    }
    return os;
}


// inline std::ostream& operator<<(std::ostream& os, const Set& set) {
//     set.print(os);  // make sure Set::print(std::ostream&) exists
//     return os;
// }


#endif // Set_H
