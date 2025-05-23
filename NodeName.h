#ifndef NodeName_H
#define NodeName_H

class NodeName
{
public:
	NodeName(RWCString name,Resource * resource);
	operator const RWCString () const;

	Resource * getResource() const ;
	void setResource(Resource * resource);

	const RWCString & getName() const;
	const RWCString  theName() ; 

private:
	const RWCString m_name;
	Resource * m_Resource;
};


#endif // NodeName_H
