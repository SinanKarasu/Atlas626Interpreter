#include	"ExceptionTypes.h"

FileNotFound::FileNotFound( const char * err):m_err(err)
		{
		}

ostream&         FileNotFound::operator<<( ostream& s )
		{
			s << m_err ;
			return s;
		}
ostream&
operator << (ostream & s,FileNotFound & e)
		{
			s << e.m_err;
			return s;
		}

PrintEvaluationRequest::PrintEvaluationRequest( const char * err,AST * comp):m_verb(err),m_comp(comp)
		{
		}

ostream&
PrintEvaluationRequest::operator<<( astream& s )
		{
			s << m_verb ;
			s << ", " ;
			s << m_comp ;
			return s;
		}

ostream&
operator << (astream & s,PrintEvaluationRequest & e)
		{
			return e.operator<<(s);
		}

TedlExecutionError::TedlExecutionError( RWCString err):m_mess(err)
		{
		}

ostream&
TedlExecutionError::operator<<( astream& s )
		{
			s << m_mess ;
			return s;
		}
ostream&
operator << (astream & s,TedlExecutionError & e)
		{
			return e.operator<<(s);
		}
