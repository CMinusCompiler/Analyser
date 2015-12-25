#include "LR1PG.h"
#include "Lexer.h"
#include "Analyser.h"
#include <stack>


namespace CM_attri_gram_set
{
		Analyser::value ag20(const Analyser::value& v)
		{
		//ag20: declar_local ¡ú declar_local declar_var
			string ID=v.getStrValue(1,0);

			int width=v.getIntValue(1,0);
			Analyser::symbol_table.var_map[ID]=Analyser::nesting_table::get_offset();
			Analyser::nesting_table::incre_offset(width);

			//it does not need to return value
			return Analyser::value();
		}
		Analyser::value ag5(const Analyser::value& v)
		{
		//ag5: declar_var ¡ú type_var ID ;
			Analyser::value val;
			
			//ID.width=1
			val.setIntValue(0,0,v.getIntValue(1,0));
			val.setStrValue(0,0,v.getStrValue(1,0));
			return val;
		}
		Analyser::value ag6(const Analyser::value& v)
		{
		//ag6: declar_var ¡ú type_var ID [ NUM ] ;

			Analyser::value val;
			
			//ID.width=ID.width*NUM.val
			val.setIntValue(0,0,(v.getIntValue(1,0))*(v.getIntValue(3,0)));
			val.setStrValue(0,0,v.getStrValue(1,0));
			return val;
		}


}


class CM_analyser:public Analyser::LR_analyser
{
public:
	static Analyser::value reduction(int produc_index,Analyser::APT_node& father)
	{
		
		vector<Analyser::ex_element> ex_elem_container;
	
		int num=production_set[produc_index].r_part_size;

		//if it is X->. epsilon, the size does not mean the real length
		if(production_set[produc_index].isWithEPSILON)
			return	Analyser::value();


		for(int i=0;i<num;i++)
		{
			ex_elem_container.push_back(LR_stack.top());
			father.children.push_back(LR_stack.top());
			LR_stack.pop();
		}



		Analyser::value val;
		{
			
			for(int i=0;i<ex_elem_container.size();i++)
			{
				for(int j=0;j<ex_elem_container[i].val.getIntValElemArraySize();j++)
					for(int k=0;k<ex_elem_container[i].val.getintValVarArraySize(j);k++)
						val.setIntValue(i,k,ex_elem_container[i].val.getIntValue(j,k));
				
				for(int j=0;j<ex_elem_container[i].val.getStrValElemArraySize();j++)
					for(int k=0;k<ex_elem_container[i].val.getStrValVarArraySize(j);k++)
						val.setStrValue(i,k,ex_elem_container[i].val.getStrValue(j,k));
			}
		}
		Analyser::ex_produc_set[produc_index].set_value(val);

		

		return Analyser::ex_produc_set[produc_index].call_attri_gram(produc_index);
	}

	static void analyse(const list<Analyser::ex_element>& ex_e_stream)
	{

		list<Analyser::ex_element> elem_stream=ex_e_stream;
		
		//We need to put # at the end of elem_stream.
		elem_stream.push_back(Analyser::ex_element(false,ter_list[string("#")],Analyser::value()));

		list<Analyser::ex_element>::const_iterator ptr=elem_stream.begin();
		int index_GOTO;

		
		{
			//it is a leaf node
			Analyser::ex_element elem(false,ter_list[string("#")],Analyser::value());

			stack_block block=stack_block(0,elem);
			LR_stack.push(block);

			//Analyser::APT::constru_stack.push(Analyser::APT_node(elem));
		}
		
		

		while(true)
		{
		 	LR1PG::action act=LR_table.at(LR_stack.top().state_index,*ptr);
			
			//$$
			if((LR_stack.top().state_index==8)&&(ptr->toString()=="ID"))
			{
				list<Analyser::ex_element>::const_iterator it=ptr;
				it++;
				
				if((it!=elem_stream.end())&&((it->toString())=="("))
					act.index=9;
				else
					act.index=7;
			}
			if(LR_stack.top().state_index==301 && (ptr->toString()=="else"))
			{
				act.type=LR1PG::action_type::shift;
				act.index=307;
			}
			//$$

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
					Analyser::APT_node father_node;

					Analyser::value val=reduction(act.index,father_node);
					index_GOTO=LR_table.at(LR_stack.top().state_index,production_set[act.index].l_part).index;

					
					Analyser::ex_element elem(true,production_set[act.index].l_part.index,val);

					father_node=elem;

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
		
		Analyser::APT::root=*(new Analyser::APT_node(Analyser::APT::root));


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
		
	}
	{
		Analyser::ex_production::add_gram(&(CM_attri_gram_set::ag20),20);
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
			ex_elem_stream.push_back(Analyser::ex_element(false,LR1PG::ter_list[*it],Analyser::value(*it)));
		
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
