#pragma   once  

#include<iostream>
#include<string>
#include<list>
using namespace std;

namespace Lexer
{
	#define ELSE 3
	#define IF 4
	#define INT 5
	#define RETURN 6
	#define VOID 7
	#define WHILE 8
	#define N 9
	#define CODENAME "a.cpp"

	extern char ch;
	extern string code;
	extern int ptr;
	extern string strToken;
	extern string reserveList[N];
	extern FILE* fp;
	extern int index;
	extern int value;
	extern list<string> token_stream;

	

	bool loadFile(char* fileName);
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
	void load_and_process();


}
