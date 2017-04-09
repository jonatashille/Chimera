#pragma once
#include "stdafx.cpp"

using namespace std;

struct S_Params
{
	int chave;
	string identificador;
	string categoria = PARAM;
	string tipo;
	string passby;
	int pai;
	int linha;
}; typedef S_Params S_Params;

struct S_Simbolos
{
	int chave;
	string identificador;
	string tipo;
	string categoria;
	string valor;
	int pai;
	bool valido;
	int linha;
	vector<S_Params> params;
	Acesso access;

	//Sobrecarga do operador < para ordenar a TS pela chave
	bool operator < (const S_Simbolos& ts)
	{
		return (chave < ts.chave);
	}

}; typedef S_Simbolos S_Simbolos;

struct S_Atual
{
	int chave;
	string categoria;
	Acesso access;
}; typedef S_Atual S_Atual;

class C_Tabela_Simbolos
{
private:
	int chave;
	//map<int, S_TS_Global> tabela_simbolos;
public:
	C_Tabela_Simbolos();
	~C_Tabela_Simbolos();

	vector<S_Simbolos> tabela_simbolos;

	bool Inserir(S_Simbolos);
	bool Constultar(string);
	string Retornar_Tipo(string);
	bool Remover(string);

	void Imprimir_TS();
};

