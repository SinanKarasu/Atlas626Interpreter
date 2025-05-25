#ifndef ConfigurationModel_H
#define ConfigurationModel_H



class ConfigurationModel : public Resource{
public:
	ConfigurationModel( RWCString & name,    RWCString & version );
	ConfigurationModel( RWCString & newName, Resource * source   );
	
	virtual Resource * clone(Resource * previous,RWCString & newName);
private:
	// Disable copy/assignment
        ConfigurationModel (const ConfigurationModel &);
        const ConfigurationModel & operator= ( const ConfigurationModel & );
};


#endif // ConfigurationModel_H
