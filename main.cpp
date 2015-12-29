#include "LR1PG.h"
#include "Lexer.h"
#include "Analyser.h"
#include <stack>


namespace CM_attri_gram_set
{
//add your attribute grammers below:
//(suggested name form: agx)
		Analyser::attribute ag3(const vector<Analyser::ex_element>& r_part)
		{
		/*ag3: declar ¡ú declar_var
		{
			nesting_table::nstack_top().var_map[declar_var.name]=nesting_table::ostack_top();
			nesting_table::ostack_top()+=declar_var.width;
			global_memory::alloc(declar_var.width);

		}*/
			string name=r_part[0].get_str("name");
			int width=r_part[0].get_int("width");


			Analyser::global_memory::alloc(width);
			Analyser::nesting_table::nstack_top()->var_map[name]=Analyser::nesting_table::ostack_top();
			Analyser::nesting_table::ostack_top()+=width;

			
			return Analyser::attribute();
		}
		Analyser::attribute ag5(const vector<Analyser::ex_element>& r_part)
		{
		/*ag5: declar_var ¡ú type_var ID ;
		{
			declar_var.width=1;
			declar_var.name=ID.name;
		}*/
		//in CM, there is not other types except int
		//the declar_var.type attribute is not used here
		//and the memory unit is int 
			Analyser::attribute attri;
			
			attri.set_value("name",r_part[1].get_str("name"));
			attri.set_value("width",1);
			
			return attri;
		}
		Analyser::attribute ag6(const vector<Analyser::ex_element>& r_part)
		{
		/*ag6: declar_var ¡ú type_var ID [ NUM ] ;
		{
			declar_var.width=1*NUM.val;
			declar_var.name=ID.name;
		}*/
		//in CM, there is not other types except int
		//the declar_var.type attribute is not used here
		//and the memory unit is int 

			Analyser::attribute attri;
			
			//ID.width=1
			//val.setIntValue(0,0,v.getIntValue(1,0));
			attri.set_value("name",r_part[1].get_str("name"));
			attri.set_value("width",1*r_part[3].get_int("val"));
			
			return attri;
		}
		Analyser::attribute ag19(const vector<Analyser::ex_element>& r_part)
		{
		/*ag19: compou_body ¡ú { N1 declar_local code }
		{
			t=nesting_table::nstack_top();
			global_memory::alloc(nesting_table::ostack_top());
			nesting_table::nstack_pop();
			nesting_table::ostack_pop();
			enterproc(nesting_table::nstack_pop(),t);

			compou_body.nextlist=code.nextlist;
		}*/

			Analyser::nesting_table* pt=Analyser::nesting_table::nstack_top();
			Analyser::global_memory::alloc(Analyser::nesting_table::ostack_top());
			Analyser::nesting_table::nstack_pop();
			Analyser::nesting_table::ostack_pop();
			
			Analyser::nesting_table::nstack_top()->nes_body_list.push_back(pt);

			
			Analyser::attribute attri;
			attri.set_value("nextlist",r_part[3].get_int("nextlist"));
			
			
			return attri;
		}

		Analyser::attribute ag29(const vector<Analyser::ex_element>& r_part)
		{
		/*ag29: lines ¡ú compou_body
		{
			lines.nextlist=compou_body.nextlist;
		}*/
			
			Analyser::attribute attri;
			attri.set_value("nextlist",r_part[0].get_int("nextlist"));
			
			
			return attri;
		}


		Analyser::attribute ag20(const vector<Analyser::ex_element>& r_part)
		{
		/*ag20: N1 ¡ú ¦Å
		{
			nesting_table* pt=new nesting_table(Analyser::global_memory::get_ptr(),Analyser::nesting_table::nstack_top());
			nesting_table::nstack_push(pt);
			nesting_table::ostack_push(0);
		}*/
			Analyser::nesting_table* pt=new Analyser::nesting_table(Analyser::global_memory::get_ptr(),Analyser::nesting_table::nstack_top());
			Analyser::nesting_table::nstack_push(pt);
			Analyser::nesting_table::ostack_push(0);

			return Analyser::attribute();
		}

		Analyser::attribute ag21(const vector<Analyser::ex_element>& r_part)
		{
		/*ag21: declar_local ¡ú declar_local declar_var
		{
			enter(declar_var.name,offset);
			offset=offset+declar_var.width;
			enter(nesting_table::nstack_top(),declar_var.name,nesting_table::ostack_top());
			nesting_table::ostack_top()+=declar_var.width;
		}*/
		//in CM, there is not other types except int
		//the declar_var.type attribute is not used here
			string name=r_part[1].get_str("name");
			int width=r_part[1].get_int("width");

			Analyser::global_memory::alloc(width);
			Analyser::nesting_table::nstack_top()->var_map[name]=Analyser::nesting_table::ostack_top();
			Analyser::nesting_table::ostack_top()+=width;
			
			//it does not need to return value
			return Analyser::attribute();
		}

		Analyser::attribute ag53(const vector<Analyser::ex_element>& r_part)
		{
		/*ag53: op_add ¡ú +
		{
			op_add.type='+';
		}*/
			Analyser::attribute attri;
			
			attri.set_value(string("type"),string("+"));
			return attri;
		}
		Analyser::attribute ag54(const vector<Analyser::ex_element>& r_part)
		{
		/*ag54: op_add ¡ú -
		{
			op_add.type='-';
		}*/
			Analyser::attribute attri;
			
			attri.set_value(string("type"),string("-"));
			return attri;
		}
		Analyser::attribute ag57(const vector<Analyser::ex_element>& r_part)
		{
		/*ag57: op_mul ¡ú *
		{
			op_add.type='*';
		}*/
			Analyser::attribute attri;
			
			attri.set_value(string("type"),string("*"));
			return attri;
		}
		Analyser::attribute ag58(const vector<Analyser::ex_element>& r_part)
		{
		/*ag58: op_mul ¡ú /
		{
			op_add.type='/';
		}*/
			Analyser::attribute attri;
			
			attri.set_value(string("type"),string("/"));
			return attri;
		}
		Analyser::attribute ag45(const vector<Analyser::ex_element>& r_part)
		{
		/*ag45: op_rel ¡ú <=
		{
			op_add.type='<=';
		}*/
			Analyser::attribute attri;
			
			attri.set_value(string("type"),string("<="));
			return attri;
		}
		Analyser::attribute ag46(const vector<Analyser::ex_element>& r_part)
		{
		/*ag46: op_rel ¡ú <
		{
			op_add.type='<';
		}*/
			Analyser::attribute attri;
			
			attri.set_value(string("type"),string("<"));
			return attri;
		}
		Analyser::attribute ag47(const vector<Analyser::ex_element>& r_part)
		{
		/*ag47: op_rel ¡ú >
		{
			op_add.type='>';
		}*/
			Analyser::attribute attri;
			
			attri.set_value(string("type"),string(">"));
			return attri;
		}
		Analyser::attribute ag48(const vector<Analyser::ex_element>& r_part)
		{
		/*ag48: op_rel ¡ú >=
		{
			op_add.type='>=';
		}*/
			Analyser::attribute attri;
			
			attri.set_value(string("type"),string(">="));
			return attri;
		}
		Analyser::attribute ag49(const vector<Analyser::ex_element>& r_part)
		{
		/*ag49: op_rel ¡ú ==
		{
			op_add.type='==';
		}*/
			Analyser::attribute attri;
			
			attri.set_value(string("type"),string("=="));
			return attri;
		}
		Analyser::attribute ag50(const vector<Analyser::ex_element>& r_part)
		{
		/*ag50: op_rel ¡ú !=
		{
			op_add.type='!=';
		}*/
			Analyser::attribute attri;
			
			attri.set_value(string("type"),string("!="));
			return attri;
		}
		Analyser::attribute ag51(const vector<Analyser::ex_element>& r_part)
		{
		/*ag51: expre_arith ¡ú expre_arith1 op_add term
		{
			global_memory::alloc(1);
			nesting_table::nstack_top().var_map[Tx]=nesting_table::ostack_top();
			nesting_table::ostack_top()+=1;

			emit(op_add.type,expre_arith1.global_ptr,term.global_ptr,Tx.global_ptr);
			expre_arith.global_ptr=nesting_table::nstack_top().get_global_ptr(Tx);
			
		}*/
			
			
			Analyser::global_memory::alloc(1);
			string Tx=Analyser::Tx_allocator::generate_str();
			Analyser::nesting_table::nstack_top()->var_map[Tx]=Analyser::nesting_table::ostack_top();
			Analyser::nesting_table::ostack_top()+=1;

			
			printf("(%s,#%d,#%d,#%d)\n",r_part[1].get_str("type").c_str(),r_part[0].get_int("global_ptr"),r_part[2].get_int("global_ptr"),Analyser::nesting_table::nstack_top()->get_global_ptr(Tx));
			
			char op[100];
			char s1[100];
			char s2[100];
			char tar[100];
			sprintf(op,"%s",r_part[1].get_str("type").c_str());
			sprintf(s1,"#%d",r_part[0].get_int("global_ptr"));
			sprintf(s2,"#%d",r_part[2].get_int("global_ptr"));
			sprintf(tar,"#%d",Analyser::nesting_table::nstack_top()->get_global_ptr(Tx));
			Analyser::next_list::create_quad(op,s1,s2,tar);
			
			Analyser::attribute attri;
			attri.set_value("global_ptr",Analyser::nesting_table::nstack_top()->get_global_ptr(Tx));
			
			return attri;
		}
		Analyser::attribute ag39(const vector<Analyser::ex_element>& r_part)
		{
		/*ag39: expre ¡ú var = expre1
		{
			emit(=,expre1.global_ptr,-,var.global_ptr);
			expre.global_ptr=var.global_ptr;

			expre.truelist=makelist(nextquad);
			expre.falselist=makelist(nextquad);
			emit(jnz,expre1.global_ptr,-,-);
			emit(j,-,-,-);
			
		}*/
			printf("(=,#%d,-,#%d)\n",r_part[2].get_int(string("global_ptr")),r_part[0].get_int(string("global_ptr")));

			
			char s1[100];
			char s2[100];
			char tar[100];
			sprintf(s1,"#%d",r_part[2].get_int(string("global_ptr")));
			sprintf(tar,"#%d",r_part[0].get_int(string("global_ptr")));
			Analyser::next_list::create_quad("=",s1,"-",tar);
			
			Analyser::attribute attri;
			attri.set_value("global_ptr",r_part[0].get_int(string("global_ptr")));

			attri.set_value("truelist",(int)(new Analyser::next_list(Analyser::next_list::get_nextquad())));
			attri.set_value("falselist",(int)(new Analyser::next_list(Analyser::next_list::get_nextquad()+1)));
			
			printf("(jnz,#%d,-,-)\n",r_part[0].get_int(string("global_ptr")));
			sprintf(s1,"#%d",r_part[0].get_int(string("global_ptr")));
			Analyser::next_list::create_quad("jnz",s1,"-","-");

			printf("(j,-,-,-)\n");
			Analyser::next_list::create_quad("j","-","-","-");



			return attri;
		}
		
		Analyser::attribute ag42(const vector<Analyser::ex_element>& r_part)
		{
		/*ag42: var ¡ú ID [ expre ]
		{
			
			global_memory::alloc(1);
			nesting_table::nstack_top().var_map[Tx]=nesting_table::ostack_top();
			nesting_table::ostack_top()+=1;

			emit([],ID.global_ptr,expre.global_ptr,Tx);
			nesting_table::nstack_top().var_map["#Tx"]=nesting_table::ostack_top();
			var.global_ptr=#Tx.global_ptr;
		}*/
			Analyser::global_memory::alloc(1);
			string Tx=Analyser::Tx_allocator::generate_str();
			Analyser::nesting_table::nstack_top()->var_map[Tx]=Analyser::nesting_table::ostack_top();
			Analyser::nesting_table::ostack_top()+=1;


			printf("([],#%d,#%d,#%d\n)",Analyser::nesting_table::nstack_top()->get_global_ptr(r_part[0].get_str(string("name"))),r_part[2].get_int(string("global_ptr")),Analyser::nesting_table::nstack_top()->get_global_ptr(Tx));

			
			char s1[100];
			char s2[100];
			char tar[100];
			sprintf(s1,"#%d",Analyser::nesting_table::nstack_top()->get_global_ptr(r_part[0].get_str(string("name"))));
			sprintf(s2,"#%d",r_part[2].get_int(string("global_ptr")));
			sprintf(tar,"#%d",Analyser::nesting_table::nstack_top()->get_global_ptr(Tx));
			Analyser::next_list::create_quad("#",s1,s2,tar);

			Analyser::attribute attri;
			attri.set_value(string("global_ptr"),Analyser::nesting_table::nstack_top()->get_global_ptr(Tx));
			
			return attri;
		}

		Analyser::attribute ag41(const vector<Analyser::ex_element>& r_part)
		{
		/*ag41: var ¡ú ID
		{
			var.global_ptr=ID.global_ptr;
		}*/
			Analyser::attribute attri;
			attri.set_value(string("global_ptr"),Analyser::nesting_table::nstack_top()->get_global_ptr(r_part[0].get_str("name")));
			return attri;
		}
		Analyser::attribute ag52(const vector<Analyser::ex_element>& r_part)
		{
		/*ag52: expre_arith ¡ú term
		{
			expre_arith.global_ptr=expre_arith.global_ptr;
		}*/
			Analyser::attribute attri;
			attri.set_value(string("global_ptr"),r_part[0].get_int(string("global_ptr")));
			return attri;
		}
		Analyser::attribute ag56(const vector<Analyser::ex_element>& r_part)
		{
		/*ag56: term ¡ú factor
		{
			term.global_ptr=factor.global_ptr;
		}*/
			Analyser::attribute attri;
			attri.set_value(string("global_ptr"),r_part[0].get_int(string("global_ptr")));
			return attri;
		}
		Analyser::attribute ag60(const vector<Analyser::ex_element>& r_part)
		{
		/*ag60: factor ¡ú var
		{
			factor.global_ptr=var.global_ptr;
		}*/
			Analyser::attribute attri;
			attri.set_value(string("global_ptr"),r_part[0].get_int(string("global_ptr")));
			return attri;
		}
		Analyser::attribute ag59(const vector<Analyser::ex_element>& r_part)
		{
		/*ag59: factor ¡ú ( expre )
		{
			factor.global_ptr=expre.global_ptr;
		}*/
			Analyser::attribute attri;
			attri.set_value(string("global_ptr"),r_part[1].get_int(string("global_ptr")));
			return attri;
		}
		Analyser::attribute ag44(const vector<Analyser::ex_element>& r_part)
		{
		/*ag44: expre_rel ¡ú expre_arith
		{
			expre_rel.global_ptr=expre_arith.global_ptr;
		}*/
			Analyser::attribute attri;
			attri.set_value(string("global_ptr"),r_part[0].get_int(string("global_ptr")));
			return attri;
		}
		Analyser::attribute ag40(const vector<Analyser::ex_element>& r_part)
		{
		/*ag40: expre ¡ú expre_rel
		{
			expre.global_ptr=expre_rel.global_ptr;

			expre.truelist=makelist(nextquad);
			expre.falselist=makelist(nextquad);
			emit(jnz,expre_rel.global_ptr,-,-);
			emit(j,-,-,-);
		}*/
			Analyser::attribute attri;
			attri.set_value(string("global_ptr"),r_part[0].get_int(string("global_ptr")));
			

			Analyser::next_list* p1=new Analyser::next_list(Analyser::next_list::get_nextquad());
			Analyser::next_list* p2=new Analyser::next_list(Analyser::next_list::get_nextquad()+1);

			attri.set_value("truelist",(int)p1);
			attri.set_value("falselist",(int)p2);
			
			printf("(jnz,#%d,-,-)\n",r_part[0].get_int(string("global_ptr")));
			char s1[100];
			sprintf(s1,"#%d",r_part[0].get_int(string("global_ptr")));
			Analyser::next_list::create_quad("jnz",s1,"-","-");

			printf("(j,-,-,-)\n");
			Analyser::next_list::create_quad("j","-","-","-");
			
			return attri;


		}
		Analyser::attribute ag55(const vector<Analyser::ex_element>& r_part)
		{
		/*ag55: term ¡ú term1 op_mul factor
		{
			global_memory::alloc(1);
			nesting_table::nstack_top().var_map[Tx]=nesting_table::ostack_top();
			nesting_table::ostack_top()+=1;

			emit(op_mul.type,term1.global_ptr,factor.global_ptr,Tx.global_ptr);
			term.global_ptr=nesting_table::nstack_top().get_global_ptr(Tx);
			
		}*/
			
			
			Analyser::global_memory::alloc(1);
			string Tx=Analyser::Tx_allocator::generate_str();
			Analyser::nesting_table::nstack_top()->var_map[Tx]=Analyser::nesting_table::ostack_top();
			Analyser::nesting_table::ostack_top()+=1;

			printf("(%s,#%d,#%d,#%d)\n",r_part[1].get_str("type"),r_part[0].get_int("global_ptr"),r_part[2].get_int("global_ptr"),Analyser::nesting_table::nstack_top()->get_global_ptr(Tx));
			
			
			char op[100];
			char s1[100];
			char s2[100];
			char tar[100];
			sprintf(op,"%s",r_part[1].get_str("type").c_str());
			sprintf(s1,"#%d",r_part[0].get_int("global_ptr"));
			sprintf(s2,"#%d",r_part[2].get_int("global_ptr"));
			sprintf(tar,"#%d",Analyser::nesting_table::nstack_top()->get_global_ptr(Tx));
			Analyser::next_list::create_quad(op,s1,s2,tar);

			Analyser::attribute attri;
			attri.set_value("global_ptr",Analyser::nesting_table::nstack_top()->get_global_ptr(Tx));
			
			return attri;
		}
		Analyser::attribute ag43(const vector<Analyser::ex_element>& r_part)
		{
		/*ag43: expre_rel ¡ú expre_arith op_rel expre_arith1
		{
			global_memory::alloc(1);
			nesting_table::nstack_top().var_map[Tx]=nesting_table::ostack_top();
			nesting_table::ostack_top()+=1;

			emit(op_rel.type,expre_arith.global_ptr,expre_arith1.global_ptr,Tx.global_ptr);
			expre_rel.global_ptr=nesting_table::nstack_top().get_global_ptr(Tx);
			
		}*/
			
			
			Analyser::global_memory::alloc(1);
			string Tx=Analyser::Tx_allocator::generate_str();
			Analyser::nesting_table::nstack_top()->var_map[Tx]=Analyser::nesting_table::ostack_top();
			Analyser::nesting_table::ostack_top()+=1;

			printf("(%s,#%d,#%d,#%d)\n",r_part[1].get_str("type"),r_part[0].get_int("global_ptr"),r_part[2].get_int("global_ptr"),Analyser::nesting_table::nstack_top()->get_global_ptr(Tx));
			
			char op[100];
			char s1[100];
			char s2[100];
			char tar[100];
			sprintf(op,"%s",r_part[1].get_str("type").c_str());
			sprintf(s1,"#%d",r_part[0].get_int("global_ptr"));
			sprintf(s2,"#%d",r_part[2].get_int("global_ptr"));
			sprintf(tar,"#%d",Analyser::nesting_table::nstack_top()->get_global_ptr(Tx));
			Analyser::next_list::create_quad(op,s1,s2,tar);

			Analyser::attribute attri;
			attri.set_value("global_ptr",Analyser::nesting_table::nstack_top()->get_global_ptr(Tx));
			
			return attri;
		}
		Analyser::attribute ag62(const vector<Analyser::ex_element>& r_part)
		{
		/*ag62: factor ¡ú NUM
		{
			global_memory::alloc(1);
			nesting_table::nstack_top().var_map[Tx]=nesting_table::ostack_top();
			nesting_table::ostack_top()+=1;

			emit(=,NUM.val,-,Tx);
			factor.global_ptr=nesting_table::nstack_top().get_global_ptr(Tx);
		}*/
			Analyser::global_memory::alloc(1);
			string Tx=Analyser::Tx_allocator::generate_str();
			Analyser::nesting_table::nstack_top()->var_map[Tx]=Analyser::nesting_table::ostack_top();
			Analyser::nesting_table::ostack_top()+=1;
			
			printf("(=,%d,-,#%d)\n",r_part[0].get_int("val"),Analyser::nesting_table::nstack_top()->get_global_ptr(Tx));
			
			
			char s1[100];
			char tar[100];
			sprintf(s1,"%d",r_part[0].get_int("val"));
			sprintf(tar,"#%d",Analyser::nesting_table::nstack_top()->get_global_ptr(Tx));
			Analyser::next_list::create_quad("=",s1,"-",tar);

			Analyser::attribute attri;
			//for the purpose of generilization, we "allocate space" for immediate number
			attri.set_value("global_ptr",Analyser::nesting_table::nstack_top()->get_global_ptr(Tx));
			
			return attri;
		}




		Analyser::attribute ag36(const vector<Analyser::ex_element>& r_part)
		{
		/*ag36: M ¡ú ¦Å
		{
			M.quad=nextquad;
		}*/
			Analyser::attribute attri;
			attri.set_value("quad",Analyser::next_list::get_nextquad());


			return attri;
		}

		Analyser::attribute ag35(const vector<Analyser::ex_element>& r_part)
		{
		/*ag35: N2 ¡ú ¦Å
		{
			N2.nextlist=makelist(nextquad);
			emit(j,-,-,-);
		}*/
			Analyser::attribute attri;
			attri.set_value("nextlist",(int)(new Analyser::next_list(Analyser::next_list::get_nextquad())));
			printf("(j,-,-,-)\n");
			Analyser::next_list::create_quad("j","-","-","-");

			return attri;
		}

		Analyser::attribute ag23(const vector<Analyser::ex_element>& r_part)
		{
		/*ag23: code ¡ú code1 M lines
		{
			backpatch(code1.nextlist,M.quad);
			code.nextlist=lines.nextlist;
		}*/
			
			Analyser::next_list* p=(Analyser::next_list*)(r_part[0].get_int("nextlist"));
			int quad=r_part[1].get_int("quad");
			Analyser::next_list::back_patch(p,quad);

			Analyser::attribute attri;
			attri.set_value("nextlist",(int)(r_part[2].get_int("nextlist")));
			return attri;
		}
		Analyser::attribute ag25(const vector<Analyser::ex_element>& r_part)
		{
		/*ag25: lines ¡ú lines_asg
		{
			lines.nextlist=makelist();
		}*/
			Analyser::attribute attri;
			//using NULL means it does not need to be backpatched
			attri.set_value("nextlist",NULL);
			return attri;
		}
		Analyser::attribute ag28(const vector<Analyser::ex_element>& r_part)
		{
		/*ag28: lines ¡ú lines_return
		{
			lines.nextlist=makelist();
		}*/
			Analyser::attribute attri;
			//using NULL means it does not need to be backpatched
			attri.set_value("nextlist",NULL);
			return attri;
		}
		Analyser::attribute ag26(const vector<Analyser::ex_element>& r_part)
		{
		/*ag26: lines ¡ú lines_select
		{
			lines.nextlist=lines_selct.nextlist;
		}*/
			Analyser::attribute attri;
			//using NULL means it does not need to be backpatched
			attri.set_value("nextlist",r_part[0].get_int("nextlist"));
			return attri;
		}
		Analyser::attribute ag27(const vector<Analyser::ex_element>& r_part)
		{
		/*ag27: lines ¡ú lines_loop
		{
			lines.nextlist=lines_selct.nextlist;
		}*/
			Analyser::attribute attri;
			//using NULL means it does not need to be backpatched
			attri.set_value("nextlist",r_part[0].get_int("nextlist"));
			return attri;
		}

		Analyser::attribute ag32(const vector<Analyser::ex_element>& r_part)
		{
		/*ag32: lines_select ¡ú if ( expre ) M lines
		{
			backpatch(expre.truelist,M.quad);
			lines_select.nextlist=merge(expre.falselist,lines.nextlist);
		}*/
			Analyser::next_list* truelist=(Analyser::next_list*)(r_part[2].get_int("truelist"));
			Analyser::next_list::back_patch(truelist,r_part[4].get_int("quad"));
			
			Analyser::next_list* falselist=(Analyser::next_list*)(r_part[2].get_int("falselist"));
			Analyser::next_list* nextlist=(Analyser::next_list*)(r_part[5].get_int("nextlist"));
			Analyser::attribute attri;
			attri.set_value("nextlist",(int)(Analyser::next_list::merge(falselist,nextlist)));
			return attri;
		}
		Analyser::attribute ag33(const vector<Analyser::ex_element>& r_part)
		{
		/*ag33: lines_select ¡ú if ( expre ) M lines N2 else M1 lines1
		{
			backpatch(expre.truelist,M.quad);
			backpatch(expre.falselist,M1.quad);
			lines_select.nextlist=merge(lines.nextlist,N2.nextlist,lines1.nextlist);
		}*/

			Analyser::next_list* truelist=(Analyser::next_list*)(r_part[2].get_int("truelist"));
			Analyser::next_list::back_patch(truelist,r_part[4].get_int("quad"));
			
			Analyser::next_list* falselist=(Analyser::next_list*)(r_part[2].get_int("falselist"));
			Analyser::next_list::back_patch(truelist,r_part[8].get_int("quad"));
			
			Analyser::next_list* lines_nextlist=(Analyser::next_list*)(r_part[5].get_int("nextlist"));
			Analyser::next_list* N2_nextlist=(Analyser::next_list*)(r_part[6].get_int("nextlist"));
			Analyser::next_list* lines1_nextlist=(Analyser::next_list*)(r_part[9].get_int("nextlist"));
			Analyser::next_list* temp_ptr= Analyser::next_list::merge(lines_nextlist,N2_nextlist);
			Analyser::attribute attri;
			attri.set_value("nextlist",(int)(Analyser::next_list::merge(temp_ptr,lines1_nextlist)));
			return attri;
		}
		Analyser::attribute ag34(const vector<Analyser::ex_element>& r_part)
		{
		/*ag34: lines_loop ¡ú while M ( expre ) M1 lines
		{
			backpatch(lines.nextlist,M.quad);
			backpatch(expre.truelist,M1.quad);
			lines_loop.nextlist=expre.falselist;
			emit(j,-,-,M.quad);
		}*/

			
			Analyser::next_list* lines_nextlist=(Analyser::next_list*)(r_part[6].get_int("nextlist"));
			Analyser::next_list::back_patch(lines_nextlist,r_part[1].get_int("quad"));
			
			Analyser::next_list* truelist=(Analyser::next_list*)(r_part[3].get_int("truelist"));
			Analyser::next_list::back_patch(truelist,r_part[5].get_int("quad"));
			
			Analyser::attribute attri;
			attri.set_value("nextlist",(int)((Analyser::next_list*)(r_part[3].get_int("nextlist"))));

			printf("(j,-,-,%d)",r_part[1].get_int("quad"));
			char tar[100];
			sprintf(tar,"%d",r_part[1].get_int("quad"));
			Analyser::next_list::create_quad("j","-","-",tar);
			return attri;
		}

		//without attribute grammer:
		Analyser::attribute ag22(const vector<Analyser::ex_element>& r_part)
		{
		/*ag21: declar_local ¡ú ¦Å
		{
		}*/
			//it does not need to return value
			return Analyser::attribute();
		}
		Analyser::attribute ag24(const vector<Analyser::ex_element>& r_part)
		{
		/*ag21: code ¡ú ¦Å
		{
		}*/
			//it does not need to return value
			return Analyser::attribute();
		}

}


class CM_LR_analyser:public Analyser::LR_analyser
{
public:
	void conflict_management(const list<Analyser::ex_element>& elem_stream,const list<Analyser::ex_element>::const_iterator& ptr,LR1PG::action& act)override
	{
			//add conflict management meotheds here
			if((LR_stack.top().state_index==8)&&(ptr->toString()=="ID"))
			{
			//Conflicts at (I8,ID):r7 vs. r9
				list<Analyser::ex_element>::const_iterator it=ptr;
				it++;
				
				if((it!=elem_stream.end())&&((it->toString())=="("))
					act.index=9;
				else
					act.index=7;
			}
			if(LR_stack.top().state_index==307 && (ptr->toString()=="else"))
			{
			//Conflicts at (I307,else):r32 vs. r35
				act.type=LR1PG::action_type::reduction;
				act.index=35;
			}
	}
}CM_analyser;


void main()
{
	
	//to get var_list, ter_list, produc_set and set_C in LR1PG
	//and var_list, ter_list, production_set in LR_analyser
	CM_analyser.load_productions(string("M_wenfa.txt"));

	for(int i=0;i<LR1PG::produc_set.size();i++)
	{
		LR1PG::produc_set[i];
		
		Analyser::ex_production produc(LR1PG::produc_set[i]);
		Analyser::ex_produc_set.push_back(produc);
	}
	{
		Analyser::ex_production::add_gram(&CM_attri_gram_set::ag5,5);
		Analyser::ex_produc_set[5].set_gram(5);
		
		Analyser::ex_production::add_gram(&CM_attri_gram_set::ag6,6);
		Analyser::ex_produc_set[6].set_gram(6);
		
		Analyser::ex_production::add_gram(&(CM_attri_gram_set::ag19),19);
		Analyser::ex_produc_set[19].set_gram(19);
		
		Analyser::ex_production::add_gram(&(CM_attri_gram_set::ag20),20);
		Analyser::ex_produc_set[20].set_gram(20);
		
		Analyser::ex_production::add_gram(&(CM_attri_gram_set::ag21),21);
		Analyser::ex_produc_set[21].set_gram(21);

		Analyser::ex_production::add_gram(&(CM_attri_gram_set::ag22),22);
		Analyser::ex_produc_set[22].set_gram(22);

		Analyser::ex_production::add_gram(&(CM_attri_gram_set::ag23),23);
		Analyser::ex_produc_set[23].set_gram(23);

		Analyser::ex_production::add_gram(&(CM_attri_gram_set::ag24),24);
		Analyser::ex_produc_set[24].set_gram(24);
		
		Analyser::ex_production::add_gram(&(CM_attri_gram_set::ag25),25);
		Analyser::ex_produc_set[25].set_gram(25);
		
		Analyser::ex_production::add_gram(&(CM_attri_gram_set::ag26),26);
		Analyser::ex_produc_set[26].set_gram(26);
		
		Analyser::ex_production::add_gram(&(CM_attri_gram_set::ag27),27);
		Analyser::ex_produc_set[27].set_gram(27);
		
		Analyser::ex_production::add_gram(&(CM_attri_gram_set::ag28),28);
		Analyser::ex_produc_set[28].set_gram(28);

		Analyser::ex_production::add_gram(&(CM_attri_gram_set::ag29),29);
		Analyser::ex_produc_set[29].set_gram(29);
		
		Analyser::ex_production::add_gram(&(CM_attri_gram_set::ag32),32);
		Analyser::ex_produc_set[32].set_gram(32);
		
		Analyser::ex_production::add_gram(&(CM_attri_gram_set::ag33),33);
		Analyser::ex_produc_set[33].set_gram(33);
		
		Analyser::ex_production::add_gram(&(CM_attri_gram_set::ag34),34);
		Analyser::ex_produc_set[34].set_gram(34);
		
		Analyser::ex_production::add_gram(&(CM_attri_gram_set::ag35),35);
		Analyser::ex_produc_set[35].set_gram(35);
		
		Analyser::ex_production::add_gram(&(CM_attri_gram_set::ag36),36);
		Analyser::ex_produc_set[36].set_gram(36);
		
		Analyser::ex_production::add_gram(&(CM_attri_gram_set::ag39),39);
		Analyser::ex_produc_set[39].set_gram(39);
		Analyser::ex_production::add_gram(&(CM_attri_gram_set::ag40),40);
		Analyser::ex_produc_set[40].set_gram(40);
		Analyser::ex_production::add_gram(&(CM_attri_gram_set::ag41),41);
		Analyser::ex_produc_set[41].set_gram(41);
		Analyser::ex_production::add_gram(&(CM_attri_gram_set::ag42),42);
		Analyser::ex_produc_set[42].set_gram(42);
		Analyser::ex_production::add_gram(&(CM_attri_gram_set::ag43),43);
		Analyser::ex_produc_set[43].set_gram(43);
		Analyser::ex_production::add_gram(&(CM_attri_gram_set::ag44),44);
		Analyser::ex_produc_set[44].set_gram(44);
		Analyser::ex_production::add_gram(&(CM_attri_gram_set::ag45),45);
		Analyser::ex_produc_set[45].set_gram(45);
		Analyser::ex_production::add_gram(&(CM_attri_gram_set::ag46),46);
		Analyser::ex_produc_set[46].set_gram(46);
		Analyser::ex_production::add_gram(&(CM_attri_gram_set::ag47),47);
		Analyser::ex_produc_set[47].set_gram(47);
		Analyser::ex_production::add_gram(&(CM_attri_gram_set::ag48),48);
		Analyser::ex_produc_set[48].set_gram(48);
		Analyser::ex_production::add_gram(&(CM_attri_gram_set::ag49),49);
		Analyser::ex_produc_set[49].set_gram(49);
		Analyser::ex_production::add_gram(&(CM_attri_gram_set::ag50),50);
		Analyser::ex_produc_set[50].set_gram(50);
		Analyser::ex_production::add_gram(&(CM_attri_gram_set::ag51),51);
		Analyser::ex_produc_set[51].set_gram(51);
		Analyser::ex_production::add_gram(&(CM_attri_gram_set::ag52),52);
		Analyser::ex_produc_set[52].set_gram(52);
		Analyser::ex_production::add_gram(&(CM_attri_gram_set::ag53),53);
		Analyser::ex_produc_set[53].set_gram(53);
		Analyser::ex_production::add_gram(&(CM_attri_gram_set::ag54),54);
		Analyser::ex_produc_set[54].set_gram(54);
		Analyser::ex_production::add_gram(&(CM_attri_gram_set::ag55),55);
		Analyser::ex_produc_set[55].set_gram(55);
		Analyser::ex_production::add_gram(&(CM_attri_gram_set::ag56),56);
		Analyser::ex_produc_set[56].set_gram(56);
		Analyser::ex_production::add_gram(&(CM_attri_gram_set::ag57),57);
		Analyser::ex_produc_set[57].set_gram(57);
		Analyser::ex_production::add_gram(&(CM_attri_gram_set::ag58),58);
		Analyser::ex_produc_set[58].set_gram(58);
		Analyser::ex_production::add_gram(&(CM_attri_gram_set::ag59),59);
		Analyser::ex_produc_set[59].set_gram(59);
		Analyser::ex_production::add_gram(&(CM_attri_gram_set::ag60),60);
		Analyser::ex_produc_set[60].set_gram(60);
		Analyser::ex_production::add_gram(&(CM_attri_gram_set::ag62),62);
		Analyser::ex_produc_set[62].set_gram(62);

		
		
		

	}


	//to get token_stream:
	Lexer::load_code(string("if.cpp"));
	
	//token_stream to elem_stream:
	list<Analyser::ex_element> ex_elem_stream;
	{
		list<Lexer::token>::const_iterator it;

		for(it=Lexer::token_stream.begin();it!=Lexer::token_stream.end();it++)
			ex_elem_stream.push_back(Analyser::ex_element(false,LR1PG::ter_list[*it],Analyser::attribute(*it)));
	}
	
	

	
	CM_analyser.load_table(string("M_TABLE.txt"));
	CM_analyser.analyse(ex_elem_stream);

	//Analyser::next_list::clean_ntar_j();

	/*
	FILE* fp;
	fp=fopen("act_list.txt","w");
	while(!Analyser::act_stack.empty())
	{
		
		cout<<Analyser::act_stack.top().toString()<<" : "<<LR1PG::produc_set[Analyser::act_stack.top().index].toString()<<endl;
		string str;
		str=Analyser::act_stack.top().toString()+" : "+LR1PG::produc_set[Analyser::act_stack.top().index].toString()+"\n";
		fputs(str.c_str(),fp);
		Analyser::act_stack.pop();
	}
	fclose(fp);
	*/
	system("pause");
}
