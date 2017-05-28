#pragma once
#include "stdafx.cpp"
#include "C_Tabela_Simbolos.h"

using namespace std;

class C_Mepa
{
private:
	string Cabelho();
	string Rodape();
	stringstream mepa;
	int linha_mepa;
	void Converter_Pos_Fixa(stack<string>&, C_Tabela_Simbolos);
	bool E_Operador(string);
	int Get_Peso_Operador(string);
	bool Validar_Identificador(string);
public:
	//Variáveis
	stack<string> pilha_ARMZ;
	stack<string> pilha_EXP;
	stack<string> pilha_EXP_args;
	vector<string> vetor_EXP;
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
	string DSVF();
	string DSVS();
	void DSVS(string);
	void NADA(string);
	string NADA();

	//Procedimentos
	void CHPR(string, string);
	void ENPR(string);
	void RTPR(string, string);

	//Programa e alocação de memória
	void INPP();
	void PARA();
	void AMEM(string);
	void DMEM(string);

	//Genéricos
	void Add_Comando(string);
	void Avaliar_Expressao(stack<string>&, const C_Tabela_Simbolos&);

	void Gerar_Arquivo(string);
};

