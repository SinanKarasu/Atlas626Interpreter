#ifndef Resource_h
#define Resource_h

#include	"Dictionary.h"
#include	"TedlDictionary.h"
#include	"AnalogResourceContext.h"
#include	"DataBusResourceContext.h"
#include	"Graph.h"

#include	"VirtualResourceAST.h"
#include	"ResourceList.h"
#include	"VertexDictionary.h"
#include	"DevicePath.h"
#include	"Circuit.h"
#include	"DFSContext.h"

#include	"StringVector.h"
#include	"ReverseMap.h"

// --------------------------- Resource ---------------------------------//

class Capability {
  public:
    Capability(AST *a = nullptr);
    virtual int  compare(const Capability &);
    virtual void require();
    virtual bool required();
    virtual void setMax(double);
    virtual void setMin(double);
    virtual void setNoun(std::string);
    virtual void setModifier(std::string);
    virtual void setAST(AST *a);
    virtual AST *getAST();

    friend Capability;

  private:
  
	// These are helper methods suggested by ChatGPT
    bool matches(const Capability &other) const {
        return this->m_noun == other.m_noun &&
               this->m_min <= other.m_min &&
               this->m_max >= other.m_max;
        // Optional: recursively match modifiers
    }
    
    bool mutualMatch(const Capability& other) const {
    	return this->matches(other) && other.matches(*this);
	}
	
	bool subsumes(const Capability& other) const {
		return (m_nounModifier == other.m_nounModifier) &&
           (m_max >= other.m_max) &&
           (m_min <= other.m_min);
	}
	// End of suggestions.

    bool        m_required;
    std::string m_noun;
    std::string m_nounModifier;
    std::string m_command;
    double      m_min;
    double      m_max;
    double      m_by;
    bool        m_limit;
    AST        *m_ast;
};

class CapabilityList : public AppendableList<Capability*> {
public:
    bool findValue(const std::string& key, AST*& result); // Stub
};

class CapabilityListIterator {
public:
    using iterator = std::list<Capability*>::iterator;

    CapabilityListIterator(CapabilityList& l)
        : current(l.begin()), end(l.end()) {}

    Capability* next() {
        if (current == end) return nullptr;
        return *current++;
    }

private:
    iterator current;
    iterator end;
};


class Resource{
public:
	Resource( Resource * previous, const RWCString & name,   const RWCString & version );
	Resource( Resource * previous, const RWCString & newName, Resource  * source  );
	virtual ~Resource();
	virtual Resource * getPrev	();
	virtual Resource * AddResource	( Resource  * resource );
	virtual Resource * clone	( Resource * previous, const RWCString & newName );
	virtual Resource * instantiate	( Resource * previous, const RWCString & newName );
	
	virtual Resource *	renamePort	( RWCString & from, RWCString & to );
	virtual Resource *	RenamePreface	( const RWCString & preface );
	virtual Vertex *	aliasPort	( RWCString & port,Vertex * v);
	virtual Vertex *	vertex		( const RWCString & name );
	virtual Vertex *	node		( const RWCString & name );
	virtual Resource *	LinkControl	( Resource *, AST *, AST *, AST * );
	virtual void		setName		( RWCString n );
	//virtual RWCString	getName		() const;
	virtual const std::string& getName() const { return m_name; }

	virtual Capability	*	findCapability	( Capability * );

	virtual CapabilityList	*	getCapabilities	();

	virtual void		insertCapability( Capability * c );

	virtual void		addNamedCapability	( RWCString n,AST * c );

	virtual AST *		getNamedCapability	( RWCString n);

	virtual void		addNounCapability	( RWCString n,AST * c );
		
	virtual DevicePath *	addPath		( DevicePath * dp );

	virtual Edge *		addWire		( Vertex * source, Vertex * dest );	
	virtual Edge *		addTwoTerm	( Vertex * source, Vertex * dest,AST * value );	
	virtual Vertex *	AddNode		( const NodeName  &, const NodeType nodeType /*=  UndefinedNodeType */);
	virtual Vertex *	AddNode		( const RWCString &, const NodeType nodeType /*=  UndefinedNodeType */);
	virtual void		AddEdge		( const RWCString &, const RWCString &, Edge * );
	virtual	VertexDictionary * nodes();
	void			insertContacts(int state,int pos,Edge * edge1,Edge * edge2);
	ResourceList &		resourceList();
	int			FindPath(Vertex   * Source,Vertex   * Dest , EdgeList & edgeList );
	Circuit * circuit() const;

	virtual int setState(int state, ReverseMap * rm=0);
	virtual int getState();
	
	// methods to be overriden
	virtual void addContacts(
					int state,
					int dstate,
					StringVector & FromList,
					StringVector & ToList,
					AST * control=0
				);
	
	virtual AST *  theCallArg(
					NounEntry* n,
					ANTLRTokenPtr p,
					RWCString & param,
					ModifierEntry * m,
					RWCString & type,
					RWCString & usage,
					int get,
					int put
				 );
	
	virtual AST *  RequirementsCheck( AST * );

	virtual AST *		createSensor(AST * nm);
	virtual AST *		createEventMonitor(AST * nm);
	virtual AST *		createSource();
	virtual AST *		createLoad();
	virtual AST *		createTimer();
	virtual AST *		createBusProtocol(AST * name);
	
	Set m_predict;

	int m_DfsNum;
	
	virtual Set getPredictor(int dfsNum);
	virtual int TellMe(Set & energizer);

	AnalogResourceContext	*	m_currentAnalogFSM;
	DataBusResourceContext	*	m_currentDataBusFSM;
	
	virtual RWCString theName(); 

	
	virtual ResourceDictionary * getResourceDictionary();
	virtual Resource * getDevice(const std::string & dev);

	// Introduce methods (EVENT,CNX,GATE)
		
	virtual	void	introduceFromEvent	(AST * e);
	virtual	void	introduceToEvent	(AST * e);
	virtual	void	introduceCnx		(AST * e);
	virtual	void	introduceMaxTime	(AST * e);
	virtual	void	introduceFromGate	(AST * e);
	virtual	void	introduceToGate		(AST * e);

	virtual	AST*	getFromEvent		(AST * e=0);
	virtual	AST*	getToEvent		(AST * e=0);
	virtual	AST*	getCnx			(AST * e=0);
	virtual	AST*	getMaxTime		(AST * e=0);
	virtual	AST*	getFromGate		(AST * e=0);
	virtual	AST*	getToGate		(AST * e=0);

	virtual int	resetResource(int softOrHard=0);
	virtual int	resetResources(int softOrHard=0);
	virtual int	connectToState
			(
				int			state,
				Association *		usingAssociation,
				EdgeList & 		edgeList
			);

	virtual int	connSetToState
			(
				Set &			state,
				Association *		usingAssociation,
				EdgeList & 		edgeList
			);

	virtual int	disconnToState
			(
				Association *		usingAssociation,
				Association & 		edgeList,
				int			state=c_Und
			);
	virtual	Vertex *checkLoopThruConnections(Vertex * v,DFSContext & c);

	SymbolDictionary *	m_callArgEntries;

	virtual	void	createReverseMap(ReverseMap * rm);
	virtual int	SimulateCircuit( Vertex * StartNode);
	virtual	int	setNounParameterValue(RWCString n,AST * d);
protected:
	ResourceList		m_ResourceList;
	VertexDictionary * 	m_nodeDictionary;
	
	SymbolDictionary 	m_capabilityNamedEntries;
	SymbolDictionary 	m_capabilityNounEntries;
	
	Circuit *		m_Circuit;
	RWCString		m_name;
	RWCString		m_preface;
	RWCString		m_version;
	//Resource *		m_resource;
	Resource *		m_previous;
	int			m_states;
	int			m_state;
	CapabilityList		m_capabilityList;
	ASTList			m_modifierList;


	//VirtualResourceAST *	_resourceAST;
        ResourceDictionary * m_ResourceDictionary;

	virtual int committed(Association * r=0);
	virtual void commit(Association * r);
	virtual void uncommit(Association * r);
		

protected:
	AST * m_FromEvent;
	AST * m_ToEvent;
	AST * m_Cnx;
	AST * m_MaxTime;
	AST * m_FromGate;
	AST * m_ToGate;

	double	m_Voltage;

	AssociationList	*	m_usingAssociationList;
	DevicePathList	*	m_devicePathList;
	
	int d_printFlag;
	
	virtual int printRequested();
	virtual void printCommitted(RWCString pre);


private:
	// Disable copy/assignment
        Resource( const Resource & );
        const Resource & operator= ( const Resource & );
};

// This is a suggestion by ChatGPT that is not used anywhere yet.

//class CapabilityMatcher {
//    const Capability& a;
//    const Capability& b;
//
//public:
//    CapabilityMatcher(const Capability& a_, const Capability& b_) : a(a_), b(b_) {}
//
//    bool run() const {
//        return a.subsumes(b) || b.subsumes(a);
//    }
//};


#endif	// Resource_h
