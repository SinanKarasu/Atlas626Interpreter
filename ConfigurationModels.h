#ifndef ConfigurationModels_H
#define ConfigurationModels_H



class ConfigurationModels : public Resource{
public:
	ConfigurationModels( Resource * previous);
	
	virtual Resource * clone(Resource * previous,RWCString & newName);

protected:
	ConfigurationModels( Resource * previous,Resource * source);

private:
	// Disable copy/assignment
        ConfigurationModels (const ConfigurationModels &);
        const ConfigurationModels & operator= ( const ConfigurationModels & );
};


#endif // ConfigurationModels_H
