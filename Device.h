#ifndef Device_H
#define Device_H



class Device : public Resource{
public:
	Device( const RWCString & name,    const RWCString & version );
	Device( const RWCString & newName, Resource * source   );
	
	virtual Resource * clone(Resource * previous, const RWCString & newName);
	virtual	Vertex *checkLoopThruConnections(Vertex * v,DFSContext & c);
private:
	// Disable copy/assignment
        Device (const Device &);
        const Device & operator= ( const Device & );
};


#endif // Device_H
