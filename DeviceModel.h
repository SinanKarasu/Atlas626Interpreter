#ifndef DeviceModel_H
#define DeviceModel_H



class DeviceModel : public Resource{
public:
	DeviceModel( RWCString & name,    RWCString & version );
	DeviceModel( RWCString & newName, Resource * source   );
	
	virtual Resource * clone	( Resource * previous,RWCString & newName);
	virtual Resource * instantiate	( Resource * previous, RWCString & newName );
private:
	// Disable copy/assignment
        DeviceModel (const DeviceModel &);
        const DeviceModel & operator= ( const DeviceModel & );
};


#endif // DeviceModel_H
