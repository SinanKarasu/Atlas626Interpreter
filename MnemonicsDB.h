#ifndef MnemonicsDB_H
#define MnemonicsDB_H


class MnemonicsDB	{

public:
	MnemonicsDB(const RWCString & nounDB,const RWCString & dimensionDB);
	
	//NounEntry * theNounEntry(const ANTLRTokenPtr  nid,int & howmany);

	NounEntry * theNounEntry(const RWCString noun);

	ModifierEntry * theModifierEntry(const std::string& text, NounEntry* nounEntry, std::string& last);

	ModifierEntry * theModifierEntry(const RWCString text,const RWCString noun,RWCString & left);

	DimensionDictionary * theDimensionDictionary(const RWCString did,ModifierEntry * modifierEntry);


	DimensionDictionary * theDimensionDictionary(RWCString quan);


	DimensionEntry * theDimensionEntry(const RWCString did,DimensionDictionary * dimensionDictionary);


	DimensionEntry * theDimensionEntry(const RWCString did,ModifierEntry * modifierEntry);

protected:

	//ModifierDictionary * LoadModifierDB(const RWCString filename);




private:
	char	getNext();
	RWCString		getDBToken();
	void			insertModifierQuantity(ModifierEntry *modifierEntry);
	void			insertSuffixes(ModifierEntry * modifierEntry);
	void			insertAllModifiers(ModifierEntry *modifierEntry,ModifierDictionary *modifierDictionary);
	void			insertNoun(NounDictionary * nounDictionary,ModifierDictionary * modifierDictionary);
	void			insertModifiers(NounEntry * nounEntry,ModifierDictionary * modifierDictionary);


	NounDictionary *	LoadAnalogNounDB(const RWCString filename,ModifierDictionary *& allModifiers);
	void			insertDimension(const RWCString quantity,DimensionDictionary * dimensionDictionary,DimensionDictionary *allDimensions);
	void			insertDimensionQuantity(QuantityDictionary * quantityDictionary,DimensionDictionary *allDimensions);
	QuantityDictionary *	LoadDimensionDB(const RWCString filename,DimensionDictionary * &allDimensions);

private:
	DimensionDictionary *	m_allDimensions;
	NounDictionary *	m_NounDictionary;
	QuantityDictionary *	m_QuantityDictionary;
	ModifierDictionary *	m_ModifierDictionary;
	const RWCString  	m_nounDB;
	const RWCString 	m_dimensionDB;
	std::ifstream		m_database;
	int			m_LineNo;
};


#endif // MnemonicsDB_H
