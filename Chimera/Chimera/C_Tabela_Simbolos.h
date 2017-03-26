#pragma once
#include "stdafx.cpp"

using namespace std;

struct S_TS_Local
{
	int chave;
	string identificador;
	string tipo;
	string categoria;
	string valor;
	string passby;
	bool parametro;
	int params;
	//string escopo;
	bool valido;
}; typedef struct S_TS_Local S_TS_Local;

struct S_TS_Global
{
	int chave;
	string identificador;
	string tipo;
	string categoria;
	string valor;
	string passby;
	bool parametro;
	int params;
	//string escopo;
	bool valido;
	map<int, S_TS_Local> local;
}; typedef S_TS_Global S_TS_Global;

struct S_Simbolos
{
	int chave;
	string identificador;
	string tipo;
	string categoria;
	string valor;
	string passby;
	bool parametro;
	int params;
	string escopo;
	bool valido;
	int linha;
}; typedef S_Simbolos S_Simbolos;

class C_Tabela_Simbolos
{
private:
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

