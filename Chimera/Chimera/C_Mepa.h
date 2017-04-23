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
	C_Mepa();
	~C_Mepa();

	//Armazenamento e E/S
	void CRCT(string);
	void CRVL(string, string);
	void LEIT();
	void IMPR();

	//Expressões
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

	//Programa e alocação de memória
	void INPP();
	void PARA();
	void AMEM(string);
	void DMEM(string);

	void Gerar_Arquivo(string);
};

