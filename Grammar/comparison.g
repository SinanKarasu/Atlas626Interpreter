class AtlasParser {


comparison_field![ModifierEntry * & modifierEntry]:
			(integer_data_store)?
			ids:integer_data_store		Fd ief:integer_evaluation_field		[modifierEntry,#ids ]
							<< #0=#(0,#ief,#ids);>>
		|	(decimal_data_store )?
			dds:decimal_data_store		Fd def:decimal_evaluation_field		[modifierEntry,#dds ]
							<< #0=#(0,#def,#dds);>>
		|	(strofchar_data_store)?
			scds:strofchar_data_store	Fd scef:strofchar_evaluation_field	[modifierEntry,#scds]
							<< #0=#(0,#scef,#scds);>>
		|	(connection_data_store)?
			cds:connection_data_store	Fd cef:connection_evaluation_field	[modifierEntry,#cds ]
							<< #0=#(0,#cef,#cds);>>
		|	(boolean_data_store)?
			bds:boolean_data_store		Fd bef:boolean_evaluation_field		[modifierEntry,#bds ]
							<< #0=#(0,#bef,#bds);>>
		|	(strofbit_data_store)?
			sbds:strofbit_data_store	Fd sbef:strofbit_evaluation_field	[modifierEntry,#sbds]
							<< #0=#(0,#sbef,#sbds);>>
		|	(bit_data_store)?
			btds:bit_data_store		Fd btef:bit_evaluation_field		[modifierEntry,#btds]
							<< #0=#(0,#btef,#btds);>>
		|	(char_data_store)?
			chds:char_data_store		Fd chef:char_evaluation_field		[modifierEntry,#chds]
							<< #0=#(0,#chef,#chds);>>
		|	//(enumeration_data_store)?
			eds:enumeration_data_store	Fd eef:enumeration_evaluation_field	[modifierEntry,#eds ]
							<< #0=#(0,#eef,#eds);>>
		;


/////------------------start-------------------------------------------------------------------------------	
integer_evaluation_field![ModifierEntry *& modifierEntry,AST * ds]:      
	<<
		DimensionEntry * dimensionEntry=0;
		AST * slot=0;
	>>
	(
		ull:integer_up_low_limit[dimensionEntry,ds]
		<< #0=#ull;>>
	|
		(
			oeo:ordered_evaluation_operator>[slot] orq:integer_quantity[modifierEntry,dimensionEntry]
				<<
					#(slot,#(new Proxy,ds),#orq);
					#0=#oeo;
				>>
		|	ueo:unordered_comparison_operator
			(
				(urq:integer_quantity[modifierEntry,dimensionEntry])?
				<< #0=#(new NoGoAction(this),#(#ueo,#(new Proxy,ds),#urq));>>
			|	une:integer_expression
				<< #0=#(new NoGoAction(this),#(#ueo,#(new Proxy,ds),#une));>>
			)
		)
	)
	;  

integer_up_low_limit![DimensionEntry *& dimensionEntry, AST * ds]:      
	<< ModifierEntry * modifierEntry=0; >>
	
	{ nom:NOM nm:integer_quantity[modifierEntry,dimensionEntry] << #0=#(new NOMOperator($nom),#nm);>> }
	uL:UL ul:integer_quantity[modifierEntry,dimensionEntry]
		lL:LL ll:integer_quantity[modifierEntry,dimensionEntry]
	<<
		#0=#(new WITHINOperator,
			#(new HiAction(this),#(new ULOperator($uL),#(new Proxy,ds),#ul)),
			#(new LoAction(this),#(new LLOperator($lL),#(new Proxy,ds),#ll,#0))
			);
	>>
	;
	
integer_quantity![ModifierEntry *& modifierEntry,DimensionEntry * &dimensionEntry]:
	ne:integer_expression << #0=#ne; >>
		{
			d:dimension[modifierEntry,dimensionEntry]
			<< #0 = #( new DimensionedNumberType( modifierEntry ), #0, #d ); >>
		}
	;

/////------------------end-------------------------------------------------------------------------------	

/////------------------start-------------------------------------------------------------------------------	
decimal_evaluation_field![ModifierEntry *& modifierEntry,AST * ds]:      
	<<
		DimensionEntry * dimensionEntry=0;
		AST * slot=0;
	>>
	(
		ull:decimal_up_low_limit[dimensionEntry,ds]
		<< #0=#ull;>>
	|
		(
			oeo:ordered_evaluation_operator>[slot] orq:decimal_quantity[modifierEntry,dimensionEntry]
				<<
					#(slot,#(new Proxy,ds),#orq);
					#0=#oeo;
				>>
		|	ueo:unordered_comparison_operator
			(
				(urq:decimal_quantity[modifierEntry,dimensionEntry])?
				<< #0=#(new NoGoAction(this),#(#ueo,#(new Proxy,ds),#urq));>>
			|	une:decimal_expression
				<< #0=#(new NoGoAction(this),#(#ueo,#(new Proxy,ds),#une));>>
			)
		)
	)
	;  

decimal_up_low_limit![DimensionEntry *& dimensionEntry, AST * ds]:      
	<< ModifierEntry * modifierEntry=0; >>
	
	{ nom:NOM nm:decimal_quantity[modifierEntry,dimensionEntry] << #0=#(new NOMOperator($nom),#nm);>> }
	uL:UL ul:decimal_quantity[modifierEntry,dimensionEntry]
		lL:LL ll:decimal_quantity[modifierEntry,dimensionEntry]
	<<
		#0=#(new WITHINOperator,
			#(new HiAction(this),#(new ULOperator($uL),#(new Proxy,ds),#ul)),
			#(new LoAction(this),#(new LLOperator($lL),#(new Proxy,ds),#ll,#0))
			);
	>>
	;
	
decimal_quantity![ModifierEntry *& modifierEntry,DimensionEntry * &dimensionEntry]:
	ne:decimal_expression << #0=#ne; >>
		{
			d:dimension[modifierEntry,dimensionEntry]
			<< #0 = #( new DimensionedNumberType( modifierEntry ), #0, #d ); >>
		}
	;

/////------------------end-------------------------------------------------------------------------------	

/////------------------start-------------------------------------------------------------------------------	
strofchar_evaluation_field![ModifierEntry *& modifierEntry,AST * ds]:      
	<<
		DimensionEntry * dimensionEntry=0;
		AST * slot=0;
	>>
	(
		ull:strofchar_up_low_limit[dimensionEntry,ds]
		<< #0=#ull;>>
	|
		(
			oeo:ordered_evaluation_operator>[slot] orq:strofchar_quantity[modifierEntry,dimensionEntry]
				<<
					#(slot,#(new Proxy,ds),#orq);
					#0=#oeo;
				>>
		|	ueo:unordered_comparison_operator
			(
				(urq:strofchar_quantity[modifierEntry,dimensionEntry])?
				<< #0=#(new NoGoAction(this),#(#ueo,#(new Proxy,ds),#urq));>>
			|	une:strofchar_expression
				<< #0=#(new NoGoAction(this),#(#ueo,#(new Proxy,ds),#une));>>
			)
		)
	)
	;  

strofchar_up_low_limit![DimensionEntry *& dimensionEntry, AST * ds]:      
	<< ModifierEntry * modifierEntry=0; >>
	
	{ nom:NOM nm:strofchar_quantity[modifierEntry,dimensionEntry] << #0=#(new NOMOperator($nom),#nm);>> }
	uL:UL ul:strofchar_quantity[modifierEntry,dimensionEntry]
		lL:LL ll:strofchar_quantity[modifierEntry,dimensionEntry]
	<<
		#0=#(new WITHINOperator,
			#(new HiAction(this),#(new ULOperator($uL),#(new Proxy,ds),#ul)),
			#(new LoAction(this),#(new LLOperator($lL),#(new Proxy,ds),#ll,#0))
			);
	>>
	;
	
strofchar_quantity![ModifierEntry *& modifierEntry,DimensionEntry * &dimensionEntry]:
	ne:strofchar_expression << #0=#ne; >>
	;

/////------------------end-------------------------------------------------------------------------------	

/////------------------start-------------------------------------------------------------------------------	
connection_evaluation_field![ModifierEntry *& modifierEntry,AST * ds]:      
	<<
		DimensionEntry * dimensionEntry=0;
		AST * slot=0;
	>>
	(
		ull:connection_up_low_limit[dimensionEntry,ds]
		<< #0=#ull;>>
	|
		(
			oeo:ordered_evaluation_operator>[slot] orq:connection_quantity[modifierEntry,dimensionEntry]
				<<
					#(slot,#(new Proxy,ds),#orq);
					#0=#oeo;
				>>
		|	ueo:unordered_comparison_operator
			(
				(urq:connection_quantity[modifierEntry,dimensionEntry])?
				<< #0=#(new NoGoAction(this),#(#ueo,#(new Proxy,ds),#urq));>>
			|	une:connection_expression
				<< #0=#(new NoGoAction(this),#(#ueo,#(new Proxy,ds),#une));>>
			)
		)
	)
	;  

connection_up_low_limit![DimensionEntry *& dimensionEntry, AST * ds]:      
	<< ModifierEntry * modifierEntry=0; >>
	
	{ nom:NOM nm:connection_quantity[modifierEntry,dimensionEntry] << #0=#(new NOMOperator($nom),#nm);>> }
	uL:UL ul:connection_quantity[modifierEntry,dimensionEntry]
		lL:LL ll:connection_quantity[modifierEntry,dimensionEntry]
	<<
		#0=#(new WITHINOperator,
			#(new HiAction(this),#(new ULOperator($uL),#(new Proxy,ds),#ul)),
			#(new LoAction(this),#(new LLOperator($lL),#(new Proxy,ds),#ll,#0))
			);
	>>
	;
	
connection_quantity![ModifierEntry *& modifierEntry,DimensionEntry * &dimensionEntry]:
	ne:connection_expression << #0=#ne; >>
	;

/////------------------end-------------------------------------------------------------------------------	
/////------------------start-------------------------------------------------------------------------------	
boolean_evaluation_field![ModifierEntry *& modifierEntry,AST * ds]:      
	<<
		DimensionEntry * dimensionEntry=0;
		AST * slot=0;
	>>
	(
		ull:boolean_up_low_limit[dimensionEntry,ds]
		<< #0=#ull;>>
	|
		(
			oeo:ordered_evaluation_operator>[slot] orq:boolean_quantity[modifierEntry,dimensionEntry]
				<<
					#(slot,#(new Proxy,ds),#orq);
					#0=#oeo;
				>>
		|	ueo:unordered_comparison_operator
			(
				(urq:boolean_quantity[modifierEntry,dimensionEntry])?
				<< #0=#(new NoGoAction(this),#(#ueo,#(new Proxy,ds),#urq));>>
			|	une:boolean_expression
				<< #0=#(new NoGoAction(this),#(#ueo,#(new Proxy,ds),#une));>>
			)
		)
	)
	;  

boolean_up_low_limit![DimensionEntry *& dimensionEntry, AST * ds]:      
	<< ModifierEntry * modifierEntry=0; >>
	
	{ nom:NOM nm:boolean_quantity[modifierEntry,dimensionEntry] << #0=#(new NOMOperator($nom),#nm);>> }
	uL:UL ul:boolean_quantity[modifierEntry,dimensionEntry]
		lL:LL ll:boolean_quantity[modifierEntry,dimensionEntry]
	<<
		#0=#(new WITHINOperator,
			#(new HiAction(this),#(new ULOperator($uL),#(new Proxy,ds),#ul)),
			#(new LoAction(this),#(new LLOperator($lL),#(new Proxy,ds),#ll,#0))
			);
	>>
	;
	
boolean_quantity![ModifierEntry *& modifierEntry,DimensionEntry * &dimensionEntry]:
	ne:boolean_expression << #0=#ne; >>
	;

/////------------------end-------------------------------------------------------------------------------	

/////------------------start-------------------------------------------------------------------------------	
strofbit_evaluation_field![ModifierEntry *& modifierEntry,AST * ds]:      
	<<
		DimensionEntry * dimensionEntry=0;
		AST * slot=0;
	>>
	(
		ull:strofbit_up_low_limit[dimensionEntry,ds]
		<< #0=#ull;>>
	|
		(
			oeo:ordered_evaluation_operator>[slot] orq:strofbit_quantity[modifierEntry,dimensionEntry]
				<<
					#(slot,#(new Proxy,ds),#orq);
					#0=#oeo;
				>>
		|	ueo:unordered_comparison_operator
			(
				(urq:strofbit_quantity[modifierEntry,dimensionEntry])?
				<< #0=#(new NoGoAction(this),#(#ueo,#(new Proxy,ds),#urq));>>
			|	une:strofbit_expression
				<< #0=#(new NoGoAction(this),#(#ueo,#(new Proxy,ds),#une));>>
			)
		)
	)
	;  

strofbit_up_low_limit![DimensionEntry *& dimensionEntry, AST * ds]:      
	<< ModifierEntry * modifierEntry=0; >>
	
	{ nom:NOM nm:strofbit_quantity[modifierEntry,dimensionEntry]  << #0=#(new NOMOperator($nom),#nm);>>}
	uL:UL ul:strofbit_quantity[modifierEntry,dimensionEntry]
		lL:LL ll:strofbit_quantity[modifierEntry,dimensionEntry]
	<<
		#0=#(new WITHINOperator,
			#(new HiAction(this),#(new ULOperator($uL),#(new Proxy,ds),#ul)),
			#(new LoAction(this),#(new LLOperator($lL),#(new Proxy,ds),#ll,#0))
			);
	>>
	;
	
strofbit_quantity![ModifierEntry *& modifierEntry,DimensionEntry * &dimensionEntry]:
	ne:strofbit_expression << #0=#ne; >>
		{
			d:dimension[modifierEntry,dimensionEntry]
			<< #0 = #( new DimensionedNumberType( modifierEntry ), #0, #d ); >>
		}
	;

/////------------------end-------------------------------------------------------------------------------	

/////------------------start-------------------------------------------------------------------------------	
bit_evaluation_field![ModifierEntry *& modifierEntry,AST * ds]:      
	<<
		DimensionEntry * dimensionEntry=0;
		AST * slot=0;
	>>
	(
		ull:bit_up_low_limit[dimensionEntry,ds]
		<< #0=#ull;>>
	|
		(
			oeo:ordered_evaluation_operator>[slot] orq:bit_quantity[modifierEntry,dimensionEntry]
				<<
					#(slot,#(new Proxy,ds),#orq);
					#0=#oeo;
				>>
		|	ueo:unordered_comparison_operator
			(
				(urq:bit_quantity[modifierEntry,dimensionEntry])?
				<< #0=#(new NoGoAction(this),#(#ueo,#(new Proxy,ds),#urq));>>
			|	une:bit_expression
				<< #0=#(new NoGoAction(this),#(#ueo,#(new Proxy,ds),#une));>>
			)
		)
	)
	;  

bit_up_low_limit![DimensionEntry *& dimensionEntry, AST * ds]:      
	<< ModifierEntry * modifierEntry=0; >>
	
	{ nom:NOM nm:bit_quantity[modifierEntry,dimensionEntry]  << #0=#(new NOMOperator($nom),#nm);>> }
	uL:UL ul:bit_quantity[modifierEntry,dimensionEntry]
		lL:LL ll:bit_quantity[modifierEntry,dimensionEntry]
	<<
		#0=#(new WITHINOperator,
			#(new HiAction(this),#(new ULOperator($uL),#(new Proxy,ds),#ul)),
			#(new LoAction(this),#(new LLOperator($lL),#(new Proxy,ds),#ll,#0))
			);
	>>
	;
	
bit_quantity![ModifierEntry *& modifierEntry,DimensionEntry * &dimensionEntry]:
	ne:bit_expression << #0=#ne; >>
		{
			d:dimension[modifierEntry,dimensionEntry]
			<< #0 = #( new DimensionedNumberType( modifierEntry ), #0, #d ); >>
		}
	;

/////------------------end-------------------------------------------------------------------------------	

/////------------------start-------------------------------------------------------------------------------	
char_evaluation_field![ModifierEntry *& modifierEntry,AST * ds]:      
	<<
		DimensionEntry * dimensionEntry=0;
		AST * slot=0;
	>>
	(
		ull:char_up_low_limit[dimensionEntry,ds]
		<< #0=#ull;>>
	|
		(
			oeo:ordered_evaluation_operator>[slot] orq:char_quantity[modifierEntry,dimensionEntry]
				<<
					#(slot,#(new Proxy,ds),#orq);
					#0=#oeo;
				>>
		|	ueo:unordered_comparison_operator
			(
				(urq:char_quantity[modifierEntry,dimensionEntry])?
				<< #0=#(new NoGoAction(this),#(#ueo,#(new Proxy,ds),#urq));>>
			|	une:char_expression
				<< #0=#(new NoGoAction(this),#(#ueo,#(new Proxy,ds),#une));>>
			)
		)
	)
	;  

char_up_low_limit![DimensionEntry *& dimensionEntry, AST * ds]:      
	<< ModifierEntry * modifierEntry=0; >>
	
	{ nom:NOM nm:char_quantity[modifierEntry,dimensionEntry] << #0=#(new NOMOperator($nom),#nm);>> }
	uL:UL ul:char_quantity[modifierEntry,dimensionEntry]
		lL:LL ll:char_quantity[modifierEntry,dimensionEntry]
	<<
		#0=#(new WITHINOperator,
			#(new HiAction(this),#(new ULOperator($uL),#(new Proxy,ds),#ul)),
			#(new LoAction(this),#(new LLOperator($lL),#(new Proxy,ds),#ll,#0))
			);
	>>
	;
	
char_quantity![ModifierEntry *& modifierEntry,DimensionEntry * &dimensionEntry]:
	ne:char_expression << #0=#ne; >>
	;

/////------------------end-------------------------------------------------------------------------------	

/////------------------start-------------------------------------------------------------------------------	
enumeration_evaluation_field![ModifierEntry *& modifierEntry,AST * ds]:      
	<<
		DimensionEntry * dimensionEntry=0;
		AST * slot=0;
	>>
	(
		ull:enumeration_up_low_limit[dimensionEntry,ds]
		<< #0=#ull;>>
	|
		(
			oeo:ordered_evaluation_operator>[slot] orq:enumeration_quantity[modifierEntry,dimensionEntry]
				<<
					#(slot,#(new Proxy,ds),#orq);
					#0=#oeo;
				>>
		|	ueo:unordered_comparison_operator
			(
				(urq:enumeration_quantity[modifierEntry,dimensionEntry])?
				<< #0=#(new NoGoAction(this),#(#ueo,#(new Proxy,ds),#urq));>>
			|	une:enumeration_expression
				<< #0=#(new NoGoAction(this),#(#ueo,#(new Proxy,ds),#une));>>
			)
		)
	)
	;  

enumeration_up_low_limit![DimensionEntry *& dimensionEntry, AST * ds]:      
	<< ModifierEntry * modifierEntry=0; >>
	
	{ nom:NOM nm:enumeration_quantity[modifierEntry,dimensionEntry] << #0=#(new NOMOperator($nom),#nm);>> }
	uL:UL ul:enumeration_quantity[modifierEntry,dimensionEntry]
		lL:LL ll:enumeration_quantity[modifierEntry,dimensionEntry]
	<<
		#0=#(new WITHINOperator,
			#(new HiAction(this),#(new ULOperator($uL),#(new Proxy,ds),#ul)),
			#(new LoAction(this),#(new LLOperator($lL),#(new Proxy,ds),#ll,#0))
			);
	>>
	;
	
enumeration_quantity![ModifierEntry *& modifierEntry,DimensionEntry * &dimensionEntry]:
	ne:enumeration_expression << #0=#ne; >>
	;

/////------------------end-------------------------------------------------------------------------------	

}
