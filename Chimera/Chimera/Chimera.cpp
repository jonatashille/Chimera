#pragma once
#include "Compilador.h"
#include "Testes.h"

using namespace std;

void Executar_Testes();

void main()
{
	Executar_Testes();

	system("pause");
}

void Executar_Testes()
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
	Compilar("Test_Files\\Programas\\P15.chi");

	//Programas com estrutura
	Compilar("Test_Files\\Programas\\S01.chi");
	Compilar("Test_Files\\Programas\\S02.chi");
	Compilar("Test_Files\\Programas\\S03.chi");
	
	//Programas com classes/orientado a objetos
	Compilar("Test_Files\\Programas\\C01.chi");
	Compilar("Test_Files\\Programas\\C02.chi");
	Compilar("Test_Files\\Programas\\C03.chi");
	Compilar("Test_Files\\Programas\\C04.chi");
	Compilar("Test_Files\\Programas\\C05.chi");
	Compilar("Test_Files\\Programas\\C06.chi");
	Compilar("Test_Files\\Programas\\C07.chi");
	Compilar("Test_Files\\Programas\\C08.chi");
	Compilar("Test_Files\\Programas\\C09.chi");
	Compilar("Test_Files\\Programas\\C10.chi");
	Compilar("Test_Files\\Programas\\C11.chi");
	Compilar("Test_Files\\Programas\\C12.chi");
	Compilar("Test_Files\\Programas\\C13.chi");
	Compilar("Test_Files\\Programas\\C14.chi");
	Compilar("Test_Files\\Programas\\C15.chi");
	Compilar("Test_Files\\Programas\\C16.chi");
	Compilar("Test_Files\\Programas\\C17.chi");
	Compilar("Test_Files\\Programas\\C18.chi");

	//Programas com Strings
	/*Compilar("Test_Files\\Programas\\T01.chi");
	Compilar("Test_Files\\Programas\\T02.chi");
	Compilar("Test_Files\\Programas\\T03.chi");
	Compilar("Test_Files\\Programas\\T04.chi");
	Compilar("Test_Files\\Programas\\T05.chi");
	Compilar("Test_Files\\Programas\\T06.chi");*/

	//Testas as opçoes da analise léxica e todos os programas acima que já foram testados
	Executar_Testes_Unitatios();
}