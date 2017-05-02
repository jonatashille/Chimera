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
	int pos_pilha;

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
	//Buscar a posição da pilha da variável
	int Buscar_Pos_Pilha(string);
	//Buscar o escopo pai
	int Buscar_Pai(string);
	//Busca dados da TS pelo identificador
	vector<S_Simbolos>::iterator Buscar_Simbolo(string);
	//Remover/Inutilizar variáveis internas/filhos - Retorna quantidade de variáveis desativadas
	int Remover_Internos(string);
	//Gravar arquivo txt com a tabela de símbolos
	void Imprimir_TS(string);
	//Gravar arquivo csv com a tabela de símbolos
	void Gravar_TS(string);

	void Erro(string);
	void Erro(string, S_Simbolos);
};

