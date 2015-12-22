#pragma   once  

#include "LR1PG.h"
#include <stack>

namespace Analyser
{
	class LR_analyser
	{
	private:
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
		static void analyse(const list<LR1PG::element>& elem_stream);


	};
}