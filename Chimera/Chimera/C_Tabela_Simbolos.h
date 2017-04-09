#pragma once
#include "stdafx.cpp"

using namespace std;

struct S_Simbolos
{
	int chave;
	string identificador;
	string tipo;
	string categoria;
	string valor;
	string passby;
	int pai;
	int classe;
	bool valido;
	int linha;
	Acesso access;

	//Sobrecarga do operador < para ordenar a TS pela chave
	bool operator < (const S_Simbolos& ts)
	{
		return (chave < ts.chave);
	}

}; typedef S_Simbolos S_Simbolos;

class C_Tabela_Simbolos
{
private:
	int chave;
public:
	C_Tabela_Simbolos();
	~C_Tabela_Simbolos();

	vector<S_Simbolos> tabela_simbolos;

	bool Inserir(S_Simbolos);
	bool Constultar(string);
	string Retornar_Tipo(string);
	bool Remover(string);
	//Utilizado para inativar os símbolos, dado um escopo
	void Inativar_Simbolos(int);

	void Existe_ID(S_Simbolos);

	void Imprimir_TS();

	void Erro(string, S_Simbolos);
};

