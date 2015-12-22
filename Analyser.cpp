#include "Analyser.h"

namespace Analyser
{
	
	void LR_analyser::shift(int state_index,const LR1PG::element& elem)
	{
		LR_stack.push(stack_block(state_index,elem));	
	}
	void LR_analyser::reduction(int produc_index)
	{
		int num=production_set[produc_index].r_part_size;

		//if it is X->. epsilon, the size does not mean the real length
		if(production_set[produc_index].isWithEPSILON)
			return;

		for(int i=0;i<num;i++)
			LR_stack.pop();
	}
	void LR_analyser::load_productions(const string& file_name)
	{
		if(LR1PG::produc_set.empty())
			LR1PG::load_productions(file_name);

		{
			var_list=LR1PG::var_list;
			ter_list=LR1PG::ter_list;

			//init element_set
			map<string,int>::iterator it;
			for(it=LR1PG::var_list.begin();it!=LR1PG::var_list.end();it++)
				element_set.insert(LR1PG::element(true,it->second));
			for(it=LR1PG::ter_list.begin();it!=LR1PG::ter_list.end();it++)
				element_set.insert(LR1PG::element(true,it->second));
		}

		production_set=LR1PG::produc_set;
	}
	void LR_analyser::analyse(const list<LR1PG::element>& e_stream)
	{
		list<LR1PG::element> elem_stream=e_stream;
		//We need to put # at the end of elem_stream.
		elem_stream.push_back(LR1PG::element(false,ter_list[string("#")]));

		list<LR1PG::element>::const_iterator ptr=elem_stream.begin();
		int index_GOTO;

		LR_stack.push(stack_block(0,LR1PG::element(false,ter_list.find("#")->second)));
		

		while(true)
		{
		 	LR1PG::action act=LR_table.at(LR_stack.top().state_index,*ptr);
			
			//$$
			if((LR_stack.top().state_index==8)&&(ptr->toString()=="ID"))
			{
				list<LR1PG::element>::const_iterator it=ptr;
				it++;
				
				if((it!=elem_stream.end())&&((it->toString())=="("))
					act.index=9;
				else
					act.index=7;

				

			}
			//$$

			bool loop_swch=false;

			switch (act.type)
			{
			case LR1PG::action_type::shift:
				if(act.index==57)
					cout<<"57"<<endl;
				shift(act.index,*ptr);
				ptr++;
				cout<<act.toString()<<endl;
				break;
			case LR1PG::action_type::reduction:
				reduction(act.index);
				index_GOTO=LR_table.at(LR_stack.top().state_index,production_set[act.index].l_part).index;
				shift(index_GOTO,production_set[act.index].l_part);
				cout<<act.toString()<<endl;
				cout<<LR1PG::action(LR1PG::action_type::shift,index_GOTO).toString()<<endl;
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
	void LR_analyser::load_table(const string& file_name)
	{
		FILE* fp;
		fp=fopen(file_name.c_str(),"r");
		if(!fp)
		{
			cout<<"Cannot open "<<file_name<<"."<<endl;
			return;
		}
		char buf[10000];
		
		while(true)
		{
			int index;
			int n=fscanf(fp,"%d",&index);
			if(n<=0)
				break;

			fgetc(fp);

			fgets(buf,10000,fp);
			if(!buf)
				return;
			string str(buf);
			size_t line_feed_pos=str.find("\n");
			str.erase(line_feed_pos);

			size_t blank_pos=str.find(" ");
			string elem_str=str.substr(0,blank_pos);
			string act_str=str.substr(blank_pos+1);

			LR1PG::element elem;
			if(ter_list.find(elem_str)!=ter_list.end())
			{
			//!isVar
				elem=LR1PG::element(false,ter_list[elem_str]);

			}
			else if(var_list.find(elem_str)!=var_list.end())
			{
			//isVar
				elem=LR1PG::element(true,var_list[elem_str]);
			}
			else
			{
				cout<<"Error: file form does not match."<<endl;
				return;
			}
			LR1PG::action act;
			switch (act_str[0])
			{
			case 's':
				act=LR1PG::action(LR1PG::action_type::shift,atoi(act_str.substr(1).c_str()));
				break;
			case 'r':
				act=LR1PG::action(LR1PG::action_type::reduction,atoi(act_str.substr(1).c_str()));
				break;
			case 'a':
				act=LR1PG::action(LR1PG::action_type::accept,-1);
				break;
			case 'e':
				act=LR1PG::action(LR1PG::action_type::error,-1);
				break;
			}
			LR_table.set(index,elem,act);
		}

		
		
		fclose(fp);	
	}

	map<string,int> LR_analyser::var_list;
	map<string,int> LR_analyser::ter_list;
	set<LR1PG::element> LR_analyser::element_set;
	vector<LR1PG::production> LR_analyser::production_set;
	LR1PG::LR_analysis_table LR_analyser::LR_table;
	
	stack<LR_analyser::stack_block> LR_analyser::LR_stack;
}