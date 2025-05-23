#include	"Impedance.h"

Impedance::Impedance(double re,double im)
	:m_re(re)
	,m_im(im)
	{
	}
		

Impedance::operator  double	() const
	{
		return m_re;
	}

int
Impedance::operator	== (const Impedance & rhs)	const 	
	{
		return m_re==rhs.m_re;
	}
	
int
Impedance::operator	>  (const Impedance & rhs)	const
	{
		return m_re>rhs.m_re;
	}

int
Impedance::operator	<  (const Impedance & rhs)	const
	{
		return m_re<rhs.m_re;
	}

Impedance
Impedance::operator	+  (const Impedance & rhs)	const
	{
		return Impedance(m_re+rhs.m_re,m_im+rhs.m_im);
	}
	
Impedance
Impedance::operator	+  (const double & x)		const
	{
		return Impedance(m_re+x,m_im);
	}

Impedance
Impedance::operator	*  (const double & x)		const
	{
		return Impedance(m_re*x,m_im*x);
	}
