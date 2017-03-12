#pragma once
#include "stdafx.cpp"
#include "S_Token_Lexema.h"

using namespace std;


class C_Analise_Lexica
{
private:
	S_Token_Lexema token_lexema;
	vector<S_Token_Lexema> tabela_token_lexema;
	string lexema;
	bool continua;
	string conteudo;
	string::iterator posicao;
	int estado;
	char ch;
	pair <string, string> resultado;
	vector<pair <string, string>> palavras_reservadas;
	int numLinha;
public:

	//Construtor Análise Léxica
	C_Analise_Lexica(string);
	~C_Analise_Lexica();

	void Iniciar();
	void Proximo_Token();

	void Montar_Tabela_Palavras_Reservadas();

	void Realizar_Analise_Lexica();
	char ProximoChar();
	bool deletar = true;
	void Estado_01();
	void Estado_02();
	void Estado_03();
	void Estado_04();
	void Estado_05();
	void Estado_06();
	void Estado_07();
	void Estado_08();
	void Estado_09();
	void Estado_10();
	void Estado_11();
	void Estado_12();
	void Estado_13();
	void Estado_14();
	void Estado_15();
	void Estado_16();
	void Estado_17();
	void Estado_18();
	void Estado_19();
	void Estado_20();
	bool Validar_Digito();
	bool Validar_Caractere();
	bool Validar_Identificador();
	string Buscar_Palavra_Reservada();
	void Voltar_Um();
	void Set_Resultado(string, string);
	void ERRO(string, string);
	void ERRO(string);
	bool Voltar_Estado_01();
	void Incrementar_Linha();

	vector<S_Token_Lexema> Get_Tabela_Token_Lexema();
	void Gravar_Tabela_Token(string);
};

