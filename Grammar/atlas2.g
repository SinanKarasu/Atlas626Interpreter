class AtlasParser {

// 18.6.8 PROCEDURAL STATEMENTS-SIGNAL

procedural_statements_signal:                    // fsd-- 11.1 
		single_action_verb_statements
	|	apply_statement
	|	remove_statement
	|	measure_statement
	|	monitor_statement
	|	verify_statement
	//	|	read_statement
	|	read_Statements
	|	initiate_statement
	|	stimulate_statement
	|	sense_statement
	|	prove_statement 
	; 

read_Statements:	// this fixes the ambiguities....
		(read_timer_statement)?
	|	read_statement
	;


single_action_verb_statements:                   // fsd-- 11.2
	(
		setup_statement
	|	connect_statement
	|	disconnect_statement
	|	arm_statement
	|	fetch_statement
	|	change_statement
	|	enable_event_statement
	|	disable_event_statement
	|	reset_statement
	) 
	;

setup_statement!:					// fsd-- 11.2.2
	verb:fstatnoSETUPfd
	(
			source : single_action_body_source	<< #0 = #source; >>
		|	sensor : single_action_body_sensor	<< #0 = #sensor; >>
		|	event  : single_action_event		<< #0 = #event ; >>
	)
	<<				
		#0 = #( #verb, #( new SetupVerb(), #0 ) );
	>>
	
	statement_terminator
	;
//  &*    This path is only required for signal based events which
//  &     are not directly referenced in SYNC, GATE, FROM, TO or BY
//  &     fields.  It configures the EVENT MONITOR defined in the
//  &     REQUIRE to detect the <event> defined in the IDENTIFY
//  &     statement but does not ENABLE it.*
   
gate_conn_or_event_field![NounEntry * nounEntry]>[AST * mt]:			// fsd-- 11.2.2A
		Fd ef:event_field>[$mt]	<< #0 = #( new GateConnEventAST ,#ef ,#0 ); >>
	|	{ Fd gf:gate_field	<< #0=#gf; >>	} 
		{ Fd mt:max_time	<< $mt=#mt; #0=#(0,#(new MaxTime,#mt),#0);>> } 
		Fd c:conn[nounEntry]	<< #0 = #( new GateConnEventAST ,#c ,#0 ); >>
	;

single_action_body_source!:				// fsd-- 11.2.2B
	<<
		NounEntry	* nounEntry=0;
		AST * re=0;
		AST * mt=0;
		AST * wf=0;
	>>
		nf:noun_field[nounEntry,re]>[nounEntry,re,wf]
		sc:statement_characteristics[nounEntry,re]
		gc:gate_conn_or_event_field[nounEntry]>[mt]
			<<	
				AST * us=new UseSource(re);
				#0 = #( us, #nf, #sc , #gc,wf);
				if(sane()&&!(us->check())){
					Error_Report(" STATEMENT is not covered",VerbLineNo);
				}
			>>
	;


measured_characteristics_with_noun_field![int mult]>[NounEntry * nounEntry, AST * re,AST * ds, AST * wf]:
	<<
		ModifierEntry	* modifierEntry=0;
		NounEntry	* nounEntry=0;
		ASTList		* mc_modifierList=0;
		AST * re=0;
		AST * wf=0;
		AST * ds=0;
		$ds=0;
	>>
	// OK , here is the problem. In SENSOR statements, we do not know the NOUN
	// until the noun_field is consumed. However we do need the NOUN for the
	// ModifierDictionary and the Dimension entries. It is pretty hard to
	// guess the modifier and then the dimension. So we use a trick.
	// We attemp to parse everything in guess mode. This creates another
	// problem for us. Namely if the guess fails, then no meaningful error
	// messages are printed out. So we duplicate the SENSOR stuff without
	// the actions and an extra token ( NoSuchToken ) appended in the end.
	// Now we know that the first rule will NEVER be satisfied. However
	// During the execution (NounEntry * nounEntry) will retrieve the
	// NounEntry field. Then we parse the statement, but now we know what
	// the NOUN is....
	(
		LP measured_characteristic[nounEntry]>[modifierEntry ,ds ]
		(
			Fd measured_characteristic[nounEntry]>[modifierEntry,ds]
		)* RP Fd 
		noun_field[nounEntry,re]>[nounEntry,re,wf] NoSuchToken
		<< if(wf)$wf=wf; >>  // sik verify this. This was a bug hidden for over 25 years.
	)?
	|
	(
		lp:LP
			<<
				if(nounEntry==0){
					Error_Report("A Legal NOUN is NOT found in this statement ",$lp);
				}
			>>
			mc1:measured_characteristic[nounEntry]>[modifierEntry ,ds ]
		<<
			mc_modifierList=new ASTList;
			mc_modifierList->insert( #mc1 );
			if(ds)$ds=ds;
		>>
		(
			f:Fd mcn:measured_characteristic[nounEntry]>[modifierEntry,ds]
				<<
					if(mult){
						mc_modifierList->insert( #mcn );
					} else {
						Error_Report("Single characteristic only",$f);
					}
					if(($ds==lMEAS) || (ds==lMEAS) ){
						Error_Report("Multiple INTO fields must define data-store",$f);
					}
						
				>>
		)* RP Fd 
		nf1:noun_field[nounEntry,re]>[nounEntry,re,wf]
		<<
			// Measured Characteristics go below the Noun..
			#0=#(#nf1, new MeasuredCharacteristicsType( mc_modifierList ) );
			#nf1->init();
			$nounEntry=nounEntry;$re=re;
		>>
	)
	|
	LP RP Fd
		nf2:noun_field[nounEntry,re]>[nounEntry,re,wf]
		<<
			// Measured Characteristics go below the Noun..
			// however we gotta fix the ModifierEntry now
			#0=#(#nf2, new ModifierListType( mc_modifierList ) );
			//#nf2->init();
			$nounEntry=nounEntry;$re=re;
		>>
	
	
	;

single_action_body_sensor!:				// fsd-- 11.2.2B 
	<<
		ModifierEntry	* modifierEntry=0;
		NounEntry	* nounEntry=0;
		AST * re=0;
		AST * ds=0;
		AST * mt=0;
		AST * wf=0;
	>>
	mcwnf:measured_characteristics_with_noun_field[1]>[nounEntry,re,ds,wf]
		sc:statement_characteristics[nounEntry,re]
		gc:gate_conn_or_event_field[nounEntry]>[mt]
			<<
				AST * us=new UseSensor(re);
				#0 =#( us,  #mcwnf, #sc, #gc ,wf);
				if(sane()&&!(us->check())){
					Error_Report(" STATEMENT is not covered",VerbLineNo);
				}
			>>
	;

event_field!>[AST * mt]:					// fsd-- 11.2.2C
	(
			FROM	fe:event TO te:event	<< #0 = #( new EventField, #fe, #te ); >>
		|	OF	ei:event_interval 	<< #0 = #( new EventField, #ei); >>
	)       
	{ Fd gf:gate_field } Fd mt:max_time		<< #0 = #(0, #0, #(new MaxTime,#mt),#gf ); $mt=#mt; >>
	;

single_action_event!:                            // fsd-- 11.2.2-1
	EVENT  
	( ev1:event << #0=#ev1; >> ( Fd evn:event << #0=#(0,#0,#evn); >> )* )
	;

connect_statement!:                              // fsd-- 11.2.3
	verb:fstatnoCONNECTfd 
	( 
			source : single_action_body_source	<< #0 = #source; >>
		|	sensor : single_action_body_sensor	<< #0 = #sensor; >>
		|	event  : single_action_event		<< #0 = #event ; >>	//<<Esc_1 >>
	)
	<<				
		#0 = #( #verb, #( new ConnectVerb(), #0 ) );
	>>
	statement_terminator
	;
//  &*    This path is only required for signal based events which
//  &     are not directly referenced in SYNC, GATE, FROM, TO or BY
//  &     fields.  It configures the EVENT MONITOR defined in the
//  &     REQUIRE to detect the <event> defined in the IDENTIFY
//  &     statement but does not ENABLE it.*
  
  
disconnect_statement!:                           // fsd-- 11.2.4
	verb:fstatnoDISCONNECTfd 
	( 
			source : single_action_body_source	<< #0 = #source; >>
		|	sensor : single_action_body_sensor	<< #0 = #sensor; >>
		|	event  : single_action_event		<< #0 = #event ; >>	//<<Esc_1 >>
	)
	<<				
		#0 = #( #verb, #( new DisconnectVerb(), #0 ) );
	>>
	statement_terminator
	;

//  &*    This path is only required for signal based events which
//  &     are not directly referenced in SYNC, GATE, FROM, TO or BY
//  &     fields.  It configures the EVENT MONITOR defined in the
//  &     REQUIRE to detect the <event> defined in the IDENTIFY
//  &     statement but does not ENABLE it.*

arm_statement!:                                  // fsd-- 11.2.5
	verb:fstatnoARMfd sensor:single_action_body_sensor
	<<				
		#0 = #( #verb, #( new ArmVerb(), #sensor ) );
	>>
	statement_terminator
	;

fetch_statement!:                                // fsd-- 11.2.6
	verb:fstatnoFETCHfd sensor:single_action_body_sensor
	<<				
		#0 = #( #verb, #( new FetchVerb(), #sensor ) );
	>>
	statement_terminator 
	;

change_statement!:                               // fsd-- 11.2.7
	verb:fstatnoCHANGEfd
	(
			source : single_action_body_source	<< #0 = #source; >>
		|	sensor : single_action_body_sensor	<< #0 = #sensor; >>
		|	event  : single_action_event		<< #0 = #event ; >>
	)
	<<				
		#0 = #( #verb, #( new ChangeVerb(), #0 ) );
	>>
	statement_terminator 
	;

//  &*    This path is only required for signal based events which
//  &     are not directly referenced in SYNC, GATE, FROM, TO or BY
//  &     fields.  It configures the EVENT MONITOR defined in the
//  &     REQUIRE to detect the <event> defined in the IDENTIFY
//  &     statement but does not ENABLE it.*

enable_event_statement!:                         // fsd-- 11.2.8
	verb:fstatnoENABLEfd
	sae:single_action_event
	<< #0= #(#verb,#(new EnableEventVerb,#sae)); >>
	statement_terminator 
	;       

disable_event_statement!:                        // fsd-- 11.2.9       
	verb:fstatnoDISABLEfd
	sae:single_action_event
	<< #0= #(#verb,#(new DisableEventVerb,#sae)); >>
	statement_terminator 
	;       

reset_statement!:                                // fsd-- 11.2.10       
	verb:fstatnoRESET 
	( 
		Fd
		(	source : single_action_body_source	<< #0 = #source; >>
		|	sensor : single_action_body_sensor	<< #0 = #sensor; >>
		)
	|	event  : single_action_event	<< #0 = #event ; >>	//<<Esc_1 >>
	)
	<<				
		#0 = #( #verb, #( new ResetVerb(), #0 ) );
	>>
	statement_terminator
	;

//  &*    This path is only required for signal based events which   
//  &     are not directly referenced in SYNC, GATE, FROM, TO or BY   
//  &     fields.  It configures the EVENT MONITOR defined in the   
//  &     REQUIRE to detect the <event> defined in the IDENTIFY    
//  &     statement but does not ENABLE it.*

//reset_signal_source!:     // fsd-- 11.2.10A
//	<<
//		NounEntry     *     nounEntry=0;
//		AST * re=0;
//	>>
//	Fd nf:noun_field[nounEntry,re]>[nounEntry,re]
//		sc:statement_characteristics[nounEntry,re]
//		gc:gate_conn_or_event_field
//		<< #0=#(0,#nf,#sc,#gc); >>
//	;

//reset_signal_sensor!:      // fsd-- 11.2.10A
//	<<
//		NounEntry     * nounEntry=0;
//		ModifierEntry * modifierEntry=0;
//		AST * re=0;
//	>>
//	Fd
//	sensor : single_action_body_sensor	<< #0 = #sensor; >>
//	;

apply_statement!:				// fsd-- 11.3
	verb:fstatnoAPPLYfd
	( 
			a:analog_signal			<< #0 = #a; >>
		|	d:drawing  {Fd c:conn[0]}	<< #0 = #(0,#d,#c); >>
		|	s:single_action_event			<< #0 = #s; >>		//<<Esc_1 >>
	)
	<<				
		#0 = #( #verb, #( new ApplyVerb(), #0 ) );
	>>
	statement_terminator 
	;
//  &*    This path is only required for signal based events which
//  &     are not directly referenced in SYNC, GATE, FROM, TO or BY   
//  &     fields.  It configures the EVENT MONITOR defined in the   
//  &     REQUIRE to detect the <event> defined in the IDENTIFY
//  &     statement but does not ENABLE it.*  

analog_signal!:					// fsd-- 11.3A
	<<
		NounEntry * nounEntry=0;
		AST * re=0;
		AST * wf=0;
	>>
		nf:noun_field[nounEntry,re]>[nounEntry,re,wf]
		sc:statement_characteristics[nounEntry,re]
		{ Fd gf:gate_field } 
		Fd cf:conn[nounEntry]
			<<	
				AST * us=new UseSource(re);
				#0 = #( us, #nf, #sc , #cf, #gf,wf);
				if(sane()&&!(us->check())){
					Error_Report(" STATEMENT is not covered",VerbLineNo);
				}
			>>
	;

remove_statement!:                               // fsd-- 11.4       
	<<
		NounEntry * nounEntry;
		AST * re=0;
		AST * wf=0;
	>>
	verb:fstatnoREMOVEfd  
	( 
			ALL				<< #0= #(new RemoveAction,#()); >>
		|	dr:drawing  {Fd  cnd:conn[0]}	<< #0= #(new RemoveDrawingAction,#dr,#cnd); >>
		|	sa:single_action_event		<< #0= #(new RemoveAction,#sa); >>
		|	(
				nf:noun_field[nounEntry,re]>[nounEntry,re,wf]
				{ Fd cnn:conn[nounEntry] }	//<< Esc_1 >>
			)?
			<<
				AST * ur=new UseResource(re);
				#0= #(new RemoveAction,#(ur,#nf,#cnn));
				if(sane()&&!(ur->check())){
					Error_Report(" STATEMENT is not covered",VerbLineNo);
				}
			>>		 
		|	rs:remove_sensor
			<< #0= #(new RemoveAction,#rs); >>
	)
	<<				
		#0 = #( #verb, #( new RemoveVerb(), #0 ) );
	>>
	statement_terminator 
	;
//  &*    This path is for time measurement statements only.*
//  &**   This path is for sensor statements only.*
//  &***  This path is only required for signal based events which
//  &     are not directly referenced in SYNC, GATE, FROM, TO or BY
//  &     fields.


remove_sensor!:                                  // fsd-- 11.4A
	<<
		NounEntry * nounEntry=0;
		ModifierEntry * modifierEntry=0;
		AST * re=0;
		AST * mt=0;
		AST * wf=0;
	>>
	(	// the following syntactic is needed for nounEntry retrieval
		// DO not take it out....
		(
			LP mc:measured_characteristic_mnemonic[nounEntry]>[modifierEntry]  RP Fd
				nfm:noun_field[nounEntry,re]>[nounEntry,re,wf]	<< #0 = #(#nfm,#mc) ; >>
		)?
	|	nf:noun_field[nounEntry,re]>[nounEntry,re,wf]			<< #0 = #nf  ; >>
	)
	sc:statement_characteristics[nounEntry,re]
	gc:gate_conn_or_event_field[nounEntry]>[mt]
		<<
			AST * ur=new UseSensor(re);
			#0=#( ur, #0,#sc,#gc );
			if(sane()&&!(ur->check())){
				Error_Report(" STATEMENT is not covered",VerbLineNo);
			}
		>>
	;

// The statement_characteristics subfield must be an empty set       
// if the nouns EARTH, COMMON, SHORT are used,       
//  otherwise it must not.       

measure_statement!:    // fsd-- 11.5       
	<<
		NounEntry     * nounEntry=0;
		ModifierEntry * modifierEntry=0;
		AST * re=0;
	>>
	verb:fstatnoMEASUREfd 
	sensor : single_action_body_sensor
	<<				
		#0 = #( #verb, #( new MeasureVerb(), #sensor ) );
	>>
	statement_terminator 
	;

monitor_statement!:                              // fsd-- 11.6
	<<
		NounEntry     * nounEntry=0;
		ModifierEntry * modifierEntry=0;
		AST * re=0;
		AST * ds=0;
		AST * mt=0;
		AST * wf=0;
	>>
	verb:fstatnoMONITORfd       
	mcwnf:measured_characteristics_with_noun_field[0]>[nounEntry,re,ds,wf]
	( 
		sc:statement_characteristics[nounEntry,re] gc:gate_conn_or_event_field[nounEntry]>[mt]
			<<
				AST * us=new UseSensor(re);
				#0 = #( us,  #mcwnf, #sc, #gc );
				if(sane()&&!(us->check())){
					Error_Report(" STATEMENT is not covered",VerbLineNo);
				}
			>>
	|	Fd drawing { Fd conn[nounEntry] } 
			<<
				AST * us=new UseSensor(re);
				#0 = #( us,  #mcwnf, #sc, #gc );
				if(sane()&&!(us->check())){
					Error_Report(" STATEMENT is not covered",VerbLineNo);
				}
			>>
	)       
	<<				
		#0 = #( #verb, #( new MonitorVerb(), #0 ) );
	>>
	statement_terminator 
	;

verify_statement!:                               // fsd-- 11.7       
	<<
		NounEntry     * nounEntry=0;
		ModifierEntry * modifierEntry=0;
		AST * re=0;AST * ds=0;AST * ef=0;
		AST * mt=0;
		AST * wf=0;
	>>
	verb:fstatnoVERIFYfd       
	mcwnf:measured_characteristics_with_noun_field[0]>[nounEntry,re,ds,wf]
		sc:eval_statement_characteristics[nounEntry,modifierEntry,re,ds]>[ef]
		gc:gate_conn_or_event_field[nounEntry]>[mt]      
			<<
				AST * us=new UseSensor(re);
				#0 = #( us,  #mcwnf, #sc, #gc );
				if(sane()&&!(us->check())){
					Error_Report(" STATEMENT is not covered",VerbLineNo);
				}
				#0 = #( #verb, #( new VerifyVerb(scope), #0, #(new CompareAction(this),ef ) ) );
			>>
	statement_terminator 
	;

read_statement!:                                 // fsd-- 11.8       
	<<
		NounEntry     * nounEntry=0;
		ModifierEntry * modifierEntry=0;
		AST * re=0;
	>>
	verb:fstatnoREADfd
	sensor : single_action_body_sensor	<< #0 = #sensor; >>
	<<				
		#0 = #( #verb, #( new ReadVerb(), #0 ) );
	>>
	statement_terminator 
	;

initiate_statement!:                             // fsd-- 11.9      
	verb:fstatnoINITIATEfd
	sensor : single_action_body_sensor	<< #0 = #sensor; >>
	<<				
		#0 = #( #verb, #( new InitiateVerb(), #0 ) );
	>>
	statement_terminator 
	;

stimulate_statement:                            // fsd-- 11.10.1       
	verb:fstatnoSTIMULATEfd 
	{ when_field Fd }  
	( 
 		aa_stimulate_statement_1 (Fd aa_stimulate_statement_1 )* |
		expression Fd on_field 
	)       
	statement_terminator 
	;       
  
aa_stimulate_statement_1:
 	( ZERO | ONE | HIZ ) Fd on_field 
	;

sense_statement:                                // fsd-- 11.10.2       
	verb:fstatnoSENSEfd 
	{ when_field Fd }       
	sensed_data       
	statement_terminator 
	;

sensed_data:                                    // fsd-- 11.10.2A
	<< TheType	storage_type;>>    
	LP 
	( 
		function[&storage_type] LP VALUE  ( Fd expression )*   RP
	|	VALUE
	| 	HIZ 
	) 
	INTO data_store RP
	Fd on_field
	;

prove_statement:                                // fsd-- 11.11.3       
	verb:fstatnoPROVE 
	{ Fd when_field }
	(
		( Fd ( ZERO | ONE | HIZ ) Fd on_field )+
	|
		prove_referenced_signal 
	)       
	statement_terminator 
	;       
      
prove_referenced_signal:                        // fsd-- 11.11.3-A       
	prove_value REF strofbit_expression       
	{ Fd ( MASKdZERO | MASKdONE ) strofbit_expression }
	{ Fd SAVEdCOMP data_store } 
	Fd on_field
	;       
  
  
prove_value:                                    // fsd-- 11.11.3-B       
	<< TheType	storage_type; int argcount=0;int lnum=0;>>
	LP
	(
		VALUE	{INTO data_store}
	|	sobf:strofbitv_function LP VALUE 
			{Fd actual_function_arguments[argcount,#sobf,lnum]}
							RP INTO data_store
	|	HIZ {INTO data_store}
	)
	RP
	;

// 18.6.9 PROCEDURAL STATEMENTS - TIMING       

procedural_statements_timing:                    // fsd-- 12.1
		do_simultaneous_structure
	|	do_timed_digital_structure
	|	do_sequential_structure
	| 	reset_timer_statement
	//	|	read_timer_statement
	//	|	wait_for_statement
	;

wait_for_statement!:			// fsd-- 12.2
	<< DimensionEntry * dimensionEntry=0; AST * t1; >>

	verb:fstatnoWAIT_FORfd  
	(
		wfte:wait_for_timer_or_event>[t1]
				{
					Fd THEN r:RESET t2:timer
					<<
						#0=#(new ResetTimerVerb,#t2);
						
						if( t1 && ( t1->data() != #t2->data() ) ){	// partial evaluation
							Error_Report("TIMERS must be the same ",$r);
						}
					>>
				}
				<< #0 = #( #verb, #wfte,#0); >>
	|	TIME tq2:time_quantity>[dimensionEntry]
		<<
			#0 = #( #verb,#(new WaitForVerb,#(new OneShotTimerType,#tq2)));
		>>
	)
	statement_terminator
	;

wait_for_timer_or_event!>[AST * t]:                        // fsd-- 12.2A
	<< DimensionEntry * dimensionEntry; >>
	
		TIMER t:timer tq:time_quantity>[dimensionEntry] {re:real_errlim[dimensionEntry]}
			<< #( new WaitForTimerAction,#t, #tq, #re ) ; $t=#t;  >>
	|	EVENT e:event  mt:max_time
			<< #0 = #(#mt,new WaitForEventAction, #e); >>
	;

do_simultaneous_structure!:                      // fsd-- 12.4.1
		<< AST	*t, *maxt; >>
	
	a:do_simultaneous_statement>[t, maxt]
	b:do_simultaneous_body
	c:end_do_statement
		<<
			if ( t )
				#0 = #( new DoSimulStructureType, #a, #b, #( #c, t, maxt ) );
			else
				#0 = #( new DoSimulStructureType, #a, #b, #( #c, maxt ) );
		>>
	;        

do_simultaneous_statement!>[AST *t, AST *maxt]:                      // fsd-- 12.4.2       
	<<
		DimensionEntry * dimensionEntry=0;
		$t = 0; $maxt = 0;
	>>
	
	verb:fstatnoDOfd SIMULTANEOUS
		<<	#0 = #( new DoSimulStatementType );	>>
		{ Fd wf:when_field				<< #0    = #( #0, #wf ); >> }
		{ Fd WITHIN tq:time_quantity>[dimensionEntry]	<< $t    = (AST*)#tq; >> }
		{ Fd mt:max_time				<< $maxt = (AST*)#mt; >> }       
		statement_terminator
	;

do_simultaneous_body!:                           // fsd-- 12.4.3
	s:do_simultaneous_body_statement	<< #0 = #( new DoSimulBodyType, #s ); >>
	;

do_simultaneous_body_statement!:
	  mp1:main_procedural_statements	<< #0 = #( new DoSimulAction, #mp1 ); >>
	( mps:main_procedural_statements	<< #0 = #( 0, #0, #( new DoSimulAction, #mps ) ); >> )*
	;

end_do_statement!:                               // fsd-- 12.4.4       
	verb:fstatnoENDfd DO  statement_terminator	<< #0 = #( new DoSimulEndDoType ); >>
	;

do_timed_digital_structure:                     // fsd-- 12.4.5
	do_timed_digital_statement
	do_timed_digital_body  
	end_do_statement
	;

do_timed_digital_statement:                     // fsd-- 12.4.6
		
	verb:fstatnoDOfd TIMED DIGITAL       
	{ Fd wf:when_field }
	( 
		( Fd stim_rate   | Fd stim_event ) { Fd sense_delay | Fd sense_event } |
		( Fd sense_event | Fd sense_rate ) 
	)
	statement_terminator 
	;       

do_timed_digital_body:                          // fsd-- 12.4.7       
	(
		stimulate_statement                   |          // fsd-- 11.11.1
		sense_statement                       |          // fsd-- 11.11.2
		prove_statement                       |          // fsd-- 11.11.3
		procedural_statements_data_processing |          // fsd-- 8.1
		do_timed_digital_structure            |          // fsd-- 12.9.5
		do_timed_control_structure            |          // fsd-- 12.9.7-A
		do_timed_simultaneous_structure                  // fsd-- 12.9.7-B
	) + 
	;

do_timed_control_structure:                     // fsd-- 12.4.7A      
		do_timed_for_then_structure
	|	do_timed_while_then_structure
	|	do_timed_conditional_structure
	|	go_to_statement 
	|	resume_while_statement
	|	resume_for_statement
	|	leave_if_statement
	|	leave_while_statement
	|	leave_for_statement 
	;

do_timed_for_then_structure:
	<< AST * ftea;>>
	for_then_statement>[ftea]
	do_timed_digital_body
	end_for_statement
	;  

do_timed_while_then_structure:
	<< AST * wtea;>>
	while_then_statement>[wtea]
	do_timed_digital_body
	end_while_statement
	;

do_timed_simultaneous_structure!:               // fsd-- 12.4.7B
		<< AST	*t, *maxt; >>
	
	a:do_simultaneous_statement>[t, maxt]
	( 
		( sense_statement | prove_statement )+ |
		( stimulate_statement)+ 
	)
	end_do_statement 
	;

do_timed_conditional_structure:                 // fsd-- 12.4.7C       
	<< AST * itv;>>
	if_then_statement>[itv]
	do_timed_digital_body       
	(
		else_if_then_statement
		do_timed_digital_body 
	)*
	{ else_statement do_timed_digital_body }       
	end_if_statement 
	;

do_sequential_structure:                        // fsd-- 12.4.8
	do_sequential_statement
	do_sequential_body
	end_do_statement 
	;

do_sequential_statement:                        // fsd-- 12.4.9
	verb:fstatnoDOfd SEQUENTIAL
	statement_terminator 
	;

do_sequential_body:                             // fsd-- 12.4.10
	(main_procedural_statements)+
	;

reset_timer_statement!:                          // fsd-- 12.5       
	verb:fstatnoRESET Fd TIMER
	(
		t1:timer << #0=#t1; >> ( Fd tn:timer << #0=#(0,#0,#tn); >> )*
		<< #0 = #( #verb, #( new ResetTimerVerb(), #0 ) ); >>
	|
		all:ALL
		<< #0 = #( #verb, #( new ResetTimerVerb(allTimers)) ); >>
	)
	statement_terminator 
	;

read_timer_statement!:                           // fsd-- 12.6 
	verb:fstatnoREADfd LP TIME INTO dds:decimal_data_store RP       
	Fd t:TIMER tm:timer       
	<<				
		#0 = #( #verb, #( new ReadTimerVerb(), #tm,#dds ) );
	>>
	statement_terminator 
	;


// 18.6.9. PROCEDURAL STATEMENTS DATABUS

procedural_statements_databus:                   // fsd-- 13.0
		do_exchange_statement
	|  	update_exchange_statement
	|	enable_exchange_configuration_statement
	| 	connect_exchange_configuration_statement
	| 	disconnect_exchange_configuration_statement
	|	disable_exchange_configuration_statement
	| 	fetch_exchange_configuration_statement  
	;

do_exchange_statement!:					// fsd-- 13.1
	verb:fstatnoDOfd EXCHANGE USING ec:exchange_configuration
	{ Fd wf:when_field } 
	{ Fd rf:role_field }
	es:exchange_set[#wf,#rf]
	<<
		AST * uec=new UseExchangeConfiguration(#ec);
		#0 =	#(#verb,#(new DoExchangeVerb,#(uec,#es,#rf),#wf));
		if(sane() && !(uec->check())){
			Error_Report(" EXCHANGE CONFIGURATION is not covered",VerbLineNo);
		}
	>>
	statement_terminator 
	;

exchange_set![AST * wf,AST * rf]:			// fsd-- 13.1-1
	<< AST * task; >>
	( 
		(
			ee:exchange_expression[rf]
		|	ZzzSTART
		)	
			Fd	(	PROCEED
					<<
						task=new StartExchangeAction;
						#(task,#ee);
						#0=new SpawnTaskAction(task);
					>>
					
				|	WAIT
					<<	#0=#(new StartExchangeAction,#ee); >>
				)
	|
		HOLD Fd eeh:exchange_expression[rf]
		<< #0=#(new SetupExchangeAction,#eeh); >>
	) 
	{ Fd mt:max_time << #0=#(#mt,#0);>> }
	;

update_exchange_statement!:                      // fsd-- 13.2 
	verb:fstatnoUPDATEfd  
	(	pu:protocol_update
		<< #0= #pu;>>
	|	eu:exchange_update
		<< #0= #eu;>>
	)
	<< #0 = #( #verb, #( new UpdateExchangeVerb,#0) ); >>
	statement_terminator 
	;

protocol_update!:                                // fsd-- 13.2A  
	p:protocol Fd PROTOCOL USING  
	ec:exchange_configuration
	{ wf:when_field }  
	( 
		{ Fd bp:bus_parameter << #0=#bp;>> }
		{ Fd spp:set_protocol_parameter << #0=#0?#(0,#0,#spp):#spp ; >> }
	|
		Fd STANDARD 
	)
	<< #0=#(#wf,#(new ProtocolUpdateAction(#p),#p,#ec,#0)); >>	// wf could be 0.It is OK.
	;

exchange_update!:                                // fsd-- 13.2B
	e:exchange Fd EXCHANGE USING
	ec:exchange_configuration
	{ wf:when_field }
	ned:new_exchange_details[#ec]
	<< #0=#(#wf,#(new ExchangeUpdateAction(e),#e,#ec,#ned)); >>	// wf could be 0.It is OK.
	;
  
  
new_exchange_details![AST * ec]:                           // fsd-- 13.2C
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
	e_c:exchange_contents[ec]
	<< #0=#(	new ExchangeDetails
			,#(new ExchangeDevices,new Talker(talkers),new Listener(listeners) )
			,#(new ExchangeContents,#e_c));
	>>
	;

exchange_contents![AST * ec]:                              // fsd-- 13.2D
	{ Fd c:COMMAND	dudc:databus_update_data[ec]	<< #0=#(0,#(new DataBusUpdateAction($c,ec),#dudc),#0); >> }
	{ Fd d:DATA	dudd:databus_update_data[ec]	<< #0=#(0,#(new DataBusUpdateAction($d,ec),#dudd),#0); >> } 
	{ Fd s:STATUS	duds:databus_update_data[ec]	<< #0=#(0,#(new DataBusUpdateAction($s,ec),#duds),#0); >> }
	{ Fd 		bp:bus_parameter		<< #0=#(0,#(new ParameterUpdateAction(ec) ,#bp  ),#0); >> }
	{ Fd 		pp:set_protocol_parameter	<< #0=#(0,#(new ProtocolUpdateAction(ec)  ,#bp  ),#0); >> }
	;

databus_update_data![AST * ec]:                            // fsd-- 13.2E
	{entire:ENTIRE }
	(
		FROM f:file << #0=#f; >>
		{pos:integer_expression <<#0 = #( new FilePositionAction, #0, #pos );>> }
			<<
			
				if($entire==0){
					#0 = #( new FileDataInputAction, 
						#(new EntireData(ec),#(new DataInputAction,new DataPump(ec))), #0 );
				} else {
					#0 = #( new FileDataInputAction, 
						#(new DataInputAction,new DataPump(ec)), #0 );
				}
			>>
	|	(	ar:array_range
			<<
				if($entire!=0){
					#0=#(new EntireData(ec),#ar);
				} else {
					#0=#(new DataPump(ec),#(new ArrayIteratorAction,#ar));
				}
			>>
		)?
	|	(ds:data_store	<< #0=#ds; >>)?
	|	c:constant	<< #0=#c;  >>
	)
	;

enable_exchange_configuration_statement!:        // fsd-- 13.3 
	<< ASTList protocolList;>>
	verb:fstatnoENABLEfd EXCHANGE CONFIGURATION  
	USING ec:exchange_configuration Fd
	eec:exchange_protocol_initialization[#ec,protocolList]
	<<
		AST * uec=new UseExchangeConfiguration(#ec);
		#0 = #(#verb,#(new EnableExchangeConfigurationVerb,#(uec,#eec)));
		//if(sane() && !(uec->check())){
		//	Error_Report(" EXCHANGE CONFIGURATION is not covered",VerbLineNo);
		//}
	>>
	statement_terminator
	;  

exchange_protocol_initialization![AST * ec,ASTList & protocolList]:
	PROTOCOL p:protocol  pi:protocol_initialization[ec]
		<<
			AST * x=0;
			if(protocolList.findValue(#p->getName(),x)){
				Error_Report("Multiple occurence of same protocol:",#p);
			} else {
				AST * pr=0;
				if(sane()&&!(pr=ec->data(#p))){
					Error_Report("EXCHANGE CONFIGURATION does not cover PROTOCOL",#p);
				}
				if((pr)&&!(pr->check(#pi->data()))){
					Error_Report("PROTOCOL does not cover PROTOCOL INITIALIZATION",#p);
				}
			}
			protocolList.insert(#p);
		>>
		{
			Fd epi:exchange_protocol_initialization[ec,protocolList]
		}
	<< #0=#(0,#(new ExchangeProtocolInitializationAction,#p,#pi),#epi); >>
	; 
 
protocol_initialization![AST * ec]:                        // fsd-- 13.3A  
	{ Fd INITIALIZE ri:resource_initialization[ec] }
	{ Fd bp:bus_parameter } 
	{ Fd spp:set_protocol_parameter }
	{ Fd CNX cnx:conn_set["",0] }
	<< #0=#(
		new ProtocolInitializationAction,
			#(#ri),
			#(new Proxy,#bp),
			#(new Proxy,#spp),
			#(new Proxy,#cnx)
		);
	>>
	;
  
resource_initialization![AST * ec]:		// fsd-- 13.3B
	<< ASTList * talkers=0;ASTList * listeners=0; >>
	{	Fd TALKER
			<< talkers=new ASTList; >>
			ddt:	databus_device
			<< talkers->append(#ddt); >>
	}
	{	Fd LISTENER
			<<listeners=new ASTList;>>
			ddl:	databus_device
			<< listeners->append(#ddl); >>
	}
	{	Fd c:COMMAND	dedc:	database_enable_data[ec]
			<< #0=#(new DataBusData($c,ec),#dedc); >>
	}
	{	Fd d:DATA	dedd:	database_enable_data[ec]	<< #0=#(new DataBusData($d,ec),#dedd); >>
	}
		<<
			#0=	#(	new DataBusDevices,new Talker(talkers),	new Listener(listeners)
					,#(new DataBusData($c,ec),#dedc),#(new DataBusData($d,ec),#dedd)
				);
		>>
	;
   
   
database_enable_data[AST * ec]:			// fsd-- 13.3C
	(
		(array_range)?
	|	(data_store)?
	|	constant
	)
	;
	 
 
connect_exchange_configuration_statement!:       // fsd-- 13.4
	verb:fstatnoCONNECTfd EXCHANGE CONFIGURATION USING ec:exchange_configuration Fd
	PROTOCOL p1:protocol Fd CNX cs1:conn_set["",0] 
		<<
			AST * up1=new UseProtocol(#p1);
			#0=#(up1,#cs1);
			if(sane() && !(up1->check())){
				Error_Report(" PROTOCOL is not covered",VerbLineNo);
			}
		>>
		(
			Fd PROTOCOL pn:protocol Fd CNX csn:conn_set["",0]
			<<
				AST * upn=new UseProtocol(#pn);
				#0=#(0,#0,#(upn,#csn));
				if(sane() && !(upn->check())){
					Error_Report(" PROTOCOL is not covered",VerbLineNo);
				}
			>>
		)*
	<<				
		AST * uec=new UseExchangeConfiguration(#ec);
		#0 =	#(#verb,#(new ConnectVerb,#(uec,#0)));
		if(sane() && !(uec->check())){
			Error_Report(" EXCHANGE CONFIGURATION is not covered",VerbLineNo);
		}
	>>
	statement_terminator 
	;
    
 
disconnect_exchange_configuration_statement!:     // fsd-- 13.5
	verb:fstatnoDISCONNECTfd EXCHANGE CONFIGURATION USING ec:exchange_configuration Fd   
	PROTOCOL p1:protocol Fd CNX cs1:conn_set["",0]
		<<
			AST * up1=new UseProtocol(#p1);
			#0=#(up1,#cs1);
			if(sane() && !(up1->check())){
				Error_Report(" PROTOCOL is not covered",VerbLineNo);
			}
		>>
		(
			Fd PROTOCOL pn:protocol Fd CNX csn:conn_set["",0]
			<<
				AST * upn=new UseProtocol(#pn);
				#0=#(0,#0,#(upn,#csn));
				if(sane() && !(upn->check())){
					Error_Report(" PROTOCOL is not covered",VerbLineNo);
				}
			>>
		)*
	<<				
		AST * uec=new UseExchangeConfiguration(#ec);
		#0 = #( #verb, #( new DisconnectVerb,#(uec,#0)));
		if(sane() && !(uec->check())){
			Error_Report(" EXCHANGE CONFIGURATION is not covered",VerbLineNo);
		}
	>>
	statement_terminator 
	;  
  
 
disable_exchange_configuration_statement!:       // fsd-- 13.6  
	verb:fstatnoDISABLEfd EXCHANGE CONFIGURATION USING ec:exchange_configuration  
	<<
		AST * uec=new UseExchangeConfiguration(#ec);
		#0 = #(#verb,#(new DisableExchangeConfigurationVerb,uec));
		if(sane() && !(uec->check())){
			Error_Report(" EXCHANGE CONFIGURATION is not covered",VerbLineNo);
		}
	>>
	statement_terminator 
	;  
  
  
fetch_exchange_configuration_statement!:         // fsd-- 13.7
	verb:fstatnoFETCHfd
	(
		pf:protocol_fetch	<< #0=#pf; >>
	|	ef:exchange_fetch	<< #0=#ef; >>
	)
	<<				
		#0 = #( #verb, #( new FetchExchangeVerb, #0 ) );
	>>
	statement_terminator 
	;
  
  
protocol_fetch!:                                 // fsd-- 13.7A
	p:protocol Fd PROTOCOL USING ec:exchange_configuration
	<< #0=#p; >>
	{ wf:when_field } 
	{ Fd bp:bus_parameter << #0= #(0,#0,#(new FetchDataBusAction,#bp)); >> }
	{ Fd fpp:fetch_protocol_parameter << #0= #(0,#0,#(new FetchDataBusAction,#fpp)); >> }
	<< #0=#(#wf,#0); >>
	;
  
  
exchange_fetch!:                                 // fsd-- 13.7B
	e:exchange Fd EXCHANGE USING ec:exchange_configuration
	{ wf:when_field }
	fed:fetched_exchange_data
	{ Fd mt:max_time }
	<< #0=#(#wf,(#mt,#fed)); >>
	;
  
  
fetched_exchange_data!:                         // fsd-- 13.7C
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
  	fi:fetched_information
	<< #0=#(new ExchangeDevices , new Talker(talkers),new Listener(listeners) ,#fi); >>
  	;
  
  
fetched_information:                           // fsd-- 13.7D
	{ Fd COMMAND databus_fetch_data }
	{ Fd DATA  databus_fetch_data  }  
	{ Fd STATUS databus_fetch_data }
	{ Fd bus_parameter }
	{ Fd fetch_protocol_parameter }
	;
  
  
databus_fetch_data:                            // fsd-- 13.7E
		(to_count_into)?
	|	data_store
	;

to_count_into:                                 // fsd-- 13.7F
	(
		TO file { integer_expression }
	|	array_range
	)
	{ Fd COUNTdINTO integer_data_store }
	;
  
// 18.6.11  FIELDS / SUBFIELDS
  
  
statement_characteristics![NounEntry * nounEntry,AST * re]:   // fsd-- 14.1 //<<Esc_2 >>

	<<
		ModifierEntry	* modifierEntry=0;
		ASTList		* modifierList=new ASTList;
	>>
	(	<<nounAllowsDescriptors(nounEntry)>>?
		(
			fd:Fd
			(
				cm:characteristic_mnemonic[nounEntry]>[modifierEntry]
				(
					(<<isType(LT(1),modifierEntry,"MO")>>? ())?
					// This path is used only for modifiers of type MO
					// as defined in Section 16.
					()
				|
					cv:characteristic_value[modifierEntry,re,#cm]
				)
				<<				
					#( #cm, #cv );
					modifierList->insert( #cm );
				>>
					
			|	ss:sync_subfield[nounEntry]>[modifierEntry]
				<<
					AST * sc=new StatementCharacteristic( modifierEntry);
					#(sc,#ss);
					modifierList->insert( sc);
				>>
			)
  		)+
  	|	// This path is used only for the nouns COMMON,EARTH and SHORT
  	)
  	
	<< #0 = #(new ModifierListType( modifierList ) ); >>
	;

characteristic_value[ModifierEntry * &modifierEntry,AST *& re,AST *& cm]:                           // fsd-- 14.1-B
		(
			<<isType(LT(1),modifierEntry,"MD")>>?
			md:modifier_descriptor[modifierEntry]
		)
//	|	ar:array_range	// covered in real_characteristic_subfield
//	|	expression	// covered in real_characteristic_subfield
	|	rs:real_characteristic_subfield[modifierEntry,re,cm]
	;
//  &*      This path is used only for modifiers of type SB
//  &       or SC as defined in Section 16.*
//  &**     This path is used only for modifiers of type AB
//  &       or AC as defined in Section 16.*
//  &***    This path is used only for modifiers of type MD
//  &       as defined in Section 16.*
  
  
real_characteristic_subfield![ModifierEntry * &modifierEntry,AST * re,AST * cm]:                   // fsd-- 14.2
	<<
		DimensionEntry * dimensionEntry=0;ANTLRTokenPtr t;
	>>
	(
		( 
			rn:RANGE	fr:dimensioned_number[modifierEntry,dimensionEntry]
			TO	to:dimensioned_number[modifierEntry,dimensionEntry]
				<<
				#0 = #( new ConstraintRange,#fr, #to );t=$rn;
				>>
		|	mx:MAX	dmx:dimensioned_number[modifierEntry,dimensionEntry]
				<<
					#0 = #( new ConstraintMax, #dmx );t=$mx;
				>>
	
		|	mn:MIN	dmn:dimensioned_number[modifierEntry,dimensionEntry]
				<<
					#0 = #( new ConstraintMin, #dmn );t=$mn;
				>>
		)
		<<
			if(
					cm
				&&	(getModifierType(re,cm)!=CapabilityModifierType)
				&&	(getModifierType(re,cm)!=LimitModifierType)
			){
				Error_Report(
					"Only CAPABILITY/LIMIT types allow:"
					,t
					);
			}			
		>>
	)
	|
	(	sv:signal_value[modifierEntry] { rel:real_errlim[dimensionEntry] }
		<<
			#0=#(0,#sv,#(new RealErrlim,#rel));

			if(getModifierType(re,cm)!=ControlModifierType){
				Error_Report(
					"SIGNAL VALUE can only be specified for CONTROL types:"
					,cm
					);
			}			
		>>
	)
	;
  

sync_subfield[NounEntry * nounEntry]>[ModifierEntry * modifierEntry]:		// fsd-- 14.3
	( 
		SYNC 
		{ sync_point[nounEntry]>[$modifierEntry]}	//<<Esc_1 >>
		TO EVENT event 
	)
	;
//  &*  This path is used with the noun WAVEFORM and with
//  &   other nouns when the sync subfield is used to
//  &   synchronize the ARM action of a sensor statement.  This
//  &   path may be used only once in a statement.*
  
  
sync_point[NounEntry * nounEntry]>[ModifierEntry * modifierEntry]:       // fsd-- 14.3A
	( 
		sync_initial>[$modifierEntry]
	|	(sync_parameter>[$modifierEntry])?
	|	sync_point_value[nounEntry]>[$modifierEntry]
	)
	;

sync_point_value![NounEntry * nounEntry]>[ModifierEntry * modifierEntry]:
		<< ModifierEntry * modifierEntry=0; >>
		characteristic_mnemonic[nounEntry]>[modifierEntry]
		{ 
			evaluation_field[modifierEntry,0]  pos_neg_edge 
		}
	;

sync_initial!>[ModifierEntry * modifierEntry]:				// fsd-- 14.3B
	<<
		RWCString left;
		DimensionEntry * dimensionEntry=0;
	>>
	INITIAL 
	(	<<$modifierEntry=mnemonicsDB->theModifierEntry(LT(1)->getText(),"~SYNC-INITIAL",left);>>
		m:ModifierCLASS
		<<
			if(!$modifierEntry || !(left=="")){ Error_Report("BAD SYNC INITIAL field ",$m);}
		>>
	
		(
			(<<isType(LT(1),$modifierEntry,"MO")>>? ())?
			// it does not have value/dimension (e.g. VOLTAGE-P-POS )
		|
			// note that furthermore unsigned_integer_number must be 0....
			unsigned_integer_number dimension[$modifierEntry,dimensionEntry]
			{real_errlim[dimensionEntry] } pos_neg_edge
		)
		//#0 = #( new SyncInitialAction, $m );
	)
	;

pos_neg_edge!:                                   // fsd-- 14.3-1 
	<<
		RWCString left;ModifierEntry * modifierEntry=0;
	>>
	<< modifierEntry=mnemonicsDB->theModifierEntry(LT(1)->getText(),"~POS-NEG-EDGE",left)>>?
			[Error_Report(RWCString("Not a valid EDGE specifier:"),LT(1));]
	m:ModifierCLASS
		<<
			if(!(left=="")){ Error_Report("BAD EDGE specifier ",$m);}
			//#0 = #( new SyncInitialAction, $m );
		>>
	;

real_errlim![DimensionEntry * &dimensionEntry]: // fsd-- 14.4
	ERRLMT 
	( 
		dl:dim_lim[dimensionEntry]
			{
				(
					AND	<< #0=new ANDErrLmt;>>
				|	OR	<< #0=new ORErrLmt;>>
				)  pl:pc_lim
			}
			<<#0=#(#0,#dl,#pl);>>	// Trick alert
						// ( and_or,dl,pc) --  or--
						// ( 0     ,dl,0)  --  ;-)
	|	pcl:pc_lim
		<< #0=#pcl;>>
	)
	;

dim_lim![DimensionEntry * &dimensionEntry]:    // fsd-- 14.5
	( 
		PLUSop hi:unsigned_dimensioned_number[0,dimensionEntry]
			MINUSop lo:unsigned_dimensioned_number[0,dimensionEntry]
			<<#0=#(new ConstraintRange,#(new NegativeOperator,#lo),#hi);>>
	|	PLUSMINUSop udn:unsigned_dimensioned_number[0,dimensionEntry]
		<<#0=#(new ConstraintRange,#(new NegativeOperator,#udn),#udn);>>
	)
	;

pc_lim:                                         // fsd-- 14.6
	( 
		PLUSop pos:unsigned_decimal_number[0] PC
			MINUSop neg:unsigned_decimal_number[0] PC
			<<#0=#(new ConstraintPercent,#neg,#pos);>>
	|	PLUSMINUSop pc:unsigned_decimal_number[0] PC 
			<<#0=#(new ConstraintPercent,#pc,#pc);>>
	)
	;

measured_characteristic![NounEntry *& nounEntry]>[ModifierEntry * modifierEntry,AST * ds]: // fsd-- 14.7       
	<<
		DimensionEntry * dimensionEntry=0;
	>>
	mcm:measured_characteristic_mnemonic[nounEntry]>[$modifierEntry]
	{ re:real_errlim[dimensionEntry] }
	(
		into:INTO target:data_store_or_array_range 
		<<
			#0=#(new MeasuredCharacteristicType($into)	,
				#(#mcm	,$ds=#target)
					,#(new ConstraintMeasurementAction
						,#(new RealErrlim,#re)
					  )
			);
		>>
	|
		<<
			#0=#(new MeasuredCharacteristicType($into)	,
				#(#mcm	,#(new Proxy(),$ds=lMEAS))
					,#(new ConstraintMeasurementAction
				           	,#(new RealErrlim,#re)
					  )
			);
		>>
	)
	;      

data_store_or_array_range:
		(ds:data_store)? | ar:array_range
	;

ordered_evaluation_operator!>[AST * slot]:
		gtcomp:GTcomp
			<<
				#0=#(new LoAction(this),$slot=new GTOperator($gtcomp));
			>>
	| 	ltcomp:LTcomp
			<<
				#0=#(new HiAction(this),$slot=new LTOperator($ltcomp));
			>>
	|	gecomp:GEcomp
		<<
				#0=#(new LoAction(this),$slot=new GEOperator($gecomp));
		>>
	|	lecomp:LEcomp
		<<
				#0=#(new HiAction(this),$slot=new LEOperator($lecomp));
		>>
	;
	
    
evaluation_field![ModifierEntry *& modifierEntry,AST * ds]:                               // fsd-- 14.8       
	<<
		DimensionEntry * dimensionEntry=0;
		AST * slot=0;
	>>
	(
		ull:up_low_limit[dimensionEntry,ds]
		<< #0=#(0,#ull,ds);>>
	|
		(
			oeo:ordered_evaluation_operator>[slot] orq:real_quantity[modifierEntry,dimensionEntry]
				<<
					#(slot,#(new Proxy,ds),#orq);
					#0=#(0,#oeo,#(new Proxy,ds));
				>>
		|	ueo:unordered_comparison_operator
			(
				(urq:real_quantity[modifierEntry,dimensionEntry])?
				<< #0=#(0,#(new NoGoAction(this),#(#ueo,#(new Proxy,ds),#urq)),#(new Proxy,ds));>>
			|	une:numeric_expression
				<< #0=#(0,#(new NoGoAction(this),#(#ueo,#(new Proxy,ds),#une)),#(new Proxy,ds));>>
			)
		)
	)
	;  


up_low_limit![DimensionEntry *& dimensionEntry, AST * ds]:	// fsd-- 14.8A      
	<< ModifierEntry * modifierEntry=0; >>
	
	{ NOM nm:real_quantity[modifierEntry,dimensionEntry]  }
	UL ul:real_quantity[modifierEntry,dimensionEntry]
		LL ll:real_quantity[modifierEntry,dimensionEntry]
	<<
		#0=#(new WITHINOperator,
			#(new HiAction(this),#(new ULOperator,#(new Proxy,ds),#ul)),
			#(new LoAction(this),#(new LLOperator,#(new Proxy,ds),#ll,#nm))
			);
	>>
	;

eval_statement_characteristics![NounEntry * nounEntry,ModifierEntry *& modifierEntry,AST * re,AST * ds]>[AST * ef]:                 // fsd-- 14.9      
	Fd ef:evaluation_field[modifierEntry,ds]
	sc:statement_characteristics[nounEntry,re]
	<<
		#0=#sc;
		$ef=#ef;
	>>
	;
  
    
max_time!:						// fsd-- 14.10      
	<< DimensionEntry * dimensionEntry=0; >>
	MAXdTIME tq:time_quantity>[dimensionEntry]
	//<< #0 = new MaxTimeAction(#tq,lMAXdTIME); >>
	<< #0=#tq; >>
	;

time_quantity!>[DimensionEntry * dimensionEntry]:	// fsd-- 14.11
	ne:numeric_expression  td:time_dim[$dimensionEntry]
		<< #0 = #( new TimeQuantityType(0), #ne, #td );>>
	;

conn![NounEntry * nounEntry]:				// fsd-- 14.13
		CNXdNUMERATOR c1:connect_set_ref["NUMERATOR_",nounEntry]
			CNXdDENOMINATOR c2:connect_set_ref["DENOMINATOR_",nounEntry]
		<<
			#0=#(new CnxField,#(new CnxNumeratorType , #c1),#(new CnxDenominatorType , #c2));
		>>
	|	CNX c:connect_set_ref["",nounEntry]
		<<
			#0=#(new CnxField,#c);
		>>
	;
//  &**  This path is used only with the suffix RATIO.*       

// sik. All Analog type verbs, go thru conn -> connect_set_ref to define CNX fields.
connect_set_ref![const RWCString &  pre,NounEntry *& nounEntry]:	// fsd-- 14.13A      
	cs1:conn_set[pre,nounEntry]	<< #0=#cs1; >>
		{ 
			REF
				(
					rp:reference_phase
					<<#0=#(0,#0,#rp);>>
				|	cs2:conn_set[ pre + "REF_" ,nounEntry ]
					<<#0=#(0,#0,#cs2);>>
				)
		}
	;

conn_set![const RWCString & ref,NounEntry * nounEntry]:			// fsd-- 14.14
	(	<< ASTBase * cds=0; >>
		{
			(pd : pin_descriptor)?
			<<
				if(!nounAllowsDescriptors(nounEntry)){
					Error_Report(" Pin Descriptors are not allowed for:"+nounEntry->noun,#pd);
				}
				cds=0;
			>>
		}
		((cd : conn_descriptor	<<cds=#(0,#(new ConnDescriptor,#cd),cds); >> )?)+
					<<#0=#(0,#(new PinDescriptor(#pd,ref),cds),#0); >>
	)+
	;       
//  &***  This bypass is allowed only for the nouns where a pin       
//  &    description is not meaningful, such as COMMON, EARTH and       
//  &    SHORT*       

conn_descriptor!:                                // fsd-- 14.14A       
	(
		c:connection
		<< #0=#c; >>
	|	cds:connection_data_store
		<< #0=#cds; >>
	)
	|	cda:conn_descriptor_arg
		<< #0=#cda; >>
	;

conn_descriptor_arg:
		LP RP
	;

signal_value_define_signal:
	<<
		ModifierEntry * modifierEntry=0;
		DimensionEntry * dimensionEntry=0;
	>>
	LP RP signal_range[modifierEntry,dimensionEntry]
	;
 
signal_value![ModifierEntry * modifierEntry]:				// fsd-- 14.15
	<<
		DimensionEntry * dimensionEntry=0;
		TheType arrayType;
	>>
	(
		(dn:dimensioned_number[modifierEntry,dimensionEntry] << #0=#dn; >> )?
	|	(de:decimal_expression  sr:signal_range[modifierEntry,dimensionEntry])?
		<< #0=#(new SignalValueType,#de,#sr); >>
	|	(
			ar:array_range
			<<
				arrayType=#ar->eval(#ar)->getType();
				if	(isType(modifierEntry,"RA")&&(arrayType==DecimalNumberValue	)){
				}else if(isType(modifierEntry,"AB")&&(arrayType==BitTypeValue		)){
				}else if(isType(modifierEntry,"AC")&&(arrayType==CharTypeValue		)){
				} else {
					Error_Report("Incompatible Modifier and Array type",#ar);
				}
			>>
			(
				asr:signal_range[modifierEntry,dimensionEntry]
			|	()
				<<
					if(arrayType==DecimalNumberValue||arrayType==IntegerNumberValue){
						Error_Report("signal range expected",#asr);
					}
				>>
			)
			<< #0=#(new SignalValueType,#ar,#asr); >>
		)?
	|	(
			sb:strofbit_expression
			<<
				if(!isType(modifierEntry,"SB")){
					Error_Report(" MODIFIER is not STRING OF BIT type",#sb);
				}
				#0=#(new SignalValueType,#sb);
			>>
		)?
	|	(
			sc:strofchar_expression
			<<
				if(!isType(modifierEntry,"SC")){
					Error_Report(" MODIFIER is not STRING OF CHAR type",#sc);
				}
				#0=#(new SignalValueType,#sc);
			>>
		)
	)
	;
//  &*    This path may only be used to specify the
//  &     requirement for a parameter within a
//  &     <define signal statement>.*

signal_range![ModifierEntry * modifierEntry,DimensionEntry * &dimensionEntry]:    // fsd-- 14.15A
	RANGE fr:dimensioned_number[modifierEntry,dimensionEntry]
	TO to:dimensioned_number[modifierEntry,dimensionEntry]
		// should the following be a RangeType or Constraint Range .. sik 10/31/2000
		<< #0 = #( new ConstraintRange,#fr, #to ); >>

	;

real_quantity![ModifierEntry *& modifierEntry,DimensionEntry * &dimensionEntry]:   // fsd-- 14.16
	ne:numeric_expression d:dim[modifierEntry,dimensionEntry]
		<< #0 = #( new DimensionedNumberType( modifierEntry ), #ne, #d ); >>
	;
    
noun_field![NounEntry * &nounEntry,AST *& re]>[NounEntry * nounEntry, AST * re, AST * wf]:                                    // fsd-- 14.18
	n:noun[nounEntry]>[$nounEntry]  USING req:requirement[re] { wf:when_field << $wf=#wf; >> }
		<< #0 =#(#wf, #n) ; $re=#req; >>
	;
    
noun![NounEntry * & nounEntry]>[NounEntry * nounEntry]:                                           // fsd-- 14.18-1       
		an:analog_noun[nounEntry]>[$nounEntry]
		<< #0=#an; >>
	;
 

dimensioned_number![ModifierEntry * modifierEntry,DimensionEntry * & dimensionEntry]:	// fsd-- 14.19
	dn:decimal_number[modifierEntry] d:dim[modifierEntry,dimensionEntry]
		<<
			#0 = #( new DimensionedNumberType( modifierEntry ), #dn, #d );
		>>
	;

unsigned_dimensioned_number![ModifierEntry * modifierEntry,DimensionEntry * & dimensionEntry]:	// (sik added)
	dn:unsigned_decimal_number[modifierEntry] d:dim[modifierEntry,dimensionEntry]
		<< #0 = #( new DimensionedNumberType( modifierEntry ), #dn, #d ); >>
	;

require_control![NounEntry * nounEntry, ASTList * modifierList]:    // fsd-- 14.20  
	<<
		ModifierEntry * modifierEntry = 0;
		StringSet modifiers;
	>>
	
	Fd CONTROL
	(
		Fd  cm:characteristic_mnemonic[nounEntry]>[modifierEntry]
		<<
			if(modifiers.contains(#cm->getName())){
				Error_Report(" Modifier " + #cm->getName() + " duplicate or invalid/bad-suffix ", #cm);
			}
			modifiers.insert(#cm->getName());
		>>
		
		(
			rcb:require_control_body[modifierEntry]
			<<
				modifierList->insert( (AST*)#(new RequireControlAction, #(#cm, #rcb ) ) );
			>>
			|
			(
				md:modifier_descriptor[modifierEntry]
				<<
					modifierList->insert( (AST*)#(new RequireControlAction, #(#cm, #md ) ) );
				>>
			)+
		)
	)+
	;
    
require_control_body![ModifierEntry * & modifierEntry]:			// fsd-- 14.20A     
	<<
		ASTVector range;
		DimensionEntry * dimensionEntry=0;
	>>
	( 
		rr:require_range[modifierEntry,dimensionEntry]
			<< #0 = #rr; >>
			(
				BY bydn:dimensioned_number[modifierEntry,dimensionEntry]
				<< #0 = #( new ByQuantization, #0, #bydn ); >>
			|	CONTINUOUS
			)
		|
			(
				dn:dimensioned_number[modifierEntry,dimensionEntry]
				<< range.append(#dn); >>
			)+
			<< #0 = #( new ConstraintArray, new ArrayType(range) ); >>

	) 
	{	
		re:real_errlim[dimensionEntry]
		<< #0 = #(0,#0, #(new RealErrlim, #re )); >>
	}
	;
  
  
require_range![ModifierEntry *& modifierEntry,DimensionEntry * & dimensionEntry]:	// fsd-- 14.20-1     

	RANGE	fr:dimensioned_number[modifierEntry,dimensionEntry]
	TO	to:dimensioned_number[modifierEntry,dimensionEntry]
		<<
			#0 = #( new ConstraintRange, #fr, #to );
		>>
	;

require_capability![NounEntry * nounEntry, ASTList * modifierList ]:       // fsd-- 14.21     
	<<
		ModifierEntry  	* modifierEntry  = 0;
		StringSet modifiers;
	>>
	Fd CAPABILITY 
	{ Fd GATE  }     
	( Fd sr:sync_requirement << #0 = #(0,#sr,#0); >> )*
	( Fd
		cm:characteristic_mnemonic[nounEntry]>[modifierEntry]
		<<
			if(modifiers.contains(#cm->getName())){
				Error_Report(" Modifier " + #cm->getName() + " duplicate or invalid/bad-suffix ", #cm);
			}
			modifiers.insert(#cm->getName());
		>>
		(
			<< DimensionEntry 	* dimensionEntry = 0; >>
			(<<isType(LT(1),modifierEntry,"MO")>>? ())?
			// This path is used only for modifiers of type MO
			// as defined in Section 16.
		|	rcc:required_characteristic_capability[modifierEntry,dimensionEntry]
			{
				re:real_errlim[dimensionEntry]
			}
			<<
				modifierList->insert( (AST *)#(new RequireCapabilityAction,#(#cm,#rcc,#( new RealErrlim,#re))) );
			>>
		|	md:modifier_descriptor[modifierEntry]
			<<
				modifierList->insert( (AST *)#(new RequireCapabilityAction,#( #cm, #md )) );
			>>
		)
	)+
	;

sync_requirement!:                               // fsd-- 14.21A     
	<<
		NounEntry * nounEntry=0;
		ModifierEntry  	* modifierEntry  = 0;
	>>
	SYNC  
	( 
	 	si:sync_initial>[modifierEntry]	<< #0 = #si; >>
	|	(sp:sync_parameter>[modifierEntry]	<< #0 = #sp; >>)?
	|	rs:ranged_sync[nounEntry]		<< #0 = #( new SyncRequirementAction, #rs ); >>
	)
	;

sync_parameter!>[ModifierEntry * modifierEntry]:
	<< RWCString left;>>
	() << $modifierEntry=mnemonicsDB->theModifierEntry(LT(1)->getText(),"~SYNC-PARAMETER",left) >>?
	m:ModifierCLASS
		<<
			if(!(left=="")){ Error_Report("BAD SYNC field ",$m);}
			#0 = #( new SyncRequirementAction, new ModifierType($m,$modifierEntry) );
		>>
	;

ranged_sync[NounEntry * nounEntry]:        // fsd-- 14.21B     
	<<
		DimensionEntry * dimensionEntry=0;
		ModifierEntry * modifierEntry=0;
	>>
	characteristic_mnemonic[nounEntry]>[modifierEntry]
	{ 
		required_range[modifierEntry,dimensionEntry]     
		pos_neg_edge
	}
	;

required_range[ModifierEntry *& modifierEntry,DimensionEntry * & dimensionEntry]:                                 // fsd-- 14.21C     
	
	r:RANGE	fr:dimensioned_number[modifierEntry,dimensionEntry]
	t:TO	to:dimensioned_number[modifierEntry,dimensionEntry]
		<<
			#0 = #( new ConstraintRange($r),#fr, #to );
		>>
	{
		b:BY by:dimensioned_number[modifierEntry,dimensionEntry]
		<< #0 = #( new ByQuantization($b), #0, #by ); >>
	}
	;

required_characteristic_capability![ModifierEntry * modifierEntry,DimensionEntry * & dimensionEntry]:             // fsd-- 14.21D     
	(
		rr:require_range[modifierEntry,dimensionEntry]
			<< #0 = #rr; >>
			{
				BY bydn:dimensioned_number[modifierEntry,dimensionEntry]
				<< #0 = #( new ByQuantization, #0, #bydn ); >>
			}
	|	(
			MAX	<< #0=new ConstraintMax;>>
		|	MIN	<< #0=new ConstraintMin;>>
		) dn:dimensioned_number[modifierEntry,dimensionEntry]
			<<
				#0=#(#0,#dn);
			>>
	)
	;
  
require_limit![NounEntry * nounEntry, ASTList * modifierList]:                                  // fsd-- 14.22     
	<<
		ModifierEntry	* modifierEntry=0;
		DimensionEntry	* dimensionEntry=0;
	>>
	Fd LIMIT      
	(
		Fd
		cm:characteristic_mnemonic[nounEntry]>[modifierEntry]
		rcl:required_characteristic_limit[modifierEntry,dimensionEntry]
		{ re:real_errlim[dimensionEntry] }
		<<
			AST * rl=new RequireLimitAction;
			#(rl,#(#cm, #rcl ,#(new RealErrlim, #re ) ) );
			modifierList->insert( rl );
		>>
	)+
	 ;     

required_characteristic_limit![ModifierEntry * modifierEntry,DimensionEntry * & dimensionEntry]:                  // fsd-- 14.22A
		rr:require_range[modifierEntry,dimensionEntry]
			<< #0 = #rr; >>
	|	(
			MAX	<< #0=new ConstraintMax;>>
		|	MIN	<< #0=new ConstraintMin;>>
		) dn:dimensioned_number[modifierEntry,dimensionEntry]
			<<
				#0=#(#0,#dn);
			>>
    	;
   
require_cnx![NounEntry * nounEntry]:     // fsd-- 14.23     
	Fd 
	( 
		CNXdNUMERATOR num:req_pin[nounEntry]    CNXdDENOMINATOR  den:req_pin[nounEntry]
		<< #0 = #( 0, #( new CnxNumeratorType, #num ), #( new CnxDenominatorType, #den ) ); >>
	|
		cnx:CNX
		{	rp:req_pin[nounEntry]
			<<
				if(!nounAllowsDescriptors(nounEntry)){
					Error_Report(" Pin Descriptors are not allowed for:"+nounEntry->noun,#rp);
				}
			>>
		
		}                          //<<Esc_1 >> 
		<<
			#0 = #rp;
		>>
	)
	<< #0 = #( new RequireCnxAction, #0 ); >>
	;     
//  &*    This bypass is allowed only for nouns where a
//  &     pin descriptor is not meaningful such as
//  &     COMMON, EARTH & SHORT.*
    
  
req_pin![NounEntry * nounEntry]:                                        // fsd-- 14.23A
	pd1:pin_descriptor
	( pda:pin_descriptor 	<< #0 = #( 0, #pd1, #pda ); >> )*
	{ REF ( pdb:pin_descriptor << #0 = #( 0, #pd1, #pdb ); >> )+ }
	;

exchange_expression![AST * rf]:				// fsd-- 14.24
	(
		<<	ASTList * efs=0;	>>
		LP ef1:exchange_frame[rf]
		<< efs=new ASTList; efs->append(#ef1);	>>
		
			(
				( AND aefn:exchange_frame[rf]	<< efs->append(#aefn);>> )+
				<< #0= new SpawnTasksAllAction(efs); >>
			|	( OR  oefn:exchange_frame[rf]	<< efs->append(#oefn); >>)+
				<< #0= new SpawnTasksAnyAction(efs); >>
			|
				<< #0= new SpawnTasksAnyAction(efs); >>
			)
		RP
	)
	 {THEN ee:exchange_expression[rf]}
	 << #0=#(0,#0,#ee); >>	// sequential execution with THEN
	;

exchange_frame![AST * rf]:                                 // fsd-- 14.25
	<<	ASTList  exs; AST * prio=0;	>>
	{ 
		STARTING wf:when_field
		ef:exchange_frequency[exs]>[AST * prio] Fd 
	}
	EXCHANGE dt1:databus_transaction[rf,exs]
		<< #0=#dt1; >>
		(
			Fd EXCHANGE dtn:databus_transaction[rf,exs]
			<< #0=#(0,#0,#dtn); >>
		)*
	<<
		#0=	#(new ExchangeFrame(prio),#(#wf,#(#ef,#(new ExchangeSchedule,#0))));
	>>
	;

exchange_frequency![ASTList & exs]>[AST * prio]:                          // fsd-- 14.25A
	<<
		DimensionEntry * dimensionEntry=0;
	>>
	{ 
		Fd WITH PRIORITY p:unsigned_integer_number	<< $prio=#p; >>
	}  
	{ Fd EVERY
		(
			EVENT pe:event
			<< #0= new PeriodicEventCounterType(#pe); >>
		|	tq:time_quantity>[dimensionEntry]
			<< #0= #(new PeriodicTimerType,#tq); >>
		) 
	}
	{ eend:exchange_end[exs] << #0=#(#eend,#0); >> }
	;

exchange_end![ASTList & exs]:                                   // fsd-- 14.25B
	<<
		DimensionEntry * dimensionEntry=0;
	>>
	Fd UNTIL
	( 
		(
			TIMER t:timer EQcomp
			<< #0=new WaitForTimerAction; >>
		|	()
			<< #0=new WaitForTimeAction;  >>
		)
		tq:time_quantity>[dimensionEntry]
		<<
		 	#0=#t?#(#0,#t,#tq):#(#0,#tq);
			#0=#(new WatchDog,#0);
		>>
	|	EVENT ev:event
		<< #0=#(new WatchDog,#(new WaitForEventAction,#ev)); >>
	|	EXCHANGE ex:exchange
		<<
			if(exs.contains(#ex->label())){
				Error_Report("An exchange must be only referenced once:",ex);
			} else {
				exs.insert(#ex->label());
			}
			#0=#(new WatchDog,#ex);
		>>
	)
	;

databus_transaction![AST * rf,ASTList & exs]:                            // fsd-- 14.25C
	<<
		DimensionEntry * dimensionEntry=0;
	>>
	ex:exchange 
		<<
			if(exs.contains(#ex->label())){
				Error_Report("An exchange must be only referenced once:",ex);
			} else {
				exs.insert(#ex->label());
			}
		>>
	{ Fd r_f:role_field  }
	{ Fd es:exchange_simulate } 
	{ Fd em:exchange_monitor }
	{ Fd DELAY tq:time_quantity>[dimensionEntry] }
	<<
		if(!rf && !#r_f){
			Error_Report("Must have a role-field:",#ex);
		}
		#0=	#(
				new DataBusTransaction,
					#ex,
					#(new RoleField, #r_f?#r_f:rf),
					#(new ExchangeSimulateAction,#es),
					#(new ExchangeMonitor,#em),
					#(new WaitForTimeAction,#tq)
			);
	>>
	;

exchange_simulate!:                              // fsd-- 14.25D
	TESTdEQUIPdSIMULATE 
	{ TALKER tsd:simulate_device	<<#0=#(new TalkerSimulateAction,#tsd);>> }
	{ LISTENER (lsd:simulate_device	<<#0=#(0,new ListenerSimulateAction,#lsd,#0);>> )+ }
	<< #0=#(new TestEquipSimulateAction,#0); >>
	;

simulate_device!:                                 // fsd-- 14.25E
	(	din:device_identifier_name
	|	uut:UUT
	)             //<<Esc_1 >>
	{ LP ( (ds:data_store)? | c:constant )  RP }
	<< #0=#(new DataBusDevice, #din?#din:#uut,#ds?#ds:#c); >>
	;
//  &*    The device must be one which has been established
//  &     as valid for the TEST-EQUIP to simulate in the
//  &     ESTABLISH statement for the protocol.*

exchange_monitor:                               // fsd-- 14.25F
	TESTdEQUIPdMONITOR 
	( 
		fpp:fetch_protocol_parameter	<< #0=#fpp; >>
	|	bp:bus_parameter		<< #0=#bp; >>
	|	( fdd:fetch_databus_data 	<< #0=#0?#(0,#0,#fdd):#fdd; >> )+
	)
	<< #0=#(new TestEquipMonitor,#0); >>
	;

fetch_databus_data!:                             // fsd-- 14.25G
	( COMMAND | STATUS | DATA )
	( 
		(array_range { Fd COUNTdINTO integer_data_store })?
	|	data_store
	)
	;

gate_field!:                                     // fsd-- 14.26       
	GATED 
	(
			FROM fe:event TO te:event
				<< #0 = #( new GateField, #fe, #te ); >>	// start 'fe' end 'te'
		|	BY by:event_interval 
				<< #0 = #( new GateField, #by ); >>		// during 'by'
	)
	;

stim_rate!:                                      // fsd-- 14.27
	STIMdRATE ssr:stim_sense_rate	<< #0 = #ssr; >>;

stim_sense_rate!:                                // fsd-- 14.67-1
	<<
		ModifierEntry * modifierEntry=0;
		DimensionEntry * dimensionEntry=0;
	>>
	(
		(
			udne:unsigned_decimal_number[modifierEntry]
			<< #0 = #udne; >>
		|	dse:data_store
			<< #0 = #dse; >>
		) dim[modifierEntry,dimensionEntry]
		<< #0 = #( new DimensionedNumberType(modifierEntry), #0, #d ); >>
		{	
			re:real_errlim[dimensionEntry]
			<< #0 = #( new RealErrlim, #0, #re ); >>
		}
	|	MAX
		(
			udnm:unsigned_decimal_number[modifierEntry]
			<< #0 = #udnm; >>
		|	dsm:data_store 
			<< #0 = #dsm; >>
		) 
		d:dim[modifierEntry,dimensionEntry]
		<< #0 = #( new ConstraintMax, #0, #d ); >>
	)
	;

sense_rate!:                                     // fsd-- 14.28       
	SENSEdRATE ssr:stim_sense_rate	<< #0 = #ssr; >>;

sense_delay:                                    // fsd-- 14.29       
	<< DimensionEntry * dimensionEntry=0; >>
	SENSEdDELAY 
	( 
		tq:time_quantity>[dimensionEntry] { re:real_errlim[dimensionEntry] << #( new RealErrlim,#tq,#re); >>}
	|	MIN time_quantity>[dimensionEntry]
	)
	;

stim_event!:                                     // fsd-- 14.30       
	STIMdEVENT e:event { mt:max_time }
	;

sense_event!:                                    // fsd-- 14.31       
	SENSEdEVENT e:event { mt:max_time }
	;

when_field!:                                     // fsd-- 14.32       
	WHEN w:when_body	<< #0 = new WhenFieldAction(#w); >>
	;


when_body!:                                      // fsd-- 14.32A
	<< DimensionEntry * dimensionEntry=0; >>
	
		({TIMER} timer)?
		{TIMER} t:timer	tq:time_quantity>[dimensionEntry] { re:real_errlim[dimensionEntry] }
		<< #( new WaitForTimerAction,#t, #tq, #re ) ; >>
	|
		{EVENT} e:event  mt:max_time
		<< #0 = #(#mt,#(new WaitForEventAction, #e)); >>
	;

digital_source_characteristics[NounEntry * nounEntry]:   // fsd-- 14.33
	<<
		ModifierEntry * modifierEntry=0;
	>>
	characteristic_mnemonic[nounEntry]>[modifierEntry]
		real_characteristic_subfield[modifierEntry,0 /*re*/ , 0]
	(
		Fd characteristic_mnemonic[nounEntry]>[modifierEntry]
			real_characteristic_subfield[modifierEntry,0 /*re*/ , 0]
	)*
	;

digital_sensor_characteristics[NounEntry * nounEntry]:       // fsd-- 14.34     
	<< ModifierEntry * modifierEntry=0; >>
		evaluation_field[modifierEntry,0] (characteristic_mnemonic[nounEntry]>[modifierEntry]
			real_characteristic_subfield[modifierEntry,0 /*re*/ , 0])*
	|	(characteristic_mnemonic[nounEntry]>[modifierEntry]
			real_characteristic_subfield[modifierEntry,0 /*re*/ , 0])+
	;
  
on_field:                                       // fsd-- 14.37       
	ON 
	( 
		(digital_source)?
	|	(digital_sensor)?
	|	configuration 
	)      
	{ { EXCEPT } CNX conn_set["",0] }
	;       

   
bus_parameter:                                  // fsd-- 14.41
	<<
		DimensionEntry * dimensionEntry=0;
		ModifierEntry * modifierEntry=0;
	>>
	BUSdPARAMETER  
	bus_value>[modifierEntry] dim[modifierEntry,dimensionEntry]
			( (Fd bus_value>[modifierEntry] dim[modifierEntry,dimensionEntry])? )*
	;

bus_value>[ModifierEntry * modifierEntry]:		// fsd-- 14.41A
	( 
		bus_parameter_name>[$modifierEntry]		//<<Esc_1 >>
	)
	(
		(constant)?
	|	data_store
	)
	;
//  &*     The bus parameter name must have been established as
//  &      valid for the databus in the associated ESTABLISH
//  &      statement.  For it to occur in an UPDATE statement
//  &      of either sort, it must have been defined as UPDATABLE
//  &      for that category of UPDATE.  It and WORD-GAP are the
//  &      only form of protocol parameter that can occur in a
//  &      FETCH statement and then only if established as
//  &      monitord by the test equipment in the <do exchange
//  &      statement>.*
  
set_protocol_parameter:                         // fsd-- 14.42
	PROTOCOLdPARAMETER
	aa_set_protocol_parameter_1 ( Fd aa_set_protocol_parameter_1 )*
	;
  
aa_set_protocol_parameter_1:
	{ INSERTdINVALID }
	protocol_parameter_set 
	;  
     
protocol_parameter_set:                         // fsd-- 14.42A
	(
		COMMANDdWORD
	|	STATUSdWORD
	|	DATAdWORD
	)	unsigned_integer_number protocol_parameter_characteristic
	|
	( 
		WORDdCOUNT { /*sign*/ unsigned_integer_number }
	|	protocol_parameter_name
			{
				(data_store)?
			| 	constant
			}
	)
	;
//  &*     The protocol parameter name must have been established
//  &      as valid for the databus in the associated ESTABLISH
//  &      statement.  For it to occur in an UPDATE statement
//  &      of either sort, it must have been defined as UPDATABLE
//  &      for that category of UPDATE.*

protocol_parameter_characteristic:             // fsd-- 14.42B
		BIT unsigned_integer_number PULSEdCODE
	|	SYNC
	|	LENGTH
	|	PARITY
	|	TdR  
	;
  
fetch_protocol_parameter:                       // fsd-- 14.43
	PROTOCOLdPARAMETER 
		protocol_parameter_fetched
		(
			Fd protocol_parameter_fetched
		)*
	;
  
protocol_parameter_fetched:                     // fsd-- 14.43A
	standard_protocol_parameter 
	FAULTdCOUNT data_store       |
	protocol_parameter_name                       //<<Esc_1 >> 
	( 
		FAULTdCOUNT data_store
	|
		data_store 
	) 
	;
//  &*     The protocol parameter name must have been established
//  &      as valid for the databus in the associated ESTABLISH
//  &      statement.  For it to occur in an UPDATE statement
//  &      of either sort, it must have been defined as UPDATABLE
//  &      for that category of UPDATE.  It is the only form of
//  &      protocol parameter that can occur in a FETCH statement
//  &      and then only if it has been established as monitord by
//  &      the test equipment in the <do exchange statement>.*
      
  
standard_protocol_parameter:                   // fsd-- 14.43B
	( 
		( 
			COMMANDdWORD
		|	STATUSdWORD
		|	DATAdWORD  
		)
		( 
			PULSEdCODE
		|	SYNC
		|	LENGTH
		|	PARITY
		|	TdR           
		) 
	)
	|	WORDdCOUNT  
	;
      
      
role_field:                                     // fsd-- 14.44
	TESTdEQUIPdROLE!	test_equip_role_name
	;
//  &*     The test_equip role name must have been established
//  &      as valid for the databus in the associated <establish
//  &      protocol statement>.*
  
  

// 18.6.12 BASIC SYMBOLS / CHARACTER SETS
   
//sign!:                                           // fsd-- 15.1.1   
//		PLUSop	<< #0 = new IdentityOperator; >>
//	|	MINUSop	<< #0 = new NegativeOperator; >>
//	;
  
number:								// fsd-- 15.2       
		decimal_number[0]
	|	digital_number 
	;

decimal_number![ModifierEntry * modifierEntry]:			// fsd-- 15.2.1
		MINUSop
			(
				udn:Unsigned_Decimal_Number
				<< #0 = new DecimalNumber($udn,-1); >>
			|	uin:Unsigned_Integer_Number
				<<
					if(modifierEntry&&isType(modifierEntry,"R","RA")){	
						#0 = new DecimalNumber($uin,-1);
					} else {
						#0 = new IntegerNumber($uin,-1);
					}
				>>
			)
	|	{PLUSop}  udnp:unsigned_decimal_number[modifierEntry]	<< #0 = #udnp; >>
	;   
  
unsigned_decimal_number![ModifierEntry * modifierEntry]:	// fsd-- 15.2.2
		udecimal:Unsigned_Decimal_Number
		<<#0=new DecimalNumber($udecimal);>> 
	|	uin:Unsigned_Integer_Number
		<<
			if(modifierEntry&&isType(modifierEntry,"R","RA")){	
				#0 = new DecimalNumber($uin);
			} else {
				#0 = new IntegerNumber($uin);
			}
		>>
	;
  
unsigned_integer_number!:                        // fsd-- 15.2.3
		uinteg:Unsigned_Integer_Number
		<<#0=new IntegerNumber($uinteg);>> 
	;

binary_number!:                                  // fsd-- 15.2.4
		binnum:Binary_Number
		<< #0=new StringOfBitType($binnum); >>
	|	sbit:Single_Bit
		<< #0=new StringOfBitType($sbit); >>
	;

single_bit!:
		sbit:Single_Bit
		<< #0=new BitType($sbit); >> ;
  
octal_number!:                                   // fsd-- 15.2.5   
		octnum:Octal_Number
		<< #0 = new StringOfBitType( $octnum ); >> ;   

hexadecimal_number!:                             // fsd-- 15.2.6   
		hexnum:Hexadecimal_Number
		<< #0 = new StringOfBitType( $hexnum ); >> ;   

digital_number:                                 // fsd-- 15.2.7   
		binary_number
	|	octal_number
	|	hexadecimal_number 
	;   
   
statement_number!:                               // fsd-- 15.3.3   
	uinteg:Unsigned_Integer_Number	<< #0 = new StatementNumber( $uinteg ); >> ;

// 18.6.13 IDENTIFIERS / LITERALS  / LABELS 
  

block!:                                          // fsd-- 4.2.2-2
	<<isType(LT(1)->getText(),Block_Name_Label)>>?
		[Error_Report(RWCString("BLOCK label expected:")+LT(1)->getText(),LT(1)->getLine());]
		id:ID
		<<#0=#(new Proxy($id),getTokenLabel($id->getText()));>>
	;   

terminal_identifier!:                            // fsd-- 6.5-2 
	<<isType(LT(1)->getText(),Terminal_Identifier_Label)>>?
		[Error_Report(RWCString("TERMINAL IDENTIFIER expected:")+LT(1)->getText(),LT(1)->getLine());]
		id:TID
		<<#0=#(new Proxy($id),getTokenLabel($id->getText()));>>
	 ;

connection!:					// fsd-- 14.24-1
		c:connection_label
		<< #0 = #( new Proxy( #c->getToken() ), #c ); >>;

connection_label!:
			tid:terminal_identifier
			<<
				#0=#tid;
			>>
		|	cid:CID
			<<
				AST * label;
				if(label=getTokenLabelGlobal( $cid->getText())){
					if(!isType($cid->getText(),Connection_Type_Label)){
						Error_Report(" Connection expected :",$cid);
					}
					#0=label;
				} else {
					#0=scope->insert_label
						(
							(AST *)#(new ConnectionTypeLabel($cid),new ConnectionType($cid))
						);
				}
			>>
	|	c:COMMON
			<<
				#0=getTokenLabelGlobal( "COMMON");
			>>
	|	e:EARTH
			<<
				#0=getTokenLabelGlobal( "EARTH");
			>>
	|	a:ATMOS
			<<
				#0=getTokenLabelGlobal( "ATMOS");
			>>
	;

parameter!:                                      // fsd-- 15.6.5  
	<<isType(LT(1)->getText(),Parameter_Label)>>?
		[Error_Report(RWCString("PARAMETER label expected:")+LT(1)->getText(),LT(1)->getLine());]
		id:ID 
		<<#0=#(new Proxy($id),getTokenLabel($id->getText()));>>
	;

message_text!:                                   // fsd-- 15.10.2   
	~StatementTerminator // {message_text}
	;

drawing_identifier!:                             // fsd-- 15.10.4   
	DrawingID	//	This is just a long document name .....
	;

module_name!:                                    // fsd-- 15.10.6
		ID // not referenced anywhere, thus not inserted. See FSD
	//  <<isType(LT(1)->getText(),Module_Name_Label)>>? id:ID << #0=#(new Proxy($id),getTokenLabel($id->getText()));>>
	;

pin_descriptor_name!:                            // fsd-- 15.10.8.1
	<<isType(LT(1)->getText(),Pin_Descriptor_Name_Label)>>?
		[Error_Report(RWCString("PIN DESCRIPTOR NAME expected:")+LT(1)->getText(),LT(1)->getLine());]
		id:EID
		<<#0=#(new Proxy($id),getTokenLabel($id->getText()));>>
	;

test_equip_role_name!:                           // fsd-- 15.10.15
	<< RWCString left;ModifierEntry * modifierEntry=0; >>
	<< modifierEntry=mnemonicsDB->theModifierEntry(LT(1)->getText(),"~TEST-EQUIP-ROLE",left)>>?
		[Error_Report(RWCString("Not a TEST-EQUIP-ROLE Name :"),LT(1));]
		m:ModifierCLASS
	<<
		if(!(left=="")){ Error_Report("TEST-EQUIP-ROLE ",$m);}
		#0 = new TestEquipRoleName($m,modifierEntry,this);
	>>
	;

bus_mode_name!:                                  // fsd-- 15.10.16
	<< RWCString left;ModifierEntry * modifierEntry=0; >>
	<< modifierEntry=mnemonicsDB->theModifierEntry(LT(1)->getText(),"~BUS-MODE",left)>>?
		[Error_Report(RWCString("Not a BUS-MODE Name :"),LT(1));]
		m:ModifierCLASS
	<<
		if(!(left=="")){ Error_Report("BAD BUS-MODE ",$m);}
		#0 = new BusModeName($m,modifierEntry,this);
	>>
	;

bus_parameter_name!>[ModifierEntry * modifierEntry]:                             // fsd-- 15.10.17
	<< RWCString left;>>
	<< $modifierEntry=mnemonicsDB->theModifierEntry(LT(1)->getText(),"~BUS-PARAMETER",left)>>?
		[Error_Report(RWCString("Not a BUS-PARAMETER Name :"),LT(1));]
		m:ModifierCLASS
	<<
		if(!(left=="")){ Error_Report("BAD BUS-PARAMETER ",$m);}
		#0 = new BusParameterName($m,$modifierEntry,this);
	>>
	;

protocol_parameter_name!:                        // fsd-- 15.10.18
	<<isType(LT(1)->getText(),Protocol_Parameter_Name_Label)>>?
		[Error_Report(RWCString("PROTOCOL PARAMETER NAME expected:")+LT(1)->getText(),LT(1)->getLine());]
		id:EID
		<<#0=#(new Proxy($id),getTokenLabel($id->getText()));>> // extend_label 
	;

bus_specification!:                              // fsd-- 15.10.19
	<<isType(LT(1)->getText(), Bus_Specification_Label)>>?
		[Error_Report(RWCString("BUS SPECIFICATION expected:")+LT(1)->getText(),LT(1)->getLine());]
		id:ID
		<<#0=#(new Proxy($id),getTokenLabel($id->getText()));>>
	;

constant_identifier![TheType *storage_type]:                            // fsd-- 15.11.2
	<<*storage_type = getType( LT(1)->getText() );>>
	<<isType(LT(1)->getText(),Constant_Identifier_Label)>>?
		[Error_Report(RWCString("CONSTANT IDENTIFIER expected:")+LT(1)->getText(),LT(1)->getLine());]
		id:ID
		<<#0=#(new Proxy($id),getTokenLabel($id->getText()));>>
	;

type_identifier!:                                // fsd-- 15.11.3
	<<isTypeGlobal(LT(1)->getText(),Type_Identifier_Label)>>?
		[Error_Report(RWCString("TYPE IDENTIFIER expected:")+LT(1)->getText(),LT(1)->getLine());]
		id:ID 
		<< #0 = #(new Proxy($id, scope), getTokenLabelGlobal( $id->getText() )); >>
	;

variable_identifier![TheType *storage_type, AST **v]:                            // fsd-- 15.11.4
	<<
		*v = getTokenLabel(LT(1)->getText());
		if ( *v ){
			*storage_type = (*v)->getType();
		}else{
			*storage_type = getType( LT(1)->getText() );
		}
	>>
	<<isType(LT(1)->getText(),Variable_Identifier_Label)>>?
		[Error_Report(RWCString("VARIABLE IDENTIFIER expected:")+LT(1)->getText(),LT(1)->getLine());]
		id:ID
		<<#0=#(new Proxy($id),*v);>>
	;

enumeration_element!:                            // fsd-- 15.11.5
	<<isType(LT(1)->getText(),Enumeration_Element_Label)>>?
		[Error_Report(RWCString("ENUMERATION ELEMENT expected:")+LT(1)->getText(),LT(1)->getLine());]
		id:ID
		<<#0=#(new Proxy($id),getTokenLabel($id->getText()));>>
	;

record_field!:                                   // fsd-- 15.11.6
	<<isType(LT(1)->getText(),Variable_Identifier_Label)>>?
		[Error_Report(RWCString("RECORD FIELD expected:")+LT(1)->getText(),LT(1)->getLine());]
		id:ID
		<<#0=#(new Proxy($id),getTokenLabel($id->getText()));>>
	;

procedure!:                                      // fsd-- 15.11.9
	<<isType(LT(1)->getText(),Procedure_Label)>>?
		[Error_Report(RWCString("PROCEDURE LABEL expected:")+LT(1)->getText(),LT(1)->getLine());]
		id:ID
		<<#0=#(new Proxy($id),getTokenLabel($id->getText()));>>
	;

function![TheType *storage_type]:                                       // fsd-- 15.11.10
	<<*storage_type = getGlobalType( LT(1)->getText() );>>
	<<isTypeGlobal( LT(1)->getText(), Function_Label )>>?
		[Error_Report(RWCString("FUNCTION LABEL expected:")+LT(1)->getText(),LT(1)->getLine());]
		id:ID
		<<#0 = #(new Proxy($id), getTokenLabelGlobal( $id->getText() ));>>
	;

signal!:                                         // fsd-- 15.11.11
	<<isType( LT(1)->getText(), Signal_Label )>>?
		[Error_Report( RWCString( "SIGNAL LABEL expected:" ) + LT(1)->getText(), LT(1)->getLine() );]
		id:ID
		<<#0=#(new Proxy($id),getTokenLabel($id->getText()));>>
	;

event!:                                          // fsd-- 15.11.12
	<< isType( LT(1)->getText(), Event_Label ) >>?
		[ Error_Report( RWCString( "EVENT LABEL expected:" ) + LT(1)->getText(), LT(1)->getLine() ); ]
		id:ID
		<< #0 = #( new Proxy($id), getTokenLabel( $id->getText() ) ); >>
	;

event_indicator!:                                // fsd-- 15.11.13
	<<isEventIndicator(LT(1)->getText())>>?
		[Error_Report(RWCString("EVENT INDICATOR LABEL expected:")+LT(1)->getText(),LT(1)->getLine());]
		id:ID
		<<#0=#(new Proxy($id),getTokenLabel($id->getText()));>>
	;

event_interval!:                                 // fsd-- 15.11.14
	<< isType( LT(1)->getText(), Event_Interval_Label ) >>?
		[ Error_Report( RWCString( "EVENT INTERVAL LABEL expected:" ) + LT(1)->getText(), LT(1)->getLine() ); ]
		id:ID
		<< #0 = #( new Proxy($id), getTokenLabel( $id->getText() ) ); >>
	;

timer!:                                          // fsd-- 15.11.15
	<< isType( LT(1)->getText(), Timer_Label ) >>?
		[ Error_Report( RWCString( "TIMER LABEL expected:" ) + LT(1)->getText(), LT(1)->getLine() ); ]
		id:ID
		<< #0 = #( new Proxy($id), getTokenLabel( $id->getText() ) ); >>
	;

protocol!:                                       // fsd-- 15.11.16
	<<isType(LT(1)->getText(),Protocol_Label)>>?
		[Error_Report(RWCString("PROTOCOL LABEL expected:")+LT(1)->getText(),LT(1)->getLine());]
		id:ID
		<<#0=#(new Proxy($id),getTokenLabel($id->getText()));>>
	;

exchange!:                                       // fsd-- 15.11.17
	<<isType(LT(1)->getText(),Exchange_Label)>>?
		[Error_Report(RWCString("EXCHANGE LABEL expected:")+LT(1)->getText(),LT(1)->getLine());]
		id:ID
		<<#0=#(new Proxy($id),getTokenLabel($id->getText()));>>
	;

configuration!:                                  // fsd-- 15.11.18
	<<isType(LT(1)->getText(),Configuration_Label)>>?
		[Error_Report(RWCString("CONFIGURATION LABEL expected:")+LT(1)->getText(),LT(1)->getLine());]
		id:ID
		<<#0=#(new Proxy($id),getTokenLabel($id->getText()));>>
	;

digital_source!:                                 // fsd-- 15.11.19
	<< isType(LT(1)->getText(), Digital_Source_Label ) >>?
		[ Error_Report( RWCString( "DIGITAL SOURCE LABEL expected:" ) + LT(1)->getText(), LT(1)->getLine()); ]
		id:ID
		<<#0=#(new Proxy($id),getTokenLabel($id->getText()));>>
	;

digital_sensor!:                                 // fsd-- 15.11.20
	<< isType(LT(1)->getText(), Digital_Sensor_Label ) >>?
		[ Error_Report(RWCString( "DIGITAL SENSOR LABEL expected:" ) + LT(1)->getText(), LT(1)->getLine() ); ]
		id:ID
		<< #0 = #( new Proxy($id), getTokenLabel( $id->getText() ) ); >>
	;

dim_name!:                                       // fsd-- 15.12.21  //  extend_label
	<< isType( LT(1)->getText(), Dim_Name_Label ) >>?
		[ Error_Report( RWCString( "DIMENSION NAME LABEL expected:" ) + LT(1)->getText(), LT(1)->getLine() ); ]
		id:EID
		<< #0 = #( new Proxy($id), getTokenLabel( $id->getText() ) ); >> 
	;

external_semantics_specification!:               // fsd-- 15.11.22
		ID
	;

external_syntax_specification!:                  // fsd-- 15.11.23
		ID
	;

drawing!:                                        // fsd-- 15.11.24
	<< isType( LT(1)->getText(), Drawing_Label ) >>?
		[ Error_Report(RWCString( "DRAWING LABEL expected:" ) + LT(1)->getText(), LT(1)->getLine() ); ]
		id:ID
		<< #0 = #( new Proxy($id), getTokenLabel( $id->getText() ) ); >>
	;

device_identifier_name:                         // fsd-- 15.11.25
	enumeration_element
	;

requirement![AST *& re]:                                    // fsd-- 15.11.26
	<< re=isType( LT(1)->getText(), Requirement_Label ) >>?
		[ Error_Report( RWCString( "REQUIREMENT LABEL expected:" ) + LT(1)->getText(), LT(1)->getLine() ); ]
		id:ID
		<< #0 = #( new Proxy($id), getTokenLabel( $id->getText() ) ); >>
	;

exchange_configuration!:                         // fsd-- 15.11.27
	<< isType(LT(1)->getText(), Exchange_Configuration_Label) >>?
		[Error_Report( RWCString( "EXCHANGE CONFIGURATION LABEL expected:" ) + LT(1)->getText(), LT(1)->getLine() ); ]
		id:ID 
		<< #0 = #( new Proxy($id), getTokenLabel( $id->getText() ) ); >>
	;

external_digital_specification:                 // fsd-- 15.11.29
		ID
	;

external_pulse_class_specification:             // fsd-- 6.9.3G
		ID
	;

program_name!:                                   // fsd-- 15.11.30
  	<< isType( LT(1)->getText(), Program_Name_Label ) >>?
		[ Error_Report( RWCString( "PROGRAM NAME expected:" ) + LT(1)->getText(), LT(1)->getLine()); ]
  		id:ID
  		<< #0 = #( new Proxy($id), getTokenLabel( $id->getText() ) ); >>
  	;
 
// 18.6.13. SEPARATORS AND STATEMENT-NUMBERS   
  
  
//fd:						// fsd-- 3.2.4   
//	Fd		// fd is the field deliminator
//	;

statement_terminator!:                           // fsd-- 3.2.6.   
  		StatementTerminator
	;   
 
// 18.6.15  DESCRIPTORS   
  

modifier_descriptor_pulse_class:                // fsd-- 6.19.3
	( 
			RZ
		|	NRZ
		|	BIP
		|	MIP
		|	AMI
		|	HDB unsigned_integer_number 
	)
	;

pin_descriptor!:                                 // fsd-- 14.14.1.1
			ppd:predefined_pin_descriptor
			<< #0=#ppd; >>
		|	tpd:tpsdefined_pin_descriptor
			<< #0=#tpd; >>
	;
	
predefined_pin_descriptor!:
		// HI,LO,VIA,A,B,C,N,TRUE,COMPL,SCREEN,GUARD,R1,R2,R3,R4,S1,S2,S3,S4,X,Y,Z
		pdc:PinDescriptorCLASS << #0=new PreDefinedPinDescriptor($pdc); >>
	;

tpsdefined_pin_descriptor:
			pin_descriptor_name
		|	terminal_identifier 
	;
	

reference_phase:                                // fsd-- 14.14.1.2
	(
		PHASEdA | PHASEdAB | PHASEdAC |
		PHASEdB | PHASEdBC | PHASEdCB |
		PHASEdC | PHASEdCA | PHASEdBA
	)
	;

modifier_descriptor[ModifierEntry *&modifierEntry]:                            // fsd-- 15.10.5
	( 
		modifier_descriptor_pulse_class
//	|	modifier_descriptor_name 
	)
	;

// 18.6.16 NOUNS

analog_noun![NounEntry * & nounEntry]>[NounEntry * nounEntry]:					// fsd-- 16.0
		<< int howmany=0;int count=0; NounEntry * saveNoun=nounEntry ; >>
		() << nounEntry = theNounEntry( LT(1) , howmany ) >>?
			[ Error_Report( "Atlas Noun expected:", LT(1) ); ]
			( nc:NounCLASS << count ++; >>)+
			<<
				if (nounEntry && (howmany!=count)){
					Error_Report( "ATLAS Noun is ambiguous at:", $nc );
				}
				#0 = new NounType( $nc, nounEntry );
				$nounEntry=nounEntry;
				if(saveNoun){
					if(nounEntry!=saveNoun){
						Error_Report("Expected "+saveNoun->noun,$nc);
					}
				}
			>>
	; 
  
   
// 18.6.17 NOUN-MODIFIERS  
  
measured_characteristic_mnemonic[NounEntry * nounEntry]>[ModifierEntry * modifierEntry] : // fsd-- 17.2
		<< RWCString left;ANTLRTokenPtr p; >>
		modifier[nounEntry,left]>[$modifierEntry,p]
		<<
			#0=new MeasuredCharacteristicMnemonicType(p,$modifierEntry,left,this);
		>>
	;

characteristic_mnemonic![NounEntry * nounEntry]>[ModifierEntry * modifierEntry]:     // fsd-- 17.2
		<< RWCString last;ANTLRTokenPtr p; >>
		modifier[nounEntry,last]>[$modifierEntry,p]
		<<
			#0=new CharacteristicMnemonicType(p,$modifierEntry,last,this);
		>>
	;

modifier![NounEntry * nounEntry,RWCString & left]>[ModifierEntry * modifierEntry,ANTLRTokenPtr p]:  // fsd-- 17.3
	() << $modifierEntry = mnemonicsDB->theModifierEntry( LT(1)->getText(), nounEntry ,left ) >>?
		[
			Error_Report( "This Modifier is not allowed", LT(1) );
			// create one so that the rest will work fine.... Under protest of course..
			$modifierEntry=new ModifierEntry(nounEntry,LT(1)->getText());
		]
		m:ModifierCLASS
	<< $p= $m; >>
	;


// 18.6.19 DIMENSIONS   

dim[ ModifierEntry * modifierEntry, DimensionEntry * & dimensionEntry ]:	// fsd-- 15.8
		(dimension[ modifierEntry, dimensionEntry ])?
  	|	
  		()
  		<<
   			dimensionEntry=0;
 			if(mnemonicsDB->theDimensionEntry( "None" , modifierEntry )){
  			} else {
  				Error_Report( "Modifier:"+modifierEntry->modifier+" Requires dimension at:", LT(1) );
  			}
  		>>
	;

dimension![ModifierEntry * modifierEntry, DimensionEntry *& dimensionEntry]:	// fsd-- 15.8
		<< DimensionEntry *  savedDimensionEntry=dimensionEntry; >>
		//()
  		//<< dimensionEntry = mnemonicsDB->theDimensionEntry( LT(1)->getText(), modifierEntry ) >>?
		//[ Error_Report( "Modifier:"+modifierEntry->modifier+" Does not admit dimension:", LT(1) ); ] 
  		did:DimensionCLASS
  			<<
  				if(!(dimensionEntry = mnemonicsDB->theDimensionEntry( $did->getText(), modifierEntry))){
  					Error_Report( "Modifier:"+modifierEntry->modifier+" Does not admit dimension:", $did );
  				} else {
  					#0 = new DimensionType( $did, dimensionEntry );
					if(savedDimensionEntry && (savedDimensionEntry!=dimensionEntry) ){
						Error_Report(	"Previous dim:"+savedDimensionEntry->dimension+
							" and This dim:"+dimensionEntry->dimension+" Do not match ",$did);
					}
				}
  			>>
	;

time_dim![DimensionEntry *& dimensionEntry]:
	<< DimensionDictionary * dimensionDictionary =  mnemonicsDB->theDimensionDictionary( "Time" ); >>
	<< dimensionEntry = mnemonicsDB->theDimensionEntry( LT(1)->getText(), dimensionDictionary ) >>?
		[ Error_Report( "Time dimension expected:", LT(1) ); ] 
	eid:EID
  		<< #0 = new DimensionType( $eid, dimensionEntry ); >>
	;

}
