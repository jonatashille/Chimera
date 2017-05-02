#pragma once
#include "stdafx.cpp"

using namespace std;

class C_Mepa
{
private:
	string Cabelho();
	string Rodape();
	stringstream mepa;
public:
	//Vari�veis
	stack<string> pilha_ARMZ;
	stack<string> pilha_EXP;
	stack<string> pilha_Com_Escrita;
	C_Mepa();
	~C_Mepa();

	//Armazenamento e E/S
	void CRCT(string);
	void CRCT_String(string);
	void CRVL(string, string);
	void ARMZ(string, string);
	void ARMZ(string);
	void LEIT();
	void LECH();
	void IMPR();
	void IMPC(string);
	void IMPE();

	//Express�es
	void SOMA();
	void SUBT();
	void MULT();
	void DIVI();
	void MODI();
	void CONJ();
	void DISJ();
	void NEGA();
	void CMME();
	void CMMA();
	void CMIG();
	void CMDG();
	void CMAG();
	void CMEG();
	void INRV();

	//Desvios
	void DSVF(string);
	void DSVS(string);
	void NADA();

	//Procedimentos
	void CHPR(string, string);
	void ENPR(string);
	void RTPR(string, string);

	//Programa e aloca��o de mem�ria
	void INPP();
	void PARA();
	void AMEM(string);
	void DMEM(string);

	//Gen�ricos
	void Add_Comando(string);

	void Gerar_Arquivo(string);
};

