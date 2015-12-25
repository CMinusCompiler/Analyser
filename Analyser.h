#pragma   once  

#include "LR1PG.h"
#include<stdarg.h>
#include <stack>

namespace Analyser
{

	class ex_element:public LR1PG::element
	{
	public:
		vector<int> values;
		ex_element():LR1PG::element(){}
		ex_element(bool isVar,int index,int a,...):LR1PG::element(isVar,index)
		{
			va_list pArg;
			va_start(pArg,a);
			for(int i = 0; i != a; ++i)
			{
				values[i]=va_arg(pArg,int);
			}
			va_end(pArg);
		}
		ex_element(const ex_element& e):LR1PG::element(e)
		{
			values=e.values;
		}
	};

	class APT_node
	{
	public:
		ex_element content;

		vector<APT_node> children;

	};

	class APT
	{
	public:
		static APT_node root;
		static stack<APT_node> constru_stack;

		static void print();
	};
	
	
	class value
	{
	
	public:
		
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
	private:
		vector<vector<int> > int_val_array;
		vector<vector<string> > str_val_array;
	};
	

	class ex_production:public LR1PG::production
	{
	public:
		value val;
		ex_production():LR1PG::production()
		{}
		ex_production(const ex_production& e):LR1PG::production(e)
		{
			gram_index=e.gram_index;
			val=e.val;
		}
		void set_gram(int index)
		{
			gram_index=index;
		}
		value call_attri_gram(const value& v)
		{
			return (attri_grams[gram_index])(v);
		}
		static int add_gram(value (*g)(const value& v))
		{
			attri_grams.push_back(g);
			return vecotr_ptr++;
		}
		

		
	private:
		int gram_index;
		
		static int vecotr_ptr;
		static vector<value (*)(const value& v)> attri_grams;
	
	};

	extern vector<ex_production> ex_produc_set;

	
	//$$
	class attri_gram_set
	{
	public:
		virtual value ag0(const value& v)
		{return v;}
		virtual value ag1(const value& v)
		{return v;}
		virtual value ag2(const value& v){return v;}
		virtual value ag3(const value& v){return v;}
		virtual value ag4(const value& v){return v;}
		virtual value ag5(const value& v){return v;}
		virtual value ag6(const value& v){return v;}
		virtual value ag7(const value& v){return v;}
		virtual value ag8(const value& v)
		{return v;}
		virtual value ag9(const value& v)
		{return v;}
		virtual value ag10(const value& v){return v;}
		virtual value ag11(const value& v){return v;}
		virtual value ag12(const value& v){return v;}
		virtual value ag13(const value& v){return v;}
		virtual value ag14(const value& v){return v;}
		virtual value ag15(const value& v){return v;}
		virtual value ag16(const value& v){return v;}
		virtual value ag17(const value& v){return v;}
		virtual value ag18(const value& v){return v;}
		virtual value ag19(const value& v){return v;}
		virtual value ag20(const value& v){return v;}
		virtual value ag21(const value& v){return v;}
		virtual value ag22(const value& v){return v;}
		virtual value ag23(const value& v){return v;}
		virtual value ag24(const value& v){return v;}
		virtual value ag25(const value& v){return v;}
		virtual value ag26(const value& v){return v;}
		virtual value ag27(const value& v){return v;}
		virtual value ag28(const value& v){return v;}
		virtual value ag29(const value& v){return v;}
		virtual value ag30(const value& v){return v;}
		virtual value ag31(const value& v){return v;}
		virtual value ag32(const value& v){return v;}
		virtual value ag33(const value& v){return v;}
		virtual value ag34(const value& v){return v;}
	};
	//$$

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
		class stack_block
		{
		public:
			int state_index;
			LR1PG::element elem;
			stack_block(int index,LR1PG::element e)
			{
				state_index=index;
				elem=e;
			}
			stack_block(const stack_block& block)
			{
				this->state_index=block.state_index;
				this->elem=block.elem;
			}
		};
		static map<string,int> var_list;
		static map<string,int> ter_list;
		static set<LR1PG::element> element_set;
		static vector<LR1PG::production> production_set;
		static LR1PG::LR_analysis_table LR_table;

	
		static stack<stack_block> LR_stack;
		static void shift(int state_index,const LR1PG::element& elem);
		static void reduction(int produc_index);

	public:
		void static load_productions(const string& file_name);
		void static load_table(const string& file_name);
		static void analyse(const list<LR1PG::element>& e_stream);


	};
}