#pragma   once  

#include "LR1PG.h"
#include<stdarg.h>
#include <stack>

namespace Analyser
{

	

	class next_list
	{
	public:
		
		static int null_addr;
		static void create_quad(const char* op,const char* s1,const char* s2,const char* tar);
		static void back_patch(next_list* pnq,int tar_addr,bool is_code_reduc=false);
		static void delete_quad(int instr_addr);

		static next_list* merge(next_list* a,next_list* b);
		static int get_nextquad();
		static void clean_ntar_j();
		static void print_quads(const string& file_name);
		next_list(int instr_addr)
		{
			this->instr_addr=instr_addr;
			this->post=NULL;
		}
		next_list(const next_list& nq)
		{
			this->instr_addr=nq.instr_addr;
			this->post=nq.post;
		}
		next_list* post;
		
	private:
		int instr_addr;
		static int program_counter;
		static int alloc();

		class quad_expression
		{
		private:
		
			string dim[4];
			int instr_addr;
			
			
		public:
			
		
			quad_expression(const char* op,const char* s1,const char* s2,const char* tar)
			{
				dim[0]=op;
				dim[1]=s1;
				dim[2]=s2;
				dim[3]=tar;
			
				instr_addr=alloc();
			}

			quad_expression(const quad_expression& quad)
			{
				dim[0]=quad.dim[0];
				dim[1]=quad.dim[1];
				dim[2]=quad.dim[2];
				dim[3]=quad.dim[3];

				instr_addr=quad.instr_addr;
			}
			void set(int i,const string& s);
			string get(int i);
			string toString();
			void set_instr_addr(int i);

		};
		
		static vector<quad_expression> quad_expres;
		

	};




	enum op_tpye
	{
		add,min,mul,div,let,lt,gt,get,eq,neq
	};

	class attribute
	{
	public:
		attribute()
		{
		}
		attribute(const attribute& a)
		{
			int_instance=a.int_instance;
			str_instance=a.str_instance;
		}
		int get_int(const string& key)const;
		string get_str(const string& key)const;
		void set_value(const string& key,int value);
		void set_value(const string& key,string value);
		bool isNull(const string& key,bool isInt)const;
		

	private:
		map<string,int> int_instance;
		map<string,string> str_instance;
	};

	

	class ex_element:public LR1PG::element
	{
	public:
		ex_element():LR1PG::element(){}
		ex_element(bool isVar,int index,const attribute& a):LR1PG::element(isVar,index)
		{
			this->attri=a;
		}
		ex_element(const ex_element& e):LR1PG::element(e)
		{
			attri=e.attri;
		}
		ex_element(const element& e):LR1PG::element(e)
		{
		}
		void set_attri(const attribute& attri);
		int get_int(const string& key)const;
		string get_str(const string& key)const;
		void set_value(const string& key,int value);
		void set_value(const string& key,string value);
		bool isNull(const string& key,bool isInt)const;
	private:
		attribute attri;
		
	};

	class APT_node:public ex_element
	{
	public:
		vector<APT_node> children;
		APT_node():ex_element()
		{}
		APT_node(const APT_node& node):ex_element(node)
		{
			children=node.children;
		}
		APT_node(bool isVar,int index,const attribute& a):ex_element(isVar,index,a)
		{}
		APT_node(const ex_element& e):ex_element(e)
		{}
	};

	class APT
	{
	public:
		static APT_node& root;
		static stack<APT_node> constru_stack;
		static void print(const string& file_name);
		static void astack_push(const LR1PG::action& act);
		static bool astack_empty();
		static void astack_pop();
		static LR1PG::action& astack_top();
	protected:
		static stack<LR1PG::action> act_stack;

	};
	
	
	
	

	class ex_production
	{
	public:
		ex_element l_part;
		vector<ex_element> r_part;
		bool isAttriLoaded;

		ex_production(const LR1PG::production& produc)
		{
		//cannot be created as a null object
			l_part=produc.l_part;
			for(int i=0;i<produc.r_part.size();i++)
				r_part.push_back(ex_element(produc.r_part[i]));
			isAttriLoaded=false;
		}
		ex_production(const ex_production& produc)
		{
			gram_index=produc.gram_index;
			r_part=produc.r_part;
			isAttriLoaded=produc.isAttriLoaded;
		}
		

		void load_attributes(const vector<attribute>& attributes);
		void load_attributes(const vector<ex_element>& ex_elem_container);
		void set_gram(int index);
		attribute call_attri_gram();
		
		static int add_gram(attribute (*g)(const vector<ex_element>& r_part),int index);
		

		
	private:
		int gram_index;
		
		
		static vector<attribute (*)(const vector<ex_element>& r_part)> attri_grams;
	
	};

	extern vector<ex_production> ex_produc_set;

	
	
	class global_memory
	{
	private:
		//record static variables and temperary variables
		static int size;
		static int ptr;
	public:
		static int get_ptr();
		static void alloc(int num);
		static void release(int num);
		
	};


	class Tx_allocator
	{
	private:
		static int index;
	public:
		static string generate_str();
	};

	

	class nesting_table
	{
	public:
	//if the grammer is with multiple kinds of variable types, 
	//it should be map<class x,int>(x is a class recording the name and type).
		map<string,int> var_map;
		list<nesting_table* > nes_body_list;
		nesting_table* pre_table;
		

		//record the begining position of this table in global_memory
		int ptr;

		nesting_table()
		{
		}
		nesting_table(int ptr,nesting_table* pre)
		{
			this->ptr=ptr;
			this->pre_table=pre;
		}
		nesting_table(const nesting_table& table)
		{
			var_map=table.var_map;
			nes_body_list=table.nes_body_list;
			pre_table=table.pre_table;

			ptr=table.ptr;
		}
		
		//same as lookup() in text book
		int get_global_ptr(const string& name);
		
		static int NULL_ADDR;
		static void stack_init();

		static void ostack_push(int ptr);
		static void ostack_pop();
		static int& ostack_top();

		static void nstack_push(nesting_table* pt);
		static void nstack_pop();
		static nesting_table* nstack_top();

		static bool pstack_is_empty();
		static int pstack_size();
		static void pstack_push(const attribute& attri);
		static void pstack_pop();
		static attribute& pstack_top();

		static map<string,int> func_arg_num;	
	private:
		
		static stack<attribute> param_stack;
		static stack<int> offsetptr_stack;
		//help record which nesting we are in
		static stack<nesting_table* > nestptr_stack;
		
	};
	//the zeroth layer
	extern nesting_table* symbol_table;
	


	class LR_analyser
	{
	protected:
		class stack_block:public APT_node
		{
		public:
			int state_index;
			
			stack_block(int index,const APT_node& e):APT_node(e)
			{
				state_index=index;
			}
			stack_block(const stack_block& block):APT_node(block)
			{
				this->state_index=block.state_index;
			}
		};
		map<string,int> var_list;
		map<string,int> ter_list;
		set<LR1PG::element> element_set;
		vector<LR1PG::production> production_set;
		LR1PG::LR_analysis_table LR_table;

	
		stack<stack_block> LR_stack;
		void shift(int state_index,const APT_node& node);
		attribute reduction(int produc_index,APT_node& father);
		virtual void conflict_management(const list<Analyser::ex_element>& elem_stream,const list<Analyser::ex_element>::const_iterator& ptr,LR1PG::action& act)=0;
		virtual void load_ags()=0;
		list<Analyser::ex_element>::const_iterator ptr;

	public:
		void load_productions(const string& file_name);
		void load_table(const string& file_name);
		void analyse(const list<Analyser::ex_element>& ex_e_stream);
		list<Analyser::ex_element>::const_iterator& get_ptr();

		
		//bool condi_swch;
	};

	
	
}