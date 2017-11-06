#include "TU_Programas.h"



TU_Programas::TU_Programas()
{
}


TU_Programas::~TU_Programas()
{
}

void TU_Programas::Executar()
{
	cout << endl << endl;
	cout << " **********************" << endl;
	cout << " INICIO TESTE PROGRAMAS" << endl;
	cout << " **********************" << endl;
	//Progamas a serem comparados
	Testar_Programa("p01"); //Programa que soma 2 valores
	Testar_Programa("p02"); //Programa para testar as express�es
	Testar_Programa("p03"); //Programa que testa conficional e loops
	Testar_Programa("p04"); //Programa que testa chamada de procedimento e vari�vel global
	Testar_Programa("p05"); //Programa faz passagem por valor e refer�ncia
	Testar_Programa("p06"); //Programa que testa retorno de funcao - Fatorial de 5 - Resultado esperado 120
	Testar_Programa("p07"); //Programa Fibonacci de 20 - Resultado esperado 6765
	Testar_Programa("p08"); //Teste de ponteiros - Troca de valores
	Testar_Programa("p09"); //Teste de ponteiros - Troca de valores chamando uma fun��o para realizar a troca e passando por referencia
	Testar_Programa("p10"); //Teste com passagem de String por par�metro
	Testar_Programa("p11"); //Teste com passagem de par�metro por refer�ncia
	Testar_Programa("p13"); //Teste com passagem de String por par�metro + parametro inteiro
	Testar_Programa("p14"); //Teste com passagem de diversas Strings por par�metro e parametros inteiros

	//Estrutura
	Testar_Programa("S01"); //Estrutura simples
	Testar_Programa("S02"); //Estrutura dentro de estrutura
	//Classes - Orienta��o � objetos
	Testar_Programa("C01"); //Classe simples
	Testar_Programa("C02"); //Classe com procedure
	Testar_Programa("C03"); //Classe com procedure 2
	Testar_Programa("C04"); //Classe dentro de classe
	Testar_Programa("C05"); //Classe com fun��es que retornarm as 4 opera��es b�sicas entre 2 valores
	Testar_Programa("C06"); //Heran�a simples
	Testar_Programa("C07"); //Exemplo de heran�a -> Animal
	Testar_Programa("C08"); //Exemplo de heran�a -> Animal com string
}

void TU_Programas::Testar_Programa(string _prog)
{
	string f1, f2;
	f1 = "Test_Files\\Programas\\" + _prog + ".mep";
	f2 = "Test_Files\\Programas\\" + _prog + "-OK.mep";

	if (arquivo.CompararArquivos(f1, f2))
		cout << _prog << " - OK" << endl;
	else
		cout << _prog << " - FALHA *******" << endl;
}
