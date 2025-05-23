
{***********************************************************************}
{             Boeing Electronics Company                                }
{                                                                       }
{  Title     :  WITS ( Wiring Integrated Test Simulator)                }
{                                                                       }
{  Written by:  Sinan  Karasu                                           }
{                                                                       }
{  Date      :  September 1 ,1986                                       }
{                                                                       }
{  Rev Date  :  05-28-1992                                              }
{                                                                       }
{  Dataset   :  TESSWITS.AAP                                            }
{                                                                       }
{  Purpose   :  Generation of FACT Wiring analyzer programs.            }
{                                                                       }
{***********************************************************************}
{  Revision Control Block:                                              }
{  Rev A: 10-22-86    Added Label 8888 to program to facilitate graceful}
{                      abort without any messages. SIK                  }
{         10-22-86    Added checks against min_v and max_v in routine   }
{                      insert_node                                      }
{         11-20-86    Fixed GetNext routine so that  two commas in a    }
{                      row would not skip to next line in a bracket     }
{                      [a,,b] type construct.                           }
{         01-09-87    Deleted 0.5 derating factor in calculation        }
{                      of maximum current in 4-wire resistance checks   }
{         01-09-87    Added Display_Lines:=0; into Add_Node routine     }
{                      to fix erroneous DW%& appearences.               }
{  Rev B: 01-26-87    Fixed addressing bug in terminal blocks           }
{  Rev C: 02-12-87    Fixed 10 Ohms boundary problem in  Cont. Checks   }
{                     ( and extensive rewrite)                          }
{  Rev D: 09-07-88    Corrected XCI and XC equivalence as first test.   }
{                     Implemented Resistors and PACKS                   }
{                                                                       }
{  Rev E: 09-12-88    Corrected resistance check algorithm.Tested       }
{                     operation  by testing against selected released   }
{                     tests.                                            }
{                                                                       }
{  Rev F: 11-07-88    Added warnings for .2 pin types if they are not   }
{                     four wire, and made $ signs acceptible in         }
{                     cable names.Functions were tested against         }
{                     selected source files.                            }
{                                                                       }
{  Rev G: 12-09-88    Added acceptance of tab characters.Also corrected }
{                     open test generation for DITMCO mode.             }
{                     Functions were tested against selected            }
{                     source files.                                     }
{                                                                       }
{  Rev H: 03-31-89    Added PIN=ZIF type for DALLAS usage.              }
{                     Fixed bug in Target_code_gen to generate          }
{                     'TO' nodes for every anonymous command.           }
{                     Functions were tested against selected            }
{                     source files.                                     }
{                                                                       }
{  Rev I: 09-30-89    Added capability for INCLUDE files. Modified      }
{                     Open_files.pas, OpenTT.pas and added file         }
{                     Open_Source.pas . Modified DITMCO Hookup logic    }
{                     to work correctly.Program was tested against      }
{                     selected test files.                              }
{                                                                       }
{  Rev J: 12-19-89    Added Offset command for 2-wire resistance checks }
{                                                                       }
{                                                                       }
{                                                                       }
{  Rev K: 04-09-90    Fixed a bug that caused a run-time error if RC    }
{                     checks were done before any XC command was done.  }
{                     No tests were impacted since this bug would       }
{                     always cause a crash in this case.                }
{                     Implemented ZIF option the way Corinth desired.   }
{                                                                       }
{  Rev L: 10-30-90    Modified ZIF option to allow pins in middle of    }
{                     connectors. Program was tested against selected   }
{                     test files.                                       }
{                                                                       }
{  Rev M: 03-14-91    Added relay energization, diodes, Zeners.         }
{                     This is another extensive rewrite. Some data      }
{                     Structures have been changed, Output statements   }
{                     have been cleaned out to make it more compatible  }
{                     across all platforms.  All I/O (well almost all)  }
{                     is now centralized. This is an attempt to clean   }
{                     house so that we can switch to graphics later.    }
{                     Program was extensively tested against several    }
{                     old source files.Added capability to specify      }
{                     number of pins for ZIF connectors.                }
{                                                                       }
{                                                                       }
{  Rev N: 11-06-91    Fixed DITMCO connector addressing problem         }
{                     Program was extensively tested against several    }
{                     old source files.                                 }
{  Rev O: 04-09-92    Fixed to allow modeling of TBs and Connectors in  }
{                     Packs.Fixed the logic in test_diode to give error }
{                     when no external access to diode is available.    }
{                                                                       }
{  Rev P: 05-28-92    This is a nicety revision. Allows doing FC,SC     }
{                     on packs with nicely skipping pins if they are    }
{                     ommited or not accessible. Also fixed so that     }
{                     Dwell time will be reported if resolution is      }
{                     not good enough.                                  }
{                                                                       }
{  Rev Q: 07-31-95    Fixed omit_node routine to 'not omit a node       }
{                     which is already HARD omitted.                    }
{                                                                       }
{***********************************************************************}


%include 'options.inc'


program wits(input,output);
                  
label 9999,8888;       
                  
                  
const             

 not_checked=0 ; checked  = 1;    
 No         =0 ; yes      = 1;                 


const             
      min_v = -30000;
      max_v =  30000;
      Bad_Address = max_v+1;
      Chassis_add = max_v+2; { must be the highest address }
      NO_NODE     = MaxInt;
      al = 16;    
      al2=2*al+1; 
      al3=3*al+1; 
      al4=4*al+1; 
      CLL=128;
      No_Pin=' ';
      MAX_DISPLAY_LINES=20;

const
      Ohms_Curr  = 0.001; {  1 milliamp }
      Wire_Imp   = 0.001; {  1 milliOhm Default}
      Contact_Imp= 0.010; { 10 milliOhm Default}
type

      charset   = set of char;                              
              
      name_str  = packed array[1..80] of char;
                  
      bit       = 0..1;
      alfa      = PACKED ARRAY[1..al] of char;
      alfa2     = PACKED ARRAY[1..al2] of char;
      alfa3     = PACKED ARRAY[1..al3] of char;
      alfa4     = PACKED ARRAY[1..al4] of char;
                                                         

      Sections  = (Parts_Section,Wires_Section,Tests_Section);

      adp_subdev_link=^adp_subdev;

      adp_subdev=  RECORD
                        CAB_Name,UUT_name:alfa;
                        address:integer;
                        Next:adp_subdev_link;
                   END;


      adp_dev_link= ^Adp_dev;
                                                
      adp_dev   =  RECORD
                        Name:alfa2;
                        address:integer      ;  { where do they start}
                        Tester_cnxs    ,        { how many connectors to tester}
                        UUT_cnxs:integer     ;  { how many connectors to UUT}
                        Tester_lo:integer    ;  { lowest pin on tester}
                        Tester_hi    :integer;  { highest pin on tester}
                        next:adp_dev_link    ;
                        Subdev,z_subdev:adp_subdev_link;
                        Hooked_up:boolean;
                        STRIP_type:Boolean;
                        ZIF_type:Boolean;
                   END;        

      adp       =  RECORD           
                     current,first:adp_dev_link;
                     open:bit;{is current device open to insertions?}
                     Next_available:Integer;{next available pin on tester}
                   END;

      Node_Types= (Bad_Type, Any_From,Cont_From,Diode_From,
                             Open_From,Power_From,
                             DC_I_From,AC_I_From,Meas_From,
                             Cont_To,Any_To,Open_To,Meas_To,
                             Power_To,Diode_To);

      Terminal_Types=(Wire_End,Pole,PS_HI,PS_REF,
                      Diode_Anode,Diode_Cathode,
                      Zener_Anode,Zener_Cathode,
                      Pin_Term);          

      Omit_Flag = (NO_OMIT,SOFT_OMIT,HARD_OMIT);
      Test_Req  = (No_Check,X_Check,O_Check,R_Check);
      Test_Reqs =  Set of Test_Req;                           
      Part_Types= (Bad_Part,Unknown,Strand,
                   Switch,Relay,
                   Diode,Zener,Resistor,Capacitor,PSupply,
                   Compound_Device,Cable
                   );

      Diode_Types=(Germanium,Silicon);

      Pin_Seq_Type= (Normal_Seq,FD_Seq);{ Old Al Iga style=FD_Seq}
      Mach_Type = (FACT_machine,DITMCO_660,DITMCO_9100);
      Cnx_Types = (Bad_Cnx,Wire,Contact,
                   Admittance,PN_Diode,Zener_Diode,Relay_Coil,P_Supply);
      Contact_States=(Opened,Closed);


                   



      units      = (BadUnit,       { Warning: Order of units is crucial }   
                    NoUnit,        { they are used as ranges in program } 
                    Mho,
                    ohm,Kohm,Mohm,
                    mvdc,vdc,Kvdc,
                    mvac,vac,Kvac,
                    uadc,madc,adc,
                    uaac,maac,aac,
                    mSec,Sec,Pct,
                    Ramp,ZeroCross,
                    mWatt,Watt);
                   
                  
      unit_set   = set of units;
                  


      Measurement= Record
                     HiLimit:Real;
                     Nominal:Real;
                     LoLimit:Real;
                     U:Units;
                   end;


      radians   = real;

      ac_volt   = record
                    mag:real;
                    faze:radians;
                  end;

      Cmplx_V   = Record
                    dc_part:real;
                    ac_part:ac_volt;
                  end;

      node_link = ^node;    
      node      = record
                    v        : integer;
                    next,prev: node_link   ;
                    other    : node_link   ;
                    y_cnx    : real;
                    tol      : real;
                    v_cnx    : measurement;
                    x_c      : bit;{ is this wire continuity tested?}
                    o_c      : bit;{ if disconnected was it tested? }
                    r_c      : bit;{ was the resistor tested? }
                    r_cc     : bit;{ or at least conditionally tested? }
                    a_w      : bit;{ is this an isthmus wire?      }
                    a_p      : bit;{   or    an isthmus path?      }
                    path     : bit;{ wires positive node lookahead}
                    rath     : bit;{ resistive positive node lookahead}
                    require  : test_req; { type of test_required }
                    j_d      : bit;{just disconn. contact not spanned yet}
                    terminal : Terminal_types;
                      case cnx : cnx_types of
                            wire:(Wire_Capacity:real);
                            Contact:(contact_state:Contact_states;
                                     contact_capacity:real;
                                    );
                            Admittance:(y_capacity:real);
                            PN_Diode:(p_rev:real);
                            Zener_Diode:();
                            Relay_Coil:();
                            P_Supply:();
                  end;         

      Pair_Link  = ^Pair;       

      Pair       = Record
                     x,y:integer;
                     next:Pair_Link;
                   End;

      state_link  = ^state;
      state     = Record             
                    desc:alfa;
                    Name:Alfa4;
                    cond:alfa;
                    oper:alfa;
                    ret :alfa;
                    c   :pair_link;
                    Next:State_Link;
                  end;

      rels       = (lt,le,eq,ge,gt);
      
      condition_link=^condition;
      condition = Record
                    desc:alfa;
                    term1,term2:integer; 
                    relop:rels;
                    v:real;u:units; 
                    cond_no:integer;
                    next:condition_link;
                  end;

      pin_link  = ^pin;
      pin       = record
                    pin   : alfa;
                    i_add : integer;{internal}
                    p     : pin_link;
                  end;
                  
      coil_link = ^coil;
      coil      =record
                   x,y:integer;
                   resistance:real;
                   next:coil_link;
                 end;

      dev_link  = ^dev;     
      dev       = record
                    dev       : alfa;
                    part_no   : alfa4;
                    dev_label : alfa4;
                    d         : dev_link;
                    Sub_dev   : dev_link;
                    Head      : dev_link;
                    p         : pin_link;
                    z_p       : pin_link;{sentinel location}
                    case part_type : part_types of
                     Strand:
                          ( 
                          );

                     Switch,relay:
                          ( curr_state  : alfa;
                            held        : boolean;
                            s           : state_link;
                            cond        : condition_link;
                            coils       : coil_link;
                          );

                     Resistor:
                          ( resistance : real;
                            tolerance  : real;
                            R_Wattage  : real;
                            max_i_ratio: real;
                          );

                     PSupply:
                          ( PS_voltage: real;
                            Hi_Pin    : Integer;
                            Ref_Pin   : Integer;
                            POS_RELAY : alfa;
                            NEG_RELAY : alfa;
                            PS_active : bit;
                          );

                     Diode:
                          ( I_rev      : real;
                            PRV        : real;
                            I_fwd      : real;
                            V_fwd      : real;
                            Vj_diode   : real;
                            R_diode    : real;
                            Diode_Type : Diode_Types; 
                          );

                     Zener:
                          ( V_zener    : real;
                            R_Zener    : real;
                            Vj_zener   : real;
                            I_Knee     : real;
                            Z_Wattage  : real;
                            Z_Tol      : real;
                          );

                  end;

      relax_modes=(ideal_wire,real_wire);

      { Test modes declare how the graph should be traversed to
        generate open tests. 
        SHORT_OPENS: we traverse short paths (span=0) and
                     jump over opens (span=1)
        RESISTOR_OPENS: we traverse short/resistive paths (span=0)
                     jump over opens (span=1)
        RESISTORS: we traverse short and resistive paths (span=0)
                     and generate resistor tests.
      
      }
                    

      Test_mode =(BAD_TEST,
                    SHORT_SHORTS,SHORT_OPENS,
                    RESIS_OPENS,RESISTORS);
                                        
      name      = record
                    dev : alfa;
                    pin : alfa;
                   end;
                  
      info_stats = (Not_defined ,
                    Being_used  ,
                    No_connex     ,
                    stubbed  ,
                    Disconnected);


      info_link=^info;
      info       = record  { con-pin no to address translation list }
                    Status   : info_stats;
                    dev      : dev_link;
                    pin      : pin_link;
                    m_add    : integer;
                    dfs_Visit: integer;
                    bfs_Visit: integer;
                    voltage  : measurement;
                    x_node   : integer;
                    r_node   : integer;
                    i_node   : integer;
                    bfs_Saw  : integer;
                    f_c      : bit;
                    s_c      : bit;
                    x_c      : bit; 
                    psbit    : bit;  {power supply island here ?}
                    forced   : bit;
                    source   : bit;
                    sense_b  : bit;
                    CNX_SAW  : cnx_types;
                    omit     : omit_flag;
                   end;                         
                  
                    
      str79     = PACKED ARRAY[1..79] of CHAR;
      str80     = PACKED ARRAY[1..80] of CHAR;
                  
      wish       = (con,dis,wrs,wires,Xxx,xci,xc,exit,n2a,i2n,opn,d,ds,da,
                    xfer,wc,hookup,rc,rci,fudge,
                    Show,apply,remove,rlx,bfs,v_del,strng,
                    cr,fnode,fc,fci,sc,sci,sw,kb,omit,unomit,pwrop,pwrcl,
                    stub,devs,conx,Ohms,
                    clear_all,no_command);

      wish_switch= record
                     switch  :char;
                     arg     :alfa;
                   end;
      wc_modes   = (wc_none,wc_first,wc_all);
                              
      cmnds      = (Cont_C,Cont_4,Open_C,
                    Insu_C,Insu_DC,Insu_AC,
                    Power_C,Meas_C,Meas_4,
                    Imped_2,Imped_4,
                    BAD_C);          
                  
                  
      phrase     = (NoPhrase,nodePhrase,commPhrase,EndSentence);
                             
      Params     = Record
                     case PT: Phrase of                                 
                        NodePhrase:( n:Name ;add:Integer;code:char);
                        CommPhrase:( comm :cmnds;
                                     relop:rels;
                                     v :real;u :units;
                                     v1:real;u1:units;
                                     v2:real;u2:units;
                                     v3:real;u3:units;
                                     v4:real;u4:units;);
                   end;


      Pin_Or_Skip= ( Pin_Data , Skip_Data ,Split_Pin);
      Jumper_case= (No_Jumper,Jumper_Begin,Jumpering);
      range      = Record
                     case RT: Pin_Or_Skip of
                        Pin_Data :(n:alfa;delta:Integer;
                                   jumper:jumper_case;
                                   sense :boolean;
                                   range:Boolean);
                        Skip_Data:(i:integer);
                        Split_Pin:();
                   End;
      range_pointer=^range_record;
      range_record= Record               
                      range_data: range;
                      Valid     : Boolean;
                      next      : range_pointer;
                    end;
      phrase_desc= record
                     case PD:phrase of
                        NodePhrase:(Address:integer);
                        CommPhrase:(comm:cmnds);
                   end;



      Cmnd_Link  = ^Cmnd;
      Cmnd       = Record
                     next   :Cmnd_link;  
                     pp     :Params;        
                   end;
      
                  
 type float_v    = record
                     a:alfa;      
                     a_right:alfa;
                     float:real;
                     flo_num:boolean;
                     split:boolean;
                   end;      
      strCLL     = packed array[1..CLL] of char;            

      line_link  = ^mess_line;
      mess_line  = record
                     mline:strCLL;
                     next :line_link;
                   end;

                  
      mess_rec   = record
                     line:strCLL;
                     length:integer;
                   end;




{ search queues }
      elementtype=Integer;
                             
      p_celltype = ^celltype;
      celltype   = record
                     element:elementtype;{node number}
                     next:p_celltype;
                   end;
      QUEUE      = record
                     front,rear,spare,temp,t_e:p_celltype;
                     count:integer;
                   end;

{ test queues }
      test_element=record
                     x,y,span:integer;
                     value:real;
                   end;

      p_test_cell = ^test_cell;
      test_cell   = record
                     element:test_element;
                     next:p_test_cell;
                   end;
      TESTQUEUE  = record
                     front,rear,spare,temp,t_e:p_test_cell;
                     count:integer;
                   end;                   
{ text queues }
      text_element=record
                     x:alfa;
                   end;

      p_text_cell = ^text_cell;
      text_cell   = record
                     element:text_element;
                     next:p_text_cell;
                   end;
      TEXTQUEUE  = record
                     front,rear,spare,temp,t_e:p_text_cell;
                   end;

{ search stacks }

      STACK       = record
                      head,z :p_celltype;
                      count  :integer;
                    end;


{ file structures }
      p_include_files = ^include_files;
      INCLUDE_FILES   = record
                        include_file:text;
                        include_name:name_str;
                        include_line:integer;
                        next:p_include_files;
                      end;

var               
       bfs_QUEUE,SUB_QUEUE           :QUEUE;
       RELAY_QUEUE                   :QUEUE;
       live_QUEUE                    :QUEUE; { for calc_circuit }
       PS_QUEUE                      :QUEUE; { for Check_Consumption}
       x_QUEUE,y_QUEUE               :QUEUE;
       temp_QUEUE                    :QUEUE; { general purpose  }

       OPENS_QUEUE,RESIS_QUEUE       :TESTQUEUE;
       RATIO_QUEUE                   :TESTQUEUE;

       PACK_QUEUE,PINS_QUEUE         :TEXTQUEUE;

       dfs_STACK                     :STACK;


       RELAY_COUNT:Integer;

       source_files,scratch : P_INCLUDE_FILES;

       uppers,lowers,alphabet,numerics:charset;
var               
       l1,l2,l3                   : integer;
       i,x                        : integer;
       V_min,V_Max,v              : integer;
       t_node,z_node              : node_link   ;
       z_info                     : info_link   ;
       cccc                       : cmnd_link;
       v1,v2                      : name;
       power_relay                : Integer;
       Pin_Seq_Mode               : Pin_Seq_Type;
       STRIP,ZIF                  : boolean;
       No_At_Field                : boolean; {'STRIP' or 'NORMAL 60' sets} 
       ZIF_Tester_Connections     : integer;
       PPC                        : integer; {Pins Per Connector }
       Target_Machine             : Mach_Type;
       wc_mode                    : wc_modes;                             
       add_wc                     : Boolean { used with wc_mode};
       From_Node                  : Integer { used in open_tests};
       XC_tested                  : Boolean { first time xci=xc command};
       Force_XC                   : Boolean { additive topology        };
       Initialized                : Boolean { circuit initialized ???  };
       Range_Pin,Range_List       : Range_pointer;

       source_name,Test_name,Error_Name,Wire_Name:name_str;
                   Test_File,Error_File,Wire_File:Text;
       Options:Name_Str;           

       Debug_On      :BOOLEAN;
       Extra_Info    :BOOLEAN;
       OverWrite     :BOOLEAN;
       Go_To_Keyboard   :BOOLEAN;
       Wire_List     :BOOLEAN;
       Wait_Manual   :BOOLEAN;           
       Line_No       :INTEGER;
          
{wishes and switches}                                 
       What       :Wish ;                 
       Old_wish   :Wish;
       Continue   :BOOLEAN;
       switches   :array [1..10] of wish_switch; 
       num_switches: integer;

       BAD_GUY    : Boolean; {Undesirable command has been used .}
                             { Integrity can not be guaranteed.}

       Done       :Boolean;

{for depth first search }
       dfs_Search :integer;

{for breadth first search }
       bfs_Search :integer;
       bfs_Cut    :integer; {used to avoid multiple visits}
{dfs_search when FS tests start }
       FS_Search_Start:integer;
       

{EXTENDED info cariables}
       Total_Iterations:Integer;
       Max_Iterations:Integer;
           
       f_dev,z_dev: dev_link;
       f_pin      : pin_link;
       Adapter    : ^adp;

{ mess_rec lines }
       display    : mess_rec;
       display_lines:integer;

       mesaj      : mess_rec;
       error_mesaj: mess_rec;
       annotation : mess_rec;
       annotate   : Boolean;
       test_line  : mess_rec;
       test_alfa  : alfa;


{ Test Parameters }    
       C_Params   : Params; { continuity        defaults }
       O_Params   : Params; { open       checks defaults }
       AC_Params  : Params; { AC insulation defaults     }
       DC_Params  : Params; { DC insulation defaults     }
       Diode_F    : Params; { Diode forward}
       Diode_R    : Params; { Diode reverse}
       Params_1   : Params; { for parsing }
       Params_2   : Params; { for parsing }
       Q_Value    : real; { default dwell time in secs }
       r2_offset  : real; { 2-wire ohms fudge offset }
       a1,a2,a3   : alfa;
       Big_A      : alfa;
       NULL       : alfa;
       NULL_2     : alfa2;
       long_name  : alfa4;
       a_alfa     : alfa;
       a_alfa2    : alfa2;
           
       Next_TB_Address,Curr_Add:Integer;
                  
       Console:Name_Str;
       PSUPPLIES:integer; { How many power supplies declared }
       DITMCO_param:alfa;
       Lowest_F_Address:Integer;           
{ -----------------large storage variables }
                  
%include 'ema_var_on.inc'
       adj                       : array[min_v..max_v] of node_link;
       inf                       : array[min_v..max_v] of info_link;
                  
%include 'ema_var_off.inc'
                  
{ ---------------------and the end of them }
                  
                  
                  
var { symbol reading variables }
 ch:char;         
 ll:integer        ;  { line length }
 cc:integer        ;  { character count }
 last_cc:integer   ;  { position of last character accepeted into 'a' }
 numero:integer    ;  { last number read}
 number:boolean    ;  { was last symbol an integer ?}
 float:real        ;  { last real number read }
 flo_num:boolean   ;  { was last symbol a real number?}
 z,z1,z2:float_v   ;  { used in get_parm routine }
 a:alfa            ;  { last symbol read }
 a_2:alfa2         ;  { last bigger symbol read }
 acceptable:charset;  { used in getsym for constructing a:alfa}
 unused:alfa       ;  { unused symbol passed back from routines }
 symbol:boolean    ;  { is the last symbol read a SYMBOL (or special char)}
 a_right:alfa      ;  { second half of a if a is floating//alfa form}
 split:boolean     ;  { true if a is in form floating//alfa}
 together:boolean  ;  { if last field and this field were butted together}
 line,full_line:packed array[1..150] of char;

var
 Section        : Sections;
 Section_Message:str79;{ used as error messages for each section} 
 err_mess:str79    ;  { primary error message }
 err_p1,err_p2:alfa;  { additional error messages }
 err_p3       :alfa;  {   "                       }
 err_Ps:integer    ;  { number of error parameters}
 kk:integer        ;
 Error_Count,warn_count:Integer;
 OK:boolean         ;
 First_Cont :Boolean; { used to determine if C command should be emitted}     
 First_Open :Boolean; { used to determine if C command should be emitted}     
 boole      :Boolean;                         
 To_Keyboard  :boolean;           
 From_Keyboard:boolean;
 Error_Option:Boolean;
 allow:omit_flag;                 

{ Instrument info }
 OHMS_HI,OHMS_LO:Integer;

{ Test Generation Control variables }
 MAX_RESISTOR_ANNOTATE:integer;


{********************************************************************}
%include 'get_value.inc'
                  
{********************************************************************}
function min(i1,i2:integer):integer;
begin if i1<i2 then min:=i1 else min:=i2; end;

{********************************************************************}
function max(i1,i2:integer):integer;
begin if i1<i2 then max:=i2 else max:=i1; end;

{**********************************************************************}
procedure write_error_c(c:char);
    
  begin
      case Error_Option of
         false: write(OUTPUT,c);
         true : write(error_file,c);
      end;
  end;

{**********************************************************************}
procedure write_error_alfa(e:alfa);
 var i:integer;
  begin
      for i:=1 to al do write_error_c(e[i]);
  end;
{**********************************************************************}
procedure write_error_ln;
 begin
      case Error_Option of
         false : writeln(OUTPUT);
         true  : writeln(error_file);
      end;
 end;
{***********************************************************************}
 procedure error(p:integer);
    var i,el:integer;

    begin         
      if error_option then begin
        writeln(Error_File,'Error at line:',Line_no);
        for i:=1 to ll do Write(Error_File,Line[i]);writeln(Error_File);
        end
      else begin
        if not from_keyboard then begin
          writeln(Output,'Error at line:',Line_no);
        end;
        for i:=1 to ll do Write(Output    ,Line[i]);writeln(Output);
      end;
      el:=0;      
      for i:=1 to 79 do if Err_Mess[i]<>' ' then el:=i;             
      for i:=1 to el do write_error_c(Err_Mess[i]);
      if p>0 then write_error_alfa(err_p1);
      if p>1 then write_error_alfa(err_p2);
      if p>2 then write_error_alfa(err_p3);
      write_error_ln;
      OK:=false;Error_Count:=Error_Count+1;
    end;          

{***********************************************************************}
 procedure warn(p:integer);
    var i,el:integer;
    begin         
      if error_option then begin
        writeln(Error_File,'Warning at line:',Line_no);
        for i:=1 to ll do Write(Error_File,Line[i]);writeln(Error_File);
        end
      else begin
        if not from_keyboard then begin
          writeln(Output,'Warning at line:',Line_no);
        end;
        for i:=1 to ll do Write(Output    ,Line[i]);writeln(Output);
      end;
      el:=0;      
      for i:=1 to 79 do if Err_Mess[i]<>' ' then el:=i;             
      for i:=1 to el do write_error_c(Err_Mess[i]);
      if p>0 then write_error_alfa(err_p1);
      if p>1 then write_error_alfa(err_p2);
      if p>2 then write_error_alfa(err_p3);
      write_error_ln;
      Warn_Count:=Warn_Count+1;
    end;          

{********************************************************************}
                  
function Mach_Add(Int_Add:Integer):Integer;
                  
Begin             
    Mach_Add:=inf[Int_Add]^.m_add;
end;              
             
                  
{*******************************************************************}
function Source_to_Int_Add(Address:Integer):integer;
        
begin
      case target_machine of
        FACT_Machine:
          begin
          end;         
        DITMCO_660,DITMCO_9100:
          begin
            If Address=0 then begin
              err_mess:=' Pin 0 is illegal for DITMCO ';
              error(0);
            end;
          end;
      OTHERWISE
            err_mess:=' BUG! in source_to_int_Add';
            error(0);
      end;
      Source_To_Int_Add:=Address;
end;

{*******************************************************************}
function legit_Source_Add(Source_Add:Integer):Boolean;
Begin               
   if (Source_Add<max_v) and (Source_Add>min_v) then begin
     legit_source_add:=true;
     end
   else begin
     legit_source_add:=false;
   end;
end;                           
                  
{***************************************************************}
 function Alfa_Length(a:alfa):Integer;
 var l:integer;   
 begin            
  l:=al;          
  while (l>0) and (a[l]=' ') do l:=l-1;
  alfa_length:=l; 
 end;             

{***************************************************************}
 function Alfa2_Length(a:alfa2):Integer;
 var l:integer;                        
 begin            
  l:=al2;          
  while (l>0) and (a[l]=' ') do l:=l-1;
  alfa2_length:=l; 
 end;             

{***************************************************************}
 function Alfa3_Length(a:alfa3):Integer;
 var l:integer;   
 begin            
  l:=al3;          
  while (l>0) and (a[l]=' ') do l:=l-1;
  alfa3_length:=l; 
 end;             

{***************************************************************}
 function Alfa4_Length(a:alfa4):Integer;
 var l:integer;   
 begin            
  l:=al4;          
  while (l>0) and (a[l]=' ') do l:=l-1;
  alfa4_length:=l; 
 end;             
                  
                  
{********************************************************************}
 procedure getch; 
    begin         
      if cc<ll then begin
        cc:=cc+1;ch:=line[cc]
        end       
      else begin  
        ch:=' '   
      end;        
    end {getch};  
{********************************************************************}
function endfile:BOOLEAN;

 var file_end:boolean;

 begin
    if From_Keyboard then begin
      file_end:=eof(Input);
      end      
    else if eof(source_files^.include_file) then begin
      if source_files^.next<>NIL then begin
        scratch:=source_files;
        source_files:=source_files^.next;
        dispose(scratch);
        Line_No:=source_Files^.Include_Line;
        file_end:=endfile;
        end
      else begin
        file_end:=true;
      end;
      end
    else begin
      file_end:=false;
    end;
    endfile:=file_end;
 end;    
             


{********************************************************************}
 procedure get_line;
 var blank_line,comment_field:boolean;
     lc:char;     

   function endline:BOOLEAN;

     begin
       if From_Keyboard then begin
         endline:=eoln(Input);
         end
       else begin
         endline:=eoln(source_files^.include_file);
       end;      
     end;    

   function line_char:char;
    var lc:char;
    begin      
      if From_Keyboard then begin
        read(Input,lc);
        end
      else begin
        read(Source_files^.include_file,lc);
      end;
      line_char:=lc;
    end;

    begin
      OK:=true;
      repeat
        ll:=0;cc:=0;
        blank_line:=true;comment_field:=false;
        if endfile then begin
          err_mess:=Section_Message;
          error(0);goto 9999;
          end
        else begin
          Line_No:=Line_No+1;
          while not endline do begin
            ll:=ll+1;lc:=line_char;
            if lc='!' then comment_field:=true;
            if lc=chr(9) then lc:=' '; { Horizontal Tab }
            if comment_field then line[ll]:=' ' else line[ll]:=lc;
            if line[ll]<>' ' then blank_line:=false;
            full_line[ll]:=lc;
          end;
          ll:=ll+1;line[ll]:=line_char;
          full_line[ll]:=line[ll];
          getch;
        end;    
      until (not blank_line) or From_Keyboard;
    end {get_line };
{************************************************************************}
 procedure skip_blanks;
  { skips blanks and sets together false (since it skipped blanks) }
    begin         
      while (cc<ll) and (ch=' ') do begin together:=false;getch;end;
    end;          

{********************************************************************}
procedure upshift(var a:alfa);
           
var i:integer;    
begin             
  for i:=1 to al do
    if a[i] in ['a'..'z'] then a[i]:=chr(Ord('A')+Ord(a[i])-Ord('a'));
end;              
                  

{**********************************************************************}
function UpperAlfa(a:alfa):alfa;
var b:alfa;       
begin             
    b:=a;         
    upshift(b);   
    UpperAlfa:=b; 
end;              
                  
                  
{************************************************************************}
 procedure getsym;
 { This procedure will get next field from the source file.       }
 { Variables set are:                                             }
 {                                                                }
 {      Symbol:    Was the last field read a symbol?              }
 {                 (defined by all characters in ACCEPTABLE)      }
 {      Together:  Set if this field read and next field are      }
 {                 butted together.                               }
 {                 e.g.                                           }
 {                                                                }
 {                 \10    : Together=true;                        }
 {                 \ 10   : Together=false;                       }
 {                 \,10   : Together=false;                       }
 {      number:    true if this field is an integer               }
 {                 ( note: a null field is an integer )           }
 {      numero:    value of this integer                          }
 {                                                                }
 {      flo_num:   true if this field is a floating number        }
 {      float:     value of this floating number                  }
 {                                                                }
 {                                                                }
 {                                                                }


                  
 var k,ks,num_sign:integer;
     flo_mul,flo_1:real;
     nomore:boolean;             
  begin {getsym}  
    kk:=al;numero:=0;
    skip_blanks; { 0 or more spaces }                               
                  
    k:=0;ks:=0;a:=' ';a_right:=' ';a_2:=' ';
    number:=true;numero:=0;symbol:=false;split:=true;together:=true;
    flo_num:=true;float:=0.0;num_sign:=1;flo_mul:=1.0;
    if ch in Acceptable then begin
      symbol:=true;
      repeat
        nomore:=(cc>=ll);      
        if ch in numerics then begin
          end     
        else if (ch='-') and (k=0) then begin
          num_sign:=-1;
          end     
        else if (ch='+') and (k=0) then begin
          num_sign:=1;
          end     
        else if (ch='.') and number then begin
          number:=false;
          end     
        else begin
          flo_num:=false;
          number:=false;
        end;      
        if k<al then begin
          k:=k+1; 
          a[k]:=ch;a_2[k]:=ch;
                  
          if number and (ch in ['0'..'9']) then begin
            numero:=10*numero + (ord(ch)-ord('0'));
            float:=numero;
            end   
          else if flo_num and (ch in ['0'..'9']) then begin
            flo_mul:=flo_mul/10.0;{since number is false we saw a '.' }
            flo_1:=(ord(ch)-ord('0'));
            float:=float+ flo_1*flo_mul
            end   
          else if flo_num and (ch='.') then begin
            end   
          else if split then begin
            if k=1 then begin
              split:=false;
              end 
            else if ch in alphabet+['%'] then begin
              ks:=ks+1;
              a_right[ks]:=ch;
              end 
            else begin
              split:=false;
            end;  
          end;    
          end 
        else if k<al2 then begin
          k:=k+1;
          a_2[k]:=ch;
          number:=false;symbol:=false;flo_num:=false;
        end;      
        getch;    
      until not (ch in acceptable) or nomore;
      last_cc:=cc;
      end         
    else if ch=',' then begin
      k:=0;Together:=false;
      end         
    else begin    
      number:=false;flo_num:=false;split:=false;
      k:=1;a[1]:=ch;
      getch;      
    end;          
                  
    if k>=kk then begin
      kk:=k
      end 
    else begin    
      repeat      
        a[kk]:=' ';
        kk:=kk-1; 
      until kk=k; 
    end;          
    skip_blanks;  
    if ch=',' then begin
      Together:=false;{ trailing comma}
      getch;     { followed by 0 or 1 comma }
      skip_blanks;   { followed by 0 or more spaces}
      end
    else if ch=' ' then begin { end of line }
      together:=false;
    end;          
    numero:=numero*num_sign;
    float:=float*num_sign;
    Big_a:=upperalfa(a);
  end;            

{*************************************************************************}
  procedure getnext;
      begin 
         if cc>=ll then begin 
           get_line;getsym;
           if a=' ' then getsym;{skip leading blank field if any}
           end
         else begin
           getsym; 
         end;
         if (a=' ') and (cc>=ll) then begin 
           get_line;getsym;
         end;
      end;
{************************************************************************}
procedure open_source(src_name:name_str);
var src:^text;
    source_name:name_str;
    include:P_INCLUDE_FILES;

begin
    source_name:=src_name;
    new(include);
    include^.next:=Source_files;
    Source_files^.Include_Line:=Line_No;
    Source_files:=include;

%include 'open_source.inc' 
    reset(source_files^.Include_file);    
end;


{*************************************************************************}
  procedure get_line_sym;

      var i:integer;file_name:name_str;
      
      begin
         get_line;getnext;
         if Big_a='INCLUDE' then begin
           getsym;
           if a_2<>' ' then begin
             file_name:=' ';
             for i:=1 to al2 do file_name[i]:=a_2[i];
             open_source(file_name);
             end
           else begin
             err_mess:='NO INCLUDE FILE NAME SPECIFIED';
             error(0);
           end;
           get_line_sym;
         end;
      end;

{*************************************************************************}
  procedure getnext_with(s:charset);
  var acceptable_save:charset;
  begin
      acceptable_save:=acceptable;
      acceptable:=acceptable+s;
      getnext;
      acceptable:=acceptable_save;
  end;

{*************************************************************************}
  procedure getsym_with(s:charset);
  var acceptable_save:charset;
  begin
      acceptable_save:=acceptable;
      acceptable:=acceptable+s;
      getsym;
      acceptable:=acceptable_save;
  end;

{*************************************************************************}
  function number_to_alfa(n,length:integer):alfa;
                  
  var             
      f,g:alfa;   
      p,m:integer;
  begin           
      f:=' ' ; g:=' ' ;
      m:=n;       
      p:=0;       
      repeat      
        p:=p+1;        
        f[p]:=chr((m mod 10)+ord('0'));
        m:=m div 10;
      until (m=0) or (p=al);
      if (n<0) and (p<al) then begin p:=p+1;f[p]:='-';end;
      while p<length do begin p:=p+1;f[p]:='0' end;
      for m:=1 to p do g[p-m+1]:=f[m];
      number_to_alfa:=g;
  end;            

{************************************************************************}
  function good_integer(f:alfa;p1,p2:integer):boolean;
  var p:integer;
  begin           
    good_integer:=true;              
    for p:=p1 to p2 do begin
      if NOT(f[p] in ['0'..'9']) then good_integer:=false;
    end;          
  end;            
{************************************************************************}
    function index(f:alfa;c:char):integer;
    var i,p:integer;               

    begin 
        p:=0;i:=1;
        while (p=0) and (i<=al) do if f[i]=c then p:=i else i:=i+1;
        index:=p;
    end;
{************************************************************************}
    function extract_alfa(f:alfa;l1,l2:integer):alfa;
    var i:integer;f1:alfa;
    begin
        f1:=' '; {ensure trailing blanks }
        for i:=1 to l2-l1+1 do f1[i]:=f[l1-1+i];
        extract_alfa:=f1;
    end;
{************************************************************************}
    function extract_to_dot(f:alfa;l1:integer):alfa;
    var f1:alfa;
        i,j:integer;
      begin
         f1:=' ';
         for i:=l1 to al do begin
           f1[i-l1+1]:=f[i];
         end;
         j:=index(f1,'.');
         if j=0 then j:=al+1;
         extract_to_dot:=extract_alfa(f1,l1,j-1);
      end;   
                  
{************************************************************************}
  function alfa_to_number(f:alfa;p1,p2:integer):integer;
  var p,pp1,n,sign:integer;
  begin           
    n:=0;         
    if f[p1]='-' then begin
      pp1:=p1+1;sign:=-1;
      end         
    else begin    
      pp1:=p1;sign:=1;
    end;          
                  
    for p:=pp1 to p2 do begin
      n:=n*10+ord(f[p])-ord('0');
    end;          
    alfa_to_number:=sign*n;
  end;            
                  
{******************************************************************}

function leading_number(a:alfa;s:integer;e:integer):integer;

var  p,i:integer;

begin
    p:=s-1;
    for i:=s to e do begin
      if (p=(i-1)) and (a[i] in ['0'..'9']) then p:=i;
    end;
    leading_number:=alfa_to_number(a,s,p);
end;          

{******************************************************************}

function trailing_letter(a:alfa;s:integer;e:integer):integer;

var  p,i:integer;

begin
    p:=s-1;
    for i:=s to e do begin
      if (p=(i-1)) and (a[i] in ['0'..'9']) then p:=i;
    end;
    if e=p+1 then begin
      trailing_letter:=Ord(a[e])-Ord('A')+1;
      end
    else begin
      trailing_letter:=0;
    end;
end;          

{******************************************************************}

function trailing_number(a:alfa):integer;

var  p,i,s,e:integer;
     done:boolean;

begin
    s:=1;e:=alfa_length(a);
    p:=e+1;done:=false;
    for i:=e downto s do begin
      if (not done and (a[i] in ['0'..'9'])) then begin
        p:=i;
        end
      else if a[i]<>' ' then begin
        done:=true;
      end;
    end;
    if p>e then begin
      trailing_number:=0;
      end
    else begin    
      trailing_number:=alfa_to_number(a,p,e);
    end;
end;          

                  
{******************************************************************}
function Alpha_Pos(Pin:Alfa):integer;
var               
    i,l:integer;  
    pin_1:alfa;   
begin             
  l:=alfa_length(Pin);
  if (l=1)then begin
    if (Pin[1] in Uppers) then begin
      Alpha_Pos:=Ord(Pin[1])-Ord('A')+1;
      end         
    else if (Pin[1] in lowers) then begin
      Alpha_Pos:=Ord(Pin[1])-Ord('a')+26+1;
      end         
    else begin    
      Alpha_Pos:=0;
    end;          
    end { single character}
  else if (l=2) and (pin[1]=pin[2]) then begin
    if pin[1] in alphabet then begin
      for i:=2 to al do pin_1[i-1]:=pin[i];pin_1[al]:=' ';
      Alpha_pos:=Alpha_Pos(Pin_1)+52;
      end         
    else begin    
      Alpha_Pos:=0;
    end;          
    end           
  else if (l=2) and (pin[1] = '*') then begin
    if (Pin[2] in uppers) then begin
      pin_1:=' ';pin_1[1]:=pin[2];
      Alpha_pos:=Alpha_Pos(Pin_1)+26;
      end         
    else begin    
      Alpha_Pos:=0;
    end;          
    end { *A-*Z }   
  else begin      
    Alpha_Pos:=0; 
  end;            
end;              
                  
{******************************************************************}
  function reg_alpha_pin(n:integer;low_flag:boolean):alfa;
  var f:alfa;     
  begin           
    f:=' ';       
    if (n>=1) and (n<=26) then begin
      f[1]:=chr(ord('A')-1+n)
      end         
    else if (n>=27) and (n<=52) then begin
      if low_flag then begin
        f[1]:=chr(ord('a')-1+n-26)
        end       
      else begin    
        f[1]:='*';
        f[2]:=chr(ord('A')-1+n-26);
      end;        
      end         
    else if (n<=78) then begin
      f[1]:=chr(ord('A')-1+n-52);
      f[2]:=f[1]; 
      end         
    else if (n<=104) then begin
      f[1]:=chr(ord('a')-1+n-78);
      f[2]:=f[1]; 
      end         
    else begin    
      f:='?'      
    end;          
    reg_alpha_pin:=f;
  end;            
                  
{******************************************************************}
  Function Next_Pin(f1,f2:Alfa;l_ex:integer):alfa;
  const PAD=FALSE;                       
  var             
     l1,l2,lv,p,p11,p12,p21,p22:integer;
     length:integer;
     i,n,n1,n2:integer;
     f,fv:alfa;   
     numeric:array[1..2] of boolean;
     left_equal,right_equal,low_flag,more:boolean;
  begin           
                  
   l1:=Alfa_Length(f1);
   l2:=Alfa_Length(f2);
   OK:=true;      
   left_equal:=true;right_equal:=true;low_flag:=false;
   p11:=1;p12:=l1;p21:=1;p22:=l2;
   repeat         
     if left_equal then begin
       if f1[p11]=f2[p21] then begin
         if (p11<l1) and (p21<l2) then {empty} else left_equal:=false;
         if left_equal then begin p11:=p11+1;p21:=p21+1;end;
         end      
       else begin 
         left_equal:=false;
       end;       
     end {left_equal};
     if right_equal then begin
       if f1[p12]=f2[p22] then begin
         if (p12>1) and (p22>1) then {empty} else  right_equal:=false;
         if right_equal then begin p12:=p12-1;p22:=p22-1;end;
         end      
       else begin 
         right_equal:=false;
       end;       
     end{right_equal};
   until (not left_equal) and (not right_equal);
                  
   numeric[1]:=true;
   for i:=p11 to p12 do numeric[1]:=numeric[1] and (f1[i] in numerics);
                  
   numeric[2]:=true;
   for i:=p21 to p22 do numeric[2]:=numeric[2] and (f2[i] in numerics);
                  
   if numeric[1] and numeric[2] then begin
                  
     repeat       
       if p11>1 then more := (f1[p11-1] in numerics) else more:=false;
       if more then p11:=p11-1;
     until not more;
     repeat       
       if p12<l1 then more := (f1[p12+1] in numerics) else more:=false;
       if more then p12:=p12+1;
     until not more;
                  
     repeat       
       if p21>1 then more := (f2[p21-1] in numerics) else more:=false;
       if more then p21:=p21-1;
     until not more;
     repeat       
       if p22<l2 then more := (f2[p22+1] in numerics) else more:=false;
       if more then p22:=p22+1;                              
     until not more;
                  
     n1:=alfa_to_number(f1,p11,p12);
     n2:=alfa_to_number(f2,p21,p22);
                  
     if (n1<n2) then begin 
       n:=n1+1;
       end
     else if (n1>n2)then begin
       n:=n1-1;
       end 
     else begin { they are equal so make sure representation is same}
       if f1=f2 then begin
         n:=n2;
         end
       else begin
         err_mess:='Pins are NOT a valid range:';
         err_p1:=f1;
         err_p2:=f2;
         error(2);
         n:=n2;{ ensure orderly exit if program logic is wrong }
       end;
     end;             

     if PAD then begin
       length:=max(p22-p21,p12-p11)+1;
       if l_ex>length then length:=l_ex;
       end        
     else begin   
       length:=min(p12-p11,p22-p21)+1;{Changed to make linda happy} 
     end;         
                  
     fv:=number_to_alfa(n,length);
     end { numeric[1] and numeric[2] }
                  
   else { one of them contains a nonnumeric } begin
     if ((p12-p11)=(p22-p21)) then begin
       low_flag:=(f1[p11] in lowers) or (f2[p21] in lowers);
       if ((f1[p11]='*') or (f2[p21]='*')) and ((p12-p11)=0) then begin
         if p12<al then p12:=p12+1;
         if p22<al then p22:=p22+1;
       end;       
       end{same length}
     else { not same length } begin
       low_flag:=(f1[p11] in lowers) or (f2[p21] in lowers);
       if (p12-p11)=0 then begin
         if (f1[p11]='*')then begin
           if p12<al then p12:=p12+1;
           if p22<al then p22:=p22+1;
         end;                                
         end      
       else if(p22-p21)=0 then begin
         if (f2[p11]='*') then begin
           if p12<al then p12:=p12+1;
           if p22<al then p22:=p22+1;
         end;     
       end;       
     end;         
                  
     f:=' ';for i:=p11 to p12 do f[i-p11+1]:=f1[i];
     n1:=alpha_pos(f);
                  
     f:=' ';for i:=p21 to p22 do f[i-p21+1]:=f2[i];
     n2:=alpha_pos(f);
                  
     if (n1>0) and (n2>0) then begin
       if n1<n2 then n:=n1+1 else if n1>n2 then n:=n1-1 else n:=n2;
       fv:=reg_alpha_pin(n,low_flag);
       end{n1>0 and n2>0}
     else begin   
       err_mess:=' Bad range:';
       err_p1:=f1;err_p2:=f2;
       error(2);  
     end {n1<=0 or n2<=0};
                  
   end{ neither is numeric };
                  
   if OK then begin
     lv:=alfa_length(fv);
     p:=0;        
     f:=' ';      
                  
     if (n=n2) and (l_ex=0) then { ensure that range terminates }
       next_pin:=f2
     else begin   
       for i:=  1   to p11-1 do begin p:=p+1;f[p]:=f1[i] end;
       for i:=  1   to   lv  do begin p:=p+1;f[p]:=fv[i] end;
       for i:=p12+1 to   l1  do begin p:=p+1;f[p]:=f1[i] end;
       next_pin:=f;
     end;         
     end          
   else begin     
     next_pin:=f2;
   end;           
 end{ procedure next_pin} ;

{***************************************************************************}
function Mach_Xlate(Int_Add:Integer):Integer;
                  
var Mach_Pin,Mach_Conn,i:Integer;
Begin             
     Case Target_Machine of
       
       FACT_machine:
         begin
           If Int_Add < 0 then begin
             Mach_Xlate:=Int_Add;
             end
           Else if Int_Add=0 then begin
             Mach_Xlate:=Chassis_Add;{ do this to fool X checks}
             end       
           Else begin   
             CASE Pin_Seq_Mode of
               FD_Seq    : Begin             
                             {  1 ---> 10011  }
                             {  2 ---> 10012  }
                             { .............  }
                             { BUT            }
                             {100 ---> 10010  }
                            
                             If ( Int_Add MOD 100 ) <> 0 then begin
                               I:=Int_Add;             
                               end
                             else begin
                               I:=Int_Add-100;
                             end;
                             Mach_Pin := (I MOD 60) +10;
                             Mach_Conn:= (I div 60)*100+10000;
                             Mach_Xlate :=Mach_Conn+Mach_Pin;
                           end;

               Normal_Seq: Begin
                             {  1 ---> 10010  }
                             {  2 ---> 10011  }
                             {    etc..       }
                             I:=Int_Add-1;
                             Mach_Pin := (I MOD 60) +10;
                             Mach_Conn:= (I div 60)*100+10000;
                             Mach_Xlate :=Mach_Conn+Mach_Pin;
                           end;
               OTHERWISE
                 err_mess:=' Internal error. wrong pin_seq.REPORT this:';
                 err_p1:=' HELP HELP';error(1);GOTO 9999;

             end;

           end;
         end;                                           

       DITMCO_660,DITMCO_9100:
         Begin
           If Int_Add < 0 then begin
             Mach_Xlate:=Int_Add;
             end
           Else if Int_Add=0 then begin
             Mach_Xlate:=Chassis_Add;{ do this to fool X checks}
             end       
           Else begin   
             CASE Pin_Seq_Mode of
               FD_Seq    : Begin
                             {  1 ---> 00001  }
                             {  2 ---> 00002  }
                             { .............  }
                             { 49 ---> 00049  }
                             { 50 ---> 00100  }
                             { 51 ---> 00101  }
                             { .............  }
                             { 99 ---> 00149  }
                             {100 ---> 00000  }
                            
                             If ( Int_Add MOD 100 ) <> 0 then begin
                               I:=(Int_Add MOD 50) + (Int_Add DIV 50)*100;
                               end
                             else begin
                               I:=(Int_Add-100)*2;
                             end;
                             Mach_Xlate :=I;
                           end;

               Normal_Seq: Begin
                             {  1 ---> 00000  }
                             {  2 ---> 00001  }
                             {    etc..       }
                             Int_Add:=Int_Add-1;
                             I:=(Int_Add MOD 50) + (Int_Add DIV 50)*100;
                             Mach_Xlate :=I;
                           end;
               OTHERWISE      
                 err_mess:=' Internal error. wrong pin_seq.REPORT this:';
                 err_p1:=' HELP HELP';error(1);GOTO 9999;

             end;
           end;
         end;
       OTHERWISE
         begin
             err_mess:='BUG! BUG! BAD target machine in MACH_XLATE';
             error(0);Mach_Xlate:=BAD_Address;;
         end;
     end;
end;                                 
                  
{*********************************************************************}
 function next_address(s_add,Increment:integer):integer;
 var I:Integer;   
 begin
         If s_add<=-1 then begin
           if s_add>min_v then begin
             Next_Address:=s_add-1;
             end
           else begin    
             Err_Mess:=' Out of terminal block address space';
             error(0);goto 9999;
           end;
           end         
         else begin        
           if s_add<max_v then begin
             Next_Address:=s_add+Increment;
             end
           else begin
             Err_Mess:=' Illegal pin address specified';
             error(0);
             next_address:=Bad_Address;
           end;
         end;
 end;             

{********************************************************************}
  Procedure Get_Next_Address(var Curr_Add:integer;
                                    delta:integer;
                              var first_pin:boolean);
   begin        
        if First_pin then begin
          First_pin:=false;
          end
        else begin
          Curr_Add:=Next_Address(Curr_Add,delta);
        end;                  
        if curr_add<0 then Next_TB_Address:=Curr_Add;
   end; 


{********************************************************************}
function nom_meas(x:real;u:units):measurement;
var z:measurement;
begin
    z.HiLimit:=x;
    z.Nominal:=x;
    z.LoLimit:=x;
    z.u:=u;
    nom_meas:=z;
end;                            
                  
{********************************************************************}
 procedure new_info(address:integer);
  var q:info_link;                                 
  begin     
     new(q);      
     with q^ do begin
       f_c:=not_checked;s_c:=not_checked;x_c:=not_checked;omit:=NO_OMIT;
       dfs_visit:=dfs_Search;bfs_visit:=bfs_Search;
       x_node:=address;r_node:=Address;i_node:=Address;psbit:=no;
       Status:=Not_defined;
       sense_b:=NO;forced:=NO;source:=NO;
     end;         
     inf[address]:=q;
  end;

{********************************************************************}
 procedure new_node(var t_node:node_link);
                                  
  begin
     new(t_node);      
     with t_node^ do begin
       o_c:=NO;x_c:=NO;r_c:=NO;r_cc:=NO;j_d:=NO;a_w:=NO;path:=NO;
       tol:=0.0;v_cnx:=Nom_meas(0.0,VDC);
     end;         
  end;

{********************************************************************}
 procedure Init_Adj(address:integer);
 var i:integer;   
     q:info_link; 
 begin                          
                  
   if address>V_Max then begin
     for i:= V_Max+1 to address do adj[i]:=z_node;
     for i:= V_Max+1 to address do inf[i]:=z_info;
     V_Max:=address;
   end;           
                  
   if address<V_min then begin
     for i:= V_min-1 downto address do adj[i]:=z_node;
     for i:= V_min-1 downto address do inf[i]:=z_info;
     V_min:=address;
   end;           
                  
   if inf[address]=z_info then begin
     new_info(address);
     end          
   else if inf[address]^.status in[No_connex,stubbed ] then begin
     err_mess:=' Address belongs to a previous cable';
     error(0);
     end
   else begin     
     err_mess:=' Address already in use by:';
     err_p1:=inf[address]^.dev^.dev;
     err_p2:=inf[address]^.pin^.pin;
     error(2);    
   end;           
                  
 end;             
           
{********************************************************************}
 procedure insert_node(Conn_Name:alfa;Pin_Name:alfa;s_Address:integer);
   var              
      d:dev_link;                
      p:pin_link;                
      Int,l:Integer;
      x:alfa;
      fail:boolean;  


  procedure Insert(     Head:dev_link;
                   var f_dev:dev_link;Conn_Name:alfa;Inserting:Boolean);       
   begin
       d:=f_dev;fail:=false;  
       z_dev^.dev:=Conn_Name { la Sentinella };
       while d^.dev<>Conn_Name do d:=d^.d;
       if d=z_dev then { not on list so insert connector } Begin
         Init_Adj(Int);                           
         if OK then begin
           New(d)    ;d^.dev:=Conn_Name;d^.d:=f_dev;f_dev:=d;d^.Head:=Head;
           d^.Sub_Dev:=Z_Dev;
           new(p);p^.pin:=Pin_Name;d^.p:=p;
           d^.p^.i_add:=Int;
           new(d^.z_p);d^.p^.p:=d^.z_p;{ sentinel location }
         end;         
         end          
       else { connector exists check to see if pin does } begin
         p:=d^.p;
         d^.z_p^.pin:=Pin_Name;
         while p^.pin<>Pin_Name do p:=p^.p;
         if p=d^.z_p then { pin does not exist (found sentinel)}begin
           If Inserting then begin
             Init_Adj(Int);
             if OK then begin
               new(d^.z_p^.p);d^.z_p^.i_add:=Int;d^.z_p:=d^.z_p^.p;
               if adapter<>NIL then begin{ remember that this pin is used}
                 if adapter^.open=YES then begin
                   with adapter^.current^ do begin
                     if Int>tester_hi then tester_hi:=Int;
                     if Int<tester_lo then tester_lo:=Int;
                   end;
                 end;
               end;
             end;       
             end
           else { pin does not exist} begin
             Err_Mess:='Pin does not  exist:';
             Err_P1:=Conn_Name;Err_P2:=Pin_Name;
             Err_Ps:=2;fail:=true;  
           end;
           end        
         else { pin already exists } begin
           Err_Mess:='Pin already exists:';
           Err_P1:=Conn_Name;Err_P2:=Pin_Name;
           Err_Ps:=2;fail:=true;  
         end;         
       end;           
                  
   end;             

begin                    
     Int:=Source_To_Int_Add(s_Address);
     if Int>max_v then begin
       err_mess:=' Pin address is too high';error(0);
       end
     else if Int<min_v then begin
       err_mess:=' Internal address is out of range ';error(0);
       end
     else begin
       x:=Conn_name;l:=index(Conn_Name,'.');
       insert(NIL,f_dev,extract_to_dot(x,1),l=0);
       while l>0 do begin
         x:=extract_alfa(x,l+1,al);
         l:=index(x,'.');
         insert(d,d^.sub_dev,extract_to_dot(x,1),l=0);
       end {while};

       if fail then begin
         error(Err_Ps);
         end
       else begin
         with inf[int]^ do begin
           dev:=d;pin:=p;m_add:=Mach_Xlate(int);Status:=Being_Used;
         end;
       end;           

     end;
end;

{********************************************************************}
 function  next_dev_pin(var V:name):name;
 var              
    d_ptr,d:dev_link;
    p_ptr,p:pin_link;
    V_RET:name;              
 begin        
     d_ptr:=f_dev;  
     z_dev^.dev:=V.dev { la Sentinella };
     V_RET:=V;       
     while d_ptr^.dev<>V.dev do d_ptr:=d_ptr^.d;
     if d_ptr=z_dev then { not on list so return same } Begin
       end          
     else if v.pin=no_pin then begin
       if d_ptr^.p<>d_ptr^.z_p then begin
         V_RET.pin:=d_ptr^.p^.pin;
         end          
       else { just return the same pin } begin
       end;
       end
     else { connector exists check to see if pin does } begin
       p_ptr:=d_ptr^.p;
       d_ptr^.z_p^.pin:=v.pin;
       while p_ptr^.pin<>v.pin do p_ptr:=p_ptr^.p;
       if p_ptr=d_ptr^.z_p then { pin does not exist }begin
         end        
       else { pin exists get the next one } begin
         p_ptr:=p_ptr^.p;
         if p_ptr=d_ptr^.z_p then { no more pins } begin
           end
         else begin
           V_RET.pin:=p_ptr^.pin;
         end;               
       end;         
     end;           
     next_dev_pin:=V_RET;             
 end;             


{********************************************************************}
                                        
function Name_Address(x:name;var fail:integer;var down:dev_link):integer;
  var j:integer;y1,y2:name;
  function Address(x:name;f_ptr:dev_link;var d_ptr:dev_link):integer;  
    var p_ptr:pin_link;                                                
        pin:alfa;
        xx:integer;              
                                       
    function equal(a,b:alfa):boolean;
    var la,lb:integer; 
    begin
        la:=alfa_length(a);lb:=alfa_length(b);
        if a=b then begin
          equal:=true;
          end
        else if not good_integer(a,1,la) then begin
          equal:=false;
          end
        else if not good_integer(b,1,lb) then begin
          equal:=false;
          end
        else begin
          equal:=(alfa_to_number(a,1,la)=alfa_to_number(b,1,lb));
        end;
     end;              

    begin                                                 
           Address:=Bad_Address;fail:=0;        
           d_ptr:=f_ptr{first_device};
           z_dev^.dev:=x.dev;
           WHILE (d_ptr^.dev <> x.dev ) DO d_ptr:=d_ptr^.d;
           if d_ptr = z_dev then begin
             fail:=1;{ device not found }
             Err_Mess:='Device/Connector Not found:';
             Err_p1:=x.dev;
             Err_Ps:=1;
             end      
           else begin 
             p_ptr:=d_ptr^.p;
             d_ptr^.z_p^.pin:=x.pin;
             WHILE (p_ptr^.pin<> x.pin ) DO p_ptr:=p_ptr^.p;
             if p_ptr=d_ptr^.z_p then begin
               xx:=Alpha_Pos(x.pin);{ see if a-z or *A-*Z }
               if (xx>=27) and (xx<=52) then begin
                 if x.pin[1]='*' then begin
                   Pin:=reg_alpha_pin(xx,true); { get the lower case version}
                   end
                 else begin
                   Pin:=reg_alpha_pin(xx,false); { get the *x  version}
                 end;
                 p_ptr:=d_ptr^.p;
                 d_ptr^.z_p^.pin:=pin;
                 WHILE (p_ptr^.pin<> pin ) DO p_ptr:=p_ptr^.p;
                 end
               else if good_integer(x.pin,1,alfa_length(x.pin)) then begin
                 p_ptr:=d_ptr^.p;
                 d_ptr^.z_p^.pin:=x.pin;
                 WHILE not equal(x.pin,p_ptr^.pin) DO p_ptr:=p_ptr^.p;
               end;
             end;

             if p_ptr=d_ptr^.z_p then begin
               fail:=2;{ device found but no pin }
               Err_Mess:='Good Device/connector but bad pin:';
               Err_p1:=x.dev;Err_p2:=x.pin;
               Err_Ps:=2;
               end    
             else begin
               Address:=p_ptr^.i_add;
             end;     
           end;       
    end;


          
begin {name_address}
    j:=address(x,f_dev,down);
    if j=BAD_ADDRESS then begin { see if a compound device}
      y1:=x;y2:=y1;l1:=index(y1.dev,'.');
      y2.dev:=extract_to_dot(y1.dev,1);
      j:=address(y2,f_dev,down);
      if fail<>1 then begin
        while l1>0 do begin
          y1.dev:=extract_alfa(y1.dev,l1+1,al);
          y2.dev:=extract_to_dot(y1.dev,1);
          j:=address(y2,down^.sub_dev,down); 
          if fail<>1 then begin{ found a device/subdevice so continue}
            l1:=index(y1.dev,'.');
            end
          else begin {fail=1 (no such device)}
            l1:=0;   { in either case we are done }
          end;
        end;
      end;
    end;
    Name_Address:=j; 
end;              
                  
{**************************************************************}
function address_of(x:name):integer;
var fail:integer;dev_ptr:dev_link;
begin
    address_of:=name_address(x,fail,dev_ptr);
end;

{********************************************************************}
 function device_exists(Dev_Name:alfa):boolean;
 var
    x:name;fail,i:integer;dev_ptr:dev_link;

 begin
    x.dev:=Dev_Name;x.pin:=NULL;
    i:=name_address(x,fail,dev_ptr);
    device_exists:=((fail=0) or (fail=2));
 end;

{********************************************************************}
 function device_pointer(Dev_Name:alfa):dev_link;
 var      
    d_ptr:dev_link;
    x:name;i,fail:integer;
 begin     
   x.dev:=Dev_Name;x.pin:=NULL;
   i:=name_address(x,fail,d_ptr);
   device_pointer:=d_ptr;
 end;

{**************************************************************}
function Good(Address:Integer):Boolean;
begin             
      if Address = Bad_Address then Good:=FALSE
      else   if Address <V_min then Good:=FALSE
      else   if Address >V_Max then Good:=FALSE
      else Good:=TRUE;
end;              
                  
{******************************************************************}
function pin_exists(v:name):boolean;
begin             
    pin_exists:=Good(Address_of(v));
end;              

{********************************************************************}
function make_sensed(Conn_Name,Pin_Name:alfa):boolean;
 var x  :name;
     i,m:integer;

 begin
     x.dev:=Conn_Name;x.pin:=Pin_Name;
     i:=Address_Of(x);
     m:=mach_xlate(i);
     case target_machine of
        FACT_Machine,DITMCO_660:
          begin
             if(( m mod 2)=0 )then begin
               inf[i]^.sense_b:=YES;
               make_sensed:=true;
               end
             else begin
               inf[i]^.sense_b:=NO;
               make_sensed:=false;
             end;
          end;
     end;
 end;

{********************************************************************}
{           Search QUEUE algorithms                                  }
{********************************************************************}
 procedure SWAP_QUEUES(var Q1,Q2:QUEUE);
 var temp:QUEUE;
 begin
   Temp:=Q1;
   Q1:=Q2;
   Q2:=Temp;
 end;
                                            
{********************************************************************}
procedure Make_STACK(var S:STACK);

  begin
     new(S.head);new(S.z);
     S.head^.next:=S.z;
     S.z^.next:=S.z;
  end;

{********************************************************************}
procedure PUSH_STACK(v:elementtype;var S:STACK);
 var t:p_celltype;
  begin
     new(t);t^.element:=v;t^.next:=S.head^.next;S.head^.next:=t;
     S.Count:=S.Count+1;
  end;

{********************************************************************}
function POP_STACK(var S:STACK):elementtype;
 var t:p_celltype;
  begin
     t:=S.head^.next;
     POP_STACK:=t^.element;S.head^.next:=t^.next;dispose(t);
     S.Count:=S.Count-1;
  end;
     
{********************************************************************}
function EMPTY_STACK(S:STACK):BOOLEAN;
 begin
     EMPTY_STACK:=(S.head^.next=S.z);
 end;

{********************************************************************}
 procedure Make_QUEUE(var Q:QUEUE);
   begin
      new(Q.front);        { create header cell}
      Q.front^.next:=NIL;Q.rear:=NIL;Q.Spare:=NIL;Q.Temp:=NIL;
      Q.rear:=Q.front;     { header is both first and last cell}
   end;

{********************************************************************}
 function EMPTY_QUEUE(Q:QUEUE):Boolean;
   begin
      EMPTY_QUEUE:= Q.front=Q.rear ;
   end;

{********************************************************************}
 function FRONT(Q:QUEUE):elementtype;
   begin
       if EMPTY_QUEUE(Q) then begin
         err_mess:=' Internal error.EMPTY QUEUE';error(0);goto 9999;
         end
       else begin
         FRONT:=Q.Front^.next^.element;
       end;
   end;

{********************************************************************}
 procedure ENQUEUE(x:elementtype;var Q:QUEUE);
   begin
      if Q.spare=NIL then begin
        new(Q.rear^.next);
        end
      else begin
         Q.rear^.next:=Q.spare;
         Q.spare:=q.spare^.next;
      end;
      Q.rear:=Q.rear^.next;
      Q.rear^.element:=x;
      Q.rear^.next:=NIL;
      Q.count:=Q.count+1;
   end;

{********************************************************************}
 procedure ENQUEUE_FRONT(x:elementtype;var Q:QUEUE);
   var t:p_celltype;
   begin
      if Q.spare=NIL then begin
        new(t);
        new(Q.rear^.next);
        end
      else begin
         t:=Q.spare;
         Q.spare:=q.spare^.next;
      end;
      t^.next:=Q.front;
      Q.front:=t;
      Q.front^.element:=x;
      Q.count:=Q.count+1;
   end;

{********************************************************************}
 procedure DEQUEUE(var Q:QUEUE);
   begin
      if EMPTY_QUEUE(Q) then begin
        err_mess:='Internal error:QUEUE is empty';
        error(0);goto 9999;
        end
      else begin
        with Q do begin
          temp:=Spare;
          Spare:=front;
          front:=front^.next;
          Spare^.next:=Temp;
          count:=count-1;
        end;
      end;
   end;
   
{********************************************************************}
 function QUEUE_COUNT( var Q:QUEUE):integer;
   begin
       QUEUE_COUNT:=Q.count;
   end;

{********************************************************************}
 procedure DELETE_ELEMENT(x:elementtype;var Q:QUEUE);
   var y:elementtype;
   begin
      if x=no_node then begin
        err_mess:='Internal error:DELETE_ELEMENT no_node';
        error(0);goto 9999;
        end
      else begin  
        ENQUEUE(no_node,Q);
        while FRONT(Q)<>no_node do begin
          y:=FRONT(Q);DEQUEUE(Q);
          if x<>y then ENQUEUE(y,Q);
        end;
        DEQUEUE(Q); { get rid of no_node }
      end;                                              
   end;
   
{********************************************************************}
   procedure enqueue_maybe(x:elementtype;var Q:QUEUE);

    begin
        if EMPTY_QUEUE(Q) then begin
            ENQUEUE(x,Q);
          end
        else begin
          if not ( x=FRONT(Q) ) then begin
            ENQUEUE(x,Q);
          end;
        end;
    end;
{********************************************************************}
 procedure FLUSH_QUEUE(var Q:QUEUE);
   begin while not EMPTY_QUEUE(Q) DO DEQUEUE(Q); end;

{********************************************************************}
 procedure Make_TEST(var Q:TESTQUEUE);
   begin
      new(Q.front);        { create header cell}
      Q.front^.next:=NIL;Q.rear:=NIL;Q.Spare:=NIL;Q.Temp:=NIL;
      Q.rear:=Q.front;     { header is both first and last cell}
      Q.count:=0;
   end;

{********************************************************************}
 function Empty_TEST(Q:TESTQUEUE):Boolean;
   begin
      Empty_TEST:= Q.front=Q.rear ;
   end;

{********************************************************************}
 function front_TEST(Q:TESTQUEUE):test_element;
   begin
       if Empty_TEST(Q) then begin
         err_mess:=' Internal error.EMPTY TESTQUEUE';error(0);goto 9999;
         end
       else begin
         FRONT_TEST:=Q.Front^.next^.element;
       end;
   end;

{********************************************************************}
 procedure enqueue_TEST(x:test_element;var Q:TESTQUEUE);
   begin
      if Q.spare=NIL then begin
        new(Q.rear^.next);
        end
      else begin
         Q.rear^.next:=Q.spare;
         Q.spare:=q.spare^.next;
      end;
      Q.rear:=Q.rear^.next;
      Q.rear^.element:=x;
      Q.rear^.next:=NIL;
      Q.count:=Q.count+1;
   end;

{********************************************************************}
 procedure DEQUEUE_TEST(var Q:TESTQUEUE);
   begin
      if Empty_TEST(Q) then begin
        err_mess:='Internal error:TESTQUEUE is empty';
        error(0);goto 9999;
        end
      else begin
        with Q do begin
          temp:=Spare;
          Spare:=front;
          front:=front^.next;
          Spare^.next:=Temp;
          count:=count-1;
        end;
      end;
   end;
{********************************************************************}
 function TEST_QUEUE_COUNT( var Q:TESTQUEUE):integer;
   begin
       TEST_QUEUE_COUNT:=Q.count;
   end;
   
{********************************************************************}
 procedure DELETE_TEST_ELEMENT(te:test_element;var Q:TESTQUEUE);
  var temp,no_test:test_element;

   function same(te1,te2:test_element):boolean;
     begin
         if inf[te1.x]^.x_node=inf[te2.x]^.x_node then begin
           same:=(inf[te1.y]^.x_node=inf[te2.y]^.x_node);
           end
         else if inf[te1.x]^.x_node=inf[te2.y]^.x_node then begin
           same:=(inf[te1.y]^.x_node=inf[te2.x]^.x_node);
         end;
     end;

   begin   
      no_test.x:=NO_NODE;no_test.y:=NO_NODE;
      ENQUEUE_TEST(no_test,Q);{ put a sentinel }
      temp:=FRONT_TEST(Q);DEQUEUE_TEST(Q);{ take out from front }
      while temp.x<>no_test.x do begin
        if not same(temp,te) then begin
          ENQUEUE_TEST(temp,Q);{ put it back in the rear }
        end;  
        temp:=FRONT_TEST(Q);DEQUEUE_TEST(Q);{ take out from front }
      end;              
   end;
   
                                                 
{********************************************************************}
 procedure FLUSH_TEST_QUEUE(var Q:TESTQUEUE);
   begin while not Empty_TEST(Q) DO DEQUEUE_TEST(Q); end;

{********************************************************************}
{ TEXT QUEUES                                                        }
{********************************************************************}
 procedure Make_TEXT(var Q:TEXTQUEUE);
   begin
      new(Q.front);        { create header cell}
      Q.front^.next:=NIL;Q.rear:=NIL;Q.Spare:=NIL;Q.Temp:=NIL;
      Q.rear:=Q.front;     { header is both first and last cell}
   end;

{********************************************************************}
 function Empty_TEXT(Q:TEXTQUEUE):Boolean;
   begin
      Empty_TEXT:= Q.front=Q.rear ;
   end;

{********************************************************************}
 function front_TEXT(Q:TEXTQUEUE):text_element;
   begin
       if Empty_TEXT(Q) then begin
         err_mess:=' Internal error.EMPTY TEXTQUEUE';error(0);goto 9999;
         end
       else begin
         FRONT_TEXT:=Q.Front^.next^.element;
       end;
   end;

{********************************************************************}
 procedure enqueue_TEXT(x:text_element;var Q:TEXTQUEUE);
   begin
      if Q.spare=NIL then begin
        new(Q.rear^.next);
        end
      else begin
         Q.rear^.next:=Q.spare;
         Q.spare:=q.spare^.next;
      end;
      Q.rear:=Q.rear^.next;
      Q.rear^.element:=x;
      Q.rear^.next:=NIL;
   end;

{********************************************************************}
 procedure DEQUEUE_TEXT(var Q:TEXTQUEUE);
   begin
      if Empty_TEXT(Q) then begin
        err_mess:='Internal error:TEXTQUEUE is empty';
        error(0);goto 9999;
        end
      else begin
        with Q do begin
          temp:=Spare;
          Spare:=front;
          front:=front^.next;
          Spare^.next:=Temp;
        end;
      end;
   end;
{********************************************************************}
 procedure dfs_node(    k:integer;
                    procedure pre_process(v:integer);
                    function criteria(t:node):boolean;
                    procedure post_process(v:integer)
                   );
           
  var t_node:node_link;      

  begin
     PUSH_STACK(k,dfs_STACK);
     repeat 
       k:=POP_STACK(dfs_STACK);
       Dfs_Search:=Dfs_Search+1;
       inf[k]^.dfs_visit:=dfs_SEARCH;
       pre_process(k);
       t_node:=adj[k];
       while t_node<>z_node do begin
         if inf[t_node^.v]^.dfs_visit<dfs_SEARCH then begin
           if criteria(t_node^) then begin
             PUSH_STACK(t_node^.v,dfs_STACK);
             inf[t_node^.v]^.dfs_visit:=dfs_SEARCH;
           end;
         end;
         post_process(k);
         t_node:=t_node^.next;
       end;
     until EMPTY_STACK(dfs_STACK);
  end;

{********************************************************************}
 procedure bfs_node(    v:integer;
                    procedure process(v:integer);
                    function criteria(t:node):boolean);
  var x,y:elementtype;
      t_node:Node_Link;
      Search_Start:Integer;
  begin
     bfs_Search:=bfs_Search+1;
     inf[v]^.bfs_Visit:=bfs_Search;
     ENQUEUE(v,bfs_QUEUE);
     while not EMPTY_QUEUE(bfs_QUEUE) do begin
       x:=FRONT(bfs_QUEUE);
       process(x);
       DEQUEUE(bfs_QUEUE);
       t_node:=adj[x];
       while t_node<>z_node do begin
         if inf[t_node^.v]^.bfs_visit<bfs_Search then begin
           if criteria(t_node^) then begin
             inf[t_node^.v]^.bfs_Visit:=bfs_Search;
             ENQUEUE(t_node^.v,bfs_QUEUE);
           end;
         end;
         t_node:=t_node^.next;
       end {while};
     end {while que is not empty };
  end;         
                 
{********************************************************************}
 procedure bfs_double(    v:integer;
                    procedure initialize(v:integer);
                    procedure process(v:integer);
                    function criteria_1(t:node):boolean;
                    function criteria_2(t:node):boolean);
  var x,y:elementtype;
      t_node:Node_Link;
      Search_Start:Integer;
  begin
     bfs_Search:=bfs_Search+1;
     inf[v]^.bfs_Visit:=bfs_Search;
     ENQUEUE(v,SUB_QUEUE);
     while not EMPTY_QUEUE(SUB_QUEUE) do begin
       x:=FRONT(SUB_QUEUE);DEQUEUE(SUB_QUEUE);
       initialize(x);
       ENQUEUE(x,bfs_QUEUE);
       while not EMPTY_QUEUE(bfs_QUEUE) do begin
         x:=FRONT(bfs_QUEUE);DEQUEUE(bfs_QUEUE);
         process(x);
         t_node:=adj[x];
         while t_node<>z_node do begin
           if inf[t_node^.v]^.bfs_visit<bfs_Search then begin
             if criteria_1(t_node^) then begin
               inf[t_node^.v]^.bfs_Visit:=bfs_Search;
               ENQUEUE(t_node^.v,bfs_QUEUE);
               end
             else if criteria_2(t_node^) then begin
               inf[t_node^.v]^.bfs_visit:=bfs_search;
               ENQUEUE(t_node^.v,SUB_QUEUE);
             end;
           end;
           t_node:=t_node^.next;
         end {while};
       end {while bfs_QUEUE is not empty};
     end {while SUB_QUEUE is not empty};
  end;         
                 
{********************************************************************}
function In_Use(i:integer):boolean;

 { This function specifies if a given node is in use or not.
   A node that belongs to a cable that is not connected to the
   UUT is called no_connex, Whereas one that was connected before
   but now has been disconnected is called stubbed }

 begin   
    if (i>=v_min) and (i<=v_max) then begin
      if inf[i]=z_info then begin
        In_use:=false;
        end
      else begin
        case inf[i]^.status of
          Not_defined :In_Use:=false;
          Being_used  :In_Use:=true;
          No_connex   :In_Use:=false;
          Stubbed     :In_Use:=true;
          Disconnected:In_Use:=false;
        end;
      end;
      end
    else begin
      err_mess:='Illegal internal address.';
      error(0);goto 9999;
    end;
 end;
                                                        
{********************************************************************}
function ext_node(i:integer):boolean;
         
 begin   
    if i<0 then begin
      ext_node:=false;
      end
    else if i=no_node then begin
      ext_node:=false;
      end
    else if inf[i]=z_info then begin
      ext_node:=false;
      end
    else begin
      case inf[i]^.status of
        Not_defined   : ext_node:=false;
        Being_used    : ext_node:=true;
        No_connex     : ext_node:=false;
        stubbed       : ext_node:=false;
        Disconnected  : ext_node:=false;
      end;
    end;
 end;    
         
{********************************************************************}
function ext_pos(i:integer):boolean;

 begin   
      ext_pos:=ext_node(i) and (i>0);
 end;                               

{********************************************************************}
function short_path(t:node):boolean;
 begin    
     case t.cnx of
       wire:short_path:=true;
       contact:short_path:=(t.contact_state=closed);
     OTHERWISE
       short_path:=false;
     end;
 end;


{********************************************************************}
function any_path(t:node):boolean;
 begin
     case t.cnx of
       Contact:any_path:=short_path(t);

       Wire,Admittance,
       PN_Diode,Zener_Diode,
       Relay_Coil,P_Supply:any_path:=true;

     OTHERWISE
       Err_Mess:=' Unspecified Path encountered in any_path!!';
       Warn(0);
       any_path:=false;
     end;
 end;

{********************************************************************}
function no_isthmus_path(t:node):boolean;
 begin
     if t.a_w=yes then begin
       no_isthmus_path:=false;
       end
     else if t.a_p=yes then begin
       no_isthmus_path:=false;
       end
     else begin
       no_isthmus_path:=true;
     end;
 end;  
       

{********************************************************************}
function short_graph(x,y:integer):boolean;
 begin
     short_graph:=(inf[x]^.x_node=inf[y]^.x_node);
 end;         

{********************************************************************}
function resis_graph(x,y:integer):boolean;
 begin
     resis_graph:=(inf[x]^.r_node=inf[y]^.r_node);
 end;         

{********************************************************************}
function ext_c_node(i:integer;four_wire:boolean):boolean;
 begin
     ext_c_node:=ext_node(i) and((inf[i]^.sense_b=YES) or not four_wire);
 end;                                            

{********************************************************************}
function lowest_node(function circuit_path(p:node):boolean;x:integer;
                     var allow:omit_flag;four_wire:boolean):integer; 
{ this function will return lowest internal address on a continuity string
  that contains node v. It will also return if there is an omit node 
  on that continuity string (i.e. allow=false)}

var low_node:integer;
    low_add :integer;
  procedure visit(k:integer);
  var t_node:node_link;
      add:integer;
  BEGIN
    inf[k]^.dfs_visit:=dfs_Search;
    t_node:=adj[k];          
    WHILE (t_node <> z_node) DO BEGIN
      if circuit_path(t_node^) then begin
        if ord(inf[t_node^.v]^.omit)>ord(allow) then begin
          allow:=inf[t_node^.v]^.omit;
        end;
        if ext_c_node(t_node^.v,four_wire) then begin
          add:=Mach_Add(t_node^.v);
          if add>=Lowest_F_Address then begin 
            if add<low_add then begin
              low_add:=add;low_node:=t_node^.v;
              end     
            else if low_node<=0 then begin { pick the first positive node }
              low_add:=add;low_node:=t_node^.v;
            end;              
            end 
          else if low_node<0 then begin  { get the minimum if no pos seen}
            if add<low_add then begin
              low_add:=add;low_node:=t_node^.v;
            end;
          end;
        end;
        if (inf[t_node^.v]^.dfs_visit<> dfs_Search) then visit(t_node^.v);
      end;
      t_node:=t_node^.next;
    end
  end;   
  
begin
  dfs_Search:=SUCC(dfs_Search);low_node:=x;allow:=NO_OMIT;
  IF (x<=v_max) and (x>=v_min) then begin
    low_add:=Mach_Add(x);
    if ord(inf[x]^.omit)>ord(allow) then allow:=inf[x]^.omit;
    visit(x);
    end
  else begin
    err_mess:='Internal error at lowest_node.Bad node specified';
    error(0);goto 8888;
  end;                                                    
  lowest_node:=low_node;
end;

{********************************************************************}
function f_node(x:integer;var allow:omit_flag;four_wire:boolean):integer; 
begin
    f_node:=lowest_node(short_path,x,allow,four_wire);
end;

{********************************************************************}
function l_node(x:integer;var allow:omit_flag;four_wire:boolean):integer; 
begin    
    l_node:=lowest_node(any_path,x,allow,four_wire);
end;

{********************************************************************}
function o_node(x:integer;var allow:omit_flag;four_wire:boolean):integer; 
begin    
    o_node:=lowest_node(no_isthmus_path,x,allow,four_wire);
end;
              
{********************************************************************}
procedure adjust_x_nodes(x:integer);
 var allow:omit_flag;
     x_node:integer;
  procedure pump(x:integer);
  begin       
      inf[x]^.x_node:=x_node;
  end;
                 
 begin
     x_node:=f_node(x,allow,false);
     bfs_node(x,pump,short_path);
 end;

{********************************************************************}
procedure adjust_r_nodes(x:integer);
 var allow:omit_flag;
     R_node:integer;
  procedure pump(x:integer);
  begin
      inf[x]^.r_node:=r_node;
      if inf[x]^.source=yes then begin
        inf[r_node]^.forced:=yes;
      end;
  end;
                 
 begin      
     R_node:=l_node(x,allow,false);
     inf[R_node]^.forced:=no;
     bfs_node(x,pump,any_path);
 end;

{******************************************************************}
procedure adjust_PS_nodes(x0:integer);
 var allow:omit_flag;
     i_node,y:integer;

 {  On Lake Washington in Washington State There is an island called }
 {  Mercer Island. Mercer Island is connected to Seattle and Bellevue}
 {  Thru single bridge at each side. ( Used to be two bridges to }
 {  Seattle but one sank.) So in this program Mercer means a (group of) }
 {  node(s) that are connected by isthmus paths to other nodes.  }
 {  This is used to determine if a resistor is testable. }
 {  Decision goes like this:  }
 {  If node '1' is Bellevue and node '2' is Mercer Island , then }
 {  if either one of '1' or '2' is not connected }



 procedure get_i_node(x:integer);
  begin
     if I_node<>No_Node then begin
       if inf[i_node]^.psbit=no then begin
         ENQUEUE(i_node,temp_QUEUE);
       end;
     end;
     i_node:=O_node(x,allow,false);
     inf[i_node]^.psbit:=no;      
  end;

  procedure pump(x:integer);
  begin
      inf[x]^.i_node:=i_node;
      if inf[x]^.forced=yes then begin
        inf[i_node]^.psbit:=yes;
        while not EMPTY_QUEUE(temp_QUEUE) do begin
          y:=FRONT(temp_QUEUE);DEQUEUE(temp_QUEUE);
          inf[y]^.psbit:=yes;
        end;
      end;
  end;
      

begin
    FLUSH_QUEUE(temp_QUEUE);i_node:=No_Node;
    bfs_double(x0,get_I_node,pump,any_path,no_isthmus_path);
end;



{********************************************************************}
procedure adjust_rxp_nodes(x:integer);
 begin 
    adjust_x_nodes(x);
    adjust_r_nodes(x);
    if PSUPPLIES>0 then begin
      adjust_PS_nodes(x);
    end;
 end;
{********************************************************************}
                  
procedure connect_wire(x,y:integer;r:real;var connected:boolean);
                  
{ This procedure will insert nodes once into x and
  once into y node list. Notice that if x=y then no wires
  will be inserted.X and Y are internal addresses  }
var other1,other2:node_link;
procedure con(x,y:integer;var connected:boolean;var t_node:node_link);
 label 99;
 VAR 
         exists : boolean;

begin             
        exists:=false;

        t_node:=adj[x];
        while (t_node<>z_node) do begin
          if t_node^.v=y then begin
            with t_node^ do begin
              if cnx=wire then begin
                Exists:=true;
                t_node:=z_node;
                end
              else begin
                t_node:=t_node^.next;
              end;
            end;
            end
          else begin
            t_node:=t_node^.next;
          end;
        end;
        if not exists then { wire does not exist create it } begin
          new_node(t_node);
          with t_node^ do begin { initialization}
            v:=y;
            cnx:=wire;Terminal:=Wire_End;y_cnx:=1000.0 {Mhos};
            tol:=0.0;wire_capacity:=10.0; { large amps }
            {tag internal connections as checked}
            if  section=Parts_Section then x_c:=YES;
            require:=X_Check;
            next:=adj[x]; prev:=z_Node ;
            if adj[x] <> Z_Node then adj[x]^.prev:=t_node;
            adj[x]:=t_node;
            connected:=true;
          end;
99:     end;
end;              
begin
      connected:=false;  
      if x<>y then begin
        con(x,y,connected,other1);
        if connected then begin 
          con(y,x,connected,other2);
          other1^.other:=other2;other2^.other:=other1;
          adjust_rxp_nodes(x);
        end;                      
      end;            
end;

{******************************************************************}
   function xrc_path(t:node;mode:test_mode):boolean;

   begin
       case mode of
          SHORT_SHORTS:xrc_path:=short_path(t);
          RESISTORS   :xrc_path:=any_path(t);
       end;
   end;


{******************************************************************}
function tag_critical(v0:integer;mode:test_mode):integer;
  var nl:bit;
      search_start:integer;
  function tag_aw(k,k0:integer;var la:bit;t_back:node_link):integer;
   var   t_node:node_link;
          nl,now,min:integer;
          pla,p_path:bit;


    { this algorithm will determine if a path to this node is an
      isthmus path or not.             
      there are three possible cases to consider:
      1: next node has not been visited
         action:visit and see if visit number returned is <= current
                node visit number ( if so obviously a cycle exists
                and wire is not crucial)
                also get the positive path exists flag.
      2: next node is where we came from 
         action:in this case just remember the link pointer for
                later action.
      3: next node is already visited
         action: obviously a connection to a previous node so
                it is not an isthmus wire and is connected      
                to outside world thru xnode (which is where the
                whole search started anyway).
              
         next set return path flags. it is not crucial if minimum node 
         seen  is strictly less then this node ( more properly if
         it is less then previous node since multiple wires
         are not allowed but this check is sufficient).
           
                                                       }

    procedure tag_crit(t:node_link;tag1,tag2:bit);

    begin
        case mode of
          SHORT_SHORTS:begin
                          t^.a_w:=tag1;t^.path:=tag2;
                       end;
          RESISTORS   :begin
                          t^.a_p:=tag1;t^.rath:=tag2;
                       end;
        end;
    end;

    BEGIN
      dfs_Search:=dfs_Search+1;
      inf[k]^.dfs_visit:=dfs_Search;now:=dfs_Search;nl:=now;min:=nl;
      t_node:=adj[k];              
      if ext_node(k) then p_path:=YES else p_path:=NO;
      WHILE (t_node <> z_node) DO BEGIN
        if xrc_path(t_node^,mode) then begin { traverse this wire }
          if inf[t_node^.v]^.dfs_visit<search_start then begin 
            nl:=tag_aw(t_node^.v,k,pla,t_node^.other);{ see if we loop back}
            if nl<=now then begin
              tag_crit(t_node,NO,pla);
              end
            else begin 
              tag_crit(t_node,YES,pla);
            end;
            if nl<min then min:=nl; { remember lowest node seen}
            if pla=YES then p_path:=YES;{ accumulate path lookahead}
            end
          else if t_node^.v=k0 then begin { this is how we came here}
            end        
          else begin { Already visited so must be a cycle}
            tag_crit(t_node,NO,YES);{path to xnode}
            p_path:=YES;{to xnode}
            if inf[t_node^.v]^.dfs_visit<min then begin
              min:=inf[t_node^.v]^.dfs_visit;
            end;
          end;
        end;
        t_node:=t_node^.next;
      end;    
      { if we saw a lower node declare return path as NO aw}
      if t_back<>NIL then begin
        if min<now then tag_crit(t_back,NO,YES) else tag_crit(t_back,YES,YES);
      end;
      tag_aw:=min;la:=P_Path;       
    end;          

  begin {tag_critical}
      nl:=NO;dfs_Search:=dfs_Search+1;search_start:=dfs_Search;
      tag_critical:=tag_aw(v0,v0,nl,NIL);
  end;                         

{********************************************************************}
procedure connect_contact(x,y:integer;r:real;var connected:boolean);
                  
{ This procedure will insert nodes once into x and
  once into y node list. Notice that if x=y then no wires
  will be inserted.X and Y are internal addresses  }
 var other1,other2:node_link;
procedure con(x,y:integer;var connected:boolean;var t_node:node_link);
 label 99;
 VAR                                                                  
         exists : boolean;
         curr_state:contact_states;


begin             
        exists:=false;

        t_node:=adj[x];
        while (t_node<>z_node) do begin
          if t_node^.v=y then begin
            with t_node^ do begin
              if cnx=Contact then begin 
                Exists:=true;
                curr_state:=contact_state;
                case curr_state of
                  Opened:
                    Begin
                      contact_state:=Closed;require:=X_check; connected:=true;
                    end;
                  Closed:
                    Begin
                      err_mess:='Contact is already closed';
                      error(0);goto 99;
                    end;
                end{case curr_state of};
                t_node:=z_node;
                end
              else begin
                t_node:=t_node^.next;
              end{cnx=contact};
            end { with };
            end
          else begin
            t_node:=t_node^.next;
          end;
        end;
                                
        if not exists then { contact does not exist create it } begin
          new_node(t_node);
          with t_node^ do begin { initialization}
            v:=y;
            cnx:=contact ;Terminal:=Pole;y_cnx:=100.0 {Mhos};
            tol:=0.0;contact_capacity:=10.0;{large amps }
            contact_state:=closed;
            require:=X_Check;
            next:=adj[x]; prev:=z_Node ;
            if adj[x] <> Z_Node then adj[x]^.prev:=t_node;
            adj[x]:=t_node;
            connected:=true;
          end;
        end;      
99:;
end;              
begin                                               
      connected:=false;        
      if x<>y then begin
        con(x,y,connected,other1);
        if connected then begin
          con(y,x,connected,other2);                  
          other1^.other:=other2;other2^.other:=other1;
          adjust_rxp_nodes(x);
        end;
      end;            
end;                

{********************************************************************}
procedure connect_two_term(x,y:integer;r1,r2,tolerance,capacity:real);
                  
label 99;
var Term1,Term2:Terminal_Types;
       Cnx_Type:Cnx_Types;
       Dev_Type:Part_Types;
       Dev_No:alfa;
       connected:boolean;
       other1,other2:node_link;
procedure con(x,y:integer;r:real;Term:Terminal_Types;var t_node:node_link);
 label 99;
 VAR                                                                       
         exists : boolean;
         curr_state:contact_states;   
begin {con}            
        exists:=false;
          
        t_node:=adj[x];
        while (t_node<>z_node) do begin
          if t_node^.v=y then begin
            with t_node^ do begin
              if cnx in [Admittance,PN_Diode,ZENER_Diode] then begin
                if inf[y]^.dev^.dev=dev_no then begin
                  Exists:=true;
                  t_node:=z_node;
                end;
              end;
            end;
            end         
          else begin
            t_node:=t_node^.next;
          end;
        end;
                                
        if not exists then { wire does not exist create it } begin
          new_node(t_node);
          with t_node^ do begin { initialization}
            connected:=true;
            v:=y;
            cnx:=Cnx_Type;
            case cnx_type of
              Admittance:
                begin               
                  y_cnx:=1.0/r;
                  v_cnx:=Nom_Meas(0.0,VDC);
                  tol:=tolerance;
                  y_capacity:=Capacity;
                end;                        
              PN_Diode:
                begin               
                  y_cnx:=1.0/r;
                  v_cnx:=Nom_Meas(0.0,VDC);
                  tol:=tolerance;
                end;
              Relay_Coil:                        
                begin               
                  y_cnx:=1.0/r;
                  v_cnx:=Nom_Meas(0.0,VDC);
                  tol:=tolerance;
                end;                        
              P_Supply:
                begin
                  y_cnx:=1000.0;
                  v_cnx:=Nom_Meas(0.0,VDC);
                end;
            OTHERWISE { force an error }
              connected:=false;
            end;
            Terminal:=Term;
            require:=X_Check;
            next:=adj[x]; prev:=z_Node ;
            if adj[x] <> Z_Node then adj[x]^.prev:=t_node;
            adj[x]:=t_node;
          end;
99:     end;
end;              
begin
      connected:=false;  
      if x<>y then begin
        Dev_Type:=inf[y]^.dev^.part_type;
        Dev_no  :=inf[y]^.dev^.dev;
        Case Dev_Type of
          Diode:  
            Begin
              Term1:=Diode_Anode;Term2:=Diode_Cathode;
              Cnx_Type:=PN_Diode;
            end;
          Resistor:
            Begin
              Term1:=Pin_Term;Term2:=Pin_Term;
              Cnx_Type:=Admittance;
            end;

          Relay:
            Begin
              Term1:=Pin_Term;Term2:=Pin_Term;
              Cnx_Type:=Relay_Coil;
            end;           
          PSupply:
            Begin
              Term1:=PS_HI;Term2:=PS_REF;
              Cnx_Type:=P_Supply;
            end;           

      
        Otherwise
            Err_Mess:=' Unimplemented device in connect_Two_Term';
            error(0);goto 99;
        end;
        con(x,y,r1,Term1,other1);
        if connected then begin
          con(y,x,r2,Term2,other2);
          other1^.other:=other2;other2^.other:=other1;
          adjust_rxp_nodes(x);adjust_rxp_nodes(y);
        end;
        if not connected then begin
          err_mess:=' Internal error in connect_two_term';error(0);
        end;             
      end;            
99:;
end;          

{********************************************************************}
procedure disconnect(x,y:integer;var disconnected:boolean);
            
  procedure dis(x,y:integer);
  var t:node_link;
  begin             

     t:=adj[x]; { from wires connected to node x}

     WHILE t <> z_node DO BEGIN
       if t^.v = y then BEGIN
         case t^.cnx of
           contact:
             begin
               if t^.contact_state=closed then begin
                 t^.contact_state:=opened;        
                 t^.require:=O_Check;
                 t^.j_d:=YES;
               end;
               t:=z_node;
               Disconnected:=true;
             end;
         OTHERWISE
           t:=t^.next;{ don't bother with wires or devices}
         end;
         end
       else begin
         t:=t^.next;
       end;
     end;
  end;        

  begin{disconnect}
  disconnected:=false;
  if (x<=v_max) and (y<=v_max) and (x>=v_min) and (y>=v_min) then begin
    if (adj[x]<>z_node) and (adj[y]<>z_node) THEN BEGIN  
      dis(x,y);
      if disconnected then begin
        dis(y,x);adjust_rxp_nodes(x);adjust_rxp_nodes(y);
      end;
    end;  
  end;            
end;              

  
{*******************************************************************}
procedure connect_pair(x,y:integer);
var  connected,found:boolean;
     nl:integer;
  begin
        if (x<>y) and OK then begin
          connect_contact(x,y,Contact_Imp,connected);
          if connected then begin
            nl:=tag_critical(x,SHORT_SHORTS);
            nl:=tag_critical(x,RESISTORS);
            enqueue_maybe(x,x_QUEUE);
            enqueue_maybe(y,y_QUEUE);
          end;
        end;
  end;                             

{*********************************************************************}
procedure disconnect_pair(x,y:integer);
label 99;
var Disconnected:boolean;
    nl:integer;
  begin
        if (x<>y) and OK then begin
          disconnect(x,y,Disconnected);
          if not Disconnected then begin
            err_mess:='No contact to disconnect';
            error(0);goto 99;
            end
          else begin
            nl:=tag_critical(x,SHORT_SHORTS);
            nl:=tag_critical(x,RESISTORS);
            nl:=tag_critical(y,SHORT_SHORTS);
            nl:=tag_critical(y,RESISTORS);
            enqueue_maybe(x,x_QUEUE);
            enqueue_maybe(y,y_QUEUE);
          end;
        end;
  99:       
  end;

{************************************************************************}
function new_block(curr_add:integer):integer;
 var i:integer;
 begin
      i:=Source_To_Int_Add(Curr_Add);
      if i>=0 then begin
        if (i mod PPC)<>1 then begin               
           i:=(((i-1)+PPC) div PPC)*PPC + 1;
        end;
        New_Block:=i;
        end
      else begin
        New_Block:=Next_TB_Address;
      end;
 end;

{**********************************************************************}
procedure make_pin_name(a1,a2:alfa; var b:alfa2;var l:integer);
var i,l1,l2,p:integer;
begin             
     l1:=alfa_length(a1);l2:=alfa_length(a2);
     b:=' ';p:=0; 
     for i:=1 to l1 do begin p:=p+1;b[p]:=a1[i];end;
     if a2<>No_Pin then begin
       p:=p+1;b[p]:='-';
       for i:=1 to l2 do begin p:=p+1;b[p]:=a2[i];end;
     end;         
     l:=p;        
end;              

{*********************************************************************}

    function full_name(pack,dev:alfa):alfa;
      var f1:alfa;
          l1,l2,i:integer;
      begin                 
          if pack=NULL then begin
            full_name:=dev;
            end
          else if pack=' ' then begin
            full_name:=dev; 
            end
          else if dev=' ' then begin
            full_name:=pack;
            end
          else if dev=NULL then begin
            full_name:=pack;
            end
          else begin
            f1:=' ';l1:=alfa_length(pack);l2:=alfa_length(dev);
            if l1+l2<AL then begin
              f1:=pack;
              f1[l1+1]:='.';
              for i:=1 to l2 do f1[i+l1+1]:=dev[i];
              full_name:=f1;
              end
            else begin
              err_mess:='PACK name is too large:';
              err_p1:=pack;err_p2:=dev;
              error(2);
              full_name:='BAD_NAME';
            end;
          end;
         
      end;
                  
{*********************************************************************}
function dev_link_to_name(t:Dev_Link):alfa;
  var n:name;b:alfa;
  Begin
       b:=' ';
       while t<>NIL do begin
         b:=full_name(t^.dev,b);           
         t:=t^.Head;
       end;
       Dev_Link_to_name:=b;
  end;            
       
{*********************************************************************}
function int_to_name(i:integer):name;
  var n:name;b:alfa;
      t:dev_link;     
  Begin
       t:=Inf[i]^.dev;
       n.dev:=Dev_Link_To_Name(t);
       n.pin:=Inf[i]^.Pin^.pin;
       int_to_name:=n;
  end;            

{**********************************************************************}
function float_to_alfa(x:real;l2:integer):alfa;
var i,j,l1,p,max_r,whole:integer;
    r:real;
    a,int_part,dec_part:alfa;
    { l2 specifies how many digits after decimal point }

begin
     a:=' ';
     if x<0.0 then begin a[1]:='-';p:=1;x:=-x;end else p:=0;
     whole:=trunc(x);
     r:=x-whole;
     if r=0.0 then begin
       int_part:=Number_To_Alfa(whole,0);
       float_to_alfa:=int_part;
       end
     else begin
       max_r:=1;
       for j:=1 to l2 do begin r:=r*10;max_r:=max_r*10;end;
       i:=round(r);if i=max_r then begin i:=0;whole:=whole+1;end;
       int_part:=Number_To_Alfa(whole,0);
       dec_part:=Number_To_alfa(i,l2);{ zero filled on the left }
       l1:=alfa_length(int_part);
       for j:=1 to l1 do begin
         p:=p+1;a[p]:=Int_Part[j];
       end;
       if i>0 then begin
         p:=p+1;a[p]:='.';
         for j:=p+1 to al do begin
           a[j]:=dec_part[j-p];
         end;
         {remove trailing zeroes}
         j:=al;
         while (a[j]='0') or (a[j]=' ') do begin a[j]:=' ';j:=j-1 end;
         if a[j]='.' then a[j]:=' ';{rid of trailing decimal point};
       end;
       float_to_alfa:=a;
     end;
end;


{**********************************************************************}
procedure write_test_c(c:char);

begin
    case To_Keyboard of
        true : write(OUTPUT,c);
        false: write(test_file,c);
    end;
end;

{**********************************************************************}
procedure write_test_blanks(count:integer);
var i:integer;
begin
    for i:=1 to count do write_test_c(' ');
end;

{**********************************************************************}
procedure write_test_alfa(c:alfa;l:integer);
var i,ll:integer;
begin
    if l=0 then ll:=alfa_length(c) else ll:=l;
    for i:=1 to ll do write_test_c(c[i]);
end;

{**********************************************************************}
procedure write_test_alfa2(c:alfa2;l:integer);
var i,ll:integer;
begin
    if l=0 then ll:=alfa2_length(c) else ll:=l;
    for i:=1 to ll do write_test_c(c[i]);
end;

{**********************************************************************}
procedure write_test_ln;

begin 
    case To_Keyboard of
        true : writeln(OUTPUT);
        false: writeln(test_file);
    end;
end;

{**********************************************************************}
procedure write_test_mess(bl:integer;var mess:mess_rec);
var i:integer;
    c:char;
begin
     case Target_Machine of
      Fact_Machine:
           Begin
             write_test_blanks(bl);
             a_alfa:='A%';write_test_alfa(a_alfa,0);write_test_blanks(1);
             for i:=1 to mess.length do write_test_c(mess.line[i]);
             write_test_c('&');write_test_ln;
           end;
      DITMCO_660:
           Begin
             for i:=1 to mess.length do begin
               c:=mess.line[i];
               if c in ['a'..'z'] then c:=chr(ord(c)-32);
               write_test_c(c);
             end;
             for i:=mess.length to 38 do write_test_c(' ');
           end;
      DITMCO_9100:
           Begin
             for i:=1 to mess.length do begin
               c:=mess.line[i];
               write_test_c(c);
             end;
             for i:=mess.length to 38 do write_test_c(' ');
           end;
      OTHERWISE                                      
           Err_Mess:='Write_test_mess:Unimplemented Target machine.';
           error(0);
      end;
end;

{**********************************************************************}
procedure clear_message(var mess:mess_rec);
begin
     mess.line:=' ';
     mess.length:=0;
end;           

{**********************************************************************}
  procedure add_one_char(c:char;var display:mess_rec);
  begin
       with display do begin
         if not OK then begin
           {null statement}
           end
         else if length<CLL then begin
           length:=length+1;line[length]:=c;
           end
         else begin
           err_mess:=' Can not fit message in one line';
           error(0);
         end;
       end;
  end;

{**********************************************************************}
  procedure add_alfa(a:alfa;var display:mess_rec);
  var i,j:integer;   
  begin
       j:=alfa_length(a);
       with display do begin
         for i:=1 to j do add_one_char(a[i],display);
       end;
  end;

{**********************************************************************}
  procedure add_alfa2(a:alfa2;var display:mess_rec);
  var i,j:integer;   
  begin
       j:=alfa2_length(a);
       with display do begin
         for i:=1 to j do add_one_char(a[i],display);
       end;                       
  end;

{**********************************************************************}
  procedure add_alfa3(a:alfa3;var display:mess_rec);
  var i,j:integer;   
  begin
       j:=alfa3_length(a);
       with display do begin
         for i:=1 to j do add_one_char(a[i],display);
       end;
  end;

{**********************************************************************}
  procedure add_alfa4(a:alfa4;var display:mess_rec);
  var i,j:integer;   
  begin
       j:=alfa4_length(a);
       with display do begin
         for i:=1 to j do add_one_char(a[i],display);
       end;
  end;

{**********************************************************************}
  procedure add_blank(var display:mess_rec);
  begin
      with display do begin
        length:=length+1;line[length]:=' ';
      end;
  end;

{**********************************************************************}
  procedure tab_to(l:integer;var display:mess_rec);
  begin
      with display do begin
        while length<l do begin add_blank(display);end;
      end;
  end;
                                    
{**********************************************************************}
procedure add_float(x:real;places:integer;var mess:mess_rec);
var a:alfa;
begin
    a:=float_to_alfa(x,places);{ 5 digits after decimal point }
    add_blank(mess);add_blank(mess);add_alfa(a,mess);
end;                   

{**********************************************************************}
procedure add_relop(relop:rels;var mess:mess_rec);
var a:alfa;
begin
        case relop of
           le : a:=' <= ';
           eq : a:=' = ';
           ge : a:=' >= ';
        end;
        add_blank(mess);add_alfa(a,mess);
end;

{**********************************************************************}
procedure add_unit(u:units;var mess:mess_rec);
var a:alfa;
begin
     case      u of
          ohm        : a:='Ohm';
          Kohm       : a:='KOhm';
          Mohm       : a:='MOhm';
          mvdc       : a:='mVDC';
          vdc        : a:='VDC';
          Kvdc       : a:='KVDC';
          mvac       : a:='mvac';
          vac        : a:='vac';
          Kvac       : a:='Kvac';
          uadc       : a:='uA';
          madc       : a:='mA';
          adc        : a:='AMP';
          uaac       : a:='ua';
          maac       : a:='ma';
          aac        : a:='AMP';
          Watt       : a:='Watt';
          mSec       : a:='mSec';
          Sec        : a:='Sec';
          Pct        : a:='Percent';
          Ramp       : a:='Ramp';
          ZeroCross  : a:='ZeroCross';
          badunit    : a:='Bad Unit';
     end;
     add_blank(mess);add_alfa(a,mess);
end;


{**********************************************************************}
procedure add_entity(x:real;u:units;var mess:mess_rec);
begin
     add_float(x,5,mess);
     add_unit(u,mess);
end;

{**********************************************************************}
  procedure add_mess(a:alfa2;var display:mess_rec);
  var i,j:integer;   
  begin
       add_blank(display);
       add_alfa2(a,display);        
  end;

{**********************************************************************}
 procedure launder_mess(display:mess_rec;BAD_cs:charset);
  var i:integer;
      a_1:alfa;
  begin
      for i:=1 to display.length do begin
        if display.line[i] in BAD_cs then begin
          err_mess:='Character is not allowed in message:';
          a_1:=' ';a_1[1]:=display.line[i];
          err_p1:=a_1;error(1);
        end;
      end;
  end;

{**********************************************************************}
procedure put_test(var t_line:mess_rec);
var i:integer;

begin
    for i:=1 to t_line.length do begin
      write_test_c(t_line.line[i]);
    end;
    write_test_ln;
end;

{**********************************************************************}
 procedure put_display(dd:wish;display:mess_rec);
 var i,j:integer;
     Emit_Halt:Boolean;
     Max_T_Line:integer;
  procedure Emit_Ditmco_Lines(d_Line:strCLL;i,j:Integer);
                                           
  var k,k1,i1,j1:integer;
      c:char;
  begin
      i1:=i;
      while (d_line[i1]=' ') and (i1<j) do i1:=i1+1;
      if i1<=j then begin
        if i1+Max_T_Line<j then begin { break into multiple lines}
          j1:=i1+Max_T_Line;
          { make sure and don't break within a word }
          if (d_line[j1]<>' ') and (d_line[j1+1]<>' ') then begin
            while (j1>=i1) and (d_Line[j1]<>' ') do  j1:=j1-1;
          end;
          end
        else begin 
          j1:=j;
        end;
        k1:=j1;
        while (k1>=i1) and (d_Line[k1]=' ') do  k1:=k1-1;
        if k1>=i1 then begin
          if dd=hookup then begin
            write_test_blanks(3);
            end
          else begin
            a_alfa:='*T';
            write_test_alfa(a_alfa,0);write_test_blanks(1);
          end;
          if i>1 then write_test_blanks(1);
          for k:=i1 to k1 do begin
            c:=d_line[k];
            if c in ['a'..'z'] then c:=chr(ord(c)-32);
            write_test_c(c);
          end;
          write_test_ln;Max_T_Line:=26;
          Emit_Ditmco_Lines(d_Line,j1+1,j)
        end;
      end;
  end;  
 begin
     case target_machine of
       FACT_Machine:
         Begin
            j:=1;
            case dd of
              d,hookup :
                  begin
                    launder_mess(display,['&','%']);
                    if display_lines>=MAX_DISPLAY_LINES then begin
                      a_alfa:='DW%';write_test_alfa(a_alfa,0);
                      wait_manual:=false;
                      display_lines:=0;
                      if display.line[1]=' ' then j:=2;
                      end
                    else begin
                      a_alfa:='W%';write_test_alfa(a_alfa,0);
                      display_lines:=display_lines+1;
                    end;
                  end;
              ds:
                  begin
                    launder_mess(display,['&','%']);
                    a_alfa:='DW%';write_test_alfa(a_alfa,0);
                    display_lines:=0;
                    wait_manual:=false;
                  end;
              da: { Display annotation }
                  begin
                    a_alfa:='W%';
                    write_test_alfa(a_alfa,0);
                  end;

            end;
            for i:=j to display.length do write_test_c(display.line[i]);
            write_test_c('&');write_test_ln;

         end;
       DITMCO_660:
         Begin
            j:=1;Max_T_Line:=27;
            case dd of
              d : begin
                    if display_lines>=MAX_DISPLAY_LINES then begin
                      display_lines:=0;
                      if display.line[1]=' ' then j:=2;
                      end
                    else begin
                      display_lines:=display_lines+1;
                    end;
                    Emit_Halt:=false;
                  end;
              ds: begin
                    Emit_Halt:=true;
                    display_lines:=0;
                  end;
              hookup:
                  begin
                    for i:=1 to display.length do begin 
                      write_test_c(display.line[i]);
                    end;
                    write_test_ln;
                  end;
            OTHERWISE 
              err_mess:='Unimplemented mode in put_display';
              error(0);
            end;
            if dd<>hookup then begin
              if Display.Length=0 then begin
                if not Emit_Halt then begin
                  a_alfa:='*T';Write_test_alfa(a_alfa,0);write_test_ln;
                end;
                end
              else begin
                Emit_Ditmco_Lines(display.line,j,Display.length);
              end;
              if Emit_Halt then begin
                a_alfa:='*H';write_test_alfa(a_alfa,0);write_test_ln;
              end;
            end;
         end;
       DITMCO_9100:
         Begin
            j:=1;Max_T_Line:=27;
            case dd of
              d : begin                
                    if display_lines>=MAX_DISPLAY_LINES then begin
                      display_lines:=0;
                      if display.line[1]=' ' then j:=2;
                      end
                    else begin
                      display_lines:=display_lines+1;
                    end;
                    Emit_Halt:=false;
                  end;
              ds: begin
                    Emit_Halt:=true;
                    display_lines:=0;
                  end;
            OTHERWISE
              err_mess:='Unimplemented mode in put_display';
              error(0);
            end;
            if Display.Length=0 then begin
              if not Emit_Halt then begin
                a_alfa:='*T';write_test_alfa(a_alfa,0);write_test_ln;
              end;
              end
            else begin
              Emit_Ditmco_Lines(display.line,j,Display.length);
            end;
            if Emit_Halt then begin
              a_alfa:='*H';;write_test_alfa(a_alfa,0);write_test_ln;
            end;
         end;
     end;
 end;                                

{********************************************************************}
procedure Check_Wait_Manual;
 begin
     if wait_manual then begin
       display.line:=' ';
       display.length:=0;
       wait_manual:=false;
       put_display(ds,display);
     end;
     if annotate then begin
       put_display(da,annotation);
       annotation.length:=0;
       annotate:=false;
     end;
 end;

{**********************************************************************}
procedure Add_Node(bl:integer; Node_Type:Node_Types; add:integer);
label 99;                  
var i,l     :integer;
    b       :alfa;
    b2      :alfa2;
    n       :name;
    cc      :char;
begin             
      Check_Wait_Manual;
      Display_Lines:=0;
      case Target_Machine of
        Fact_Machine:
            Begin
               Case Node_Type of
                  Any_From  , Diode_From,
                  Cont_From , Open_From, 
                  Power_From, Meas_From   : cc:='I';

                  DC_I_From , AC_I_From   : cc:='F';
                                       
                  Cont_To   , Open_To ,
                  Meas_To   , Diode_To, 
                  Any_to                  : cc:='T';
                  
                  Power_To                : cc:='V';

               OTHERWISE
                  err_mess:='BUG! BUG! Add_Node BUG!';
                  error(0);goto 99;
               end;
              

               for i:=1 to bl do write_test_c(' ');
               if add=no_node then begin
                 Err_Mess:='Internal error. Nonexistent node specified';
                 error(0);goto 99;
                 end
               else if not ext_node(add) then begin
                 Err_Mess:='Internal error. Negative address specified';
                 error(0);goto 99;
                 end
               else begin
                 if add=0 then i:=0 else i:=Mach_Add(add);
               end;
               b:=Number_To_Alfa(i,5);    
               write_test_c(cc);
               for i:=1 to 5 do begin     
                 write_test_c(b[i]);
               end;

               n:=Int_To_Name(add);
               make_pin_name(n.dev,n.pin,b2,l);

               if l>10 then begin
                 err_mess:='conn-pin name is longer than 10 characters:';
                 err_p1:=n.dev;err_p2:=n.pin;warn(2);
               end;

               write_test_blanks(3);write_test_c('%');
               for i:=1 to l do write_test_c(b2[i]);
               write_test_c('&');write_test_ln;
    
               if (cc='T') or (cc='F') then begin
                 case wc_mode of
                   wc_first:begin
                              if add_wc then begin
                                write_test_mess(6,mesaj);
                              end;
                            end;
                   wc_all:begin
                              write_test_mess(6,mesaj);
                            end;
                   wc_none:{none};
                 end;
                 add_wc:=false; 
               end;
            end {target=fact};
        DITMCO_660:
            Begin
               if not ext_node(add) then begin
                 Err_Mess:='Internal error. Negative address specified';
                 error(0);goto 99;
                 end
               else begin
                 if add=0 then i:=0 else i:=Mach_Add(add);
               end;             
               b:=Number_To_Alfa(i,5);    
               Case Node_Type of
                  Any_From  , Diode_From,
                  Cont_From , Open_From, 
                  Power_From, Meas_From   : cc:='X'; { No parameters }

                  DC_I_From               : cc:='F';
                  AC_I_From               : cc:='S';
                  Cont_To,Any_To          : cc:='C';
                  Open_To                 : cc:='L';
                  Diode_To                : cc:='K';{'K' no parameters DIODE}
                  Power_To                : cc:='?';
                                                    {'V' no parameters DCV}
                                                    {'W' no parameters ACV}
                                                    {'J' no parameters RES}
                                                    
                                              
               OTHERWISE
                  err_mess:='BUG! BUG! Add_Node BUG!';
                  error(0);goto 99;
               end;

               write_test_c('*');write_test_c(cc);
               for i:=1 to 5 do begin     
                 write_test_c(b[i]);           
               end;

               case cc of { parameter section }
                  'C','L','F','S':
                       begin
                          for i:=1 to 5 do write_test_c(DITMCO_Param[i]);
                       end;
               OTHERWISE
                       write_test_blanks(5);
               end;

               case cc of  { English description }
                  'X':begin
                        a_alfa2:='  LOAD OUTPUT REGISTER';
                        write_test_alfa2(a_alfa2,0);
                        write_test_blanks(17);
                      end;
                  'K':begin
                        a_alfa2:='  DIODE TEST';
                        write_test_alfa2(a_alfa2,0);
                        write_test_blanks(27);
                      end;
               OTHERWISE
                 write_test_mess(0,mesaj);
               end;
               write_test_blanks(2);write_test_c(cc);write_test_blanks(1);

               n:=Int_To_Name(add);
                        
               write_test_alfa(n.dev,10);
               write_test_alfa(n.pin,10);
               write_test_ln;
            end {target=Ditmco_660};
        DITMCO_9100:
            Begin
               if not ext_node(add) then begin
                 Err_Mess:='Internal error. Negative address specified';
                 error(0);goto 99;
                 end
               else begin
                 if add=0 then i:=0 else i:=Mach_Add(add);
               end;
               b:=Number_To_Alfa(i,5);    
               Case Node_Type of
                  Any_From  , Diode_From,
                  Cont_From , Open_From, 
                  Power_From, Meas_From   : cc:='X'; { No parameters }

                  DC_I_From               : cc:='F';
                  AC_I_From               : cc:='S';
                  Cont_To  ,Any_To        : cc:='C';
                  Open_To                 : cc:='L';
                  Diode_To                : cc:='K';{'K' no parameters DIODE}
                  Power_To                : cc:='?';
                                                    {'V' no parameters DCV}
                                                    {'W' no parameters ACV}
                                                    {'J' no parameters RES}
                                                    
                                              
               OTHERWISE
                  err_mess:='BUG! BUG! Add_Node BUG!';
                  error(0);goto 99;
               end;

               write_test_c('*');write_test_c(cc);
               for i:=1 to 5 do begin     
                 write_test_c(b[i]);           
               end;

               case cc of { parameter section }
                  'C','L','F','S':
                       begin
                          for i:=1 to 5 do write_test_c(DITMCO_Param[i]);
                       end;
               OTHERWISE
                       write_test_blanks(5);
               end;

               case cc of  { English description }
                  'X':begin 
                        a_alfa2:='  LOAD OUTPUT REGISTER';
                        write_test_alfa2(a_alfa2,0);
                        write_test_blanks(17);
                      end;
                  'K':begin
                        a_alfa2:='  DIODE TEST';
                        write_test_alfa2(a_alfa2,0);
                        write_test_blanks(27);
                      end;
               OTHERWISE
                 write_test_mess(0,mesaj);
               end;
               write_test_blanks(2);
               write_test_c(cc);
               write_test_blanks(1);

               n:=Int_To_Name(add);

               write_test_alfa(n.dev,al);
               write_test_alfa(n.pin,al);
               write_test_ln;
            end {target=Ditmco_9100};
        OTHERWISE
              err_mess:='Add_Node:Unimplemented Target.';
        end;

99:;
end;              

{*****************************************************************}
procedure tag_cut_set(n1,n2:integer);
    procedure tag_node_link(t_node:node_link);
     { This procedure will tag an arc as tested if it is a    } 
     { contact.                                               }

     begin
         with t_node^ do begin                                      
           case cnx of
             contact:
               begin
                 o_c:=YES;other^.o_c:=YES;
               end;
           OTHERWISE
           end;
         end;
     end;

     procedure tag_cut(n:integer);
     var t_node:node_link;

     begin
         t_node:=adj[n];
         while t_node<>z_node do begin
           with t_node^ do begin
             if ext_node(inf[v]^.x_node) then begin
               if inf[v]^.x_node<inf[n]^.x_node then begin
                 tag_node_link(t_node);
               end;
             end;
           end;
           t_node:=t_node^.next;
         end;
     end;


 begin
     bfs_Node(n1,tag_cut,short_path);
 end;
 
{********************************************************************}
procedure fs_check(i:integer;incremental:boolean;what:wish);
 var already_checked,testable:boolean;
     Node_Type:Node_Types;                 
 begin             
      case what of
        fc,fci:begin
                 already_checked:=inf[i]^.f_c=checked;
                 Node_Type:=DC_I_From;
               end;               
        sc,sci:begin
                 already_checked:=inf[i]^.s_c=checked;               
                 Node_Type:=AC_I_From;
               end;
      OTHERWISE                      
        err_mess:='Internal error in fs_check.Bad MODE';
        error(0);goto 8888;
      end;

      if in_use(i) then begin
        if incremental and already_checked then begin
          { Null statement }
          end
        else begin
          case target_machine of
            FACT_Machine:testable:=true;
            DITMCO_660  :testable:=mach_add(i)<>0;
            DITMCO_9100 :testable:=mach_add(i)<>0;
          OTHERWISE
            err_mess:='Internal error!!BAD Target Machine in fs_check.';
            error(0);goto 8888;
          end;
          case what of
            fc,fci:inf[i]^.f_c:=checked;
            sc,sci:inf[i]^.s_c:=checked;
          end;
          if testable then begin
            Add_Node(6,Node_Type,i);
            tag_cut_set(i,NO_NODE);
          end;
        end;
        end       
      else begin  
        err_mess:='Internal error in fs_check. Illegal node';
        error(0);
      end;        
 end;             
                  
{*****************************************************************}
 function switch_is(ss:charset):boolean;
  var i:integer;
  begin
      switch_is:=false;
      for i:=1 to num_switches do begin
        if switches[i].switch in ss then switch_is:=true;
      end;
  end;
                

{*****************************************************************}
 procedure fs_check_pin(v:name;incremental:boolean;what:wish;report:boolean);
  type modes=(current,lowest);
            
  var i,n,n1:integer;
      mode:modes;
      allow:Omit_Flag;
      already_checked:boolean;
  begin
      n:=Address_of(v);mode:=lowest;{ was CURRENT}
      if good(n)then begin
        if switch_is( ['L','l'] ) then mode:=lowest;
        if switch_is( ['F','f'] ) then mode:=current;
        allow:=inf[n]^.omit;
        already_checked:=inf[n]^.dfs_Visit>FS_Search_Start;
        if mode=lowest then begin
          n:=l_node(n,allow,false);
          end
        else begin
          n1:=l_node(n,allow,false);
        end;
        if ext_node(n) then begin
          if ord(allow) = ord(NO_OMIT) then begin
            if not already_checked then begin
              fs_check(n,incremental,what);
            end;
            end
          else begin
            if report then begin
              err_mess:='Attempting check for an omitted pin:';
              err_p1:=v.dev;err_p2:=v.pin;error(2);
            end;
          end;
          end
        else if report then begin
          err_mess:=' No external path to this node:';
          err_p1:=v.dev;err_p2:=v.pin;error(2);
        end;
        end                                   
      else begin
        err_mess:='F Check pin not found:';
        err_p1:=v.dev;err_p2:=v.pin;
        error(2);
      end;
  end;

{*****************************************************************}
  procedure f_check_pin(v:name;b,r:boolean);
  begin
     fs_check_pin(v,b,fc,r);
  end;
  
{*****************************************************************}
  procedure s_check_pin(v:name;b,r:boolean);
  begin                        
     fs_check_pin(v,b,sc,r);
  end;

{*****************************************************************}
 procedure omit_node(v:integer;flag:omit_flag);
  begin
      if ord(inf[v]^.omit) < ord(HARD_OMIT) then begin
        inf[v]^.omit:=flag;
      end;
  end;
{*****************************************************************}
 procedure omit_pin(v:name;b,r:boolean);
  var i,n:integer;
      flag:omit_flag;

  begin
      n:=Address_of(v);
      if good(n)then begin
        flag:=SOFT_OMIT;
        for i:=1 to num_switches do begin
          if switches[i].switch in ['H','h'] then flag:=HARD_OMIT;
        end;
        omit_node(n,flag);
        end
      else if r then begin
        err_mess:='Omit pin not found:';
        err_p1:=v.dev;err_p2:=v.pin;                                 
        error(2);
      end;
  end;
           
{*****************************************************************}
 procedure unomit_pin(v:name;b,r:boolean);
  var n:integer;
  begin
      n:=Address_of(v);
      if good(n)then begin
        if ord(inf[n]^.omit) < ord(HARD_OMIT) then begin
          inf[n]^.omit:=NO_OMIT;
          end
        else if r then begin
          err_mess:='Can not UnOmit a HARD_OMIT pin:';
          err_p1:=v.dev;err_p2:=v.pin;
          warn(2);
        end;
        end
      else begin
        err_mess:='UnOmit pin not found:';
        err_p1:=v.dev;err_p2:=v.pin;
        error(2);
      end;                     
  end;

{*****************************************************************}
 procedure stub_node(v:name;b,r:boolean);
  var i,n:integer;

  begin
      n:=Address_of(v);
      if good(n)then begin
        inf[n]^.status:=stubbed ;
        end
      else if r then begin
        err_mess:='Stub pin not found:';
        err_p1:=v.dev;err_p2:=v.pin;                                 
        error(2);
      end;
  end;
           
{**************************************************************}
 procedure process_range(     Conn_Name:alfa;
                                tch:char;
                             procedure symget;
                             procedure process_pin(v:name;b,r:boolean);
                             flag,report:boolean);
 label 99;
 var more:boolean;
     f,f1,f2:alfa;
     v:name; 
     acceptable_save:charset;
                                                               
  begin
         f1:=NULL;v.dev:=Conn_Name;
         acceptable_save:=acceptable;
         acceptable:=acceptable-['-'];
         symget;More:=true;             
  
         while More do begin
           if a[1]=tch then begin
             More:=false;
             end
                                        
           else begin

             { preliminary filter to ensure we don't pick a ghost pin}
             if a[1]<>'-' then f1:=NULL;

             case a[1] of               
                '-':begin
                      if f1<>NULL then begin{ make sure we saw range start}
                        symget;
                        f2:=a;         
                        f:=next_pin(f1,f2,0);
                        if not OK then goto 99;
                        if (f1<>f2) { preliminary check }then begin
                          f:=f1;
                          repeat
                            f:=next_pin(f,f2,0);
                            if not OK then goto 99;
                            v.pin:=f;
                            process_pin(v,flag,report);
                          until (f=f2);
                        end;
                        end
                      else begin
                        err_mess:='range start pin is missing';
                        error(0);goto 99;
                      end;
                      f1:=NULL;symget;
                    end;

             OTHERWISE { must be symbol }
                    if symbol then begin
                      f1:=a;v.pin:=f1;
                      process_pin(v,flag,report);
                      if not OK then goto 99;
                      end
                    else begin
                      err_mess:=' Bad pin :';
                      err_p1:=a;
                      error(1);goto 99;
                    end;
                    symget;
             end { CASE }; 
           end;                      
         end; { while more do }
99:  acceptable:=acceptable_save;
     end;                                                      



{*********************************************************************}
 procedure Get_Device_Pins(     Device_Name:alfa;
                           var     Curr_Add:integer;
                                term:char );

 label 99;      
 var jumper,more,connected:boolean;
     Prev_Address,x,y:Integer;
     f,f1,f2:alfa;
     First_pin:Boolean;
     i:integer; { loop counter variable }


  begin
         jumper:=false;Prev_Address:=NO_NODE;           
         f1:=NULL;first_pin:=true;
         getnext;More:=true;             
  
         while More do begin
           if a[1]=term then begin
             if jumper then begin
               err_mess:=' Mismatched right parenthesis';
               error(0);goto 99;
               end
             else begin
               More:=false;
             end;
             end
                                                     
                                        
           else begin

             { preliminary filter to ensure we don't pick a ghost pin}
             if a[1]<>'-' then f1:=NULL;

             case a[1] of                     
                '-':begin
                      if f1<>NULL then begin{ make sure we saw range start}
                        getnext;
                        f2:=a;
                        f:=next_pin(f1,f2,0);
                        if not OK then goto 99;
                        if (f1<>f2) { preliminary check }then begin
                          f:=f1;
                          repeat
                            f:=next_pin(f,f2,0);
                            if OK then begin
                              Get_Next_Address(Curr_Add,1,First_Pin);
                              insert_node(Device_Name,f,Curr_Add);
                              if not OK then goto 99;
                              if jumper then begin
                                if Prev_Address<>NO_NODE Then begin
                                  x:=Source_To_Int_Add(Prev_Address);
                                  y:=Source_To_Int_Add(Curr_Add);
                                  connect_wire(x,y,wire_imp,connected);
                                  Prev_Address:=Curr_Add;
                                  end
                                else begin
                                  err_mess:=' Internal Jumper error.REPORT';
                                  error(0);
                                end;
                              end;             
                            end;
                            if not OK then goto 99;
                          until (f=f2);
                        end;
                        end
                      else begin
                        err_mess:='range start pin is missing';
                        error(0);goto 99;
                      end;
                      f1:=NULL;getnext;
                    end;

                '(':begin
                      if not jumper then begin
                        jumper:=true;Prev_Address:=NO_NODE;
                        end
                      else begin
                        err_mess:= ' "(" is out of sequence';
                        error(0);goto 99;
                      end;
                      getnext;
                    end;

                ')':begin    
                      if jumper and more then begin
                        jumper:=false;
                        end
                      else begin
                        err_mess:= ' ")" is out of sequence';
                        error(0);goto 99;
                      end;
                      getnext;
                    end;

             OTHERWISE { must be symbol }
                    if symbol then begin
                      f1:=a;
                      Get_Next_Address(Curr_Add,1,First_Pin);
                      insert_node(Device_Name,f1,Curr_Add);
                      if not OK then goto 99;
                      if jumper then begin
                        if prev_Address<>NO_NODE then begin
                          x:=Source_To_Int_Add(Prev_Address);
                          y:=Source_To_Int_Add(Curr_Add);
                          connect_wire(x,y,wire_imp,connected);
                        end;
                        prev_Address:=Curr_Add;{ remember for ties}
                      end;
                      end
                    else begin
                      err_mess:=' Bad pin :';
                      err_p1:=a;
                      error(1);goto 99;
                    end;
                    getnext;
             end { CASE };
           end;
         end; { while more do }
         { set up the return value of address}
         Get_Next_Address(Curr_Add,1,First_Pin);
99:  end;
{**************************************************************}
 procedure Get_Range(     Conn_Name:alfa;
                                tch:char);
 label 99;            
 var jumper:jumper_case;
     more,connected:boolean;
     Prev_Address,x,y:Integer;
     f,f1,f2:alfa;
     Increment:integer;
     i:integer; { loop counter variable }
     Range_Pin:Range_Pointer;
     sensed:boolean;

  Procedure Push_Pin(Pin_Name:alfa;del:integer;jmpr:jumper_case;rng:Boolean);
   var Temp_Ptr:range_pointer;
   begin
       Temp_ptr:=Range_List;
       While Temp_Ptr^.Valid do begin
         If Temp_Ptr^.Range_Data.RT=Pin_Data then begin
           If Pin_Name=Temp_Ptr^.Range_Data.n then begin
             Err_Mess:='Pin is doubly defined in brackets :';
             Err_P1:=Pin_Name;error(1);goto 99;
           end;
         end;
         Temp_Ptr:=Temp_Ptr^.next;
       end;
  
       With Range_Pin^ Do Begin
         With Range_Data do Begin
           RT:=Pin_Data;
           n:=Pin_Name;
           delta:=del;
           jumper:=jmpr;
           Range:=rng;
           Sense:=sensed;
         end;
         Valid:=True;
       end;
  
       if Range_Pin^.next=NIL then begin
         new(Range_Pin^.next);
         Range_Pin^.next^.next:=NIL;
       end;
  
       Range_Pin:=Range_Pin^.next;
       Range_Pin^.Valid:=False;
       If Jumper=Jumper_Begin then Jumper:=Jumpering;
   end;
  
  Procedure Push_Skip(Skip:integer);
   begin
       With Range_Pin^ Do Begin
         With Range_Data do Begin                   
           RT:=Skip_Data;
           delta:=Skip;
         end;
         Valid:=True;
       end; 

       if Range_Pin^.next=NIL then begin
         new(Range_Pin^.next);
         Range_Pin^.next^.next:=NIL;
       end;
  
       Range_Pin:=Range_Pin^.next;
       Range_Pin^.Valid:=False;
   end;
  
  Procedure Push_Split;
   begin
       With Range_Pin^ Do Begin
         With Range_Data do Begin                   
           RT:=Split_Pin;
         end;
         Valid:=True;
       end; 

       if Range_Pin^.next=NIL then begin
         new(Range_Pin^.next);
         Range_Pin^.next^.next:=NIL;
       end;
  
       Range_Pin:=Range_Pin^.next;
       Range_Pin^.Valid:=False;
   end;
  
  begin
         Range_Pin:=Range_List;Sensed:=false;
         jumper:=No_Jumper;Prev_Address:=NO_NODE;Increment:=1;f1:=NULL;
         getnext;More:=true;
         while More do begin
           if a[1]=tch then begin
             if jumper in [jumper_begin,jumpering] then begin
               err_mess:=' Mismatched right parenthesis';
               error(0);goto 99;
               end
             else begin
               More:=false;
             end;
             end
           else begin

             case a[1] of               
                '-':begin
                      if f1=NULL then begin{ make sure we saw range start}
                        err_mess:=' Bad Pin range ( no starting pin)';
                        err_p1:='-';error(1);goto 99;
                      end;
                      getnext; f2:=a; f:=f1;
                      Push_Pin(f,Increment,jumper,true);
                      if not OK then goto 99;
                      while (f<>f2) do begin
                        f:=next_pin(f,f2,0);
                        if not OK then goto 99;
                        Push_Pin(f,Increment,jumper,true);
                      end;
                      f1:=NULL;getnext;
                    end;
  
                '(':begin
                      if jumper=No_Jumper then begin
                        jumper:=Jumper_Begin;Prev_Address:=NO_NODE;
                        end
                      else begin
                        err_mess:= ' "(" is out of sequence';
                        error(0);goto 99;
                      end;
                      getnext;
                    end;
  
                ')':begin
                      if(jumper in [jumper_begin,jumpering]) and more then begin
                        jumper:=No_Jumper;
                        end
                      else begin
                        err_mess:= ' ")" is out of sequence';
                        error(0);goto 99;
                      end;
                      getnext;
                    end;
                '.':begin
                      increment:=Alfa_to_Number(a,2,alfa_length(a));
                      if increment=0 then increment:=1;{ do not allow 0}
                      if increment=2 then begin
                        sensed:=true;
                        end
                      else begin
                        sensed:=false;
                      end;
                      getnext; 
                    end;
                '\':begin
                      If together then begin
                        getnext;
                        if good_integer(a,1,alfa_length(a)) Then begin
                          Push_Skip(alfa_to_number(a,1,alfa_length(a)));
                          getnext;{consumed}
                          end
                        else begin{ do not consume just increment}
                          Push_Skip(1);
                        end;
                        end
                      else begin {just a \ }
                        Push_Skip(1);        
                        getnext;{consumed}
                      end;
                    end;
                '^':begin
                      Push_Split;        
                      getnext;{consumed}
                    end;


             OTHERWISE { must be symbol }
                    
                    if symbol then begin
                      if ch='-' then begin
                        { range start. defer until next pass}
                        f1:=a;
                        end
                      else begin
                        f1:=a;
                        Push_Pin(f1,Increment,jumper,false);f1:=NULL;
                      end;
                      end
                    else begin
                      err_mess:=' Bad pin :';
                      err_p1:=a;
                      error(1);goto 99;
                    end;
                    getnext;
             end { CASE };
           end;
         end; { while more do }
99:  end;      

{****************************************************************}
function unit(aa:alfa):units;
 var bb,cc: alfa; 
     u:units;     
     fc:char;     
     i:integer;   
 begin            
      if aa[1] in ['u','U','M','m','K','k'] then begin         
        fc:=aa[1];
        for i:=1 to al-1 do bb[i]:=aa[i+1];
        bb[al]:=' ';
        u:=unit(bb);
        if (fc='u')or (fc='U') then begin
          case u of
            ADC  : unit:=uADC;
            AAC  : unit:=uAAC;
          OTHERWISE                   
            Unit:=BadUnit;
          end;    
          end     
        else if (fc='M')or (fc='m') then begin
          case u of
            Ohm  : unit:=MOhm;
            VDC  : unit:=mVDC;
            VAC  : unit:=mVAC;
            ADC  : unit:=mADC;
            AAC  : unit:=mAAC;
            Sec  : unit:=mSec;
            Watt : unit:=mWatt;
          OTHERWISE                   
            Unit:=BadUnit;
          end;    
          end     
        else if (fc='K')  or (fc='k') then begin
          case u of
            Ohm  : unit:=KOhm ;
            VDC  : unit:=KVDC ;
            VAC  : unit:=KVAC ;
          OTHERWISE
            Unit:=BadUnit;
          end;    
        end;              
        end       
      else begin  
        cc:=upperalfa(aa);
        if      cc=' '     then unit:=Ohm
        else if cc='OHM'   then unit:=Ohm
        else if cc='OHMS'  then unit:=Ohm
        else if cc='V'     then unit:=VDC
        else if cc='VOLT'  then unit:=VDC
        else if cc='VOLTS' then unit:=VDC
        else if cc='VDC'   then unit:=VDC
        else if cc='VAC'   then unit:=VAC
        else if cc='A'     then unit:=ADC
        else if cc='AMP'   then unit:=ADC
        else if cc='AMPS'  then unit:=ADC
        else if cc='ADC'   then unit:=ADC
        else if cc='AAC'   then unit:=AAC
        else if cc='WATTS' then unit:=Watt
        else if cc='WATT'  then unit:=Watt
        else if cc='W'     then unit:=Watt
        else if cc='SEC'   then unit:=Sec
        else if cc='SECS'  then unit:=Sec
        else if cc='PCT'   then unit:=Pct
        else if cc='PCNT'  then unit:=Pct
        else if cc='RAMP'  then unit:=Ramp
        else if cc='%'     then unit:=Pct
        else                    unit:=BadUnit;
                  
      end;        
end;              

{**************************************************************}
procedure getv;   
  begin           
    getsym_with(['-']);       
    z.a:=a;       
    z.a_right:=a_right;
    z.float:=float;
    z.flo_num:=flo_num;
    z.split:=split;
 end;             

{*******************************************************************}
 procedure get_parm(      required:boolean;
                    var      z1,z2:float_v;
                        Good_Units:unit_set;
                    var          v:real;
                    var          u:units);
                  
  var un:units;   
                  
  begin           
      if z1.flo_num then begin
        if z2.a=NULL then begin
          getv;z2:=z;
        end;      
                  
        un:=unit(z2.a);
        if un in good_units then begin
          v:=z1.float; { consume z1}
          u:=un;       {consume z2}
          getv;z1:=z;
          z2.a:=NULL;
          end     
        else if un<>BadUnit then begin
          if required then begin
            Err_Mess:='Wrong units:';
            Err_P1:=z1.a;Err_P2:=z2.a;error(2);
            getv;z1:=z;z2.a:=NULL;
            end   
          else begin
            { do not consume but save for later }
          end;      
          end            
        else { Units is missing} begin
          Err_Mess:='Units is missing for:';
          Err_P1:=z1.a;error(1);
          z1:=z2;{ skip over z1 and continue on}
          z2.a:=NULL;
        end;      
        end       
                  
      else if z1.split then begin
        un:=unit(z1.a_right);
        if un in good_units then begin
          v:=z1.float; { consume z1}
          u:=un;       {consume z1}
          if z2.a=NULL then begin  getv;z2:=z; end;
          z1:=z2; 
          z2.a:=NULL;
          end     
                  
        else if un<>BadUnit then begin
          if required then begin
            Err_Mess:='Wrong units:';
            Err_P1:=z1.a;error(1);
            getv;z1:=z;z2.a:=NULL;
            end   
          else begin
            { do not consume but save for later }
          end;    
          end     
        else begin
          err_mess:='Units is missing from:';
          err_p1:=z1.a;
          error(1);
        end;      
        end       
      else if required then begin
        Err_Mess:='Required Command parameters are missing';
        error(0); 
      end;        
  end;            


{********************************************************************}
procedure normalize(v1:real;u1:units;var v2:real;var u2:units);
                                
  Begin           
      if u1 in [mVDC,mVac,mADC,mAAC,mSec] then begin
        v2:=v1*1e-3;
        case u1 of
           mVDC: u2:=VDC;
           mVac: u2:=Vac;
           mADC: u2:=ADC;
           mAac: u2:=AAC;
           mSec: u2:=Sec;
        end;      
        end       
      else if u1 in [KOhm,KVDC,KVAC] then begin
        v2:=v1*1e3;
        case u1 of 
           KOhm: u2:=Ohm;
           KVDC: u2:=VDC;
           KVac: u2:=Vac;
        end;      
        end       
      else if u1 in [MOhm] then begin
        v2:=v1*1e6;
        u2:=Ohm;  
        end       
      else begin  
        v2:=v1;   
        u2:=u1;   
      end;        
                  
 end;             

{**********************************************************************}
procedure get_state(             d:dev_link;
                          pos_name:alfa;
                         long_name:alfa4;
                     var state_ptr:state_link;
                     var   ret_pos:alfa);
                     
var t_state,long_state:state_link;

begin
       if d^.part_type in [Switch,Relay] then begin
         t_state:=d^.s;state_ptr:=NIL;long_state:=NIL;ret_pos:=' ';
         while t_state<>NIL do begin
           if t_state^.desc=pos_name then begin
             state_ptr:=t_state;
           end;        
           if long_name=t_state^.name then begin
             if long_name<>' ' then long_state:=t_state;
           end;
           t_state:=t_state^.next;
         end;
         if state_ptr=NIL then state_ptr:=long_state;

         if state_ptr<>NIL then begin
           ret_pos:=state_ptr^.ret;{remember just in case}
         end;
         end
       else begin                              
         err_mess:=' Device is not a Relay or Switch';
         error(0);
       end;
end;


{**********************************************************************}
procedure switch_state(part_type:part_types;
                       swtch:alfa;to_pos:alfa;
                       long_name:alfa4;
                  var old_pos:alfa);
label 99;
var this_state,next_state:state_link;
    t_pair:pair_link;
    d:dev_link;
    ret_pos:alfa;
    long_alfa:alfa4;

 procedure disconnect_device_contacts(x,y:integer);
    var xlo,ylo:integer;
    begin                                       
        disconnect_pair(x,y);
        xlo:=inf[x]^.R_node;ylo:=inf[y]^.R_node;
        if (inf[xlo]^.forced=yes) or (inf[ylo]^.forced=yes) then begin
          Enqueue_maybe(x,live_QUEUE);
          Enqueue_maybe(y,live_QUEUE);
        end;
    end;

 procedure connect_device_contacts(x,y:integer);
    begin
        connect_pair(x,y);
        if inf[inf[x]^.x_node]^.forced=yes then begin
          Enqueue_maybe(x,live_QUEUE);
        end;
    end;




begin                                              
     d:=device_pointer(swtch);{get the device pointer}
     if d<>z_dev then begin
       if part_type=Unknown then Part_Type:=d^.part_type;
       long_alfa:=' ';{ just a dummy field to supply to get_state}

       {get current state info}
       get_state(d,d^.curr_state,long_alfa,this_state,ret_pos);
       if not OK then goto 99;

       if to_pos=NULL then begin {see if there is a return position}
         if this_state<>NIL then begin
           if this_state^.oper='>' then begin
             switch_state(part_type,swtch,this_state^.ret,long_name,old_pos);
             goto 99;
             end
           else begin
             Err_mess:=' This position does not have a return:';
             Err_P1:=to_pos;
             error(1);goto 99;
           end;
           end
         else begin
           err_mess:='**Internal error**. NIL This_state';
           error(0);goto 99;
         end;
         end
       else begin
         get_state(d,to_pos,long_name,next_state,ret_pos);
         if not OK then goto 99;
         if next_state=NIL then begin
           err_mess:=' No such state for this relay/switch';
           error(0);goto 99;
         end;
       end;
       
       {DISCONNECT Old Position}
       old_pos:=d^.curr_state;           
       if d^.curr_state<>' ' then{something to disconnect} begin
         if this_state<>NIL then {all is OK} begin
           if next_state<>NIL then begin
             if this_state<>next_state then begin
               t_pair:=this_state^.c;
               while t_pair<>NIL do begin
                 disconnect_device_contacts(t_pair^.x,t_pair^.y);
                 t_pair:=t_pair^.next; 
               end;
             end;
           end;
           end
         else begin
           err_mess:='WITS Internal error NIL this_state.REPORT';
           error(0);goto 99;
         end;
       end;


       {CONNECT New position}
       if next_state<>NIL then begin
         if this_state<>next_state then begin
           t_pair:=next_state^.c;
           while t_pair<>NIL do begin
             connect_device_contacts(t_pair^.x,t_pair^.y);
             t_pair:=t_pair^.next;
           end;
           d^.curr_state:=next_state^.desc;
         end;
         end
       else begin
         err_mess:='No such position for this switch';
         error(0);goto 99;
       end;
       end
     else begin
       err_mess:=' No such device:';
       err_p1:=swtch;error(1);goto 99;
     end;                     
 99:;
end;

{**********************************************************************}
procedure get_node(bb:alfa;var v:name);
var p,i:integer;  
    aa:alfa;      
  begin { parse device-pin  into v.dev and v.pin ; return No_Pin if no pin }
     if bb=NULL then begin
       getsym_with(['-']);    
       aa:=a;     
       end        
     else begin   
       aa:=bb;    
     end;                                 
                  
     v.dev:=' ';p:=al+1;                                                
                  
     for i:=1 to al do if aa[i]='-' then if p>i then p:=i;{find first '-'}
     for i:=1 to p-1 do v.dev[i]:=aa[i];
     upshift(v.dev);
                  
     v.pin:=No_Pin;for i:=p+1 to al do v.pin[i-p]:=aa[i];{remainder in pin}
                  
  end;              
{**********************************************************************}
procedure get_equiv(required:boolean;bb:alfa;var v:name);
var p,i:integer;  
    aa:alfa;      
  begin { parse device-pin  into v.dev and v.pin ; return No_Pin if no pin }
     aa:=' ';
     if bb=NULL then begin
       if ch='=' then begin
         getsym;{throw it away}
         getsym_with(['-']);    
         aa:=a;
         end
       else if required then begin
         err_mess:=' = expected ';
         error(0);
       end;     
       end        
     else if bb='=' then begin 
       getsym_with(['-']);
       aa:=a; 
       end   
     else if required then  begin
       err_mess:=' = expected ';
       error(0);   
     end;                                 
                  
     v.dev:=' ';p:=al+1;                                                
                  
     for i:=1 to al do if aa[i]='-' then if p>i then p:=i;{find first '-'}
     for i:=1 to p-1 do v.dev[i]:=aa[i];
     upshift(v.dev);
                  
     v.pin:=No_Pin;for i:=p+1 to al do v.pin[i-p]:=aa[i];{remainder in pin}
                  
  end;              
                  
{**********************************************************************}
procedure getnext_node(bb:alfa;var v:name);
var p,i:integer;  
    aa:alfa;      
  begin { parse device-pin  into v.dev and v.pin ; return No_Pin if no pin }
     if bb=NULL then begin
       getnext_with(['-']);    
       aa:=a;     
       end        
     else begin             
       aa:=bb;    
     end;                                 
                  
     v.dev:=' ';p:=al+1;                                                
                  
     for i:=1 to al do if a[i]='-' then if p>i then p:=i;{find first '-'}
     for i:=1 to p-1 do v.dev[i]:=aa[i];
     upshift(v.dev);
                  
     v.pin:=No_Pin;for i:=p+1 to al do v.pin[i-p]:=aa[i];{remainder in pin}
                  
  end;              
                  

{************************************************************************}
procedure get_parts;
label 99,999;     
var n1,n2,n3,x,y:integer;
    f,f1,f2,f3:alfa;
    conn_name,old_name:alfa;
    conn_add,Top_Pin:integer;
    strip_pin,strip_loc,strip_bank:integer;
    strip_size,used_strip_pins:integer;
    cont_line:boolean;
    conn_1,conn_2,a_dev:alfa;
    section_message_save:str79;
    section_save:sections;
    acceptable_save:charset;

  procedure insert_part_pin(Conn_Name:alfa;Pin_Name:alfa;s_Address:integer);

    begin
        insert_node(Conn_Name,Pin_Name,s_Address);
        if STRIP then begin
          if OK and (strip_size>0) then begin
            if ext_node(s_address) then begin
              if used_strip_pins<strip_size then begin
                used_strip_pins:=used_strip_pins+1;
                end
              else begin
                err_mess:='Number of pins>strip size:';
                err_p1:=Conn_Name;err_p2:=Pin_Name;
                error(2);
              end;
            end;
          end;
        end;
    end;
  function end_line:boolean;
   begin
      end_line:=(a=' ');
   end;

  function insert_internal_pin(dev:alfa;pin:alfa):integer;
   var first_pin:boolean;
   var v:name;
   begin  
      insert_part_pin(dev,pin,Next_TB_Address);
      Next_TB_Address:=Next_Address(Next_TB_Address,1);
      Curr_Add:=Next_TB_Address;
      v.dev:=dev;v.pin:=pin;
      insert_internal_pin:=Address_Of(v);
   end;
                     
   {-------------------------------------------------------------}
   Procedure Get_Range_Conn(    Conn_Name:Alfa;
                                      var Curr_Add :Integer);
  
   label 99;
   type
         Ignore_ptr= ^Ignore_Pin;
         Ignore_Pin= Record
                       Pin:alfa;
                       next:ignore_ptr;
                     end;
   var
         Ignore,Ignore_Head,Temp_Ptr:Ignore_Ptr;
         Must_Process,connected:Boolean;
         first_pin:Boolean;
         Increment,Prev_Address:Integer;
         del:integer;
   Begin                               
        Prev_Address:=NO_NODE;
        first_pin:=true;Increment:=1;
        Range_List^.Valid:=False;
        get_range(conn_name,']');
        if NOT OK then goto 99;
        getsym;Ignore_Head:=NIL;
        while not end_line do begin
          if ignore_head=NIL then begin
            new(ignore_head);ignore:=ignore_head;ignore^.next:=NIL;
            end
          else begin
            new(ignore^.next);
            ignore:=ignore^.next;ignore^.next:=NIL;
          end;
          ignore^.Pin:=a;
          getsym;
        end;
  
        range_pin:=Range_List;Ignore:=Ignore_Head;
        While range_pin^.Valid do begin
          With range_pin^ do begin
            With range_data do begin        
              case RT of
                Pin_Data: 
                   Begin
                     if Ignore<>NIL then begin
                       if Ignore^.Pin=n then begin
                         if range then begin
                           { legitimate ignore pin}
                           end
                         else begin
                           err_mess:=' Ignore pin did not appear in a range:';
                           err_p1:=Ignore^.pin;{ proceed anyway }
                           error(1);
                         end;
                         Temp_Ptr:=Ignore;  
                         Ignore:=Ignore^.next;{ skip it}
                         Dispose(Temp_Ptr);
                         Must_Process:=false;
                         end
                       else begin     
                         Must_Process:=true;
                       end;
                       end
                     else begin
                       Must_Process:=true;
                     end;
                     if Must_Process Then begin
                       Get_Next_Address(Curr_Add,delta,first_pin);
                       Insert_part_Pin(Conn_Name,n,Curr_Add);
                       If OK then begin
                         If sense then begin
                           If not make_sensed(Conn_Name,n) then begin
                             err_mess:='Pin is not a 4-wire pin: ';
                             err_p1:=Conn_Name;err_p2:=n;
                             warn(2);
                           end;
                         end;
                         If Jumper=Jumpering then begin
                           If Prev_Address<>NO_NODE then begin
                             x:=Source_To_Int_Add(Prev_Address);
                             y:=Source_To_Int_Add(Curr_Add);
                             connect_wire(x,y,Wire_Imp,connected);
                           end;
                           Prev_Address:=Curr_Add;
                           end
                         else if Jumper=Jumper_Begin then begin

                           Prev_Address:=Curr_Add;
                           end
                         else begin
                           Prev_Address:=NO_NODE;
                         end;
                       end;
                     end;
                   end;               
  
                Skip_Data:
                    Begin
                      for del:=1 to delta do begin
                        Curr_Add:=next_address(curr_add,1);
                        if curr_add<0 then Next_TB_Address:=Curr_Add;
                      end;
                    end;

                Split_Pin:
                    Begin
                      Curr_Add:=New_block(curr_add);
                      if curr_add<0 then Next_TB_Address:=Curr_Add;
                    end;
              end;
            end;
          end;
          range_pin:=range_pin^.next;
        end;
        if Ignore<>NIL then begin
          err_mess:='Bad Ignore Pin(s) (or out of sequence): ';
          err_P1:=Ignore^.Pin;error(1);
          while Ignore<>NIL do begin 
            Temp_Ptr:=Ignore;
            Ignore:=Ignore^.next;
            dispose(Temp_Ptr);
          end;
        end;
        if first_pin then begin
          err_mess:=' No pins defined for this connector';
          warn(0);                 
          end
        else begin
          Curr_Add:=Next_Address(Curr_Add,1);
          if Curr_add<0 then Next_TB_Address:=Curr_Add;
        end;
99:end;                
                                             

{------------------------------------------------------------------}
   procedure add_Cable_Desc(Cable_Name:alfa2;
                              cab_conn,UUT_conn:alfa;
                              conn_add:integer);
     label 99;        
     const max_conn=200;
     var                         
       Pxxx,i:integer;
       t_adp_subdev:adp_subdev_link;
       t_adp_dev:adp_dev_link;
       t_info:info_link;
     procedure new_adp_dev(var t:adp_dev_link);
       begin
          new(t);
          with t^ do begin
            new(z_subdev);name:=' ';
            subdev:=z_subdev;next:=NIL;
            if STRIP  then begin
              tester_cnxs:=0;address:=MaxInt;
              end
            else if ZIF  then begin
              tester_cnxs:=0;address:=MaxInt;
              end
            else begin
              tester_cnxs:=Max_Conn+1;address:=New_Block(Curr_Add);
            end;
            STRIP_type:=STRIP;
            ZIF_type:=ZIF;
            UUT_cnxs:=0;Hooked_up:=false;
            if Conn_Add=NO_NODE then begin
              tester_hi:=-MAXINT;
              tester_Lo:=MAXINT;
              end
            else begin
              tester_lo:=Conn_Add  {adapter^.next_available};
              if STRIP then begin
                tester_hi:=strip_size;
                end
              else begin
                 {tester_hi:=tester_lo+PPC-1;} { old formula 2-04-1988}
                 tester_hi:=((tester_lo-1) div PPC)*PPC + PPC;
              end;
            end;
          end;                                            
       end;

  procedure close_cable_desc;
    var i:integer;
    begin
          if adapter<>NIL then begin
            with adapter^ do begin
              if OPEN=YES then begin
                Open:=NO;{close current Cable}
                with current^ do begin
                  if STRIP  then begin
                    top_pin:=tester_hi;
                    end
                  else if ZIF  then begin
                    tester_lo:=((tester_lo-1) div PPC)*PPC+1;
                    tester_hi:=((tester_hi-1) div PPC)*PPC+PPC;
                    top_pin:=tester_lo+tester_cnxs*PPC-1;
                    end
                  else begin
                    tester_lo:=((tester_lo-1) div PPC)*PPC+1;
                    tester_hi:=((tester_hi-1) div PPC)*PPC+PPC;
                    {top_pin:=tester_lo+tester_cnxs*PPC-1; del 11-07-91 }
                    top_pin:=address   +tester_cnxs*PPC-1;{ add 11-07-91 }
                  end;      
                  if top_pin>v_max then init_adj(top_pin);
                  for i:=tester_lo to top_pin do begin
                    if inf[i]=z_info then begin
                      new_info(i);
                      inf[i]^.status:=no_connex;
                    end;
                  end;
                  if STRIP then begin
                    Next_Available:=Next_Available+Strip_Size;
                    end
                  else begin
                    if tester_cnxs<=0 then begin
                      { Hardwired connector case }
                      end
                    else if (tester_hi-tester_lo+1)> tester_cnxs*PPC then begin
                      err_mess:='Too many pins in CABLE';
                      error(0);                    
                      end
                    else if tester_hi>(Curr_Add+tester_cnxs*PPC-1) 
                      then begin
                      err_mess:='Inconsistent pin allocation from:';
                      err_p1:=Number_to_alfa(tester_lo,0);err_p2:=' TO:';
                      err_p3:=Number_to_alfa(tester_hi,0);error(3);
                    end;
                    if tester_cnxs=0 then begin
                      Next_available:=New_Block(Curr_Add);
                      end
                    else begin
                      { Note: Next_available never seems to be used}
                      {Next_available:=Next_available+Tester_cnxs*PPC;}
                      Next_Available:=New_Block(Curr_Add);{6-nov-1991}
                    end;
                  end;
                end;
              end;{OPEN=YES}
            end;
          end;
    end;

    procedure get_new_adp_conn_maybe;
     begin
         if adapter=NIL then begin
           new(Adapter);
           adapter^.current:=NIL;
           adapter^.first:=NIL;
           adapter^.next_available:=1;
         end;                             
         with adapter^ do begin
           if current=NIL  then begin
             new_adp_dev(current);
             first:=current;
             open:=YES;
             end
           else if (open=NO) or (Cable_Name<>current^.name) then begin
             close_cable_desc;
             new_adp_dev(t_adp_dev);
             current^.next:=t_adp_dev;
             current:=t_adp_dev;
             open:=YES;
           end;
         end;
     end;

     begin 
      
        {  Everything that are associated with connections to tester    }
        {  is considered included in the adapter. Major portion of     }
        {  the adapter consists of cables. Cables are syntactically    }
        {  described as follows:                                       }
        {                                                              }
        {  CABLE:TE1234-56                                             }
        {    @P4:J1,1,10                                               }
        {    @P5:J2,101,20                                             }
        {    @P6:N/C                                                   }
        {  END.                                                        }
        {                                                              }
        {  WITS will consider this cable to be as follows              }
        {                                                              }
        {  FACT side                                       UUT side    }
        {                                                              }
        {     P1 ----------|                 P4                        }
        {        001-100   |                 1-10                      }
        {                  |                                           }
        {     P2 ----------|                 P5                        }
        {        101-200   |                 101-120                   }
        {                  |                                           }
        {     P3 ----------|                 P6                        }
        {        201-300                     N/C                       }
        {                                                              }
        {                                                              }
        {    Data representation:                                      }
        {                                                              }
        {     Adapter                                                  }
        {         |                                                    }
        {         |---Cable                                            }
        {         |     |-----subdevice                                }
        {         |     |-----subdevice                                }
        {         |     |-----subdevice                                }
        {         |                                                    }
        {         |---Cable                                            }
        {                |                                             }
        {                etc.                                          }
        {                                                              }
        {                                                              }
        {                                                              }
        {                                                              }



        if (Cable_Name=NULL_2) and (Cab_Conn=NULL) then begin
          close_cable_desc;
          end                  
        else begin
          if (Conn_add<0) then begin          
            err_mess:=' Bad address for a cable';
            error(0);goto 99;
          end;
          if (Cab_Conn[1] in ['P']) then begin
             if ZIF then begin
               Pxxx:=trailing_letter(Cab_conn,2,alfa_length(Cab_conn));
               end
             else if good_integer(Cab_conn,2,alfa_length(Cab_conn))then begin
               Pxxx:=alfa_to_number(Cab_conn,2,alfa_length(Cab_conn));
               end
             else begin
               Err_Mess:=' Bad number in Pxxx field:';
               Err_P1:=Cab_Conn;error(1);goto 99;
             end;
            if Pxxx in [0,2..Max_Conn] then begin
              get_new_adp_conn_maybe;
  
              with adapter^ do begin
                current^.Name:=Cable_Name; 
                current^.z_subdev^.Cab_Name:=Cab_conn { Leporello };
                current^.z_subdev^.UUT_Name:=UUT_conn ;
                if ZIF then begin 
                  current^.tester_cnxs:=ZIF_Tester_Connections; 
                  end
                else begin
                  if current^.tester_cnxs>=Pxxx then begin
                    current^.tester_cnxs:=Pxxx-1;
                  end;
                end;
                if current^.tester_cnxs<0 then current^.tester_cnxs:=0;
                if current^.address>conn_add then current^.address:=conn_add;
                t_adp_subdev:=current^.subdev;
                while t_adp_subdev^.Cab_name<>cab_conn do begin
                  t_adp_subdev:=t_adp_subdev^.next;
                end;
                if t_adp_subdev=current^.z_subdev then begin
                  current^.z_subdev^.address:=Conn_Add;
                  current^.UUT_cnxs:=current^.UUT_cnxs+1;
                  new(t_adp_subdev);current^.z_subdev^.next:=t_adp_subdev;
                  current^.z_subdev:=t_adp_subdev;
                  end
                else begin
                  { already exists so must be continuation}
                end;
              end {with};
              end
            else begin             
              err_mess:='Bad Pxxx number in cable descriptor';
              err_P1:=cab_conn;error(1);goto 99;
            end;                                
            end            
          else if STRIP then begin
            get_new_adp_conn_maybe;
            with adapter^ do begin
              current^.Name:=Cable_Name; 
              current^.z_subdev^.Cab_Name:=Cab_conn { Leporello };
              current^.z_subdev^.UUT_Name:=UUT_conn ;
              current^.tester_cnxs:=current^.tester_cnxs+strip_size;
              if current^.address>conn_add then current^.address:=conn_add;
              t_adp_subdev:=current^.subdev;
              while t_adp_subdev^.Cab_name<>cab_conn do begin
                t_adp_subdev:=t_adp_subdev^.next;
              end;
              if t_adp_subdev=current^.z_subdev then begin
                current^.z_subdev^.address:=Conn_Add;
                current^.UUT_cnxs:=current^.UUT_cnxs+1;
                new(t_adp_subdev);current^.z_subdev^.next:=t_adp_subdev;
                current^.z_subdev:=t_adp_subdev;
                end
              else begin
                { already exists so must be continuation}
              end;
            end {with};
            end
          else if ZIF then begin
            get_new_adp_conn_maybe;
            with adapter^ do begin
              current^.Name:=Cable_Name; 
              current^.z_subdev^.Cab_Name:=Cab_conn { Leporello };
              current^.z_subdev^.UUT_Name:=UUT_conn ;     
              current^.tester_cnxs:=ZIF_Tester_Connections;
              if current^.address>conn_add then current^.address:=conn_add;
              t_adp_subdev:=current^.subdev;
              while t_adp_subdev^.Cab_name<>cab_conn do begin
                t_adp_subdev:=t_adp_subdev^.next;
              end;
              if t_adp_subdev=current^.z_subdev then begin
                current^.z_subdev^.address:=Conn_Add;
                current^.UUT_cnxs:=current^.UUT_cnxs+1;
                new(t_adp_subdev);current^.z_subdev^.next:=t_adp_subdev;
                current^.z_subdev:=t_adp_subdev;
                end
              else begin
                { already exists so must be continuation}
              end;
            end {with};
            end 
          else begin
            err_mess:=' Bad connector descriptor';
            err_p1:=Cab_conn;error(1);goto 99;
          end;                                
        end;       
 99:end;

{------------------------------------------------------------------}
   procedure get_part(pack,a_dev:alfa);
   label 99;
   var i:integer;
   cable_Name:alfa2;
   cab_conn:alfa;
    procedure get_sub_cable(Cable_Name:alfa2;Cab_Conn:alfa;Cable_sub:Boolean);
     label 99;
     var i:integer;


     function available_space(strip_size:integer;var strip_pin:integer):boolean;
      var strip_bank,strip_loc,row_size:integer;
      begin
          strip_bank:=(strip_pin-1) div 1200;
          strip_loc:=((strip_pin-1) mod 1200)+1;
          if strip_loc<=840 then begin
            row_size:=420;
            end
          else begin
            row_size:=360;
          end;
          if (strip_size+((strip_loc-1) mod 420))>row_size then begin
            available_space:=false;
            if      strip_loc< 420 then strip_loc:=421
            else if strip_loc< 840 then strip_loc:=841
            else if strip_pin<1200 then begin
              strip_loc:=1;
              strip_bank:=strip_bank+1;
              end
            else begin
              err_mess:=' Can not find a new row for connector';
              error(0);goto 99;
            end;
            strip_pin:=strip_loc+strip_bank*1200;
            end
          else begin
            available_space:=true;
          end;                                     
      end{available_space};

      begin                                     
        if a[1]='+' then begin
          for i:=1 to al-1 do a[i]:=a[i+1];
          a[al]:=' ';
          if a=Old_Name then begin
            cont_line:=true;
          end;   
          conn_name:=a;
          end { a[1]='+' }
        else if a=Old_Name then begin                  
          cont_line:=true;
          conn_name:=a;
          Old_Name:=a;
          end   
        else begin                 
          conn_name:=a;
          cont_line:=false;
        end;
        conn_name := full_name(pack,a);     
        old_name:=conn_Name;
        getsym;  
        if a=' ' then begin
          Conn_Add:=Next_TB_Address;
          Next_TB_Address:=Next_Address(Next_TB_Address,1);
          Curr_Add:=Conn_Add;
          end      
        else if a='+' then begin
          if cont_line then begin
            Conn_Add:=Curr_Add;
            end    
          else if Cable_sub then begin
            Conn_Add:=Curr_Add;
            end
          else begin
            err_mess:='Bad address specified';
            error(0);
          end;     
          end      
        else if number then begin
          if legit_source_add(numero) then begin
            Curr_Add:=Numero;
            Conn_Add:=Mach_Xlate(Curr_add);   
            end
          else begin
            err_mess:=' Illegal Address specification';
            error(0);goto 99;
          end;
          end      
        else if STRIP and (Big_A='S') then begin
          Curr_Add:=strip_pin;
          Conn_Add:=Mach_Xlate(Curr_add);
          end
        else if ZIF   and (Big_A='S') then begin
          Curr_Add:=new_block(max(curr_add,top_pin));
          Conn_Add:=Mach_Xlate(Curr_add);
          end
        else if (Big_A='S') then begin
          Curr_Add:=new_block(max(curr_add,top_pin));
          Conn_Add:=Mach_Xlate(Curr_add);
          end
        else begin 
          err_mess:=' Address field does not contain legal number';
          error(0);
          goto 99; 
        end;       
        Strip_Pin:=curr_add;
        if STRIP and (Conn_Add>=0)then begin

          if (strip_pin mod 3<>1) and cable_sub then begin
            err_mess:='Illegal address for a strip connector.';
            error(0);goto 99;
          end;
 
          strip_size:=0;used_strip_pins:=0;
          if (ch='(')then begin
            getsym;{ throw away '(' }
            if ch in ['D','d'] then getch;
            getsym;
            while a<>')' do begin
              if end_line then begin
                err_mess:=' Premature end of line for STRIP connector';
                error(0);goto 99;
              end;
              if number then begin
                if (numero mod 3)=0 then begin
                  strip_size:=strip_size+numero;
                  end
                else begin
                  err_mess:='Strip size must be multiple of 3';
                  error(0);goto 99;
                end;
                end
              else begin
                err_mess:=' Illegal STRIP size descriptor';
                err_p1:=a;error(0);goto 99;
              end;
              if ch in ['D','d'] then getch;
              getsym;
            end{while};
            if (strip_size<=0) or (strip_size>420) then begin 
              err_mess:='Bad STRIP connector size descriptor??';
              error(0);goto 99;
            end;
            if not available_space(strip_size,strip_pin) then begin
              Curr_add:=strip_pin;
              Conn_Add:=Mach_Xlate(strip_pin);
            end;
            strip_pin:=strip_pin+Strip_Size;{ calc next available strip_pin}
          end;
        end{STRIP};

        add_Cable_Desc(Cable_Name,CAB_conn,conn_name,Curr_Add);

        getsym;  
        if a='[' then begin
          Get_Range_Conn(Conn_Name,Curr_Add);
          if not OK then goto 99;
          end      
        else if symbol then  begin    
          if number then {numeric connector} begin
            n1:=numero;
            f1:=next_pin(a,a,3);
            getsym;
            if a='-' then begin
              getsym;
              if number then begin
                n2:=numero;
                f2:=next_pin(a,a,3); { pad to 3 digits }
                end
              else begin
                err_mess:='second pin expected to be numeric';
                error(0);goto 99;
              end; 
              end  
            else begin
              f2:=f1;n2:=n1;
              f1:='1';n1:=1;
              f1:=Next_Pin(f1,f1,3);{ pad it if necessary}
              if a<>' ' then begin
                if number then begin
                  f3:=f2;n3:=n2;
                  f2:=a;n2:=numero;{ split connector}
                  if n3 <= n2 then begin
                    err_mess:='Split point is Bad:';
                    err_p1:=f2;error(1);goto 99;
                  end;
                  end
                else begin
                  err_mess:='Number expected for split connector';
                  error(0);goto 99;
                end;
                end
              else begin
                f3:=' ';
              end; 
                   
            end;   
            if OK then begin
              Insert_part_Pin(Conn_Name,f1,Curr_Add);{ first pin}
              if not OK then goto 99;
              Curr_Add:=Next_Address(Curr_Add,1);
              if curr_add<0 then Next_TB_Address:=Curr_Add;
              f:=f1;
              while (f<>f2) and OK do begin{insert the rest of first part}
                f:=Next_Pin(f,f2,0);
                Insert_part_Pin(Conn_Name,f,Curr_Add);
                if not OK then goto 99;
                Curr_Add:=Next_Address(Curr_Add,1);
                if curr_add<0 then Next_TB_Address:=Curr_Add;
              end;
                   
              if (f3<>' ') and OK then begin
                if Curr_Add>=0 then begin
                  Curr_Add:=New_Block(Curr_Add);
                end;
                while (f<>f3) and OK do begin
                  f:=Next_Pin(f,f3,0);
                  Insert_part_Pin(Conn_Name,f,Curr_Add);
                  if not OK then goto 99;
                  Curr_Add:=Next_Address(Curr_Add,1);
                  if curr_add<0 then Next_TB_Address:=Curr_Add;
                end;
              end; 
            end;   
            end    
                   
          else { must be that funny skip pin format } begin
            n1:=Alpha_Pos(a);
            getsym;
            n2:=Alpha_Pos(a);
            if n1>0 then begin
              for i:=1 to n1 do begin
                if i=n2 then begin
                  getsym;
                  if a=' ' then begin
                    n2:=0;
                    end
                  else begin
                    n2:=Alpha_Pos(a);
                    if n2=0 then begin
                      err_mess:='Pin must be an alphabetic pin:';
                      err_p1:=a;error(1);goto 99;
                      end
                    else if n2<=i then begin
                      err_mess:='Skip pin is out of order:';
                      err_p1:=a;error(1);goto 99;
                    end;
                  end;
                  end
                else begin                
                  f:=Reg_Alpha_Pin(i,False);
                  Insert_part_Pin(Conn_Name,f,Curr_Add);
                  if not OK then goto 99;
                  Curr_Add:=Next_Address(Curr_Add,1);
                  if curr_add<0 then Next_TB_Address:=Curr_Add;
                  if not OK then goto 99;
                end;
              end;          
            end;   
          end;     
          end      
        else if end_line then { no pins just a signal name } begin
          f:=No_Pin;  
          Insert_part_Pin(Conn_Name,f,Curr_Add);
          if not OK then goto 99;
          Curr_Add:=Next_Address(Curr_Add,1);
          if curr_add<0 then Next_TB_Address:=Curr_Add;
          end      
        else begin 
          err_mess:='Unrecognized Pin descriptor field:';
          err_p1:=a;error(1);goto 99;
        end;
99:   end;                         

  procedure get_device(pack,f1:alfa);
    procedure get_switch(s1:alfa);
      label 99;           
      var i              :integer;
          part_no        :alfa4;
          p_label        :alfa4;
          Pos_Name       :alfa4;
          Semicolon_found:Boolean;
          d:dev_link;
          t_pair:pair_link;
          t_state:state_link;
          Default_found:boolean;
          Default_State:alfa;
          Old_Pos:alfa; { just a dummy }
{  This procedure reads a switch description. A switch is described as 
   follows :

   s1:SWITCH:Part_no:Label
      TERMINALS: terminal list;
      POSITIONS:
        pos [ name ]:cond>pos (term,term,...),(term,term,....),
                    (term,...),......;
        pos [ name ]:cond>pos etc. ;
      END.                                

      e.g. A DPDT switch would be coded as follows:
      
    S1:SWITCH:DPDT:Engine starter
         TERMINALS:NC1,C1,NO1,NC2,C2,NO2;
         POSITIONS:
         1 [ON] :(NO1,C1),(NO2,C2);
         2 [OFF]:(NC1,C1),(NC2,C2);  
                                          }

      begin            
         default_found:=false;i:=0;getch;part_no:=' ';
         while (ch<>':') and (i<al4) and (cc<ll) do begin
           i:=i+1; part_no[i]:=ch; getch;
         end;
            
         if ch<>':' then begin
           err_mess:=' : missing after switch PART NO';
           error(0);goto 99;
         end;
 
         i:=0;getch;p_label:=' ';
         while (i<al4) and (cc<ll) do begin
           i:=i+1; p_label[i]:=ch; getch;
         end; 
   
         get_line_sym;
         if Big_a='TERMINALS' then begin
           getnext;
           if a=':' then begin
             get_device_pins(s1,Next_TB_Address,';');
             end
           else begin
             err_mess:=' ":"  expected after TERMINALS';
             error(0);goto 99;
           end;
           end            
         else begin
           Err_Mess:=' TERMINALS keyword is missing!';
           error(0);goto 99;
         end;
         d:=device_pointer(s1);
         d^.part_type:=Switch;
         d^.part_no:=part_no;
         d^.dev_label:=p_label;
         d^.curr_state:=' ';
         d^.s:=NIL;               
         getnext;
         if Big_a='POSITIONS' then begin
           getnext;
           if a=':' then begin {POSITIONS:}
             v1.dev:=s1;
             getnext;{1....}
             while Big_a<>'END.' do begin
               new(t_state);t_state^.next:=d^.s;t_state^.c:=NIL;
               d^.s:=t_state;
               with t_state^ do begin
                 desc:=a;{ remember position identifier}
                 cond:=' ';oper:=' ';ret:=' ';
               end;
               if symbol then begin
                 Pos_Name:=' ';
                 if ch='[' then begin {get position name}
                   getch;i:=0;
                   while (ch<>']') and (cc<ll) and (i<al4) do begin
                     i:=i+1;
                     Pos_Name[i]:=ch;getch;
                   end;{1[START]}
                   if ch=']' then begin 
                     getch;
                     end
                   else begin
                     err_mess:='Position name is too long';
                     error(0);goto 99;
                   end;
                 end;
                 t_state^.name:=Pos_Name;{Position label(NAME)}
                 getnext;
                 if a=':' then begin
                   getnext_with(['~']);
                   if symbol then begin
                     t_state^.cond:=a;
                     for i:=1 to al do begin
                       if a[i]='*' then begin
                         if Default_found then begin
                           Err_Mess:=' Multiple default declarations';
                           Error(0);goto 99;
                           end
                         else begin
                           Default_Found:=true;
                           Default_State:=t_state^.desc;
                         end;
                       end{if '*' };
                     end{for};
                     getnext;
                     if a[1] in ['>'] then begin
                       t_state^.oper:=a;
                       getnext;
                       if symbol then begin
                         t_state^.ret:=a;
                         getnext;
                         end
                       else begin
                         err_mess:='Return position expected';
                         error(0);goto 99;
                       end;
                     end;
                   end;
                   Semicolon_Found:=false;
                   while not semicolon_found do begin
                     if a=';' then begin
                       semicolon_found:=true;
                       end
                     else if a='(' then begin
                       getnext;
                       if symbol then begin 
                         v1.pin:=a;x:=address_of(v1);
                         if good(x) then begin
                           getnext;
                           while (a<>')') and OK do begin
                             if symbol then begin
                               v1.pin:=a;
                               y:=address_of(v1);
                               if good(y) then begin
                                 new(t_pair);t_pair^.next:=t_state^.c;
                                 t_state^.c:=t_pair;
                                 t_pair^.x:=x;t_pair^.y:=y;
                                 {exercise contacts to sensitize them}
                                 connect_pair(x,y);
                                 disconnect_pair(x,y);
                                 {that was hard exercise}
                                 x:=y;
                                 getnext;
                                 end
                               else { bad y address } begin
                                 Err_Mess:='Bad switch pin:';
                                 Err_P1:=v1.Pin;
                                 error(1);goto 99;
                               end;
                               end
                             else begin
                               err_mess:='Pin name expected';
                               error(0);
                             end;
                           end;{ while }
                           end
                         else{ bad x address} begin
                           Err_Mess:='Bad switch pin:';
                           Err_P1:=v1.Pin;
                           error(1);goto 99;
                         end;
                         end
                       else { first symbol after ( is bad } begin
                         err_mess:='Pin name expected';
                         error(0);
                       end;
                       getnext;
                       end
                     else begin
                       err_mess:='Parenthesis missing!';
                       error(0);goto 99;
                     end;
                   end{while not semicolon_found};
                   end
                 else begin
                   Err_Mess:=' colon expected after position name';
                   error(0);goto 99;
                 end;
                 end
               else begin
                 Err_Mess:=' Switch Position Identifier expected!';
                 error(0);goto 99;
               end;
               getnext;
             end{while not END. };
             end
           else begin
             err_mess:=' ":"  expected after POSITIONS';
             error(0);goto 99;
           end;
           end
         else begin
           Err_Mess:=' POSITIONS keyword is missing!';
           error(0);goto 99;
         end;

         if default_found then begin
           with d^ do begin
             switch_state(Switch,s1,Default_state,dev_label,old_pos);
           end;             
         end;
             
99:
    end{get_switch};   

    procedure get_relay(s1:alfa);
      label 99;           
      var i              :integer;
          part_no        :alfa4;
          p_label        :alfa4;
          Pos_Name       :alfa4;
          Semicolon_found:Boolean;
          d:dev_link;
          t_pair:pair_link;
          t_state:state_link;
          t_cond :condition_link;
          t_coil :coil_link;
          resistance:real;unit:units;
          Default_found:boolean;
          Default_State:alfa;
          Old_Pos:alfa;
{  This procedure reads a relay description. A relay is described as 
   follows :

   s1:RELAY:Part_no:24vdc
      [required]
      TERMINALS: terminal list;

      [optional]
      COIL: posterm [resistance] negterm;
            posterm [resistance] negterm;
          ...etc...
        if not specified then following is assumed:
        COILS: X1 200 Ohms X2;

      [optional]
      CONDITIONS:
        cond:term1 > 23 VDC term2;
        cond:term1 < 15 VDC term2;
        ERROR:term1 <0 VDC term2;
        if not specified then following is assumed:
        CONDITIONS:
                 E:X1 > 17 VDC  X2;
                 D:X1 < 12 VDC  X2;

                 note: if X1 to X2 is between 12 to 17 VDC then an error
                       message will be issued.

      [required]
      POSITIONS:
        pos [ name ]:cond>pos (term,term,...),(term,term,....),
                    (term,...),......;
        pos [ name ]:cond>pos etc. ;
      END.                      

      note: conditions are optional and if not specified then automatic
            energization is not possible.

      e.g. A DPDT relay would be coded as follows:
      
    K1:RELAY:DPDT:Engine starter
         TERMINALS:NC1,C1,NO1,NC2,C2,NO2;
         POSITIONS:
         1 [ON] :D(NO1,C1),(NO2,C2);  ! Deenergized
         2 [OFF]:E(NC1,C1),(NC2,C2);  ! Energized
                                          }

  function condition_name(a:alfa):alfa;
    label 99;
    var i,j:integer;
        temp:alfa;
        found,first:boolean;
    begin
       j:=1;temp:=' ';found:=false;first:=false;
       for i:=1 to al do begin
         if a[i]='*' then begin
           if found then begin
             err_mess:=' Too many asterisks';
             warn(0);
             end
           else begin 
             found:=true;first:=(i=1);
           end;
           end
         else begin
           if found and not first then begin
             if a[i]<>' ' then begin
               err_mess:='Misplaced asterisk in:';
               err_p1:=a;error(1);goto 99;
             end;
           end;
           temp[j]:=a[i];
           j:=j+1;
         end;
       end;
99:    condition_name:=temp;
     end;
  


  procedure get_pin_name;
    begin
       if symbol then begin
         v1.pin:=a;
         x:=address_of(v1);
         if not good(x) then begin   
           err_mess:= 'Bad pin specified:';err_p1:=a;
           error(1);goto 99;
         end;               
         end
       else begin
         err_mess:=' Pin name expected , NOT:';err_p1:=a;
         error(1);goto 99;
       end;
    end;      

      begin
         RELAY_COUNT:=RELAY_COUNT+1;            
         default_found:=false;i:=0;getch;part_no:=' ';
         while (ch<>':') and (i<al4) and (cc<ll) do begin
           i:=i+1; part_no[i]:=ch; getch;
         end;
              
         if ch<>':' then begin
           err_mess:=' : missing after relay PART NO';
           error(0);goto 99;
         end;
 
         i:=0;getch;p_label:=' ';
         while (i<al4) and (cc<ll) do begin
           i:=i+1; p_label[i]:=ch; getch;
         end;
                                
         get_line_sym;
         if Big_a='TERMINALS' then begin
           getnext;
           if a=':' then begin
             get_device_pins(s1,Next_TB_Address,';');
             end
           else begin
             err_mess:=' ":"  expected after TERMINALS';
             error(0);goto 99;
           end;
           end            
         else begin
           Err_Mess:=' TERMINALS keyword is missing!';
           error(0);goto 99;
         end;
         getnext;{discard semicolon}
         d:=device_pointer(s1);
         d^.part_type:=relay;
         d^.part_no:=part_no;
         d^.dev_label:=p_label;
         d^.curr_state:=' ';
         d^.s:=NIL;
         d^.cond:=NIL;
         d^.coils:=NIL;
         while (Big_a='COIL')  do begin
           getnext;
           if a=':' then begin
              getnext;{trash :}
              if not symbol then begin
                err_mess:=' Bad Pin Name ';err_p1:=a;
                error(0);goto 99;
              end;          
              x:=insert_internal_pin(s1,a);
              if not OK then goto 99;
              getv;z1:=z;z2.a:=NULL;
              get_parm(true,z1,z2,[Ohm,KOhm,MOhm] ,resistance,unit);
              if not OK then goto 99;
              normalize(resistance,unit,resistance,unit);
              if z1.a=NULL then getnext;
              if not symbol then begin
                err_mess:=' Bad Pin Name ';err_p1:=a;
                error(0);goto 99;
              end;
              y:=insert_internal_pin(s1,a);
              connect_two_term(x,y,resistance,resistance,0.50,10.0);{ 50% }
              new(t_coil);
              t_coil^.x:=x;t_coil^.y:=y;t_coil^.resistance:=resistance;
              t_coil^.next:=d^.coils;d^.coils:=t_coil;
              if ch=';' then getnext;getnext;
             end
           else begin
             err_mess:=' ":"  expected after COIL';
             error(0);goto 99;                    
           end;
         end;

         if Big_a='CONDITIONS' then begin
           getnext;
           if a=':' then begin {CONDITIONS:}
             v1.dev:=s1;
             getnext;{A....}
             while ((Big_a<>'END.') and (Big_a<>'POSITIONS')) do begin
               new(t_cond);t_cond^.next:=d^.cond;
               d^.cond:=t_cond;
               if symbol then begin
                 t_cond^.desc:=a;{ remember condition identifier}
                 end
               else begin
                 err_mess:=' Bad condition name ';
                 err_p1:=a;error(1);goto 99;
               end;
               getnext;
               if a=':' then begin
                 with t_cond^ do begin
                   getnext;get_pin_name;
                   term1:=x;
                   case ch of 
                     '<' : relop:=le;
                     '>' : relop:=ge;
                   { '=' : relop:=eq;} { Take this out }
                   OTHERWISE
                     err_mess:=' < or >  expected ';
                     error(0);goto 99;
                   end;
                   getnext;{ get and trash relop }
                   getv;z1:=z;z2.a:=NULL;
                   get_parm(true,z1,z2,[VDC,VAC] ,v,u);
                   normalize(v,u,v,u);
                   if z1.a=NULL then getnext;get_pin_name;
                   term2:=x;
                 end;
               end;
               if ch=';' then getnext;getnext;
             end{while not END. };
             end
           else begin
             err_mess:=' ":"  expected after CONDITIONS';
             error(0);goto 99;
           end;             
         end;

         if Big_a='POSITIONS' then begin
           getnext;
           if a=':' then begin {POSITIONS:}
             v1.dev:=s1;
             getnext;{1....}
             while (Big_a<>'END.') do begin
               new(t_state);t_state^.next:=d^.s;t_state^.c:=NIL;
               d^.s:=t_state;
               with t_state^ do begin
                 desc:=a;{ remember position identifier}
                 cond:=' ';oper:=' ';ret:=' ';
               end;
               if symbol then begin
                 Pos_Name:=' ';
                 if ch='[' then begin {get position name}
                   getch;i:=0;
                   while (ch<>']') and (cc<ll) and (i<al4) do begin
                     i:=i+1;
                     Pos_Name[i]:=ch;getch;
                   end;{1[START]}
                   if ch=']' then begin 
                     getch;
                     end
                   else begin
                     err_mess:='Position name is too long';
                     error(0);goto 99;
                   end;
                 end;
                 t_state^.name:=Pos_Name;{Position label(NAME)}
                 getnext;
                 if a=':' then begin
                   getnext;
                   if symbol then begin
                     t_state^.cond:=condition_name(a);
                     for i:=1 to al do begin
                       if a[i]='*' then begin
                         if Default_found then begin
                           Err_Mess:=' Multiple default declarations';
                           Error(0);goto 99;
                           end
                         else begin
                           Default_Found:=true;
                           Default_State:=t_state^.desc;
                         end;
                       end{if '*' };
                     end{for};
                     getnext;
                     if a[1] in ['>'] then begin
                       t_state^.oper:=a;
                       getnext;
                       if symbol then begin
                         t_state^.ret:=a;
                         getnext;
                         end
                       else begin
                         err_mess:='Return position expected';
                         error(0);goto 99;
                       end;
                     end;
                   end;
                   Semicolon_Found:=false;
                   while not semicolon_found do begin
                     if a=';' then begin
                       semicolon_found:=true;
                       end
                     else if a='(' then begin
                       getnext;
                       if symbol then begin 
                         v1.pin:=a;x:=address_of(v1);
                         if good(x) then begin
                           getnext;
                           while (a<>')') and OK do begin
                             if symbol then begin
                               v1.pin:=a;
                               y:=address_of(v1);
                               if good(y) then begin
                                 new(t_pair);t_pair^.next:=t_state^.c;
                                 t_state^.c:=t_pair;
                                 t_pair^.x:=x;t_pair^.y:=y;
                                  {exercise contacts to sensitize them}
                                 connect_pair(x,y);
                                 disconnect_pair(x,y);
                                  {that was hard exercise}
                                 x:=y;
                                 getnext;
                                 end
                               else { bad y address } begin
                                 Err_Mess:='Bad relay pin:';
                                 Err_P1:=v1.Pin;
                                 error(1);goto 99;
                               end;
                               end
                             else begin
                               err_mess:='Pin name expected';
                               error(0);
                             end;
                           end;{ while }
                           end                   
                         else{ bad x address} begin
                           Err_Mess:='Bad relay pin:';
                           Err_P1:=v1.Pin;
                           error(1);goto 99;
                         end;
                         end
                       else { first symbol after ( is bad } begin
                         err_mess:='Pin name expected';
                         error(0);
                       end;
                       getnext;
                       end
                     else begin
                       err_mess:='Parenthesis missing!';
                       error(0);goto 99;
                     end;
                   end{while not semicolon_found};
                   end
                 else begin
                   Err_Mess:=' colon expected after position name';
                   error(0);goto 99;
                 end;
                 end             
               else begin
                 Err_Mess:=' relay Position Identifier expected!';
                 error(0);goto 99;
               end;
               getnext;
             end{while not END. };
             end
           else begin
             err_mess:=' ":"  expected after POSITIONS';
             error(0);goto 99;
           end;             
           end
         else begin
           Err_Mess:=' POSITIONS keyword is missing!';
           error(0);goto 99;
         end;

         if default_found then begin
           with d^ do begin
             switch_state(Relay,s1,Default_state,dev_label,old_pos);
           end;
         end;


99:
      end{get_relay}; 




    procedure get_resistor(r1:alfa);
      label 99;           
      var i              :integer;
          part_no        :alfa4;
          Semicolon_found:Boolean;
          d:dev_link;
          v:real;u:units;              
          first_pin,dummy:boolean;                  
{  This procedure reads a resistor description. A resistor is described as 
   follows :            

   r1:RESISTOR:Part_no:[optional_pins] resistance tol power_rating;
 }
      begin            
         i:=0;getch;part_no:=' ';
         while (not (ch in [':'])) and (i<al4) and (cc<ll) do begin
           i:=i+1; part_no[i]:=ch; getch;
         end;

         if ch=':' then begin
           getsym;{ skip over : }
           end
         else begin
           err_mess:=' Colon expected after part number';
           error(0);goto 99;
         end;

         if ch='[' then begin
           getsym;{get and discard '['}
           get_device_pins(r1,Next_TB_Address,']');
           if not OK then goto 99;
           end
         else begin
           f1:='1';f2:='2';
           x:=Insert_Internal_pin(r1,f1);
           if OK then begin
             x:=insert_internal_pin(r1,f2);
             if not OK then goto 99;
           end;
         end;             

                               
         d:=device_pointer(r1);
         d^.part_no:=part_no;
         d^.part_type:=resistor;
              
         getv;z1:=z;z2.a:=NULL;
         with d^ do begin
           get_parm(true,z1,z2,[Ohm,KOhm,MOhm] ,v,u);
           normalize(v,u,resistance,u);
           if resistance<0.001 then begin
             err_mess:= ' Minimum resistance is 0.001 Ohms';
             error(0);goto 99;
           end;
           get_parm(true,z1,z2,[PCT]           ,v,u);
           normalize(v,u,tolerance ,u);
           v:=1.0/8.0;u:=WATT;
           get_parm(false,z1,z2,[WATT]          ,v,u);
           normalize(v,u,R_wattage ,u);
           unused:=z1.a;
         end;
         if OK then begin
           if d^.p<>d^.z_p then begin
             x:=d^.p^.i_add;
             if d^.p^.p<>d^.z_p then begin
               y:=d^.p^.p^.i_add;
               if d^.p^.p^.p<>d^.z_p then begin
                 err_mess:=' More then two pins';error(0);goto 99;
               end;
               end
             else begin
               err_mess:='Only one resistor pin';
               error(0);goto 99;
             end;
             end
           else begin
             err_mess:='No resistor pins defined';
             error(0);goto 99;
           end;

           connect_two_term(x,y,d^.resistance,
                                d^.resistance,
                                d^.tolerance/100.0,
                                d^.R_wattage);

           omit_node(x,SOFT_OMIT);omit_node(y,SOFT_OMIT);
           d^.max_i_ratio:=0;
         end;

         if ch=';' then getsym; {skip semicolon if it is there}
      
99:
      end{get_resistor};
 
    procedure get_diode(r1:alfa);
      label 99;           
      var i              :integer;
          part_no        :alfa4;
          Semicolon_found:Boolean;
          d:dev_link;
          v:real;u:units;
          first_pin,dummy:boolean;                  
{  This procedure reads a Diode description. A diode is described as 
   follows :                    

   r1:DIODE:Part_no:[pins]Ge  PRV 100 vdc Ir 0.1 mADC If 1 ADC Vf 1.1 VDC;
                     0pt  Si  optional    optional  optional optional
 }                        
      begin            
         i:=0;getch;part_no:=' ';semicolon_found:=false;
         while (not (ch in [':'])) and (i<al4) and (cc<ll) do begin
           i:=i+1; part_no[i]:=ch; getch;
         end;

         if ch=':' then begin
           getsym;{ skip over : }
           end
         else begin
           err_mess:=' Colon expected after part number';
           error(0);goto 99;
         end;

         if ch='[' then begin
           getsym;{get and discard '['}
           get_device_pins(r1,Next_TB_Address,']');
           if not OK then goto 99;
           end
         else begin
           f1:='C';f2:='A';
           x:=Insert_Internal_pin(r1,f1);
           if OK then begin
             x:=insert_internal_pin(r1,f2);
             if not OK then goto 99;
           end;
         end;             

         d:=device_pointer(r1);
         with d^ do begin
           part_no:=part_no;
           part_type:=diode;
           R_diode:=10.0; { some small value}
           tolerance:=10.0;  { some large value}
           PRV:=100.0; { peak reverse voltage }
           I_fwd:=1.0; { Amps }
           I_rev:=0.0001; { Amps }
           V_fwd:=1.1;   { max forward voltage }
         end;
             
         getsym;
         if Big_a='GE' then begin
           d^.Diode_Type:=Germanium;
           d^.Vj_diode:=0.3;
           d^.V_fwd:=0.6;   { max forward voltage }
           end
         else if Big_a='SI' then begin
           d^.Diode_Type:=Silicon;
           d^.Vj_diode:=0.7;
           d^.V_fwd:=1.1;   { max forward voltage }
           end
         else begin
           Err_Mess:=' Diode Type (Ge or Si) not specified';
           error(0);
         end;
                      
         getv;z1:=z;z2.a:=NULL;
         with d^ do begin
           while not semicolon_found do begin
             if (upperalfa(z1.a)='PRV')then begin
                getv;z1:=z;
                get_parm(true,z1,z2,[VDC..KVDC],PRV,u);
                normalize(PRV,u,PRV,u);
                end
             else if(upperalfa(z1.a)='IR')then begin
                getv;z1:=z;
                get_parm(false,z1,z2,[uADC..ADC],I_rev,u);
                normalize(I_rev,u,I_rev,u);
                end
             else if(upperalfa(z1.a)='IF')then begin
                getv;z1:=z;
                get_parm(false,z1,z2,[uADC..ADC],I_fwd,u);
                normalize(I_fwd,u,I_fwd,u);
                end
             else if(upperalfa(z1.a)='VF')then begin
                getv;z1:=z;
                get_parm(false,z1,z2,[VDC..VDC],V_fwd,u);
                normalize(V_fwd,u,V_fwd,u);
                end
             else if(z1.a=';')then begin
                semicolon_found:=true;
                end
             else begin
                semicolon_found:=true;
                err_mess:=' Unexpected data ';
                error(0);             
             end;
           end;
         end;
         if OK then begin
           if d^.p<>d^.z_p then begin
             x:=d^.p^.i_add;
             if d^.p^.p<>d^.z_p then begin
               y:=d^.p^.p^.i_add;
               if d^.p^.p^.p<>d^.z_p then begin
                 err_mess:=' More then two pins';error(0);goto 99;
               end;
               end
             else begin
               err_mess:='Only one diode pin';
               error(0);goto 99;
             end;
             end
           else begin
             err_mess:='No diode pins defined';
             error(0);goto 99;
           end;

           connect_two_term(x,y,d^.resistance,
                                d^.resistance,
                                d^.tolerance/100.0,
                                d^.I_fwd);

           omit_node(x,SOFT_OMIT);omit_node(y,SOFT_OMIT);
           d^.max_i_ratio:=0;
         end;

         if ch=';' then getsym; {skip semicolon if it is there}
      
99:
      end{get_diode}; 
 
    procedure get_zener(r1:alfa);
      label 99;           
      var i              :integer;
          part_no        :alfa4;
          Semicolon_found:Boolean;
          d:dev_link;
          v:real;u:units;
          first_pin,dummy:boolean;                  
{  This procedure reads a Diode description. A diode is described as 
   follows :                    

   r1:ZENER:Part_no:[pins] Ik 5 mADC , Vz 0.0 VDC , PWR 1 W , TOL 5 % ;
                    
 }                        
      begin            
         i:=0;getch;part_no:=' ';semicolon_found:=false;
         while (not (ch in [':'])) and (i<al4) and (cc<ll) do begin
           i:=i+1; part_no[i]:=ch; getch;
         end;

         if ch=':' then begin
           getsym;{ skip over : }
           end
         else begin
           err_mess:=' Colon expected after part number';
           error(0);goto 99;
         end;

         if ch='[' then begin
           getsym;{get and discard '['}
           get_device_pins(r1,Next_TB_Address,']');
           if not OK then goto 99;
           end
         else begin
           f1:='C';f2:='A';
           x:=Insert_Internal_pin(r1,f1);
           if OK then begin
             x:=insert_internal_pin(r1,f2);
             if not OK then goto 99;
           end;
         end;             

         d:=device_pointer(r1);
         with d^ do begin
           part_no:=part_no;
           part_type:=Zener;
           R_Zener:=10.0;
           Vj_Zener:=0.7;
           V_Zener:=0.0;
           tolerance:=10.0;  { some large value}
           PRV:=100.0; { peak reverse voltage }
         end;
             
         getv;z1:=z;z2.a:=NULL;
         with d^ do begin
           while not semicolon_found do begin
             if (upperalfa(z1.a)='Vz')then begin
                getv;z1:=z;
                get_parm(true,z1,z2,[VDC..KVDC],V_Zener,u);
                normalize(V_Zener,u,V_Zener,u);
                end
             else if(upperalfa(z1.a)='Ik')then begin
                getv;z1:=z;
                get_parm(false,z1,z2,[uADC..ADC],I_knee,u);
                normalize(I_knee,u,I_knee,u);
                end
             else if(upperalfa(z1.a)='PWR')then begin
                getv;z1:=z;
                get_parm(false,z1,z2,[mWATT..WATT],Z_Wattage,u);
                normalize(I_fwd,u,Z_Wattage,u);
                end
             else if(upperalfa(z1.a)='TOL')then begin
                getv;z1:=z;
                get_parm(false,z1,z2,[PCT],Z_Tol,u);
                end
             else if(z1.a=';')then begin
                semicolon_found:=true;
                end
             else begin
                semicolon_found:=true;
                err_mess:=' Unexpected data ';
                error(0);             
             end;
           end;
         end;
         if OK then begin
           if d^.p<>d^.z_p then begin
             x:=d^.p^.i_add;
             if d^.p^.p<>d^.z_p then begin
               y:=d^.p^.p^.i_add;
               if d^.p^.p^.p<>d^.z_p then begin
                 err_mess:=' More then two pins';error(0);goto 99;
               end;
               end
             else begin
               err_mess:='Only one diode pin';
               error(0);goto 99;
             end;
             end
           else begin
             err_mess:='No diode pins defined';
             error(0);goto 99;
           end;

           connect_two_term(x,y,d^.resistance,
                                d^.resistance,
                                d^.tolerance/100.0,
                                d^.Z_wattage);

           omit_node(x,SOFT_OMIT);omit_node(y,SOFT_OMIT);
           d^.max_i_ratio:=0;
         end;

         if ch=';' then getsym; {skip semicolon if it is there}
      
99:
      end{get_zener}; 

    procedure get_psupply(ps:alfa);
      label 99;           
      var i              :integer;
          Semicolon_found:Boolean;
          d:dev_link;
          v:real;u:units;              
          first_pin,dummy:boolean;                  
{  This procedure reads a power supply description. A power supply is 
   described as follows :

   ps:PSUPPLY:;
 }                           
      begin            
         i:=0;
         if ch=':' then begin                           
           getsym;{ skip over : }
           end
         else begin
           err_mess:=' Colon expected after PSUPPLY';
           error(0);goto 99;
         end;

         f1:='POS';f2:='NEG';
         x:=Insert_Internal_pin(ps,f1);
         if not OK then goto 99;
         y:=insert_internal_pin(ps,f2);
         if not OK then goto 99;
                               
         d:=device_pointer(ps);
         d^.part_no:='POWER_SUPPLY';
         d^.part_type:=psupply;
         d^.Hi_Pin:=x; { save for easy reference }
         d^.Ref_Pin:=y;
         d^.PS_Voltage:=0.0;

         inf[x]^.source:=yes;inf[y]^.source:=yes;
         connect_Two_Term(x,y,0.0,0.0,0.0,0.0);
         PSUPPLIES:=PSUPPLIES+1;
99:
      end{get_psupply}; 
                   
  procedure get_pack(fpack:alfa);
      label 99;           
      var i              :integer;
          part_no        :alfa4;
          p_label        :alfa4;
          Semicolon_found,connected:Boolean;
          d:dev_link;
   function subs_pack:name;
     var v:name;
     begin
        getnext_node(a,v);             
        if Upperalfa(v.dev)='PACK' then begin
          v.dev:=fpack;
          end
        else begin
          v.dev:=full_name(fpack,v.dev);
        end;
        subs_pack:=v;
     end;              
                
      begin            
         i:=0;getch;part_no:=' ';semicolon_found:=false;
         while (ch<>':') and (i<al4) and (cc<ll) do begin
           i:=i+1; part_no[i]:=ch; getch;
         end;
            
         if ch<>':' then begin
           err_mess:=' : missing after switch PART NO';
           error(0);goto 99;
         end;
 
         i:=0;getch;p_label:=' ';
         while (i<al4) and (cc<ll) do begin
           i:=i+1; p_label[i]:=ch; getch;
         end; 
   
         get_line_sym;
         if Big_a='TERMINALS' then begin
           getnext;
           if a=':' then begin
             get_device_pins(fpack,Next_TB_Address,';');
             end
           else begin
             err_mess:=' ":"  expected after TERMINALS';
             error(0);goto 99;
           end;
           end            
         else begin         
           Err_Mess:=' TERMINALS keyword is missing!';
           error(0);goto 99;
         end;
         d:=device_pointer(fpack);
         d^.part_type:=Compound_Device;
         d^.part_no:=part_no;
         d^.dev_label:=p_label;
         getnext;                  
         repeat
           get_part(fpack,a);
           getnext;
         until (Big_a='CONNECT') or (NOT OK);
         if Big_A='CONNECT' then begin
           if ch=':' then getnext;{skip if ":" }
           getnext_with(['-']);
           while not semicolon_found do begin
             if a=';' then begin
               semicolon_found:=true;
               end
             else if a='(' then begin
               getnext_with(['-']);
               if symbol then begin 
                 v1:=subs_pack;x:=address_of(v1);
                 if good(x) then begin
                   getnext_with(['-']);   
                   while (a<>')') and OK do begin
                     if symbol then begin
                       v1:=subs_pack;
                       y:=address_of(v1);
                       if good(y) then begin
                         connect_wire(x,y,Wire_Imp,connected);
                         x:=y;
                         getnext_with(['-']);
                         end
                       else { bad y address } begin
                         Err_Mess:='Bad Pack pin:';
                         Err_P1:=v1.Pin;
                         error(1);goto 99;
                       end;
                       end
                     else begin
                       err_mess:='Pin name expected';
                       error(0);
                     end;
                   end;{ while }
                   end
                 else{ bad x address} begin
                   Err_Mess:='Bad Pack pin:';
                   Err_P1:=v1.Pin;
                   error(1);goto 99;
                 end;
                 end
               else { first symbol after ( is bad } begin
                 err_mess:='Pin name expected';
                 error(0);
               end;
               getnext_with(['-']);
               end
             else begin
               err_mess:='Parenthesis or ; missing!';
               error(0);goto 99;
             end;                    
           end{while not semicolon_found};
           getnext;
         end;
                              
         if Big_A<>'END.' then begin
           Err_mess:=' "END." is missing';
           error(0);
         end;
  99:
   end;    

    begin {get_device};          
       getsym;
       if ch=':' then begin
         if Big_a='SWITCH' then begin
           get_switch(full_name(pack,f1));           
           end
         else if Big_a='RELAY' then begin
           get_relay(full_name(pack,f1));
           end 
         else if Big_a='DIODE' then begin
           get_diode(full_name(pack,f1));
           end
         else if Big_a='PSUPPLY' then begin
           get_psupply(full_name(pack,f1));
           end
         else if Big_a='RESISTOR' then begin
           get_resistor(full_name(pack,f1));
           end
         else if Big_a='PACK' then begin
           get_pack(full_name(pack,f1));
           end
         else if Big_a='CAPACITOR' then begin
           end
         else if Big_a='ZENER' then begin
           get_Zener(full_name(pack,f1));
           end
         else begin
           err_mess:='Unknown Part: ';
           err_P1:=a;  
           error(1);
         end;
         end
       else begin
         err_mess:='Missing colon after device type';
         error(0);
       end;
    end{get_device};

    begin {get_part}
        if ch=':' then {device or cable } begin
          if upperalfa(a_dev)='CABLE' then begin
            getsym{throw away ':' };
            if ZIF then begin
              getsym;
              if numero>0 then begin
                ZIF_tester_connections:=numero;
                end
              else begin
                err_mess:='Number expected between :: for ZIF';
                error(0);goto 99;
              end;
              getsym;{throw away ':' };
            end;
       
            getnext_with(['-',' ','$','=']);
            Cable_Name:=a_2;
            get_line_sym;
            while (Big_A<>'END.') do begin
              if a='@' then begin
                getsym;
                if symbol then begin
                  Cab_conn:=Big_A;
                  getsym;
                  if a=':' then begin
                    getsym;
                    if Big_A='N/C' then begin
                      Add_Cable_desc(Cable_Name,Cab_conn,Big_A,NO_NODE);
                      end                               
                    else begin
                      get_sub_cable(Cable_Name,Cab_Conn,True);
                    end;
                    end
                  else begin
                    err_mess:=' ":" expected';
                    error(0);goto 99;
                  end;
                  end
                else begin
                  err_mess:='@connector name expected';
                  error(0);goto 99;
                end;
                end
              else if a[1]='+' then begin
                get_sub_cable(Cable_Name,Cab_Conn,True);
                end
              else if STRIP then begin
                Cab_conn:=NULL;
                get_sub_cable(Cable_Name,Cab_Conn,True);
                end
              else if ZIF  then begin
                Cab_conn:=NULL;
                get_sub_cable(Cable_Name,Cab_Conn,True);
                end
              else begin
                err_mess:=' @ expected';
                error(0);goto 99;
              end;
              get_line_sym;           
            end; 
            Add_Cable_Desc(NULL_2,NULL,NULL,NO_NODE);
            end
          else begin
            if not device_exists(full_name(pack,a_dev)) then begin
              getsym{throw away :}; 
              get_device(pack,a_dev);
              GOTO 99; {nothing more to process in this line}
              end
            else begin
              Err_Mess:=' This device was already defined:';
              err_p1:=conn_name;
              error(1);goto 99;
            end;
          end;
          end
        else begin
          get_sub_cable(NULL_2,NULL,false);
        end;
  99:;            
  end {get_part}; 
         
  procedure interconnect_devs(v1,v2:name);
   var v_1,v_2  :name;
       connected:boolean;
   procedure make_good(var v:name);
    var x:integer;        
    begin
       x:=address_of(V);
       if not good(x) then begin
         if device_exists(V.dev) then begin
           if V.pin=no_pin then begin
             v:=next_dev_pin(v);
             x:=address_of(v);
             if not good(x) then begin
               err_mess:=' Device exists Pin does not:';
               err_p1:=v.dev;err_p2:=v.pin;error(2);
             end;
           end;
           end
         else begin
           err_mess:=' Device does not exist:';
           err_p1:=v.dev;error(1);
         end;
       end;
    end;
   begin
      make_good(v1);make_good(v2);
      if OK then begin
        repeat 
          if (v1.pin<>v2.pin) then begin
            err_mess:=' Pins do not have the same name:';
            err_p1:=v1.pin;err_p2:=v2.pin;warn(2);
          end;
          connect_wire(address_of(v1),address_of(v2),Wire_Imp,connected);
          v_1:=v1;v1:=next_dev_pin(v_1);
          v_2:=v2;v2:=next_dev_pin(v_2);
        until (v_1.pin=v1.pin) or (v_2.pin=v2.pin);
      end;
   end;
  begin { get_parts }          
     Force_XC:=true;
     Section_save:=section;
     section_message_save:=Section_Message;
     Acceptable_Save:=Acceptable;
     Section_Message:='Incomplete Connectors/Parts section';
     Acceptable:=['A'..'Z','a'..'z','0'..'9','*','+','.','/','_'];
     Section:=Parts_Section;
     Initialized:=false;

     strip_pin:=Curr_Add+((Curr_Add-1) mod 3)+1; 
     top_pin:=0;
     old_name:=' ';conn_name:=' ';
     cont_line:=false;

     while true do begin
       f1:=' ';f2:=' ';f3:=' ';           
       if NOT OK then begin 
         get_line_sym;  
         end
       else begin
         getnext;
       end;  
       if (Big_a='/E') then goto 999;
       if (Big_a='TARGET') and (ch='=') then begin
         getsym{=};getsym;
         if Big_a='FACT' then begin
           Target_Machine:=FACT_Machine;
           end
         else if Big_a='DITMCO' then begin
           Target_Machine:=DITMCO_660;
           end
         else begin
           err_mess:='Illegal TARGET type specification:';
           err_p1:=a;error(1);
         end;
         GOTO 99 { end of line };
       end;

       if (Big_a='PIN') and (ch='=') then begin
         getsym{=};getsym;STRIP:=false;ZIF:=false;
         if Big_a='NORMAL' then begin
           Pin_Seq_Mode:=Normal_Seq;getsym;
           if a=' ' then begin
             PPC:=100;
             end
           else if number then begin
             if numero in [60,100]  then begin
               PPC:=numero;
               end         
             else begin
               err_mess:=' Illegal pins/connector:';
               err_p1:=a;error(1);
             end;
             if numero in [60] then begin
               No_At_Field:=true;
             end;
             end
           else begin
             err_mess:=' Pins/connector number expected:';
             err_p1:=a;error(1);
           end; 
           end
         else if Big_a='FD' then begin
           Pin_Seq_Mode:=FD_Seq;
           end
         else if Big_a='STRIP' then begin
           { if any other connector appeared before this, allow them }
           strip_pin:=Curr_Add+( (Curr_add-1) mod 3);
           STRIP:=true;
           No_At_Field:=true;
           end
         else if Big_a='ZIF' then begin
           { if any other connector appeared before this, allow them }
           Pin_Seq_Mode:=Normal_Seq;
           ZIF:=true;
           No_At_Field:=true;
           getsym;
           if end_line then begin
             PPC:=60;
             end
           else begin
             if number then begin
               PPC:=numero;
               end
             else begin
               Err_Mess:='Number of pins for ZIF expected';
               error(0);GOTO 99;
             end;
           end;
           end
         else begin
           err_mess:='Illegal Pin Sequence type specification:';
           err_p1:=a;error(1);
         end;
         GOTO 99 { end of line };          
       end;

       if (Big_a='END.')then begin
         err_mess:=' END. is out of place';error(0);goto 99;
       end;                

       if (Big_a='INTERCONNECT')then begin
         getnext_with(['-']);get_node(a,v1);
         getnext_with(['-']);get_node(a,v2);
         interconnect_devs(v1,v2);
       end;
  
       conn_1:=a;{Big_a};conn_2:=a;{Big_a};   { was CASEFOLD}         
       if ch='-' then begin
         getsym{over '-' };getsym;conn_2:=a;{Big_a};{ was CASEFOLD}
         {now do the range}
         a_dev:=next_pin(conn_1,conn_2,0);
       end;
       if OK then begin
         get_part(NULL,conn_1);
         if conn_1<>conn_2 then begin
           a_dev:=conn_1;  
           repeat
             a_dev:=next_pin(a_dev,conn_2,0);         
             get_part(NULL,a_dev);           
           until not OK or (a_dev=conn_2);
         end;
       end;
  99:  ;          
     end;         
                  
 999:if Error_Count>0 then begin
       Err_Mess:=' Aborting Due to errors';
       Error(0);goto 8888;
     end;              
     { restore everything }
     Section:=Section_save;
     Section_Message:=section_message_save;
     Acceptable:=Acceptable_Save;
end;              
                  
                  
{******************************************************************}
                  
procedure get_wires;
label 999;
const pcs=30;
var prev_conn:array[1..pcs] of alfa;
    f,f2: alfa;
    c_no,i:integer;
    v1,v2:name;   
    more:boolean;
    section_message_save:str79;
    section_save:sections;
    acceptable_save:charset;
          
  procedure get_terminal(
                var  old_conn:alfa    { old connector for this field};
                var  f_old   :alfa    { carry over from last call   };
                var  more    :boolean                                ;
                var  v       :name
                         );
  label 99;       
                  
  var f1:alfa;    
                  
  begin           
      if f_old=null then begin
        getsym; 
        f1:=a;    
        end       
      else begin  
        f1:=f_old;
        f_old:=NULL;
      end;        
                  
      if f1='-' then begin { -pin  format }
        err_mess:=' Pin must be Device-Pin format, -pin is not allowed';
        more:=false;
        error(0);goto 999; 
        end       
      else{ con,pin or con-pin or (old_conn-) pin format}begin
        v.dev:=f1;{upperalfa(f1)}; { was CASEFOLD}
        getsym; 
        if a='-' then { con-pin format } begin
          getsym;
          v.pin:=a;
          if Pin_Exists(v) then begin
            more:=(cc<>ll);
            old_conn:=v.dev;
            end   
          else begin
            err_mess:=' Bad connector/pin :';
            err_p1:=v.dev;
            err_p2:=v.pin;
            error(2);
            more:=false;
          end;    
          end     
        else  begin { try conn,pin format}
          v.pin:=a;
          if Pin_Exists(v) then begin
            old_conn:=v.dev;
            more:=(cc<>ll);
            end   
                  
          else begin
            if a=' ' then f2:=NULL else f2:=a;
            v.pin:=No_Pin;
            if Pin_Exists(v) then { a pinless connector } begin
              f_old:=f2;
              more:=(f2<>NULL);
              end 
            else if (old_conn=null) then begin
              err_mess:=' Bad device/pin :';
              err_p1:=v.dev;
              err_p2:=v.pin;
              error(2);
              more:=false;
              end 
            else { try out (old_conn) pin format } begin
              f_old:=f2;   { remember the last symbol for next pass}
              v.pin:=v.dev;
              v.dev:=old_conn;
              if Pin_Exists(v) then begin
                more:=(f2<>NULL); { still got the f_old to process }
                end
              else begin
                err_mess:=' can not associate pin with a node :';
                err_p1:=v.pin;
                error(1);
                more:=false;
              end;
            end;  
          end;    
        end;      
      end;        
99:end;           
                  
 begin {get_wires}
     Force_XC:=true;
     Section_save:=section;
     section_message_save:=Section_Message;
     Acceptable_Save:=Acceptable;
     Section_Message:='Incomplete Wire list section';
     Acceptable:=['A'..'Z','a'..'z','0'..'9','*','+','.','/','_'];
     Section:=Wires_Section;
                  
     for i:=1 to PCS do prev_conn[i]:=null;
                  
     while true do begin
       get_line_sym;  
       c_no:=0;   
       more:=true;  
       if (Big_a='/E') then goto 999;
       f:=a;      
       while  more do begin
         c_no:=c_no+1;          
         get_terminal(prev_conn[c_no],f,more,v2);
         if c_no>1 then begin
           if OK then
           connect_wire(Address_Of(v1),Address_Of(v2),Wire_Imp,boole);
         end;     
         v1:=v2;  
       end;       
       if (c_no=1) and OK then begin
         err_mess:=' Not enough nodes in line';
         error(0);
       end;       
     end;         

999: if Error_Count>0 then begin
       Err_Mess:=' Aborting Due to errors';
       Error(0);goto 8888;
     end;          
     Section:=Section_save;
     Section_Message:=section_message_save;
     Acceptable:=Acceptable_Save;
end;              
         

{********************************************************************}
function tryable(            t:node;
                        spans:integer;
                 var     span:integer  {really var};
                 var dev_name:alfa     {really var}):boolean;

  BEGIN      
     if short_path(t) then begin
       tryable:=true;span:=0;{visit connected nodes}
       end
     else if t.cnx=contact then begin
       with t do begin
         if (j_d=YES) then begin
           { just disconnected node }  
           if spans=0 then {remember name of device spanned} begin
             dev_name:=inf[t.v]^.dev^.dev;
             tryable:=true;span:=1;
             end
           else if inf[t.v]^.dev^.dev=dev_name then begin
             { we cross multiple opens if they are on the same device}
             tryable:=true;span:=1;
             end
           else begin
             tryable:=false;
           end; 
           end
         else if (o_c=NO) then begin
           if spans=0 then begin
             tryable:=true;span:=1;
             end
           else begin
             tryable:=false;
           end;
           end
         else begin
           tryable:=false;
         end;                                
       end{with};
       end
     else begin
       tryable:=false;
     end;                     
  end;

{********************************************************************}
function first_positive(x,max_span:integer):integer;
var k_pos:integer;
    min_span:integer;

  { This procedure will find the first positive node that is within }
  { max_spans of node x                                             }
  { e.g.  x:=first_positive(x,0); will return the first positive    }
  { node on the same string.                                        }
  { Whereas x:=first_positive(x,3); will return the first positive  }
  { node within three switch (open) jumps                           }

  procedure visit(k,spans:integer);
  var t_node:node_link;
      span:integer;
      dev_name:alfa;

  BEGIN           
    inf[k]^.dfs_visit:=dfs_Search;
    t_node:=adj[k];
    if ext_node(k) and (spans<=max_span) then begin
      if spans<min_span then begin
        min_span:=spans;
        k_pos:=k;
      end;
    end;

    WHILE (t_node <> z_node)  DO BEGIN
      if tryable(t_node^,spans,span,dev_name)then begin
        if inf[t_node^.v]^.dfs_visit<> dfs_Search then begin 
          visit(t_node^.v,spans+span);
        end;
      end;
      t_node:=t_node^.next;
    end;           
  end;            
                  
begin    
    k_pos:=x;
    min_span:=max_span+1;            
    dfs_Search:=SUCC(dfs_Search);
    IF in_use(x) then visit(x,max_span);
    first_positive:=k_pos;                                 
end;              

{********************************************************************}
procedure write_name(var ofile:text;v:integer;var ll:integer);
  var n:name;
      b:alfa2;
      i,l:integer;
  begin
      n:=Int_To_Name(v);
      make_pin_name(n.dev,n.pin,b,l);
      for i:=1 to l do begin 
        write(ofile,b[i]);
      end;
      ll:=ll+l;
      if ll>80-al then begin
        writeln(ofile);write(ofile,'      ');ll:=6;
      end;
  end;

                  
{********************************************************************}
function continuity(x,y:integer;traceback:boolean):boolean;
var    
    found:boolean;
    ll:integer;
  procedure visit(k:integer);
  var t_node:node_link;
  BEGIN           
    inf[k]^.dfs_visit:=dfs_Search;
    t_node:=adj[k];
    WHILE (t_node <> z_node) and (not found) DO BEGIN
      if short_path(t_node^)then begin
        if t_node^.v=y then begin
          found:= true;
          if traceback then begin 
            write_name(OUTPUT,y,ll);
            write(OUTPUT,' ==> ');ll:=ll+5;
          end;
          end
        else begin    
          if inf[t_node^.v]^.dfs_visit<> dfs_Search then begin
            visit(t_node^.v);
            if found and traceback then begin
              write_name(OUTPUT,t_node^.v,ll);
              write(OUTPUT,' ==> ');ll:=ll+5;
            end;
          end;
        end;
      end;
      t_node:=t_node^.next;
    end;           
  end;            
                  
begin             
   dfs_Search:=SUCC(dfs_Search);
   found:=false;ll:=1;     
   IF   (x<=v_max) and
        (y<=v_max) and     
        (x>=v_min) and
        (y>=v_min) and
        (adj[x]<>z_node) and
        (adj[y]<>z_node)      then begin
             visit(x);
             if found and traceback then begin
               write_name(OUTPUT,x,ll);
               writeln(OUTPUT);
             end;
   end;
   continuity := found;
end;              


{*****************************************************************}
{procedure tag_o_c(x,y:integer);
var tx,ty:node_link;
    Done:Boolean;

begin      
   done:=false;
   tx:=adj[x];
   while (tx<>z_node) and not done do begin
     if tx^.v=y and tx^.cnx=contact then begin
       ty:=tx^.other;
       tx^.o_c:=YES;ty^.o_c:=YES;
       tx^.require:=NO_Check;ty^.require:=NO_Check;
       done:=true;                         
       end;
     end;
     tx:=tx^.next;                      
   end;
end;}
    
                  
                  
{********************************************************************}
procedure fs_tests(incremental:boolean;what:wish);             
var start_search,i,v:integer;
    allow:omit_flag;              
                  
 begin            
                  
   { Note: this routine depends on if a node has been visited
           during execution of this routine. so if any routine is
           called that changes the dfs_visit of a node , extreme
           caution should be taken that it does not invalidate
           logic of this routine !!!!!!!!!!!!!!!!!!.
     Caution: Logic of F_Node routine and this routine work
              together to NOT generate f checks for a string that
              contains node 0. If a string contains node 0 then
              it's f_node is 0 since this routine skips 0 , no f checks
              will be generated. }
                  
   dfs_Search:=SUCC(dfs_Search);
   start_search:=dfs_Search;
                  
   for i:=1 to v_Max do begin
     if adj[i]=z_node then begin
       if in_use(i) then begin
         if inf[i]^.omit=NO_OMIT then fs_check(i,incremental,what);
       end;       
       end        
     else {adj[i]<>z_node} begin
       { next statement uses side effect from f_node routine. }
       { if dfs_visit of a node is >=start_search then it is  }
       { in the same string                                   }
       if inf[i]^.dfs_visit<start_search then begin{side effect from f_node}
         v:=f_node(i,allow,false);
         if (i=v) and (allow=NO_OMIT) then begin
           fs_check(v,incremental,what);
         end;
       end;       
     end;         
   end{ for };    
 end;             
                  
{********************************************************************}
procedure f_c_tests(incremental:boolean);
begin
    fs_tests(incremental,fc);
end;             
{********************************************************************}
{********************************************************************}
procedure s_c_tests(incremental:boolean);
begin
    fs_tests(incremental,sc);
end;             

{********************************************************************}
procedure add_param(bl:integer;c:char;xxxxx,digits:integer);      
var a:alfa;    
    i:integer;    

 begin 
       case target_machine of                     
         FACT_Machine:
            begin
              a:=Number_To_Alfa(xxxxx,digits);
              for i:=1 to bl do write_test_blanks(1);
              write_test_c(C);
              for i:=1 to digits do write_test_c(a[i]);
              write_test_ln;
            end;
         DITMCO_660:
            begin
              a:=Number_To_Alfa(xxxxx,digits);
              case c of
                'M':Begin
                      a_alfa2:='*M';
                      Write_test_alfa2(a_alfa2,0);
                      for i:=1 to digits do write_test_c(a[i]);
                      write_test_ln;
                    end;
              OTHERWISE
                DITMCO_PARAM:=Number_To_Alfa(xxxxx,digits);
              end;
            end;
         DITMCO_9100:
            begin
              a:=Number_To_Alfa(xxxxx,digits);
              case c of
                'M':Begin
                      a_alfa2:='*M';
                      Write_test_alfa2(a_alfa2,0);
                      for i:=1 to digits do write_test_c(a[i]);
                      write_test_ln;
                    end;
              OTHERWISE
                DITMCO_PARAM:=Number_To_Alfa(xxxxx,digits);
              end;
            end;
       OTHERWISE
            err_mess:='BUG! BUG! Bad target machine';
       end;
 end;             
{*********************************************************************}
                  
 function withintol(x1,x2,tol:real):boolean;
   begin          
    if x2=0.0 then begin
      withintol:=(abs(x1)<=0.000001);
      end
    else begin
      withintol:=(abs((x1-x2)/x2)<tol);
    end;
   end;           
{*********************************************************************}
                  
 function within(x1,x2:real):boolean;
   begin                          
    within:=withintol(x1,x2,0.0001);
   end;           
{*********************************************************************}
 procedure emit_q_wait(bl:integer;v:real;u:units);
 var  qqq,q2,q3:integer;
      v1,q2r:real;
      u1:units;   
 begin            
     normalize(v,u,v1,u1);q2:=0;q3:=0;
     if (v1=q_value)then begin
       { nothing to do }
       end        
     else if v1=-1.0 then begin
       { nothing to do.no dwell specified let the old one ride}
       end        
     else if (v1<90.001) and (v1>0.0) then begin
       q_value:=v1;
       while v1>0.0901 do begin
         v1:=v1/10.0;
         q3:=q3+1;
       end;       
       q2r:=0.0;  
       while (not within(q2r,v1*100.0)) and (q2r<10.0) do q2r:=q2r+1.0;
       if q2r<10.0 then begin
         q2:=round(q2r);qqq:=q2*10+q3;
         Add_Param(bl,'Q',qqq,3);
         end      
       else begin 
         err_mess:='Dwell time must be single digit precision';
         error(0);
       end;       
       end        
     else begin   
       err_mess:='Bad Dwell parameter';
       error(0);  
     end;         
 end;             
             


{**********************************************************************}
procedure Add_Comm(bl:integer; Comm_P:params);   
label 99;         
var i,xxx,xxxxx,x1,x2,x3,x4,x5,x12,x234,x45:integer;
    y1,y2,rdelay,rx234:real;   
    u_new:units;  
    cch:char;     
    col_1:integer;
    param_1:params;
    delay:integer;
begin
  case target_machine of                             
    FACT_Machine:
       Begin 
         with Comm_P do begin
                               
           { now process command}
     
           case comm of
             Cont_C,   
             Open_C : BEGIN
                        normalize(v,u,y1,u_new);
                       
                        if (y1>=1.0) and (y1<=9.9E6) then begin
                          x3:=0;
                          while (y1>=10.0) do begin
                            y1:=y1/10.0;   
                            x3:=x3+1;
                          end;
                          y1:=y1*10.0;
                          x12:=round(y1);
                          end
                        else begin
                          err_mess:='C/O command Ohms value is out of range';
                          error(0);goto 99;
                        end;
                        normalize(v1,u1,y1,u_new);
                       
                        if      round(y1*1000.0)=10 then x4:=0
                        else if round(y1*1000.0)=50 then x4:=1
                        else if round(y1*100.0) =10 then x4:=2
                        else if round(y1*100.0) =50 then x4:=3
                        else if round(y1*10.0)  =10 then x4:=4
                        else if round(y1*10.0)  =15 then x4:=5
                        else if round(y1*10.0)  =20 then x4:=6
                        else if round(y1*10.0)  =25 then x4:=7
                        else if round(y1*10.0)  =30 then x4:=8
                        else begin
                          err_mess:='C/O Command current is not a good value.';
                          error(0);goto 99;
                        end;
                        emit_q_wait(bl,v2,u2);{ add dwell time first}
                        if relop=le then x5:=0 else x5:=1;
                        xxxxx:=x12*1000+x3*100+x4*10+x5;
                        add_param(bl,'C',xxxxx,5);
                        clear_message(mesaj);
                        if relop=le then begin
                          a_alfa2:='Continuity';add_mess(a_alfa2,mesaj);
                          end
                        else begin
                          a_alfa2:='Open';add_mess(a_alfa2,mesaj);;
                        end;
                        add_relop(relop,mesaj);add_entity(v,u,mesaj);
                        a_alfa2:='At';
                        add_mess(a_alfa2,mesaj);add_entity(v1,u1,mesaj);
                        if v2>0.0 then add_entity(v2,u2,mesaj);
                      END;
             Cont_4 : BEGIN
                        normalize(v,u,y1,u_new);
                       
                        if (y1>=0.001) and (y1<=0.99) then begin
                          if (y1<0.10) then begin
                            x3:=9;y1:=y1*1000.0;
                            end
                          else begin
                            x3:=8;y1:=y1*100;
                          end;       
                          x12:=round(y1);
                          end
                        else begin
                          err_mess:='C4 command Ohms value is out of range';
                          error(0);goto 99;
                        end;
                        normalize(v1,u1,y1,u_new);
                       
                        if      round(y1*1000.0)=10 then x4:=0
                        else if round(y1*1000.0)=50 then x4:=1
                        else if round(y1*100.0) =10 then x4:=2
                        else if round(y1*100.0) =50 then x4:=3
                        else if round(y1*10.0)  =10 then x4:=4
                        else if round(y1*10.0)  =15 then x4:=5
                        else if round(y1*10.0)  =20 then x4:=6
                        else if round(y1*10.0)  =25 then x4:=7
                        else if round(y1*10.0)  =30 then x4:=8
                        else begin
                          err_mess:='C4 Command current is not a good value.';
                          error(0);goto 99;
                        end;
                        emit_q_wait(bl,v2,u2);{ add dwell time first}
                        if relop=le then x5:=0 else x5:=1;
                        xxxxx:=x12*1000+x3*100+x4*10+x5;
                        add_param(bl,'C',xxxxx,5);
                        clear_message(mesaj);
                        a_alfa2:='4-wire Continuity';add_mess(a_alfa2,mesaj);
                        add_relop(relop,mesaj);add_entity(v,u,mesaj);
                        a_alfa2:='At';
                        add_mess(a_alfa2,mesaj);add_entity(v1,u1,mesaj);
                        if v2>0.0 then add_entity(v2,u2,mesaj);
                      END;
                       
             Meas_C,
             Meas_4:  BEGIN
                        normalize(v,u,y1,u_new);
                        if u_new in [VDC,Vac] then begin
                          if u_new=VDC then begin
                            if comm=meas_c then x1:=1 else x1:=7;
                            end
                          else begin
                            if comm=meas_c then begin
                              x1:=4;
                              end  
                            else begin
                              err_mess:='4-wire acv is not available';
                              error(0);goto 99;
                            end;
                          end;
                          if (y1>=0.01) and (y1<=999.0) then begin
                            x5:=0;
                            while (y1>0.999499) do begin
                              y1:=y1/10.0;
                              x5:=x5+1;
                            end;
                            y1:=y1*1000.0;
                            x234:=round(y1);
                            end
                          else begin
                            err_mess:='Meas Voltage value is out of range';
                            error(0);goto 99;
                          end;
                          end
                    
                        else if u_new in [ADC,Aac] then begin
                          if comm=meas_c then begin
                            if u=ADC then x1:=2 else x1:=5;
                            if (y1>=0.0095) and (y1<=0.9995) then begin
                              x5:=0;
                              y1:=y1*1000.0;
                              x234:=round(y1);
                              end
                            else begin
                              err_mess:='Meas Current value is out of range';
                              error(0);goto 99;
                            end;
                            end
                          else begin
                            err_mess:=' 4-wire current meas is not available';
                            error(0);goto 99;
                          end;
                          end
                        else if u_new=Ohm then begin
                          if comm=meas_c then x1:=3 else x1:=6;
                          if (y1>=0.0) and (y1<=9.99E6) then begin
                            x5:=0;
                            if      y1>999500.0 then begin 
                              x5:=4;y1:=y1/10000.0;
                              end
                            else if y1>99950.0  then begin 
                              x5:=3;y1:=y1/1000.0;
                              end
                            else if y1>9995.0  then begin 
                              x5:=2;y1:=y1/100.0;
                              end
                            else if y1>999.5   then begin 
                              x5:=1;y1:=y1/10.0;
                              end
                            else if y1>99.95 then begin 
                              x5:=0;y1:=y1/1.0;
                              end
                            else if y1>9.995 then begin
                              if comm=meas_c then begin
                                x5:=0;y1:=y1/1.0;
                                end
                              else begin { meas_4}
                                x5:=8;y1:=y1/0.1;
                              end;
                              end
                            else if y1>0.9995 then begin
                               if comm=meas_c then begin
                                x5:=0;y1:=y1/1.0;
                                end                 
                              else begin { meas_4}
                                x5:=9;y1:=y1/0.01;
                              end;
                              end
                            else if y1>0.0009995 then begin
                              if comm=meas_c then begin
                                err_mess:=' Can not do in 2-wire ohms';
                                end
                              else begin { meas_4}
                                x5:=5;y1:=y1/0.001;
                              end;
                              end
                            else if y1=0.0  then begin
                              if comm=meas_c then begin
                                x5:=7;y1:=0.0;
                                end
                              else begin { meas_4}
                                x5:=7;y1:=y1/0.001;
                              end;
                              end
                            else begin
                              err_mess:=' Illegal Ohms value';
                              error(0);goto 99;
                            end;
                            x234:=round(y1);
                            end
                          else begin
                            err_mess:='Meas Ohms value is out of range';
                            error(0);goto 99;
                          end;
                        end;        
                        rx234:=x234;
                        if not withintol(rx234,y1,0.0015) then begin
                          err_mess:='Value is beyond resolution of FACT';
                          error(0);goto 99;
                        end;  
                       
                        if relop=le then begin
                          cch:='O';
                          end
                        else if relop=ge then begin
                          cch:='U';
                          end
                        else begin
                          cch:='X';
                        end;
     
                        normalize(v2,u2,y1,u_new);
                        delay:=0;
                        if y1<>0.0 then begin
                          delay:=round(y1*10.0);
                          rdelay:=delay;
                          if (delay<1) or (delay>999) then begin
                            err_mess:=' Delay parameter is out of range';
                            error(0);goto 99;
                            end
                          else if not within(rdelay,y1*10.0) then begin
                            err_mess:=' Delay parameter is illegal';
                            error(0);goto 99;
                          end;
                        end;
                       
                        clear_message(mesaj);    
                        a_alfa2:='Measure ';add_mess(a_alfa2,mesaj);
                        if comm=meas_4 then begin
                          a_alfa2:='4-wire ';add_mess(a_alfa2,mesaj);
                        end;
                        if relop<>eq then add_relop(relop,mesaj);
                        add_entity(v,u,mesaj); 
                        if relop=eq then add_entity(v1,Pct,mesaj);
                        if v2<>0.0 then add_entity(v2,u2,mesaj);
                        
                        xxxxx:=x1*10000+x234*10+x5;
     
                        if (relop=eq) and (comm=meas_c) then begin
                          if v1<>5.0 then begin
                            write_test_blanks(bl);
                            if v1 =10.0 then begin
                              a_alfa:='Y010';
                              write_test_alfa(a_alfa,0);
                              end
                            else if v1=20.0 then begin
                              a_alfa:='Y020';
                              write_test_alfa(a_alfa,0);
                              end
                            else begin
                              err_mess:=' Bad percentage value';
                              error(0);goto 99;
                            end;
                            write_test_ln;
                          end;
                        end;                            
                        if delay<>0 then Add_Param(bl,'U',delay,5);
                        Add_Param(bl,cch,xxxxx,5);
                      END;

     
             Insu_DC: BEGIN
                        param_1:=Comm_P;param_1.comm:=Insu_C;
                        Add_Comm(bl,param_1);
                      END;          
     
             Insu_AC: BEGIN
                        param_1:=Comm_P;param_1.comm:=Insu_C;
                        Add_Comm(bl,param_1);
                      END;             
     
     
             Insu_C:  BEGIN
                        {AC: vac   ramp/ZeroCross ac_curr  dwell  }
                        {DC: vdc   ohm              --     dwell  }
                        {    u,v   u1,v1          u2,v2    u3,v3  }
                        normalize(v,u,y1,u_new);
                        if u_new=VDC then begin
                          if      round(y1)=28.0   then x3:=0
                          else if round(y1)=100.0  then x3:=1
                          else if round(y1)=250.0  then x3:=2
                          else if round(y1)=500.0  then x3:=3
                          else if round(y1)=750.0  then x3:=4
                          else if round(y1)=1000.0 then x3:=5
                          else if round(y1)=1250.0 then x3:=6
                          else if round(y1)=1500.0 then x3:=7
                          else begin
                            err_mess:='Insulation voltage is illegal';
                            error(0);goto 99;
                          end;
                          col_1:=1;
                       
                          normalize(v1,u1,y1,u_new);
                       
                          if (y1<1e5) or (y1>900e6)then begin
                            err_mess:='Insulation Ohms is out of range';
                            error(0);goto 99;
                          end;
                       
                          y1:=y1/1.0E5; { make it 1 to 9000}
                          i:=round(y1);y2:=i;
                          if abs(y1-y2)<0.001 then begin
                            if i<10 then begin
                              x1:=i;x2:=3;
                              end
                            else if i<100 then begin
                              if (i mod 10)<>0 then begin
                                err_mess:='Ohms value is not correct';
                                error(0);goto 99;
                              end;
                              x1:=i div 10;
                              x2:=0;
                              end
                            else if i<1000 then begin
                              if (i mod 100)<>0 then begin
                                err_mess:='Ohms value is not correct';
                                error(0);goto 99;
                              end;   
                              x1:=i div 100;
                              x2:=1;
                              end
                            else begin
                              if (i mod 1000)<>0 then begin
                                err_mess:='Ohms value is not correct';
                                error(0);goto 99;
                              end;
                              x1:=i div 1000;
                              x2:=2;
                            end;
                            end
                          else begin
                            err_mess:='Insulation Ohms value is wrong';
                            error(0);goto 99;
                          end;
                       
                          end
                        else if u_new=VAC then begin { AC dialectric }
                          col_1:=6;
                          if (y1>=99.999) and (y1<=1500.01) then begin
                            x45:=02;
                            while y1>100.0 do begin
                              y1:=y1-50.0;
                              x45:=x45+1;
                            end;
                            if not within(y1,100.0) then begin
                              Err_Mess:=' AC test voltage is illegal';
                              Error(0);goto 99;
                            end;
                            end
                          else begin
                            Err_Mess:='AC test voltage is out of range';
                            Error(0);goto 99;
                          end;
                          normalize(v2,u2,y1,u_new);
                          if (y1>=0.000499) and (y1<=0.01551) then begin
                            x12:=01;
                            while y1>0.00051 do begin
                              y1:=y1-0.0005;
                              x12:=x12+1;
                            end;
                            if not within(y1,0.0005) then begin
                              Err_Mess:=' AC test current is illegal';
                              Error(0);goto 99;
                            end;
                            end
                          else begin
                            Err_Mess:='AC test current is out of range';
                            Error(0);goto 99;
                          end;
                          xxxxx:=x12*1000+9*100+x45;
                          add_param(1,'L',xxxxx,5);
                       
                          x1:=4;
                          if u1=Ramp then x2:=4 else x2:=5;
                          x3:=4;
                        end;
                       
                        normalize(v3,u3,y1,u_new);
                        x5:=0;
                        while y1>0.091 do begin
                          y1:=y1/10.0;
                          x5:=x5+1;
                        end;
                       
                        if abs(round(y1*100.0)-y1*100.0)<0.001 then begin
                          if x5>=0 then begin
                            y1:=y1*100.0;x4:=round(y1);
                            end
                          else begin
                            Err_Mess:='Dwell time is out of range';
                            error(0);goto 99;
                          end;
                          end
                        else begin
                          Err_Mess:='Dwell time is Out of resolution/range';
                          error(0);goto 99;
                       
                        end;
                       
                        xxxxx:=x1*10000+x2*1000+x3*100+x4*10+x5;
                        add_param(col_1,'L',xxxxx,5);
                        clear_message(mesaj);
                        a_alfa2:='Leakage';add_mess(a_alfa2,mesaj);
                        add_entity(v,u,mesaj);
                        if u1=ZeroCross then begin
                          add_unit(u1,mesaj);
                          end
                        else if u1=Ramp then Begin
                          add_unit(u1,mesaj);
                          end
                        else begin
                          add_entity(v1,u1,mesaj);
                        end;
                        if u2<>BadUnit then add_entity(v2,u2,mesaj);
                        add_entity(v3,u3,mesaj);
                      END;
     
              Power_C:BEGIN
                        xxx:=round(v1)*100+round(v);
                        Add_Param(6,'P',xxx,3);
                      END;
     
              
           end {CASE}             
         end {WITH};
       end; {FACT_Machine}
  
    DITMCO_660:                
       Begin 
         with Comm_P do begin
                               
           { now process command}
     
           case comm of
             Cont_C,   
             Open_C : BEGIN
                        normalize(v,u,y1,u_new);
                       
                        if (y1>=1.0) and (y1<=900000.0) then begin
                          x2:=1;
                          while (y1>=10.0) do begin
                            y1:=y1/10.0;   
                            x2:=x2+1;
                          end;
                          x3:=round(y1);
                          end
                        else begin
                          err_mess:='C/O command Ohms value is out of range';
                          error(0);goto 99;
                        end;

                        normalize(v1,u1,y1,u_new);
                       
                        if      round(y1*1000.0)=10 then x1:=0
                        else if round(y1*100.0) =10 then x1:=1
                        else if round(y1*100.0) =50 then x1:=2
                        else if round(y1*10.0)  =10 then x1:=3
                        else if round(y1*10.0)  =15 then x1:=4
                        else if round(y1*10.0)  =20 then x1:=5
                        else if round(y1*10.0)  =25 then x1:=6
                        else if round(y1*1000.0)=05 then x1:=7
                        else begin
                          err_mess:='C/O Command current is not a good value.';
                          error(0);goto 99;
                        end;
                        normalize(v2,u2,y1,u_new);
                        if       y1<0.1   then begin
                          x4:=1;x5:=3;
                          end
                        else if  y1<1.0   then begin
                          x4:=1;x5:=round(y1*10.0);
                          end
                        else if  y1<10.0   then begin
                          x4:=2;x5:=round(y1*1.0);
                          end
                        else if  y1<100.0   then begin
                          x4:=3;x5:=round(y1/10.0);
                          end
                        else begin
                          err_mess:='C/O Command Dwell is not a good value.';
                          error(0);goto 99;
                        end;

                        xxxxx:=x1*10000+x2*1000+x3*100+x4*10+x5;
                        if relop=le then begin
                          add_param(0,'C',xxxxx,5);
                          end
                        else begin
                          add_param(0,'L',xxxxx,5);
                        end;
                        clear_message(mesaj);
                        add_entity(v,u,mesaj);
                        if relop=le then begin
                          a_alfa2:='OR LESS';add_mess(a_alfa2,mesaj);
                          end       
                        else begin
                          a_alfa2:='OR GREATER';add_mess(a_alfa2,mesaj);
                        end;
                        add_entity(v1,u1,mesaj);
                        if v2>0.0 then add_entity(v2,u2,mesaj);
                      END;
                       
     
             Insu_DC: BEGIN
                        param_1:=Comm_P;param_1.comm:=Insu_C;
                        Add_Comm(0,param_1);
                      END;          
     
             Insu_AC: BEGIN
                        param_1:=Comm_P;param_1.comm:=Insu_C;
                        Add_Comm(0,param_1);
                      END;             
     
     
             Insu_C:  BEGIN
                        {AC: vac   ramp/ZeroCross   --     dwell  }
                        {DC: vdc   ohm              --     dwell  }
                        {    u,v   u1,v1          u2,v2    u3,v3  }
                        normalize(v,u,y1,u_new);
                        if u_new=VDC then begin
                          if      round(y1)<=28.0   then begin
                            if round(y1)>=5.0 then begin
                              x1:=0;
                              end
                            else begin
                              err_mess:='Insulation voltage is illegal';
                              error(0);goto 99;
                            end;
                            end
                          else if round(y1) =100.0  then x1:=1
                          else if round(y1) =250.0  then x1:=2
                          else if round(y1) =500.0  then x1:=3
                          else if round(y1) =750.0  then x1:=4
                          else if round(y1) =1000.0 then x1:=5
                          else if round(y1) =1250.0 then x1:=6
                          else if round(y1) =1500.0 then x1:=7
                          else begin
                            err_mess:='Insulation voltage is illegal';
                            error(0);goto 99;
                          end;
                       
                          normalize(v1,u1,y1,u_new);
                       
                          if (y1<1e6) or (y1>9000e6)then begin
                            err_mess:='Insulation Ohms is out of range';
                            error(0);goto 99;
                          end;
                          x2:=2;
                          y1:=y1/1.0E6; { make it 1 to 9000}
                          while y1>9.5 do begin
                            y1:=y1/10.0;x2:=x2+1;
                          end;
                          x3:=round(y1);
                          y2:=x3;
                          if not within(y1,y2) then begin
                            err_mess:='Ohms value is outside DITMCO resolution';
                            error(0);goto 99;
                          end;
                          end
                        else if u_new=VAC then begin { AC dialectric }
                          if y1=500.0 then begin
                            x1:=3;
                            end
                          else if y1=750.0 then begin
                            x1:=4;
                            end
                          else if y1=1000.0 then begin
                            x1:=5;
                            end
                          else if y1=1500.0 then begin
                            x1:=7;
                            end
                          else begin
                            Err_Mess:=' AC test voltage is illegal';
                            Error(0);goto 99;
                          end;
                          x2:=0;x3:=0;
                        end;
                       
                        normalize(v3,u3,y1,u_new);
                        x5:=0;
                        if y1=0.0 then begin
                          x4:=2;x5:=5;
                          end 
                        else if (y1>0.95) and (y1<9.5) then begin
                          x4:=2;x5:=round(y1);
                          end
                        else if (y1>9.5) and (y1<90.5) then begin
                          x4:=3;x5:=round(y1/10.0);
                          end
                        else begin
                          Err_Mess:=' Illegal Dwell time';
                          Error(0);goto 99;
                        end;
                        xxxxx:=x1*10000+x2*1000+x3*100+x4*10+x5;
                        add_param(0,'S',xxxxx,5);
                        clear_message(mesaj);
                        add_entity(v,u,mesaj);
                        if u1=ZeroCross then begin
                          end
                        else if u1=Ramp then Begin
                          end
                        else begin
                          add_entity(v1,u1,mesaj);
                        end;
                        add_entity(v3,u3,mesaj);
                      END;
     
              Power_C:BEGIN
                        err_mess:=' Relays 96 thru 99 not available yet';
                        error(0);goto 99;
                      END;
          
                                  
           OTHERWISE
              Err_Mess:=' Bad DITMCO_660 command';error(0);goto 99;
           end {CASE}  
         end {WITH};
       end; {DITMCO_660}

    DITMCO_9100:                
       Begin 
         with Comm_P do begin
                               
           { now process command}
     
           case comm of
             Cont_C,   
             Open_C : BEGIN
                        normalize(v,u,y1,u_new);
                       
                        if (y1>=1.0) and (y1<=900000.0) then begin
                          x2:=1;
                          while (y1>=10.0) do begin
                            y1:=y1/10.0;   
                            x2:=x2+1;
                          end;
                          x3:=round(y1);
                          end
                        else begin
                          err_mess:='C/O command Ohms value is out of range';
                          error(0);goto 99;
                        end;

                        normalize(v1,u1,y1,u_new);
                       
                        if      round(y1*1000.0)=10 then x1:=0
                        else if round(y1*100.0) =10 then x1:=1
                        else if round(y1*100.0) =50 then x1:=2
                        else if round(y1*10.0)  =10 then x1:=3
                        else if round(y1*10.0)  =15 then x1:=4
                        else if round(y1*10.0)  =20 then x1:=5
                        else if round(y1*10.0)  =25 then x1:=6
                        else if round(y1*1000.0)=05 then x1:=7
                        else begin
                          err_mess:='C/O Command current is not a good value.';
                          error(0);goto 99;
                        end;
                        normalize(v2,u2,y1,u_new);
                        if       y1<0.1   then begin
                          x4:=1;x5:=3;
                          end
                        else if  y1<1.0   then begin
                          x4:=1;x5:=round(y1*10.0);
                          end
                        else if  y1<10.0   then begin
                          x4:=2;x5:=round(y1*1.0);
                          end
                        else if  y1<100.0   then begin
                          x4:=3;x5:=round(y1/10.0);
                          end
                        else begin
                          err_mess:='C/O Command Dwell is not a good value.';
                          error(0);goto 99;
                        end;

                        xxxxx:=x1*10000+x2*1000+x3*100+x4*10+x5;
                        if relop=le then begin
                          add_param(0,'C',xxxxx,5);
                          end
                        else begin
                          add_param(0,'L',xxxxx,5);
                        end;
                        clear_message(mesaj);
                        add_entity(v,u,mesaj);
                        if relop=le then begin
                          a_alfa2:='OR LESS';add_mess(a_alfa2,mesaj);
                          end       
                        else begin
                          a_alfa2:='OR GREATER';add_mess(a_alfa2,mesaj);
                        end;
                        add_entity(v1,u1,mesaj);
                        if v2>0.0 then add_entity(v2,u2,mesaj);
                      END;
                       
     
             Insu_DC: BEGIN
                        param_1:=Comm_P;param_1.comm:=Insu_C;
                        Add_Comm(0,param_1);
                      END;          
     
             Insu_AC: BEGIN
                        param_1:=Comm_P;param_1.comm:=Insu_C;
                        Add_Comm(0,param_1);
                      END;             
     
     
             Insu_C:  BEGIN
                        {AC: vac   ramp/ZeroCross   --     dwell  }
                        {DC: vdc   ohm              --     dwell  }
                        {    u,v   u1,v1          u2,v2    u3,v3  }
                        normalize(v,u,y1,u_new);
                        if u_new=VDC then begin
                          if      round(y1)=5.0    then x1:=0
                          else if round(y1)=28.0   then x1:=0
                          else if round(y1)=100.0  then x1:=1
                          else if round(y1)=250.0  then x1:=2
                          else if round(y1)=500.0  then x1:=3
                          else if round(y1)=750.0  then x1:=4
                          else if round(y1)=1000.0 then x1:=5
                          else if round(y1)=1250.0 then x1:=6
                          else if round(y1)=1500.0 then x1:=7
                          else begin
                            err_mess:='Insulation voltage is illegal';
                            error(0);goto 99;
                          end;
                       
                          normalize(v1,u1,y1,u_new);
                       
                          if (y1<1e6) or (y1>9000e6)then begin
                            err_mess:='Insulation Ohms is out of range';
                            error(0);goto 99;
                          end;
                          x2:=2;
                          y1:=y1/1.0E6; { make it 1 to 9000}
                          while y1>9.5 do begin
                            y1:=y1/10.0;x2:=x2+1;
                          end;
                          x3:=round(y1);
                          y2:=x3;
                          if not within(y1,y2) then begin
                            err_mess:='Ohms value is outside DITMCO resolution';
                            error(0);goto 99;
                          end;
                          end
                        else if u_new=VAC then begin { AC dialectric }
                          if y1=500.0 then begin
                            x1:=3;
                            end
                          else if y1=750.0 then begin
                            x1:=4;
                            end
                          else if y1=1000.0 then begin
                            x1:=5;
                            end
                          else if y1=1500.0 then begin
                            x1:=7;
                            end
                          else begin
                            Err_Mess:=' AC test voltage is illegal';
                            Error(0);goto 99;
                          end;
                          x2:=0;x3:=0;
                        end;
                       
                        normalize(v3,u3,y1,u_new);
                        x5:=0;
                        if y1=0.0 then begin
                          x4:=2;x5:=5;
                          end 
                        else if (y1>0.95) and (y1<9.5) then begin
                          x4:=2;x5:=round(y1);
                          end
                        else if (y1>9.5) and (y1<90.5) then begin
                          x4:=3;x5:=round(y1/10.0);
                          end
                        else begin
                          Err_Mess:=' Illegal Dwell time';
                          Error(0);goto 99;
                        end;
                        xxxxx:=x1*10000+x2*1000+x3*100+x4*10+x5;
                        add_param(0,'S',xxxxx,5);
                        clear_message(mesaj);
                        add_entity(v,u,mesaj);
                        if u1=ZeroCross then begin
                          end
                        else if u1=Ramp then Begin
                          end
                        else begin
                          add_entity(v1,u1,mesaj);
                        end;
                        add_entity(v3,u3,mesaj);
                      END;
     
              Power_C:BEGIN
                        err_mess:=' Relays 96 thru 99 not available yet';
                        error(0);goto 99;
                      END;
          
           OTHERWISE
              Err_Mess:=' Bad DITMCO_660 command';error(0);goto 99;
           end {CASE}  
         end {WITH};
       end; {DITMCO_9100}
  OTHERWISE
       err_mess:='Add_Comm: Target machine specification error';
       error(0);goto 99;
  end;

  add_wc:=true;   
  
99:end;           

{**********************************************************************}
procedure TARGET_code_gen(level:integer;c:cmnd;toto:node_types);
label 99;
var

      Param_1:Params;
      x:integer; 
      v_pwr,v_ohm:real; u_pwr,u_ohm:units;
      curr,curr1,curr2:real;
                                         
      part_cmnd:cmnd;

  procedure next_code_gen(toto:node_types);
   begin    
        if (c.next<>NIL) and OK then begin
          c:=c.next^;
          TARGET_code_gen(level,c,toto);
        end;
   end;

begin {Target_Code_Gen}
    if OK then begin
        Part_Cmnd.next:=NIL;

        case c.pp.PT of
          NodePhrase:                               
             BEGIN
               level:=level+1;
               x:=c.pp.add;
               if x<0 then x:=first_positive(x,0);
               if ext_node(x) then begin
                 if level=1 then begin
                   Add_Node(1,Any_From,x);{**fix**}
                   end
                 else begin     
                   if c.pp.code='V' then begin
                     Add_Node(6,Power_To,x);{**fix**}
                     end          
                   else begin
                     Add_Node(6,toto,x);{**fix**}
                   end
                 end;
                 next_code_gen(toto);
                 end
               else begin
                 Err_Mess:=' No external path to node:';
                 Err_P1:=c.pp.n.dev;Err_P2:=c.pp.n.pin;
                 Error(2);
               end;
             END;                                
  
          CommPhrase:
             BEGIN
               case c.pp.comm of
                 Meas_C:
                    BEGIN
                      Add_Comm(6,c.pp);
                      Next_code_gen(meas_to);
                    END;  

                 Meas_4:
                    BEGIN                  
                      { Calculate test current}
                      { Cxxxxx }
                      param_1:=C_Params;

                      with param_1 do begin
                        v:=2.0   ; u:=Ohm  ; v1:=c.pp.v3; u1:=c.pp.u3;
                        v2:=-1.0 ; u2:=sec ; { delay is DVM delay }
                      end;
                      
                      if c.pp.u3 in [madc..ADC] then begin
                        with param_1 do begin
                          v1:=c.pp.v3; u1:=c.pp.u3;
                        end;
                        end
                      else if c.pp.u3 in [WATT] then begin {calculate current}
                        { first calculate current for resistor at 20 volts}
                        {    weeeellll really 20.0-2.0=18 volts }
                        normalize(c.pp.v,c.pp.u,v_ohm,u_ohm);
                        curr1:=(20.0-2.0)/(v_ohm);{ max 20 volts on fact}

                        { now multiply it by tolerance to make sure it }
                        {   it will not go over 20 volts before it is  }
                        {   out of tolerance                           }
                        if c.pp.relop=eq then begin
                          curr1:=curr1*(1.00-c.pp.v1/100.0)
                        end;

                        { now calculate current from maximum rating    }
                        {  of the resistor.                            }
                        normalize(c.pp.v3,c.pp.u3,v_pwr,u_pwr);
                        curr2:=sqrt(v_pwr/v_ohm);
                        if curr1<curr2 then curr:=curr1 else curr:=curr2;
                        if      curr<0.05            then  curr:=0.01
                        else if curr<0.10            then  curr:=0.05
                        else if curr<0.50            then  curr:=0.10
                        else if curr<1.00            then  curr:=0.50
                        else if curr<1.50            then  curr:=1.00
                        else if curr<2.00            then  curr:=1.50
                        else if curr<2.50            then  curr:=2.00
                        else if curr<3.00            then  curr:=2.50
                        else                               curr:=3.00;
                        Param_1.v1:=curr; Param_1.u1:=ADC;
                      end;
                      Add_Comm(6,param_1);           
                      if c.pp.relop=eq then begin
                        { lower limit }
                        { Uxxxxx Txxxxx Oxxxxx Txxxxx }
                        param_1:=c.pp;
                        param_1.relop:=ge;
                        param_1.v:=c.pp.v*(1.00-c.pp.v1/100.0);
                        Add_Comm(6,param_1);
                        Add_Node(6,Meas_To,c.next^.pp.add);

                        { upper limit }
                        param_1:=c.pp;
                        param_1.relop:=le;
                        param_1.v:=c.pp.v*(1.00+c.pp.v1/100.0);
                        Add_Comm(6,param_1);
                        end
                      else begin
                        { lower or upper limit test }
                        { Uxxxxx Txxxxx   or   Oxxxxx Txxxxx}
                        param_1:=c.pp;
                        Add_Comm(6,param_1);
                      end;
                      next_code_gen(meas_to);
                    END;
                                
                 Power_C:                               
                    BEGIN
                      param_1:=c.pp;
                      Add_Comm(6,param_1);
                      param_1.v :=Power_Relay;
                      param_1.v1:=1.0;
                      if c.next^.pp.pt=NodePhrase then begin
                        Part_cmnd.pp:=c.next^.pp;Part_Cmnd.pp.code:='V';
                        Part_Cmnd.next:=NIL;
                        TARGET_code_gen(level,Part_Cmnd,Power_to);
                        end
                      else begin
                        err_mess:=' Internal V node error ';
                        error(0);goto 99;
                      end;
                      Add_Comm(6,param_1);
                      c:=c.next^;
                      next_code_gen(power_to);
                    END;

                 Cont_c: 
                    BEGIN
                      Add_Comm(6,c.pp);
                      next_code_gen(Cont_to);
                    END;

                 Open_c: 
                    BEGIN
                      Add_Comm(6,c.pp);
                      next_code_gen(Open_to);
                    END;

                 OTHERWISE { no checks for now }

                     Add_Comm(6,c.pp);
                     next_code_gen(toto);

               end;
                  
                  
             END; 
                  
                  
          EndSentence: ;{ no code generation }
                  
        end;      
    end;
99: { get out only }              
end;              

                    
                                              
{******************************************************************}
function Circuit_Initialized:boolean;
var device:dev_link;
begin
    if not initialized then begin
      device:=f_dev;
      while device<>z_dev do begin
        if device^.part_type in [Switch,Relay] then begin
          if device^.curr_state=' ' then begin  
            initialized:=false;
          end;
        end;
        device:=device^.d;
      end;
    end;
    Circuit_Initialized:=Initialized;
end;  


{********************************************************************}
procedure swap_maybe(var Hi,Lo:real);
var temp:real;
begin
    if Lo>Hi then begin
      Temp:=Lo;Lo:=Hi;Hi:=temp;
    end;
end;

{********************************************************************}
    function y_meas(x,y:real):measurement;
    var z:measurement;
    begin              
        z.HiLimit:=x*(1.0/(1.0-y));
        z.nominal:=x;
        z.LoLimit:=x*(1.0/(1.0+y));
        z.u:=Mho;
        y_meas:=z;
    end;               

{********************************************************************}
function sub_meas(x,y:measurement):measurement;
var z:measurement;
begin
    if x.u=y.u  then begin
      z.HiLimit:=x.HiLimit - y.HiLimit;
      z.Nominal:=x.Nominal - y.Nominal;
      z.LoLimit:=x.LoLimit - y.LoLimit;
      z.u:=x.u;
      end
    else begin
      err_mess:='Internal error. Bad measurement algebra';
      error(0);HALT;
    end;
    sub_meas:=z;
end;                            

{********************************************************************}
function Add_meas(x,y:measurement):measurement;
var z:measurement;
begin
    if x.u=y.u  then begin
      z.HiLimit:=x.HiLimit+y.HiLimit;
      z.Nominal:=x.Nominal+y.Nominal;
      z.LoLimit:=x.LoLimit+y.LoLimit;
      z.u:=x.u;
      end
    else begin
      err_mess:='Internal error. Bad measurement algebra';
      error(0);HALT;
    end;
    Add_meas:=z;
end;                            


{********************************************************************}
function Mul_meas(x,y:measurement;u:units):measurement;
var z:measurement;
    temp:real;
begin
    z.HiLimit:=x.HiLimit*y.HiLimit;
    z.Nominal:=x.Nominal*y.Nominal;
    z.LoLimit:=x.LoLimit*y.LoLimit;
    z.u:=u;
    Mul_meas:=z;
end;                            

{********************************************************************}
function div_meas(x,y:measurement;u:units):measurement;
var z:measurement;
    temp:real;
  function dv(x,y:real):real;
  begin                  
      if x=0.0 then dv:=0 else if y=0.0 then dv:=1.0e38 else dv:=x/y;
  end;

begin                                                         
    z.HiLimit:=dv(x.HiLimit,y.HiLimit);
    z.Nominal:=dv(x.Nominal,y.Nominal);
    z.LoLimit:=dv(x.LoLimit,y.LoLimit);
    z.u:=u;
    div_meas:=z;
end;                            

{********************************************************************}
function neg_meas(x:measurement):measurement;
var z:measurement;
begin
    neg_Meas:=Sub_Meas(nom_meas(0.0,x.u),x)
end;                            

{********************************************************************}
function Max_meas(x:measurement;y:real):real;
 begin
    if abs(x.Hilimit)>abs(y) then y:=x.Hilimit;
    if abs(x.Nominal)>abs(y) then y:=x.Nominal;
    if abs(x.Lolimit)>abs(y) then y:=x.Lolimit;
    Max_Meas:=y;
 end;
{********************************************************************}
function diff_voltage(x,y:integer;tag:boolean):measurement;
 var done:boolean; voltage:measurement;
     ratio:real;
 procedure visit(k:integer;volt:measurement);
  var t_node:node_link;
      add:integer;
  BEGIN
    inf[k]^.dfs_visit:=dfs_Search;
    t_node:=adj[k];          
    WHILE (t_node <> z_node) and ((not done) or tag)  DO BEGIN
      if any_path(t_node^) then begin
        if tag then begin
          if t_node^.cnx=Admittance then begin
            with t_node^ do begin
              ratio:=y_cnx*v_cnx.nominal/Ohms_Curr;
              if ratio>inf[k]^.dev^.max_i_ratio then begin
                inf[k]^.dev^.max_i_ratio:=ratio;
              end;
            end;
          end;
        end;
        if t_node^.v=y then begin
          if not done then begin
            done:=true;voltage:=add_meas(volt,t_node^.v_cnx);
          end;
          end
        else if (inf[t_node^.v]^.dfs_visit<> dfs_Search) then begin
          visit(t_node^.v,add_meas(volt,t_node^.v_cnx));
        end;
      end;
      t_node:=t_node^.next;
    end
  end;
  
begin { diff_voltage }
  done:=false;
  dfs_Search:=dfs_Search+1;
  IF (x<=v_max) and (x>=v_min) then begin
    visit(x,Nom_Meas(0.0,VDC));
    end
  else begin
    err_mess:='Internal error at diff_voltage.Bad node specified';
    error(0);goto 8888;
  end;
  if done then begin 
    diff_voltage:=neg_meas(voltage);
    end
  else begin
    err_mess:='Internal path error in Diff_Voltage. Report';
    error(0);goto 9999;
  end;
end;

{********************************************************************}
procedure Evaluate_relay(v0:integer;var changed:boolean);
 var d:dev_link;
     t_coil:coil_link;
     found,energized:boolean;
     X1,X2:integer;
     match:boolean;
 procedure energize(v1,v2:integer;var energized:boolean);
  var t_cond:condition_link;
      satisfied:boolean;
      voltage:measurement;
  function check_condition(c:condition;v:measurement):boolean;
    var cc:boolean;                   
    begin      
       case c.relop of
         lt:cc:=(v.Nominal< c.v);
         le:cc:=(v.Nominal<=c.v);
         eq:cc:=(v.Nominal =c.v);
         ge:cc:=(v.Nominal>=c.v);
         gt:cc:=(v.Nominal> c.v);
       end;
       check_condition:=cc;
    end;                      

    procedure goto_condition(c:condition;var went:boolean);
     var t_state:state_link;
     var old_pos:alfa;yet:boolean;                                  
     begin
         t_state:=d^.s;went:=false;yet:=false;
         while t_state<>NIL do begin
           if t_state^.cond=c.desc then begin
             if not yet then begin
               switch_state(Relay,Dev_Link_To_Name(d)
                                      ,t_state^.desc,t_state^.name,old_pos);
               if t_state^.desc<>old_pos then begin
                 went:=true;
                 if debug_on then begin
                   writeln(OutPut,' Relay:',d^.dev,' Switched ');
                   writeln(OutPut,'  From:',Old_Pos,' To:',t_state^.desc);
                 end;
               end;
               yet:=true;
               end
             else begin
               err_mess:=' Conflict in conditions in relay:';
               err_p1:=d^.dev;error(1);goto 9999;
             end;
           end;
           t_state:=t_state^.next;
         end;
     end;

  begin {energize}
     t_cond:=d^.cond;
     while (t_cond<>NIL) do begin
       with t_cond^ do begin
         if v1=term1 then match:=(v2=term2);
         if v2=term1 then match:=(v1=term2);
         if match then begin
           X1:=term1;X2:=term2;
           voltage:=diff_voltage(X1,X2,false);
           satisfied:=check_condition(t_cond^,voltage);
           if satisfied then begin       
             goto_condition(t_cond^,energized);
           end;
         end;
       end;
       t_cond:=t_cond^.next;
     end;
  end;
 begin {evaluate_relay }
    Changed:=false;
    d:=inf[v0]^.dev;found:=false;t_coil:=d^.coils;
    while t_coil<>NIL do begin
      with d^.coils^ do begin
        if (v0=x) or (v0=y) then begin
          energize(x,y,energized);
          Changed:=Changed or energized;
        end;
      end;
      t_coil:=t_coil^.next;
    end;            
 end;

                      
         
{********************************************************************}
procedure relax_wires(v0:integer;relax_mode:relax_modes;epsilon:real);
{ this procedure will calculate voltages  from v0 thru all associated paths}
{  Note that v0 must be a power supply pin. (otherwise this routine will
   select one for you )}

  const iterations=100;

  var delta:real;
      iteration:integer;
      sum1,sum2,sum3:measurement;         
      correction,tension,differ:measurement;      
      Zero_VDC,Zero_MHO,Zero_ADC:measurement;    

     function Node_Voltage(n:integer):Measurement;

        begin
           case relax_mode of
              ideal_wire:Node_Voltage:=inf[inf[n]^.x_node]^.voltage;
              real_wire :Node_Voltage:=inf[n]^.voltage;
           end;
        end;
 
      procedure Set_Node_Voltage(n:integer;volts:measurement);

        begin
           case relax_mode of
              ideal_wire:inf[inf[n]^.x_node]^.voltage:=volts;
              real_wire :inf[n]^.voltage:=volts;
           end;
        end;
                                               

procedure relax(v0:integer); 

  var  search_start:integer;
       dum1:measurement;
 

    procedure visit(k,k0:integer;           { Node we are visiting, where from }
                    Continuous:boolean);    { k,k0 connected ?                 }
   

    var   t_node:node_link;

       procedure xfer(t_node:node_link;zero:boolean);
        var Zero_Meas:Measurement;

        begin
           with t_node^ do begin
             if zero then begin
               v_cnx:=Zero_VDC;
               other^.v_cnx:=Zero_VDC;
               end
             else begin
               v_cnx:=sub_meas(v_cnx,differ);
               other^.v_cnx:=add_meas(other^.v_cnx,differ);
             end;
           end;
        end;
          
       function no_resistance(t:node):boolean;

        begin                 
           case relax_mode of
              ideal_wire:no_resistance:=short_path(t);
              real_wire :no_resistance:=false;
           end;
        end;             


          
       procedure adjust_v_cnx(v:integer);
        var t_node:node_Link;
        begin          
           t_node:=adj[v];                
           while t_node<>z_node do begin
             if resis_graph(v,t_node^.v) then begin
               xfer(t_node,no_resistance(t_node^));
             end;
             t_node:=t_node^.next;
           end;
        end;   
                                        
       procedure get_sums(v:integer);
        { This routine will calculate three crucial sums for 
          each node. Kirchhoff current law is used }

        { given a node then by Kirchoff current law we have:  }


        var t_node:node_Link;
            yy,yv,ii,vd:measurement;
            voltage,Vz,Vj,Yd:real;

        begin {get_sums}         
           t_node:=adj[v];                
           while t_node<>z_node do begin
             case t_node^.cnx of
               admittance,Relay_Coil:
                  BEGIN
                    yy:=y_meas(t_node^.y_cnx,t_node^.tol);
                    sum2:=add_meas(sum2,yy);{admittance}
                    yv:=mul_meas(yy,t_node^.v_cnx,ADC);
                    sum1:=add_meas(sum1,yv);{current}     
                   END;
               contact,wire:
                   BEGIN
                     if (relax_mode=real_wire) then begin
                       if short_path(t_node^)then begin
                         yy:=y_meas(t_node^.y_cnx,t_node^.tol);
                         sum2:=add_meas(sum2,yy);{admittance}
                         yv:=mul_meas(yy,t_node^.v_cnx,ADC);
                         sum1:=add_meas(sum1,yv);{current}
                       end;
                     end;
                   END;
               P_Supply:
                   BEGIN
                     with inf[v]^.dev^ do begin
                       if v=Hi_Pin then begin
                         voltage:=PS_voltage;
                         end
                       else if v=Ref_Pin then begin
                         voltage:=-PS_voltage;
                         end
                       else begin
                         err_mess:='Internal logic error at get_sums';
                         error(0);
                       end;
                     end;
                     yy:=y_meas(t_node^.y_cnx,t_node^.tol);
                     sum2:=add_meas(sum2,yy);{admittance}
                     yv:=mul_meas(yy,t_node^.v_cnx,ADC);
                     sum1:=add_meas(sum1,yv);{current}     

                     ii:=nom_meas(voltage*t_node^.y_cnx,ADC);
                     sum3:=add_meas(sum3,ii);{current}
                   end;
               PN_Diode:
                   BEGIN
                     with inf[v]^.dev^ do begin
                       Yd:=1.0/R_diode;
                       Vj:=Vj_diode;        { 0.7 for Si , 0.3 for Ge }
                     end;
                     case t_node^.Terminal of
                       Diode_Anode:
                           BEGIN
                             with t_node^ do begin
                               if v_cnx.nominal>Vj then begin 
                                 yy:=Nom_Meas(Yd,MHO);
                                 sum2:=add_meas(sum2,yy);{admittance}
                                 vd:=sub_meas(v_cnx,nom_meas(Vj,VDC));
                                 yv:=mul_meas(yy,vd,ADC);
                                 sum1:=add_meas(sum1,yv);{current}        
                               end;
                             end;
                           END;    
                         Diode_Cathode:
                           BEGIN
                             with t_node^ do begin
                               if v_cnx.nominal<-Vj then  begin
                                 yy:=Nom_Meas(Yd,MHO);
                                 sum2:=add_meas(sum2,yy);{admittance}
                                 vd:=add_meas(v_cnx,nom_meas(Vj,VDC));
                                 yv:=mul_meas(yy,vd,ADC);
                                 sum1:=add_meas(sum1,yv);{current}        
                               end;
                             end;
                           END;    
                       OTHERWISE
                         Err_Mess:='Internal error at get_sums(1)!!';
                         error(0);
                       END;
                     END;
               Zener_Diode:
                   BEGIN
                     With inf[v]^.dev^ do begin
                       Vz:=V_Zener;
                       Vj:=Vj_zener;
                       Yd:=1/R_zener;
                     end;
                     case t_node^.Terminal of
                       Zener_Anode:
                           BEGIN
                             with t_node^ do begin
                               if v_cnx.nominal>Vj then begin 
                                 yy:=Nom_Meas(Yd,MHO);
                                 sum2:=add_meas(sum2,yy);{admittance}
                                 vd:=sub_meas(v_cnx,nom_meas(Vj,VDC));
                                 yv:=mul_meas(yy,vd,ADC);
                                 sum1:=add_meas(sum1,yv);{current}        
                                 end
                               else if v_cnx.nominal<-Vz then  begin
                                 yy:=Nom_Meas(Yd,MHO);
                                 sum2:=add_meas(sum2,yy);{admittance}
                                 vd:=add_meas(v_cnx,nom_meas(Vz,VDC));
                                 yv:=mul_meas(yy,vd,ADC);
                                 sum1:=add_meas(sum1,yv);{current}        
                               end;
                             end;
                           END;    
                         Zener_Cathode:
                           BEGIN
                             with t_node^ do begin
                               if v_cnx.nominal>Vz then begin 
                                 yy:=Nom_Meas(Yd,MHO);
                                 sum2:=add_meas(sum2,yy);{admittance}
                                 vd:=sub_meas(v_cnx,nom_meas(Vz,VDC));
                                 yv:=mul_meas(yy,vd,ADC);
                                 sum1:=add_meas(sum1,yv);{current}        
                                 end
                               else if v_cnx.nominal<-Vj then  begin
                                 yy:=Nom_Meas(Yd,MHO);
                                 sum2:=add_meas(sum2,yy);{admittance}
                                 vd:=add_meas(v_cnx,nom_meas(Vj,VDC));
                                 yv:=mul_meas(yy,vd,ADC);
                                 sum1:=add_meas(sum1,yv);{current}        
                               end;
                             end;
                           END;    
                       OTHERWISE
                         Err_Mess:='Internal error at get_sums(1)!!';
                         error(0);
                       END;
                     END;
             OTHERWISE
               Err_Mess:='Internal error at get_sums(2)!!';
               error(0);
             end;

             t_node:=t_node^.next;
           end;                                                  
           if (v=OHMS_HI) then begin
             sum3:=add_meas(sum3,Nom_Meas(Ohms_Curr,ADC));
             end
           else if (v=OHMS_LO) then begin
             sum3:=add_meas(sum3,Nom_Meas(-Ohms_Curr,ADC));
           end;
        end;   
                     
    BEGIN {visit}                
      dfs_Search:=dfs_Search+1;inf[k]^.dfs_visit:=dfs_Search;
      t_node:=adj[k];
      WHILE (t_node <> z_node) DO BEGIN
        with t_node^ do begin
          if inf[v]^.dfs_visit<=Search_Start then begin{not visited yet}
            if no_resistance(t_node^) then begin { hard connected }
              visit(v,k,relax_mode=ideal_wire);
              end
            else if any_path(t_node^) then begin  { span device } 
              { adjust target nodes voltage }
              tension:=add_meas(Node_Voltage(k),v_cnx);
              Set_Node_Voltage(v,tension);
              visit(v,k,false);
              { readjust voltage now that we are back }
              tension:=add_meas(Node_Voltage(k),v_cnx);
              Set_Node_Voltage(v,tension);
            end;
            end
          else begin
            if no_resistance(t_node^) then begin
              Differ:=v_cnx;
              xfer(t_node,true);
              Delta:=Max_Meas(differ,Delta);
              end
            else begin
              correction:=sub_meas(Node_Voltage(v),Node_Voltage(k));
              correction:=add_meas(correction,v_cnx);
              correction:=div_meas(correction,nom_meas(2.0,NoUnit),VDC);   
              differ:=sub_meas(v_cnx,correction);
              xfer(t_node,false);
              Delta:=Max_Meas(differ,Delta);
            end;
          end;
        end;
        t_node:=t_node^.next;
      end;                                                       
      if not continuous then begin
        sum1:=Zero_ADC;
        sum2:=Zero_MHO;
        sum3:=Zero_ADC;           
        bfs_node(k,get_sums,no_resistance);
        differ:=add_meas(sum1,sum3);
        differ:=div_meas(differ,sum2,VDC);
        bfs_node(k,adjust_v_cnx,no_resistance);
        Delta:=Max_Meas(differ,Delta);
      end;
    end;          
                  
 begin {relax}           
     if adj[v0]<>z_node then begin
       dfs_Search:=dfs_Search+1;
       search_start:=dfs_Search;
       Set_Node_Voltage(v0,nom_meas(0.0,VDC));
       visit(v0,v0,false); 
     end;
 end;
begin {relax_wires}
     Zero_ADC:=nom_meas(0.0,ADC);
     Zero_VDC:=Nom_Meas(0.0,VDC);
     Zero_MHO:=Nom_Meas(0.0,MHO);
     iteration:=0;
     repeat
       delta:=0.0;iteration:=iteration+1;
       relax(v0);
       if debug_on then begin
         writeln(Output,' Delta,Relax_Mode:',delta,Relax_Mode);
       end;
     until (abs(delta)<epsilon) or (iteration>iterations);
     if Iteration>Max_Iterations then Max_Iterations:=Iteration;
     Total_Iterations:=Total_Iterations+Iteration;

 end;             
                      

{********************************************************************}
procedure calc_circuit(epsilon:real);
 var Circuit_Changed,Relay_Changed,Chatter:Boolean;
     zero_meas:measurement;
     Calc_Iteration:integer;
     Max_Calc_Iterations:Integer;
     Toggles,Max_Toggles:integer;

 procedure push_relays(v:integer);
   var t_node:node_Link;
   begin          
      t_node:=adj[v];                
      while t_node<>z_node do begin
        case t_node^.cnx of
          Relay_Coil:
            begin
              Enqueue(t_node^.v,RELAY_QUEUE);
            end;
        OTHERWISE
        end;        
        t_node:=t_node^.next;
      end;   
   end;                                     
                
   procedure zero_all(x:integer);
     var t_node:node_link;

     begin
         t_node:=adj[x];
         while t_node<>z_node do begin
           with t_node^ do begin
             v_cnx:=zero_meas;
             other^.v_cnx:=zero_meas;
           end;
           t_node:=t_node^.next;
         end;
     end;

 function Too_Much(c:integer):Boolean;
  var Chattering:boolean;
  begin
      Chattering:=( c > Max_Calc_Iterations );
      Too_Much:=Chattering;
      Chatter:=Chatter or Chattering;
  end;

 procedure process_queue(var Q:queue;
                         procedure process(z:integer);
                         function  escape(c:integer):boolean);
  var x,loop_count:integer;
  begin
      loop_count:=0;Max_Calc_Iterations:=2*QUEUE_COUNT(Q)+2*RELAY_COUNT+10;
      while (not EMPTY_QUEUE(Q)) and (not escape(loop_count)) do begin
        x:=FRONT(Q);DEQUEUE(Q);{ take out from front }
        process(x);loop_count:=loop_count+1;
        if debug_on then begin
          writeln(OUTPUT,' Count,lc:',QUEUE_COUNT(Q),loop_count);
        end;
      end;
      
  end;                   


 procedure Check_Relays(v:integer);
   var x:integer;
   begin
        relax_wires(v,ideal_wire,epsilon);
        relax_wires(v,real_wire,epsilon);
        bfs_Node(v,push_relays,any_path);
        while not EMPTY_QUEUE(RELAY_QUEUE) do begin
          x:=FRONT(RELAY_QUEUE);DEQUEUE(RELAY_QUEUE);
          Evaluate_Relay(x,Relay_changed);
          if Relay_Changed then Toggles:=Toggles+1;
        end;
   end;
          
 begin { calc_circuit }

    zero_meas:=Nom_Meas(0.0,vdc);
   { bfs_node(v,zero_all,any_path); }

    Calc_Iteration:=0;
    Max_Calc_Iterations:= 10 ;
    Max_Toggles:= 10 + 2*RELAY_COUNT;
    Chatter:=false;

    repeat
      Circuit_Changed:=false;Toggles:=0;
      Calc_Iteration:=Calc_Iteration+1;
      process_queue(live_QUEUE,Check_Relays,Too_Much);
    until (not Circuit_Changed) 
       or ( Calc_Iteration > Max_Calc_Iterations)
       or ( Toggles > Max_Toggles );

    Chatter:=Chatter or  ( Calc_Iteration > Max_Calc_Iterations );
    Chatter:=Chatter or  (Toggles > Max_Toggles);

    if Chatter then begin
      Err_Mess:=' Circuit did not Stabilize , Relays are Chattering';
      Error(0);
      if debug_on then begin
        end 
      else if To_Keyboard then begin
        end
      else begin
        Err_Mess:=' Aborting !!!';error(0);
        goto 9999;
      end;
    end;

 end;


{******************************************************************}
Procedure Check_Consumption(var Q:QUEUE);
  var x:integer;
      power,voltage,current:real;
      t_node:node_link;
      dev:dev_link;
      Consumption:str79;

  procedure Report_Dev(y:integer);

     begin
         
         Err_Mess:=Consumption;
         Err_P1:=inf[y]^.dev^.dev;
         Warn(1);

     end;


  procedure check(v:integer);

     begin
         t_node:=adj[v];dev:=inf[v]^.dev;
         while t_node<>z_node do begin
           with t_node^ do begin
             case cnx of
               admittance:
                  BEGIN
                    power:=v_cnx.nominal*v_cnx.nominal*y_cnx;
                    if power>y_Capacity then begin
                      Consumption:=' Power Exceeded:';
                      Report_Dev(v);
                    end;
                  END;

               Relay_Coil:
                  BEGIN
                    voltage:=v_cnx.nominal;
                  END;

               wire:
                  BEGIN
                    current:=v_cnx.nominal*y_cnx;
                    if abs(current)>Wire_Capacity then begin
                      Consumption:='Wire Current exceeded:';
                      Report_Dev(v);
                    end;
                  END;

               contact:
                  BEGIN
                    current:=v_cnx.nominal*y_cnx;
                    if abs(current)>Contact_Capacity then begin
                      Consumption:='Contact Current exceeded:';
                      Report_Dev(v);
                    end;
                  END;

               PN_Diode:
                  BEGIN
                     case t_node^.Terminal of
                       Diode_Anode:
                           BEGIN
                              if v_cnx.nominal>0.7 then begin 
                                 power:=(v_cnx.nominal-0.7);
                                 power:=power*power*0.1;
                              end;
                           END;    
                         Diode_Cathode:
                           BEGIN
                               if v_cnx.nominal<-0.7 then  begin
                                 voltage:=v_cnx.nominal;
                               end;
                           END;    
                     OTHERWISE
                         Err_Mess:='Internal error at Check_Consumption(1)!!';
                         error(0);
                     END;
                   END;

               P_Supply:
                   BEGIN
                   END;

             OTHERWISE
               Err_Mess:='Internal error at check_consumption(2)!!';
               error(0);
             end;
           end;
           t_node:=t_node^.next;
         end;
     end;

  begin
      ENQUEUE(no_node,Q);
      while FRONT(Q) <> no_node do begin
        x:=FRONT(Q);DEQUEUE(Q);{ take out from front }
        Bfs_Node(x,check,any_path);
        ENQUEUE(x,Q);{ put it back in in the back }
      end;
      DEQUEUE(Q); { get rid of no_node }         
  end;                   



{******************************************************************}
function Ohmmeter(Hi,Lo:Integer):Measurement;
label 99;
var 
    Curr_Hi,Curr_Lo:Real;
    R_Rev,R:Measurement;
                  
 begin
     if inf[Hi]^.r_node<>inf[Lo]^.r_node then begin
       Err_Mess:=' Nodes are not on the same circuit ';
       Error(0);goto 99;
     end;

     OHMS_HI:=Hi;OHMS_LO:=Lo;
     ENQUEUE(OHMS_HI,live_QUEUE);
     Calc_Circuit(Ohms_Curr*0.001);
     R_Rev:=Div_Meas(Diff_Voltage(Hi,Lo,TRUE),Nom_Meas(Ohms_Curr,ADC),Ohm);
     R:=R_Rev;
     R.HiLimit:=R_Rev.LoLimit;
     R.LoLimit:=R_Rev.HiLimit;
     OhmMeter:=R;
     OHMS_HI:=NO_NODE;OHMS_LO:=NO_NODE;
 99:
 end;

{********************************************************************}
function lowest_resistance(x:integer;var Res_value:real):integer;
var Min_Resistance:real;res:measurement;
    min_Res_Node,y:integer;

  { This procedure will find the external node that has the least   }
  { resistance path to the external world. This is the way the      }
  { algorithm works:}
  {   we start out from the node and start adding the resistances   }
  {   until we hit an external node and remember the resistance as  }
  {   the new minimum if it is the new minimum. }
  {   If anywhere along the path we see a non-isthmus path, then    }
  {   we give up the additions and ENQUEUE the external nodes we    }
  {   see after that to be tested by using ohmmeter routine later   }



  procedure visit(k:integer;to_here:real;isthmus:boolean);
  var t_node:node_link;

  BEGIN           
    inf[k]^.dfs_visit:=dfs_Search;

    if ext_node(k) then begin
      if isthmus then begin
        if to_here<Min_Resistance then begin
          Min_Resistance:=to_here;
          Min_Res_Node:=k;
        end;
        end
      else begin
        ENQUEUE(k,temp_QUEUE);
      end;
    end;

    t_node:=adj[k];

    WHILE (t_node <> z_node)  DO BEGIN
      if inf[t_node^.v]^.dfs_visit<> dfs_Search then begin 
        if (any_path(t_node^))then begin
          with t_node^ do begin
            visit(v,to_here+1.0/y_cnx,(a_p=yes) and isthmus);
          end;
        end;
      end;
      t_node:=t_node^.next;
    end;           
  end;            
                  
begin    
    Min_Resistance:=1.0E38;Min_Res_Node:=No_Node;
    dfs_Search:=SUCC(dfs_Search);
    IF in_use(x) then visit(x,0.0,true);
         
    while not EMPTY_QUEUE(temp_QUEUE) do begin
      y:=FRONT(temp_QUEUE);DEQUEUE(temp_QUEUE);
      res:=ohmmeter(x,y);
      if res.nominal<Min_Resistance then begin
        Min_Resistance:=res.nominal;Min_Res_Node:=y;
      end;
    end;
   
    Res_Value:=Min_Resistance;
    lowest_resistance:=Min_Res_Node;              
end;              




{********************************************************************}
procedure gen_PSupply_cmnd(voltage,current:real);
var aa:alfa;

begin
    case Target_Machine of
      FACT_Machine:
         BEGIN
           add_one_char('[',Test_Line);
           aa:='ISET';add_alfa(aa,Test_Line);add_blank(Test_Line);
           aa:=Float_To_Alfa(current,1);add_alfa(aa,Test_Line);
           add_one_char(';',Test_Line);

           aa:='VSET';add_alfa(aa,Test_Line);add_blank(Test_Line);
           aa:=Float_To_Alfa(voltage,1);add_alfa(aa,Test_Line);
           put_test(Test_Line);   

         END;

      Ditmco_660:
         BEGIN
           Err_Mess:='Not implemented for DITMCO_660 yet';
           error(0);
         END;
  
    OTHERWISE
      Err_Mess:=' Bad target machine in gen_psupply_cmnd';
      error(0);
    END;
    

end;

{********************************************************************}
procedure Apply_PSupply(device:alfa;
                        voltage:real;u:units;current:real;x,y:integer);
                  
label 99;
var Term1,Term2:Terminal_Types;
       Dev_ptr:Dev_link;
       connected:boolean;

begin
      connected:=false;  
      if x<>y then begin
        Dev_ptr:=Device_Pointer(Device);
        with dev_Ptr^ do begin
          If Part_Type=PSupply then begin
            connect_Contact(Hi_Pin,x,Contact_Imp,connected);
            if connected then begin
               ENQUEUE(x,live_QUEUE);
               ENQUEUE(Hi_Pin,PS_QUEUE);
               connect_Contact(Ref_Pin,y,Contact_Imp,connected);
               dev_Ptr^.PS_Voltage:=voltage;
               Calc_Circuit(voltage*0.0001);
               Check_Consumption(PS_QUEUE);
               gen_psupply_cmnd(voltage,current); 
            end;
            end
          else begin
            Err_Mess:=' Wrong device in Apply_PSupply';
            error(0);goto 99;
          end;
        end{with};
      end;            
99:;
end;

{********************************************************************}
procedure Remove_PSupply(device:alfa;x,y:integer);
                  
label 99;
var    Dev_ptr:Dev_link;

begin
      if x<>y then begin
        Dev_ptr:=Device_Pointer(Device);
        with Dev_Ptr^ do begin
          If Part_Type=PSupply then begin
            disconnect_pair(Hi_Pin,x);
            disconnect_pair(Ref_Pin,y);
            enqueue_maybe(x,live_QUEUE);
            enqueue_maybe(y,live_QUEUE);
            enqueue_maybe(Hi_Pin,live_QUEUE);
            enqueue_maybe(Ref_Pin,live_QUEUE);
            DELETE_ELEMENT(Hi_Pin,PS_QUEUE);
            Calc_Circuit(1.0*0.001);
            end
          else begin
            Err_Mess:=' Wrong device in Remove_PSupply';
            error(0);goto 99;
          end;
        end;
      end;            
99:;
end;



{******************************************************************}
function Clean_Float(xx:real;res,dir:Integer):Real;
                       
{ This function will truncate (dir=-1) or round up (dir=+1) }
{ final resolution will be res digits                       }

var x,rx:real;
    i:integer;

 begin
     x:=abs(xx);rx:=1.0;
     if x<0.001 then begin
       x:=0.0;
       end
     else if x=1.0 then begin
       x:=1.0;
       end
     else begin
       if x<1.0 then begin
         while x<1.0 do begin x:=x*10;rx:=rx/10.0; end;
         end
       else begin
         while x>=10.0 do begin x:=x/10.0;rx:=rx*10; end;
       end;

       for i:=1 to (res-1) do begin
         x:=x*10;rx:=rx/10.0;
       end;

       if dir=+1 then begin
         x:=x+0.9995;
         end
       else if dir=-1 then begin
         x:=x;
       end;

       x:=trunc(x);
       x:=x*rx;
     end;
     if xx<0.0 then x:=-x;
     Clean_Float:=x;
 end;
     


{******************************************************************}
procedure Get_Current_Ratios(v,max_count:integer);
  var min_curr:real;
  
  procedure enqueue_ratio(x,y:integer;current:real);

   var te,te1:test_element;
       go_ahead,yet:boolean;
       i:integer;
   begin

        if TEST_QUEUE_COUNT(RATIO_QUEUE)<max_count then begin
          go_ahead:=true;
          end
        else begin
          go_ahead:=false;
        end;      
    
        if go_ahead then begin
          te.x:=x;te.y:=y;te.value:=current;yet:=false;
          for i:=1 to TEST_QUEUE_COUNT(RATIO_QUEUE) do begin
            te1:=FRONT_TEST(RATIO_QUEUE);DEQUEUE_TEST(RATIO_QUEUE);
            if (te.value>te1.value) and not yet then begin
              enqueue_test(te,RATIO_QUEUE);
              yet:=true;
            end;
            ENQUEUE_TEST(te1,RATIO_QUEUE);
          end;
          if not yet then begin
            if EMPTY_TEST(RATIO_QUEUE) then begin 
              ENQUEUE_TEST(te,RATIO_QUEUE);
              end
            else begin
              te1:=FRONT_TEST(RATIO_QUEUE);
              if current>0.05*te1.value then begin
                ENQUEUE_TEST(te,RATIO_QUEUE);
              end;
            end;
          end;
        end;
   end;
 
 procedure push_ratio(k:integer);
    var current:real;

    begin
        
        t_node:=adj[k];
        while t_node<>z_node do begin
          with t_node^ do begin
            if k<v then begin
              case cnx of
                admittance:
                   BEGIN
                       current:=abs(y_cnx*v_cnx.nominal);
                       enqueue_ratio(k,v,current);
                   END;
    
              OTHERWISE
          
              end;
            end;
          end;
          t_node:=t_node^.next;
        end;

    end;

begin
    FLUSH_TEST_QUEUE(RATIO_QUEUE);
    bfs_Node(v,push_ratio,any_path);

end;

{******************************************************************}
 function r_value(r,r2_offset:real;dir:integer;four_wire:boolean;
                                   var how:cmnds):real;
    begin
        if four_wire then begin
          if dir=+1 then begin
            if r<9990.0 then begin
              r_value:=Clean_Float(r,3,dir);
              end     
            else begin
              four_wire:=false;
            end;
            end
          else {if dir=-1 then} begin
            if r<9990.0 then begin
              r_value:=Clean_Float(r,3,dir);
              end     
            else begin
              four_wire:=false;
            end;
          end;
          how:=Meas_4;
        end;

        if not four_wire then begin
          r:=r+r2_offset;
          if dir=+1 then begin
            if r<9.5 then begin
              r_value:=Clean_Float(r,1,dir);
              end     
            else if r<99.5 then begin
              r_value:=Clean_Float(r,2,dir);
              end              
            else begin
              r_value:=Clean_Float(r,3,dir);
            end;
            end
          else {if dir=-1 then} begin
            if r<10.0 then begin
              r_value:=Clean_Float(r,1,dir);
              end     
            else if r<100.0 then begin
              r_value:=Clean_Float(r,2,dir);
              end              
            else begin
              r_value:=Clean_Float(r,3,dir);
            end;
          end;
          How:=Meas_c;
        end;
      end;    


{******************************************************************}
    procedure rcheck(v:integer;var x_node:integer;four_wire:boolean);
     var c:cmnd_link;
         test_value:Measurement;
         how:cmnds;
         Test_It:Boolean;
         HiThresh,LoThresh,Max_Current:Real;
         ratio,te1:test_element;
         i:integer;

     procedure new_cmnd(var c:cmnd_link);
      begin
         if c^.next=NIL then begin
           new(c^.next);
         end;
         c:=c^.next;c^.next:=NIL;
      end;                                         
     begin              
          if ext_node(x_node) then begin
            if inf[x_node]^.x_node<>inf[v]^.x_node then begin
              if inf[x_node]^.r_node=inf[v]^.R_node then begin
                c:=cccc;
                c^.pp.PT:=NodePhrase;   
                c^.pp.n:=Int_to_name(x_node);
                c^.pp.add:=x_node;c^.pp.code:=chr(0);

                Test_It:=false;
               
                test_value:=Ohmmeter(x_node,v);
                Get_current_ratios(x_node,MAX_RESISTOR_ANNOTATE);
                if four_wire then begin
                  Hithresh:=0.100;LoThresh:=0.100;
                  end
                else begin 
                  Hithresh:=1.000;LoThresh:=1.000;
                end;
                if test_value.Lolimit>=LoThresh then begin
                  new_cmnd(c);
                  c^.pp.PT:=CommPhrase;
                  c^.pp.v:=r_value(test_value.LoLimit,r2_offset
                                              ,-1,four_wire,how);
                  c^.pp.u:=Ohm;
                  c^.pp.Comm:=How;
                  c^.pp.v2:=0.0;c^.pp.u2:=sec;
                  c^.pp.v3:=0.125;c^.pp.u3:=Watt;{ fudge at 1/8 watt }
                  c^.pp.relop:=ge;
              
                  new_cmnd(c);
                  c^.pp.PT:=NodePhrase;   
                  c^.pp.n:=Int_to_name(v);
                  c^.pp.add:=v;c^.pp.code:=chr(0);
                  Test_it:=true;
                end;

                if test_value.HiLimit>=HiThresh then begin
                  new_cmnd(c);
                  c^.pp.PT:=CommPhrase;
                  c^.pp.v:=r_value(test_value.HiLimit,r2_offset
                                              ,+1,four_wire,how);
                  c^.pp.u:=Ohm;
                  c^.pp.Comm:=How;
                  c^.pp.v2:=0.0;c^.pp.u2:=sec;
                  c^.pp.v3:=0.125;c^.pp.u3:=Watt;
                  c^.pp.relop:=le;
        
                  new_cmnd(c);
                  c^.pp.PT:=NodePhrase;   
                  c^.pp.n:=Int_to_name(v);
                  c^.pp.add:=v;c^.pp.code:=chr(0);
                  Test_it:=true;
                end;
   
                if debug_on then begin
                  with test_value do begin
                    writeln(OUTPUT,' Test_Value:',LoLimit,HiLimit);
                  end;
                end;
              
                if Test_It then begin
                  if not EMPTY_TEST(RATIO_QUEUE) then begin
                    te1:=FRONT_TEST(RATIO_QUEUE);
                    max_current:=te1.value;
                    for i:=1 to TEST_QUEUE_COUNT(RATIO_QUEUE) do begin
                      ratio:=FRONT_TEST(RATIO_QUEUE);
                      DEQUEUE_TEST(RATIO_QUEUE);
                      with ratio do begin
                        if debug_on then begin
                          writeln(output,x,y,value);
                        end;
                        if value>=0.05*Max_Current then begin
                          if annotate then begin
                            add_one_char(',',annotation);
                          end;
                          add_alfa(dev_link_to_name(inf[x]^.dev),annotation);
                          annotate:=true;
                        end;
                      end;
                    end;
                  end;
                  new_cmnd(c);
                  c^.pp.PT:=EndSentence;
      
                  TARGET_code_gen(0,cccc^,Meas_To);
                end;
              end;
            end;
          end;                    
     end;             
                  
                       
{******************************************************************}
procedure rc_tests(four_wire:boolean);
var te:test_element;
    resistance:measurement;

begin                    
    while not EMPTY_TEST(RESIS_QUEUE) do begin
      te:=FRONT_TEST(RESIS_QUEUE);DEQUEUE_TEST(RESIS_QUEUE);
      rcheck(te.x,te.y,four_wire);
    end;
end;                      

                     

{******************************************************************}
function live_circuit(x,y:integer):boolean;
 var x_forced,y_forced:Boolean;
 begin
     if PSUPPLIES=0 then begin
       live_circuit:=false;
       end
     else begin
       x_forced:=(inf[inf[x]^.i_node]^.psbit=yes); 
       y_forced:=(inf[inf[y]^.i_node]^.psbit=yes);
       live_circuit:=x_forced and y_forced;
     end;
 end;

{******************************************************************}
procedure xc_test(p_1:params;v0:integer;
                  incremental:boolean;four_wire:boolean;
                  mode:test_mode);

{ this procedure will generate continuity tests from v to all higher }
{   addressed nodes. Also it will push into resistor QUEUE all the }
{   resistors that should be checked for possible resistance tests.}

var  search_start:integer;
     x_node:integer;
     nl:integer;                        
     dummy:boolean;
                            
    procedure rcpush(v:integer;var x_node:integer);
     var te:test_element;                         
     begin
         te.x:=v;
         te.y:=x_node;
         ENQUEUE_TEST(te,RESIS_QUEUE);
     end;             
                  
                
                     
    procedure xcheck(p_1:params;v:integer;var x_node:integer);
     begin              
          if ext_node(x_node) then begin
            if x_node<>v then begin
              add_node(1,Cont_From,x_node);
              x_node:=-1;
              if First_Cont then begin
                add_comm(6,P_1);
                if OK then begin
                  if not four_wire then begin
                    C_Params:=P_1;
                  end;
                end;
                First_Cont:=false;
              end;
            end;
          end;                    
          add_node(6,Cont_To,v);
     end;             
                  
    function crit(t:node_link):boolean;

    begin
        case mode of
          SHORT_SHORTS:begin
                          crit:=(t^.a_w=YES);
                       end;
          RESISTORS   :begin
                          crit:=(t^.a_p=YES);
                       end;
        end;
    end;

    procedure tag_cr(t:node_link;tag:bit);

    begin
        case mode of
          SHORT_SHORTS:begin
                          t^.x_c:=tag;
                       end;
          RESISTORS   :begin
                          t^.r_c:=tag;
                       end;
        end;
    end;

    function visit(k,k0,r_node:integer;var ripple:boolean;t_back:node_link):boolean;
    var   t_node:node_link;
          tested,force:Boolean;

     function good_r_check(x,y:integer):boolean;
      var good:boolean;
      begin
         good:=(not short_graph(x,y)) and ext_c_node(y,four_wire);
         good_r_check:=good and (not live_circuit(x,y));
      end;


     function rcx_node(x:integer;var y:integer;four_wire:boolean):boolean;
    { This function will return in y the first node that can be used for}
    { resistance checks. ( first node that  breadth-first search sees.}
      var found:boolean;
      procedure get_z_value(z:integer);
       begin
          if not found then begin
            if ext_c_node(z,four_wire) then begin
             y:=z;found:=true;
             end;
          end;
       end;

       function shorty(t:node):boolean;
        begin
           if found  then begin
             shorty:=false;{end of search}
             end
           else begin
             shorty:=short_path(t);
           end;
        end;


      begin
          found:=false;
          bfs_Node(x,get_z_value,shorty);
          rcx_node:=found;
      end;


     function rn:integer;
      var value:integer;
      begin
         if rcx_node(k,value,four_wire) then begin
           rn:=value;
           end
         else if rcx_node(k0,value,four_wire) then begin
           rn:=value;
           end
         else begin
           rn:=r_node;
         end;
      end;
                           
    BEGIN { visit }
      inf[k]^.dfs_visit:=dfs_Search;
      inf[k]^.bfs_SAW:=bfs_Search;
      inf[k]^.Cnx_SAW:=BAD_cnx;
        
      tested:=false;            

      { consider this node only if we can generate tests from it and to it}  
      if (ext_c_node(k,four_wire)) and (k<>k0) then begin
        if (not incremental) or ripple then begin 
          case mode of
            SHORT_SHORTS:begin
                            if not live_circuit(k,x_node) then begin
                              xcheck(p_1,k,x_node);
                              tested:=true;ripple:=false;
                            end;
                         end;
            RESISTORS   :begin 
                            if not short_graph(k,x_node) then begin
                              if good_r_check(k,r_node) then begin
                                rcpush(k,r_node);
                                tested:=true;ripple:=false;
                                end
                              else if good_r_check(k,x_node) then begin
                                rcpush(k,x_node);
                                tested:=true;ripple:=false;
                              end;
                            end;
                         end;
          end;

        end;
      end;

      t_node:=adj[k];    
      WHILE (t_node <> z_node) DO BEGIN
        if xrc_path(t_node^,mode) then begin { traverse this wire }
          if inf[t_node^.v]^.dfs_visit<search_start then begin
            if not live_circuit(k,t_node^.v) then begin
              if (t_node^.require=X_Check) then begin
                force:=true;{ make sure tests ripple to later nodes}
                end
              else if (t_node^.x_c=NO) and (crit(t_node))then begin
                force:=true;{ make sure tests ripple to later nodes}
                end
              else begin
                force:=ripple;
              end;
            end;
            if visit(t_node^.v,k,rn,force,t_node^.other) then begin
              tested:=true;force:=false;      
              if crit(t_node) then tag_cr(t_node,YES);
              t_node^.require:=NO_Check;
            end;
          end;   
        end;
        t_node:=t_node^.next;
      end;

      if t_back<>NIL then begin
        if tested then begin
          if crit(t_back) then tag_cr(t_back,YES);
          t_back^.Require:=NO_Check;
        end;         
      end;
      visit:=tested;
    end;          
                  
 begin {xc_test}           
     if adj[v0]<>z_node then begin

       dfs_Search:=dfs_Search+1;
       search_start:=dfs_Search;

       nl:=tag_critical(v0,mode);

       x_node:=v0;
       dfs_Search:=dfs_Search+1;
       search_start:=dfs_Search;      
       dummy:=false;
       dummy:=visit(v0,v0,v0,dummy,NIL);
       if mode=RESISTORS then begin
         rc_tests(four_wire);
       end; 
     end;
 end;
          
{******************************************************************}
function Big_node(x0:integer;four_wire:boolean):integer;
var Big,Count,Maxim,Ext:integer;

 { This routine gets the node that has the most connections coming }
 { into it. This allows resistor tests to be done from a common }
 { node as much as possible.}

 procedure init(x:integer);
  begin
      count:=0;Ext:=NO_NODE;
  end;
      
 procedure get_max(x:integer);
  var t_node:node_link;
  begin
      t_node:=adj[x];
      if ext_c_node(x,four_wire)then begin
        Ext:=x;
      end;
      while t_node<>z_node do begin
        count:=count+1;
        if count>maxim then begin
          if ext<>NO_NODE then begin
            Big:=ext;maxim:=count;
          end;
        end;
        t_node:=t_node^.next;
      end;
  end;                 


begin
    Big:=x0;count:=0;maxim:=0;
    bfs_double(x0,init,get_max,short_path,any_path);
    if ext_c_node(inf[Big]^.x_node,four_wire) then begin
      Big_Node:=inf[Big]^.x_node;
      end
    else begin
      Big_Node:=Big;
    end;
end;


{******************************************************************}
procedure xc_tests(p_1:params;incremental,four_wire:boolean;mode:test_mode);

 { This Routine is the one that does all the continuity and
 { resistance tests. }


 procedure test_thru_queue(var Q:queue);
  var v:integer;
  begin
      enqueue(NO_NODE,Q);{ put a sentinel }
      x:=FRONT(Q);DEQUEUE(Q);{ take out from front }
      while x<>NO_NODE do begin
        ENQUEUE(x,Q);{ put it back in the rear }
        if four_wire then begin
          v:=f_node(x,allow,four_wire);
          end
        else begin
          v:=inf[x]^.x_node;
        end;
        if ext_c_node(v,four_wire) then begin
          xc_test(p_1,v,incremental,four_wire,mode);
          inf[v]^.x_c:=checked;
        end;  
        x:=FRONT(Q);DEQUEUE(Q);{ take out from front }
      end;
  end;                   

 procedure test_all;
  var i,Big,v:integer;
      allow  :omit_flag;
             
  begin {test_all}
      FOR i:= max(0,V_min) TO V_Max DO BEGIN
        if in_use(i) then begin   
          case mode of
             SHORT_SHORTS:v:=f_node(i,allow,four_wire);
             RESISTORS   :v:=l_node(i,allow,four_wire);
          end;

          if ext_c_node(v,four_wire) then begin
            if  v=i then begin
              case mode of
                 SHORT_SHORTS:Big:=v;
                 RESISTORS   :Big:=Big_Node(v,four_wire);
              end;
              xc_test(p_1,Big,(XC_Tested and incremental),four_wire,mode);
              inf[v]^.x_c:=checked;
            end;              
            end
          else begin
            inf[i]^.bfs_saw:=bfs_Search;
          end;
        end;
      END;
  end;

begin {xc_tests}
    { First_XC is used to determine to see if an 'xc' command }
    { is already executed. Note that an 'xc@4' does not exhaust}
    { all posibilities therefore Force_XC remains true. }
    First_Cont:=True; { make sure C command is emitted}
    if Force_XC or (not incremental) then begin
      test_all;
      XC_Tested:=true;
      if Switch_is(['4']) then begin 
        Force_XC:=true;{ 4-wire does not exhaust possibilities }
        end
      else begin
        Force_XC:=false;
      end;
      end
    else begin
      Test_thru_queue(x_QUEUE);
      Test_thru_queue(y_QUEUE);
    end;
end;                      

  
{******************************************************************}
procedure test_ENQUEUE(te:test_element);

begin
    if inf[te.x]^.r_node=inf[te.y]^.r_node then begin
      ENQUEUE_TEST(te,RESIS_QUEUE);
      end
    else begin
      ENQUEUE_TEST(te,OPENS_QUEUE);
    end;
end;

{******************************************************************}
procedure open_test(v0:integer;
                    O_Params:Params;Incremental:boolean;Mode:Test_Mode);
{ this procedure will generate open tests from v0 to all open nodes}
  
var  dfs_start,bfs_Start:integer;
     dummy_dev:alfa;
     dummy:boolean;
     dummy_rn:integer;


    function visit(k,x_node:integer;
                      spans:integer;
                       yacc:real;
                      dev_name:alfa):integer;
               
    var   t_node:node_link;
          tested,may_test:boolean;
          span:integer;
          bfs_Tag:Integer;
          te:test_element;
          y_span:real;
          rn:integer;                           
          dn:alfa;

      function spanable(            t:node;
                          spans:integer;                        
                   var     span:integer;    
                   var   y_span:real;
                   var   dev_name:alfa):boolean;   
           
        var span_it:boolean;

        function open_span(mode:test_mode):boolean;
         begin 
             case mode of
                SHORT_OPENS:open_span:=true;
                RESIS_OPENS:open_span:=true;
                RESISTORS  :open_span:=false;
             end;
         end;

        BEGIN

           { Algorithm: 
             If this node (k) and the other node (t.v) have the same 
             r_node then they are on the same resistive network ,
             therefore t.v should be visited. If 
                              }
          
          
           if inf[t.v]^.x_node=inf[k]^.x_node then begin {connected}
             span_it:=true;{visit connected nodes}
             y_span:=t.y_cnx;
             span:=0;
             end
           else if inf[t.v]^.r_node=inf[k]^.r_node then begin
             case mode of
                SHORT_OPENS:span_it:=false;
                RESIS_OPENS:span_it:=true;
                RESISTORS  :span_it:=true;
             end;
             y_span:=t.y_cnx;
             span:=0;
             end
           else if t.cnx=contact then begin{ Not connected }
             y_span:=0.0;
             with t do begin
               if (j_d=YES) or (o_c=NO) or (not incremental) then begin
                 { just disconnected contact }  
                 if spans=0 then {remember name of device spanned} begin
                   dev_name:=inf[t.v]^.dev^.dev;
                   span_it:=open_span(mode);
                   end
                 else if inf[t.v]^.dev^.dev=dev_name then begin
                   { we cross multiple opens if they are on the same device}
                   span_it:=open_span(mode);
                   end                   
                 else begin
                   span_it:=(open_span(mode) and (o_c=NO));
                 end;
                 end 
               else begin
                 span_it:=false;
               end;                                
             end{with};
             span:=1;
             end   
           else begin
             span_it:=false;
           end;
           spanable:=span_it;
        end;
                         
    procedure add_pos(bl:integer;n:node_types;v:integer);
    begin           
        if v<0 then begin
          v:=inf[v]^.x_node;
          if v<0 then begin
            err_mess:=' Internal error. Attempt to gen a test to (-) node';
            error(0);goto 9999;     
          end;
        end;
        add_node(bl,n,v);
    end;

                   
    procedure tag_node_link(t_node:node_link;span:integer);
     { This procedure will tag an arc as tested if it is.     } 
     { Decision that if it has been tested is made by calling }
     { routine. If span is 1 then the cut has been tested;    }
     { otherwise just disconnected flags are cleared (j_d)    }
     { so that a just_in_case test will not be generated again}

     begin
         if span>0 then begin
           with t_node^ do begin                                      
             case cnx of
               contact:
                 begin
                   if span=1 then begin
                     o_c:=YES;other^.o_c:=YES;
                     require:=NO_Check;other^.require:=NO_Check;
                   end;
                   j_d:=NO;Other^.j_d:=NO;
                 end;
             OTHERWISE
             end;
           end;
         end;
     end;

     procedure tag_tested(v0:integer);
     var tagged:boolean;
     { This procedure is the process(x) procedure for tagging  }
     { Cut sets. It is invoked by the bfs_node routine and tags}
     { the cut that surrounds this subgraph.                   }
                                     
     {WARNING: the order of te.x and te.y is very important.   }
     { SEE test_for routine and the commented out line         }
     {  in the following for more info.                        }

      function tag_it(x,y:integer):boolean;
       var t_node:node_link;
     
      begin
         tag_it:=false;
         t_node:=adj[v0];
         while t_node<>z_node do begin
           if inf[k]^.x_node=inf[y]^.x_node then begin
             if inf[t_node^.v]^.x_node=inf[x]^.x_node then begin
               tag_node_link(t_node,te.span);tag_it:=true;
             end;
             end   
           else if inf[k]^.x_node=inf[x]^.x_node then begin
             if inf[t_node^.v]^.x_node=inf[y]^.x_node then begin
               tag_node_link(t_node,te.span);tag_it:=true; 
             end;
           end;
           t_node:=t_node^.next;
         end;
      end;

     begin
         if not tag_it(te.x,te.y) then begin
           tagged:=tag_it(te.y,te.x);
           end
         else begin
           tagged:=false;
         end;
     end;


     function test_for(t:node_link;var te:test_element):Test_Reqs;

     { WARNING: In the following te.x must be the SEEN node and te.y }
     {          must be =k or this node. Otherwise tag_tested routine}
     {          will not function correctly. }

     var Tests:Test_Reqs;

       {}
       procedure add(test:test_req);begin Tests:=Tests+[test]; end;
       {}   
         


     begin {test_for}
         Tests:=[];
         if (inf[k]^.r_node=inf[t^.v]^.r_node)then begin
           if ext_node(inf[k]^.x_node) then begin
             if (t^.r_c=NO)  then begin
               add(R_Check);                            
               te.x:=t^.v;te.y:=k;te.span:=0;
               end
             else if (t^.r_cc=NO) then begin          
               add(R_Check);
               te.x:=t^.v;te.y:=k;te.span:=0;
               end
             else if (not incremental) then begin
               add(R_Check);
               te.x:=t^.v;te.y:=k;te.span:=0;
               end
             else begin
             {}
             end;
           end;
           end
         else if ext_node(inf[t^.v]^.x_node) then begin
           if ext_node(inf[k]^.x_node) then begin
             if (t^.o_c=NO)  then begin
               add(O_Check);
               te.x:=t^.v;te.y:=k;te.span:=1;
               end
             else if (t^.j_d=YES) then begin          
               add(O_Check);
               te.x:=t^.v;te.y:=k;te.span:=1;
               end
             else if (not incremental) then begin
               add(O_Check);
               te.x:=t^.v;te.y:=k;te.span:=1;
               end
             else begin
             {}
             end;
             end
           else if (inf[x_node]^.r_node=inf[t^.v]^.r_node)then begin
             if ext_node(inf[x_node]^.x_node) then begin
               if (t^.r_c=NO)  then begin
                 add(R_Check);
                 te.x:=x_node;te.y:=t^.v;te.span:=0;
                 end
               else if (t^.r_cc=NO) then begin          
                 add(R_Check);
                 te.x:=x_node;te.y:=t^.v;te.span:=0;
                 end
               else if (not incremental) then begin
                 add(R_Check);
                 te.x:=x_node;te.y:=t^.v;te.span:=0;
                 end
               else begin
               {}
               end;
             end;
             end
           else if (t^.o_c=NO) and ext_node(x_node) then begin 
             if (inf[x_node]^.x_node<inf[t^.v]^.x_node) then begin
               add(O_Check);
               te.x:=x_node;te.y:=t^.v;te.span:=span+spans;
               end
             else begin
               {}
             end;
             end           
           else begin
             {}
           end;               
           end 
         else begin
           {}
         end;
         Test_for:=Tests;
     end;
                 
     procedure prelim_tags(x:integer);
      var t_node:node_link;t_x:integer;
      BEGIN
          t_node:=adj[x];                     
          while t_node<>z_node do begin
            tested:=false;
            t_x:=inf[t_node^.v]^.x_node;{remember target x_node}
            { Algorithm:
              it k and t_node^.v are connected then no open/resistance checks
              else up to this point every node we visited thru dfs , we
              performed a bfs on all the connected nodes. Therefore if
              inf[t_node^.v]^.bfs_visit<bfs_Start then we did not see that
              node yet and we shall not generate a test for it (yet).
              Otherwise we have already seen t_node^.v and it is not connected
              to this node( not directly anyway) so we check to see if a
              test should be generated for it.      }


            if inf[t_node^.v]^.x_node=inf[k]^.x_node then begin{connected}
              end
            else if inf[t_node^.v]^.bfs_visit<bfs_Start then begin{not seen yet}
              end
            else begin { Node is a member of previously visited subgraph }
              if ext_node(inf[t_node^.v]^.x_node) then begin { path to outside }
                if inf[t_x]^.bfs_Saw=bfs_Search then begin{ another path}
                  {we have already seen t_x from this node (see next clause)}
                  end                                   
                else begin { see if should be tested }       
                  inf[t_x]^.cnx_SAW:=t_node^.cnx;
                  inf[t_x]^.bfs_SAW:=bfs_Search;{ no multiple tests}
                  case t_node^.cnx of                 
                    contact:{must be open (already tested for connectivity)}
                      BEGIN
                        if t_node^.contact_state=opened then begin {reduntant}
                          if O_Check in test_for(t_node,te) then begin    
                            ENQUEUE_TEST(te,OPENS_QUEUE);{ possibly testable}
                            tested:=true;
                          end;
                        end;
                      END;
                    admittance,Relay_Coil:
                      BEGIN
                        if R_Check in test_for(t_node,te) then begin
                          ENQUEUE_TEST(te,RESIS_QUEUE);
                        end;
                      END; 
                  OTHERWISE { Not testable }
                  END;
                end;                      
                end
              else begin 
                { no path to outside }
              end;
            end;
            t_node:=t_node^.next;
          end;
      END;
                                            
     procedure add_test(te:test_element);
     var x1,y1:integer;
      procedure fix_xy(x,y:integer;var x1,y1:integer);
       { This procedure ensures that TO node is never CHASSIS=0}
       begin
           if y=0 then begin
             if ext_pos(inf[y]^.x_node) then begin
               x1:=x;y1:=inf[y]^.x_node;
               end
             else begin
               x1:=y;y1:=x;
             end;
             end
           else if (y<0) and (inf[y]^.x_node=0) then begin
             x1:=y;y1:=x;
             end
           else begin
             x1:=x;y1:=y;
           end;
       end;
           
     begin {add_test}                      
         with te do begin
           fix_xy(x,y,x1,y1);
           if from_node=NO_NODE then begin
             From_Node:=x1;
             Add_Pos(1,Open_From,x1);
             end
           else begin
             if inf[y]^.x_node=inf[From_Node]^.x_node then begin
               fix_xy(x,y,y1,x1);{ swap them if possible }
             end;
             if inf[x1]^.x_node<>inf[From_Node]^.x_node then begin
               add_pos(1,Open_From,x1);
               From_Node:=x1;
             end;
           end;

           if First_Open then begin
             Add_Comm(6,O_Params);
             First_Open:=false;
           end;
                                             
           add_pos(6,Open_To,y1);
         end;
     end;
     
    function y_y(y_1,y_2:real):real;
      BEGIN
         if (y_1=0.0) or (y_2=0.0) then begin
           y_y:=0.0;
           end
         else begin
           y_y:=(y_1+y_2)/(y_1*y_2);
         end;
      END;

  procedure add_tests;
    begin
        if not EMPTY_TEST(OPENS_QUEUE) then begin
          bfs_Tag:=bfs_Search;
          while not EMPTY_TEST(OPENS_QUEUE) do begin
            te:=FRONT_TEST(OPENS_QUEUE);DEQUEUE_TEST(OPENS_QUEUE);
            Add_Test(te);               
            bfs_Node(k,tag_tested,short_path);
          end;
        end;
    end;
           
    BEGIN {visit}                                
      tested:=false;
      dfs_Search:=dfs_Search+1;inf[k]^.dfs_visit:=dfs_Search;
      t_node:=adj[k];
      if inf[k]^.bfs_visit<=bfs_Start then begin
        bfs_Node(k,prelim_tags,short_path);
        add_tests;
      end;

      WHILE (t_node <> z_node) DO BEGIN
        rn:=NO_NODE;
        with t_node^ do begin
          if inf[v]^.dfs_visit<dfs_start then begin 
            dn:=dev_name;
            if spanable(t_node^,spans,span,y_span,dn)then begin
             if span=-1 then begin {resistive path}
               span:=1; 
               if ext_node(inf[t_node^.v]^.x_node) then begin
                 rn:=visit(v,t_node^.v,spans+span,y_y(yacc,y_span),dn);
                 end
               else begin
                 rn:=visit(v,x_node,spans,y_y(yacc,y_span),dn);
               end;
               end
             else if span=0 then begin { just a connection }
               rn:=visit(v,x_node,spans,y_y(yacc,y_span),dn);
               end
             else if ext_node(k) then begin { pass this node as x_node}
               rn:=visit(v,k,span,y_y(yacc,y_span),dn);
               end
             else if ext_node(inf[k]^.x_node) then begin { find an x_node }
               rn:=visit(v,inf[k]^.x_node,span,y_y(yacc,y_span),dn);
               end
             else begin {use original x_node}
                                                                    
               { Now what we do here is this:                          }
               { The reason we are visiting the next node is , we      }
               { came from an external node thru jumping over a contact}
               { and we cannot generate a test at this node ( because  }
               { if we could then we would not be in this else clause) }
               { So if an external node is passed to us, that means    }
               { that succeeding nodes could not generate a test either}
               { so we generate a test here if we can, otherwise we    }
               { pass back a NO_NODE so that previous nodes will not   }
               { generate a test either.                               }
               { may_test is used to see if connected subgraph we are  }
               {  about to visit has already been visited (thru another}
               {  arc).                                                }

               may_test:=(inf[v]^.bfs_visit<=bfs_cut);
               may_test:=true;{ for now }
               rn:=visit(v,x_node,spans+span,y_y(yacc,y_span),dn);

               if (rn<>NO_NODE) then begin
                 te.x:=x_node;te.y:=rn;te.span:=spans+span;
                 if may_test {true} then begin        
                   if ext_node(rn) then begin
                     if inf[x_node]^.x_node<>inf[rn]^.x_node then begin
                       if debug_on then begin
                         writeln
                           (output,' gen test:',te.x,te.y,te.span,line_no);
                       end;
                       TEST_ENQUEUE(te);
                       add_tests;{tested:=true;}
                       end
                     else if debug_on then begin
                       writeln
                         (output,' skipped=:',te.x,te.y,te.span,line_no);
                     end;
                   end;
                   end                             
                 else if debug_on then begin
                   writeln
                     (output,' may_test~:',te.x,te.y,te.span,line_no);
                 end;
                 end
               else if debug_on then begin
                 writeln(output,' NO_NODE:',x_node,v,spans+span,line_no);
               end;
             end;
            end;
          end ;
        end{with t_node^};
        t_node:=t_node^.next;
      end;                                 
      if tested then begin
        visit:=NO_NODE;
        end
      else begin
        if ext_node(k) then begin
          visit:=k;
          end
        else if ext_node(inf[k]^.x_node) then begin
          visit:=inf[k]^.x_node;
          end
        else begin
          visit:=NO_NODE;
        end;
      end;
    end;          
                  
 begin            
     if adj[v0]<>z_node then begin
                                     
       dfs_Search:=dfs_Search+1;
       dfs_start:=dfs_Search;
       bfs_Search:=bfs_Search+1;                       
       bfs_Start:=bfs_Search;
       dummy_rn:=visit(v0,v0,0,0.0,dummy_dev); 
     end;
 end;             

{******************************************************************}
procedure open_tests(O_Params:params;Incremental:Boolean;Mode:Test_Mode);
 var i,j,x,y      :Integer;

  procedure test_all;
   var i:integer;
   begin  
       FOR i:= max(0,v_min) TO V_Max DO BEGIN
         if in_use(i) then begin   
           x:=F_Node(i,allow,false);                                   
           if ext_c_node(x,false) then begin
             if  x=i then begin
               if inf[x]^.bfs_Visit<=bfs_cut then begin
                 open_test(x,O_params,Incremental,Mode);
               end;
             end;              
           end;
         end;
       END;
   end;
          

 procedure test_thru_queue(var Q:QUEUE);
  var x:integer;
  begin
     ENQUEUE(NO_NODE,Q);{ put a sentinel }
     x:=FRONT(Q);DEQUEUE(Q);{ take out from front }
     while x<>NO_NODE do begin
       ENQUEUE(x,Q);{ put it back in the rear }
       x:=First_Positive(x,0);
       if inf[inf[x]^.x_node]^.bfs_visit<=bfs_cut then begin
         if ext_node(x) then begin
           open_test(x,O_params,Incremental,Mode);
         end;
       end;
       x:=FRONT(Q);DEQUEUE(Q);
     end;
  end;

 begin
     First_Open:=true;
     From_Node:=NO_NODE;
     bfs_Cut:=bfs_Search;

     if (not incremental) then begin
       test_all;
       end
     else begin
       Test_thru_queue(x_QUEUE);
       Test_thru_queue(y_QUEUE);
     end;
   end;          


{******************************************************************}
 function get_ext(x:integer):integer;
 begin
     if x<0 then Get_Ext:=First_Positive(x,0) else Get_Ext:=x;
     if Not Ext_Node(x) then begin 
       err_mess:='  Node is not an external node.';
       error(0);
     end;
 end;
          
                  
{********************************************************************}
 Procedure Echo(Line:Str80);
  BEGIN           
   If Debug_On Then BEGIN
    writeln(OUTPUT,Line_No:3,' ',Line);
  END             
 END;             
                  
                  
{**************************************************************}
function get_nodes(var x,y:integer):boolean;
                  
  var v1,v2:name; 
                  
begin             
       get_node(NULL,v1);x:=Address_of(v1);
       if good(x) then begin
         get_node(NULL,v2);y:=Address_of(v2);
         if good(y) then begin
           get_nodes:=true;
           end    
         else begin
           error(err_Ps);
           get_nodes:=false;
         end;     
         end      
       else begin 
         error(err_Ps);
         get_nodes:=false;
       end;       
end;              

{**********************************************************************}
function p_punch(b:alfa):integer;
 begin
      p_punch:=-1;
      if alfa_length(b)=4 then begin
        if b[1]='P' then begin
          if b[2] in ['0'..'9'] then begin
            if b[3] in ['0'..'9'] then begin
              if b[4] in ['0'..'9'] then begin
                p_punch:=Alfa_to_Number(b,2,4);
              end;
            end;
          end;   
        end;
      end;
 end;
                  
{***************************************************************}
function Good_Cmnd(f:alfa; var ccmnd:Cmnds):Boolean;
var aa:alfa;      
     p:integer;             
begin             
    Good_Cmnd:=True;
    aa:=UpperAlfa(f);
    if      (aa='C')  or (aa='CONT')  then  ccmnd:=Cont_C
    else if (aa='O')  or (aa='OPEN')  then  ccmnd:=Open_C
    else if (aa='FC') or (aa='LDC')   then  ccmnd:=Insu_DC
    else if (aa='SC') or (aa='LAC')   then  ccmnd:=Insu_AC
    else if (aa='I')  or (aa='INS')   then  ccmnd:=Insu_C
    else if (aa='M')  or (aa='MEAS')  then  ccmnd:=Meas_C
    else if (aa='M4') or (aa='M4')    then  ccmnd:=Meas_4
    else if (aa='C4') or (aa='C4')    then  ccmnd:=Cont_4
    else begin    
      P:=P_Punch(aa);                              
      if P in [96..99] then begin
        Power_Relay:=P;
        ccmnd:=Power_C; 
        end
      else begin
        Good_Cmnd:=false;
      end;
    end;          
end;              
                  
{****************************************************************}
procedure parse_cc(cc:cmnds;var p_p:params;var unused:alfa);
                  
 begin  {parse_cc}
                  
   p_p.PT:=CommPhrase;
                  
   case cc of     
                  
     Cont_C: BEGIN
                p_p:=c_params;p_p.comm:=cc;
                getv;z1:=z;z2.a:=NULL;
                with p_p do begin
                  get_parm(false,z1,z2,[Ohm,KOhm,MOhm] ,v ,u );
                  get_parm(false,z1,z2,[mADC,ADC]      ,v1,u1);
                  get_parm(false,z1,z2,[mSec,Sec]      ,v2,u2);
                  unused:=z1.a;
                end;
                if OK then c_params:=p_p;
              END;
          
     Cont_4: BEGIN
                p_p:=c_params;p_p.comm:=cc;p_p.v1:=2.0;
                getv;z1:=z;z2.a:=NULL;
                with p_p do begin
                  get_parm(True ,z1,z2,[Ohm,KOhm,MOhm] ,v ,u );
                  get_parm(False,z1,z2,[mADC,ADC]      ,v1,u1);
                  get_parm(false,z1,z2,[mSec,Sec]      ,v2,u2);
                  unused:=z1.a;
                end;
              END;
                  
     Open_C : BEGIN
                p_p:=O_Params;
                getv;z1:=z;z2.a:=NULL;
                with p_p do begin
                  get_parm(false,z1,z2,[Ohm,KOhm,MOhm] ,v ,u );
                  get_parm(false,z1,z2,[mADC,ADC]      ,v1,u1);
                  get_parm(false,z1,z2,[mSec,Sec]      ,v2,u2);
                  unused:=z1.a;
                end;
                if OK then O_Params:=p_p;
              END;
                  
     Insu_DC: BEGIN
                getv;z1:=z;z2.a:=NULL;
                get_parm(false,z1,z2,[VDC..KVDC] ,p_p.v,p_p.u );
                get_parm(false,z1,z2,[KOhm,MOhm]  ,p_p.v1,p_p.u1);
                get_parm(false,z1,z2,[mSec..Sec] ,p_p.v3,p_p.u3);
                unused:=z1.a;
              END;
                  
     Insu_AC: BEGIN
                getv;z1:=z;z2.a:=NULL;
                get_parm(false,z1,z2,[VAC..KVAC],p_p.v,p_p.u );
                if upperalfa(z1.a)='RAMP' then begin
                  p_p.u1:=Ramp;
                  getv;z1:=z;
                end;
                get_parm(false,z1,z2,[mAac..Aac,MADC..ADC],p_p.v2,p_p.u2);
                if p_p.u2=mADC then p_p.u2:=mAac;
                if p_p.u2= ADC then p_p.u2:= Aac;
                get_parm(false,z1,z2,[mSec..Sec]        ,p_p.v3,p_p.u3);
                unused:=z1.a;
              END;
                  
     Meas_C : BEGIN
                p_p.comm:=Meas_C;
                p_p.v:=0.0;   p_p.u:=BadUnit;
                p_p.v1:=5.0;  p_p.u1:=Pct;
                p_p.v2:=0.0;  p_p.u2:=Sec;  
                getv;
                if      (z.a='>') or (upperalfa(z.a)='GE') then begin
                  p_p.relop:=ge;
                  getv;
                  end
                else if (z.a='<') or (Upperalfa(z.a)='LE') then begin
                  p_p.relop:=le;
                  getv;
                  end
                else begin
                  p_p.relop:=eq;
                end;
                  
                z1:=z;z2.a:=NULL;
                  
                get_parm(true,z1,z2,[Ohm..AAC],p_p.v,p_p.u);
                  
                if p_p.relop=eq then  begin
                  get_parm(false,z1,z2,[PCT],p_p.v1,p_p.u1);
                end;

                get_parm(false,z1,z2,[mSec..Sec],p_p.v2,p_p.u2);
                  
                unused:=z1.a;
              END;

     Meas_4 : BEGIN
                p_p.comm:=Meas_4;
                p_p.v:=0.0;   p_p.u:=BadUnit;
                p_p.v1:=5.0;  p_p.u1:=Pct;
                p_p.v2:=0.0;  p_p.u2:=Sec;  
                getv;
                if      (z.a='>') or (upperalfa(z.a)='GE') then begin
                  p_p.relop:=ge;
                  getv;
                  end
                else if (z.a='<') or (Upperalfa(z.a)='LE') then begin
                  p_p.relop:=le;
                  getv;
                  end
                else begin
                  p_p.relop:=eq;
                end;
                  
                z1:=z;z2.a:=NULL;
                  
                get_parm(true,z1,z2,[Ohm..MOhm],p_p.v,p_p.u);
                  
                if p_p.relop=eq then  begin
                  get_parm(true,z1,z2,[PCT],p_p.v1,p_p.u1);
                end;

                get_parm(false,z1,z2,[mSec..Sec],p_p.v2,p_p.u2);
     
                if (upperalfa(z1.a)='AT')then begin
                  getv;z1:=z;
                  get_parm(true,z1,z2,[mADC..ADC],p_p.v3,p_p.u3);
                  end
                else if(upperalfa(z1.a)='MAX')then begin
                  getv;z1:=z;
                  get_parm(true,z1,z2,[Watt],p_p.v3,p_p.u3);
                  end
                else begin
                  Err_Mess:='Current/Power parameter is required for M4';
                  error(0);
                end;
                unused:=z1.a;
              END;                          

     Power_C: BEGIN
                p_p.comm:=Power_C;
                p_p.v :=Power_Relay;   p_p.u :=BadUnit;
                p_p.v1:=0.0        ;   p_p.u1:=BadUnit;
                unused:=NULL;{ no field is used }    
              END             
  end;            
                  
end;              
                  
{****************************************************************}
                  
procedure get_field(         prev_phrase:Phrase_desc;
                             f          :alfa;
                     var     c          :cmnd_link);
          
 label 99;                                                      
 var f1,unused:alfa;  
     t:cmnd_link;         
     cc:cmnds;    
                  
 begin            
                  
      if f=NULL then begin
        getsym;   
        f1:=a;    
        end       
      else begin  
        f1:=f;    
      end;        
                  
      c^.pp.PT:=EndSentence;{Sentinel to force no code generation past errors}
                                  
      if f1=' ' then begin { end of line }
        if Prev_Phrase.PD<>CommPhrase Then begin
          c^.pp.PT:=EndSentence;
          end
        else begin
          Err_Mess:=' Line may not end with a command:';
          error(0);goto 99;
        end;
        end       
      else if f1='-' then begin { field begins with dash }
        err_mess:=' - is not allowed here:';
        err_p1:='-';
        error(1); 
        end              
      else begin  
        get_node(f1,v1);
        x:=address_of(v1);
                                          
        if good(x) then begin { a good node name }
          c^.pp.PT:=nodephrase;
          c^.pp.n:=v1;
          c^.pp.add:=x;c^.pp.code:=chr(0);{null character for commands}
                  
          if c^.next=NIL then begin
            new(t);t^.next:=NIL;
            c^.next:=t;
          end;
          if Prev_Phrase.PD=CommPhrase then begin
            if Prev_Phrase.Comm=Meas_4 then begin
              Case Target_Machine OF
                FACT_machine:
                  begin
                    if mach_add(x) mod 2 <>0 then begin
                      Err_Mess:='TO address must be even';
                      error(0);goto 99;
                    end;
                  end;
                OTHERWISE
                  begin
                    Err_mess:=' 4-wire Measurement Illegal';
                    error(0);goto 99;
                  end;
              end {CASE};
            end;
          end;
          Prev_Phrase.PD:=NodePhrase;Prev_Phrase.Address:=x;        
          get_field(Prev_Phrase,NULL,c^.next);
          end     
        else begin { maybe it is a command }
          if Good_cmnd(f1,cc) then begin

            if Prev_Phrase.PD=NoPhrase then begin
              err_mess:=' No node name before command';
              error(0);goto 99;
              end
            else if prev_phrase.PD=CommPhrase then begin
              err_mess:=' Two commands in sequence are illegal:';
              err_p1:=f1;error(1);goto 99;
              end
            else begin
              c^.pp.PT:=CommPhrase;
              c^.pp.comm:=cc;
              if cc=Meas_4 then begin
                if Prev_Phrase.PD=NodePhrase then begin
                  case Target_Machine of
                    FACT_machine:
                      begin
                        if mach_add(Prev_Phrase.Address) mod 2 <>0 then begin
                          Err_Mess:='FROM address must be even';
                          error(0);goto 99;
                        end;
                      end;
                    OTHERWISE
                      begin
                        Err_mess:=' 4-wire Measurement Illegal';
                        error(0);goto 99;
                      end;
                  end{CASE};
                    
                end;
              end;
                     
              parse_cc(cc,c^.pp,unused); { get parameters if any} 
                  
              if c^.next=NIL then begin
                new(t);t^.next:=NIL;
                c^.next:=t;
              end;                     
              Prev_Phrase.PD:=CommPhrase;Prev_Phrase.Comm:=cc;  
              get_field(Prev_Phrase,unused,c^.next);{ go to next field }
            end;
            end   
          else begin
            Err_Mess:='Bad connector/command:';
            Err_P1:=f1;
            Error(1);
          end;    
        end;      
      end;        
  99:
  end;            
                  
                  
                  


{**********************************************************************}
procedure Actuate_Device(Device,from_pos,to_pos:alfa;long_name:alfa4);
   { this procedure makes the Operator to put a switch to a position}
label 99;
var                          
    i,l,l_l:integer;
    dev:dev_link;
    from_state,to_state:state_link;
    trailer,printed:boolean;
    ret_pos,dummya:alfa;
    long_alfa:alfa4;
    sw_dis : mess_rec;                     

      
  procedure conjunction(var printed:boolean;i,j:integer);
  begin
       if printed then begin
         if i=j then begin
           dummya:='and';add_alfa(dummya,sw_dis);add_blank(sw_dis);
           end
         else begin
           dummya:=',';add_alfa(dummya,sw_dis);
         end;            
       end;
       printed:=true;
  end;
begin
   dev:=device_pointer(Device);
   if dev<>z_dev then begin
     long_alfa:=' ';{ just a dummy field}
     get_state(dev,from_pos,long_alfa,from_state,ret_pos);
     if not OK then goto 99;
     sw_dis.line:=' ';sw_dis.length:=0;
     case dev^.part_type of
       SWITCH:
          Begin      
             trailer:=false;
             if (to_pos=NULL) and dev^.held then begin
               get_state(dev,ret_pos,long_alfa,to_state ,dummya );
               if to_state<>NIL then begin
                 dummya:=' Release ';add_alfa(Dummya,sw_dis);add_blank(sw_dis);
                 add_alfa4(dev^.dev_label,sw_dis);
                 dummya:='(';add_alfa(Dummya,sw_dis);
                 add_alfa(dev^.dev,sw_dis);

                 Long_alfa:=').';add_alfa4(long_alfa,sw_dis);
                 l:=alfa_length(to_state^.cond);
                 if to_state^.cond[l]<>'~' then begin
                   Long_alfa:='Verify it returns to';
                   add_alfa4(long_alfa,sw_dis);
                   add_blank(sw_dis);                        
                   if to_state^.name<>' ' then begin
                     add_alfa4(to_state^.name,sw_dis);
                     end
                   else begin
                     add_alfa(to_state^.desc,sw_dis);
                   end;
                   Dummya:=' Position';add_alfa(Dummya,sw_dis);
                 end;
                 end
               else begin
                 Err_Mess:=' Cannot find position to return to!!:';
                 Err_P1:=Ret_Pos;
                 Error(1);goto 99;
               end;
               end
             else begin
               get_state(dev,to_pos  ,long_name,to_state  ,dummya );
               if not OK then goto 99;        
  
               if (to_state<>NIL) then begin
                 add_blank(sw_dis);
                 if to_state^.cond=' ' then begin
                   Dummya:='SET';add_alfa(Dummya,sw_dis);add_blank(sw_dis);
                   trailer:=true;
                   end
                 else {action commands} begin
                   l:=alfa_length(to_state^.cond);
                   dev^.held:=false;printed:=false;
                              
                   if to_state^.cond[l]='~' then l_l:=l-1 else l_l:=l;
                   for i:=1 to l do begin
             
                     case to_state^.cond[i] of
                       '*':  {default};
     
                       'D':  begin
                               conjunction(printed,i,l_l);
                               Dummya:='Depress';add_alfa(Dummya,sw_dis);
                               add_blank(sw_dis);
                               trailer:=true;
                             end;
          
                       'P':  begin
                               conjunction(printed,i,l_l);
                               Dummya:='Pull ';add_alfa(Dummya,sw_dis);
                               add_blank(sw_dis);
                               trailer:=true;
                             end;
      
                       'S':  begin
                               conjunction(printed,i,l_l);
                               Dummya:='Set';add_alfa(Dummya,sw_dis);
                               add_blank(sw_dis);
                               trailer:=true;
                             end;
      
                       'R':  begin
                               conjunction(printed,i,l_l);
                               Dummya:='Release';add_alfa(Dummya,sw_dis);
                               add_blank(sw_dis);
                               trailer:=true;
                             end;
      
                       'T':  begin
                               conjunction(printed,i,l_l);
                               Dummya:='Turn';add_alfa(Dummya,sw_dis);
                               add_blank(sw_dis);
                               trailer:=true;
                             end;
      
                       'H':  begin
                               conjunction(printed,i,l_l);
                               Dummya:='HOLD';add_alfa(Dummya,sw_dis);
                               add_blank(sw_dis);
                               dev^.held:=true;
                             end;
        
                       '~':  begin { no trailer}
                               trailer:=false;
                             end;

                     OTHERWISE
                             Err_mess:=' Bad switch position command:';
                             err_p1:=to_state^.cond;error(1);goto 99;
                     end;
                   end{for};
                 end{action commands};  
                 add_alfa4(dev^.dev_label,sw_dis);
                 dummya:='(';add_alfa(Dummya,sw_dis);
                 add_alfa(dev^.dev,sw_dis);
                 dummya:=')';add_alfa(Dummya,sw_dis);
                 if trailer then begin
                   dummya:=' to';add_alfa(Dummya,sw_dis);add_blank(sw_dis);
                   if to_state^.name<>' ' then begin
                     add_alfa4(to_state^.name,sw_dis);
                     end                   
                   else begin
                     add_alfa(to_state^.desc,sw_dis);
                   end;
                   dummya:=' Position';add_alfa(Dummya,sw_dis);
                 end;
                 end
               else begin
                 Err_Mess:='Can not actuate this switch!Help!.REPORT!';
                 Error(0);goto 99;
               end;
             end;
             wait_manual:=true;{ will be used in XCI command}
             Put_Display(d,sw_dis);
          end;
       RELAY:
          begin
             {nothing to do yet}
          end;
       OTHERWISE
          Err_Mess:='This is not a device i can manipulate !';
       END;
       end
     else begin
       err_mess:='No such device:';
       err_p1:=Device;error(1)
     end;
 99:;
end;

{********************************************************************}

function Nth_letter(n:integer):alfa;

var  value:alfa;

begin
    value:=' ';
    if(n>=0) and (n<=26) then begin
      value[1]:=chr(Ord('A')+n-1);
      end
    else begin
      value[1]:='?';
    end;
    Nth_letter:=value;
end;          
      
{********************************************************************}
procedure Cable_Hookup(Cable_Name:alfa2);
 var t_adp_dev:adp_dev_link;
     display:mess_rec;
     a1:alfa;
     i,cx_add:integer;
     printed:boolean;

 procedure Hookup_Cable(var ad:adp_dev;UUT_Conn:alfa);
  var lines:integer;
  procedure Hookup_to_tester(ad:adp_dev;UUT_Conn:alfa);
   var i,j:integer;
       a1:alfa;
   begin
      printed:=false;
      for j:=1 to ad.tester_cnxs do begin
        display.length:=0;
        tab_to(4,display);
        if not printed then add_alfa2(ad.name,display);printed:=true;
        tab_to(24,display);
        a1:='P';add_alfa(a1,display);
        a1:=Number_to_alfa(j,0);add_alfa(a1,display);
        tab_to(32,display);
        a1:='TO';add_alfa(a1,display);
        tab_to(42,display);
        case Target_Machine of
          FACT_Machine: 
            begin
              a1:='FACT';add_alfa(a1,display);
              tab_to(50,display);
              if No_At_Field then begin
                a1:=number_to_alfa(Mach_Xlate(ad.address+(j-1)*60),0);
                add_alfa(a1,display);
                end
              else begin
                a1:='J';add_alfa(a1,display);
                a1:=number_to_alfa((ad.address div PPC)+j,0);
                add_alfa(a1,display);
              end;
            end;
          DITMCO_660:
            begin
              cx_add:=Mach_Xlate(ad.address+(j-1)*100);
              tab_to(50,display);
              a1:=number_to_alfa(cx_add div 100,3);
              add_alfa(a1,display);
              a1:='-';
              add_alfa(a1,display);
              a1:=number_to_alfa((cx_add div 100)+1,3);
              add_alfa(a1,display);
            end;
        OTHERWISE
          err_mess:='Internal error.Bad target machine in hookup';
          error(0);
        end;
        put_display(hookup,display);
      end;                                                     
   end; 

  procedure Hookup_ZIF_to_tester(ad:adp_dev;UUT_Conn:alfa);
   var i,j:integer;
       a1:alfa;
   begin
      printed:=false;
      for j:=1 to ad.tester_cnxs do begin
        display.length:=0;
        tab_to(4,display);
        if not printed then begin
          a1:='Connect';add_alfa(a1,display);
          tab_to(12,display);
          add_alfa2(ad.name,display);printed:=true;
        end;
        tab_to(32,display);
        a1:='P1';add_alfa(a1,display);
        a1:=Nth_letter(j);add_alfa(a1,display);
        tab_to(40,display);
        a1:='TO';add_alfa(a1,display);
        tab_to(48,display);
        case Target_Machine of
          FACT_Machine: 
            begin
              a1:='Address';add_alfa(a1,display);
              tab_to(58,display);
              if No_At_Field then begin
                a1:=number_to_alfa(Mach_Xlate(ad.address+(j-1)*60),0);
                add_alfa(a1,display);
                end
              else begin
                a1:='J';add_alfa(a1,display);
                a1:=number_to_alfa((ad.address div PPC)+j,0);
                add_alfa(a1,display);
              end;
            end;
        OTHERWISE
          err_mess:='Internal error.Bad target machine in hookup_ZIF';
          error(0);
        end;
        put_display(hookup,display);
      end;                                                     
   end; 
                                  
  procedure Hookup_to_UUT(ad:adp_dev;UUT_Conn:alfa);
   var t_adp_subdev:adp_subdev_link;
       i:integer;
       a1:alfa;
       a2:alfa2;
   begin
       t_adp_subdev:=ad.subdev;
       while t_adp_subdev<>ad.z_subdev do begin
         with t_adp_subdev^ do begin
           display.length:=0;
           tab_to(4,display);
           if (ad.tester_cnxs<=0) then begin
             add_alfa2(ad.name,display);
             end
           else begin
             tab_to(24,display);
             add_alfa(CAB_Name,display);
           end;
           tab_to(32,display);
           if UUT_Name='N/C' then begin
             a2:='NOT Connected!';
             add_alfa2(a2,display);
             end
           else begin
             a1:='TO';add_alfa(a1,display);
             tab_to(42,display);
             case Target_Machine of
               FACT_Machine:
                 begin
                   add_alfa(UUT_name,display);
                 end;
               DITMCO_660:
                 begin
                   tab_to(50,display);
                   add_alfa(UUT_name,display);
                 end;
             end;
           end;
           put_display(hookup,display);
         end;
         t_adp_subdev:=t_adp_subdev^.next;
       end;
       if printed then begin
         display.length:=0;
         put_display(hookup,display);
       end; 
   end; 

  procedure Hookup_ZIF_to_UUT(ad:adp_dev;UUT_Conn:alfa);
   var t_adp_subdev:adp_subdev_link;
       i:integer;
       NONE_Count:integer;
       a1:alfa;
       a2:alfa2;
   begin
       t_adp_subdev:=ad.subdev;NONE_Count:=0;
       while t_adp_subdev<>ad.z_subdev do begin
         with t_adp_subdev^ do begin
           display.length:=0;
           tab_to(4,display);
           if (ad.tester_cnxs<=0) then begin
             add_alfa2(ad.name,display);
             end
           else begin
             tab_to(32,display);
             add_alfa(CAB_Name,display);
           end;
           tab_to(40,display);
           a1:='TO' ;add_alfa(a1,display);
           tab_to(50,display);
           a1:='UUT';add_alfa(a1,display);
           if UUT_Name='N/C' then begin
             tab_to(58,Display);
             a2:='NONE';
             add_alfa2(a2,display);
             NONE_Count:=NONE_Count+1;
             tab_to(63,display);
             add_alfa(Number_To_Alfa(NONE_Count,0),Display);
             end
           else begin
             tab_to(58,Display);
             case Target_Machine of
               FACT_Machine:
                 begin
                   add_alfa(UUT_name,display);
                 end;
             OTHERWISE
               err_mess:='Bad target machine in Hookup_ZIF_to_UUT';
               error(0);
             end;
           end;
           put_display(hookup,display);
         end;
         t_adp_subdev:=t_adp_subdev^.next;
       end;
       if printed then begin
         display.length:=0;
         put_display(hookup,display);
       end; 
   end; 

  procedure hookup_strip(ad:adp_dev;UUT_Conn:alfa);
   var j:integer;

   begin
        printed:=false;
        display.length:=0;
        tab_to(4,display);
        if not printed then add_alfa2(ad.name,display);printed:=true;
        tab_to(24,display);
        a1:='TO';add_alfa(a1,display);
        add_blank(display);
        a1:='FACT';add_alfa(a1,display);
        add_blank(display);
        a1:='ADDRESS';add_alfa(a1,display);
        add_blank(display);
        a1:=number_to_alfa(Mach_Xlate(ad.address),0);
        add_alfa(a1,display);
        a1:=' AND UUT';add_alfa(a1,display);
        add_blank(display);
        add_alfa(ad.subdev^.UUT_name,display);
        put_display(hookup,display);
   end;
                     
  begin
      if not ad.Hooked_up then begin
        if ad.STRIP_type then begin
          Hookup_strip(ad,UUT_Conn);
          ad.Hooked_up:=printed;
          end     
        else if ad.ZIF_type then begin
          Hookup_ZIF_to_tester(ad,UUT_Conn);
          Hookup_ZIF_to_UUT   (ad,UUT_Conn);
          ad.Hooked_up:=printed;
          end     
        else begin
          lines:=ad.tester_cnxs+ad.UUT_cnxs;
          if lines<=MAX_DISPLAY_LINES then begin
            if (lines+display_lines)>=MAX_DISPLAY_LINES then begin
              display.length:=0;
              put_display(ds,display);
            end;
          end;
          Hookup_to_Tester(ad,UUT_Conn);
          Hookup_to_UUT   (ad,UUT_Conn);
          ad.Hooked_up:=printed;
        end;
      end;
  end;

  function UUT_Name_Match(ad:adp_dev;a1:alfa):boolean;
  var t_subdev:adp_subdev_link;
      b:boolean;
   begin
      b:=false;
      t_subdev:=ad.subdev;
      while t_subdev<>ad.z_subdev do begin
        b:=b or (t_subdev^.UUT_name=a1);
        t_subdev:=t_subdev^.next;
      end;
      UUT_Name_match:=b;
   end;

 begin
      for i:=1 to al do a1[i]:=Cable_Name[i];
      t_adp_dev:=adapter^.first;
      while t_adp_dev<>NIL do begin
        if t_adp_dev^.name=Cable_Name then begin
          Hookup_CABLE(t_adp_dev^,NULL);
          end
        else if UUT_name_match(t_adp_dev^,a1) then begin
          Hookup_CABLE(t_adp_dev^,a1);
          end
        else if Cable_Name=NULL_2 then begin
          Hookup_CABLE(t_adp_dev^,NULL);
        end;
        t_adp_dev:=t_adp_dev^.next;
      end;
      if Printed and (Cable_Name=NULL_2) then begin
        display.length:=0;
        Wait_Manual:=true;
      end;
 end;


           
{**********************************************************************}
%include 'prmpt.inc'

{**********************************************************************}
procedure Get_Wish(var what:wish);
var b:alfa;       
    p:integer;
    acceptable_switches:charset;
 function lower_case(c:char):char;
   begin 
     if c in ['A'..'Z'] then lower_case:=chr(ord(c)+32) else lower_case:=c;
   end;

 function upper_case(c:char):char;
   begin 
     if c in ['a'..'z'] then upper_case:=chr(ord(c)-32) else upper_case:=c;
   end;

 procedure wish_switches;
  label 99;
  var i,l:integer;
  function good_switch(c:char):boolean;
   begin
      good_switch:=(upper_case(c) in acceptable_switches)
                   or (lower_case(c) in acceptable_switches);
   end;
  
  function arg_allowed(c:char):boolean;
   begin
      arg_allowed:=lower_case(c) in acceptable_switches;
   end;

  begin
      getsym;{skip over @ }
      getsym;
      l:=alfa_length(a);
      if l>0 then begin 
        for i:=1 to l do begin
          if good_switch(a[i]) then begin
            num_switches:=num_switches+1;
            switches[num_switches].switch:=a[i];
            end
          else begin
            Err_Mess:=' Switch is not allowed for this command:';
            err_p1:=' ';err_p1[1]:=a[i];error(1);goto 99;
          end;
        end;
        if (ch=':') and together then begin
          if arg_allowed(switches[num_switches].switch) then begin
            getsym;{throw away ':' }
            if together then begin
              getsym;switches[num_switches].arg:=a;
              end
            else begin
              err_mess:=' Switch argument expected after colon:';
              error(0);goto 99;
            end;
            end  
          else begin
            err_mess:='Argument is not allowed for switch:';
            err_p1:=' ';err_p1[1]:=switches[num_switches].switch;
            error(1);goto 99;
          end;
        end;
        if ch='@' then wish_switches;
        end
      else begin
        err_mess:='Switches expected after @';
        error(0);goto 99;
      end;
99:  end;                         

begin             
                  
      What:=no_Command;continue:=false;
      acceptable_switches:=[];
      P:=p_punch(Big_a);

      if      Big_a='+' then begin what:=Old_wish;continue:=true;end
      else if (P>=0) and (P<=95)    then what:=pwrcl     
      else if (P>=100) and (P<=195) then what:=pwrop     
      else if  P=999                then what:=clear_all
      else if Big_a='#'         then what:=xfer
      else if Big_a='CON'       then what:=con
      else if Big_a='WC'        then what:=wc
      else if Big_a='D'         then what:=d
      else if Big_a='DS'        then what:=ds
      else if Big_a='DIODE'     then what:=cr
      else if Big_a='DIS'       then what:=dis
      else if Big_a='FC'        then begin 
        what:=fc;acceptable_switches:=['L','F','g'];{lowest,force,group}
        end
      else if Big_a='FCI'       then begin 
        what:=fci;acceptable_switches:=['L','F','g'];
        end
      else if Big_a='OMIT'      then begin
        what:=omit;acceptable_switches:=['H','S','g'];
        end
      else if Big_a='UNOMIT'    then what:=unomit
      else if Big_a='STUB'      then what:=stub
      else if Big_a='DEVICES'   then what:=devs
      else if Big_a='CONNECT'   then what:=conx
      else if Big_a='XCI'       then begin 
        what:=xci;acceptable_switches:=['4','m','r'];
        end 
      else if Big_a='XC'        then begin
        what:=xc;acceptable_switches:=['4','m','r'];
        end
      else if Big_a='RCI'       then begin 
        what:=rci;acceptable_switches:=['4'];
        end 
      else if Big_a='RC'        then begin
        what:=rc;acceptable_switches:=['4'];
        end
      else if Big_a='SC'        then begin
        what:=sc;acceptable_switches:=['L','g'];
        end
      else if Big_a='SCI'       then begin
        what:=sci;acceptable_switches:=['L','g'];
        end
      else if Big_a='SW'        then what:=sw
      else if Big_a='APPLY'     then what:=apply
      else if Big_a='REMOVE'    then what:=remove
      else if Big_a='HOOKUP'    then what:=hookup
      else if Big_a='KEYBOARD'  then what:=kb
      else if Big_a='OFFSET'    then what:=fudge
      else if Big_a='/E'        then what:=exit
      else if Big_a='EX'        then what:=exit
      else if Big_a='EXI'       then what:=exit
      else if Big_a='EXIT'      then what:=exit
      else if To_Keyboard then begin     

        if      Big_a='WRS'       then what:=wrs
        else if Big_a='WIRES'     then what:=wires
        else if Big_a='XXX'       then begin
          what:=xxx;acceptable_switches:=['a','A'];
          end
        else if Big_a='N2A'       then what:=n2a
        else if Big_a='SHOW'      then what:=Show
        else if Big_a='I2N'       then what:=i2n  
        else if Big_a='RELAX'     then what:=rlx
        else if Big_a='OHMS'      then begin
          what:=ohms;acceptable_switches:=['E'];
          end
        else if Big_a='STRING'    then what:=strng
        else if Big_a='STR'       then what:=strng
        else if Big_a='VDIFF'     then what:=v_del
        else if Big_a='bfs'       then what:=bfs
        else if Big_a='OPN'       then what:=opn  
        else if Big_a='FNODE'     then what:=fnode
        else                       what:=no_command;
        end

      else                      what:=no_command;

      if what in [pwrcl,pwrop] then begin
        power_relay:=P mod 100;
      end;

      if not continue then begin
        num_switches:=0;
        if (ch='@') and together then begin
          wish_switches;
        end;                             
      end;

      Old_wish:=what;
end;              
{**********************************************************************}
procedure write_node(v:integer;var ll:integer);
var n:name;
    b:alfa2;
    i,l:integer;
begin
        n:=Int_To_Name(v);
        make_pin_name(n.dev,n.pin,b,l);
        ll:=ll+l+1;
        if ll>70 then begin ll:=0;writeln(OUTPUT);end;
        for i:=1 to l do write(OUTPUT,b[i]);
end;     

procedure write_flags(t_node:node_link;var ll:integer);              

  procedure write_flag(c:char);
   begin write(OUTPUT,c);ll:=ll+1;end;

 begin                                
    with t_node^ do begin
      write(OUTPUT,'.');
      case cnx of

        contact: 
         begin
           if contact_state=opened then write_flag('~');{ is contact closed?}
           if o_c=YES              then write_flag('O');{contact was O checked}
           if j_d=YES              then write_flag('d');{just disconnected};
         end;

        wire: 
         begin
           write_flag('w');
         end;

        admittance:
         begin
           write_flag('r');
         end;

      OTHERWISE
        write_flag('?');
      end; 

      if a_w=YES then write_flag('a');{crucial path}
      if x_c=YES then write_flag('X');{ wire/contact was x checked once}
      if require=O_Check then write_flag('o');
      if require=X_Check then write_flag('x');
      if path=YES        then write_flag('p');{opposite end leads to outside}
    end;                          
 end;
                       
{***********************************************************************}
 procedure get_display(p:integer);
 var i,j:integer;
 begin                   
       j:=p-1;
       for i:=p to ll-1 do begin
         if full_line[i]<>' ' then j:=i;
       end;
       display.line:=' ';
       for i:=p to j do display.line[i-(p-1)]:=full_line[i];
       display.length:=j-p+1;
 end;

{**********************************************************************}
procedure write_error_mess(var mess:mess_rec);
var i:integer;
begin
    for i:=1 to mess.length do write_error_c(mess.Line[i]);
    write_error_ln;
end;
{**********************************************************************}
  procedure  Show_results;
   type
      modes=(Opens,Wrs_xc,Wrs_rc,Cntct_xc,Cntct_rc,Sngl_Wr);
   var
      i      : integer;
      printed: boolean;
      mode   : modes;
      n:integer;
      Header:Str79;
      cnx_count:integer;
      Only_wire,Only_Resistor,Report_It:boolean;

    procedure write_name(v:integer;var ll:integer);
      var n:name;
          b:alfa2;
          i,l:integer;
      begin
          n:=Int_To_Name(v);
          make_pin_name(n.dev,n.pin,b,l);
          for i:=1 to l do begin 
            write_error_c(b[i]);
          end;
          ll:=ll+l;
          if ll>80-al then begin
            write_error_ln;for i:=1 to 6 do write_error_c(' ');ll:=6;
          end;
      end;




  function should_print(t_node:node_link):boolean;          

   begin
         with t_node^ do begin
           case mode of
             Wrs_xc  :Begin
                        should_print:=(x_c=NO) and (cnx=Wire);
                      end;
                     
             Wrs_rc  :Begin
                        should_print:=(x_c=NO) and (r_c=NO) and (cnx=Wire);
                      end;
                                                                       
             Cntct_xc:Begin
                        should_print:=(x_c=NO) and (cnx=Contact);
                      end; 
                                                  
             Cntct_rc:Begin
                        should_print:=(x_c=NO) and (r_c=NO) and (cnx=Contact);
                      end;

             Opens   :Begin
                        should_print:=(o_c=NO) and (cnx=Contact);
                      end;
                                   
             Sngl_wr :Begin
                        should_print:=false;
                      end;

           end;                                                   
         end;
   end;


   procedure show;
    var t_node:node_link;
        i,ll,x:integer;
        Headed:Boolean;




    procedure process_preamble(x:integer);
      var i:integer;
      begin
          if not Headed then begin
             for i:=1 to 79 do write_error_c(Header[i]);write_error_ln;
             Headed:=true;
          end;
          if not printed then begin
            ll:=1;write_error_c(' ');
            if x<>No_Node then begin
              write_name(x,ll);write_error_c(' ');ll:=ll+1;
            end;
            printed:=true;
          end;
      end;

     begin {show}
         Headed:=false;
         for i:=v_min to v_max DO BEGIN 
           t_node:=adj[i];printed:=false;
           cnx_count:=0;Only_wire:=true;Only_Resistor:=true;
           FLUSH_QUEUE(temp_QUEUE);
           while t_node <> z_node do BEGIN
             with t_node^ do begin
               if v>i then begin
                 if should_print(t_node) then begin
                   ENQUEUE(v,temp_QUEUE);
                 end;
               end;
               if cnx<>Wire then begin
                 Only_wire:=false;
               end;
               if cnx<>Admittance then begin
                 Only_resistor:=false;
               end;
             end;
             cnx_count:=cnx_count+1;
             t_node:=t_node^.next;
           end;

           if cnx_count=1 then begin
             if Only_wire then begin 
               Report_it:=true;
               end
             else if Only_Resistor then begin
               Report_it:=true;
               end
             else begin
               Report_it:=false;
             end;
             end
           else begin
             Report_it:=(QUEUE_COUNT(temp_QUEUE)>0);
           end;

           while not EMPTY_QUEUE(temp_QUEUE) do begin
             x:=FRONT(temp_QUEUE);DEQUEUE(temp_QUEUE);
             if report_it then begin
               process_preamble(i);
               write_name(x,ll);write_error_c(' ');
               ll:=ll+1;n:=n+1;
             end;
           end;

           if (cnx_count=1) and (mode=Sngl_wr) then begin
             if not ext_node(i) then begin
               if Only_Wire then begin
                 process_preamble(No_Node);
                 write_name(i,ll);write_error_c(' ');
                 ll:=ll+1;n:=n+1;
               end;
             end;
           end;
           if printed then write_error_ln;
         end; 
     end;

 begin

     mode:=Wrs_xc;
     Header:=' Continuity of following wires has not been "xc" verified';
     n:=0;show;
     if n>0 then begin
       clear_message(error_mesaj);add_blank(error_mesaj);
       add_alfa(number_to_alfa(n,0),error_mesaj);
       a_alfa2:=' Wires Not XC verified ';add_alfa2(a_alfa2,error_mesaj);
       write_error_mess(error_mesaj);write_error_ln;
     end;

     mode:=Wrs_rc;
     Header:=' Continuity of following wires has not been verified';
     n:=0;show;
     if n>0 then begin
       clear_message(error_mesaj);add_blank(error_mesaj);
       add_alfa(number_to_alfa(n,0),error_mesaj);
       a_alfa2:=' Wires Not verified ';add_alfa2(a_alfa2,error_mesaj);
       write_error_mess(error_mesaj);write_error_ln;
     end;

     mode:=Cntct_xc;
     Header:=' Continuity of following contacts has not been "xc" verified';
     n:=0;show;
     if n>0 then begin
       clear_message(error_mesaj);add_blank(error_mesaj);
       add_alfa(number_to_alfa(n,0),error_mesaj);
       a_alfa2:=' Contacts ';add_alfa2(a_alfa2,error_mesaj);
       write_error_mess(error_mesaj);write_error_ln;
     end;

     mode:=Cntct_rc;
     Header:=' Continuity of following contacts has not been verified';
     n:=0;show;
     if n>0 then begin
       clear_message(error_mesaj);add_blank(error_mesaj);
       add_alfa(number_to_alfa(n,0),error_mesaj);
       a_alfa2:=' Contacts ';add_alfa2(a_alfa2,error_mesaj);
       write_error_mess(error_mesaj);write_error_ln;
     end;

     mode:=opens;
     Header:=' DisContinuity of following contacts has not been verified';
     n:=0;show;
     if n>0 then begin
       clear_message(error_mesaj);add_blank(error_mesaj);
       add_alfa(number_to_alfa(n,0),error_mesaj);
       a_alfa2:=' Opens ';add_alfa2(a_alfa2,error_mesaj);
       write_error_mess(error_mesaj);write_error_ln;
     end;

     mode:=Sngl_Wr;
     Header:=' Following Internal nodes have single wire connections';;
     n:=0;show;
     if n>0 then begin
       clear_message(error_mesaj);add_blank(error_mesaj);
       add_alfa(number_to_alfa(n,0),error_mesaj);
       a_alfa2:=' Dead Ended Internal Wires';add_alfa2(a_alfa2,error_mesaj);
       write_error_mess(error_mesaj);write_error_ln;
     end;

 end;     

{********************************************************************}
procedure show_extras;
  begin
      writeln(output,' Total dfs searches:',dfs_Search);
      writeln(output,' Total bfs searches:',bfs_Search);
      if v_min <0 then begin
        writeln(output,' Total Internal Nodes:',-v_min);
      end;
      writeln(output,' Total External Nodes:', v_max);

      writeln(output,' Total Iterations:',Total_Iterations);
      writeln(output,' Max   Iterations:',Max_Iterations);
  end;           


{********************************************************************}
procedure show_string(v0:integer);
 var 
     ll,l:integer;
 procedure visit(k:integer;depth:integer);
  var t_node:node_link;


  BEGIN
    inf[k]^.dfs_visit:=dfs_Search;
    t_node:=adj[k];write(output,' ');
    ll:=ll+1;write_name(output,k,ll);
    WHILE (t_node <> z_node) DO BEGIN
      if short_path(t_node^) then begin
        if (inf[t_node^.v]^.dfs_visit<> dfs_Search) then begin
          visit(t_node^.v,depth+1);
        end;
      end;
      t_node:=t_node^.next;
    end
  end;
  
begin
  dfs_Search:=dfs_Search+1;ll:=1;
  IF (v0<=v_max) and (v0>=v_min) then begin
    visit(v0,0);writeln(output);
  end;
end;


{********************************************************************}
procedure show_info(v:integer);
 begin   
     write(Output,' Node:',v);
     with inf[v]^ do begin
       write(Output,' Voltage:',voltage.nominal);
     end;                                    
     writeln(Output);
 end;

{*************************************************************************}
 procedure generate_wire_list(var where:text);
 var b,Old_Conn:alfa;
     i,j,x:integer;
               
   Function Ditmco_Pin(Ditmco_Address:integer):integer;
   var pin:integer;
   begin
       if Ditmco_Address mod 200 = 0 then begin
         Pin:=100;
         end
       else if Ditmco_Address mod 100 = 0 then begin
         Pin:=50;
         end
       else begin
         Pin:=Ditmco_Address Mod 100;
         if (Ditmco_Address div 100) mod 2 = 1 then Pin:=Pin+50;
       end;  
       Ditmco_Pin:=Pin;
   end;

 begin
     case target_machine of
       FACT_Machine: 
         begin   
           for i:=max(0,v_min) to v_max do begin
              if In_use(i) then begin
                x:=Inf[i]^.m_add;
                if x=Chassis_Add then x:=0;
                b:=Number_To_Alfa(x,5);
                write(where,'  ');
                for j:=1 to 5 do write(where,b[j]);
                v1:=Int_to_Name(i);
                write(where,'  ');write(Where,v1.dev);
                write(where,'  ');write(where,v1.pin);
                if No_At_Field then begin
                  end
                else begin
                  write(where,'  ');write(where,'J');
                  write(where,((i-1) div PPC)+1);
                end;      
                writeln(where);                    
              end;
           end;
         end;
       DITMCO_660,DITMCO_9100:
         begin
           Old_Conn:=' ';
           writeln(where);
           for i:=Max(0,v_min) to v_max do begin
             if In_use(i) then begin
               x:=Inf[i]^.m_add;
               if x=Chassis_Add then x:=0;

               v1:=Int_to_Name(i);
               if(v1.dev<>Old_Conn)then begin
                 writeln(where);writeln(where);
                 Old_Conn:=v1.dev;
               end;

               b:=Number_To_Alfa(x,5);
               write(where,'  ');
               for j:=1 to 5 do write(where,b[j]);

               write(where,'                     ');
               b:=Number_To_Alfa(Ditmco_Pin(x),3);
               for j:=1 to 3 do write(where,b[j]);

               write(where,'                 ');
               write(where,v1.dev);

               x:=alfa_length(v1.pin);
               write(where,'  ');
               for j:=1 to x do write(where,v1.pin[j]);

               writeln(where);                    
             end;
           end;
           writeln(where,'@@');
         end;
     OTHERWISE
       err_mess:=' BUG! Bad target machine in generate_wire_list';
       error(0);
     end;  
 end;

{********************************************************************}
procedure interpret;
var i,j,x,y:integer;
    n:name;
    This_Phrase:Phrase_Desc;
    voltage,current:real;
    volts:measurement;
    resistance:measurement;
    u:units;
    Boole:Boolean;
    v1,v2:name;
    f1,f2:alfa;
  procedure clear_tester;
  begin
      case target_machine of
        FACT_machine:Add_Param(0,'P',999,3);
        DITMCO_660  :Add_Param(0,'M',00030,5);
        DITMCO_9100 :Add_Param(0,'M',00030,5);
      end;
  end;

  procedure Operate_Power_Relay(what:wish;Relay:Integer);
  var x1,x2,x3,x4,x5,xxx,xxxxx:integer;
  begin                 
      case target_machine of
        FACT_Machine: 
          begin
            case what of
              pwrop: x1:=1;
              pwrcl: x1:=0;
            end; 
            xxx:=x1*100+Relay;
            Add_Param(0,'P',xxx,3);
          end;
        DITMCO_660: 
          begin
            x3:=Relay div 10;
            x5:=Relay mod 10;
            case what of
              pwrop: x4:=2;
              pwrcl: x4:=1;
            end;
            xxxxx:=x3*100+x4*10+x5;
            Add_Param(0,'M',xxxxx,5);
          end;
        DITMCO_9100: 
          begin
            x3:=Relay div 10;
            x5:=Relay mod 10;
            case what of
              pwrop: x4:=2;
              pwrcl: x4:=1;
            end;
            xxxxx:=x3*100+x4*10+x5;
            Add_Param(0,'M',xxxxx,5);
          end;
      OTHERWISE
        err_mess:='BUG! Bad target machine';
        error(0);
      end;
  end;

  procedure Operate_PSupply(what:wish;Relay:Integer);
  var Power_Supply:alfa;
      volts:real;unit:units;
      dev:dev_link;
  begin
      
      case Relay of
        96: BEGIN 
              power_supply:='PS96'; 
              volts:=28.0;unit:=VDC;
            END;

        97: BEGIN 
              power_supply:='PS97'; 
              volts:=26.0;unit:=VAC;
            END;

        98: BEGIN 
              power_supply:='PS98'; 
              volts:=5.0;unit:=VAC;
            END;

        99: BEGIN 
              power_supply:='PS99'; 
              volts:=115.0;unit:=VDC;
            END;

      otherwise
        power_supply:='UNDEFINED';
      end;

      dev:=device_pointer(power_supply);
      if dev<>NIL then begin    
        case target_machine of
          FACT_Machine: 
            begin
              case what of
                pwrop: Apply_Psupply(Power_Supply,volts,unit,1.0,x,y);
                pwrcl: Remove_Psupply(Power_Supply,x,y);
              end; 
            end;
          DITMCO_660: 
            begin
              case what of
                pwrop: Apply_Psupply(Power_Supply,volts,unit,1.0,x,y);
                pwrcl: Remove_Psupply(Power_Supply,x,y);
              end;
            end;
          DITMCO_9100: 
            begin
              case what of
                pwrop: Apply_Psupply(Power_Supply,volts,unit,1.0,x,y);
                pwrcl: Remove_Psupply(Power_Supply,x,y);
              end;
            end;
        OTHERWISE
          err_mess:='BUG! Bad target machine';
          error(0);
        end;
      end;
  end;


  procedure operate_power_relays(what:wish;Device,v1,v2:alfa);
    var n1,n2:integer;
        dev_ptr:dev_link;


    begin
        n1:=trailing_number(v1);
        n2:=trailing_number(v2);
        
        Dev_ptr:=Device_Pointer(Device);
        case what of
          pwrcl: BEGIN
                   with dev_ptr^ do begin
                     POS_RELAY:=v1;
                     NEG_RELAY:=v2;
                    end;
                 END;
          pwrop: BEGIN
                   with dev_ptr^ do begin
                     if (v1=NULL) or (v1=' ') then begin
                       n1:=trailing_number(POS_RELAY);
                       end
                     else begin
                       n1:=trailing_number(v1);
                     end;
                     if (v2=NULL) or (v2=' ') then begin
                       n2:=trailing_number(NEG_RELAY);
                       end
                     else begin
                       n2:=trailing_number(v2);
                     end;
                   end;
                 END;
       
        OTHERWISE 
            err_mess:=' Internal error (wish) in operate_power_relays';
            error(0);
        end;

        operate_power_relay(what,n1);
        operate_power_relay(what,n2);
    end;

  procedure process_device(dev_name:alfa;Procedure proc_pin(v:name;b,r:boolean);
                                                   flag,report:boolean);
    var d_ptr:dev_link;
        p_ptr:pin_link;
        v    :name;

    begin
        d_ptr:=device_pointer(dev_name);v.dev:=dev_name;
        p_ptr:=d_ptr^.p;
        WHILE (p_ptr<>d_ptr^.z_p) and OK DO begin
          v.pin:=p_ptr^.pin;
          proc_pin(v,flag,report);
          p_ptr:=p_ptr^.p;
        end;
    end;
        


  procedure process_node(v1:name;Procedure proc_pin(v:name;b,r:boolean);
                                           flag,report:boolean);
    begin
        if v1.pin='[' then begin
          process_range(v1.dev,']',getsym,proc_pin,flag,report);
          end
        else if good(address_of(v1)) then begin
          proc_pin(v1,flag,true);
          end
        else if (v1.pin=No_Pin) and (ch='[') then begin
          getsym;{skip over ']'}
          process_range(v1.dev,']',getsym,proc_pin,flag,report);
          end
        else if device_exists(v1.dev) and (v1.pin=no_pin) then begin
          process_device(v1.dev,proc_pin,flag,{ report } false);
          end
        else begin
          err_mess:=' Pin does not exist:';
          err_p1:=v1.dev;err_p2:=v1.pin;
          error(2);
        end; 
    end;  


    procedure test_diode(anode_node,cathode_node:integer;find:boolean);
    label 99;
    var
       Diode_Fwd    : Params; { Diode forward}
       Diode_Rev    : Params; { Diode reverse}
       Anode,Cathode:Integer;
       res1,res2:real;
       how:cmnds;

    begin

        if find then begin
          anode:=lowest_resistance(anode_node,res1);
          cathode:=lowest_resistance(cathode_node,res2);
          end
        else begin
          anode:=anode_node;cathode:=cathode_node;
          res1:=0.0;res2:=0.0;
        end;

        if anode = no_node then begin
          err_mess:='No external access to Anode of this diode';
          error(0);goto 99;
          end
        else if cathode=no_node then begin
          err_mess:='No external access to Cathode of this diode';
          error(0);goto 99;
        end;

        Diode_Fwd:=Diode_F;
        Diode_Fwd.v:=Diode_F.v+res1+res2;
        Diode_Fwd.v:=r_value(Diode_Fwd.v,0.0,+1,false,how);

        Diode_Rev:=Diode_R;
        
        case target_machine of

          FACT_Machine:
             Begin
               Add_Node(1,Cont_From,anode);
               Add_Comm(6,Diode_Fwd);
               Add_Node(6,Cont_To,cathode);
               Add_Comm(6,Diode_Rev);
               Add_Node(6,Open_To,cathode);
             end;
          DITMCO_660:
             Begin
               if find then begin
                 err_mess:='DITMCO 660 has only one diode test';
                 warn(0);
                 end
               else begin
                 Add_Node(1,Diode_From,anode_node);
                 Add_Node(1,Diode_To,cathode_node);
               end;
             end;
          DITMCO_9100:
             Begin
               Add_Node(1,Diode_From,anode);
               Add_Node(1,Diode_To,cathode);
             end;
        OTHERWISE
           err_mess:='BUG! BUG!  Bad target machine';
           error(0);
        end;
99:   end;

begin {interpret}
    while not done do begin
  
      if From_Keyboard then begin
        prmpt; 
      end;
                             
      get_line_sym;
      Get_Wish(What);
  
      OK:=true;
  
      case what of
  
      con: BEGIN
             if get_nodes(x,y)then begin
               connect_pair(x,y);
               BAD_GUY:=true;
             end; 
           END;   
                  
      dis: BEGIN  
             if get_nodes(x,y) then begin
               disconnect_pair(x,y);
               BAD_GUY:=true;
             end;                             
           END;   
                  
      wrs: BEGIN  
             x:=v_min;y:=v_max;getsym;
             if (symbol and number)then begin
               x:=max(numero,v_min);y:=x;
               getsym;
               if (symbol and number)then begin
                 y:=min(numero,v_max);
               end
             end;

             for j:=x to y DO BEGIN 
               t_node:=adj[j];                
               if t_node<>z_node then begin
                 i:=0;
                 write(OUTPUT,j:5);      
                 write(OUTPUT,'-->');
                 while t_node <> z_node do BEGIN
                   write(OUTPUT,t_node^.v:6);
                   write_flags(t_node,i);
                   t_node:=t_node^.next;
                 end;
                 writeln(OUTPUT);
               end;
             end; 
           end;   
                
      wires:BEGIN                

              get_node(NULL,v1);x:=Address_Of(v1); 
              if not good(x) then begin
                 x:=v_min;
                 y:=V_Max;
                 end
              else begin
                get_node(NULL,v1);y:=Address_Of(v1); 
                if not good(y) then begin
                  y:=x;
                end;
              end;

              if x>y then begin
               j:=x;x:=y;y:=j;
              end;
              for j:=x to y DO BEGIN      
                t_node:=adj[j];
                if t_node<>z_node then begin
                  i:=0;       
                  write_node(j,i);write(OUTPUT,'-->');
                  while t_node <> z_node do BEGIN
                    with t_node^ do begin
                      write_node(v,i);
                    end;
                    write_flags(t_node,i);
                    write(OUTPUT,' ');i:=i+1;            
                    t_node:=t_node^.next;
                  end;
                  writeln(OUTPUT);
                end;
              end; 
            END;           
                  
      v_del:BEGIN  { voltage between two points }
               if get_nodes(x,y) then begin
                 volts:=diff_voltage(x,y,false);
                 with volts do begin
                   writeln(OUTPUT,hilimit,nominal,lolimit);
                 end;
               end;       
            end;        

      Xxx: BEGIN  { are two nodes connected }
             if get_nodes(x,y) then begin
               Boole:=continuity(x,y,switch_is(['A','a']));
               writeln(OUTPUT,Boole);
             end;       
           end;        

      Ohms:BEGIN  { resistance between two nodes }
             if get_nodes(x,y) then begin
               if switch_is(['E','e'])then begin
                 x:=Get_Ext(x);y:=Get_Ext(y);
               end;
               resistance:=Ohmmeter(x,y);
               if OK Then begin
                 write(OUTPUT,resistance.LoLimit);
                 write(OUTPUT,resistance.nominal);
                 writeln(OUTPUT,resistance.HiLimit);
               end;
             end;       
           end;        

      wc : BEGIN  { conditional block messages }
             getsym;
             if (Big_a='FIRST') and (ch=' ') then begin
               wc_mode:=wc_first;
               end
             else if (Big_a='ALL') and (ch=' ') then begin
               wc_mode:=wc_all;
               end
             else if (Big_a='NONE') and (ch=' ') then begin
               wc_mode:=wc_none;
               end
             else begin 
               j:=0;
               for i:=4 to ll-1 do begin
                 if full_line[i]<>' ' then j:=i-3;
               end;
               if j=0 then begin
                 write_test_mess(6,mesaj);
                 end    
               else begin
                 clear_message(mesaj);
                 for i:=1 to j do add_one_char(full_line[i+3],mesaj);
                 write_test_mess(6,mesaj);
               end;
             end;
           end;


      cr : BEGIN  { diode test }
             if get_nodes(x,y) then begin
               if not continuity(x,y,false) then begin
                 if x<0 then x:=first_positive(x,0);
                 if y<0 then y:=first_positive(y,0);
                 if ext_pos(x) and ext_node(y) then begin
                   test_diode(x,y,false);
                   end
                 else begin
                   test_diode(x,y,true);
                 end;
                 end
               else begin
                 err_mess:='There is a jumper accross this diode';
                 error(0);
               end;
               end
             else begin   
               err_mess:=' Illegal diode node';
               error(0);
             end; 
           end;   
                  
      xc,xci:
           BEGIN  { incremental continuity/open checks }
             Params_1:=C_Params;  
             unused:=' ';
             if ch='<' then getsym;{ skip < if exists }
             if ch<>'>' then begin
               if switch_is(['4']) then begin
                 parse_cc(Cont_4,params_1,unused);
                 end
               else begin
                 parse_cc(Cont_c,params_1,unused);
               end;
             end;

             params_2:=O_Params;
             if unused='>' then begin
               parse_cc(Open_c,params_2,unused);
               end
             else if ch='>' then begin
               getsym;
               parse_cc(Open_c,params_2,unused);
             end;

             if unused<>' ' then begin
               err_mess:='Extraneous characters at the end of line';
               error(0);
             end;

             if OK then begin
               xc_tests(params_1,what=xci,switch_is(['4']),SHORT_SHORTS);
               if switch_is(['r','R']) then begin
                 xc_tests(params_1,what=xci,switch_is(['4']),RESISTORS);
               end;

               open_tests(params_2,what=xci,SHORT_OPENS);
               if switch_is(['r','R']) then begin
                 open_tests(params_2,what=xci,RESIS_OPENS);
                { open_tests(params_2,what=xci,RESISTORS);}
               end;
             end;
             FLUSH_QUEUE(x_QUEUE);
             FLUSH_QUEUE(y_QUEUE);
             FLUSH_TEST_QUEUE(RESIS_QUEUE);
           END;                          
                  
      rc,rci:
           BEGIN  { incremental continuity/open checks }
             getsym;
             unused:=a;
             if unused<>' ' then begin
               err_mess:='Extraneous characters at the end of line';
               error(0);
             end;
             params_1:=C_Params;params_2:=O_Params;
             if OK then begin
               xc_tests(params_1,what=rci,switch_is(['4']),RESISTORS);
               open_tests(params_2,what=rci,RESIS_OPENS);
             end;
             FLUSH_QUEUE(x_QUEUE);
             FLUSH_QUEUE(y_QUEUE);
             FLUSH_TEST_QUEUE(RESIS_QUEUE);
           END;   
                  
                  
      n2a: BEGIN { Name to address conversion }
             get_node(NULL,v1);x:=Address_of(v1);
             if good(x) then begin
               writeln(OUTPUT,' Address:=',x,' FACT:',Mach_Add(x));
               end
             else begin
               err_mess:='Bad Name specified';
               error(0);
             end; 
           end;   

      rlx: BEGIN { Relax from a node }
             get_node(NULL,v1);x:=Address_of(v1);
             if good(x) then begin
               ENQUEUE(x,live_QUEUE);
               calc_circuit(wire_imp*0.001);
               end
             else begin
               err_mess:='Bad Name specified';
               error(0);
             end; 
           end;   

      bfs: BEGIN { breadth-first-search from a node }
             get_node(NULL,v1);x:=Address_of(v1);
             if good(x) then begin
               bfs_node(x,Show_Info,short_path);
               end
             else begin
               err_mess:='Bad Name specified';
               error(0);
             end; 
           end;   

      strng:BEGIN { depthth-first-string from a node }
              get_node(NULL,v1);x:=Address_of(v1);
              if good(x) then begin
                show_string(x);
                end
              else begin
                err_mess:='Bad Name specified';
                error(0);
              end; 
            end;   

      Apply:BEGIN { APPLY Power to a node }
             getsym;f1:=Big_a;
                                          
             getv;z1:=z;z2.a:=NULL;
             get_parm(true,z1,z2,[mVDC..VDC] ,voltage,u);
             normalize(voltage,u,voltage,u);

             get_parm(true,z1,z2,[mADC..ADC] ,current,u);
             normalize(current,u,current,u);

             get_node(z1.a,v1);x:=Address_of(v1);
             get_equiv(true,NULL,v1);
             get_node(NULL,v2);y:=Address_of(v2);
             get_equiv(true,NULL,v2);
             if good(x) and good(y) then begin
               if true  then begin   { Must check shorts }
                 if OK then Apply_Psupply(f1,voltage,u,current,x,y);
                 if OK then Operate_Power_Relays(pwrcl,f1,v1.dev,v2.dev);
                 end
               else begin
                 err_mess:=' Attempt to short power supply(s):';
                 error(1);
               end;
               end
             else begin
               err_mess:='Bad Name specified';
               error(0);
             end; 
           end;   

      remove:BEGIN { Remove Power to a node }
             getsym;f1:=Big_a;
             get_node(NULL,v1);x:=Address_of(v1);
             get_equiv(false,NULL,v1);
             get_node(NULL,v2);y:=Address_of(v2);
             get_equiv(false,NULL,v1);
             if good(x) and good(y) then begin
               Remove_Psupply(f1,x,y);
               operate_power_relays(pwrop,f1,v1.dev,v2.dev);
               end
             else begin
               err_mess:='Bad Name specified';
               error(0);
             end; 
           end;   

      opn: BEGIN { open test }
             get_node(NULL,v1);x:=Address_of(v1);
             if good(x) then begin
               open_test(x,O_params,TRUE,SHORT_OPENS);
               end
             else begin
               err_mess:='Bad Name specified';
               error(0);
             end; 
           end;   
                  
      i2n: BEGIN { address to name conversion }
             getsym;
             if number then begin
               if good(numero) then begin
                 if In_use(Numero) then begin
                   n:=int_to_name(numero);
                   write(OUTPUT,' Conn/Dev:',n.dev);
                   writeln(OUTPUT,'    Pin:',n.pin);
                   end
                 else begin
                   err_mess:=' Address is not being used';
                   error(0);
                 end;
                 end
               else begin
                 err_mess:='Bad Number specified';
                 error(0);
               end;
             end; 
           end;         

      Show:BEGIN
             Show_Results;
           end;
                  
      xfer:BEGIN { transfer command as is}
             j:=0;
             for i:=2 to ll-1 do if full_line[i]<>' ' then j:=i;
             for i:=2 to j do write_test_c(full_line[i]);
             write_test_ln;
           end;   

                               
      d  :BEGIN { display message }
               get_display(last_cc);
               put_display(d,display);
           end;   
      
      ds :BEGIN { display message }
               get_display(last_cc);
               put_display(ds,display);
           end;   
                  
      fnode:BEGIN                         
              get_node(NULL,v1);x:=Address_of(v1);
              if good(x) then begin
                if in_use(x) then begin
                  x:=f_node(x,allow,false);
                  n:=int_to_name(x);
                  write(OUTPUT,' FNODE:',n.dev,n.pin,' Allow=',allow);
                end;
                end
              else begin
                writeln(OUTPUT,'Node name expected');
              end;
            end;  
                  
      omit :BEGIN 
               while (cc<ll) and OK do begin
                 get_node(NULL,v1);
                 process_node(v1,omit_pin,false,true);
               end;
            end;  

      unomit:BEGIN 
               if cc<ll then begin
                 while (cc<ll) and OK do begin
                   get_node(NULL,v1);
                   process_node(v1,unomit_pin,false,true);
                 end;
                 end              
               else begin
                 for i:=v_min to v_max do begin
                   if inf[i]^.omit=SOFT_OMIT then inf[i]^.omit:=NO_OMIT;
                 end;
               end;
             end;  

      Hookup:BEGIN 
               if cc<ll then begin
                 while (cc<ll) and OK do begin
                   getsym;
                   Cable_Hookup(a_2);
                 end;
                 end              
               else begin
                 Cable_Hookup(NULL_2);
               end;
             end;  
                  
      fc,fci:BEGIN  
             if continue then begin
               getsym;unused:=a;
               end
             else begin
               params_1:=DC_params;
               parse_cc(Insu_DC,params_1,unused);
               if OK then  add_comm(1,params_1);
             end;     
             if (unused=' ') and OK then begin
               if OK then begin
                 f_c_tests(what=fci);   
               end;
               end
             else begin
               while (unused<>' ') and OK do begin
                 get_node(unused,v1);
                 i:=Address_of(v1);
                 FS_Search_Start:=dfs_Search;
                 process_node(v1,f_check_pin,(what=fci),false);
                 getsym;unused:=a;
               end;
             end; 
           END;   

      sc,sci:BEGIN  
             if continue then begin
               getsym;unused:=a;
               end
             else begin
               params_1:=AC_params;
               parse_cc(Insu_AC,params_1,unused);
               if OK then  add_comm(1,params_1);
             end;  
             if (unused=' ') and OK then begin
               if OK then begin
                 s_c_tests(what=sci);
               end;
               end
             else begin
               while (unused<>' ') and OK do begin
                 get_node(unused,v1);
                 FS_Search_Start:=dfs_Search;
                 process_node(v1,s_check_pin,(what=sci),false);
                 getsym;unused:=a;
               end;
             end; 
           END;   
      
         sw: BEGIN
               getsym;a1{swtch}:=Big_a;
               if symbol then begin
                 getsym;
                 if Big_a='TO' then begin
                   long_name:=' ';{remember rest of line as long_name}
                   for i:=cc to min(cc+al4,ll) do begin
                     long_name[i-cc+1]:=full_line[i];
                   end;
                   getsym;a2{position}:=a;
                   switch_state(Unknown,a1,a2,long_name,a3);
                   if OK then Actuate_Device(a1,a3,a2,long_name);
                   end
                 else if a=' ' then {spring loaded return} begin
                   long_name:=' ';
                   switch_state(Unknown,a1,NULL,long_name,a3);
                   if OK then Actuate_Device(a1,a3,NULL,long_name);
                   end
                 else begin
                   err_mess:='TO expected';error(0);
                 end;
                 end
               else begin
                 err_mess:='Switch name expected';error(0);
               end;
             END;

      pwrop,pwrcl: 
             BEGIN
               Operate_Power_Relay(what,Power_Relay);
               { if OK then Operate_PSupply(what,Power_Relay);}
             END;

      clear_all:
             BEGIN
               Clear_Tester;
             END;
            
      kb  :BEGIN  

             To_Keyboard:=true;
             From_Keyboard:=true;
           END;   

      stub:BEGIN
             getsym;unused:=a;
             while (unused<>' ') and OK do begin
               get_node(unused,v1);
               i:=Address_of(v1);
               FS_Search_Start:=dfs_Search;
               process_node(v1,stub_node,true,true);
               getsym;unused:=a;
             end; 
           END;
                             
      devs:BEGIN  
             { Read Model into memory }               
             get_parts;                                     
                 
             case target_machine of
                FACT_Machine:
                   BEGIN
                     if v_min<=0 then inf[000]^.omit:=HARD_OMIT; {CHASSIS}
                   END;
                DITMCO_660:  
                   BEGIN
                     if v_min<=0 then inf[000]^.omit:=HARD_OMIT; {CHASSIS}
                   END;
                DITMCO_9100: 
                   BEGIN
                     if v_min<=0 then inf[000]^.omit:=HARD_OMIT; {CHASSIS}
                   END;
             OTHERWISE           

             end;
           END;
      conx:BEGIN  
             get_wires;    
           END;
      exit:BEGIN                
             case target_machine of
               FACT_Machine:
                 begin
                   write_test_c('E');write_test_ln;
                 end;
               DITMCO_660:
                 begin
                   a_alfa:='*END';write_test_alfa(a_alfa,0);write_test_ln;
                   a_alfa:='END';write_test_alfa(a_alfa,0);write_test_ln;
                 end;
               DITMCO_9100:
                 begin
                   a_alfa:='*END';write_test_alfa(a_alfa,0);write_test_ln;
                   a_alfa:='END';write_test_alfa(a_alfa,0);write_test_ln;
                 end;
             end;
             Done:=true;
           END;    

      fudge:BEGIN
              getsym;
              if flo_num then begin
                r2_offset:=float;
                end
              else begin
                err_mess:='BAD Resistance offset value';
                error(0);
              end;
            END; 
                
                  
      OTHERWISE
           This_Phrase.PD:=NoPhrase;   
           get_field(This_Phrase,a,cccc);
           if OK then TARGET_code_gen(0,cccc^,Any_To);
      END;        
    end;          
end;
{**********************************************************************}
procedure Instruct;
begin
      writeln(OUTPUT,' Dataset TESSWITS.AAP        Date:05-28-92  ');
      writeln(OUTPUT,' Usage:  WITS Source_File Options');
      writeln(OUTPUT,'         Options: O = Overwrite existing Test File');
      writeln(OUTPUT,'                  E = Put errors in .ERR');
      writeln(OUTPUT,'                  W = Put wire list in .WL');
      writeln(OUTPUT,'                  D6= DITMCO 660 code generation');
      writeln(OUTPUT,'                  K = Go to Keyboard mode after test');
      writeln(OUTPUT,'                              (CRT is the TEST file)');
      writeln(OUTPUT,'                  # = Extended information');
      goto 8888;
  
end;
  
{**********************************************************************}
procedure Make_Name(IName:Name_Str;Ext:Alfa;var OName:Name_Str);
          
var i,k,l:integer;
    done:BOOLEAN;
begin
    l:=80;
    Done:=false;
    while not DONE do begin
      if l=0 then begin
        Instruct;goto 9999;
        end
      else if IName[l]=' ' then begin
        l:=l-1;   
        end
      else begin
        Done:=true;
      end;
    end{while};
    k:=l;

    done:=false;

    while not done do begin
      if k=0 then begin
        Done:=true;
        end
      Else if IName[k] ='.' then begin
        if k=1 then begin 
          Instruct;goto 9999;
        end;
        Done:=true;
        end
      Else begin
        k:=k-1;
      end;
    end;

    for i:=1 to 80 do OName[i]:=IName[i];
    If k<>0 then l:=k-1;


    OName[l+1]:='.';

    for i:=1 to al do OName[l+i+1]:=Ext[i];
 
end;

{**********************************************************************}
{***********  MAIN Procedure  *****************************************}
{**********************************************************************}
                  
begin             
                  
%include 'machine_unique.inc'


    A1:='sf';
    make_name(source_Name,A1,Source_Name);

    A1:='asc';
    make_name(source_Name,A1,Test_Name);

    A1:='wl';
    make_name(source_Name,A1,Wire_Name);

    A1:='err';
    make_name(source_Name,A1,Error_Name);

    {**Error_Name:=Console; **}
    From_Keyboard:=False;
    Go_To_Keyboard:=False;
    OverWrite:=False;
    Extra_Info:=False;
    Wire_List:=false;
    Target_Machine:=FACT_Machine;
    Lowest_F_Address:=10010;
    PPC:=100;
    PSUPPLIES:=0;
    RELAY_COUNT:=0;
    OHMS_HI:=no_node;
    OHMS_LO:=no_node;


{ Test Control Variables  }
    MAX_RESISTOR_ANNOTATE:=10;

{ Extended info variables }
    Total_Iterations:=0;
    Max_Iterations:=0;
{}


    uppers     := ['A'..'Z'];
    lowers     := ['a'..'z'];
    alphabet   := uppers + lowers;
    Numerics   := ['0'..'9'];

    i:=1;              
    while i<=80 do begin
      Case Options[i] of
        '?'    :Begin
                  {**Test_Name:=Console;**}
                end;
        '~'    :Begin
                  Debug_On := TRUE ;
                end;
        'e','E':Begin
                  Error_Option:=true;
                end;
        'k','K':Begin
                  To_Keyboard:=true;
                  Go_To_Keyboard:=True;
                  {**Test_Name:=Console;**}
                end;
        'o','O':Begin
                  OverWrite:=true;
                end;
        'w','W':Begin
                  Wire_List:=True;
                end;
        '#'    :Begin
                  Extra_Info:=True;
                end;
        'd','D':Begin
                  if Options[i+1]='6' then begin
                    Target_Machine:=DITMCO_660;
                    Lowest_F_Address:=0;{ DITMCO has no ground}
                    i:=i+1;
                    end
                  else if Options[i+1]='9' then begin
                    Target_Machine:=DITMCO_9100;
                    Lowest_F_Address:=0;{ DITMCO has no ground}
                    i:=i+1;
                    end
                  else begin
                    Writeln(OUTPUT,'Bad Option:',Options[i]);
                    goto 9999;
                  end;
                end;
 
        ' '    :;

      Otherwise
                Writeln(OUTPUT,'Bad Option:',Options[i]);
                goto 9999;
      end;
      i:=i+1; 
    end;
       
    Test_Line.length:=0;           
    Annotation.length:=0;


    BAD_GUY:=false;
    Wait_Manual:=false;
    Pin_Seq_Mode:=Normal_Seq;
    wc_mode:=wc_first;
    add_wc:=true;
    Old_Wish:=No_Command;
    Display_lines:=0;
    Force_XC:=true;
    XC_tested:=false;
    r2_offset:=0.0;

{ Open files }    
    new(source_files);                
%include 'open_source.inc'
    Source_files^.next:=NIL;
    Source_files^.include_name:=source_name;
    Reset(Source_Files^.include_file);

%include 'open_files.inc'

{ Initialize pointers  }
    new(z_node); z_node^.next:=z_node ; z_node^.prev:=z_node ;
                  
    new(z_info);  

{ now get a z_dev and make first dev f_dev point to it. Also get a first
    pin f_pin and make f_dev first pin and f_dev sentinel pin (f_dev.z_p)
    point to it. This ensures that Address_Of routine will find NO
    connectors and pins unless some has been put in there. }
                  
    new(z_dev);new(z_dev^.sub_dev);z_dev^.sub_dev:=z_dev;
    f_dev:=z_dev;new(f_pin);z_dev^.p:=f_pin;
    z_dev^.z_p:=f_pin;f_pin^.pin:='?';f_pin^.i_add:=Bad_Address;
    f_pin^.p:=NIL;
    new(cccc);cccc^.next:=NIL;

    New(Range_List);Range_Pin:=Range_List;
    Range_List^.next:=NIL;Range_List^.Valid:=False;

    Next_TB_Address:=-1;
    
{ Adapter variables }
    Adapter:=NIL;
    STRIP:=false;
    No_At_Field:=false;
    ZIF:=false;

    V_min:=1;     
    V_Max:=0;     
    dfs_Search:=0; 
    bfs_Search:=0; 
    
{ QUEUES }
    MAKE_QUEUE(bfs_QUEUE);MAKE_QUEUE(SUB_QUEUE);
    MAKE_QUEUE(RELAY_QUEUE);MAKE_QUEUE(live_QUEUE);
    MAKE_QUEUE(x_QUEUE);MAKE_QUEUE(y_QUEUE);
    MAKE_TEST(OPENS_QUEUE);MAKE_TEST(RESIS_QUEUE);
    MAKE_TEST(RATIO_QUEUE);

    MAKE_QUEUE(temp_QUEUE);
    MAKE_QUEUE(PS_QUEUE);
    ADAPTER:=NIL;

    Done:=false;

    Error_Count:=0;Warn_count:=0;

    for i:=1 to al  do NULL[i]:=chr(0);
    for i:=1 to al2 do NULL_2[i]:=chr(0);
                 
{ Read Model into memory }               
    Section_Message:='Incomplete Connectors/Parts section';
    Acceptable:=['A'..'Z','a'..'z','0'..'9','*','+','.','/','_'];
    Section:=Parts_Section;
 
    cc:=0;ll:=0;ch:=' ';kk:=al;
    curr_add:=1;
    get_parts;                                     

    case target_machine of
      FACT_Machine:BEGIN
                     if v_min<=0 then inf[000]^.omit:=HARD_OMIT; {CHASSIS}
                   END;
      DITMCO_660:  BEGIN
                     if v_min<=0 then inf[000]^.omit:=HARD_OMIT; {CHASSIS}
                   END;
      DITMCO_9100: BEGIN
                     if v_min<=0 then inf[000]^.omit:=HARD_OMIT; {CHASSIS}
                   END;
      OTHERWISE           

    end;

                  
    Section_Message:='Incomplete Wire list section';
    Acceptable:=['A'..'Z','a'..'z','0'..'9','*','+','.','/','_'];
    Section:=Wires_Section;
    get_wires;    

    Section_Message:='Incomplete Test section';
    Acceptable:=['A'..'Z','a'..'z','0'..'9','*','+','.','/','_','-','%'];
    Section:=Tests_Section;              
{ set defaults }  
                  
    Q_value:=-1.0;
                  
    With Diode_F DO Begin
      PT:=CommPhrase ;comm:=Cont_C;relop:=le;
      v:=300.0    ;  u:= Ohm  ; v1:=10.0    ;  u1:=mADC ;
      v2:=-1.0   ;  u2:=Sec ;
    end;          

    With Diode_R DO Begin
      PT:=CommPhrase ;comm:=Open_C;relop:=ge;
      v:=900.0   ;  u:=KOhm  ; v1:=10.0    ;  u1:=mADC ;
      v2:=-1.0   ;  u2:=Sec ;
    end;          

    With C_Params DO Begin
      PT:=CommPhrase ;comm:=Cont_C;relop:=le;
      v:=2.0     ;  u:= Ohm  ; v1:=0.5    ;  u1:=ADC ;
      v2:=-1.0   ;  u2:=Sec ;
    end;          
                                                       
    With O_Params DO Begin
      PT:=CommPhrase ;comm:=Open_C;relop:=ge;
      v:=900.0   ;  u:=KOhm  ; v1:=0.5    ;  u1:=ADC ;
      v2:=-1.0   ;  u2:=Sec ;
    end;          

    With DC_Params DO Begin
      PT:=CommPhrase ;comm:=Insu_DC;relop:=ge;
      v:=1000.0  ;u:=VDC     ; v1:=500.0  ;  u1:=MOhm ;
      v2:=-1.0   ;u2:=BadUnit; v3:=2.0    ;  u3:=Sec  ;
    end;          

    With AC_Params DO Begin
      PT:=CommPhrase ;comm:=Insu_AC;relop:=ge;
      v:=1500.0  ;  u:=VAC   ; v1:=-1.0   ;  u1:=ZeroCross;
      v2:=1.0    ;  u2:=mAAC ; v3:=5.0    ;  u3:=Sec  ;
    end;               
          
    done:=false;  
    
    interpret;

    if From_Keyboard then begin
      end
    else if Go_To_Keyboard then begin
      Done:=false;
      From_Keyboard:=true;
      To_Keyboard:=true;
      Interpret;
    end;          

    if (error_count=0) and (not To_Keyboard) then show_results;
    if Extra_Info then Show_Extras;                  

9999:
    if error_count=0 then begin         
      { Generate Connector List if required }

      case target_machine of
        FACT_machine :
              Begin
                if Wire_List then begin
                  Generate_Wire_List(wire_file);
                end;
              end;
        DITMCO_660:
              Begin
                Generate_Wire_List(test_file);
              end;
      OTHERWISE
      end;

    end;
                  
    if error_count>0 then writeln(OUTPUT,' Total Errors:',Error_Count);
    if Warn_count>0 then writeln(OUTPUT,' Total Warnings:',Warn_Count);
                  
8888: { just get out }
end.

              
