#include "LR1PG.h"
#include "Lexer.h"
#include "Analyser.h"
#include <stack>


namespace CM_attri_gram_set
{
//add your attribute grammers below:
//(suggested name form: agx)
		
		Analyser::attribute ag5(const vector<Analyser::ex_element>& r_part)
		{
		/*ag5: declar_var �� type_var ID ;
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
		/*ag6: declar_var �� type_var ID [ NUM ] ;
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
		/*ag19: compou_body �� { N1 declar_local code }
		{
			t=nesting_table::nstack_top();
			nesting_table::nstack_pop();
			global_memory::alloc(nesting_table::ostack_top());
			nesting_table::ostack_pop();
			enterproc(nesting_table::nstack_pop(),t);
		}*/

			Analyser::nesting_table t=Analyser::nesting_table::nstack_top();
			Analyser::nesting_table::nstack_pop();
			Analyser::global_memory::alloc(Analyser::nesting_table::ostack_top());
			Analyser::nesting_table::ostack_pop();
			Analyser::nesting_table::nstack_top().nes_body_list.push_back(t);
		}

		Analyser::attribute ag20(const vector<Analyser::ex_element>& r_part)
		{
		/*ag20: N1 �� ��
		{
			nesting_table t;
			nesting_table::nstack_push(t);
		}*/
			Analyser::nesting_table t(Analyser::global_memory::get_ptr());
			Analyser::nesting_table::nstack_push(t);
		}

		Analyser::attribute ag21(const vector<Analyser::ex_element>& r_part)
		{
		/*ag21: declar_local �� declar_local declar_var
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

			Analyser::nesting_table::nstack_top().var_map[name]=Analyser::nesting_table::ostack_top();
			Analyser::nesting_table::ostack_top()+=width;
			
			//it does not need to return value
			return Analyser::attribute();
		}

		Analyser::attribute ag53(const vector<Analyser::ex_element>& r_part)
		{
		/*ag53: op_add �� +
		{
			op_add.type='+';
		}*/
			Analyser::attribute attri;
			
			attri.set_value(string("type"),Analyser::op_tpye::add);
		}
		Analyser::attribute ag54(const vector<Analyser::ex_element>& r_part)
		{
		/*ag54: op_add �� -
		{
			op_add.type='-';
		}*/
			Analyser::attribute attri;
			
			attri.set_value(string("type"),Analyser::op_tpye::min);
		}
		Analyser::attribute ag57(const vector<Analyser::ex_element>& r_part)
		{
		/*ag57: op_mul �� *
		{
			op_add.type='*';
		}*/
			Analyser::attribute attri;
			
			attri.set_value(string("type"),Analyser::op_tpye::mul);
		}
		Analyser::attribute ag58(const vector<Analyser::ex_element>& r_part)
		{
		/*ag58: op_mul �� /
		{
			op_add.type='/';
		}*/
			Analyser::attribute attri;
			
			attri.set_value(string("type"),Analyser::op_tpye::div);
		}
		Analyser::attribute ag45(const vector<Analyser::ex_element>& r_part)
		{
		/*ag45: op_rel �� <=
		{
			op_add.type='<=';
		}*/
			Analyser::attribute attri;
			
			attri.set_value(string("type"),Analyser::op_tpye::let);
		}
		Analyser::attribute ag46(const vector<Analyser::ex_element>& r_part)
		{
		/*ag46: op_rel �� <
		{
			op_add.type='<';
		}*/
			Analyser::attribute attri;
			
			attri.set_value(string("type"),Analyser::op_tpye::lt);
		}
		Analyser::attribute ag47(const vector<Analyser::ex_element>& r_part)
		{
		/*ag47: op_rel �� >
		{
			op_add.type='>';
		}*/
			Analyser::attribute attri;
			
			attri.set_value(string("type"),Analyser::op_tpye::gt);
		}
		Analyser::attribute ag48(const vector<Analyser::ex_element>& r_part)
		{
		/*ag48: op_rel �� >=
		{
			op_add.type='>=';
		}*/
			Analyser::attribute attri;
			
			attri.set_value(string("type"),Analyser::op_tpye::get);
		}
		Analyser::attribute ag49(const vector<Analyser::ex_element>& r_part)
		{
		/*ag49: op_rel �� ==
		{
			op_add.type='==';
		}*/
			Analyser::attribute attri;
			
			attri.set_value(string("type"),Analyser::op_tpye::eq);
		}
		Analyser::attribute ag50(const vector<Analyser::ex_element>& r_part)
		{
		/*ag50: op_rel �� !=
		{
			op_add.type='!=';
		}*/
			Analyser::attribute attri;
			
			attri.set_value(string("type"),Analyser::op_tpye::neq);
		}
		Analyser::attribute ag51(const vector<Analyser::ex_element>& r_part)
		{
		/*ag51: expre_arith �� expre_arith1 op_add term
		{
			global_memory::alloc(1);
			nesting_table::nstack_top().var_map["#Tx"]=nesting_table::ostack_top();
			nesting_table::ostack_top()+=1;
			emit(op_add.type,expre_arith1.global_ptr,term.global_ptr,#Tx.global_ptr);
			expre_arith.global_ptr=#Tx.global_ptr;
		}*/
			
			Analyser::attribute attri;
			Analyser::global_memory::alloc(1);
			string Tx=Analyser::Tx_allocator();
			Analyser::nesting_table::nstack_top().var_map[string("#Tx")]=Analyser::nesting_table::ostack_top();
			printf("(%s,%d,%d,%d)\n",r_part[1].get_str("type"),r_part[0].get_int("global_ptr"),r_part[2].get_int("global_ptr"),Analyser::nesting_table::nstack_top().get_global_ptr(string("#Tx")));
			attri.set_value("global_ptr",Analyser::nesting_table::nstack_top().get_global_ptr(string("#Tx")));

		}
		Analyser::attribute ag39(const vector<Analyser::ex_element>& r_part)
		{
		/*ag39: expre �� var = expre1
		{
			emit(=,expre1.global_ptr,-,var.global_ptr);
			expre.global_ptr=var.global_ptr;
			

		}*/
			printf("(=,%d,-,%d)\n",r_part[2].get_int(string("global_ptr")),r_part[0].get_int(string("global_ptr")));
			Analyser::attribute attri;
			attri.set_value("global_ptr",r_part[0].get_int(string("global_ptr")));
		}
		
		Analyser::attribute ag42(const vector<Analyser::ex_element>& r_part)
		{
		/*ag42: var �� ID [ expre ]
		{
			

			emit(=,expre1.global_ptr,-,var.global_ptr);
			expre.global_ptr=var.global_ptr;
			

		}*/
			printf("(=,%d,-,%d)\n",r_part[2].get_int(string("global_ptr")),r_part[0].get_int(string("global_ptr")));
			Analyser::attribute attri;
			attri.set_value("global_ptr",r_part[0].get_int(string("global_ptr")));
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
	CM_analyser.load_productions(string("wenfa.txt"));

	for(int i=0;i<LR1PG::produc_set.size();i++)
	{
		LR1PG::produc_set[i];
		
		Analyser::ex_production produc(LR1PG::produc_set[i]);
		Analyser::ex_produc_set.push_back(produc);
	}
	{
		Analyser::ex_production::add_gram(&(CM_attri_gram_set::ag21),21);
		Analyser::ex_produc_set[21].set_gram(21);
		Analyser::ex_production::add_gram(&CM_attri_gram_set::ag5,5);
		Analyser::ex_produc_set[5].set_gram(5);
		Analyser::ex_production::add_gram(&CM_attri_gram_set::ag6,6);
		Analyser::ex_produc_set[6].set_gram(6);
	}


	//to get token_stream:
	Lexer::load_code(string("b.cpp"));
	
	//token_stream to elem_stream:
	list<Analyser::ex_element> ex_elem_stream;
	{
		list<Lexer::token>::const_iterator it;

		for(it=Lexer::token_stream.begin();it!=Lexer::token_stream.end();it++)
			ex_elem_stream.push_back(Analyser::ex_element(false,LR1PG::ter_list[*it],Analyser::attribute(*it)));
	}
	
	

	
	CM_analyser.load_table(string("TABLE.txt"));
	CM_analyser.analyse(ex_elem_stream);


	system("pause");
}
