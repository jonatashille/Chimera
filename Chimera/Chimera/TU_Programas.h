#pragma once
#include "stdafx.cpp"
#include "C_Arquivo.h"

class TU_Programas
{
private:
	C_Arquivo arquivo;
public:
	TU_Programas();
	~TU_Programas();

	void Executar();

	void Testar_Programa(string);

};

