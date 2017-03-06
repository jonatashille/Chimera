#pragma once

#include "stdafx.cpp"

using namespace std;

struct S_Token_Lexema
{
	// TODO 3 : Explicar o motivo dessa struct
	string token;
	string lexema;
	int linha;
}; typedef struct S_Token_Lexema S_Token_Lexema;
