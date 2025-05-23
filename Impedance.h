#ifndef Impedance_H
#define Impedance_H


const double c_Inf = 1.0e34;

class Impedance {

public:
	Impedance(double re=c_Inf,double im=0.0);
	operator double  () const ;
	int		operator	== (const Impedance & rhs)	const 	;
	int		operator	>  (const Impedance & rhs)	const	;
	int		operator	<  (const Impedance & rhs)	const	;
	Impedance	operator	+  (const Impedance & rhs)	const	;
	Impedance	operator	+  (const double & x)		const	;
	Impedance	operator	*  (const double & x)		const	;

	double m_re;
	double m_im;

};




#endif // Impedance_H
