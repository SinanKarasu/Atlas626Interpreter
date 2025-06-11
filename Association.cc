#include	"Resource.h"
#include	"Vertex.h"
#include	"TAGContext.h"
#include	"Edge.h"

AssociationStack associationStack;
ConnectRequestStack connectRequestStack;

// An Association is an EdgeList that is associated with a Vertex
// and a Resource. This is the basic structure all the connect/disconnect
// logic is buit upon.

Association::Association(Vertex * uutv,ConnectRequest * cr):m_uutv(uutv),m_ConnectRequest(cr)
	{
	}


RWCString
Association::theName()
	{
		return RWCString("[")+m_ConnectRequest->theName()+"]"+m_uutv->theName();
	}

void
Association::insertEdge( Edge *  e)
	{
		if(!contains(e)){
			insert(e);
		}
	}

Vertex *
Association::getVertex() const { return m_uutv; }

Resource *
Association::getResource() const { return m_ConnectRequest->getResource(); }

int
Association::clean(Edge *& e)
	{
		// We check to see if Edge *e has committed itself to this
		// Association. If it has then we remove it from Us.
		// The idea here is this. Assume that a connection was made which
		// required certain edges to be committed to complete the connection.
		// Now if all the edges that made the connection ask this association
		// to remove it , then in the end , if the association became empty,
		// then it could be thrown away, because all that was done is now
		// successfully undone.
		int status;
		assert(contains(e));
		if (e->committed(this)) {
			//assert(0);
			e->uncommit(this);
			status=1;
		} else {
			status=0;
		}
		remove(e);
		return status;
	}
int
Association::cleanAll()
	{
		// Here we are paranoid. First we create a new EdgeList and
		// copy all the pointers we need to remove. This is done so that
		// we will not delete pointers from the same EdgeList we are
		// iterating over.
		EdgeList l;
		//EdgeListIterator elit(*this);
		for(const auto& elit: *this) {
		//while(++elit){
			if(!elit->committed(this)){
				l.insert(elit);
			} else {
				//assert(0);
			}
		}
			
		//EdgeListIterator lit(l);
		for(const auto& lit: l){
		//while(++lit){
			Edge * e=lit;
			clean(e);
		}

		if(size()==0){
			return 1;
		} else {
			return 0;
		}		
	}		

void
Association::init( Vertex * uutv,ConnectRequest * cr )
		{
			m_uutv=uutv;
			m_ConnectRequest = cr;
		}

int 
Association::isEmpty()
	{
		return (size()==0);
	}

////Edge * 
////Association::removeEdge(Edge * e)
////	{
////		
////		if (e == 0) {	// either remove any unspecified edge and return it
////			if (size()) {
////				return removeFirst();
////			}
////		} else {	// or remove a specified edge and return it
////			if ( remove(e) ){
////				return e;
////			}
////		}
////		return 0;
////	}
	
////Edge * 
////Association::getEdge(Edge * e)
////	{
////		if (e == 0) {
////			if (size() == 0) {
////				return first();
////			}
////		} else {
////			if (contains(e)) {
////				return e;
////			}
////		}
////		return 0;
////	}
////

void
Association::print()
	{
		EdgeList::print(this);
	}

////AssociationIterator::AssociationIterator( Association &d )
////        :EdgeListIterator (d)
////	{
////	}

////AssociationList::AssociationList():RWTValSlist<Association *>()
////	{
////	}
////
//RWCString
//AssociationList::theName()
//	{
//		
//		return RWCString("[")+m_ConnectRequest->theName()+"]"+m_uutv->theName();
//	}

AssociationList::AssociationList():AppendableList<Association *>() {}

void
AssociationList::print()
	{
		//AssociationListIterator alit(*this);
		for(const auto& alit: *this) {
		//while(++alit){
 			Association * a=alit;
			std::cout << "//-Association " << a->theName() << std::endl;
			a->print();
			std::cout <<"//----- end Association" <<  std::endl;
		}
	}

// AssociationListIterator::AssociationListIterator( AssociationList &d )
// 	{
// 	}

// A ConnectRequest is a AssociationList
// It associates to a Vertex *portv , and a Resource * r, a bunch of Associations
// (in other words, a bunch of Vertices and associated Edge commitments.
// So if DCV HI is connected to P1-A and P1-B, then we would have a
// ConnectRequest for DVM-HI that consists of two EdgeLists (Associations),
// one for P1-A and the other for P1-B.

ConnectRequest::ConnectRequest(Vertex * portv,ConnectRequests * crs)
	:m_portv(portv),m_ConnectRequests(crs)
	{
	}
			

Vertex * 
ConnectRequest::getVertex() const
	{
		return m_portv;
	}

RWCString
ConnectRequest::theName()
	{
		return m_ConnectRequests->theName()+m_portv->theName();
	}

// e.g.:
// Context:
// This is DVM-HI
// v is P1-A
// e is a relay.

Association *
ConnectRequest::insertEdge(Vertex * uutv,Edge * e)
	{
		Association    *a = 0;

		if (!(a = find(uutv))) {
			a = associationStack.getOne(uutv, this);
			insert(a);
		}
		if (e) {
			a->insertEdge(e);
		}
		return a;
	}

Association * 
ConnectRequest::getOne(Vertex * uutv)
	{
		Association    *x;

		if (uutv) {
			if (x = find(uutv)) {
				return x;
			} else {
				return insertEdge(uutv, 0);
			}
		} else if (size() == 0) {
			return 0;
		} else {
			return front();
		}
	}
	
Resource *
ConnectRequest::getResource() const { return m_ConnectRequests->getResource(); }

// ConnectRequest is never deleted, but stashed away when empty. A retrieved one
// is reconfigured thru init.

void
ConnectRequest::init( Vertex * portv,ConnectRequests * crs )
	{
		m_portv = portv;
		m_ConnectRequests = crs;
	}
// delet an Association from a ConnectRequest
void 
ConnectRequest::deleteOne(Association *& a)
	{
		assert(contains(a));
		remove(a);
		////////associationStack.push(a);
		a = 0;
	}

int 
ConnectRequest::isEmpty()
	{
		return (size() == 0);
	}
		
int
ConnectRequest::clean(Association *& x)
	{
		assert(contains(x));

		if( x->cleanAll() ){
			deleteOne(x);
			return 1;
		} else {
			return 0;
		}
	}

int
ConnectRequest::cleanAll()
	{
		AssociationList l;

		//AssociationListIterator alit(*this);
		for(const auto& alit: *this) {
		//while (++alit) {
			if (alit->cleanAll()) {
				l.insert(alit);
			}
		}

		//AssociationListIterator lit(l);
		for(const auto& lit: l) {
		//while (++lit) {
			Association *a=lit;
			deleteOne( a );
		}

		if (size() == 0) {
			return 1;
		} else {
			return 0;
		}

	}

void
ConnectRequest::print()
	{
		//AssociationListIterator alit(*this);
		for(const auto& alit: *this) {
		//while (++alit) {
			alit->print();
		}

	}

Association *
ConnectRequest::find(Vertex * uutv)
	{
		//AssociationListIterator alit(*this);
		for(const auto& alit: *this) {
		//while (++alit) {
			if (alit->getVertex() == uutv) {
				return alit;
			}
		}
		return 0;
	}


////ConnectRequestIterator::ConnectRequestIterator( ConnectRequest &d )
////	:AssociationListIterator (d)
////	{
////	}

// ConnectRequestList::ConnectRequestList()
ConnectRequestList::ConnectRequestList():AppendableList<ConnectRequest *>()
{
}
// 	:RWTValSlist<ConnectRequest *>()
// 	{
// 	}

// ConnectRequestListIterator::ConnectRequestListIterator( ConnectRequestList &d )
// 	:RWTValSlistIterator< ConnectRequest *> (d)
// 	{
// 	}

// Now we are finally to mother of all CONNECT/DISCONNECT logic
// ConnectRequests keep track of alt the connections fo a Resource

ConnectRequests::ConnectRequests(Resource * r):m_Resource(r)
	{
	}
			
RWCString
ConnectRequests::theName()
	{
		return RWCString("^")+m_Resource->theName();
	}

	
ConnectRequest * 
ConnectRequests::insertEdge(Vertex * portv,Vertex * uutv,Edge * e)
	{
		// *portv is the port vertex of the resource 
		// *uutv  is the endpoints of a CONNECT/DISCONNECT request
		// and Edge * e is somewhere in the connectivity diagram.
		
		ConnectRequest *c = 0;

		if (!(c = find(portv))) {
			//c = new ConnectRequest(portv, m_Resource);
			c = connectRequestStack.getOne(portv, this);
			insert(c);
		}
		if (uutv) {
			c->insertEdge(uutv, e);
		}
		return c;
	}

Resource *
ConnectRequests::getResource() const { return m_Resource; }

ConnectRequest * 
ConnectRequests::getOne(Vertex * portv)
	{
		ConnectRequest *x;

		if (portv) {
			if (x = find(portv)) {
				return x;
			} else {
				return insertEdge(portv, 0, 0);
			}
		} else if (size() == 0) {
			return 0;
		} else {
			return front();
		}
	}
	
void 
ConnectRequests::deleteOne(ConnectRequest *& a)
	{
		assert(contains(a));
		assert(a->size() == 0);
		remove(a);
		////////connectRequestStack.push(a);
		a = 0;
	}

int 
ConnectRequests::isEmpty()
	{
		return (size()==0);
	}
		
int 
ConnectRequests::clean(ConnectRequest *& x)
	{
		ConnectRequestList l;
		assert(contains(x));
		return x->cleanAll();
	}

int 
ConnectRequests::cleanAll()
	{
		ConnectRequestList l;
		//ConnectRequestListIterator crlit(*this);
		for(const auto& crlit: *this) {
		//while (++crlit) {
			if (crlit->cleanAll()) {
				l.insert(crlit);
			}
		}
		//ConnectRequestListIterator lit(l);
		for(const auto& lit: l) {
		//while (++lit) {
			ConnectRequest * cr = lit;
			deleteOne( cr );
		}
		if (size() == 0) {
			return 1;
		} else {
			return 0;
		}
	}

void 
ConnectRequests::print()
	{
		//ConnectRequestListIterator crlit(*this);
		for(const auto& crlit: *this) {
		//while (++crlit) {
			crlit->print();
		}
	}
		

		
void
ConnectRequests::init( Resource * r )
			{
				m_Resource = r;
			}
				
ConnectRequest * 
ConnectRequests::find(Vertex * portv)
		{
			//ConnectRequestListIterator crlit(*this);
			for(const auto& crlit: *this) {
			//while(++crlit){
				if(crlit->getVertex()==portv){
					return crlit;
				}
			}
			return 0;	
		}

////ConnectRequestsIterator::ConnectRequestsIterator( ConnectRequestList &d )
////	:ConnectRequestListIterator (d)
////	{
////	}

//----------------------------------------------------------
AssociationStack::AssociationStack():m_instantiated(0){}
Association *
AssociationStack::getOne(Vertex * uutv,ConnectRequest * cr)
		{
			if(empty()){
				m_instantiated++;
				return new Association(uutv,cr);
			} else {
				Association *a= top(); pop();
				a->init(uutv,cr);
				return a;
			}
		}


AssociationListStack::AssociationListStack():m_instantiated(0){}
AssociationList *
AssociationListStack::getOne()
		{
			if(empty()){
				m_instantiated++;
				return new AssociationList;
			} else {
				auto t=top(); pop();
				return t;
			}
		}

ConnectRequestStack::ConnectRequestStack():m_instantiated(0){}
ConnectRequest *
ConnectRequestStack::getOne(Vertex * portv,ConnectRequests * crs)
		{
			if(empty()){
				m_instantiated++;
				return new ConnectRequest(portv,crs);
			} else {
				ConnectRequest *cr= top(); pop();
				cr->init(portv,crs);
				return cr;
			}
		}
