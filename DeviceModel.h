#ifndef DeviceModel_H
#define DeviceModel_H



class DeviceModel : public Resource{
public:
	DeviceModel( const RWCString & name, const RWCString & version );
	DeviceModel( const RWCString & newName, Resource * source   );
	
	virtual Resource * clone	( Resource * previous, const RWCString & newName);
	virtual Resource * instantiate	( Resource * previous, const RWCString & newName );
private:
	// Disable copy/assignment
        DeviceModel (const DeviceModel &);
        const DeviceModel & operator= ( const DeviceModel & );
};


#endif // DeviceModel_H
