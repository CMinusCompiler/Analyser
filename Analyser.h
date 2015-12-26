#pragma   once  

#include "LR1PG.h"
#include<stdarg.h>
#include <stack>

namespace Analyser
{
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
		int get_int(const string& key);
		string get_str(const string& key);
		void set_value(const string& key,int value);
		void set_value(const string& key,string value);
		bool isNull(const string& key);
		

	private:
		map<string,int> int_instance;
		map<string,string> str_instance;
	};


	class value
	{
	
	public:
		value(int num,const string& str)
		{
			setIntValue(0,0,num);
			setStrValue(0,0,str);
		}
		value(int num)
		{
			setIntValue(0,0,num);
		}
		value(const string& str)
		{
			setStrValue(0,0,str);
		}
		value(){}
		value(const value& v)
		{
			int_val_array=v.int_val_array;
			str_val_array=v.str_val_array;
		}
		void setIntValue(int elem_index,int val_index,int value);
		void setStrValue(int elem_index,int val_index,string str);
		int getIntValue(int elem_index,int val_index) const;
		string getStrValue(int elem_index,int val_index) const;
		int getStrValElemArraySize()const;
		int getIntValElemArraySize()const;
		int getStrValVarArraySize(int index)const;
		int getintValVarArraySize(int index)const;
	private:
		//the first dimension discribe the elem index
		//the second dimension discribe the ith varible of the elem
		vector<vector<int> > int_val_array;
		vector<vector<string> > str_val_array;
	};

	class ex_element:public LR1PG::element
	{
	public:
		value val;
		ex_element():LR1PG::element(){}
		ex_element(bool isVar,int index,const value& v):LR1PG::element(isVar,index)
		{
			val=v;
		}
		ex_element(const ex_element& e):LR1PG::element(e)
		{
			val=e.val;
		}
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
		APT_node(bool isVar,int index,const value& v):ex_element(isVar,index,v)
		{}
		APT_node(const ex_element& e):ex_element(e)
		{}
	};

	class APT
	{
	public:
		static APT_node& root;
		static stack<APT_node> constru_stack;
		static void print();
	};
	
	
	
	

	class ex_production:public LR1PG::production
	{
	public:
		value val;
		ex_production():LR1PG::production()
		{}
		ex_production(const ex_production& produc):LR1PG::production(produc)
		{
			gram_index=produc.gram_index;
			val=produc.val;
		}
		ex_production(const LR1PG::production& produc):LR1PG::production(produc)
		{
		}
		ex_production(const LR1PG::production& produc,const value& v):LR1PG::production(produc)
		{
			val=v;
		}
		void set_value(const value& v);
		void set_gram(int index);
		value call_attri_gram(const value& v);
		
		static int add_gram(value (*g)(const value& v),int index);
		

		
	private:
		int gram_index;
		
		
		static vector<value (*)(const value& v)> attri_grams;
	
	};

	extern vector<ex_production> ex_produc_set;

	
	

	class nesting_table
	{
	public:
	//if the grammer is with multiple kinds of variable types, 
	//it should be map<class x,int>(x is a class recording the name and type).
		map<string,int> var_map;
		list<nesting_table> nes_body_list;
		nesting_table()
		{
		}
		nesting_table(const nesting_table& table)
		{
			var_map=table.var_map;
			nes_body_list=table.nes_body_list;
		}
		static void stack_push(nesting_table* ptr);
		static void stack_pop();
		static nesting_table* stack_top();
		static int get_offset();
		static void incre_offset(int size);
	private:
		static vector<int> memory_area;
		static int offset_pos;
		static stack<nesting_table* > nestptr_stack;
	};
	//the zeroth layer
	extern nesting_table symbol_table;
	


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
		static map<string,int> var_list;
		static map<string,int> ter_list;
		static set<LR1PG::element> element_set;
		static vector<LR1PG::production> production_set;
		static LR1PG::LR_analysis_table LR_table;

	
		static stack<stack_block> LR_stack;
		static void shift(int state_index,const APT_node& node);
		static value reduction(int produc_index,APT_node& father);

	public:
		void static load_productions(const string& file_name);
		void static load_table(const string& file_name);
		void static analyse(const list<Analyser::ex_element>& ex_e_stream);
		

	};
}