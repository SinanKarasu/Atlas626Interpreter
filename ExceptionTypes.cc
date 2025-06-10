#include	"ExceptionTypes.h"

FileNotFound::FileNotFound( const char * err):m_err(err)
		{
		}

std::ostream&         FileNotFound::operator<<( std::ostream& s )
		{
			s << m_err ;
			return s;
		}
std::ostream&
operator << (std::ostream & s,FileNotFound & e)
		{
			s << e.m_err;
			return s;
		}

PrintEvaluationRequest::PrintEvaluationRequest( const char * err,AST * comp):m_verb(err),m_comp(comp)
		{
		}

std::ostream&
PrintEvaluationRequest::operator<<( astream& s )
		{
			s << m_verb ;
			s << ", " ;
			s << m_comp ;
			return s;
		}

std::ostream&
operator << (astream & s,PrintEvaluationRequest & e)
		{
			return e.operator<<(s);
		}

TedlExecutionError::TedlExecutionError( RWCString err):m_mess(err)
		{
		}

std::ostream&
TedlExecutionError::operator<<( astream& s )
		{
			s << m_mess ;
			return s;
		}
std::ostream&
operator << (astream & s,TedlExecutionError & e)
		{
			return e.operator<<(s);
		}
