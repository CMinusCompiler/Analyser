#include "Analyser.h"

namespace Analyser
{
	
	int attribute::get_int(const string& key)const
	{
		return (map<string,int>(this->int_instance))[key];
	}
	string attribute::get_str(const string& key)const
	{
		return (map<string,string>(this->str_instance))[key];
	}

	void attribute::set_value(const string& key,int value)
	{
		this->int_instance[key]=value;
	}

	void attribute::set_value(const string& key,string value)
	{
		this->str_instance[key]=value;
	}

	bool attribute::isNull(const string& key,bool isInt)const
	{
		if(isInt)
			return (this->int_instance.find(key)==this->int_instance.end());
		else
			return (this->str_instance.find(key)==this->str_instance.end());
	}
		
	
	
	APT_node& APT::root=APT_node();
	stack<APT_node> APT::constru_stack;

	
	void ex_element::set_attri(const attribute& attri)
	{
		this->attri=attri;
	}
	int ex_element::get_int(const string& key)const
	{
		return attri.get_int(key);
	}
	string ex_element::get_str(const string& key)const
	{
		return attri.get_str(key);
	}
	void ex_element::set_value(const string& key,int value)
	{
		attri.set_value(key,value);
	}
	void ex_element::set_value(const string& key,string value)
	{
		attri.set_value(key,value);
	}
	bool ex_element::isNull(const string& key,bool isInt)const
	{
		return attri.isNull(key,isInt);
	}


	vector<attribute (*)(const vector<ex_element>& r_part)> ex_production::attri_grams;
	void ex_production::set_gram(int index)
	{
			gram_index=index;
	}

	attribute ex_production::call_attri_gram()
	{
	//if the attribute grammer has not been defined,
	//return a null object
			if(!(this->isAttriLoaded))
				return attribute();
			
			if(gram_index<attri_grams.size()&&attri_grams[gram_index])
				return (attri_grams[gram_index])(r_part);
			else
				return attribute();
	}
	void ex_production::load_attributes(const vector<attribute>& attributes)
	{
		if(r_part.size()!=attributes.size())		
		{
			cout<<"Param form does not match;"<<endl;
			return;
		}

		for(int i=0;i<r_part.size();i++)
			r_part[i].set_attri(attributes[i]);
		isAttriLoaded=true;
	}
	void ex_production::load_attributes(const vector<ex_element>& ex_elem_container)
	{
		if(r_part.size()!=ex_elem_container.size())
		{
			cout<<"Param form does not match;"<<endl;
			return;
		}
		this->r_part=ex_elem_container;
	}

	int ex_production::add_gram(attribute (*g)(const vector<ex_element>& r_part),int index)
	{
		if(!(index<attri_grams.size()))
		{
			int d=index-attri_grams.size()+1;
			for(int i=0;i<d;i++)
				attri_grams.push_back((attribute (*)(const vector<ex_element>& r_part))int());
		}

		attri_grams[index]=g;
		return index;
	}
	
	
		
		
	
	nesting_table symbol_table;
	vector<int> nesting_table::memory_area;
	int nesting_table::offset_pos=0;
	stack<nesting_table* > nesting_table::nestptr_stack;

	vector<ex_production> ex_produc_set;



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
	attribute LR_analyser::reduction(int produc_index,APT_node& father)
	{
		int num=production_set[produc_index].r_part_size;

		//if it is X->. epsilon, the size does not mean the real length
		if(production_set[produc_index].isWithEPSILON)
			return attribute(); 

		for(int i=0;i<num;i++)
			LR_stack.pop();

		return attribute();
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
				/*...*/	
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
					Analyser::APT_node father_node;

					Analyser::attribute l_part_attri=reduction(act.index,father_node);
					index_GOTO=LR_table.at(LR_stack.top().state_index,production_set[act.index].l_part).index;

					
					Analyser::ex_element elem(true,production_set[act.index].l_part.index,l_part_attri);

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