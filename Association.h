#ifndef Association_H
#define Association_H
#include	"EdgeList.h"

class	ConnectRequests;
class	ConnectRequest;
class	Resource;

class Association : public EdgeList {
public:
	Association(Vertex * v,ConnectRequest * cr);
	void insertEdge( Edge *  e);

	Vertex		* getVertex() const ;
	Resource	* getResource() const ;

	int isEmpty();
	
	RWCString theName();
		
	Edge * removeEdge(Edge * e);

	int clean( Edge *& e);
	int cleanAll();
	
	Edge * getEdge(Edge * e);
	void init (Vertex * uutv, ConnectRequest * cr);

	void print();

private:
	Vertex * m_uutv;
	ConnectRequest * m_ConnectRequest;
};

////class AssociationIterator : public EdgeListIterator{
////public:
////	AssociationIterator( Association &d );
////};

class AssociationList : public AppendableList<Association *>{
public:
	AssociationList();
	void print();
//	RWCString theName();
};

class AssociationListIterator : public RWTValSlistIterator<Association *>{
public:
	AssociationListIterator( AssociationList &d );
};



class  ConnectRequest : public AssociationList{
public:
	ConnectRequest(Vertex * portv,ConnectRequests * r);

	Vertex * getVertex() const ;

	Association * insertEdge(Vertex * v,Edge * e);

	Association * getOne(Vertex * v);
	
	RWCString theName();

	Resource	* getResource() const ;

	void deleteOne(Association *& a);

	int isEmpty();
		
	int clean( Association *& x);
	int cleanAll( );

	void init (Vertex * portv, ConnectRequests * crs);

	void print();
	
private:
	Vertex * m_portv;
	ConnectRequests * m_ConnectRequests;
	Association * find(Vertex * uutv);

};

////class ConnectRequestIterator : public AssociationListIterator{
////public:
////	ConnectRequestIterator( ConnectRequest &d );
////};

class ConnectRequestList : public AppendableList<ConnectRequest *>{
public:
	ConnectRequestList();
};

////class ConnectRequestListIterator : public RWTValSlistIterator<ConnectRequest *>{
////public:
////	ConnectRequestListIterator( ConnectRequestList &d );
////};
////

class ConnectRequests : public ConnectRequestList {
public:
	ConnectRequests(Resource * r=0);
			

	ConnectRequest * insertEdge(Vertex * portv,Vertex * uutv,Edge * e);

	ConnectRequest * getOne(Vertex * portv);

	RWCString theName();
	
	void deleteOne(ConnectRequest *& a);

	int isEmpty();
		
	int clean(ConnectRequest *& x);
	int cleanAll( );

	Resource	* getResource() const ;
	
	void init ( Resource * r);
		
	void print();
				
private:
	ConnectRequest * find(Vertex * portv);

	Resource * m_Resource;
};

////class ConnectRequestsIterator : public ConnectRequestListIterator{
////public:
////	ConnectRequestsIterator( ConnectRequestList &d );
////};


class AssociationStack : public RWTStack< Association * , RWTValOrderedVector < Association * > >{
public:
	AssociationStack();
	Association * getOne(Vertex * w,ConnectRequest * cr);

private:
	int m_instantiated;
};


class AssociationListStack : public RWTStack< AssociationList * , RWTValOrderedVector < AssociationList * > >{
public:
	AssociationListStack();
	AssociationList * getOne();

private:
	int m_instantiated;
};

class ConnectRequestStack : public RWTStack< ConnectRequest * , RWTValOrderedVector < ConnectRequest * > >{
public:
	ConnectRequestStack();
	ConnectRequest * getOne(Vertex * portv, ConnectRequests * crr);

private:
	int m_instantiated;
};


class ConnectRequestListStack : public RWTStack< ConnectRequestList * , RWTValOrderedVector < ConnectRequestList * > >{
public:
	ConnectRequestListStack();
	ConnectRequestList * getOne();

private:
	int m_instantiated;
};

#endif // Association_H
