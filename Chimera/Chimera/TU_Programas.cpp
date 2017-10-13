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
	Testar_Programa("p01");
	Testar_Programa("p02");
	Testar_Programa("p03");

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

void TU_Programas::Teste_P01()
{


}

void TU_Programas::Teste_P02()
{
}

void TU_Programas::Teste_P03()
{
}

void TU_Programas::Teste_P04()
{
}

void TU_Programas::Teste_P05()
{
}

void TU_Programas::Teste_P06()
{
}

void TU_Programas::Teste_P07()
{
}

void TU_Programas::Teste_P08()
{
}

void TU_Programas::Teste_P09()
{
}

void TU_Programas::Teste_P10()
{
}

void TU_Programas::Teste_P11()
{
}

void TU_Programas::Teste_P12()
{
}

void TU_Programas::Teste_P13()
{
}

void TU_Programas::Teste_P14()
{
}

void TU_Programas::Teste_Q01()
{
}

void TU_Programas::Teste_Q02()
{
}

void TU_Programas::Teste_Q03()
{
}

void TU_Programas::Teste_S01()
{
}

void TU_Programas::Teste_C01()
{
}
