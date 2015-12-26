#include "Lexer.h"



namespace Lexer
{
	char ch;
	string code;
	int ptr=0;
	string strToken;
	string reserveList[RESERVELISTSIZE]={"","","","else","if","int","return","void","while"};//�����ֱ�
	FILE* fp;
	int index;
	int value;



	list<token> token_stream;
	//list<string> token_stream;

	
	
	
	void GetChar()
	{
		if(code.at(ptr)=='\0')
		{
			printf("Error in GetChar(): ptr pointing at the end");
			return;
		}
		ch=code.at(ptr);
		ptr++;
	
	}
	void GetBC()
	{
		while(ch=='\n'||ch=='\t'||ch==' ')
		if(ptr<code.length ())
			GetChar();
		else
			break;

	}
	void Concat()
	{
		strToken.push_back(ch);
	}
	bool IsLetter()
	{
		return (ch>='a'&&ch<='z')||(ch>='A'&&ch<='Z');
	}
	bool IsDigit()
	{
		return ch>='0'&&ch<='9';
	}
	int Reserve()
	{//��strToken�е��ַ������ұ����ֱ������Ǳ������򷵻��ֱ���룬���򷵻�0��
		for(int i=0;i<RESERVELISTSIZE;i++)
		{
			if(strToken.compare(reserveList[i])==0)
			  return i;
		}
		return 0;
	}
	void Retract()
	{//������ָʾ���ص�һ���ַ�λ�ã���ch��Ϊ�հ���
		if(ptr>0)
			ptr--;
		else
			cout<<"ERROR:�ѻص�����ʼλ�ã��޷������ص���"<<endl;
		ch=NULL;
	}
	string InsertId()
	{//��strToken�б�ʶ��������ű����ط��ű�ָ��
		cout<<strToken<<endl;
		return "null";
	}
	string InsertConst()
	{//��strToken�г������볣�������س�����ָ��
		cout<<strToken<<endl;
		return "null";
	}
	void detectOneWord()
	{
		strToken.clear();
		GetChar();
		
		GetBC();

		if(!(ptr<code.length()))
			return;

		if(IsLetter())
		{
			while(IsLetter())
			{
				Concat();
				GetChar();
			}
			Retract();
			index=Reserve();
			if(index==0)
			{
				//value=InsertId();
				//cout<<"#ID,"<<value;
				InsertId();
				//cout<<"#ID,"<<strToken<<endl;
				token t(string("ID"),strToken);
				
				token_stream.push_back(t);
			}
			else
			{
				//cout<<index<<",-"<<endl;
				token_stream.push_back(token(string(reserveList[index])));
			}
		}
		else if(IsDigit())
		{
			while(IsDigit())
			{
				Concat();
				GetChar();
			}
			Retract();
			//value=InsertConst();
			//cout<<"#NUM,"<<value;
			InsertConst();
			//cout<<"#NUM,"<<strToken<<endl;
			token_stream.push_back(token(string("NUM"),atoi(strToken.c_str())));
			
		}
		else if(ch=='+') 
		{
			//cout<<"#PLUS,-"<<endl;
			token_stream.push_back(token(string("+")));
			
			
		}
		else if(ch=='-') 
		{
			//cout<<"#MINUS,-"<<endl;
			token_stream.push_back(token(string("-")));
			
			
		}
		else if(ch=='*')
		{
			//cout<<"#MULI,-"<<endl;
			token_stream.push_back(token(string("*")));
			
			
		}
		else if(ch=='/')
		{
			//cout<<"#DIV,-"<<endl;
			token_stream.push_back(token(string("/")));
			
			
		}
		else if(ch=='<')//<=
		{
			GetChar();
			if(ch=='=')
			{
				//cout<<"#LETHAN,-"<<endl;
				token_stream.push_back(token(string("<=")));
				
			}
			else 
			{
				//Retract();cout<<"#LTHAN,-"<<endl;
				token_stream.push_back(token(string("<")));
				
				
			}
		}
		else if(ch=='>')//>=
		{
			GetChar();
			if(ch=='=')
			{
				//cout<<"#GETHAN,-"<<endl;
				token_stream.push_back(token(string(">=")));
			
			
			}
			else 
			{
				Retract();
				//cout<<"#GTHAN,-"<<endl;
				token_stream.push_back(token(string(">")));
				
				
			}
		}
		else if(ch=='=')//==
		{
			GetChar();
			if(ch=='=') 
			{
				//cout<<"#EQUAL,-"<<endl;
				token_stream.push_back(token(string("==")));
				
			}
			else
			{
				Retract();
				//cout<<"#ASSIGN,-"<<endl;
				token_stream.push_back(token(string("=")));
				
			}
		}
		else if(ch=='!')
		{
			GetChar();
			if(ch=='=')
			{
				//cout<<"#NEQUAL,-"<<endl;
				token_stream.push_back(token(string("!=")));
				
				
			}
			else 
			{
				//cout<<"ERROR:!"<<endl;
				token_stream.push_back(token(string("=")));
				
			}
		}
		else if(ch==';') 
		{
			//cout<<"#SEMICOLON,-"<<endl;
			token_stream.push_back(token(string(";")));
			
		}
		else if(ch==',')
		{
			//cout<<"#COMMA,-"<<endl;
			
			token_stream.push_back(token(string(",")));
			
		}
		else if(ch=='(')
		{
			//cout<<"#LBRACKET,-"<<endl;
			
			token_stream.push_back(token(string("(")));
			
		}
		else if(ch==')')
		{
			//cout<<"#RBRACKET,-"<<endl;
			token_stream.push_back(token(string(")")));
			
			
		}
		else if(ch=='[')
		{
			//cout<<"#LSRACKET,-"<<endl;
			token_stream.push_back(token(string("[")));
			
		}
		else if(ch==']')
		{
			//cout<<"#RSRACKET,-"<<endl;
			
			token_stream.push_back(token(string("]")));
			
		}
		else if(ch=='{')
		{
			//cout<<"#CLRACKET,-"<<endl;
			token_stream.push_back(token(string("{")));
			
			
		}
		else if(ch=='}')
		{
			//cout<<"#CRRACKET,-"<<endl;
			token_stream.push_back(token(string("}")));
			
			
		}
		else if(ch==' ') ;//ĩβ�ո�Ĵ�������
		else 
		{
			cout<<"error"<<endl;
			//token_stream.push_back(string("ERROR"));
		}
	}
	bool deleteNote()
	{//ȥ��ע�ͣ���Ϊ�ո�
		while(1)
		{
			size_t pos1=code.find("/*");
			if(pos1==string::npos)
			   if(code.find("*/")==string::npos)
					return true;
			   else 
				   return false;
			else
				code.replace (pos1,2,"  ");//��ֹ/*/   ����
			size_t pos2=code.find("*/");
			if(pos2==string::npos)
				return false;
			else
				code.replace(pos1,pos2-pos1+2," ");
		
		}
	}
	void load_code(const string& file_name)
	{

		FILE* fp;
		char c;
		fp=fopen(file_name.c_str(),"r");
		if(!fp)
		{
			cout<<"Error in loading."<<endl;
			return;
		}
		while((c=fgetc(fp))!=EOF)
		{
			code.push_back(c);
		}
		

		if(!deleteNote())
		{
			cout<<"Error in deletint note."<<endl;
			return;
		}
		cout<<code<<endl;
		while(ptr<code.length())
			  detectOneWord();
		cout<<"Lexing done."<<endl;
   
	}

}

