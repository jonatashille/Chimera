#pragma once
#include "Compilador.h"
#include "Testes.h"

using namespace std;

void main()
{
	//Programas comuns
	Compilar("Test_Files\\Programas\\p01.chi");
	Compilar("Test_Files\\Programas\\p02.chi");
	Compilar("Test_Files\\Programas\\p03.chi");
	Compilar("Test_Files\\Programas\\p04.chi");
	Compilar("Test_Files\\Programas\\p05.chi");
	Compilar("Test_Files\\Programas\\p06.chi");
	Compilar("Test_Files\\Programas\\p07.chi");
	Compilar("Test_Files\\Programas\\p08.chi");
	Compilar("Test_Files\\Programas\\p09.chi");
	Compilar("Test_Files\\Programas\\P10.chi");
	Compilar("Test_Files\\Programas\\P11.chi");
	Compilar("Test_Files\\Programas\\P12.chi");//Falta testes - Problema a repassar para o professor
	Compilar("Test_Files\\Programas\\P13.chi");
	Compilar("Test_Files\\Programas\\P14.chi");
	
	//Programas com estrutura
	Compilar("Test_Files\\Programas\\S01.chi");
	Compilar("Test_Files\\Programas\\S02.chi");
	Compilar("Test_Files\\Programas\\S03.chi");//TODO Continuar a desenvolver este
	//Programas com classes/orientado a objetos
	Compilar("Test_Files\\Programas\\C01.chi");
	Compilar("Test_Files\\Programas\\C02.chi");
	Compilar("Test_Files\\Programas\\C03.chi");
	Compilar("Test_Files\\Programas\\C04.chi");
	Compilar("Test_Files\\Programas\\C05.chi");
	Compilar("Test_Files\\Programas\\C06.chi");
	Compilar("Test_Files\\Programas\\C07.chi");
	Compilar("Test_Files\\Programas\\C08.chi");

	//Testas as opçoes da analise léxica e todos os programas acima que já foram testados
	Executar_Testes_Unitatios();

	system("pause");
}