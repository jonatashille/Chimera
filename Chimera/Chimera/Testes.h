#pragma once
#include "TU_Analise_Lexica.h"
#include "TU_Programas.h"

using namespace std;

void Executar_Testes_Unitatios()
{
	TU_Analise_Lexica tu_lexico;
	tu_lexico.Executar();

	TU_Programas tu_programas;
	tu_programas.Executar();

}
