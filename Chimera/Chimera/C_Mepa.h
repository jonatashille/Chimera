#pragma once
#include "stdafx.cpp"
#include "C_Tabela_Simbolos.h"

using namespace std;

struct S_EXP
{
	string token;
	bool end_elemento = false;
	int parente;

	S_EXP make_exp(string _token, bool _end_elemento, int _parente)
	{
		S_EXP sexpressao;
		sexpressao.token = _token;
		sexpressao.end_elemento = _end_elemento;
		sexpressao.parente = _parente;
		return sexpressao;
	}

}; typedef S_EXP S_EXP;

class C_Mepa
{
private:
	string Cabelho();
	string Rodape();
	stringstream mepa;
	int linha_mepa;
	void Converter_Pos_Fixa(stack<S_EXP>&, C_Tabela_Simbolos);
	bool E_Operador(string);
	int Get_Peso_Operador(string);
	bool Validar_Identificador(string);
public:
	//Variáveis
	stack<S_Id_Pai> pilha_ARMZ; //Identificador e Pai
	stack<S_EXP> pilha_EXP;
	stack<S_EXP> pilha_EXP_args;
	vector<string> vetor_EXP;
	stack<string> pilha_Com_Escrita;
	string ultimo_comando;
	C_Mepa();
	~C_Mepa();

	//Armazenamento e E/S
	void CRCT(string);
	void CRCT_String(string, int);
	void CRVL(string, string);
	void CRVL_String(string, int, int);//Imprime os caracteres
	void CRVL_String_Param(string, int, int);//Não imprime, apenas carrega os valores da String
	void CRVI(string, string);
	void CRVI_String(string, int, int);//Imprime os caracteres
	void CRVI_String_Param(string, int, int);//Não imprime, apenas carrega os valores da String
	void CREN(string, string);
	void ARMZ(string, string);
	void ARMZ(string);
	void ARMI(string, string);
	void ARMI(string);
	void LEIT();
	void LECH();
	void IMPR();
	void IMPC();
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
	void DMEM(int);

	//Genéricos
	void Add_Comando(string);
	void Avaliar_Expressao(stack<S_EXP>&, const C_Tabela_Simbolos&);

	void Gerar_Arquivo(string);
};

