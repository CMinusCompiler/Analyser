#include "LR1PG.h"
#include "Lexer.h"
#include "Analyser.h"
#include <stack>




void main()
{
	
	
	
	//to get var_list, ter_list, produc_set and set_C:
	LR1PG::load_productions(string("wenfa.txt"));
	
	//to get token_stream:
	Lexer::load_code(string("a.cpp"));
	
	//token_stream to elem_stream:
	list<LR1PG::element> elem_stream;

	{
		list<string>::iterator it;
		for(it=Lexer::token_stream.begin();it!=Lexer::token_stream.end();it++)
			elem_stream.push_back(LR1PG::element(false,LR1PG::ter_list[*it]));
	}
	
	//load productions and LR table:
	Analyser::LR_analyser::load_productions(string("wenfa.txt"));
	Analyser::LR_analyser::load_table(string("TABLE.txt"));

	Analyser::LR_analyser::analyse(elem_stream);
	

	system("pause");
}
