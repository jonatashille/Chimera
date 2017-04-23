#pragma once
#include "stdafx.cpp"

using namespace std;

struct S_Simbolos
{
	int chave;
	string identificador;
	string categoria;
	string tipo;
	bool array = false;
	string valor;
	int qtd_params;
	string passby;
	int pai;
	int classe;
	Acesso access;
	bool valido;
	int linha;

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
	//Verifica se já existe a ID na TS
	void Existe_ID(S_Simbolos);
	//Verificar se Identificador é um Array
	bool Verificar_Array(string);
	//Buscar categogia do identificador
	string Buscar_Categoria(string);
	//Buscar tipo do identificador
	string Buscar_Tipo(string);
	//Buscar quantidade de parâmetros de uma função/Procedimento
	int Buscar_Qtd_Params(string);
	//Busca dados da TS pelo identificador
	vector<S_Simbolos>::iterator Buscar_Simbolo(string);
	void Imprimir_TS();

	void Erro(string);
	void Erro(string, S_Simbolos);
};

