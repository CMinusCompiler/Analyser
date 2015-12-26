#include "LR1PG.h"
#include "Lexer.h"
#include "Analyser.h"
#include <stack>


namespace CM_attri_gram_set
{
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


class CM_analyser:public Analyser::LR_analyser
{
public:
	static Analyser::attribute reduction(int produc_index,Analyser::APT_node& father)
	{
		
		vector<Analyser::ex_element> ex_elem_container;
	
		int num=production_set[produc_index].r_part_size;

		//if it is X->. epsilon, the size does not mean the real length
		if(production_set[produc_index].isWithEPSILON)
			return	Analyser::attribute();


		for(int i=0;i<num;i++)
		{
			ex_elem_container.push_back(LR_stack.top());
			father.children.push_back(LR_stack.top());
			LR_stack.pop();
		}

		//get attributes from the stack(contained in ex_elem_container)
		//assign attributes to the ex_production to be reducted
		//in other words, prepare the attribute grammer with the attributes
		Analyser::ex_produc_set[produc_index].load_attributes(ex_elem_container);
		

		return Analyser::ex_produc_set[produc_index].call_attri_gram();
	}

	static void analyse(const list<Analyser::ex_element>& ex_e_stream)
	{

		list<Analyser::ex_element> elem_stream=ex_e_stream;
		
		//We need to put # at the end of elem_stream.
		elem_stream.push_back(Analyser::ex_element(false,ter_list[string("#")],Analyser::attribute()));

		list<Analyser::ex_element>::const_iterator ptr=elem_stream.begin();
		int index_GOTO;

		
		{
			//it is a leaf node
			Analyser::ex_element elem(false,ter_list[string("#")],Analyser::attribute());

			stack_block block=stack_block(0,elem);
			LR_stack.push(block);

			//Analyser::APT::constru_stack.push(Analyser::APT_node(elem));
		}
		
		

		while(true)
		{
		 	LR1PG::action act=LR_table.at(LR_stack.top().state_index,*ptr);
			
				
			//add conflict treatment here
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
			//

			bool loop_swch=false;

			switch (act.type)
			{
			case LR1PG::action_type::shift:
				{
				//nodes shifted directly are leaf nodes
					
					shift(act.index,*ptr);
					ptr++;
					cout<<act.toString()<<endl;
					break;
				}
			case LR1PG::action_type::reduction:
				{
					//Analyser::APT_node& father_node=*(new Analyser::APT_node());
					Analyser::APT_node& father_node=*(new Analyser::APT_node());

					Analyser::attribute l_part_attri=reduction(act.index,father_node);
					index_GOTO=LR_table.at(LR_stack.top().state_index,production_set[act.index].l_part).index;

					if(index_GOTO==37)
						cout<<37<<endl;


					shift(index_GOTO,father_node);

					Analyser::APT::root=father_node;

					cout<<act.toString()<<endl;
					cout<<LR1PG::action(LR1PG::action_type::shift,index_GOTO).toString()<<endl;
					break;
				}
			case LR1PG::action_type::accept:
				{
					cout<<"Accept!"<<endl;
					loop_swch=true;
					break;
				}
			case LR1PG::action_type::error:
				{
					cout<<"Error."<<endl;
					loop_swch=true;
					break;
				}
			}

			if(loop_swch)
				break;
		}
		
//		Analyser::APT::root=*(new Analyser::APT_node(Analyser::APT::root));


	}
};


void main()
{
	
	//to get var_list, ter_list, produc_set and set_C in LR1PG
	//and var_list, ter_list, production_set in LR_analyser
	CM_analyser::load_productions(string("wenfa.txt"));

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
	
	/*
	//load productions and LR table:
	Analyser::LR_analyser::load_productions(string("wenfa.txt"));
	Analyser::LR_analyser::load_table(string("TABLE.txt"));

	Analyser::LR_analyser::analyse(elem_stream);
	*/

	
	CM_analyser::load_table(string("TABLE.txt"));
	CM_analyser::analyse(ex_elem_stream);


	system("pause");
}
