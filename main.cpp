#include "LR1PG.h"
#include "Lexer.h"
#include "Analyser.h"
#include <stack>




void main()
{
	LR1PG::load_productions("wenfa.txt");
	LR1PG::generate_table();
	system("pause");
	//to get var_list, ter_list, produc_set and set_C:
	LR1PG::load_productions("wenfa.txt");
	LR1PG::set_C_construction();
	//to get token_stream:
	Lexer::load_and_process();

	list<LR1PG::element> elem_stream;

	{
		list<string>::iterator it;
		for(it=Lexer::token_stream.begin();it!=Lexer::token_stream.end();it++)
			elem_stream.push_back(LR1PG::element(false,LR1PG::ter_list[*it]));
	}
	
	//still need to load LR_table:
	//...

	Analyser::LR_analyser::load_productions("wenfa.txt");
	

	system("pause");
}
