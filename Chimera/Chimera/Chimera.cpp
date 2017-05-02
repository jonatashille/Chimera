#pragma once
#include "Compilador.h"
#include "Testes.h"

using namespace std;

string Validar_Exp(string _exp);
string Exp(stack<char>, stack<char>);

void main()
{

	//Compilar("Test_Files\\TF_001.chi");
	//Compilar("Test_Files\\TF_002.chi");
	//Compilar("Test_Files\\TF_003.chi");
	//Compilar("Test_Files\\TF_004.chi");
	//Compilar("Test_Files\\TF_005.chi");
	//Compilar("Test_Files\\Programas\\p01.chi");
	Compilar("Test_Files\\Programas\\p02.chi");
	//Compilar("Test_Files\\Programas\\p03.chi");
	//Compilar("Test_Files\\Programas\\p04.chi");
	//Compilar("Test_Files\\Programas\\p06.chi");
	//Compilar("Test_Files\\Programas\\p12.chi");
	//Executar_Testes_Unitatios();

	//string exp = "a+b*c";
	//cout << Validar_Exp(exp) << endl;

	//system("pause");
}

string Validar_Exp(string _exp)
{
	stack<char> vl, op;
	for (auto it = _exp.begin(); it != _exp.end(); it++)
	{
		switch (*it) {
		case '+': op.push(*it); break;
		case '-': op.push(*it); break;
		case '*': op.push(*it); break;
		case '/': op.push(*it); break;
		default: vl.push(*it); break;
		}
	}

	return "";
}

string Exp(stack<char> _vl, stack<char> _op)
{
	stringstream ss;
	if (!_op.empty())
	{
		ss << _vl.top(); _vl.pop();
		ss << _vl.top(); _vl.pop();
	}
	while (!_vl.empty())
	{
		ss << _vl.top(); _vl.pop();
		ss << _vl.top(); _vl.pop();
		if (_op.top() == '*' || _op.top() == '/')
		{
			if (_vl.empty())
			{
				ss << _op.top();
				return ss.str();
			}
			else
			{
				ss << _vl.top(); _vl.pop();
			}
			
		}
	}
	return "";
}