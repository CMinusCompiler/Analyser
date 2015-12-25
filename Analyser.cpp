#include "Analyser.h"

namespace Analyser
{
	
	
	
	
	
	APT_node& APT::root=APT_node();
	stack<APT_node> APT::constru_stack;

	vector<value (*)(const value& v)> ex_production::attri_grams;
	void ex_production::set_gram(int index)
	{
			gram_index=index;
	}
	value ex_production::call_attri_gram(const value& v)
	{
			return (attri_grams[gram_index])(v);
	}
		
	int ex_production::add_gram(value (*g)(const value& v),int index)
	{
		if(!(index<attri_grams.size()))
		{
			for(int i=0;i<index-attri_grams.size()+1;i++)
				attri_grams.push_back((value (*)(const value& v))int());
		}

		attri_grams[index]=g;
		return index;
	}
	void ex_production::set_value(const value& v)
	{
		val=v;
	}
		
		
	
	nesting_table symbol_table;
	vector<int> nesting_table::memory_area;
	int nesting_table::offset_pos=0;
	stack<nesting_table* > nesting_table::nestptr_stack;

	vector<ex_production> ex_produc_set;


	void value::setIntValue(int elem_index,int val_index,int value)
	{
		if(!(elem_index<int_val_array.size()))
		{
			int d=elem_index-int_val_array.size()+1;
			for(int i=0;i<d;i++)
				int_val_array.push_back(vector<int>());
		}
		if(!(val_index<int_val_array[elem_index].size()))
		{
			int d=val_index-int_val_array[elem_index].size()+1;
			for(int i=0;i<d;i++)
				(int_val_array[elem_index]).push_back(int());
		}

		(int_val_array[elem_index])[val_index]=value;

	}
	void value::setStrValue(int elem_index,int val_index,string str)
	{
		if(!(elem_index<int_val_array.size()))
		{
			int d=elem_index-int_val_array.size()+1;
			for(int i=0;i<d;i++)
				str_val_array.push_back(vector<string>());
		}
		if(!(val_index<int_val_array[elem_index].size()))
		{
			int d=val_index-int_val_array[elem_index].size()+1;
			for(int i=0;i<d;i++)
				(str_val_array[elem_index]).push_back(string());
		}

		(str_val_array[elem_index])[val_index]=str;
	}
	int value::getIntValue(int elem_index,int val_index)const 
	{
		if(!(elem_index<int_val_array.size()))
		{
			cout<<"Exceeded limit of the array."<<endl;
			return int();
		}
		if(!(val_index<int_val_array[elem_index].size()))
		{
			cout<<"Exceeded limit of the array."<<endl;
			return int();
		}

		return (int_val_array[elem_index])[val_index];
	}
	string value::getStrValue(int elem_index,int val_index)const 
	{
		if(!(elem_index<str_val_array.size()))
		{
			cout<<"Exceeded limit of the array."<<endl;
			return int();
		}
		if(!(val_index<str_val_array[elem_index].size()))
		{
			cout<<"Exceeded limit of the array."<<endl;
			return int();
		}

		return (str_val_array[elem_index])[val_index];
	}
	int value::getStrValElemArraySize()const
	{
		return str_val_array.size();
	}
	int value::getIntValElemArraySize()const
	{
		return int_val_array.size();
	}
	int value::getStrValVarArraySize(int index)const
	{
		return str_val_array[index].size();
	}
	int value::getintValVarArraySize(int index)const
	{
		return int_val_array[index].size();
	}


	void nesting_table::stack_push(nesting_table* ptr)
	{
		nestptr_stack.push(ptr);
	}
	void nesting_table::stack_pop()
	{
		nestptr_stack.pop();
	}
	nesting_table* nesting_table::stack_top()
	{
		return nestptr_stack.top();
	}
	int nesting_table::get_offset()
	{
		return offset_pos;
	}
	void nesting_table::incre_offset(int size)
	{
		offset_pos+=size;
	}

	void LR_analyser::shift(int state_index,const APT_node& node)
	{
		LR_stack.push(stack_block(state_index,node));	

		//APT::constru_stack.push(APT_node(elem));
	}
	value LR_analyser::reduction(int produc_index,APT_node& father)
	{
		int num=production_set[produc_index].r_part_size;

		//if it is X->. epsilon, the size does not mean the real length
		if(production_set[produc_index].isWithEPSILON)
			return value(); 

		for(int i=0;i<num;i++)
			LR_stack.pop();

		return value();
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
	void LR_analyser::analyse(const list<Analyser::ex_element>& ex_e_stream)
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