#include "LR1PG.h"
#include "Lexer.h"
#include "Analyser.h"
#include <stack>


namespace CM_attri_gram_set
{
//add your attribute grammers below:
//(suggested name form: agx)
		Analyser::attribute ag21(const vector<Analyser::ex_element>& r_part)
		{
		/*ag21: declar_local ¡ú declar_local declar_var
		{
			enter(declar_var.name,declar_var.type,offset);
			offset=offset+declar_var.width;
		}*/
		//in CM, there is not other types except int
		//the declar_var.type attribute is not used here
			string name=r_part[1].get_str("id");
			int width=r_part[1].get_int("width");
			Analyser::symbol_table.var_map[name]=Analyser::nesting_table::get_offset();
			Analyser::nesting_table::incre_offset(width);

			//it does not need to return value
			return Analyser::attribute();
		}
		Analyser::attribute ag5(const vector<Analyser::ex_element>& r_part)
		{
		/*ag5: declar_var ¡ú type_var ID ;
		{
			declar_var.type=type_var.type;
			declar_var.width=1;
			declar_var.name=ID.name;
		}*/
		//in CM, there is not other types except int
		//the declar_var.type attribute is not used here
		//and the memory unit is int 
			Analyser::attribute attri;
			
			//ID.width=1
			//val.setIntValue(0,0,v.getIntValue(1,0));
			attri.set_value("name",r_part[1].get_str("name"));
			attri.set_value("width",1);
			
			return attri;
		}
		Analyser::attribute ag6(const vector<Analyser::ex_element>& r_part)
		{
		/*ag6: declar_var ¡ú type_var ID [ NUM ] ;
		{
			declar_var.type=type_var.type;
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
		Analyser::ex_produc_set[20].set_gram(20);
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
