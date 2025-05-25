#ifndef AdaptationModel_H
#define AdaptationModel_H



class AdaptationModel : public Resource{
public:
	AdaptationModel( RWCString & name,    RWCString & version );
	AdaptationModel( RWCString & newName, Resource * source   );
	
	virtual Resource * clone(Resource * previous,RWCString & newName);
private:
	// Disable copy/assignment
        AdaptationModel (const AdaptationModel &);
        const AdaptationModel & operator= ( const AdaptationModel & );
};


#endif // AdaptationModel_H
