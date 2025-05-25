//
//  NestedTokenStream.h
//  

#ifndef NESTED_TOKEN_STREAM_H
#define NESTED_TOKEN_STREAM_H

/*  This file should be accompanied by DISCLAIMER.TXT stating disclaimers */

//sik #include "tokens.h"
#include "AToken.h"
#include "ATokenStream.h"

class AtlasDLG;

class NestedTokenStream : public ANTLRTokenStream {
public:
    /* ctor */                  NestedTokenStream();
    /* ctor */                  NestedTokenStream(AtlasDLG *);
    /* dtor */                  ~NestedTokenStream();
    virtual _ANTLRTokenPtr      getToken();
    virtual ANTLRParser *       getParser();
    virtual ANTLRParser *       setParser(ANTLRParser *);
    virtual void                newTokenStream(AtlasDLG *ats);
    void                        setEofToken(ANTLRTokenType t);
    void                        setToken(ANTLRAbstractToken *);
    void                        trackColumns();
    int                         begcol();
    void                        skip();
protected:
    ANTLRParser                 *currentParser;
    AtlasDLG                    **stack;
    int                         maxDepth;
    int                         depth;
    ANTLRTokenType              eofToken;
    ANTLRAbstractToken          *token_to_fill;
    virtual void                panic(const char *);
    int                         track_columns;
    int                         skipFlag;
private:
    void                        init();
};

#endif
