
#include	"AtlasToken.h"


AtlasToken::AtlasToken(AtlasTokenType t, const ANTLRChar *s) : ANTLRRefCountToken(t,s)
		{ setType(t); _line = 0; setText(s); }

AtlasToken::AtlasToken()
		{ setType((AtlasTokenType)0); _line = 0; setText(""); }
	
AtlasToken::~AtlasToken() {;}

AtlasTokenType AtlasToken::getType()	const	{ return _type; }

void AtlasToken::setType(AtlasTokenType t)	{ _type = t; }

int AtlasToken::getLine()		const	{ return _line; }

void AtlasToken::setLine(int line)		{ _line = line; }

ANTLRChar * AtlasToken::getText()	const	{ return  (ANTLRChar *) (_text.data()); }
						//{ ANTLRChar *t = new ANTLRChar( *(_text.data()) );
						//  return t;
						//}

void AtlasToken::setText( const ANTLRChar *s )
		{ _text=s; }

ANTLRAbstractToken *AtlasToken::makeToken(AtlasTokenType tt,
								ANTLRChar *txt,
								int line)
	{
		ANTLRAbstractToken *t = new AtlasToken(tt,txt);
		t->setLine(line);
		return t;
	}
