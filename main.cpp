#include "LR1PG.h"
#include "Lexer.h"
#include "Analyser.h"
#include <stack>




void main()
{
	
	
	
	//to get var_list, ter_list, produc_set and set_C:
	LR1PG::load_productions("wenfa.txt");
	
	//to get token_stream:
	Lexer::load_code("a.pp");

	list<LR1PG::element> elem_stream;

	{
		list<string>::iterator it;
		for(it=Lexer::token_stream.begin();it!=Lexer::token_stream.end();it++)
			elem_stream.push_back(LR1PG::element(false,LR1PG::ter_list[*it]));
	}
	
	//still need to load productions and LR table:
	Analyser::LR_analyser::load_productions(string("wenfa.txt"));
	Analyser::LR_analyser::load_table("TABLE.txt");

	
	

	system("pause");
}
