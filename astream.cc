
#include	"astream.h"
#include	"AtlasAST.h"

astream::astream():fstream(), binary(0), field_width(0){};
astream::astream( const char* a, int x ):fstream( a, x ), binary(0), field_width(0){};
						
void	astream::bin()			{ binary = 1; };
void	astream::reset()		{ binary = 0; };
			
void	astream::width( int &fw )	{ field_width = fw; fstream::width( fw ); };

astream	&operator<<( astream &s, const RWBitVec *x )
	{
		int	bit   = x->length() - 1;
		int	flag  = 1;

		// Strip leading zero's  [9.3.5 Paragraph 2]
		while ( bit >= 0  &&  ! (*x)(bit) )
			--bit;

		if ( s.binary ){
			
			s.fstream::width( 1 );
			
			while ( bit < (s.field_width-1) ){
				s.fstream::operator<<( ' ' );
				--s.field_width;
			}
			
			flag = ( s.field_width ? s.field_width-1 : bit );
			
			while ( (bit >= 0) && (flag >= 0) ){
			
				if ( bit > flag )
					s.fstream::operator<<( '#' );
				
				else if ( (*x)(bit) )
					s.fstream::operator<<( 1 );
					
				else
					s.fstream::operator<<( 0 );
				
				--flag;
				--bit;
			}
			
			s.reset();
		}else{
			long	value = 0;
		
			while ( bit >= 0 ){
			
				value = value << 1;
				if ( (*x)(bit) ) value |= 1;
				--bit;
			}

			s.fstream::operator<<( value );
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



