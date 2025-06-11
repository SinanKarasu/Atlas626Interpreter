
#include	"astream.h"
#include	"AtlasAST.h"

astream::astream() : std::fstream(), binary(0), field_width(0) {}
astream::astream( const char* a, int x ):std::fstream( a, x ), binary(0), field_width(0){};
						
void	astream::bin()			{ binary = 1; };
void	astream::reset()		{ binary = 0; };
// astream& astream::width(int w) {
//     out_.width(w);   // assuming out_ is an internal std::ostream
//     return *this;
// }
void astream::width( int fw ) { field_width = fw; std::fstream::width( fw ); };
// accept temporaries also.

astream &operator<<(astream &s, const BitVec *x) {
    int bit = x->size() - 1;
    int spacePadding = 1;

    while (bit >= 0 && !(*x)[bit])
        --bit;

    if (s.binary) {
        s.width(1);
        while (bit < (s.field_width - 1)) {
            s << ' ';
            --s.field_width;
        }

        spacePadding = (s.field_width ? s.field_width - 1 : bit);

        while (bit >= 0 && spacePadding >= 0) {
            if (bit > spacePadding)
                s << '#';
            else if ((*x)[bit])
                s << int(1);
            else
                s << int(0);

            --spacePadding;
            --bit;
        }

        s.reset();
    } else {
        long value = 0;
        while (bit >= 0) {
            value <<= 1;
            if ((*x)[bit]) value |= 1;
            --bit;
        }
        s << value;
    }
    return s;
}

	
astream	&operator>>( astream &s, AST * a )
	{
		return a->operator>>( s );
	}
	
astream	&operator<<( astream &s, AST * a )
	{
		return a->operator<<( s );
	}

astream& operator>>(astream& s, BitVec& b) {
    // Define how you want BitVec to be read from astream
    // For now, a stub to get it linking:
    return s;
}

