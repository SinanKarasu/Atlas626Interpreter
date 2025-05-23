#ifndef AtlasLexer_h
#define AtlasLexer_h


class AtlasLexer : public AtlasDLG{
public:
	AtlasLexer(DLGInputStream *in,unsigned bufsize=4000)
		:AtlasDLG(in,bufsize)
		{
			init();
		}

	void init()
		{
			homeState=STARTAtlas;
			resetModeStack();
			trackColumns();
			sawFStatno=0;
		}
	
	virtual     ~AtlasLexer(){};
};

#endif
