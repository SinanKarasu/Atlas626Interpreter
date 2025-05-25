#ifndef ResourceAST_h
#define ResourceAST_h

class ResourceAST : public AST{
public:
	ResourceAST(ANTLRTokenPtr p=0);
protected:
	TheType				m_storage;
	RWCString			m_name;			// Label Name
	int checkSubsume(AST * subSet,AST * supSet);
};


#endif // ResourceAST_h
