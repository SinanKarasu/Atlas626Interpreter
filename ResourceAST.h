#ifndef ResourceAST_h
#define ResourceAST_h

class ResourceAST : public AST{
public:
	ResourceAST(ANTLRTokenPtr p=0);
protected:
	TheType				m_storage;
	std::string			m_name;			// Label Name
	int checkSubsume(AST * subSet,AST * supSet);
};

using DeviceDictionary = AppendableMap<int, ResourceAST*>;

#endif // ResourceAST_h
