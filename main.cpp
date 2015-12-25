#include "LR1PG.h"
#include "Lexer.h"
#include "Analyser.h"
#include <stack>

class CM_attri_gram_set:public Analyser::attri_gram_set
{
public:
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

}ag_set;

class CM_analyser:public Analyser::LR_analyser
{
public:
	static void analyse(const list<LR1PG::element>& e_stream)
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
				if(act.index==6)
					cout<<"6"<<endl;
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
};


void main()
{
	
	

	
	//to get var_list, ter_list, produc_set and set_C:
	LR1PG::load_productions(string("wenfa.txt"));
	
	for(int i=0;i<LR1PG::produc_set.size();i++)
	{
//		Analyser::ex_production((LR1PG::produc_set)[i]);

	
	}

	//to get token_stream:
	Lexer::load_code(string("b.cpp"));
	
	//token_stream to elem_stream:
	list<LR1PG::element> elem_stream;
	{
		list<string>::iterator it;
		for(it=Lexer::token_stream.begin();it!=Lexer::token_stream.end();it++)
			elem_stream.push_back(LR1PG::element(false,LR1PG::ter_list[*it]));
	}
	
	/*
	//load productions and LR table:
	Analyser::LR_analyser::load_productions(string("wenfa.txt"));
	Analyser::LR_analyser::load_table(string("TABLE.txt"));

	Analyser::LR_analyser::analyse(elem_stream);
	*/

	CM_analyser::load_productions(string("wenfa.txt"));
	CM_analyser::load_table(string("TABLE.txt"));
	CM_analyser::analyse(elem_stream);


	system("pause");
}
