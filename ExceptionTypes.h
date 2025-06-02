#ifndef ExceptionTypes_h
#define ExceptionTypes_h

#include	<iostream>
#include	"pcctscfg.h"
#include	"AtlasAST.h"

class FileNotFound 
{
public:
	FileNotFound( const char * err);
	std::ostream&        operator<<( std::ostream& s );
	friend std::ostream& operator << (std::ostream & s,FileNotFound & e);
private:
	const char * m_err;

};

class PrintEvaluationRequest 
{
public:
	PrintEvaluationRequest( const char * err,AST * comp);
	std::ostream&        operator<<( astream& s );
	friend std::ostream& operator << (astream & s,PrintEvaluationRequest & e);
private:
	RWCString m_verb;
	AST * m_comp;

};

class TedlExecutionError 
{
public:
	TedlExecutionError( RWCString err);
	std::ostream&        operator<<( astream& s );
	friend std::ostream& operator << (astream & s,TedlExecutionError & e);
private:
	RWCString m_mess;

};

#endif
