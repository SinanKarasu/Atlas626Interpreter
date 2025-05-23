#ifndef astream_h
#define astream_h

#include	<iostream>
#include	<iomanip>
#include	"Std.h"

#include	"ASTBase.h"
//#include	"ATokPtr.h"
//#include	"PBlackBox.h"

using namespace std;  // Safe here


class	astream : public fstream {
		friend fstream	&operator>>( fstream&, astream& );
		friend fstream	&operator<<( fstream&, const astream& );
		
		public:
			astream();
			astream( const char* a, int x );
						
			void	bin();
			void	reset();
			
			void	width( int &fw );
			
			int	binary;
			int	field_width;
	};

// astream		&operator<<( astream &s, const RWBitVec *x );
astream &operator<<(astream &s, const RWBitVec &x);

astream		&operator>>( astream &s, AST *a );
astream		&operator<<( astream &s, AST *a );


#endif
