#ifndef TPSHardware_H
#define TPSHardware_H



class TPSHardware : public Resource{
public:
	TPSHardware( const RWCString & name, const RWCString & version );
	TPSHardware( const RWCString & newName, Resource * source   );
	
	virtual Resource * clone(Resource * previous,const RWCString & newName);
private:
	// Disable copy/assignment
        TPSHardware (const TPSHardware &);
        const TPSHardware & operator= ( const TPSHardware & );
};


#endif // TPSHardware_H
