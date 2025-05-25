
#ifndef AtlasToken_h
#define AtlasToken_h

#include	"AtlasStd.h"

typedef ANTLRTokenType AtlasTokenType;

class AtlasToken : public ANTLRRefCountToken {
protected:
	AtlasTokenType _type;
	int _line;
	RWCString _text;

public:
	AtlasToken( AtlasTokenType t, const ANTLRChar *s );
	AtlasToken();
	virtual ~AtlasToken();

	AtlasTokenType	getType()	const;
	void		setType( AtlasTokenType t );
	int		getLine()	const;
	void		setLine(int line);
	ANTLRChar *	getText()	const;
	void		setText( const ANTLRChar *s );
	int	        			col;		// column
	
	virtual ANTLRAbstractToken *makeToken(
						AtlasTokenType tt,
						ANTLRChar *txt,
						int line
					);
	class AtlasDLG *	dlgLexer;
};

#endif
