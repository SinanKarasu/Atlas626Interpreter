#ifndef DataBusTypeAST_h
#define DataBusTypeAST_h

#include	"AtlasAST.h"
#include	"InitList.h"
#include	"NounsModifiersDimensions.h"
#include	"TimerObject.h"


class	DataBusTypeAST : public AST{
	public:
		DataBusTypeAST(ANTLRTokenPtr	p=0,	TheType t=UndefinedTypeValue);
		DataBusTypeAST(AST	 *	a,	TheType t=UndefinedTypeValue);
		DataBusTypeAST(ANTLRTokenPtr  p,AST * a,TheType t=UndefinedTypeValue);
		virtual	~DataBusTypeAST();
	
		virtual AST *	eval	( AST * a=0  );
		virtual AST *	assign	( AST * a    );
		virtual	AST *	data	( AST *  a = 0 );
		virtual	Long	compare	( AST *  o     ) const;
		virtual AST *	check	( AST * a=0 );
		virtual TheType	getType	( AST * a=0  ) const;
		virtual AST *	Accept	( ASTVisitor & );
	protected:
		AST * m_AST;

	private:
		TheType	_storage;
};	

class	TestEquipMonitor : public DataBusTypeAST{
	public:
		TestEquipMonitor ( ASTList *  equipList=0 );

	private:
		ASTList * m_equipList;
};

class	ExchangeMonitor : public DataBusTypeAST{
	public:
		ExchangeMonitor ( ANTLRTokenPtr p=0 );

	private:
};

class	TestEquipBusRole : public DataBusTypeAST{
	public:
		TestEquipBusRole ( ASTList *  b);
		virtual	AST *	check	( AST * a );
		virtual Long	compare	( AST * o ) const;
		virtual	AST *	data	( AST * a = 0 );
		virtual	AST *	init	( AST * a = 0 );
		virtual void	print	( AST * a = 0 ) const;
		virtual AST *	assign	( AST * a    );


	private:
	ASTList * m_mmsList;
};

class	Talker : public DataBusTypeAST{
	public:
		Talker ( ASTList *  equipList=0);
		virtual AST *	check	( AST * a=0 );
		virtual	AST *	data	( AST *  a = 0 );

	private:
		ASTList * m_equipList;
};

class	Listener : public DataBusTypeAST{
	public:
		Listener ( ASTList *  equipList=0);
		virtual AST *	check	( AST * a=0 );
		virtual	AST *	data	( AST *  a = 0 );

	private:
		ASTList * m_equipList;
};


class	DataBusDevice : public DataBusTypeAST{
	public:
		DataBusDevice ( ANTLRTokenPtr p=0);
		DataBusDevice ( AST * a);

	private:
};

class	BusRedundancyMode : public DataBusTypeAST{
	public:
		BusRedundancyMode ( ANTLRTokenPtr p=0);

	private:
};

class	RedundantBus : public DataBusTypeAST{
	public:
		RedundantBus ( ANTLRTokenPtr p=0);

	private:
};

class	AlternateBusTransmit : public DataBusTypeAST{
	public:
		AlternateBusTransmit ( ANTLRTokenPtr p=0);

	private:
};

class	DataBusDevices : public DataBusTypeAST{
	public:
		DataBusDevices ( ANTLRTokenPtr p=0);
		DataBusDevices ( AST * a);
		virtual AST *	check	( AST * a=0 );

	private:
};

class	Command : public DataBusTypeAST{
	public:
		Command ( AST *  b = 0);

	private:
};

class	Data : public DataBusTypeAST{
	public:
		Data ( AST *  b = 0);

	private:
};

class	DataBusStatus : public DataBusTypeAST{
	public:
		DataBusStatus ( AST *  b = 0);

	private:
};

class	DataBusData : public DataBusTypeAST{
	public:
		DataBusData ( ANTLRTokenPtr p,AST * a);

	private:
};


class	ExchangeModels : public DataBusTypeAST{
	public:
		ExchangeModels ( ASTList *  b);
		virtual AST *	assign	( AST * a    );
		virtual	AST *	data	( AST *  a = 0 );
		virtual	Long	compare	( AST *  o     ) const;
		virtual AST *	check	( AST * a=0 );
		virtual AST 	*	init	( AST * a=0 );
		virtual void	print	( AST * a = 0 ) const;
		virtual AST	*	succ	( AST * a = 0 );

	public:
		ASTList		* m_exchangeModelList;
		ASTListIterator	* m_it;
};

class	ExchangeFrame : public DataBusTypeAST{
	public:
		ExchangeFrame ( AST * a=0 );

	private:
};

class	ExchangeSchedule : public DataBusTypeAST{
	public:
		ExchangeSchedule ( AST * a=0 );

	private:
};

class	ExchangeDetails : public DataBusTypeAST{
	public:
		ExchangeDetails ( AST * a=0 );

	private:
};

class	ExchangeContents : public DataBusTypeAST{
	public:
		ExchangeContents ( AST * a=0 );

	private:
};

class	RoleField : public DataBusTypeAST{
	public:
		RoleField ( AST * a=0 );

	private:
};

class	WatchDog : public DataBusTypeAST{
	public:
		WatchDog ( AST * a=0 );

	private:
};


class	TestEquipRoleName : public DataBusTypeAST{
	public:
		TestEquipRoleName ( ANTLRTokenPtr p, ModifierEntry * me,AtlasParser * parser=0 );
		virtual RWCString       getName() const;
		virtual Long		compare	( AST * o ) const;
		virtual AST 	*	check	( AST * a=0 );
		virtual AST 	*	init	( AST * a=0 );
		virtual	AST *	data	( AST * a = 0 );
		virtual void	print	( AST * a = 0 ) const;
		
	protected:
		ModifierEntry * m_modifierEntry;
		AtlasParser * m_parser;

	private:
};

class	BusModeName : public DataBusTypeAST{
	public:
		BusModeName ( ANTLRTokenPtr p, ModifierEntry * me,AtlasParser * parser =0);
		virtual RWCString       getName() const;
		virtual Long		compare	( AST * o ) const;
		virtual AST 	*	check	( AST * a=0 );
		virtual AST 	*	init	( AST * a=0 );
		virtual	AST *	data	( AST * a = 0 );
		virtual void	print	( AST * a = 0 ) const;

	protected:
		ModifierEntry * m_modifierEntry;
		AtlasParser * m_parser;
};

class	DataBusTransaction : public DataBusTypeAST{
	public:
		DataBusTransaction ( AST * a=0 );

	private:
};

class	BusParameterName : public DataBusTypeAST{
	public:
		BusParameterName( ANTLRTokenPtr p, ModifierEntry * modifierEntry, AtlasParser * parser = 0 );
		virtual RWCString       getName() const;
		virtual Long		compare	( AST * o ) const;
		virtual AST 	*	check	( AST * a=0 );
		virtual AST 	*	init	( AST * a=0 );
		virtual	AST *	data	( AST * a = 0 );
		virtual void	print	( AST * a = 0 ) const;
		
	protected:
		ModifierEntry * m_modifierEntry;
		AtlasParser * m_parser;
};


class	BusParameter : public DataBusTypeAST{
	public:
		BusParameter( AST * a);
		
};

class	BusParameters : public DataBusTypeAST{
	public:
		BusParameters( AST * a = 0 );
		
};

class	ProtocolParameter : public DataBusTypeAST{
	public:
		ProtocolParameter( AST * a );
}		
;

class	ProtocolParameters : public DataBusTypeAST{
	public:
		ProtocolParameters( AST * a = 0 );
		
};

class	EntireData : public DataBusTypeAST{
	public:
		EntireData ( AST * a );
};

class	DataPump : public DataBusTypeAST{
	public:
		DataPump (AST * a );
};

class	ExchangeDefinition : public DataBusTypeAST{
	public:
		ExchangeDefinition ( AST * a = 0 );
		virtual AST 	*	check	( AST * a=0 );
		
};

class	DefineExchangeFields : public DataBusTypeAST{
	public:
		DefineExchangeFields (AST * a = 0  );
};

class	ExchangeDevices : public DataBusTypeAST{
	public:
		ExchangeDevices (AST * a = 0  );
};


#endif // DataBusTypeAST_h
