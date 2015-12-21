#include "Analyser.h"
#include "stdafx.h"
namespace Analyser
{
	
	void LR_analyser::shift(int state_index,const LR1PG::element& elem)
	{
		LR_stack.push(stack_block(state_index,elem));	
	}
	void LR_analyser::reduction(int produc_index)
	{
		int num=production_set[produc_index].r_part_size;
		for(int i=0;i<num;i++)
			LR_stack.pop();
	}
	void LR_analyser::load(const map<string,int>& v_list,const map<string,int>& t_list,
			const vector<LR1PG::production> produc_set,const set<LR1PG::LR_item_closure>& C,const LR1PG::LR_analysis_table& table)
	{
		{
			var_list=v_list;
			ter_list=t_list;

			map<string,int>::iterator it;
			for(it=LR1PG::var_list.begin();it!=LR1PG::var_list.end();it++)
				element_set.insert(LR1PG::element(true,it->second));
			for(it=LR1PG::ter_list.begin();it!=LR1PG::ter_list.end();it++)
				element_set.insert(LR1PG::element(true,it->second));
		}

		production_set=produc_set;
		LR_table=table;
		
		{
			set<LR1PG::LR_item_closure>::iterator it;
			for(it=C.begin();it!=C.end();it++)
				S[it->index]=*it;
		}



	}
	void LR_analyser::analyse(const list<LR1PG::element>& elem_stream)
	{
		list<LR1PG::element>::const_iterator ptr=elem_stream.begin();


		LR_stack.push(stack_block(0,LR1PG::element(false,ter_list.find("#")->second)));
		while(true)
		{
		 	LR1PG::action act=LR_table.at(LR_stack.top().state_index,*ptr);
			
			bool loop_swch=false;

			switch (act.type)
			{
			case LR1PG::action_type::shift:
				shift(act.index,*ptr);
				ptr++;
				break;
			case LR1PG::action_type::reduction:
				reduction(act.index);
				shift(LR_stack.top().state_index,production_set[act.index].l_part);
				break;
			case LR1PG::action_type::accept:
				cout<<"Accept!"<<endl;
				loop_swch=true;
				break;
			case LR1PG::action_type::error:
				cout<<"Error."<<endl;
				loop_swch=true;
				break;
			}

			if(loop_swch)
				break;
		}
	}


	map<string,int> LR_analyser::var_list;
	map<string,int> LR_analyser::ter_list;
	set<LR1PG::element> LR_analyser::element_set;
	vector<LR1PG::production> LR_analyser::production_set;
	LR1PG::LR_analysis_table LR_analyser::LR_table;
	map<int,LR1PG::LR_item_closure> LR_analyser::S;
	stack<LR_analyser::stack_block> LR_analyser::LR_stack;
}