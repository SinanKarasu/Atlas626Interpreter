#ifndef DeviceModels_H
#define DeviceModels_H



class DeviceModels : public Resource{
public:
	DeviceModels( Resource * previous);
	
	virtual Resource * clone(Resource * previous,RWCString & newName);

protected:
	DeviceModels( Resource * previous,Resource * source);

private:
	// Disable copy/assignment
        DeviceModels (const DeviceModels &);
        const DeviceModels & operator= ( const DeviceModels & );
};


#endif // DeviceModels_H
