class AtlasParser {

assignment_expression!:
	(	(integer_data_store)?
		iae:integer_assignment_expression
			<< #0 = #iae; >>
	|	(decimal_data_store)?
		dae:decimal_assignment_expression
			<< #0 = #dae; >>
	|	(char_data_store)?
		cae:char_assignment_expression
			<< #0 = #cae; >>
	|	(strofchar_data_store)?
		scae:strofchar_assignment_expression
			<< #0 = #scae; >>
	|	(bit_data_store)?
		bitae:bit_assignment_expression
			<< #0 = #bitae; >>
	|	(strofbit_data_store)?
		sbae:strofbit_assignment_expression
			<< #0 = #sbae; >>
	|	(boolean_data_store)?
		bae:boolean_assignment_expression
			<< #0 = #bae; >>
	|	(array_data_store)?
		ads:array_assignment_expression
			<< #0 = #ads; >>
	|	(connection_data_store)?
		coae:connection_assignment_expression
			<< #0 = #coae; >>
	|	(record_data_store)?
		rae:record_assignment_expression
			<< #0 = #rae; >>
	|	cc:calculate_condition 
			<< #0 = #cc; >>
	)
	;   

integer_assignment_expression!:
		target:integer_data_store equals:EQUALS source:integer_expression
			<<#0=#(new AssignAction($equals),#source,#target);>>
	;
	<<
		Error_Report(" Statement right side does not evaluate to INTEGER",VerbLineNo);
	>> 

decimal_assignment_expression!:
		target:decimal_data_store equals:EQUALS source:numeric_expression
			<<#0=#(new AssignAction($equals),#source,#target);>>
	;   
	<<
		Error_Report(" Statement right side does not evaluate to DECIMAL",VerbLineNo);
	>> 

char_assignment_expression!:
		target:char_data_store equals:EQUALS source:char_expression
			<<#0=#(new AssignAction($equals),#source,#target);>>
	;
	<<
		Error_Report(" Statement right side does not evaluate to CHAR",VerbLineNo);
	>> 

strofchar_assignment_expression!:
		target:strofchar_data_store equals:EQUALS source:strofchar_expression
			<<#0=#(new AssignAction($equals),#source,#target);>>
	;   
	<<
		Error_Report(" Statement right side does not evaluate to STRING OF CHAR",VerbLineNo);
	>> 

bit_assignment_expression!:
		target:bit_data_store equals:EQUALS source:bit_expression
			<<#0=#(new AssignAction($equals),#source,#target);>>
	;
	<<
		Error_Report(" Statement right side does not evaluate to BIT",VerbLineNo);
	>> 
	
strofbit_assignment_expression!:
	target:strofbit_data_store equals:EQUALS source:strofbit_expression
		<< #0=#(new AssignAction($equals),#source,#target); >>
	;   
	<< Error_Report( "Statement right side does not evaluate to STRING OF BIT", VerbLineNo ); >> 


boolean_assignment_expression!:
	target:boolean_data_store equals:EQUALS source:boolean_expression
		<< #0 = #( new AssignAction($equals), #source, #target ); >>
	;   
	<< Error_Report( "Statement right side does not evaluate to BOOLEAN", VerbLineNo ); >> 

array_assignment_expression!:
	target:array_data_store equals:EQUALS source:array_expression
		<< #0 = #( new AssignAction($equals), #source, #target ); >>
	;
	<< Error_Report( "Statement right side does not evaluate to ARRAY", VerbLineNo ); >>


connection_assignment_expression!:
	target:connection_data_store equals:EQUALS source:connection_expression
		<< #0 = #( new AssignAction($equals), #source, #target ); >>
	;   
	<< Error_Report( "Statement right side does not evaluate to CONNECTION", VerbLineNo ); >> 

record_assignment_expression!:
	target:record_data_store equals:EQUALS source:record_data_store
		<<
			#0 = #( new AssignAction($equals), #source, #target );
			if ( #target->getInteger() != #source->getInteger() )
				Error_Report( "Record Types must match for assignment", VerbLineNo );
		>>
	;   
	<< Error_Report( "Statement right side does not evaluate to record", VerbLineNo ); >> 

notable_condition!:                                      // fsd-- 14.12       
			MANUAL INTERVENTION	<< #0=#(new Proxy,lMANUAL); >>
		|	MAXdTIME		<< #0=#(new Proxy,lMAXdTIME); >> 
	;
  
nonnotable_condition!:
		|	HI	<< #0=#(new Proxy,lHI); >>
		|	LO	<< #0=#(new Proxy,lLO); >>
		|	GO	<< #0=#(new Proxy,lGO); >>
		|	NOGO	<< #0=#(new Proxy,lNOGO); >>
	;

condition:
		notable_condition
	|	nonnotable_condition
	;
	
calculate_condition!:                            // fsd-- 8.2A
	target:condition	equals:EQUALS	source:boolean_constant 
			<<#0=#(new AssignAction($equals),#source,#target);>>
	;



numeric_data_store!:
		(ids:integer_data_store)?
			<<#0=#ids;>>
	|	dds:decimal_data_store
			<<#0=#dds;>>
	;
	<< Error_Report( "Numeric data-store expected ", VerbLineNo ); >> 


connection_data_store!:
		<< TheType st;>>
		ds:data_store_id[&st] 
			(
				<<st==ConnectionTypeValue>>?
				<<#0=#ds;>>
			)
	;
	<< Error_Report( "CONNECTION data-store expected ", VerbLineNo ); >> 

enumeration_data_store!:
		<< TheType st;>>
		ds:data_store_id[&st] 
			(
				<<st==EnumerationTypeValue>>?
				<<#0=#ds;>>
			)
	;
	<< Error_Report( "ENUMERATION data-store expected ", VerbLineNo ); >> 

integer_data_store!:
		<< TheType st;>>
		ds:data_store_id[&st] 
			(
				<<st==IntegerNumberValue>>?
				<<#0=#ds;>>
			)
	;
	<< Error_Report( "INTEGER data-store expected ", VerbLineNo ); >> 

array_data_store!:
		<< TheType st;>>
		ds:data_store_id[&st]
			(
				<<st==ArrayTypeValue>>?
				<<#0=#ds;>>
			)
	;
	<< Error_Report( "ARRAY data-store expected ", VerbLineNo ); >> 

integer_constant_identifier!:
		<< TheType st;>>
		ci:constant_identifier[&st] 
			(
				<<st==IntegerNumberValue>>?
				<<#0=#ci;>>
			)
	;
	<< Error_Report( "INTEGER CONSTANT identifier expected ", VerbLineNo ); >> 

decimal_constant_identifier!:
		<< TheType st;>>
		ci:constant_identifier[&st] 
			(
				<<st==DecimalNumberValue>>?
				<<#0=#ci;>>
			)
	;
	<< Error_Report( "DECIMAL CONSTANT identifier expected ", VerbLineNo ); >> 
	
decimal_data_store!:
		<< TheType st;>>
		ds:data_store_id[&st] 
			(
				<<st==DecimalNumberValue>>?
				<<#0=#ds;>>
			)
	;
	<< Error_Report( "DECIMAL data-store expected ", VerbLineNo ); >> 
	
char_data_store!:
		<< TheType st;>>
		ds:data_store_id[&st] 
			(
				<<st==CharTypeValue>>?
				<<#0=#ds;>>
			)
	;
	<< Error_Report( "CHAR data-store expected ", VerbLineNo ); >> 
	
strofchar_data_store!:
		<< TheType st;>>
		ds:data_store_id[&st] 
			(
				<<st==StringOfCharTypeValue>>?
				<<#0=#ds;>>
			)
	;
	<< Error_Report( "STRING OF CHAR data-store expected ", VerbLineNo ); >> 
	
bit_data_store!:
		<< TheType st;>>
		ds:data_store_id[&st] 
			(
				<< st == BitTypeValue >>?
				<< #0 = #ds; >>
			)

	;
	<< Error_Report( "BIT data-store expected ", VerbLineNo ); >> 

strofbit_data_store!:
		<< TheType st; >>
	ds:data_store_id[&st] 
		(
			<< st == StringOfBitTypeValue >>?
				<< #0 = #ds; >>
		)
	;
	<< Error_Report( "STRING OF BIT data-store expected ", VerbLineNo ); >> 

record_data_store!:
		<< TheType st; >>
	ds:data_store_id[&st]
		(
			<< st == RecordTypeValue >>? 
				<< #0 = #ds; >>
		)
	;
	<< Error_Report( "RECORD data-store expected ", VerbLineNo ); >> 


boolean_data_store!:
		<< TheType st; >>
	ds:data_store_id[&st] 
		(
			<< st == BooleanTypeValue >>?
				<< #0 = #ds; >>
		)
	;
	<< Error_Report( "BOOLEAN data-store expected ", VerbLineNo ); >> 


digclass_data_store!:
		<< TheType st; >>
	ds:data_store_id[&st] 
		(
			<< st == DigClassTypeValue >>?
				<< #0 = #ds; >>
		)
	;
	<< Error_Report( "DIG-CLASS data-store expected ", VerbLineNo ); >> 

charclass_data_store!:
		<< TheType st; >>
	ds:data_store_id[&st] 
		(
			<< st == CharClassTypeValue >>?
				<< #0 = #ds; >>
		)
	;
	<< Error_Report( "CHAR-CLASS data-store expected ", VerbLineNo ); >> 

data_store!:
		<< TheType st; >>
	dsi:data_store_id[&st]
		<< #0 = #dsi; >>
	;


data_store_id![TheType *st]:                          // fsd-- 14.39
		<<	AST *	vv;
			AST *	v;
			Scope * savescope = scope;
			TheType	sst;
		>>
		
	var:variable_identifier[st,&vv]
		<< #0 = #var; >>
		
	{	<< TheType	ssst = *st; >>
		(	<< sst = ssst; >>	//init-action
			vi:variable_indices[&sst, vv, #var]
			<< #0 = #vi; >>
			( << *st = sst; >> )
		)? 
		|
		(
			<< TheType	ssst = *st; >>	//init-action

			(	po:PERIODop
					<< ssst == RecordTypeValue >>?
						[Error_Report("Record Variable expected",VerbLineNo);]
				( << scope = vv->ASTdown()->getScope(); >> )	// note that this is an init-action...
										// do not remove these ()
				
				recfld:data_store_id[st]
					<< #0 = #recfld; >>
				( << scope = scope->getPrev(); >> )
			)
		)
	}
	;
	<<
			scope = savescope;
	>>

variable_indices![TheType *st,AST *vv,AST * var]:

		<< AST *v;TheType ssst=*st;>>
	(
		lp:LP
			(
				<<
						ssst == ArrayTypeValue
					||	ssst == StringOfBitTypeValue
					||	ssst == StringOfCharTypeValue
				>>?
					[Error_Report( "Array/String Variable expected", $lp );]
			)
			
		(	// do not remove these ()
			<<	// note that this is an init-action....
				v = vv->eval();		// get the ArrayType address
				v = v->eval(v);		// wind down and get the element
				ssst = v->getType();
			>>

			indx1:index
				<<
					#0 = #( new IndexAction, var, #indx1 );
				>>
		)
		(
			cn:Fd
				(
					<<
							ssst == ArrayTypeValue
						||	ssst == StringOfBitTypeValue
						||	ssst == StringOfCharTypeValue
					>>?
						[Error_Report( "Array/String is not multidimensional.", $cn );]
				)
			indxn:index
				(	// do not remove these ()
					<<	// note that this is an init-action....
						v = v->eval(v); // wind down more
						ssst = v->getType();
					>> 
					<<
						#0 = #( new IndexAction, #0, #indxn );
					>>
				)
		)*
		
		RP	(	<< *st=ssst; >>	)
	)
	
	{
		(LP)? vin:variable_indices[ &ssst, v, (AST *)#0 ]
			<< #0 = #vin;	>>
		(	<< *st = ssst;	>>	)
	}
	;

array_expression!:
	ads:array_data_store
		<< #0 = #ads; >>
	;

array_range!:                                    // fsd-- 14.40
	<<
		ASTVector range;
		AST * lo, *hi, *by;
	>>
	ads:array_data_store
		LP
			( 
				(
					ir:index_range>[ lo, hi, by ]
					<< #0 = #( new ArraySliceType(lo, hi, by), #ads ); >>
				)?
			|
				idx1:index	<< range.append( #idx1 ); >>
				
				( Semi_Colon idxn:index	<< range.append( #idxn ); >> )+ 
				<< #0 = #( new ArrayElementsType(range), #ads ); >>
			) 
			RP
	;

index_range!>[AST * lo, AST * hi, AST * by]:                  // fsd-- 14.40A
	idx1:index THRU idx2:index
		{
			BY by:index
			<< $by = #by; >>
		}
		<< $lo = #idx1; $hi = #idx2; >>
	;

index:
		(integer_index)?
	|	(char_index)?
	|	enum_index
	;

integer_index:                                         // fsd-- 14.17
	integer_expression
	;       

char_index:                                         // fsd-- 14.17
	char_expression
	;
	      
enum_index:                                         // fsd-- 14.17
	enumeration_expression
	;

connection_expression!:
		cds:connection_data_store
			<< #0=#cds;>>
	|	cc:connection_constant
			<< #0=#cc;>>
	;


character_string!:                               // fsd-- 15.10.1   
		cs:Character_String
			<< #0 = new StringOfCharType($cs); >>
	|	sc:Single_Char
			<< #0 = new StringOfCharType($sc); >>

	;  
	
single_char!:
	sc:Single_Char
		<< #0=new CharType($sc); >>
	;
  
boolean_constant!:
		ZzzFALSE
			<< #0=new BooleanType(0);>>
	|	ZzzTRUE
			<< #0=new BooleanType(1);>>
	; 
		 
digclass_constant!:
		bnr:BNR 
			<< #0=new DigClassType($bnr);>>
	|	b1c:B1C 
			<< #0=new DigClassType($b1c);>>
	|	b2c:B2C
			<< #0=new DigClassType($b2c);>>
	|	bsm:BSM 
			<< #0=new DigClassType($bsm);>>
	|	bcd:BCD 
			<< #0=new DigClassType($bcd);>>
	|	sbcd:SBCD
			<< #0=new DigClassType($sbcd);>>
	; 

boolean_expression_term!:		// fsd-- 8.2D
		(store:	boolean_data_store)?		<< #0 = #store; >>
	|	bc:	boolean_constant		<< #0 = #bc; >>
	|	cond:	notable_condition		<< #0 = #cond; >>
	|	(be:	boolean_evaluation)?		<< #0 = #be; >>
	|	func:	boolv_function_expression	<< #0 = #func; >>
	|	LP expr:boolean_expression RP		<< #0 = #expr; >>
	;

notted_boolean_expression!:		// prec. level 1
		notop:NOTop	nottedarg:boolean_expression_term
			<< #0 = #( new NOTOperator($notop), #nottedarg ); >>
			
	|	primarg:boolean_expression_term
			<< #0 = #primarg; >>
		
	|	nnc:nonnotable_condition
			<< #0 = #nnc; >>
		
	;

xor_boolean_expression!:
		arg1:notted_boolean_expression
			<< #0 = #arg1; >>
		(
			xorop:XOR 	xorarg:notted_boolean_expression		
				<<
					#0=#(new XOROperator($xorop),#(0,#0,#xorarg));
				>>			
		)*
		;


and_boolean_expression!:
		arg1:xor_boolean_expression
			<< 
				#0=#arg1;
			>>
		( 
			andop:AND 	andarg:xor_boolean_expression		
				<<
					#0=#(new ANDOperator($andop),#(0,#0,#andarg));
				>>			
		)*
		;


or_boolean_expression!:
		arg1:and_boolean_expression
			<< #0 = #arg1; >>
		( 
			orop:OR 	orarg:and_boolean_expression		
				<< #0 = #( new OROperator($orop),#(0,#0,#orarg) ); >>			
		)*
		;


boolean_expression!:
		(
			num1:numeric_expression
				<<
					#0=#num1;
				>>
		 	ncomp:comparison_operator numn:numeric_expression
		 		<<
		 			#0=#(#ncomp,#(0,#0,#numn));
		 		>>
		 )?
		|	
		(
			schar1:strofchar_expression
				<<
					#0=#schar1;
				>>
		 	scomp:comparison_operator scharn:strofchar_expression
		 		<<
		 			#0=#(#scomp,#(0,#0,#scharn));
		 		>>
		)?
		|
		(
			conn1:connection_expression
				<<
					#0=#conn1;
				>>
		 	conncomp:comparison_operator connn:strofchar_expression
		 		<<
		 			#0=#(#conncomp,#(0,#0,#connn));
		 		>>
		)?
		|
		(
			bool1:or_boolean_expression
				<<
					#0=#bool1;
				>>
		 	(
		 		boolcomp:unordered_comparison_operator booln:or_boolean_expression
		 			<<
		 				#0=#(#boolcomp,#(0,#0,#booln));
		 			>>
		 	)*
	 	)?
	 	|
		(
			sbit1:strofbit_expression
				<<
					#0=#sbit1;
				>>
		 	sbcomp:comparison_operator sbitn:strofbit_expression
		 		<<
		 			#0=#(#sbcomp,#(0,#0,#sbitn));
		 		>>
		 )?
	 	|
		(
			bit1:bit_expression
				<<
					#0=#bit1;
				>>
		 	bcomp:comparison_operator bitn:bit_expression
		 		<<
		 			#0=#(#bcomp,#(0,#0,#bitn));
		 		>>
		)?
		|
		(
			c1:char_expression
				<<
					#0=#c1;
				>>
		 	ccomp:comparison_operator cn:char_expression
		 		<<
		 			#0=#(#ccomp,#(0,#0,#cn));
		 		>>
		)?
		|
		(
			e1:enumeration_expression
				<<
					#0=#e1;
				>>
		 	ecomp:comparison_operator en:enumeration_expression
		 		<<
		 			#e1->check(#en);
		 			#0=#(#ecomp,#(0,#0,#en));
		 		>>
		)
	;
   

enumeration_expression!:
		(	eds:	enumeration_data_store	<< #0=#eds;>>	)?
	|	(	ec:	enumeration_constant		<< #0=#ec;>>	)?
	|	(	efe:	enumv_function_expression	<< #0=#efe;>>	)
	;

digclass_expression!:
		dc:	digclass_constant		<< #0=#dc;>>
	|	dds:	digclass_data_store		<< #0=#dds;>>
	;
	
charclass_constant!:
		a7:ASCII7
		<< #0=new CharClassType($a7);>>
	;

charclass_expression!:
		cc:charclass_constant
		<< #0=#cc;>>
	|	cds:charclass_data_store
		<< #0=#cds;>>
	;

//--------------------------------------------------------------------------------------------//
//   String Of Bit expression                                                                 //
//--------------------------------------------------------------------------------------------//

strofbit_expression_term!:						// fsd-- 8.2D
		dig:	digital_number			<<#0=#dig;>>
	|	(store:	strofbit_data_store		<<#0=#store;>>	)?
	|	func:	strofbitv_function_expression	<<#0=#func;>>
	|	LP expr:strofbit_expression		<<#0=#expr;>> RP
	;

notted_strofbit_expression!: // prec. level 1
		notop:NOTop
			nottedarg:strofbit_expression_term
				<<
					#0=#(new NOTOperator($notop),#nottedarg);
				>>
	|	primarg:strofbit_expression_term
			<<
				#0=#primarg;
			>>
		
	;

xor_strofbit_expression!:
		arg1:notted_strofbit_expression
			<<
				#0=#arg1;
			>>
		(
			xorop:XOR 	xorarg:notted_strofbit_expression		
				<<
					#0=#(new XOROperator($xorop),#(0,#0,#xorarg));
				>>			
		)*
		;


and_strofbit_expression!:
		arg1:xor_strofbit_expression
			<< 
				#0=#arg1;
			>>
		( 
			andop:AND 	andarg:xor_strofbit_expression		
				<<
					#0=#(new ANDOperator($andop),#(0,#0,#andarg));
				>>			
		)*
		;


or_strofbit_expression!:
		arg1:and_strofbit_expression
			<<
				#0=#arg1;
			>>
		( 
			orop:OR 	orarg:and_strofbit_expression		
				<<
					#0=#(new OROperator($orop),#(0,#0,#orarg));
				>>			
		)*
		;


strofbit_expression!:
		(
			barg1:or_strofbit_expression
			<<
				#0=#barg1;
			>>
		) 	
	;
   
//--------------------------------------------------------------------------------------------//
//   Bit expression                                                                           //
//--------------------------------------------------------------------------------------------//
bit_expression_term!:						// fsd-- 8.2D
		(store:	bit_data_store		<<#0=#store;>>)?
	|	LP expr:bit_expression RP	<<#0=#expr;>>
	|	constant:single_bit		<<#0=#constant;>>
	|	func:bitv_function_expression	<<#0=#func;>>
	;

notted_bit_expression!: // prec. level 1
		notop:	NOTop nottedarg:bit_expression_term
				<<#0=#(new NOTOperator($notop),#nottedarg);>>
	|	primarg:bit_expression_term
				<<#0=#primarg;>>
		
	;

xor_bit_expression!:
		arg1:notted_bit_expression	<<#0=#arg1;>>
		(
			xorop:XOR 	xorarg:notted_bit_expression		
				<<#0=#(new XOROperator($xorop),#(0,#0,#xorarg));>>			
		)*
		;


and_bit_expression!:
		arg1:xor_bit_expression	<< #0=#arg1;>>
		( 
			andop:AND 	andarg:xor_bit_expression		
				<<#0=#(new ANDOperator($andop),#(0,#0,#andarg));>>			
		)*
		;


or_bit_expression!:
		arg1:and_bit_expression	<< #0=#arg1;>>
		( 
			orop:OR 	orarg:and_bit_expression		
				<<#0=#(new OROperator($orop),#(0,#0,#orarg));>>			
		)*
		;


bit_expression!:
		barg1:or_bit_expression	<< #0=#barg1;>>
	;
   



// Numeric expression constructs


connection_constant!:                                       // fsd-- 8.2E
		CONNECTION c:connection
		<< #0=#c;>>
	;



enumeration_constant!:                                       // fsd-- 8.2E
		pe:predeclared_enumeration
		<< #0=#pe;>>
	|	ee:enumeration_element
		<< #0=#ee;>>
	;


integer_constant!:                                       // fsd-- 8.2E
		udn:unsigned_integer_number
		<< #0=#udn;>>
	|	ici:integer_constant_identifier
		<< #0=#ici;>>
	;

decimal_constant!:                                       // fsd-- 8.2E
		udn:unsigned_decimal_number[0]
		<< #0=#udn;>>
	|
	(	ici:integer_constant_identifier
		<< #0=#(new IntegerToDecimalAction,#ici);>>
	)?
	|	dci:decimal_constant_identifier
		<< #0=#dci;>>
	;


 
constant:                                       // fsd-- 8.2E
		connection_constant
	|	character_string
	|	(integer_constant)?
	|	(decimal_constant)?
	|	(boolean_constant)?
	|	enumeration_constant
	|	digital_number
	;
 




//--------------------------------------------------------------------------------------------//
//   numeric expression constructs                                                            //
//--------------------------------------------------------------------------------------------//

//--------------------------------------------------------------------------------------------//
//   integer resultant constructs                                                             //
//--------------------------------------------------------------------------------------------//

unary_operator!:
			minusop:MINUSop
				<<
					#0=new NegativeOperator($minusop);
				>>			
		|	plusop:PLUSop
		|
	;


integer_expression_term!:						// fsd-- 8.2D
		(nc	:	integer_constant		<<#0=#nc;>>)?
	|	LP ne	:	integer_expression 	RP	<<#0=#ne;>>
	|	(nds	:	integer_data_store		<<#0=#nds;>>)?
	|	nfe	:	intv_function_expression	<<#0=#nfe;>>
	;





exponential_integer_expression!: // prec. level 2
		arg1:integer_expression_term	<< #0=#arg1;>>
		(
			expop:EXPOop exparg:integer_expression_term
			<<#0=#(new ExpoOperator($expop),#(0,#0,#exparg));>>			
		)*
	;


multiplying_integer_expression!: 	// prec. level 3
		arg1:exponential_integer_expression	<<#0=#arg1;>>
		( 
			mulop:MULop 	mularg:exponential_integer_expression		
				<<#0=#(new TimesOperator($mulop),#(0,#0,#mularg));>>			
		|	divop:DIVop divarg:exponential_integer_expression	
				<<#0=#(new DIVOperator($divop),#(0,#0,#divarg));>>			
		|	modop:MODop modarg:exponential_integer_expression
				<<#0=#(new MODOperator($modop),#(0,#0,#modarg));>>				
		)*
	;

add_sub_operator!:
	(	addop:PLUSop	<<#0=new AddOperator($addop);>>
	|	minusop:MINUSop	<<#0=new SubtractOperator($minusop);>>
	)
	;
	
integer_expression!: // prec. level 4
		uo:unary_operator arg1:multiplying_integer_expression	<<#0=#(#uo,#arg1);>>
		(
			as:add_sub_operator	argn:multiplying_integer_expression
				<<#0=#(#as,#(0,#0,#argn));>>			
		)*
	;



//--------------------------------------------------------------------------------------------//
//   decimal resultant constructs                                                             //
//--------------------------------------------------------------------------------------------//
decimal_expression_term!:						// fsd-- 8.2D
	(	nc:	decimal_constant			<<#0=#nc;>>)?
	|	(nds:	numeric_data_store			<<#0=#nds;>>)?
	|	LP ne:decimal_expression RP			<<#0=#ne;>>
	|	nfe:decv_function_expression	<<#0=#nfe;>>
	;



exponential_decimal_expression!: // prec. level 2
		arg1:decimal_expression_term	<<#0=#arg1;>>
		(
			expop:EXPOop exparg:decimal_expression_term
				<<#0=#(new ExpoOperator($expop),#(0,#0,#exparg));>>			
		)*
	;


multiplying_decimal_expression!: 	// prec. level 3
		arg1:exponential_decimal_expression	<<#0=#arg1;>>
		( 
			mulop:MULop 	mularg:exponential_decimal_expression		
				<<#0=#(new TimesOperator($mulop),#(0,#0,#mularg));>>			
 		| 	divideop:DIVIDEop dividearg:exponential_decimal_expression	
				<<#0=#(new DivideOperator($divideop),#(0,#0,#dividearg));>>			
		)*
	;
	
decimal_expression!:		// prec. level 4
		uo:unary_operator  arg1:multiplying_decimal_expression	<<#0=#(#uo,#arg1);>>
		(
			as:add_sub_operator	argn:multiplying_decimal_expression
				<<#0=#(#as,#(0,#0,#argn));>>			
		)*
	;


//--------------------------------------------------------------------------------------------//
//   str_char resultant constructs                                                            //
//--------------------------------------------------------------------------------------------//

string_of_char!:
		cs:character_string			<< #0 = #cs; >>
	|	LP sce:strofchar_expression RP		<< #0 = #sce; >>
	|	( scds:strofchar_data_store		<< #0 = #scds; >> )?
	|	( scfe:strofcharv_function_expression	<< #0 = #scfe; >> )
	;


strofchar_expression!:
	arg1:string_of_char	<< #0 = #arg1; >>
	(
		concatop:CONCATEop	concatarg:string_of_char
			<<#0 = #(new ConcatOperator($concatop),#(0,#0,#concatarg));>>
	)*			
	;


//--------------------------------------------------------------------------------------------//
//   char resultant constructs                                                                //
//--------------------------------------------------------------------------------------------//

char_expression!:
		(cds:	char_data_store			<< #0=#cds;>>	)?
	|	(pdfn:	predefined_charv_function	<< #0=#pdfn;>>	)?
	|	(cs:	single_char			<< #0=#cs;>>	)
	|	(cvf:	charv_function			<< #0=#cvf;>>	)
	;

expression!:                                     // fsd-- 8.2B
		(ce:	char_expression		<< #0= #ce;	>>)?
	|	(soce:	strofchar_expression	<< #0= #soce;	>>)?
	|	(be:	bit_expression		<< #0= #be;	>>)?
	|	(sobe:	strofbit_expression	<< #0= #sobe;	>>)?
	|	(ie:	integer_expression	<< #0= #ie;	>>)?
	|	(ne:	decimal_expression	<< #0= #ne;	>>)?
	|	(le:	boolean_expression	<< #0= #le;	>>)?
	|	(cone:	connection_expression	<< #0= #cone;	>>)?
	|	(ae:	array_expression	<< #0= #ae;	>>)?
	|	(ee:	enumeration_expression	<< #0= #ee;	>>)
	;

numeric_expression!:
		(ie:	integer_expression	<< #0= #ie;>>	)?
	|	(ne:	decimal_expression	<< #0= #ne;>>	)
	;


boolv_function!:
		<< TheType	storage_type;>>
		func:function[&storage_type] 
			(
				<<storage_type == BooleanTypeValue>>?
			)
			<<#0 = #func;>>
	;


enumv_function!:
		<< TheType	storage_type;>>
		func:function[&storage_type] 
			(
				<<storage_type == EnumerationTypeValue>>?
			)
			<<#0 = #func;>>
	;

bitv_function!:
		<< TheType	storage_type;>>
		func:function[&storage_type] 
			(
				<<storage_type == BitTypeValue>>?
			)
			<<#0 = #func;>>
	;

strofbitv_function!:
		<< TheType	storage_type;>>
		func:function[&storage_type] 
			(
				<<storage_type == StringOfBitTypeValue>>?
			)
			<<#0=#func;>>
	;

charv_function!:
		<< TheType	storage_type;>>
		func:function[&storage_type] 
			(
				<<storage_type == CharTypeValue>>?
			)
			<<#0=#func;>>
	;

strofcharv_function!:
		<< TheType	storage_type;>>
		func:function[&storage_type] 
			(
				<<storage_type == StringOfCharTypeValue>>?
				<<#0=#func;>>
			)
	;

intv_function!:
		<< TheType	storage_type;>>
		func:function[&storage_type] 
			(
				<<storage_type == IntegerNumberValue>>?
				<<#0=#func;>>
			)
	;

decv_function!:
		<< TheType	storage_type;>>
		func:function[&storage_type] 
			(
				<<storage_type == DecimalNumberValue>>?
				<<#0=#func;>>
			)
	;


//--------------------------------------------------------------------------------------------//
//   function expression constructs                                                            //
//--------------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------//
//                                                                                            //
//--------------------------------------------------------------------------------------------//

boolv_function_expression!:
		<< int argcount=0; >>
		(	pdfn:	predefined_boolv_function	<< #0=#pdfn;	>>)?
		|
		(	fn:boolv_function	args:function_arguments[argcount,#fn]
				<<
					#0=#(new ExecuteFunctionAction,#fn,#args);
					((ActionAST *)#0)->setInteger(0,argcount);
				>>
		)
	;


enumv_function_expression!:
		<< int argcount=0; >>
		(	pdfn:predefined_enumv_function		<< #0=#pdfn; >>)?
	|
		(	fn:enumv_function
				args:function_arguments[argcount,#fn]
				<<
					#0=#(new ExecuteFunctionAction,#fn,#args);
					((ActionAST *)#0)->setInteger(0,argcount);
					// ((ActionAST *)#0)->check();
				>>
		)
	;


//--------------------------------------------------------------------------------------------//
//                                                                                            //
//--------------------------------------------------------------------------------------------//


intv_function_expression!:                            // fsd-- 8.2F    
		<< int argcount=0; >>
		(	pdfn:predefined_intv_function	<< #0=#pdfn; >>)?
	|	(	fn:intv_function
			args:function_arguments[argcount,#fn]
				<<
					#0=#(new ExecuteFunctionAction,#fn,#args);
					((ActionAST *)#0)->setInteger(0,argcount);
				>>
		)
	;

decv_function_expression!:                            // fsd-- 8.2F    
		<< int argcount=0; >>
		(pdfn:predefined_decv_function	<< #0=#pdfn; >>	)?
	|	(decv_function)?
		fn:decv_function args:function_arguments[argcount,#fn]
				 <<
					#0=#(new ExecuteFunctionAction,#fn,#args);
					((ActionAST *)#0)->setInteger(0,argcount);
				>>
	|	ife:intv_function_expression
		<< #0=#(new IntegerToDecimalAction,#ife);>>
	;
	
	


//--------------------------------------------------------------------------------------------//
//                                                                                            //
//--------------------------------------------------------------------------------------------//

strofbitv_function_expression!:                            // fsd-- 8.2F    
		<< int argcount=0; >>
		(	pdfn:predefined_strofbitv_function	<< #0=#pdfn; >>	)?
		|
		(	fn:strofbitv_function
			args:function_arguments[argcount,#fn]
				<<
					#0=#(new ExecuteFunctionAction,#fn,#args);
					((ActionAST *)#0)->setInteger(0,argcount);
				>>
		)
	;


//--------------------------------------------------------------------------------------------//
//                                                                                            //
//--------------------------------------------------------------------------------------------//

bitv_function_expression!:                            // fsd-- 8.2F    
		<< int argcount=0; >>
		(	pdfn:predefined_bitv_function	<< #0=#pdfn; >>	)?
		|
		(	fn:bitv_function
			args:function_arguments[argcount,#fn]
				<<
					#0=#(new ExecuteFunctionAction,#fn,#args);
					((ActionAST *)#0)->setInteger(0,argcount);
				>>
		)
	;


//--------------------------------------------------------------------------------------------//
//                                                                                            //
//--------------------------------------------------------------------------------------------//

strofcharv_function_expression!:                            // fsd-- 8.2F    
		<< int argcount=0; >>
		(	pdfn:predefined_strofcharv_function	<< #0=#pdfn; >>	)?
		|
		(	fn:strofcharv_function
			args:function_arguments[argcount,#fn]
				<<
					#0=#(new ExecuteFunctionAction,#fn,#args);
					((ActionAST *)#0)->setInteger(0,argcount);
					// ((ActionAST *)#0)->check();
				>>
		)
	;


charv_function_expression!:                            // fsd-- 8.2F    
		<< int argcount=0; >>
		(	pdfn:predefined_charv_function	<< #0=#pdfn; >>	)?
		|
		(	fn:charv_function
			args:function_arguments[argcount,#fn]
				<<
					#0=#(new ExecuteFunctionAction,#fn,#args);
					((ActionAST *)#0)->setInteger(0,argcount);
					// ((ActionAST *)#0)->check();
				>>
		)
	;
  
function_arguments![int &argcount,AST * fn]:
	<< int lnum=0; >>
	{
		lp:LP << lnum=$lp->getLine(); >>
			afa:actual_function_arguments[argcount,fn,lnum]
		RP
		<< #0=#afa; >>
	}
	;


actual_function_arguments![int &argcount,AST * fn,int lnum]:
	( expr1:expression
		<<
			argcount++;
			ArgCheck(argcount,fn,#expr1,lnum);
			#0=#(0,#expr1); // Build a list e1
		>>
		(
			dd:Fd exprn:expression
				<<
					argcount++;
					int l=$dd->getLine();// we know that "," is a token
					ArgCheck(argcount,fn,#exprn,l);
					#0=#(0,#0,#exprn); // e1,e2,e3,...
				>>
		)*
	)
	;


	
	
unordered_comparison_operator!:
		eqcomp:EQcomp
			<<
				#0=#(new EQOperator($eqcomp));
			>>
	|	necomp:NEcomp
			<<
				#0=#(new NEOperator($necomp));
			>>
	;

ordered_comparison_operator!:
		gtcomp:GTcomp
			<<
			#0=#(new GTOperator($gtcomp));
			>>
	| 	ltcomp:LTcomp
			<<
				#0=#(new LTOperator($ltcomp));
			>>
	|	gecomp:GEcomp
		<<
			#0=#(new GEOperator($gecomp));
		>>
	|	lecomp:LEcomp
		<<
			#0=#(new LEOperator($lecomp));
		>>
	;

comparison_operator:                          // fsd-- 8.2-3
		unordered_comparison_operator
	|	ordered_comparison_operator
	;

       
boolean_evaluation:                             // fsd-- 8.2H
	<<
		ModifierEntry * modifierEntry;
		DimensionEntry * dimensionEntry;
	>>
	numeric_data_store
	{
		NOM real_quantity[modifierEntry,dimensionEntry]
	}     
	UL real_quantity[modifierEntry,dimensionEntry]
	LL real_quantity[modifierEntry,dimensionEntry]
	;

///////////////////////////////////////////////////////////////////
// Debugger support.....
debug_evaluate!:	// Debugger support
	(
		(dex:expression			<< #0=#dex;>>	)?
	|	(dae:assignment_expression	<< #0=#dae;>>	)?
	|	()
	)	Eof
	;


}
