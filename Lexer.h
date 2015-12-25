#pragma   once  

#include<iostream>
#include<string>
#include<list>
#include"Analyser.h"
using namespace std;

namespace Lexer
{
	//#define ELSE 3
	//#define IF 4
	//#define INT 5
	//#define RETURN 6
	//#define VOID 7
	//#define WHILE 8
	#define RESERVELISTSIZE 9
	

	class token:public string,public Analyser::value
	{
	public:
		token():string(),Analyser::value()
		{}
		token(const token& t):string(t),Analyser::value(t)
		{}
		token(const string& str,const string& str_val,int int_val):string(str),Analyser::value(int_val,str_val)
		{}
		token(const string& str,const string& str_val):string(str),Analyser::value(str_val)
		{}
		token(const string& str,int int_val):string(str),Analyser::value(int_val)
		{}
		token(const string& str):string(str),Analyser::value()
		{}
	
	};


	extern char ch;
	extern string code;
	extern int ptr;
	extern string strToken;
	extern string reserveList[RESERVELISTSIZE];
	extern FILE* fp;
	extern int index;
	extern int value;
	extern list<token> token_stream;

	

	void GetChar();
	void GetBC();
	void Concat();
	bool IsLetter();
	bool IsDigit();
	int Reserve();
	void Retract();
	string InsertId();
	string InsertConst();
	void detectOneWord();
	bool deleteNote();//È¥µô×¢ÊÍ
	void load_code(const string& file_name);


}
