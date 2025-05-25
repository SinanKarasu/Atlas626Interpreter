#ifndef NestedParserBlackBox_h
#define NestedParserBlackBox_h

#include	<iostream>
#include	"pcctscfg.h"
#include	"NestedTokenStream.h"

#include	"ExceptionTypes.h"


template<class Lexer, class Parser, class Token>
class  NestedParserBlackBox {
protected:
	DLGFileInput *in;
	Lexer *lexer;
	_ANTLRTokenPtr tok;
	ANTLRTokenBuffer *pipe;
	Parser *_parser;
	FILE *file;
	NestedTokenStream    *nestedTokenStream;
public:
	
	NestedParserBlackBox(const char *fname)
		{
			file = fopen(fname, "r");
			if ( file==NULL ) {throw( FileNotFound(fname) ); }
			else {
				_init();
			}
		}

	~NestedParserBlackBox()
		{
			delete in; delete lexer; delete pipe; delete _parser; delete tok;
			fclose(file);
		}
		
	void ResetFile(const char * newFilename)
		{
			FILE * newFILE=fopen(newFilename,"r");
			if ( newFILE==NULL ) {throw( FileNotFound(newFilename) ); }
			DLGFileInput  *dlgFileInput=new DLGFileInput(newFILE);
			lexer = new Lexer(dlgFileInput,2000);
			lexer->nestedTokenStream=nestedTokenStream;
			lexer->setToken(tok);
			lexer->init();
			lexer->filename=newFilename;
			nestedTokenStream->newTokenStream(lexer);
			_parser->init();
		}

	void ResetString(const char * str,int _mode=-1)
		{
			DLGStringInput  *dlgStringInput=new DLGStringInput(str);
			lexer = new Lexer(dlgStringInput);
			lexer->nestedTokenStream=nestedTokenStream;
			lexer->setToken(tok);
			lexer->init();
			if(_mode!=-1){
				lexer->mode(_mode);
				lexer->homeState=(_mode);
			}
			lexer->filename=str;
			nestedTokenStream->newTokenStream(lexer);
			_parser->init();
		}

	Parser *parser()
		{
			return _parser;
		}
	Lexer  *getLexer()
		{
			return lexer;
		}
private:
	void _init()
		{
 			in = new DLGFileInput(file);
			lexer = new Lexer(in);
			nestedTokenStream=new NestedTokenStream(lexer);
			pipe = new ANTLRTokenBuffer(nestedTokenStream,1);
			tok = new Token;
			lexer->setToken(tok);
			_parser = new Parser(pipe);
			_parser->init();
   		
		}
};

#endif
