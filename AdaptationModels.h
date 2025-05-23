#ifndef AdaptationModels_H
#define AdaptationModels_H



class AdaptationModels : public Resource{
public:
	AdaptationModels( Resource * previous);
	
	virtual Resource * clone(Resource * previous,RWCString & newName);

protected:
	AdaptationModels( Resource * previous,Resource * source);

private:
	// Disable copy/assignment
        AdaptationModels (const AdaptationModels &);
        const AdaptationModels & operator= ( const AdaptationModels & );
};


#endif // AdaptationModels_H
