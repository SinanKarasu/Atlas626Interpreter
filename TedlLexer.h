#ifndef TedlLexer_h
#define TedlLexer_h


class TedlLexer : public AtlasDLG{
public:
	TedlLexer(DLGInputStream *in,unsigned bufsize=4000)
		:AtlasDLG(in,bufsize)
		{
			init();
		}
	void init()
		{
			homeState=STARTTedl;
			resetModeStack();
			trackColumns();
		}
	
	virtual     ~TedlLexer(){};
};

#endif
