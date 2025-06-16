//! SYNTAX-RULES
// The comment on the right hand side is the 
// Section Number in this ATLAS Standard for the 
// textual definition of the FSD Production Rule
// for each ATLAS Construct/
  
  
//  18.6.1.  ATLAS TEST SPECIFICATION STRUCTURE


class AtlasParser {
<<
public:
	Scope *	getScope()		;
	void	setScope(Scope * s)	;
	
	AST * getTokenLabel (const ANTLRChar * name,const Scope * s=0) const ;
	AST * getTokenLabelGlobal (const ANTLRChar * name,const Scope * s=0) const ;
	AST * getEventIndicatorLabel	( const ANTLRChar * name, const Scope * s = 0 ) const;

	virtual void syn(
				ANTLRAbstractToken	*	tok,
				ANTLRChar		*	egroup,
				SetWordType		*     	eset,
				ANTLRTokenType			etok,
				int 				k
			);

	virtual void edecode(SetWordType *a);
	
	
private:

	AST *	isType(ANTLRChar *,LabelType);
	AST *	isEventIndicator(ANTLRChar *);
	AST *	isTypeGlobal(ANTLRChar *,LabelType);
	Scope *	getScope(ANTLRTokenPtr);
	
	LabelType getLabelType		( const RWCString & name, Scope * s=0 ) const;
	LabelType getLabelTypeGlobal	( const RWCString & name, Scope * s=0 ) const;
	TheType getType		( ANTLRChar * name );
	TheType getGlobalType	( ANTLRChar * name );

	AST * insertFstatno(ANTLRTokenPtr t, Scope * s);	
	RWTStack<Scope *,RWTValOrderedVector<Scope *> > scope_stack;


	int FlagStatement;
	
	int VerbLineNo;
	int _prev_test;
	int _prev_step;
	LineAction *_prev_LineAction;
	int VerbStatNo;
	Scope *	scope,*globalScope,*externalScope;
	FstatnoDictionary * modulestatscope;
	int ContextDepth;
	int MaxContextDepth;
	void incrementContextDepth(AST * a=0,EntryType et=EntryUndefined);
	void incrementContextLevel();
	void decrementContextDepth();
	
	void insertTarget(RWInteger sno,AST * a);
	void insertUnresolved(RWInteger sno,AST * a);
	void ResolveTargets();	
	
	RWTValVector<RWInteger>	ContextLevel;
	RWTValVector<Scope *>	StatementScopes;
	RWTValVector<AST *>	LevelEntry;
	RWTValVector<EntryType> LevelType;
	RWTValVector<int> LevelSnum;
	
	AST * verifyGoToTarget(TargetStatement * unresolved,TargetStatement * target);
	AST * verifyLeaveResumeTarget(AST * snum,EntryType lt);
	
	GoToDictionary		UnresolvedTargets;
	EntryDictionary		GoToTargets;
	
	RWCString	CurrentProcedureOrFunction;
	AST *		CurrentProcedureOrFunctionLabel;
	//	RWCString CurrentFunction;
	
	//	SignalWatcher * m_kb_watcher;
	AST * lManualInterventionEvent;

	int nounAllowsDescriptors(const NounEntry * nounEntry);
private:
	AST *lMEAS,	*lHI,	*lLO,	*lGO,	*lNOGO,	*lMANUAL,	*lMAXdTIME;
	ANTLRTokenType parserMode;
	ASTList * allTimers;
	ASTList * allEscapes;
public:
	void setGO	()	;
	void setNOGO	()	;
	void setHI	()	;
	void setLO	()	;
	
	int getGO	()	;
	int getNOGO	()	;
	int getHI	()	;
	int getLO	()	;
	
	void insertEscape(AST * ei,AST * ep);

	void removeEscape(AST * ei);
	ModifierEntry * theModifierEntry(const RWCString modifier,NounEntry * nounEntry,RWCString & left);

protected:
	std::ofstream	tracefile;
	
	virtual void tracein(const char *r);
	virtual void traceout(const char *r);
private:
	int isType(ANTLRTokenPtr ,ModifierEntry * m,const RWCString  a);

	int isType(ModifierEntry * m,const RWCString  a);

	int isType(ModifierEntry * m,const RWCString a,const RWCString  b);

	int isType(ModifierEntry * m,const RWCString a,const RWCString  b,const RWCString c);

	ModifierEntry * insertQuotedModifier(const RWCString quoted,NounEntry * nounEntry);

	//DimensionDictionary * allDimensions;
	//NounDictionary * nounDictionary;
	//QuantityDictionary * quantityDictionary;
	//ModifierDictionary * modifierDictionary;
	MnemonicsDB * mnemonicsDB;
NounEntry *
theNounEntry(const ANTLRTokenPtr  nid,int & howmany);

TheType	getModifierType(AST * re,AST * cm);

void reset();
	
Scope * getGlobalScope() const;
Scope * getExternalScope() const;

public:
	void init();
	void done();


	// debug flags
	int debug_flag;

//#include "theEntries.h"
>>

atlas_initialization![const RWCString & dbd]:
		<< // gotta be an init action... These stuff are needed globals........
		
			//nounDictionary		= LoadAnalogNounDB("Noun.db",modifierDictionary);
			//quantityDictionary	= LoadDimensionDB("Dimension.db",allDimensions);
			
			
			mnemonicsDB= new MnemonicsDB(dbd+"Noun.db",dbd+"Dimension.db");

			// scope info
        		scope			= new GlobalScope;
        		globalScope		= scope;
        		externalScope=0;

			reset();

			// 'MEASUREMENT',HI,LO,GO,NOGO labels...
			lMEAS	= 0;
			lHI	= 0;
			lLO	= 0;
			lGO	= 0;
			lNOGO	= 0;

			//parserMode=DLGminToken;

			allTimers=new ASTList;
	


			scope->insert_label( (AST *)
 				#(new FileLabel( new AtlasToken(AtlasTokenType(0),"stdin" )), new FileType(0) )
        		);
			scope->insert_label( (AST *)
 				#(new FileLabel( new AtlasToken(AtlasTokenType(0),"stdout" )), new FileType(0) )
        		);
			scope->insert_label( (AST *)
 				#(new FileLabel( new AtlasToken(AtlasTokenType(0),"stderr" )), new FileType(0) )
        		);
	lMEAS	=	scope->insert_label( (AST *)
 				#(new VariableIdentifierLabel(new AtlasToken(AtlasTokenType(0),"'MEASUREMENT'" )),new DecimalNumber)
        		);
	lHI	=	scope->insert_label( (AST *)
 				#(new VariableIdentifierLabel(new AtlasToken(AtlasTokenType(0),"HI" )),new BooleanType)
        		);
	lLO	=	scope->insert_label( (AST *)
 				#(new VariableIdentifierLabel(new AtlasToken(AtlasTokenType(0),"LO" )),new BooleanType)
        		);
	lGO	=	scope->insert_label( (AST *)
 				#(new VariableIdentifierLabel(new AtlasToken(AtlasTokenType(0),"GO" )),new BooleanType)
        		);
	lNOGO	=	scope->insert_label( (AST *)
 				#(new VariableIdentifierLabel(new AtlasToken(AtlasTokenType(0),"NOGO" )),new BooleanType)
        		);
	lMANUAL	=	scope->insert_label( (AST *)
 				#(new VariableIdentifierLabel(new AtlasToken(AtlasTokenType(0),"MANUAL INTERVENTION" )),new BooleanType)
        		);
	lMAXdTIME=	scope->insert_label( (AST *)
 				#(new VariableIdentifierLabel(new AtlasToken(AtlasTokenType(0),"MAX-TIME" )),new BooleanType)
        		);
        		ANTLRTokenPtr  earth=new AtlasToken(AtlasTokenType(EARTH),"EARTH" );
			scope->insert_label( (AST *)
 				#(new ConnectionTypeLabel(earth),	new ConnectionType(earth))
        		);
        		ANTLRTokenPtr  common=new AtlasToken(AtlasTokenType(COMMON),"COMMON" );
			scope->insert_label( (AST *)
 				#(new ConnectionTypeLabel(common),	new ConnectionType(common))
        		);
        		ANTLRTokenPtr  atmos=new AtlasToken(AtlasTokenType(ATMOS),"ATMOS" );
			scope->insert_label( (AST *)
 				#(new ConnectionTypeLabel(atmos),	new ConnectionType(atmos))
        		);
        		
        		lManualInterventionEvent = new EventIndicatorType(0,lMANUAL);

			//----------------------------------------------------------------------//
			// Following enables SIGINT delivery as MANUAL INTERVENTION....		//
			// This is disabled right now and is done thru the GUI or some other	//
			// mechanism.								//
			// m_kb_watcher=new SignalWatcher(SIGINT,lManualInterventionEvent);	//
			//----------------------------------------------------------------------//

        >>
	;
	
//atlas![RWCString file_name]:
//		atlas_test_specification_structure[file_name]
//		<<
//			ResolveTargets(); // Resolve the Unresolved GOTO targets.
//		>>
//			Eof
//	;
 
//atlas_test_specification_structure[RWCString file_name]:              // fsd-- 3.1
//	atlas_program_structure[file_name]
//	(
//		(  atlas_module_structure[file_name] )*
//	|	(  non_atlas_module_structure )*
//	)
//   ;
// &*  Mandatory. * 
// &** Optional; none or any number. * 
  
  
atlas_program_structure[RWCString file_name]:                         // fsd-- 3.1.1
        <<
        	scope		=	globalScope;
        	scope		=	new ExternalScope(scope);
        	externalScope	=	scope;
        	scope		=	new ProgramScope(file_name,scope,this);
        	modulestatscope	=	scope->getFstatnoDictionary();
        	reset();
        >>
	begin_atlas_program_statement
	program_preamble_structure
	commence_main_procedure_statement
	main_procedural_structure
	terminate_atlas_program_statement
	<<
		ResolveTargets(); // Resolve the Unresolved GOTO targets.
		//#0=#atss;
		scope=scope->getPrev();
	>>
	Eof!
	;
  
  
atlas_module_structure[RWCString file_name]:                          // fsd-- 3.1.2 
        <<
        	scope		=	globalScope;
        	scope		=	new ExternalScope(scope);
        	externalScope	=	scope;
        	scope		=	new ModuleScope(file_name,scope,this);
        	modulestatscope	=	scope->getFstatnoDictionary();
        	reset();
        >>
	begin_atlas_module_statement
	module_preamble_structure
	terminate_atlas_module_statement
	<<
		ResolveTargets(); // Resolve the Unresolved GOTO targets.
		scope=scope->getPrev();
	>>
	Eof!
	;
 
  
// non_atlas_module_structure :;                  // fsd-- 3.1.3 
  
// The definitions of non_atlas_module_structure 
// and its combination with atlas_module_structure 
// are outside the scope of this formal definition 

 
// 18.6.2. STRUCTURE DELIMITER STATEMENT
  
  
begin_atlas_program_statement!:                  // fsd-- 4.1.1     
	verb:fstatnoBEGINfd ATLAS PROGRAM     
	{
		id:ID
		<< scope->insert_label( new ProgramNameLabel($id));>>
	}
	<< #0=#(#verb,new ExecuteProgramAction($id,scope,this));>>
	statement_terminator
	;     
  
  
terminate_atlas_program_statement!:              // fsd-- 4.1.2     
	verb:fstatnoTERMINATEfd ATLAS PROGRAM     
	{ program_name }     
	<<#0=#verb;>>
	statement_terminator 
	;     
  
  
begin_atlas_module_statement!:                   // fsd-- 4.1.3
	verb:fstatnoBEGINfd ATLAS MODULE id:ID
		<<scope->insert_label(new ModuleNameLabel($id));>>
	<< #0=#verb;>>
	statement_terminator 
	;
  
  
terminate_atlas_module_statement:               // fsd-- 4.1.4
	verb:fstatnoTERMINATEfd ATLAS MODULE      
	module_name
	statement_terminator 
	;
  
 
block_structure!:                                 // fsd-- 4.2.1 
	<< ANTLRTokenPtr bbeg,bend;>>
	bbs:begin_block_statement>[bbeg]
		<<
        		scope	= new BlockScope(scope,#bbs);
			incrementContextDepth(#bbs,EntryBLOCK);
		>>
	bb:block_body
	ebs:end_block_statement>[bend]
		<<
			#0=#(0,#bbs,new TargetAction(#ebs),#bb,#ebs);
			decrementContextDepth();
			if((bbeg!=0) && (bend!=0)){ // names have been given
				if(RWCString(bbeg->getText())!=RWCString(bend->getText())){
					Error_Report( "BLOCK BEGIN/END names do not match ", bend );
				}
			}
			scope=scope->getPrev();
		>>
 ;
 

begin_block_statement!>[ANTLRTokenPtr bbeg]:                          // fsd-- 4.2.2
	<<	AST * x=0;	>>
	verb:fstatnoBEGINfd BLOCK    
	{ Fd id:ID <<x=scope->insert_label(new BlockNameLabel($id));>> } 
	<<
		#0=#(0,#verb,#(new BeginBlockAction($id),x));
		$bbeg=$id;
	>>
	statement_terminator
  ;     
  

 
block_body:                                      // fsd-- 4.2.3 
	{ local_preamble_structure! }
	main_procedural_structure
	;
      
leave_block_statement!:                          // fsd-- 4.2.4     
	blk:BLOCK     
		{ STEP st:statement_number }     
		<<#0=#(new LeaveAction($blk,scope),verifyLeaveResumeTarget(#st,EntryBLOCK));>>
		statement_terminator
	;     
  
  
end_block_statement!>[ANTLRTokenPtr bend]:                            // fsd-- 4.2.5     
	<<	AST * x=0;	>>
	verb:fstatnoENDfd BLOCK     
	{ Fd id:ID <<x=getTokenLabel($id->getText());>> } 
	statement_terminator
	<<
		#0=#(#verb,#(new EndBlockAction($id)));
		$bend=$id;
	>>
	;     
  
  
commence_main_procedure_statement!:              // fsd--  4.3     
	verb:fstatnoCOMMENCEfd MAIN PROCEDURE	<< #0 = #verb; >>
	statement_terminator 
	;     
   

// 18.6.3 PREAMBLE STATEMENTS
  
program_preamble_structure:                      // fsd-- 6.1.1
	( include_statement )*
	preamble_fixed_sequence 
	preamble_variable_sequence
 ; 
 
 
preamble_fixed_sequence:                         // fsd-- 6.1.1-A
	( extend_atlas_statement )*  ( require_statement )* 
	;
	
// &*    This path is used only in those unusual applications
// &     when no signal resources are demanded by the program or module.
// &     When the program or module demands real signal resources for
// &     its implementation the ATLAS test requirement must be
// &     written in terms of virtual resources as defined by
// &     a <require statement> $. *
 
  
preamble_variable_sequence:                      // fsd-- 6.1.1-B
	( 
		declare_statement
	|	establish_protocol_statement
	)*
	( 
		define_statement_structure
	|	identify_statements
	)* 
	;
 
module_preamble_structure!:                       // fsd-- 6.1.2
	preamble_fixed_sequence
	preamble_variable_sequence
	; 


local_preamble_structure:                        // fsd-- 6.2
	( declare_statement )+ ;

declare_statement!:                              // fsd-- 6.3
	verb:fstatnoDECLAREfd 
	( 
		constant_declaration
	|	type_declaration
	|	vd:variable_declaration
			<<#0=#vd;>>
	)     
	statement_terminator
	;     
  
  
constant_declaration!:                          // fsd-- 6.3A
	CONSTANT Fd
		(
			aa_constant_declaration_1
			(
				Semi_Colon aa_constant_declaration_1
			)*
		)
	;

aa_constant_declaration_1!:
		id:ID 
		IS c:constant 
		<<#(scope->insert_label(new ConstantIdentifierLabel($id)),#c);>>
	;
     
        
type_declaration!:                              // fsd-- 6.3B
		TYPE Fd  ti1:type_identifier_New IS t1:type
			<< #(#ti1,#t1);>>
			(
				Semi_Colon  tin:type_identifier_New IS tn:type
					<< #( #tin, #tn );>>
			)*
	;

type_identifier_New!:
		id:ID << #0 = scope->insert_label(new TypeIdentifierLabel($id));>>
	;
            
variable_declaration!:                               // fsd-- 6.3C
	<<
		Scope * which=0;
	>>
		external_global_fd>[which]
		VARIABLE Fd vt1:variable_type[which]
			<< #0 = #vt1; >>
			(
				Semi_Colon vtn:variable_type[which]
				<< #0 = #0 ? #(0,#0,#vtn) : #(0,#vtn); >>
			)*
	;     
  
  
variable_type![Scope * which]:                                  // fsd-- 6.3D
		<<	
			ASTList *Label_List=0;      
			InitList *Init_List=0;
		>>   
		id1:ID
		<<
			Label_List=new ASTList;
			Label_List->insert(new VariableIdentifierLabel($id1));
		>>
		(
			Fd idn:ID
			<< Label_List->insert(new VariableIdentifierLabel($idn)); >>
		)*
		IS t:type { << Init_List=new InitList;>> initial[Init_List] }
		<<
		ASTListIterator var_it = Label_List->begin();
   		int more = 0;

		for (; var_it != Label_List->end(); ++var_it, more = 1) {
    		AST* p = *var_it;
        	if (sane()) {
            	#( p, more ? #t->clone() : #t );
            	which->insert_label(p, scope);
        	} else {
            	delete p;
        	}
    	}

			if(sane()) {
				if ( Init_List->empty() ){
					delete Label_List;
					delete Init_List;
				}else{
					if(which==getExternalScope()){
						Error_Report("Can not INITIALIZE EXTERNAL variables",VerbLineNo);
					}
					#0 = #( 0, new VarInitAction( Label_List, Init_List ) );
					((AST *)#0)->eval();		// make sure that it works....
				};
			} else {
				delete Label_List;
				delete Init_List;
			}
		>>
	;

	 
type!:                                           // fsd-- 6.3E
		ti:type_identifier	<< if(sane()){#0 = #ti->eval()->clone( scope );} >>
	|	bt:base_type		<< if(sane()){#0 = #bt;} >>
	|	pdt:pre_declared_type	<< if(sane()){#0 = #pdt;} >>
	|	st:structured_type	<< if(sane()){#0 = #st;} >>
	;

enum_type!:
		<<
			ASTList  * Enum_List;
			StringVector  evec;
			long	pos=0;
		>>
    		ENUMERATION LP
			ee1:enumeration_element_New
				<<
					Enum_List= new ASTList;
					#(#ee1,#(new EnumerationType( #ee1->getToken(), pos++ ), new EnumerationsType(Enum_List)));
					Enum_List->insert( #ee1->ASTdown() );
					evec.push_back( #ee1->getName() );
				>>
				
			 	(
			 		Fd  een:enumeration_element_New
						<<
							if ( !evec.contains(#een->getName()) )
							{
								#(#een,#(new EnumerationType(#een->getToken(),pos++),new EnumerationsType(Enum_List)));
								Enum_List->insert(#een->ASTdown());
								evec.push_back(#een->getName());
							}else{
								Error_Report( "Duplicate Enumeration ", #een );
							}							
						>>
			 	)*
			RP
			<<
				#0 = #( new EnumerationType, new EnumerationsType(Enum_List) );
			>>
	;
	<<
		delete Enum_List;
	>>

conn_type!:
	<<
		SymbolDictionary  * Conn_List=0;
		StringVector  *cvec=0;
		long	pos=0;
	>>
    	CONNECTION LP
		cl1:connection_label
			<<
				Conn_List= new SymbolDictionary;
				cvec=new StringVector;
////				Conn_List->insertKeyAndValue(#cl1->getName(),#cl1);
				(*Conn_List)[#cl1->getName()] = #cl1;

				cvec->push_back(#cl1->getName());
			>>
			
		 	(
		 		Fd  cln:connection_label
					<<
						if(!cvec->contains(#cln->getName()))
						{
							Conn_List->insertKeyAndValue( #cln->getName(),#cln);
							cvec->append( #cln->getName() );
						}else{
							Error_Report( "Duplicate Connection ", #cln );
						}							
					>>
		 	)*
		RP
		<<
			#0 = #( new ConnectionType, new ConnectionsType(Conn_List) );
			delete cvec;
		>>
	;
	<<
		delete Conn_List;
		delete cvec;
	>>
	
integer_type!:
		integ:INTEGER	<< #0 = new IntegerNumber($integ); >>;

decimal_type!:
		deka:DECIMAL	<< #0 = new DecimalNumber($deka); >>;
	
char_type!: 
		ct:CHAR		<< #0 = new CharType($ct); >>;

bit_type!: 
		bt:BIT		<< #0 = new BitType($bt); >>;

base_type:                                      // fsd-- 6.3F
		enum_type
	|	conn_type
	|	integer_type
	|	decimal_type
	|	char_type
	|	bit_type
	;	

enumeration_element_New!:
		id:ID	<< #0 = scope->insert_label( new EnumerationElementLabel( $id ) ); >>;
	
pre_declared_type!:                              // fsd-- 6.3G
	(
		b:BOOLEAN	<< #0 = new BooleanType($b); >>
	|	c:CHARdCLASS	<< #0 = new BooleanType($c); >>
	|	d:DIGdCLASS	<< #0 = new DigClassType($d); >>
	)
	;
 
 
structured_type!:                                // fsd-- 6.3H
	//<<ASTList * Rec_List=0;>>
	( 
		ARRAY LP ai:array_indices RP OF t:type
			<<
				if(#t){	// or maybe if(sane()){ .....
					#0 = #ai;
					//((ArrayType *)#0)->insert(#t);
					#ai->insert(#t);
				}
			>>
	|
		RECORD  OF  LSQUARE
			<<
				scope = new RecordScope(scope);
				//Rec_List = new ASTList;
			>>
			rf1:record_field_type
				//<< Rec_List->insert( #rf1 ); >>
				<<	scope->insertSymbolValue(#rf1);	>>
				(
					Semi_Colon rfn:record_field_type
					//<< Rec_List->insert( #rfn ); >>
					<<	scope->insertSymbolValue(#rfn);	>>
				)* 
				RSQUARE
				<<
					#0 = new RecordType( scope /*, Rec_List*/ );
					scope = scope->getPrev();
				>>
	|
		str:STRING LP c:integer_constant RP  OF
			(	BIT	<< #0 = new StringOfBitType( $str, #c->getInteger() ); >>
			|	CHAR	<< #0 = new StringOfCharType( $str, #c->getInteger() ); >>
			)
	|
		f:ZZFILE OF
			(	ft:type		<< #0 = #( new FileType( $f ), #ft ); >>
			|	txt:TEXT	<< #0 = #( new FileType( $f ), new TextType( $txt ) ); >>
			) 
	)
	;
 
array_indices!:
 	lo:constant THRU hi:constant
		{ Fd ai:array_indices }
			<< #0 = #( new ArrayType( #ai, #lo->getInteger(), #hi->getInteger() ) ); >>
	;		


record_field_type:
		<< ASTList *Label_List=0; >>
		
		rf1:record_field_New
			<<
				Label_List = new ASTList;
				Label_List->insert( #rf1 );
			>>
			
			(
				Fd rfn:record_field_New
					<< Label_List->insert( #rfn ); >>
			)* 
			IS t:type
			<<
				ASTListIterator var_it = Label_List->begin();

////				for ( int more_objects = 0; ++var_it; more_objects = 1 )
////				{
////					if ( more_objects ) {
////						#( *var_it.key, #t->clone() );
////					} else {
////						#( *var_it.key, #t );
////					}
////				}
				int more_objects = 0;
				for ( ASTListIterator var_it = Label_List->begin(); var_it != Label_List->end(); ++var_it, more_objects = 1 )
				{
				    AST* temp_ast = *var_it;
					if ( more_objects ) {
						#( temp_ast, #t->clone() );
					} else {
						#( temp_ast, #t );
					}
				}

			>>
        ;     

record_field_New!:
		id:ID 
		//<< #0 = scope->insert_label( new VariableIdentifierLabel($id) ); >>
		<< #0 = #( new VariableIdentifierLabel($id) ); >>
	;
 
initial![InitList  * Init_List]:                                        // fsd-- 6.3I     
		INITIAL EQUALS initialiser[Init_List]
	;

initialiser![InitList  * Init_List]:
		initialiser_clause[Init_List] (Fd initialiser_clause[Init_List])*
 	;
 	
initialiser_clause![InitList  * Init_List]:                                   // fsd-- 6.3J
		<<InitList * init_list;InitData * init_data;>>
		rep:integer_constant OF     
		( 
			data1:constant
			<<
				init_data=new InitData(#rep,#data1,0);
				Init_List->append(init_data);
				
			>>
		|	LP
				<< init_list=new InitList;>>
				initialiser[init_list]
				RP
				<<
					if ( init_list->isEmpty() )
					{
						delete init_list;
					}else{
						init_data = new InitData( #rep, 0, init_list );
						Init_List->append( init_data );
					}
				>>
		)
	|	LP
			<< init_list=new InitList;>>
			initialiser[init_list]
			RP
			<<
				if(init_list->isEmpty()){
					delete init_list;
				} else {
					init_data=new InitData(new IntegerNumber(1),0,init_list);
					Init_List->append(init_data);
				}
		>>
	|	data2:constant
		<<
				init_data=new InitData(new IntegerNumber(1),#data2,0);
				Init_List->append(init_data);
				//cout << Init_List->entries() << endl;
		>>
	;
  

define_statement_structure!:                      // fsd-- 6.4
	verb:fstatnoDEFINEfd id:ID Fd
	(
		define_drawing_statement[#verb,$id]
	|	define_signal_statement[#verb,$id]
	|	define_procedure_structure[#verb,$id]
	| 	define_function_structure[#verb,$id]
	|	define_exchange_statement[#verb,$id]
	|	define_exchange_configuration_statement[#verb,$id]
	|	define_digital_configuration_structure[#verb,$id]
	)
  	;
 
define_drawing_statement![AST * verb,ANTLRTokenPtr id]:                       // fsd-- 6.5
	//-------//verb:fstatnoDEFINEfd id:ID  Fd
		DRAWING
	<<scope->insert_label(new DrawingLabel(id));>>
	Fd drawing_identifier                //
	drawing_definition
	statement_terminator
	;
  
  
drawing_definition:                             // fsd-- 6.5-1
	{
		Fd TERMINALS ( id:CID <<scope->insert_label(new TerminalIdentifierLabel($id));>> )+ 
		{Fd conn[0] }
	}
	;
 
 
define_signal_statement![AST * verb,ANTLRTokenPtr id]:                        // fsd-- 6.6
	//-------//verb:fstatnoDEFINEfd id:ID  Fd
		SIGNAL
	<<scope->insert_label(new SignalLabel(id));>>
	Fd
	//  fields
	statement_terminator
	;
 

// fields is not defined further,
// NOTE in Section 6.8.3 refers.
 
 
define_procedure_structure![AST * verb,ANTLRTokenPtr id]:                      // fsd-- 6.7.1
	<<
		ANTLRTokenPtr procdef,procend;
		Scope * which;
	>>
	dps:define_procedure_statement[verb,id]>[procdef,which]
	(
		(<<which==getExternalScope()>>? ())?
	|
		<<incrementContextDepth(#dps,EntryPROCEDURE);>>
		code:procedure_body
		eps:end_procedure_statement>[procend]
			<<
				#(0,#dps,#code,#eps); // nothing to return.... disconnected tree
				decrementContextDepth();
				if(RWCString(procdef->getText())!=RWCString(procend->getText())){
					Error_Report( "PROCEDURE names do not match ", procend );
				}
			>>
	)
	;  
// &*   This bypass must be used if the <define
// &    procedure statement> has the EXTERNAL
// &    attribute.  It must not be used otherwise.*
 
 
  
define_procedure_statement![AST * verb,ANTLRTokenPtr id]>[ANTLRTokenPtr procdef,Scope * which]:                     // fsd-- 6.7.2
	<<
		Scope * ts;
		AST * proc;
		int argcount=0,rescount=0;
	>>      
	//-------//verb:fstatnoDEFINEfd  id:ID Fd
		external_global_fd>[$which] PROCEDURE
		<<
			ts = new ProcedureScope(scope);		// get a new scope
			scope=ts;			// make procedure scope the current scope
			verb->setScope(scope);		// needed for LineAction::trace()
			$procdef=id;
		>> 
		pd:parameter_definition[argcount,rescount]    
		<<
			AST * newproc=new ProcedureLabel(id,this,scope);
			scope->setParameterMode(0);
			newproc->setInteger(0,argcount); // set the argcount
			newproc->setInteger(1,rescount); // set the result count
			if(#pd){
				#0=#(0,#(newproc,#pd),verb);
			} else {
				#0=#(0,#(newproc,#(new Proxy,#(new Proxy,new Proxy))),verb);
			}
			proc=$which->get_label(newproc);
			if(proc){
				Error_Report("Unable to create procedure:",newproc);
			} else {
				proc=$which->insert_label(newproc,scope);
			}
			
			CurrentProcedureOrFunction=id->getText();
			CurrentProcedureOrFunctionLabel=proc;
		>>
		statement_terminator
		<<
			if($which==getExternalScope()){
				scope=scope->getPrev();	// no body will be defined
			}
		>>
  ;     

  
parameter_definition![int &argcount,int &rescount]:                           // fsd-- 6.7.2A     
	{	LP
			<<
				scope->setParameterMode(1);	// we will get the parameters
			>>
	
			fp1:formal_parameter[argcount]
			<<
				#0=#(0,#(new Proxy,#(new Proxy,new Proxy)),#fp1);// need Proxy for it's down field
				//            ^         ^        ^
				//          ->fp1      ->rp1	saves		// this is where they go
				//
				//	Proxy-----> #fp1 --> .... --> #fpn
				//	  |
				//	  v
				//	Proxy-----> #rp1 --> .... --> #rpn
				//	  |
				//	  v
				//	Proxy-----> save1--> .... --> saven
				//
				//
			>>
    		(
    			Fd fpn:formal_parameter[argcount]
				<<
					#(0,#0,#fpn);	// add fp2,...
				>>
    		)*  
    		RP 
	}     
		{
			<<int saves=0;>>
		RESULT LP 
			rp1:formal_parameter[rescount]
			<<
				if(!#0)#0=#(0,#(new Proxy,#(new Proxy,new Proxy)));
				#(0,#0->down(),#rp1);
				for(;saves<rescount;saves++){
					#(0,#0->down()->down(),new Proxy);	// save location
				}
			>>
			(
				Fd rpn:formal_parameter[rescount]
				<<
					#(0,#0->down(),#rpn);
					for(;saves<rescount;saves++){
						#(0,#0->down()->down(),new Proxy);	// save location
					}
				>>
			)* 
			 RP 
		}
	;     

  
// Subsequent usage of parameters in the FSD must     
// comply with the declared type.  Therefore     
// <parameter does not normally occur in the FSD.     
  

 
procedure_body:                                  // fsd-- 6.7.3 
	{local_preamble_structure}
 	main_procedural_structure 
	; 


leave_procedure_or_function_statement!:                      // fsd-- 6.7.4
	id:ID 
	<<
		if(CurrentProcedureOrFunction!=$id->getText()){
			Error_Report(" Illegal Leave Procedure ",$id);
			// Note that returning the Func/Proc Label will cause the
			// Execute Loop to end....
		}
		#0=#(new GoToAction($id,scope),new TargetAction(CurrentProcedureOrFunctionLabel));
	>>
	statement_terminator 
	;
  
  
end_procedure_statement!>[ANTLRTokenPtr procend]:                        // fsd-- 6.7.5     

	verb:fstatnoENDfd      
		<<
			scope=scope->getPrev();
		>>
		id:ID
	statement_terminator
		<<
			$procend=$id;
			CurrentProcedureOrFunction="";
			CurrentProcedureOrFunctionLabel=0;
		>>
	;     
  


define_function_structure[AST * verb,ANTLRTokenPtr id]:	// fsd-- 6.8.1 
	<<
		ANTLRTokenPtr fndef,fnend;
		Scope * which=0;
	>>
	dfs:define_function_statement[verb,id]>[fndef,which]
	(
		(<<which==getExternalScope()>>? ())?
	|	<<incrementContextDepth(#dfs,EntryFUNCTION);>>
		function_body 
		end_function_statement>[fnend]
		<<
			decrementContextDepth();
			if(RWCString(fndef->getText())!=RWCString(fnend->getText())){
				Error_Report( "Function names do not match ", fnend );
			}
		>>
	)
	;  

define_function_statement![AST * verb,ANTLRTokenPtr id]>[ANTLRTokenPtr fndef,Scope * which]:                      // fsd-- 6.8.2     
	<<
		Scope * ts;
		AST * func;
		int argcount=0;
	>>      
	//-------//verb:fstatnoDEFINEfd  id:ID Fd
		external_global_fd>[$which] FUNCTION    
		<<
			ts = new FunctionScope(scope);	// get a new scope
			scope=ts;			// make function scope the current scope
			verb->setScope(scope);		// needed for LineAction::trace()
			$fndef=$id;
		>>
	{
		LP
			<<
				scope->setParameterMode(1);	// we will get the parameters
			>>
		
		fp1:formal_parameter[argcount]
		<<
			#0=#(0,#fp1);
		>>
		(
			Fd fpn:formal_parameter[argcount]
				<<
					#0=#(0,#0,#fpn);
				>>
		)*
		RP
		<<    
			scope->setParameterMode(0);
		>>
	}
	t:type
	<<
		AST * newfunc=new FunctionLabel(id,this,scope);
		newfunc->setInteger(0,argcount); // set the argcount
		#0=#(newfunc,#0); // make the args stuff down, body will be siblings....
		func=$which->get_label(newfunc);// in previous scope insert it as a function
		if(func){
			Error_Report("Unable to create function:",newfunc);
		} else {
			func=$which->insert_label(newfunc);
		}
		scope->insert_label((AST *)#(new VariableIdentifierLabel($id),#t));// here insert it as a variable
		CurrentProcedureOrFunction=$id->getText();
		CurrentProcedureOrFunctionLabel=func;
	>>
  	statement_terminator
		<<
			if($which==getExternalScope()){
				scope=scope->getPrev();	// no body will be defined
			}
		>>
	;     

function_body:                                   // fsd-- 6.8.3 
	{ local_preamble_structure }  
	main_procedural_structure
	;
   
//leave_function_statement!:                       // fsd-- 6.8.4

//		id:ID <<CurrentFunction==$id->getText()>>?
//			[Error_Report(" Illegal Leave Function ",$id);]
//		<<#0=#(new LeaveAction(0,scope),verifyLeaveResumeTarget(0,EntryFUNCTION));>>
//		statement_terminator 
//	;     

end_function_statement!>[ANTLRTokenPtr fnend]:                         // fsd-- 6.8.5     

	verb:fstatnoENDfd
		<<
			scope=scope->getPrev();
		>> 
		id:ID
	statement_terminator
		<<
			$fnend=$id;
			CurrentProcedureOrFunction="";	
			CurrentProcedureOrFunctionLabel=0;
		>>
	;

formal_parameter![int &argcount]:                               // fsd-- 14.38
		<<
			VarNamesList Var_Names_List  ;AST * local;AST * param;
		>>      
		id1:ID	<< Var_Names_List.insert(ANTLRTokenPtr($id1)); >>
		(
			Fd idn:ID	<< Var_Names_List.insert(ANTLRTokenPtr($idn)); >>
		)* IS t:type
////		<<
////			auto var_it = Var_Names_List.begin();  // std::list<ANTLRTokenPtr>::iterator
////			while (++var_it){
////				// dummy local variable
////				local=new VariableIdentifierLabel(*var_it.key());
////				scope->insert_label((AST *)#(local,#0?(sane()?#t->clone():0):#t));
////				
////				if(#0){
////					#0=#(0,#0,#(new ParameterLabel(*var_it.key()),local));
////				} else {
////					#0=#(new ParameterLabel(*var_it.key()),local);
////				}
////				argcount++;
////			};
////		>>
		
		<<
		for (auto var_it = std::next(Var_Names_List.begin()); var_it != Var_Names_List.end(); ++var_it) {
		    // dummy local variable
		    local = new VariableIdentifierLabel(*var_it);
		    scope->insert_label((AST *)#(local, #0 ? (sane() ? #t->clone() : 0) : #t));
		
		    if (#0) {
		        #0 = #(0, #0, #(new ParameterLabel(*var_it), local));
		    } else {
		        #0 = #(new ParameterLabel(*var_it), local);
		    }
		    argcount++;
		};
		>>

		
		
	;

require_statement!:                              // fsd-- 6.9
	<<
		VarNamesList Var_Names_List  ;
		Scope * which=0;
		ANTLRTokenPtr name;
	>>
	
	verb:fstatnoREQUIREfd
		req1:requirement_New>[name]
		<<
			Var_Names_List.insert( name );
			
		>>
		(
			Fd requirement_New>[name]
			<<
				Var_Names_List.insert( name );
			>>
		)*
		
  		Fd  external_global_fd>[which]
  		sr:signal_requirement
  		statement_terminator
  		<<
////			VarNamesListIterator var_it(Var_Names_List);
////  			for( int more_objects=0;++var_it;more_objects=1 ){
////  				AST * new_req=	which->insert_label( new RequirementLabel( var_it.key() ),scope);		
////				if ( more_objects ) {
//// 					#( new_req, #sr->clone() );
//// 				} else {
//// 					#( new_req, #sr );
//// 				}
////  				if(sane()&&!(new_req->check())){
////  					Error_Report("REQUIREMENT is NOT satisfied:",var_it.key());
////  				}
////				#0=#(0,#(new Proxy,new_req),#0);
////  			}
  			
  			for (auto var_it = std::next(Var_Names_List.begin()); var_it != Var_Names_List.end(); ++var_it) {
  				////AST * new_req=	which->insert_label( new RequirementLabel( var_it.key() ),scope);	
  				AST* new_req = which->insert_label(new RequirementLabel(*var_it), scope);

				if (var_it != std::next(Var_Names_List.begin())) {
 					#( new_req, #sr->clone() );
 				} else {
 					#( new_req, #sr );
 				}
  				if(sane()&&!(new_req->check())){
  					Error_Report("REQUIREMENT is NOT satisfied:", *var_it);
  				}
				#0=#(0,#(new Proxy,new_req),#0);
  			}
			#0 = #( #verb, #( new RequireSignalVerb, #0 ) );
  			//	LineAction
  			//	RequireSignalVerb
  			//	Proxy -----------> Proxy --------->....
  			//	RequirementLabel   RequirementLabel ....
  			//	signal_requirement signal_requirement ....
  		>>
	;
  
requirement_New!>[ANTLRTokenPtr name]:
		id:ID	<< $name=$id; >>;	

signal_requirement!:      // fsd-- 6.9A 
	<<
		NounEntry	*	nounEntry=0;
		ASTList		*	reqmodifierList = new ASTList;
		ASTList		*	cntmodifierList = new ASTList;
		ASTList		*	capmodifierList = new ASTList;
		ASTList		*	limmodifierList = new ASTList;
		AST		*	n;
	>>
	ra:require_analog[reqmodifierList]>[nounEntry,n]
	
	{	rcnt:require_control	[nounEntry,cntmodifierList]	}
	{	rcap:require_capability	[nounEntry,capmodifierList]	}
	{	rlim:require_limit	[nounEntry,limmodifierList]	}
	{	rcnx:require_cnx	[nounEntry]			}
	
////	<<
////		// Verify that everything that occured in rmodifierList
////		// also occurs in smodifierList
////		ASTListIterator reqita(*reqmodifierList);
////		ASTListIterator cntita(*cntmodifierList);
////		ASTListIterator capita(*capmodifierList);
////		while(++reqita){
////			cntita.reset();
////			int found=0;
////			capita.reset();
////			while(++cntita){
////				if(reqita.key()->getName()==cntita.key()->getName()){
////					found=1;
////					break;
////				}
////			}
////			if(!found){
////				while(++capita){
////					if(reqita.key()->getName()==capita.key()->getName()){
////						found=1;
////						break;
////					}
////				}
////			}
////			if(!found){
////				Error_Report("MODIFIER "+reqita.key()->getName()+" Has no  CHARACTERISTICS specified",reqita.key());
////			}
////		}
////		// Verify that no modifiers  exist both in 
////		// control and capability lists
////		cntita.reset();
////		while(++cntita){
////			capita.reset();
////			while(++capita){
////				if(cntita.key()->getName()==capita.key()->getName()){
////					Error_Report(
////						"MODIFIER "+cntita.key()->getName()+" is both in CONTROL and CAPABILITY:",
////						VerbLineNo
////					);
////				}
////			}
////		}
////		#(0,n,new ReqModifierListType(cntmodifierList,capmodifierList,limmodifierList),#rcnx);
////		// try without RequireAnalogAction
////		//	#0=#(new RequireAnalogAction,#ra);
////		#0=#ra;
////		// what do we have?
////		// RequireAnalogAction
////		//      |
////		//      v
////		//    Virtual(Something)
////		//      |
////		//      v
////		//    Noun --> ModifierListType --> #rcnx
////		//      |
////		//      v
////		//    mcm(empty for source etc..)
////		 
////	>>

	<<
	    for (auto reqit = std::next(reqmodifierList->begin()); reqit != reqmodifierList->end(); ++reqit) {
	        bool found = false;
	
	        for (auto cntit = std::next(cntmodifierList->begin()); cntit != cntmodifierList->end(); ++cntit) {
	            if ((*reqit)->getName() == (*cntit)->getName()) {
	                found = true;
	                break;
	            }
	        }
	
	        if (!found) {
	            for (auto capit = std::next(capmodifierList->begin()); capit != capmodifierList->end(); ++capit) {
	                if ((*reqit)->getName() == (*capit)->getName()) {
	                    found = true;
	                    break;
	                }
	            }
	        }
	
	        if (!found) {
	            Error_Report("MODIFIER " + (*reqit)->getName() + " Has no CHARACTERISTICS specified", *reqit);
	        }
	    }
	
	    // Verify that no modifiers exist in both control and capability lists
	    for (auto cntit = std::next(cntmodifierList->begin()); cntit != cntmodifierList->end(); ++cntit) {
	        for (auto capit = std::next(capmodifierList->begin()); capit != capmodifierList->end(); ++capit) {
	            if ((*cntit)->getName() == (*capit)->getName()) {
	                Error_Report(
	                    "MODIFIER " + (*cntit)->getName() + " is both in CONTROL and CAPABILITY:",
	                    VerbLineNo
	                );
	            }
	        }
	    }
	
	    #(0, n, new ReqModifierListType(cntmodifierList, capmodifierList, limmodifierList), #rcnx);
	    #0 = #ra;
	>>

	;
							//<<Esc_1 >> 
//  &*     This bypass must be used with nouns that do not  
//  &      require a CNX field, e.g. TIME INTERVAL. *

require_analog![ASTList * modifierList]>[NounEntry * nounEntry,AST * n ]:    // fsd-- 6.9-1
	<< NounEntry * nounEntry=0; ModifierEntry * modifierEntry=0;>>
	(
		( 
			sns:SENSOR LP
			mcn1:measured_characteristic_mnemonic[nounEntry]>[modifierEntry]
				<< modifierList->insert( #mcn1 ); >>
				(
					Fd mcnn:measured_characteristic_mnemonic[nounEntry]>[modifierEntry]
					<< modifierList->insert( #mcnn ); >>
				)* 
				<< #0 = new VirtualSensor( $sns ); >>
				RP
		|	em:EVENT MONITOR LP mc:monitored_characteristic_mnemonic[nounEntry]>[modifierEntry] RP
			<<
				modifierList->insert( #mc );
				#0 = new VirtualEventMonitor( $em );
			>>
		)
		
		Fd n1:noun[nounEntry]>[$nounEntry]
			<<
				// There are 2 ways to do this. One is to get the #mcn{1,2}
				// without worrying abou the Noun, and then once we get the noun
				// call the init method of noun and have it fix the 
				// #mcn..
				// The other (which is what is done here), is to let pccts
				// do a guess pass. First time thru find out what the
				// Noun is, and then do the right thing...
				#0 = #( #0, #(#n1 ,new ModifierListType(modifierList)));
				// now we know what the Noun is... So fix modifierList
				//#n1->init();
				$n=#n1;
			>>
	)?
	|
	(	<< nounEntry=0; >>
		(
		|	src:SOURCE	<< #0 = new VirtualSource( $src ); >>
		|	ld:LOAD		<< #0 = new VirtualLoad( $ld ); >>
		|	tm:TIMER
				<<
					#0 = new VirtualTimer( $tm ); allTimers->insert((AST *)#0);
					nounEntry=mnemonicsDB->theNounEntry("TIME INTERVAL");
				>>
		)
		Fd n2:noun[nounEntry]>[$nounEntry]
			<<
				// Note that modifierList is empty here. Just used as
				// a place holder.....
				#0 = #( #0, #(#n2 ,new ModifierListType(modifierList)));
				$n=#n2;
			>>
	)
	;
 
monitored_characteristic_mnemonic[NounEntry * nounEntry]>[ModifierEntry * modifierEntry]:  // fsd-- 6.9-2 
	measured_characteristic_mnemonic[nounEntry]>[$modifierEntry]
	;  
 

include_statement:                              // fsd-- 6.10
	<< int moduleType=0; >>
	verb:fstatnoINCLUDEfd     
	(
		NONdATLAS MODULE	<<	moduleType=1;	>>
	|	ATLAS MODULE		<<	moduleType=0;	>>
	)
	id1:ID
		<<
			if(moduleType==1){
				#(
					getExternalScope()->insert_nonatlas(
						new NonAtlasModuleLabel($id1,getExternalScope())
						,scope
					)
				);
			}
		>>
		(	Fd
			idn:ID
			<<
				if(moduleType==1){
					#(
					getExternalScope()->insert_nonatlas(
						new NonAtlasModuleLabel($idn,getExternalScope()))
						,scope
					);
				}
			>>
		)*
	statement_terminator
	;
 
  
// the module name is established by an independently
// compiled ATLAS or NON-ATLAS MODULE.  It is not     
// referenced in any other statement and therefore     
// the module name has no directive prefix ( < nor > )

establish_protocol_statement!:                   // fsd-- 6.11
	<<
		Scope * which=0;
	>>
	verb:fstatnoESTABLISHfd external_global_fd>[which] bp:BUSdPROTOCOL bpid:ID Fd
	SPEC spec:ID Fd
	dd:databus_definition 
		<<
			AST * bpl = which->insert_label(new ProtocolLabel($bpid),scope);
			AST * bsl = new BusSpecificationLabel($spec);

			/////////#(#verb,#(new EstablishProtocolVerb,#(bpl,#(new VirtualProtocol($spec),bsl,#dd))));


			#(bpl,#(new VirtualProtocol($spec),bsl,#dd));
  			if(sane()&&!(bpl->check())){
  				Error_Report("BUS-PROTOCOL is NOT covered"+bpl->getName(),bpl);
  			}
		>>
	statement_terminator
	;
 
 
databus_definition!:                             // fsd-- 6.11A
	bip:bus_installation_parameters Fd
	brd:bus_redundancy_definition
	{ Fd CNX  (pd:pin_descriptor << #0=#0?#(0,#0,#pd):#pd; >> )+ }
	<< #0 = #(0,#(new Proxy,#bip),#(new Proxy,#brd), #( new RequireCnxAction, #0 )); >>

	;
  
  
bus_redundancy_definition!:                      // fsd-- 6.11B
	<<
		DimensionEntry * dimensionEntry=0;
	>>
	(
		s:STANDARD	<< #0=new BusRedundancyMode($s); >>
	|	f:FAULTdTEST	<< #0=new BusRedundancyMode($f); >>
	)
	(
		REDUNDANT BUS	<< #0=#(#0,new RedundantBus); >>					// In redundant this is the first.
	|	PRIMARY BUS Fd abt:ALTERNATEdBUSdTRANSMIT p:protocol		// this is the second.
			{
				DELAY tq:time_quantity>[dimensionEntry]	// primary to redundant delay
			}
			<<	#0=#(#0,#(new AlternateBusTransmit($abt),#p,#tq)); >>
	|	BUS	// Run of the mill bus.
	)
	
  ; 
 
 
bus_installation_parameters!:                    // fsd-- 6.11C
	<< ASTList * csdList = 0;ASTList * emList=0;AST * tem=0; >>
	tebr:test_equip_bus_role 
	{
		Fd TESTdEQUIPdMONITOR
		<<	csdList=new ASTList;
			tem=new TestEquipMonitor(csdList);
		>>
		csd1:command_status_data
			<< csdList->insert(#csd1); >>
			(
				Fd csdn:command_status_data
				<< csdList->insert(#csdn); >>
			)*
	}
	<< emList=new ASTList; >>
	( Fd em:exchange_model << emList->insert(#em); >> )+
	{ Fd ebp:establish_bus_parameters }
	{ Fd epp:establish_protocol_parameters }
	<<
		#0=	#(0,	#(#tebr,			tem)	);
		#0=	#(0,#0,	#(new ExchangeModels(emList))		);
		#0=	#(0,#0,	#(new BusParameters,		#ebp)	);
		#0=	#(0,#0,	#(new ProtocolParameters,	#epp)	);
	>>
	;
 
command_status_data:
	(
		COMMAND
	|	STATUS
	|	DATA
	)
	;
 
test_equip_bus_role!:                            // fsd-- 6.11D
	<< ASTList * mmsList=0; >>
	TESTdEQUIPdROLE
	mms1:monitor_master_slave
			<< (mmsList=new ASTList)->insert(#mms1); >>
	(
		Fd mmsn:monitor_master_slave
			<< mmsList->insert(#mmsn); >>
	)*
	<< #0=new TestEquipBusRole(mmsList); >>
	;
  

monitor_master_slave:
		test_equip_role_name
	;    
//  &*    The name must be one that has been introduced by an
//  &     EXTEND statement of the associated type.*

// in exchange_model , type really means exchange_index_type defined as follows...

//exchange_index_type!:	//	fsd-- 6.11.4.2(rule 3)
//		enum_type
//	|	str:STRING LP c:integer_constant RP  OF
//			(	BIT	<< #0 = new StringOfBitType( $str, #c->getInteger() ); >>
//			|	CHAR	<< #0 = new StringOfCharType( $str, #c->getInteger() ); >>
//			)
//	|	integer_type
//	|	decimal_type
//	;
// so we must either change the rule, or verify after parse.... sik...


exchange_model!:                                 // fsd-- 6.11E
	<< ASTList * talkers=0;ASTList * listeners=0; >>
	BUSdMODE bmn:bus_mode_name
	{ Fd TALKER
		<<talkers=new ASTList;>>
		t:establish_device
		<< talkers->append(#t); >>
	} 
	{ Fd LISTENER
		<<listeners=new ASTList;>>
		(l:establish_device
		<< listeners->append(#t); >>
		)+
	}
	ebi:establish_bus_info
	{ Fd INITIALIZE {Fd COMMAND LP ct:type RP } { Fd DATA LP dt:type RP} }
	<<
		#0=	#(	0
				,#(#bmn,#(new DataBusDevices,new Talker(talkers),new Listener(listeners)))
				,#ebi
				,#(new InitializeExchangeAction,new Command(#ct),new Data(#dt))
			);
	>>
	;
  


establish_device!:				// fsd-- 6.11F
	(
		ee:enumeration_element
			<<#0=#(new DeviceIdentifierLabel(#ee->getToken()),#(new Proxy,#ee));>>
	|	teq:TESTdEQUIP
			<<#0=new DeviceIdentifierLabel($teq);>>
	|	uut:UUT  
			<<#0=new DeviceIdentifierLabel($uut);>>
	)
	{ LP t:type  RP }
	<< #0=#(#0, #t); >>
	
	;
  
//device_identifier_label!:
//	<<isType(LT(1)->getText(),Device_Identifier_Label)>>? id:ID << #0=#(new Proxy($id),getTokenLabel($id->getText()));>>
//	;    
  
establish_bus_parameters:                       // fsd-- 6.11G
	BUSdPARAMETER 
	establish_bus_parameter (Fd establish_bus_parameter)*
	;
        
establish_bus_parameter:
	{ fetch_update_spec }
	bus_parameter_range 
	;
       
        
bus_parameter_range:                           // fsd-- 6.11H
	<<
		ModifierEntry * modifierEntry=0;
	>>
	bpn:bus_parameter_name>[modifierEntry]
	{ bpvr:bus_parameter_value_range[modifierEntry] }
	<<#0=#(#bpn,#bpvr);>>
	; 
 
fetch_update_spec:                              // fsd-- 6.11I
	( 
		UPDATABLEdFETCHABLE
	|	FETCHABLE
	|	UPDATABLE  
	)
	(  
		PROTOCOLdEXCHANGE
	|	PROTOCOL
	|	EXCHANGE    
	)
	;
 
  
bus_parameter_value_range![ModifierEntry * &modifierEntry]:                      // fsd-- 6.11J
	<<
		DimensionEntry * dimensionEntry=0;
	>>
	( 
		RANGE from:dimensioned_number[modifierEntry,dimensionEntry] 
			TO to:dimensioned_number[modifierEntry,dimensionEntry] 
		<< #0=#(new ConstraintRange,#from,#to);>>
		{
			BY by:dimensioned_number[modifierEntry,dimensionEntry]
			<<#0=#(new ByQuantization,#0,#by);>>
		}
	|
		(
			MAX	<<#0=new ConstraintMax;>>
		|	MIN	<<#0=new ConstraintMin;>>
		)	dc:dimensioned_number[modifierEntry,dimensionEntry]
		<<#0=#(#0,#dc);>>
	)
	;

//	Bogus.. Examples allow negative numbers.....
//dimensioned_constant[ModifierEntry * &modifierEntry,DimensionEntry * & dimensionEntry]:                            // fsd-- 6.11K
//		decimal_constant dim[modifierEntry,dimensionEntry]
//	;
 
 
establish_protocol_parameters:                   // fsd-- 6.11L
	PROTOCOLdPARAMETER
		establish_protocol_parameter (Fd establish_protocol_parameter)*
	;
  
establish_protocol_parameter:
	{ fetch_update_spec }
	protocol_parameter_types 
	; 
        
  
protocol_parameter_types:                        // fsd-- 6.11M
	( 
		COMMANDdWORD                 |
		STATUSdWORD                  |
		DATAdWORD  
	)
	( 
		PULSEdCODE                     |
		PARITY                         |
		LENGTH                         |
		SYNC                           |
		TdR 
	) 
	|
	( 
		protocol_parameter_name                 //<<Esc_1 >>
		{ LP type RP }                   |
		WORDdCOUNT 
	) 
	;
//  &*    The name must be one that has been introduced by an
//  &     EXTEND statement of the associated type.*
 
 
establish_bus_info:                            // fsd-- 6.11N
	{ Fd COMMAND LP type RP }
	{ Fd DATA LP  type  RP } 
	{ Fd STATUS LP type RP } 
	;
 
    
identify_statements!:                             // fsd-- 6.12
	<<
		Scope * which=0;
	>>
	verb:fstatnoIDENTIFYfd external_global_fd>[which]
	(
		TIMER	identify_timer_statement[which]
	|
		EVENT	(
				id:ID AS
				(
					identify_signal_based_event_statement[$id,which]
				|	identify_event_based_event_statement[$id,which]
				|	identify_time_based_event_statement[$id,which]
				)
			|	identify_event_interval_statement[which]
			|	identify_event_indicator_statement[which]
			)
	)
	;
 
identify_timer_statement![Scope * which]:   // fsd-- 6.13
	<<
		NounEntry * nounEntry=0;
		ModifierEntry * modifierEntry=0;
		RWCString left;
		AST * re=0;
	>>
	id:ID TIME INTERVAL
		<<
			nounEntry=mnemonicsDB->theNounEntry("TIME INTERVAL");
		>>  
		USING req:requirement[re] Fd time:TIME
		<< modifierEntry=mnemonicsDB->theModifierEntry(RWCString($time->getText()),nounEntry,left);>>
		rcs:real_characteristic_subfield[modifierEntry,re ,0]
		<<
			#(which->insert_label(new TimerLabel($id),scope),#req,#rcs);
		>>
	statement_terminator
	;     
  

identify_signal_based_event_statement![ANTLRTokenPtr id,Scope * which]:          // fsd-- 6.14     
	/* verb:fstatnoIDENTIFYfd external_global_fd EVENT id:ID AS */
		(
			ae:analog_event
			<< #( which->insert_label(new EventLabel(id),scope), #ae ); >>
		|	de:digital_event
			<< #( which->insert_label(new EventLabel(id),scope), #de ); >>
		)
	statement_terminator
	;     
  
  
analog_event!:                                   // fsd-- 6.14A    
	<<
		NounEntry * nounEntry=0;
		DimensionEntry * dimensionEntry=0;
		AST * re=0;
		AST * wf=0;
	>>
	nf:noun_field[nounEntry,re]>[nounEntry,re,wf] Fd  
	se:signal_based_event_body[nounEntry]     
	sc:statement_characteristics[nounEntry,re] Fd  
	{ TESTdEQUIPdDELAY tq:time_quantity>[dimensionEntry] Fd }
	c:conn[nounEntry]
	<<
		AST * uem=new UseEventMonitor(re);
		#0=#(uem,#(#nf,#se),#sc,#c,#tq);
		if(sane()&&!(uem->check())){
			Error_Report(" ANALOG EVENT is not covered",VerbLineNo);
		}
	>>
	;
  
digital_event!:                                  // fsd-- 6.14B  
	LP VALUE RP  REF val:strofbit_expression     
	{ 
		( 
			Fd MASKdONE
		|
			Fd MASKdZERO 
		) 
		mask:strofbit_expression
	}     
	es:event_sensing Fd of:on_field
	;
       
signal_based_event_body![NounEntry * nounEntry]:   // fsd-- 6.14C     
	<<
		ModifierEntry * modifierEntry=0;
		DimensionEntry * dimensionEntry=0;
	>>
	cm:characteristic_mnemonic[nounEntry]>[modifierEntry] EQcomp
		rq:real_quantity[modifierEntry,dimensionEntry] 
		{ re:real_errlim[dimensionEntry] }     
		sc:signal_changes[dimensionEntry]
		<< #0=#(0,#(#cm,#rq,#re),#sc); >>
	;
     
  
signal_changes![DimensionEntry *& dimensionEntry]:                                 // fsd-- 6.14D     
	<<
		ModifierEntry * modifierEntry=0;
	>>
	{ Fd i:INCREASING << #0=new EventSlopePosType($i); >> | Fd d:DECREASING << #0=new EventSlopeNegType($d); >> }
	{ 
		Fd h:HYSTERESIS rq:real_quantity[modifierEntry,dimensionEntry]
		{ re:real_errlim[dimensionEntry] }
		<< #0=#(#0,#(new HysteresisType($h),#rq,#re)); >>
	}
	;

  
event_sensing:                                  // fsd-- 6.14E
	<<
		DimensionEntry	* dimensionEntry=0;
		ModifierEntry	* modifierEntry=0;
	>>  
	( 
		Fd MINIMUMdSENSEdRATE numeric_expression dim[modifierEntry,dimensionEntry]
	|	Fd SENSEdEVENT event 
	) 
	;
     
  
identify_event_based_event_statement![ANTLRTokenPtr id,Scope * which]:           // fsd-- 6.15
	<<
		DimensionEntry * dimensionEntry=0;
	>>
	/* verb:fstatnoIDENTIFYfd external_global_fd EVENT id:ID AS */
	ebed:event_based_event_definition[dimensionEntry]     
	<<
		#(which->insert_label(new EventLabel(id),scope),#ebed);
	>>
	statement_terminator
	;
     
  
event_based_event_definition![DimensionEntry *& dimensionEntry]:                   // fsd-- 6.15A     
	( 
		tq:time_quantity>[dimensionEntry] AFTER ev:event
		<< #0= #(#(new WaitForEventAction,#ev),#(new WaitForTimeAction,#tq)); >>
	|	e:event gf:gate_field
		<< #0= #(new WaitForEventAction,#e,#gf); >>	// This is not correct
	|	MANUALdINTERVENTION
		<< #0= new WaitForManualAction(lManualInterventionEvent); >>
	|	ieea:identify_event_every_after
		<< #0= #(new WaitForEventAction,#ieea); >>
	) 
	;     
  
identify_event_every_after!:                     // fsd-- 6.15B     
	EVERY ec:unsigned_integer_number
	OCCURRENCES OF pe:event  << #0= new PeriodicEventCounterType(#pe,#ec); >>
	{ 
		AFTER     
		{ ac:unsigned_integer_number OCCURRENCES OF }     
		oe:event	<< #0= #( 0, new OnceEventCounterType(#oe,#ac), #0); >>
	}
	;     
  
  
identify_event_interval_statement![Scope * which]:              // fsd-- 6.16
	<< AST * label;AST * from; AST * to; >>
	/* verb:fstatnoIDENTIFYfd external_global_fd EVENT */ INTERVAL id:ID AS
	ift:identify_event_from_to>[from,to]
	<< 	#(	which->insert_label(new EventIntervalLabel($id),scope),
				new EventIntervalType(from,to)
		);
	>>
	statement_terminator
	;     
  
  
identify_event_from_to!>[AST * from,AST * to]:                         // fsd-- 6.16A
	<<
		DimensionEntry * dimensionEntry=0;
	>>  
	FROM fe:event << $from=#fe; >>
	{
		FOR tq:time_quantity>[dimensionEntry]
		<< #($to=new OneShotTimerType,#tq); >>
	|	TO te:event
		<< $to=#te; >>
	}
	;
  
  
identify_event_indicator_statement![Scope * which]:             // fsd-- 6.17
	<< AST * label; >>
	/* verb:fstatnoIDENTIFYfd external_global_fd EVENT */ INDICATOR id:ID AS SET BY
	e:event
	<<
		#(
			which->insert_eilabel(new EventIndicatorLabel($id),scope),
				new EventIndicatorType(#e,getTokenLabel($id->getText()))
		);
	>>
	statement_terminator 
	;     
  
  
identify_time_based_event_statement![ANTLRTokenPtr id,Scope * which]:            // fsd-- 6.18    
	<<
		DimensionEntry * dimensionEntry=0;AST * label;
	>>  
	/* verb:fstatnoIDENTIFYfd external_global_fd EVENT  id:ID  AS */
	TIMER t:timer EQcomp tq:time_quantity>[dimensionEntry] { re:real_errlim[dimensionEntry] }
	<<
		#(
			which->insert_label(new EventLabel(id),scope),
				#(new TimeBasedEventType(#t),
					#tq,#re
				)
		);
	>>
	statement_terminator
	;
     
  
define_digital_configuration_structure[AST * verb,ANTLRTokenPtr id]:          // fsd-- 6.19.1
	define_digital_configuration_statement[verb,id] 
	configuration_body 
	end_digital_configuration_statement
	;


define_digital_configuration_statement![AST * verb,ANTLRTokenPtr id]:         // fsd-- 6.19.2
	//-------//verb:fstatnoDEFINEfd id:ID  Fd
		DIGITAL CONFIGURATION
		<<scope->insert_label(new ConfigurationLabel(id));>>
	statement_terminator
	;

configuration_body:                             // fsd-- 6.19.1-1
	(
		verb:fstatnoDEFINEfd id:ID  Fd DIGITAL
		(	define_digital_source_statement[#verb,$id]
		|	define_digital_sensor_statement[#verb,$id]
		)
	)+
	(specify_delay_min_statement )*
	;

 
 
define_digital_source_statement[AST * verb,ANTLRTokenPtr id]:                // fsd-- 6.19.3
	//-------//verb:fstatnoDEFINEfd id:ID  Fd DIGITAL
		SOURCE
		<<scope->insert_label(new DigitalSourceLabel(id));>>  
	{ 
		Fd SPEC
		external_digital_specification (Fd external_digital_specification )*
	}
	digital_source_definition
	statement_terminator
	;
   
  
digital_source_definition:                      // fsd-- 6.19.3A
	<<
		NounEntry * nounEntry;
		AST * re=0;
		AST * wf=0;
	>>
	{Fd noun_field[nounEntry,re]>[nounEntry,re,wf]			//<<Esc_1 >>
	statement_characteristics[nounEntry,re]	}		//<<Esc_1 >>
	source_digital_characteristics
	Fd CNX conn_set["",nounEntry]
	;
//  &*   This by_pass is used when the relevant information is
//  &    completely defined in the specification identified by
//  &    the <external digital specification>. *
  

source_digital_characteristics:                 // fsd-- 6.19.3B
	<<
		DimensionEntry * dimensionEntry=0;
	>>
	{source_logic_levels}	//<<Esc_1 >>
	{Fd ILLEGAL STATE INDICATOR boolean_data_store}
	digital_format[dimensionEntry]

	
	;
//  &*   This by_pass is used when the relevant information is
//  &    completely defined in the specification identified by
//  &    the <external digital specification>. *
 
 

     
source_logic_levels:
	(Fd (def_dig_source_levels	|	def_dig_sequence))+
	;  
  
digital_format[DimensionEntry *& dimensionEntry]:                                 // fsd-- 6.19.3D
	(
		Fd SKEWdTIME	MAX time_quantity>[dimensionEntry]	// <<Esc_1 >> <<Esc_2 >>
	|
		{	Fd serial_format 				}	//<<Esc_1 >>
		{	Fd pulse_class					}	//<<Esc_1 >>
		{	Fd digital_synchronization[dimensionEntry]	}
	)
	;
//  &*   This bypass is used only if all the information has
//  &    been defined in the <external digital specification>
//  &    and the signals are parallel. *
//  &**  This path is used only with parallel digital signals. *
  
  
serial_format:                                  // fsd-- 6.19.3E
	( 
		SERIALdMSBdFIRST         |
		SERIALdLSBdFIRST 
	)
	Fd
	( <<RWCString("WORD-LENGTH")== LT(1)->getText()>>? wl:NounCLASS
		unsigned_integer_number BITS
	)
	;
 
 
logic_alternation:                              // fsd-- 6.19.3F
	EXCEPT AT EVERY unsigned_integer_number
	CONSECUTIVE OCCURRENCES
	;

 
pulse_class:                                    // fsd-- 6.19.3G
	PULSEdCLASS 
	( 
		HDB unsigned_integer_number
	|	RZ
	|	NRZ
	|	BIP
	|	MIP
	|	AMI 
	) Fd
	;

 
digital_synchronization![DimensionEntry *& dimensionEntry]:	// fsd-- 6.19.3H
	<<
		ModifierEntry * modifierEntry=0;
	>>
	(
		( BITdRATE | BITdPERIOD ) real_quantity[modifierEntry,dimensionEntry]
		{ bit_rate_range }
	|	SYNCdBITdTRANSITION TO EVENT 
		event max_time 
	) Fd
	;
  
  
bit_rate_range:                                 // fsd-- 6.19.3I
	<<
		DimensionEntry	* dimensionEntry=0;
		ModifierEntry	* modifierEntry=0;
	>>
	RANGE <<dimensionEntry=new DimensionEntry("","Digital Bit Rate");>>
		brfr:unsigned_dimensioned_number[modifierEntry,dimensionEntry]
		TO brto:unsigned_dimensioned_number[modifierEntry,dimensionEntry]
		<< #0=#(new ConstraintRange,#brfr,#brto);>>
	;
  
  
def_dig_source_levels:                          // fsd-- 6.19.3J
	<<
		NounEntry * nounEntry=0;
	>>
		( LEVELdLOGICdZERO | LEVELdLOGICdONE )
		digital_source_characteristics[nounEntry]
		{ logic_alternation }
	|	( LEVELdLOGICdQUIES | LEVELdLOGICdHIZ )
		digital_source_characteristics[nounEntry] Fd 
	;
  
  
def_dig_sequence:                               // fsd-- 6.19.3K
		( TRANSdLOGICdZERO | TRANSdLOGICdONE )
		aa_def_dig_sequence_1 ( OR aa_def_dig_sequence_1)*
	|	TRANSdSYNC ( QUIES | "0" | "1") ("0" | "1" )
	|	TRANSdSYNCdBIT 
	;

aa_def_dig_sequence_1:
	( "0" | "1" )  ( "0" | "1" ) 
	;

define_digital_sensor_statement[AST * verb,ANTLRTokenPtr id]:                 // fsd-- 6.19.4
	//-------//verb:fstatnoDEFINEfd id:ID  Fd DIGITAL
		SENSOR
	<<scope->insert_label(new DigitalSensorLabel(id));>>
	{ 
		Fd SPEC external_digital_specification 
			( Fd external_digital_specification )*
	}
	digital_sensor_definition     
	statement_terminator
	;
     
  
digital_sensor_definition:                      // fsd-- 6.19.4A     
	<<
		NounEntry * nounEntry=0;
		ModifierEntry * modifierEntry=0;
		AST * re=0;
		AST * wf=0;
	>>
	Fd LP measured_characteristic_mnemonic[nounEntry]>[modifierEntry] RP  //<<Esc_1 >>    
	Fd noun_field[nounEntry,re]>[nounEntry,re,wf]			//<<Esc_1 >> 
	statement_characteristics[nounEntry,re]		//<<Esc_1 >> 
	sensor_digital_characteristics Fd CNX conn_set["",nounEntry]
	;     
//  &*   This by_pass is used when the relevant information is     
//  &    completely defined in the specification identified by     
//  &    the <external digital specification>. *     
  
 
sensor_digital_characteristics:                 // fsd-- 6.19.4B     
	<<
		DimensionEntry * dimensionEntry=0;
	>>
	( Fd sensor_logic_levels )*                   //<<Esc_1 >> 
	{Fd ILLEGAL STATE INDICATOR boolean_data_store}
	digital_format[dimensionEntry]          // <<Esc_1 >> 
	;     
//  &*   This by_pass is used when the relevant information is     
//  &    completely defined in the specification identified by     
//  &    the <external digital specification>. *     
  
  
sensor_logic_levels:                            // fsd-- 6.19.4C     
	def_dig_sensor_levels  | def_dig_sequence 
	;
  
def_dig_sensor_levels:                          // fsd-- 6.19.4D     
	<<
		NounEntry * nounEntry=0;
	>>
		( 
			LEVELdLOGICdZERO
		|	LEVELdLOGICdONE
		)
		digital_sensor_characteristics[nounEntry]
		{ logic_alternation }
	|	( 
			LEVELdLOGICdQUIES
		|	LEVELdLOGICdHIZ 
		)
		digital_sensor_characteristics[nounEntry] Fd 
	;
       
specify_delay_min_statement:                    // fsd-- 6.19.5  
	<<
		DimensionEntry * dimensionEntry=0;
	>>
	verb:fstatnoSPECIFYfd DELAYdMIN
	unsigned_decimal_number[0] time_dim[dimensionEntry] FROM  
	( 
		//(digital_source)?
		digital_source TO digital_sensor
	|	digital_sensor TO digital_source
	)  
	statement_terminator
	;  
 
 
end_digital_configuration_statement:            // fsd-- 6.19.6
	verb:fstatnoENDfd configuration
	statement_terminator
	;
 

define_exchange_statement![AST * verb,ANTLRTokenPtr id]:                      // fsd-- 6.20
	<<
		AST * ec=0;
		Scope * which=0;
	>>
	//-------//verb:fstatnoDEFINEfd id:ID  Fd
	external_global_fd>[which] EXCHANGE Fd PROTOCOL p:protocol Fd
		<< ec=which->insert_label(new ExchangeLabel(id),scope);>>
	ed:exchange_definition[ec]
		<<	// kind of like a procedure..... isolated incident...
			#( ec,#(new VirtualExchange,#ed,#p ));
			if(sane()&&!(ec->check())){
				Error_Report(ec->getName()+" Does not cover:",#p);
			}
		>>
	statement_terminator
	;
 
 
exchange_definition![AST * ec]:                            // fsd-- 6.20A
	BUSdMODE	bmn:bus_mode_name
	ed:exchange_devices
	def:define_exchange_fields[ec]
	<< #0=#(new ExchangeDefinition,#(#bmn,#ed),#def); >>
	;
//  &*     The <bus_mode_name> must be one associated with the
//  &      particular <protocol> by its ESTABLISH statement.*
 

exchange_devices!:                               // fsd-- 6.20B
	<< ASTList * talkers=0;ASTList * listeners=0; >>
	{ Fd TALKER 
		<<talkers=new ASTList;>>
		ddt:databus_device
		<< talkers->append(#ddt); >>
	}
	{ Fd LISTENER
		<<listeners=new ASTList;>>
		( ddl:databus_device
		<< listeners->append(#ddl); >>
		)+
	}
	<< #0=#(new ExchangeDevices , new Talker(talkers),new Listener(listeners) ); >>
	;
 
 
databus_device!:                                 // fsd-- 6.20C
	<< AST * sub=0; >>
	( 
		din:device_identifier_name
		<< #0=#(new DataBusDevice(#din)); >>
	|	te:TESTdEQUIP
		<< #0=#(new DataBusDevice($te)); >>
	|	uut:UUT                      
		<< #0=#(new DataBusDevice($uut)); >>
	)
	{ 
		LP
			(
				(c:constant	<<	sub=#c;	>> )?
			|	ds:data_store	<<	sub=#ds;>>
			)  RP 
	}
	<<	#0=#(#0,sub);	>>
	;
//  &*     The device identifier name must have been established
//  &      as a valid TALKER or LISTENER in the <establish statement>
//  &      as appropriate for the <protocol>.*
  
 
define_exchange_fields![AST * ec]:                         // fsd-- 6.20D
	{ Fd c:COMMAND	dddc:	define_databus_data[ec]	<< #0=#(0,#(new DataBusData($c,ec),#dddc),#0); >>}
	{ Fd d:DATA	dddd:	define_databus_data[ec]	<< #0=#(0,#(new DataBusData($d,ec),#dddd),#0); >>}
	{ Fd s:STATUS	ddds:	define_databus_data[ec]	<< #0=#(0,#(new DataBusData($s,ec),#ddds),#0); >>}
	{ Fd		bp:	bus_parameter		<< #0=#(0,#(new BusParameter(ec)  ,#bp  ),#0); >>}
	{ Fd		spp:	set_protocol_parameter	<< #0=#(0,#(new ProtocolParameter(ec)  ,#bp  ),#0); >>}
	<< #0= #(new DefineExchangeFields, #0); >>
	;
 
 
define_databus_data![AST * ec]:                            // fsd-- 6.20E
	{entire:ENTIRE }
	(
		(array_range)?
	|	(data_store)?
	|	constant
	)
	;
 
extend_atlas_statement!:                         // fsd-- 6.21
	verb:fstatnoEXTENDfd ATLAS
	( 
		noun_extension
	|	connection_extension
	|	modifier_extension
	|	databus_extension      
	)
	statement_terminator
	;
  
  
noun_extension!:                                 // fsd-- 6.21A
	NOUNS id:ID   Fd FOR VERBS
	( verb_info  )+ Fd 
	SPEC external_semantics_specification
	;
  
  
connection_extension!:                           // fsd-- 6.21B
	<<NounEntry * nounEntry=0;>>
	CONNECTIONS FOR noun[nounEntry]>[nounEntry] Fd extend_cnx
	{ Fd SPEC external_semantics_specification }
	;
  
  
modifier_extension!:                             // fsd-- 6.21C
	<<
		NounEntry * nounEntry=0;
		ModifierEntry * modifierEntry=0;
	>>
	MODIFIERS FOR noun[nounEntry]>[nounEntry]     
	(	<< modifierEntry=0; >>
		Fd modifier_definition[nounEntry]>[modifierEntry] 
			Fd USAGE usage_info[modifierEntry]
	 )+
	{ Fd SPEC external_semantics_specification }
	;     
  
  
 
extend_cnx!:                                     // fsd-- 6.21D
		CNXdNUMERATOR ext_pin CNXdDENOMINATOR ext_pin
	|	CNX ext_pin
	;     
  
  
modifier_definition![NounEntry * nounEntry]>[ModifierEntry * modifierEntry]:     // fsd-- 6.21E
	<<
		ModifierEntry * modifierEntry=0;
		RWCString left;
		ANTLRTokenPtr p;
	>>
	(
		id:ID
		<<
			$modifierEntry=insertQuotedModifier($id->getText(),nounEntry);
		>>
			(
				modifier_typing[$modifierEntry]
			|	MODIFIERdDESCRIPTOR extend_modifier_descriptor[$modifierEntry]
			|	SPECIALdSYNTAX external_syntax_specification
			|	MNEMONICdONLY 
			)
	|	modifier[0,left]>[modifierEntry,p]
			<<
				if(sane()){
					$modifierEntry=modifierEntry->clone();
					nounEntry->modifierDictionary.insertKeyAndValue
							($modifierEntry->modifier,$modifierEntry);
					if(!(left=="")){
						Error_Report(" BAD MODIFIER prototype ",p);
					}
				}
			>>
	)
	;
  
  
modifier_typing![ModifierEntry * &modifierEntry]:                                // fsd-- 6.21F
	( MODdARRAY | MODdVALUE )
	mod_type[modifierEntry]
	{ dimension_extension[modifierEntry] }
	;
 
 
usage_info![ModifierEntry * modifierEntry]:	// fsd-- 6.21G
		eid:EID
		<< modifierEntry->setUsage($eid);>>
	;
  

ext_pin:                                        // fsd-- 6.21H  
	(pin_name) + {  REF (pin_name) + } 
	;


pin_name:                                       // fsd-- 6.21I
	id:ID <<scope->insert_label(new PinDescriptorNameLabel($id));>>
	|	pin_descriptor 
	;
  
  
dimension_extension![ModifierEntry *& modifierEntry]:                            // fsd-- 6.21J
	<<
		DimensionDictionary * dimensionDictionary=0;
		DimensionEntry * dimensionEntry=0;
	>>
	MODdDIM
	<< dimensionDictionary=new DimensionDictionary; >>
		
	(
		new_dimension[modifierEntry,dimensionDictionary]
		(
			dimension[0,dimensionEntry]
			<< dimensionDictionary->insertKeyAndValue(dimensionEntry->dimension,dimensionEntry); >>
		)*
	|	(
			dimension[0,dimensionEntry]
			<< dimensionDictionary->insertKeyAndValue(dimensionEntry->dimension,dimensionEntry); >>
		)+
	|	NONE
		<<
			dimensionEntry=new DimensionEntry;
			dimensionEntry->dimension=RWCString("None");
			dimensionDictionary->insertKeyAndValue(dimensionEntry->dimension,dimensionEntry);
		>>
	)
	;
  
  
new_dimension[ModifierEntry *& modifierEntry,DimensionDictionary *& dimensionDictionary]:// fsd-- 6.21K
	<<
		DimensionEntry * dimensionEntry=0;
	>>
	(
		id:ID
		<<
			QuantityList *ql=0;
			std::regex single_quote("'");
			dimensionEntry=new DimensionEntry;
			ql=modifierEntry->quantitiesList[0];
			dimensionEntry->quantity=(*ql)[0]->quantity;
			dimensionEntry->dimension=$id->getText();
			dimensionEntry->dimension = std::regex_replace(dimensionEntry->dimension, single_quote, "");
			//dimensionEntry->dimension(re)="";
			dimensionDictionary->insertKeyAndValue(dimensionEntry->dimension,dimensionEntry);
		>>
	) +

	
	
	
	{ DIMdSPEC external_semantics_specification }
	;
  
  
verb_info:                                      // fsd-- 6.21L
		DEFINE		| IDENTIFY	| 	REQUIRE		| APPLY
	|	REMOVE		| MEASURE	| 	MONITOR		| VERIFY
	|	READ		| INITIATE	| 	SETUP		| CONNECT
	|	DISCONNECT	| ARM		| 	FETCH		| CHANGE
	|	RESET		| WAIT FOR	|	STIMULATE	| SENSE
	|	PROVE		| DO       
	;
   
  
extend_modifier_descriptor![ModifierEntry * modifierEntry]:						// fsd-- 6.21M
	( 	new_modifier_descriptor[modifierEntry]
			(
				omd:modifier_descriptor[modifierEntry]
				//<< modifierEntry->addModifierDescriptor(#omd->getName()); >>
			)*
	|	(
			md:modifier_descriptor[modifierEntry]
			//<< modifierEntry->addModifierDescriptor($md->getText()); >>
		)+ 
	)
	;
  
  
new_modifier_descriptor![ModifierEntry *& modifierEntry]:		// fsd-- 6.21N
	(
		id:ID	//<< modifierEntry->addModifierDescriptor($id->getText()); >>
	) +
	{
		MODdDESCdSPEC ess:external_semantics_specification
			//<< modifierEntry->m_modDescSpec==$ess->getText()); >>
	}
	;
 
  
databus_extension!:                              // fsd-- 6.21O
	<<
		RWTValSlist<RWCString> modifiers;
		ModifierEntry * modifierEntry=0;
		NounEntry * nounEntry=0;
		
	>>
	(	BUSdPARAMETER		<<
						nounEntry=mnemonicsDB->theNounEntry("~BUS-PARAMETER");
						modifierEntry=new DataBusEntry(nounEntry);
					>>
		get_extension_names[modifiers] extend_parameter_definition[modifierEntry]
		(Fd get_extension_names[modifiers] extend_parameter_definition[modifierEntry])*
	|	PROTOCOLdPARAMETER	<<
						nounEntry=mnemonicsDB->theNounEntry("~PROTOCOL-PARAMETER");
						modifierEntry=new DataBusEntry(nounEntry);
					>>
		get_extension_names[modifiers] extend_parameter_definition[modifierEntry]
		(Fd get_extension_names[modifiers] extend_parameter_definition[modifierEntry])*
	|	BUSdMODE		<<
						nounEntry=mnemonicsDB->theNounEntry("~BUS-MODE");
						modifierEntry=new DataBusEntry(nounEntry);
					>>
		get_extension_names[modifiers]
	|	TESTdEQUIPdROLE		<<
						nounEntry=mnemonicsDB->theNounEntry("~TEST-EQUIP-ROLE");
						modifierEntry=new DataBusEntry(nounEntry);
					>>
		get_extension_names[modifiers]
	|	EXCHANGEdPIN		<<
						nounEntry=mnemonicsDB->theNounEntry("~EXCHANGE-PIN");
						modifierEntry=new DataBusEntry(nounEntry);
					>>
		get_extension_names[modifiers]
	)
////		<<
////			RWCRegexp re("'");RWCString pruned;
////			ModifierEntry * me;
////			QuantityList * quantityList=new QuantityList;
////			modifierEntry->insertQuantityList(quantityList); 
////			for(int i=0; i< modifiers.entries();i++){
////				pruned=(modifiers[i](re)="");	// get rid of the single quotes.
////				if(i==0){
////					me=modifierEntry;
////					quantityList->insert(new Quantity("q"+pruned));
////				} else {
////					me=modifierEntry->clone();
////				}
////				me->modifier=pruned;
////				nounEntry->modifierDictionary.insertKeyAndValue(me->modifier,me);
////			}
////		
////		>>

		<<
			std::regex single_quote("'");
			std::string pruned;
			ModifierEntry * me;
			QuantityList * quantityList=new QuantityList;
			modifierEntry->insertQuantityList(quantityList); 
			for(auto it = modifiers.begin(); it != modifiers.end(); ++it) {
				pruned = std::regex_replace((*it), single_quote, "");

				//pruned=(*it)="");	// get rid of the single quotes.
				if(it==modifiers.begin()){
					me=modifierEntry;
					quantityList->append(new Quantity("q"+pruned));
				} else {
					me=modifierEntry->clone();
				}
				me->modifier=pruned;
				nounEntry->modifierDictionary.insertKeyAndValue(me->modifier,me);
			}
		
		>>


	;
 
get_extension_names![RWTValSlist<RWCString> & modifiers]:
	id:ID
		<<
			modifiers.push_back($id->getText());
		>>
		{
			Fd get_extension_names[modifiers]
		}
	;
   
extend_parameter_definition![ModifierEntry * & modifierEntry]:	// fsd-- 6.21P
		MODdVALUE mod_type[modifierEntry] { dimension_extension[modifierEntry] }
	|	MODIFIERdDESCRIPTOR extend_modifier_descriptor[modifierEntry]
	|	MNEMONICdONLY
	;  
  
  
mod_type![ModifierEntry *&modifierEntry]:		// fsd-- 6.21-1
		eid:EID
			<<
				modifierEntry->setTypeCode($eid);
			>>
	;
  
  
define_exchange_configuration_statement![AST * verb,ANTLRTokenPtr id]:        // fsd-- 6.22
	<<
		AST * ecl=0;
		Scope * which=0;
	>>
	//-------//verb:fstatnoDEFINEfd id:ID Fd
		external_global_fd>[which] EXCHANGE CONFIGURATION
	<<
		ecl=which->insert_label(new ExchangeConfigurationLabel(id),scope);
	>>
	( p1:protocol  (Fd pn:protocol << #(0,#p1,#pn); >> )* )
	<<  #( ecl, #(new VirtualExchangeConfiguration ,#p1 )); >>
	statement_terminator
	;
     
// 18.6.4 MAIN PROCEDURAL STRUCTURE
 
main_procedural_structure:                       // fsd-- 7.1
	( 
		main_procedural_statements
	|	block_structure                              //<<Esc_1 >> 

	)+
	;
//&    * This path is for <atlas program structure> only. * 


main_procedural_statements:                      // fsd-- 7.2 
		procedural_statements_data_processing
	|	procedural_statements_input_output
	|	procedural_statements_timing	// multiple anbiguities
	|	procedural_structures_control
	|	procedural_statements_control
	|	procedural_statements_signal
	|	procedural_statements_databus
	|	procedural_statements_leave
	|	procedural_statements_resume
	|	flag_statement
	;


flag_statement!:
	BFlag
	<< FlagStatement=1; >>
	;
	

procedural_statements_leave!:                     // fsd-- 7.2A 
	verb:fstatnoLEAVEfd
	( 
		lbs:leave_block_statement
		<< #0=#(#verb,#lbs); >>
	|	lis:leave_if_statement
		<< #0=#(#verb,#lis); >>
	|	lws:leave_while_statement
		<< #0=#(#verb,#lws); >>
	|	lds:leave_for_statement          
		<< #0=#(#verb,#lds); >>
	|	lps:leave_procedure_or_function_statement
		<< #0=#(#verb,#lps); >>
			//|       lfs:leave_function_statement
			//	<< #0=#(#verb,#lfs); >>
	)
	;
  
  
procedural_statements_resume:                    // fsd-- 7.2B
	( 
		resume_while_statement
	|	resume_for_statement    
	)
	;
 
// the correspondence between a leave- OR resume_statement
// (eg. leave_block_statement) and the appropriate 
// enclosing structure (e.g. block_structure) 
// is checked semantically. 


//  18.6.5 PROCEDURAL STATEMENTS - DATA PROCESSING

procedural_statements_data_processing:           // fsd-- 8.1 
		calculate_statement
	|
		compare_statement 
	;


calculate_statement!:                            // fsd-- 8.2
	verb:fstatnoCALCULATEfd as1:assignment_expression
		<< #0=#(0,#as1);>>
			(
				Fd asn:assignment_expression
					<<#0=#(0,#0,#asn);>>
			)*
			<<#0=#(#verb,#0);>>
	statement_terminator
	;
 	
   
      

 
predeclared_enumeration!:                        // fsd-- 8.2G
	(
		boolean_constant
	|	digclass_constant
	|	ASCII7
	)
	;

predefined_boolv_function!:
	(
		ZZEOF	LP ee:file	RP
			<< #0=#(new EOFFunction,#ee);>>
	|	ODD	LP 	(
					(oie:integer_expression
						<< #0=#(new ODDFunction,#oie);>>
					)?
				|	obe:bit_expression
					<< #0=#(new ODDFunction,#obe);>>
				)
				RP
	)
	;
		
predefined_bitv_function!:
	(
		PE	LP	(
					(sbee:strofbit_expression
						<< #0=#(new PEFunction,#sbee);>>
					)?
				|	bee:bit_expression
					<< #0=#(new PEFunction,#bee);>>
				)
				RP
	|	PO	LP	(
					(sbeo:strofbit_expression
						<< #0=#(new POFunction,#sbeo);>>
					)?
				|	beo:bit_expression
					<< #0=#(new POFunction,#beo);>>
				)
				RP
	)
	;
 	
predefined_intv_function!:
	(	
		ORD	LP	(	(oee:enumeration_expression
		)?
					<< #0=#(new ORDFunction,#oee);>>
				|	oce:char_expression
					<< #0=#(new ORDFunction,#oce);>>
				)
				RP
	|	LEN	LP	(
					(lsob:strofbit_expression
						<< #0=#(new LENFunction,#lsob);>>
					)?
				|	lsoc:strofchar_expression
					<< #0=#(new LENFunction,#lsoc);>>
				)
				RP
	|	LOCN	LP	(
					(losob1:strofbit_expression Fd losob2:strofbit_expression
						<< #0=#(new LOCNFunction,#losob1,#losob2);>>
					)?
				|	losoc1:strofchar_expression Fd losoc2:strofchar_expression
					<< #0=#(new LOCNFunction,#losoc1,#losoc2);>>
				)
				RP
	|	COUNT	LP	(
					(cbe:bit_expression Fd csbe:strofbit_expression)?
					<< #0=#(new COUNTFunction,#cbe,#csbe);>>
				|	cce:char_expression Fd csce:strofchar_expression
					<< #0=#(new COUNTFunction,#cce,#csce);>>
				)
				RP
	|	SIZE	LP f:file			RP
			<< #0=#(new SIZEFunction,#f);>>
	|	INT	LP ide:decimal_expression	RP
			<< #0=#(new INTFunction,#ide);>>
	|	ROUND	LP rde:decimal_expression	RP
			<< #0=#(new ROUNDFunction,#rde);>>
	|	ABS	LP aie:integer_expression	RP
			<< #0=#(new ABSFunction,#aie);>>
	)
	;



predefined_decv_function!:
	(
		DEC	LP ddce:digclass_expression Fd dsbe:strofbit_expression Fd die:integer_expression	RP
			<< #0=#(new DECFunction,#ddce,#dsbe,#die);>>
	|	SIN	LP sne:numeric_expression	RP
			<< #0=#(new SINFunction,#sne);>>
	|	COS	LP cne:numeric_expression	RP
			<< #0=#(new COSFunction,#cne);>>
	|	TAN	LP tne:numeric_expression	RP
			<< #0=#(new TANFunction,#tne);>>
	|	LN	LP lnne:numeric_expression	RP
			<< #0=#(new LNFunction,#lnne);>>
	|	LOG	LP lne:numeric_expression	RP
			<< #0=#(new LOGFunction,#lne);>>
	|	ALOG	LP alone:numeric_expression	RP
			<< #0=#(new ALOGFunction,#alone);>>
	|	ATAN	LP ane:numeric_expression	RP
			<< #0=#(new ATANFunction,#ane);>>
	|	EXP	LP ene:numeric_expression	RP
			<< #0=#(new EXPFunction,#ene);>>
	|	SQRT	LP sqne:numeric_expression	RP
			<< #0=#(new SQRTFunction,#sqne);>>
	|	ABS	LP absde:decimal_expression	RP
			<< #0=#(new ABSFunction,#absde);>>
	)
	;
	
	
predefined_numv_function!:
		(pif:predefined_intv_function)?
			<<#0=#pif;>>
	|	pdf:predefined_decv_function
			<<#0=#pdf;>>
	;

predefined_enumv_function!:
	( 
		SUCC	LP	se1:enumeration_expression	RP
			<< #0=#(new SUCCFunction,#se1);>>
	|	PRED	LP	pe1:enumeration_expression	RP
			<< #0=#(new PREDFunction,#pe1);>>
	)
	;

predefined_charv_function!:                            // fsd-- 8.2-1
	(
		CHAR	LP ccce:charclass_expression Fd csbe:strofbit_expression	RP
			<< #0=#(new CHARFunction,#ccce,#csbe);>>
	|	SUCC	LP	sce:char_expression	RP
			<< #0=#(new SUCCFunction,#sce);>>
	|	PRED	LP	pce:char_expression	RP
			<< #0=#(new PREDFunction,#pce);>>
	)
	;
 
predefined_strofcharv_function!:			// fsd-- 8.2-1
	(
		DATE	{LP RP}
			<< #0=#(new DATEFunction);>>
	|	COPY	LP csce:strofchar_expression Fd cie2:integer_expression Fd cie3:integer_expression RP
			<< #0=#(new COPYstocFunction,#csce,#cie2,#cie3);>>
	|	DELETE	LP dsce:strofchar_expression Fd die2:integer_expression Fd die3:integer_expression RP
			<< #0=#(new DELETEstocFunction,#dsce,#die2,#die3);>>
	|	INSERT	LP isce1:strofchar_expression Fd isce2:strofchar_expression Fd iie:integer_expression RP
			<< #0=#(new INSERTstocFunction,#isce1,#isce2,#iie);>>
	)
	;
  
predefined_strofbitv_function!:			// fsd-- 8.2-1
		BITS	LP bcce:charclass_expression Fd bce:char_expression RP
			<< #0=#(new BITSFunction,#bcce,#bce);>>
	|	COPY	LP csbe:strofbit_expression Fd cie2:integer_expression Fd cie3:integer_expression RP
			<< #0=#(new COPYstobFunction,#csbe,#cie2,#cie3);>>
	|	ROTATE	LP rsbe:strofbit_expression Fd rie:integer_expression RP
			<< #0=#(new ROTATEFunction,#rsbe,#rie);>>
	|	SHIFT	LP ssbe:strofbit_expression Fd sie:integer_expression RP
			<< #0=#(new SHIFTFunction,#ssbe,#sie);>>
	|	AdSHIFT	LP asbe:strofbit_expression Fd aie:integer_expression RP
			<< #0=#(new A_SHIFTFunction,#asbe,#aie);>>
	|	DIG	LP ddce:digclass_expression Fd dne:numeric_expression Fd 
				die3:integer_expression Fd die4:integer_expression RP
			<< #0=#(new DIGFunction,#ddce,#dne,#die3,#die4);>>
	|	DELETE	LP dsbe:strofbit_expression Fd delie2:integer_expression Fd delie3:integer_expression RP
			<< #0=#(new DELETEstobFunction,#dsbe,#delie2,#delie3);>>
	|	INSERT	LP isbe1:strofbit_expression Fd isbe2:strofbit_expression Fd iie:integer_expression RP
			<< #0=#(new INSERTstobFunction,#isbe1,#isbe2,#iie);>>
	;
	  
compare_statement!:                              // fsd-- 8.3
	<<
		ModifierEntry * modifierEntry=0;
	>>
	verb:fstatnoCOMPAREfd
	cf:comparison_field[modifierEntry]
	<<
		#0=#( #verb, #( new CompareVerb(scope),#(new CompareAction(this), #cf ) ) );
	>>
	statement_terminator
	;     

///comparison_field!:
///	(numeric_data_store)?
///	nds:numeric_data_store Fd ef:evaluation_field[modifierEntry,#nds]
  
  
// 18.6.6 PROCEDURAL STATEMENTS - INPUT/OUTPUT       
     
procedural_statements_input_output:              // fsd-- 9.1
		input_statement
	|	output_statement
	|	enable_file_access_statement
	|	disable_file_access_statement
	|	create_file_statement
	|	delete_file_statement
	;

input_statement!:                                // fsd-- 9.2       
	verb:fstatnoINPUTfd       
	(
		txti:text_input	<< #0=#txti; >>
	|	GOdNOGO
	|	dati:data_input	<< #0=#dati; >>
	)
	{ Fd mt:max_time	<< #0=#(#mt,#0); >> }
	<< #0=#(#verb,#0); >>
	statement_terminator
	;       
    
    
data_input!:                                     // fsd-- 9.2A       
	FROM f:file << #0 = #f; >>
	{  pos:integer_expression << #0 = #( new FilePositionAction, #0, #pos ); >> } 
	Fd   INTO an:aa_data_input_n
		<< #0 = #( new FileDataInputAction, #an, #0 ); >>
	;
aa_data_input_n!:
	a1:aa_data_input { Fd an:aa_data_input_n }
		<< #0 = #( new Proxy, #(new DataInputAction, #a1), #an ); >>
	;
 
aa_data_input!:
		(ar:array_range)?	<< #0 = #ar; >>
	|	ds:data_store		<< #0 = #ds; >>
	;
   
    
text_input!:                                     // fsd-- 9.2B       
	TEXT 
		(
			FROM f:file	<<#0=#f;>>
		|	()		<< #0 = #( new Proxy, getTokenLabel( "stdin" )); >>
		)	
		Fd  INTO	an:aa_text_input_n
				<<
					#0=#(new FileTextInputAction,#an,#0);
				>>
   ;       

aa_text_input_n!:
	a1:aa_text_input { Fd an:aa_text_input_n }
		<< #0 = #( new Proxy, #(new TextInputAction, #a1), #an ); >>

	;

aa_text_input!:
	( 
		(
			(ar:array_range)?	<< #0 = #ar; >>
		|	ds:data_store		<< #0 = #ds ; >>
		)
		{ ifo:input_format[(AST *)#0] << #0 = #( 0, #0, #ifo ); >> } 
	) 
	;
	
input_format![AST * arg]:   
	<< AST * targ = arg; >>   
	(	<<	if ( targ->getType() == ArraySliceTypeValue )
	  			targ = arg->eval( arg ); // targ = arg->theStorage( arg );
		>>
	)
	( 
		hfmt:HEXADECIMAL
			<<	if ( targ->getType() != StringOfBitTypeValue ){
					Error_Report( "HEXADECIMAL only allows STRING OF BIT.", $hfmt );
				}
				#0 = new HexFormatAction( $hfmt );
			>>
	|	bfmt:BINARY
			<<	if ( targ->getType() != StringOfBitTypeValue ){
					Error_Report( "BINARY only allows STRING OF BIT.", $bfmt );
				}
				#0 = new BinFormatAction( $bfmt ); >>
	|	ofmt:OCTAL
			<<	if ( targ->getType() != StringOfBitTypeValue ){
					Error_Report( "OCTAL only allows STRING OF BIT.", $ofmt );
				}
				#0 = new OctFormatAction( $ofmt ); >>
	//|	()
	//		<<	#0 = new NoFormatAction; >>
	)      
	;       


output_statement!:                               // fsd-- 9.3       
	verb:fstatnoOUTPUTfd
	(
		txto:text_output <<#0=#(#verb,#txto);>>
	|	dato:data_output <<#0=#(#verb,#dato);>>
	)
	statement_terminator
	;
    
data_output!:                                    // fsd-- 9.3A       
	TO f:file			<< #0 = #f; >>
	{ pos:integer_expression	<< #0 = #( new FilePositionAction, #0, #pos ); >> }
	Fd   FROM  an:aa_data_output_n
		<< #0 = #( new FileDataOutputAction, #an, #0 ); >>
	;

aa_data_output_n!:
	a1:aa_data_output { Fd an:aa_data_output_n }
		<< #0 = #( new Proxy, #(new DataOutputAction, #a1), #an ); >>
	;

aa_data_output!:
		(ar:array_range)?	<< #0 = #ar; >>
	|	e:expression		<< #0 = #e ; >>
	;
    
text_output!:                                    // fsd-- 9.3B       
	TEXT 
	(	
		TO f:file	<< #0 = #f; >>
	|	()		<< #0 = #( new Proxy, getTokenLabel( "stdout" ) ); >>
	) 
	Fd  FROM  an:aa_text_output_n
		<< #0 = #( new FileTextOutputAction, #an, #0 ); >>
	;

aa_text_output_n!:
	a1:aa_text_output { Fd an:aa_text_output_n }
		<< #0 = #( new Proxy, #(new TextOutputAction, #a1), #an ); >>
	;
  
aa_text_output!:
	(		(ar:array_range)?	<< #0 = #ar; >>
		|	e:expression		<< #0 = #e ; >>
	)
		{ ofo:output_format[(AST *)#0] << #0 = #( 0, #0, #ofo ); >> } 
	;

output_format![AST * arg]:                               // fsd-- 9.3C    
		<< AST * targ = arg; >>   
	COLON
	(	<<	if ( targ->getType() == ArraySliceTypeValue )
	  			targ = arg->eval( arg ); // targ = arg->theStorage( arg );
		>>
	)
	( 
		hfmt:HEXADECIMAL
			<<	if ( targ->getType() != StringOfBitTypeValue ){
					Error_Report( "HEXADECIMAL only allows STRING OF BIT.", $hfmt );
				}
				#0 = new HexFormatAction( $hfmt );
			>>
	|	efmt:EdFORMAT
			<< 	if ( targ->getType() != DecimalNumberValue ){
					Error_Report( "E-FORMAT only allows DECIMAL.", $efmt );
				}
				#0 = new ExpFormatAction( $efmt );
			>>
	|	bfmt:BINARY
			<<	if ( targ->getType() != StringOfBitTypeValue ){
					Error_Report( "BINARY only allows STRING OF BIT.", $bfmt );
				}
				#0 = new BinFormatAction( $bfmt ); >>
	|	ofmt:OCTAL
			<<	if ( targ->getType() != StringOfBitTypeValue ){
					Error_Report( "OCTAL only allows STRING OF BIT.", $ofmt );
				}
				#0 = new OctFormatAction( $ofmt ); >>
	|	()
			<<	#0 = new NoFormatAction; >>
	)      
	fw:integer_expression { COLON precision:integer_expression }
		<< #0 = #( #0, #fw, #precision ); >>
	;       

    
enable_file_access_statement!:                   // fsd-- 9.4
		<<	int	mode   = 0;
			int	create = FALSE;
		>>
	verb:fstatnoENABLEfd
	(
		(
				OUTPUT TO	<< mode = 2; >>	// Write Only mode
			|	IdO		<< mode = 3; >>	// Read Write mode
		)
		(
				OLD		<< create = FALSE; >>
			|	NEW		<< create = TRUE; >>
		)
		| INPUT FROM	<< create = FALSE; mode = 1; >>	// Read Only mode
	)

	e:strofchar_expression Fd via:VIA f:file       
                << #0 = #( new EnableFileAction( $via, mode, create ), #f, #e ); >>
                
	statement_terminator 
	;       
    

file!:						// fsd-- 9.4A
	<< TheType	storage_type;>>
	
	ds:data_store_id[&storage_type] 
		(
			<< storage_type == FileTypeValue >>?
			<< #0 = #ds; >>
		)
	;
  
  
disable_file_access_statement!:                  // fsd-- 9.5       
	verb:fstatnoDISABLEfd  f:file	<< #0 = #( new DisableFileAction, #f ); >>
	statement_terminator 
	;       
  
  
create_file_statement!:                          // fsd-- 9.6       
	verb:fstatnoCREATEfd e:strofchar_expression { Fd s:STATUS bds:boolean_data_store }
		<< #0 = #( new CreateFileAction( $s ), #e, #bds ); >>
	statement_terminator 
	;
       
    
delete_file_statement!:                          // fsd-- 9.7       
	verb:fstatnoDELETEfd e:strofchar_expression { Fd s:STATUS bds:boolean_data_store }       
		<< #0 = #( new DeleteFileAction( $s ), #e, #bds ); >>
	statement_terminator
	;       


// 18.6.7 PROCEDURAL STATEMENTS - CONTROL
    

procedural_statements_control:                   // fsd-- 10.1 
	 	go_to_statement
	|	perform_statement
	|	finish_statement
	//|	wait_for_manual_intervention_statement
	|	enable_disable_manual_intervention_structure
	|	enable_digital_configuration_statement
	|	enable_escape_statement
	|	disable_escape_statement
	|	wait_FOR_statements
	;
 
procedural_structures_control:                   // fsd-- 10.1 
		if_then_else_else_if_structure
	|	while_then_structure
	|	for_then_structure
	;
 
 
 
if_then_else_else_if_structure!:                  // fsd-- 10.2.1
	<< AST* itv; >>
	ifs:if_then_statement>[itv]
		<<
			#0 = #ifs;
			incrementContextDepth( itv, EntryIF );
		>>
	ifbody:procedural_segment_if_then_else_else_if
	( 	//( fstatnoELSE IF )?
		elif:else_if_then_statement
		elifbody:procedural_segment_if_then_else_else_if
		<<
			#0 = #( 0, #0, new TargetAction( #elif ), #ifbody, #elif );
			#ifbody = #elifbody;
			incrementContextLevel();
		>>
	)*
	{ 
		el:else_statement               
		elbody:procedural_segment_if_then_else_else_if
		<<
			#0=#(0,#0,new TargetAction(#el),#ifbody,#el);
			#ifbody=#elbody;
			incrementContextLevel();
		>>   
	}
	eif:end_if_statement
	<<
		#0=#(0,#0,new TargetAction(#eif),#ifbody,#eif);
		decrementContextDepth();
	>>
	;    


if_then_statement!>[AST * itv]:                              // fsd-- 10.2.2       
	verb:fstatnoIFfd bexpr:boolean_expression Fd THEN
	<<
		#0=#(0,#verb,#($itv=new IfThenVerb,#bexpr));
	>>     
	statement_terminator 
	;       


procedural_segment_if_then_else_else_if:         // fsd-- 10.2.3 
	(main_procedural_statements)+
	; 
    
    
leave_if_statement!:                             // fsd-- 10.2.4
	ift:IF     
	{ STEP st:statement_number }       
	<<#0=#(new LeaveAction($ift,scope),verifyLeaveResumeTarget(#st,EntryIF));>>	
	statement_terminator 
	;       
    
    
else_statement!:                                 // fsd-- 10.2.5       
	verb:fstatnoELSE
	<<
		#0=#(0,#verb,new ElseVerb);
	>>     
	statement_terminator 
	;       
    
    
end_if_statement!:                               // fsd-- 10.2.6       
	verb:fstatnoENDfd IF       
	<<
		#0=#(0,#verb,new EndIfVerb);
	>>     
	{ Fd message_text }       
	statement_terminator 
	;       


else_if_then_statement!:                         // fsd-- 10.2.7       
	verb:fstatnoELSE IF Fd bexpr:boolean_expression
		Fd THEN
		<<
			#0=#(0,#verb,#(new ElseIfThenVerb,#bexpr));
		>>     
	statement_terminator
	;       
        

while_then_structure!:                            // fsd-- 10.3.1 
	<< AST* wtea;>>
	wts:while_then_statement>[wtea]
		<< 
			incrementContextDepth(wtea,EntryWHILE);
		>>
	wbody:procedural_segment_while_then   
	ew:end_while_statement
		<<
			#0=#(0,#wts,new TargetAction(#ew),#wbody,new TargetAction(#wts),#ew);
			decrementContextDepth();
		>>
	;   
    

while_then_statement!>[AST * wtea]:                           // fsd-- 10.3.2       
	verb:fstatnoWHILEfd bexpr:boolean_expression Fd THEN 
	<<
		#0=#(0,#verb,#($wtea=new WhileThenVerb,#bexpr));
	>>      
	statement_terminator
	;       
    

procedural_segment_while_then:                   // fsd-- 10.3.3 
	(main_procedural_statements)+
	; 
    

leave_while_statement!:                          // fsd-- 10.3.4
	wh:WHILE  
	{ STEP st:statement_number }      
	<<#0=#(new LeaveAction($wh,scope),verifyLeaveResumeTarget(#st,EntryWHILE));>>
	statement_terminator 
	;       
    
    
resume_while_statement!:                         // fsd-- 10.3.5       
	verb:fstatnoRESUMEfd WHILE       
	{ STEP st:statement_number }       
	<<#0=#(#verb,new TargetAction(verifyLeaveResumeTarget(#st,EntryWHILE)));>>
	statement_terminator
	;       
    
    
end_while_statement!:                            // fsd-- 10.3.6       
	verb:fstatnoENDfd WHILE
	<<
		#0=#(0,#verb,new EndWhileVerb);
	>>       
	{ Fd message_text }       
	statement_terminator
	;       
    

for_then_structure!:                             // fsd-- 10.4.1
	<< AST* ftea;>>
	fts:for_then_statement>[ftea]
		<<
			incrementContextDepth(ftea,EntryFOR);
		>>
	psft:procedural_segment_for_then 
	efs:end_for_statement 
		<< 
			decrementContextDepth();
			#0=#(0,#fts, new TargetAction(#efs),#psft,new TargetAction(ftea),#efs);
		>>
	;  



for_then_statement!>[AST * ftea]:                             // fsd-- 10.4.2       
	verb:fstatnoFORfd ftc:for_then_control
	<<	$ftea=new ForThenEntryAction;
		#0=#(0,#verb,new ForThenVerb,#($ftea,#ftc));
	>>      
	Fd THEN statement_terminator
	;       
    
 
for_then_control!:
		(integer_data_store)?
		ids:integer_data_store EQUALS iftr:integer_for_then_range[#ids]
				<<#0=#iftr;>>
	|	(decimal_data_store)?
		dds:decimal_data_store EQUALS dftr:decimal_for_then_range[#dds]
				<<#0=#dftr;>>
	|	(char_data_store)?
		cds:char_data_store EQUALS cftr:char_for_then_range[#cds]
				<<#0=#cftr;>>
	|	eds:enumeration_data_store EQUALS eftr:enum_for_then_range[#eds]
				<<#0=#eftr;>>
	;


integer_for_then_range![AST * ids]:
	<<ASTVector range;>>
		
	iefr:integer_expression<<range.append(#iefr);>>
	(
		THRU ieto:integer_expression { BY ieby:integer_expression}
			<<
				#0=#(0,#(new ForCompareAction,ids,#ieto),
						#(new ScalarIteratorAction,#iefr,#ieby));
			>>
	|	( Fd ien:integer_expression<<range.append(#ien);>>)*
			<<
				#0=#(0,#(new ForCompareAction,ids),
						#(new ArrayIteratorAction,new ArrayType(range)));
			>>
	)
	;

decimal_for_then_range![AST * dds]:
	<<ASTVector range;>>
	defr:decimal_expression<<range.append(#defr);>>
	(
		THRU deto:decimal_expression { BY deby:decimal_expression}
			<<
				#0=#(0,#(new ForCompareAction,dds,#deto),
						#(new ScalarIteratorAction,#defr,#deby));
			>>
	|	( Fd den:decimal_expression <<range.append(#den);>>)*
			<<
				#0=#(0,#(new ForCompareAction,dds),
							#(new ArrayIteratorAction,new ArrayType(range)));
			>>
	)
	;

char_for_then_range![AST * cds]:
	<<ASTVector range;>>
	cefr:char_expression<<range.append(#cefr);>>
	(
		THRU ceto:char_expression
			<<
				#0=#(0,#(new ForCompareAction,cds,#ceto),
						#(new ScalarIteratorAction,#cefr));
			>>
	|	( Fd cen:char_expression <<range.append(#cen);>>)*
			<<
				#0=#(0,#(new ForCompareAction,cds),
					#(new ArrayIteratorAction,new ArrayType(range)));
			>>
	)
	;	

enum_for_then_range![AST * eds]:
	<<ASTVector range;>>
	eefr:enumeration_expression
		<<
			#eefr->check( eds );
			range.append(#eefr);
		>>
	(
		THRU eeto:enumeration_expression
			<<
				#eeto->check( eds );
				#0=#(0,#(new ForCompareAction,eds,#eeto),
						#(new ScalarIteratorAction,#eefr));
			>>
	|	( Fd een:enumeration_expression <<range.append(#een);>>)*
			<<
				#0=#(0,#(new ForCompareAction,eds),
					#(new ArrayIteratorAction,new ArrayType(range)));
			>>
	)
	;
   

    

procedural_segment_for_then:                     // fsd-- 10.4.3 
	(main_procedural_statements)+
	; 

leave_for_statement!:                            // fsd-- 10.4.4       
	fr:FOR       
	{ STEP st:statement_number }       
	<<#0=#(new LeaveAction($fr,scope),verifyLeaveResumeTarget(#st,EntryFOR));>>
	statement_terminator 
	;       
    
    
resume_for_statement!:                           // fsd-- 10.4.5
	<< AST * target=0, * skiptarget=0;>>
	verb:fstatnoRESUMEfd FOR       
	{ STEP st:statement_number }
	<<
		#0=#(#verb,
			#(new ResumeForAction,verifyLeaveResumeTarget(#st,EntryFOR)));
	>>
	statement_terminator 
	;       
    
    
end_for_statement!:                              // fsd-- 10.4.6       
	verb:fstatnoENDfd FOR
	<< #0=#(0,#verb,new EndForVerb);>>      
	{ Fd message_text }       
	statement_terminator
	;       
    
    
go_to_statement!:                                // fsd-- 10.5 
	<<   TargetStatement * target;TargetStatement * unresolved;>>   
	verb:fstatnoGO_TOfd
		st:STEP
		snum:statement_number
		statement_terminator
////		<<
////			#0=#(#verb,#(new GoToAction($st,scope),#snum));
////			int tno=((StatementNumber *)#snum)->getInteger(0);
////			unresolved=new TargetStatement(#snum,ContextLevel,ContextDepth);
////			if(!GoToTargets.findValue(tno,target)){
////				insertUnresolved(tno,#snum);
////			} else {
////				#snum->init(verifyGoToTarget(unresolved,target));
////			}
////		>>
		<<
			#0=#(#verb,#(new GoToAction($st,scope),#snum));
			int tno=((StatementNumber *)#snum)->getInteger(0);
			unresolved=new TargetStatement(#snum,ContextLevel,ContextDepth);
			auto it = GoToTargets.find(tno);
			if (it == GoToTargets.end()) {
				insertUnresolved(tno,#snum);
			} else {
				#snum->init(verifyGoToTarget(unresolved,target));
			}
		>>

	;       
    
    
perform_statement!:                              // fsd-- 10.6       
	verb:fstatnoPERFORMfd  proc:procedure  pv:perform_variables[#proc]
	<< #0 = #( #verb, #pv );>>    
	statement_terminator
	;       
    
    
perform_variables![AST * proc]:                              // fsd-- 10.6-A
		<<	
			// get the number of variables+results
			int vars=proc->getInteger(0)+proc->getInteger(1);
			int times=1;
			int check=vars;
		>>
		{
			c:Fd perform_table[vars,times]
			<<	
				if(check){ // make sure not zero
					if((vars % check)){
						Error_Report( "Procedure arg count and TABLE VARS do not divide", #c );
					}
				} else if(vars){				
					Error_Report( "Procedure does not take any arguments", $c );
				}
			>>
		}
		( 
			ap1:actual_parameters[proc,vars,times,check]>[times]
			<<
				#0=#(new ExecuteProcedureAction,#(#(0,new Proxy,#ap1),proc) );
			>>
			(
				Fd apn:actual_parameters[proc,vars,times,check]>[times]
				<<
					#0=#(0,#0,#(new ExecuteProcedureAction,#(#(0,new Proxy,#apn),proc) ) );
				>>
			)*
		)
		<<
			if(times!=0){
				Error_Report( "PROCEDURE/TABLE TIMES  number is wrong", VerbLineNo );
			}
			#0=#(0,new ExecuteAction,#0);
		>>
	;       
    
    
perform_table![int & vars,int & times]:						// fsd-- 10.6-B       
	TABLE v:unsigned_integer_number VAR t:unsigned_integer_number TIMES
	<< vars=#v->getInteger();times=#t->getInteger(); >>
	;       
    
    
actual_parameters![AST * proc,int vars,int times,int check]>[int left]:	// fsd-- 10.6-C     
	<< int argcount=0,rescount=0;>>
	{
		lp:LP ( expr1:expression 
				<<
					argcount++;vars--;
					if(sane())ArgCheck(argcount,proc,#expr1,$lp->getLine()); // we know that "(" is a token
					#0=#(0,#(new Proxy,new Proxy),#expr1); // Build a list e1
				>>
		
				( 
					dd:Fd exprn:expression
						<<
							argcount++;vars--;
							int l=$dd->getLine();// we know that "," is a token
							if(sane())ArgCheck(argcount,proc,#exprn,l);
							#(0,#expr1,#exprn); // add e2,e3,...
						>>
				)* 
			
			) RP 
	}
	{ 
		RESULT llp:LP
			(
				res1:data_store
				<<
					rescount++;vars--;
					if(!#0)#0=#(new Proxy,new Proxy);	// must be no arguments, just results
					if(sane())ResCheck(rescount,proc,#res1,$llp->getLine()); // we know that "(" is a token
					#(0,#0->down(),#res1); // Build a list res1 on the down-right node
				>>
				(
					ddd:Fd resn:data_store
					<<
						rescount++;vars--;
						int l=$ddd->getLine();// we know that "," is a token
						if(sane())ResCheck(rescount,proc,#res1,l); // we know that "(" is a token
						#(0,#res1,#resn); // add res2,res3,....
					>>
				)*
			) RP 
	}
	<<
		if(argcount!=proc->getInteger(0)){
			Error_Report( "PROCEDURE ARG count and PERFORM ARG count do not match", VerbLineNo );
		} else if(rescount!=proc->getInteger(1)){
			Error_Report( "PROCEDURE RESULT count and PERFORM RESULT count do not match", VerbLineNo );
		} else if(vars && (vars % check) ){
			Error_Report( "Procedure arg count and TABLE VARS do not divide", VerbLineNo );
		}
		if(!#0)#0=#(new Proxy,new Proxy);
		$left=times-1;
	>>
	;       
      
    

finish_statement!:                               // fsd-- 10.7       
	verb:fstatnoFINISH
	<<#0=#(#verb,#(new FinishVerb,new FinishAction));>>
	statement_terminator 
	;       
  

enable_disable_manual_intervention_structure:	// fsd-- 10.8.1 
		enable_manual_intervention_statement
	|	disable_manual_intervention_statement 
	;

enable_manual_intervention_statement!:		// fsd-- 10.8.2       
	verb:fstatnoENABLEfd MANUAL INTERVENTION     
	<<#0=#(#verb,new EnableManualVerb(lManualInterventionEvent));>>
	statement_terminator
	;       
    
    
disable_manual_intervention_statement!:		// fsd-- 10.8.3       
	verb:fstatnoDISABLEfd MANUAL INTERVENTION       
	<<#0=#(#verb,new DisableManualVerb(lManualInterventionEvent));>>
	statement_terminator 
	;       
    

wait_for_manual_intervention_statement!:         // fsd-- 10.9       
	verb:fstatnoWAIT_FORfd
		MANUAL INTERVENTION       
	<<
		#0=	#(
				#verb,
			#(	new WaitForManualVerb,
				new WaitForManualAction(lManualInterventionEvent)
			)
			);
	>>
	statement_terminator 
	;       
    
    
enable_digital_configuration_statement:         // fsd-- 10.10      
	verb:fstatnoENABLEfd DIGITAL CONFIGURATION configuration  
	{ 
		LP 
			aa_enable_digital_configuration_statement_1 
			(Fd aa_enable_digital_configuration_statement_1)*
			RP 
	}
	statement_terminator
	;
  
aa_enable_digital_configuration_statement_1:
	(
		decimal_number[0]
	|	data_store
	|	EQUALS 
	)
	;  
  
enable_escape_statement!:			// fsd-- 10.11.1
	verb:fstatnoENABLEfd ESCAPE TO PROCEDURE proc:procedure
	IF ei:escape_indicator
	<<
		#0=
		#(	#verb,
			#(new EnableEscapeVerb(this),
				#ei,#(new ExecuteProcedureAction,#(#(0,new Proxy,#(new Proxy,new Proxy)),#proc) )
			)
		);
		if((0!=#proc->getInteger(0))||(0!=#proc->getInteger(1))){
			Error_Report( "ESCAPE PROCEDURE can not have arguments", #proc );
		}
	>>
	statement_terminator
	;
  
  
escape_indicator!:				// fsd-- 10.11.1A
	( 
		ei:event_indicator		<< #0=#(new Proxy,#ei		); >>
	|	MAXdTIME			<< #0=#(new Proxy,lMAXdTIME	); >>
	|  	MANUALdINTERVENTION		<< #0=#(new Proxy,lMANUAL	); >>
	|  	GO				<< #0=#(new Proxy,lGO		); >>
	|  	NOGO				<< #0=#(new Proxy,lNOGO		); >>
	|  	HI				<< #0=#(new Proxy,lHI		); >>
	|  	LO				<< #0=#(new Proxy,lLO		); >>
	)
	;
  
  
disable_escape_statement!:			// fsd-- 10.11.2
	verb:fstatnoDISABLEfd ESCAPE
	{ ei:escape_indicator }
	<<#0=#(#verb,#(new DisableEscapeVerb(this),#ei));>>
	statement_terminator
	;

external_global_fd!>[Scope * which]:
	(	EXTERNAL Fd	<< $which=getExternalScope()	; >>
	|	GLOBAL Fd	<< $which=getGlobalScope()	; >>
	|	()		<< $which=getScope()		; >>
	)
	;
	
wait_FOR_statements:
		(fstatnoWAIT_FORfd	MANUAL INTERVENTION)?
		wait_for_manual_intervention_statement
	|	wait_for_statement
	;

}
