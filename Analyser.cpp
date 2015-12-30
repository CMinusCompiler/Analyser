#include "Analyser.h"
#include <sstream>
namespace Analyser
{
	stack<LR1PG::action> act_stack;


	int next_list::get_nextquad()
	{
		return program_counter;
	}
	int next_list::alloc()
	{
		return program_counter++;
	}
	int next_list::program_counter=0;
	int next_list::null_addr=-1;

	string next_list::quad_expression::toString()
	{
		string str="("+dim[0]+","+dim[1]+","+dim[2]+","+dim[3]+")";
		return str;
	}
	
	string next_list::quad_expression::get(int i)
	{
		return dim[i];
	}

	void next_list::clean_ntar_j()
	{
		vector<quad_expression>::iterator it=quad_expres.begin();
		for(;it<quad_expres.end();)
		{
			if((('j'==(it->get(0))[0]))&&((string("-")==(it->get(3)))))
				it=quad_expres.erase(it);
			else
				it++;
		}
		
	
	}
	
	void next_list::quad_expression::set(int i,const string& s)
	{
		this->dim[i]=s;
	}

	vector<next_list::quad_expression> next_list::quad_expres;

	void next_list::create_quad(const char* op,const char* s1,const char* s2,const char* tar)
	{
		quad_expres.push_back(quad_expression(op,s1,s2,tar));
	}



	void next_list::back_patch(next_list* pnq,int tar_addr)
	{
		next_list* p=pnq;
		while(p)
		{
			stringstream ss;
			ss<<tar_addr;
			string addr;
			ss>>addr;
			if(p->instr_addr>0)
				quad_expres[p->instr_addr].set(3,addr);
			p=p->post;
			
		}
	
	}
	next_list* next_list::merge(next_list* a,next_list* b)
	{
		next_list* p=a;
		while(p->post)
		{
			p=p->post;
		}
		p->post=b;

		return a;
	}


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
			if((!(this->isAttriLoaded))&&(!(LR1PG::produc_set[this->gram_index].isWithEPSILON)))
				return attribute();
			//$$
			//return attribute();
			//$$
			
			
			if(gram_index<attri_grams.size()&&attri_grams[gram_index])
				return (attri_grams[gram_index])(r_part);
			else
				return attribute();
	}
	void ex_production::load_attributes(const vector<attribute>& attributes)
	{
		/*
		if(r_part.size()!=attributes.size())		
		{
			cout<<"Param form does not match;"<<endl;
			return;
		}
		*/
		for(int i=0;i<r_part.size();i++)
			r_part[i].set_attri(attributes[i]);
		isAttriLoaded=true;
	}
	void ex_production::load_attributes(const vector<ex_element>& ex_elem_container)
	{
		/*
		if(r_part.size()!=ex_elem_container.size())
		{
			cout<<"Param form does not match;"<<endl;
			return;
		}*/
		this->r_part=ex_elem_container;
		isAttriLoaded=true;
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
	
	vector<ex_production> ex_produc_set;
		
		
	
	nesting_table* symbol_table;

	stack<nesting_table* > nesting_table::nestptr_stack;
	stack<int> nesting_table::offsetptr_stack;
	stack<attribute> nesting_table::param_stack;
	map<string,int> nesting_table::func_arg_num;

	vector<int> global_memory::mem;
	int global_memory::ptr=0;
	int global_memory::get_ptr()
	{
		return ptr;
	}
	void global_memory::alloc(int num)
	{
		for(int i=0;i<num;i++)
			mem.push_back(int());
		ptr=mem.size();
	}
	int& global_memory::at(int pos)
	{
		return mem[pos];
	}
	
	int Tx_allocator::index=0;
	string Tx_allocator::generate_str()
	{
		string str("#T");
		string index_str;
		stringstream ss;
		ss<<index;
		ss>>index_str;
		str+=index_str;
		index++;
		return str;
	}


	int nesting_table::get_global_ptr(const string& name)
	{
		if(var_map.find(name)!=var_map.end())
			return this->ptr+var_map[name];
		
		nesting_table* tp;
		tp=this->pre_table;
		
		while(true)
		{
			if(!tp)
				break;

			if(tp->var_map.find(name)!=tp->var_map.end())
				return tp->var_map[name];
			else
				tp=tp->pre_table;
		}
		cout<<"variable "<<name<<" has not been defined."<<endl;
		return NULL_ADDR;
	}

	void nesting_table::stack_init()
	{
		nstack_push(new nesting_table(0,NULL));
		ostack_push(0);
		symbol_table=nstack_top();

		//create return_w_reg for return phrases
		global_memory::alloc(1);
		nesting_table::nstack_top()->var_map["#return_w_reg"]=Analyser::nesting_table::ostack_top();
		nesting_table::ostack_top()+=1;
		//create return_r_reg for invoke phrases
		global_memory::alloc(1);
		nesting_table::nstack_top()->var_map["#return_r_reg"]=Analyser::nesting_table::ostack_top();
		nesting_table::ostack_top()+=1;
		//create return_addr_reg;
		global_memory::alloc(1);
		nesting_table::nstack_top()->var_map["#return_addr_reg"]=Analyser::nesting_table::ostack_top();
		nesting_table::ostack_top()+=1;
		
	}
	void nesting_table::ostack_push(int ptr)
	{
		offsetptr_stack.push(ptr);
	}
	void nesting_table::ostack_pop()
	{
		offsetptr_stack.pop();
	}
	int& nesting_table::ostack_top()
	{
		return offsetptr_stack.top();
	}

	int nesting_table::NULL_ADDR=-1;
	void nesting_table::nstack_push(nesting_table* pt)
	{
		nestptr_stack.push(pt);
	}
	void nesting_table::nstack_pop()
	{
		nestptr_stack.pop();
	}
	nesting_table* nesting_table::nstack_top()
	{
		return nestptr_stack.top();
	}
	

	int nesting_table::pstack_size()
	{
		return param_stack.size();
	}
	bool nesting_table::pstack_is_empty()
	{
		return param_stack.empty();
	}

	void nesting_table::pstack_push(const attribute& attri)
	{
		param_stack.push(attri);
	}
	void nesting_table::pstack_pop()
	{
		param_stack.pop();
	}
	attribute& nesting_table::pstack_top()
	{
		return param_stack.top();
	}


	list<Analyser::ex_element>::const_iterator& LR_analyser::get_ptr()
	{
		return ptr;
	}
	void LR_analyser::shift(int state_index,const APT_node& node)
	{
		LR_stack.push(stack_block(state_index,node));	
		
		//APT::constru_stack.push(APT_node(elem));
	}
	attribute LR_analyser::reduction(int produc_index,APT_node& father)
	{
		act_stack.push(LR1PG::action(LR1PG::action_type::reduction,produc_index));

		vector<Analyser::ex_element> ex_elem_container;
		

		int num=production_set[produc_index].r_part_size;

		for(int i=0;i<num;i++)
			ex_elem_container.push_back(Analyser::ex_element());


		//if it is X->. epsilon, the size does not mean the real length
		if(production_set[produc_index].isWithEPSILON)
			return Analyser::ex_produc_set[produc_index].call_attri_gram();


		for(int i=0;i<num;i++)
		{
			ex_elem_container[num-i-1]=LR_stack.top();
			father.children.push_back(LR_stack.top());
			LR_stack.pop();
		}

		//get attributes from the stack(contained in ex_elem_container)
		//assign attributes to the ex_production to be reducted
		//in other words, prepare the attribute grammer with the attributes
		Analyser::ex_produc_set[produc_index].load_attributes(ex_elem_container);
		

		return Analyser::ex_produc_set[produc_index].call_attri_gram();
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

		ptr=elem_stream.begin();
		int index_GOTO;

		
		{
			//it is a leaf node
			Analyser::ex_element elem(false,ter_list[string("#")],Analyser::attribute());

			stack_block block=stack_block(0,elem);
			LR_stack.push(block);

			
		}
		
		//push zeroth layer info into stacks
		nesting_table::stack_init();
		
		while(true)
		{
		 	LR1PG::action act=LR_table.at(LR_stack.top().state_index,*ptr);
			
			//add conflict treatment here
			conflict_management(elem_stream,ptr,act);
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
					

					Analyser::APT_node father_node;
					

					//$$
					if(act.index==55)
						cout<<55<<endl;
					//$$

					Analyser::attribute l_part_attri=reduction(act.index,father_node);
					index_GOTO=LR_table.at(LR_stack.top().state_index,production_set[act.index].l_part).index;

					
					Analyser::ex_element elem(true,production_set[act.index].l_part.index,l_part_attri);

					father_node=elem;
					shift(index_GOTO,father_node);

					//$$
					if(index_GOTO==197)
						cout<<197<<endl;
					//$$
					


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

	
}