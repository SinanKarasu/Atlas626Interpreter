#ifndef Dictionary_h
#define Dictionary_h

#include	"AtlasAST.h"

//#include	"Std.h"
//#include	"Types.h"
// #include	"AtlasAST.h"
//                       symbol table support                        //

unsigned LabelHash(const RWCString& str);

// chat class AST; // forward declaration
class Resource;
class Capability;
class ResourceAST;

class SymbolDictionary : public RWTValHashDictionary<RWCString,AST *> {
	public:
		SymbolDictionary();

	private:
		enum { NbrBuckets = RWDEFAULT_CAPACITY };
};

class SymbolDictionaryIterator : public RWTValHashDictionaryIterator<RWCString,AST *>{
	public: SymbolDictionaryIterator( SymbolDictionary &d );
};

class FstatnoDictionary : public RWTValHashDictionary<RWInteger,Fstatno *>{
	public:
		FstatnoDictionary();

	private:
		enum { NbrBuckets = RWDEFAULT_CAPACITY };
};

class GoToStatementStack : public RWTStack< TargetStatement * , RWTValOrderedVector < TargetStatement * > >{
	public:
};

class GoToDictionary : public RWTValHashDictionary< RWInteger , GoToStatementStack * >{
	public:
		GoToDictionary();

	private:
		enum { NbrBuckets = RWDEFAULT_CAPACITY };
};

class GoToDictionaryIterator : public RWTValHashDictionaryIterator< RWInteger,GoToStatementStack *>{
	public: GoToDictionaryIterator(GoToDictionary &d);
};

class EntryDictionary : public RWTValHashDictionary< RWInteger,TargetStatement * >{
	public:
		EntryDictionary();

	private:
		enum { NbrBuckets = RWDEFAULT_CAPACITY };
};


class DeviceDictionary : public RWTValHashDictionary< RWCString, ResourceAST * >{
	public:
		DeviceDictionary();
		
	private:
		enum { NbrBuckets = RWDEFAULT_CAPACITY };
};

class DeviceDictionaryIterator : public RWTValHashDictionaryIterator< RWCString, ResourceAST * >{
public:
	DeviceDictionaryIterator( DeviceDictionary & d );
};

// chat class ASTList : public RWTValSlist<AST *>{
// chat public:
// chat 	RWBoolean	findValue( RWCString, AST *& );
// chat };
// chat 
// chat class ASTListIterator : public RWTValSlistIterator<AST *>{
// chat public:
// chat 	ASTListIterator(ASTList &d);
// chat };


class ASTList {
private:
    std::list<AST*> items;
public:
    void insert(AST* ast) { items.push_back(ast); }

    bool findValue(const std::string& key, AST*& result);

    // maybe iterator access too
    auto begin() { return items.begin(); }
    auto end()   { return items.end();   }
};


class ASTListIterator {
public:
    using iterator = std::list<AST*>::iterator;

    ASTListIterator(ASTList& list)
        : current(list.begin()), end(list.end()) {}

    AST* key() {
        if (current == end) return nullptr;
        return *current;
    }

    void operator++() { if (current != end) ++current; }
    bool hasMore() const { return current != end; }

private:
    iterator current;
    iterator end;
};


class StringSet : public RWTValHashSet< RWCString >{
public:
		StringSet();
private:
		enum { NbrBuckets = RWDEFAULT_CAPACITY };
};

class	ErrorLimit{
	public:
		ErrorLimit();
		
		virtual int	compare( ErrorLimit & );
		
	private:
		double		m_percentage;
		RWCString	m_nounModifier;
		double		m_min;
		double		m_max;
};

class	Capability{
	public:
		Capability(AST * a=0);
		
		// compare()
		// Returns -1 ==> Less than, or the passed characteristic within.
		//         +1 ==> Greater than, or the passed characteristic is outside
		//		  the bounds of this Characteristic.
				  
		virtual int		compare( Capability & );
		virtual void		require();
		virtual RWBoolean	required();
		virtual void		setMax	( double );
		virtual void		setMin	( double );
		virtual void		setNoun	( RWCString );
		virtual void		setModifier	( RWCString );
		virtual	void		setAST	( AST * a);
		virtual	AST	*	getAST	();

	friend Capability;
	
	private:
		RWBoolean	m_required;
		RWCString	m_noun;
		RWCString	m_nounModifier;
		RWCString	m_command;
		double		m_min;
		double		m_max;
		double		m_by;
		RWBoolean	m_limit;
		//ErrorLimit	m_errorLimit;
		AST	*	m_ast;
};

class	CapabilityList : public RWTPtrSlist<Capability>{
	public:
		RWBoolean	findValue( RWCString, AST *& );
};

class	CapabilityListIterator : public RWTPtrSlistIterator<Capability>{	public:
		CapabilityListIterator( CapabilityList & l );
};

class ReverseMapEntry;

class ReverseMapDictionary : public RWTValHashDictionary<RWCString,ReverseMapEntry *> {
	public:
		ReverseMapDictionary();

	private:
		enum { NbrBuckets = 10007 };
};

class ReverseMapDictionaryIterator : public RWTValHashDictionaryIterator<RWCString,ReverseMapEntry *>{
	public: ReverseMapDictionaryIterator( ReverseMapDictionary &d );
};

#endif //Dictionary_h
