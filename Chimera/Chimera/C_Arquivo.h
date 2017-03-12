/****************************************************************************************
Objetivo >> Implementar a Classe Arquivo para a manipula��o de arquivos.

Observa��es >>

Criador >> J�natas Alexandre Hille - 28/04/2013
*****************************************************************************************/
#pragma once
#include "stdafx.cpp"

using namespace std;

class C_Arquivo
{
private:
	fstream arquivo; //atributo para manipular a fstream arquivo
	string linha; // atributo que guardar� o conte�de de uma linha do arquivo
	int numLinha; //Faz o controle do n�mero da linha
public:
	C_Arquivo(const string&);
	~C_Arquivo();

	bool ProximaLinha();
	const string getLinha() const;
	const int getNumeroLinha() const;
	const string getConteudoArquivo();
};

