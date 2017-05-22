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
	string rotulo;
	int k; //Endere�o L�xico
	int n; //Endere�o de mem�ria (Usado para par�metros, primeiro inicia com -4, pr�ximos -5,-6...)

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
	bool Consultar(string);
	//Utilizado para inativar os s�mbolos, dado um escopo
	void Inativar_Simbolos(int);
	//Verifica se j� existe a ID na TS
	void Existe_ID(S_Simbolos);
	//Verificar se Identificador � um Array
	bool Verificar_Array(string);
	//Buscar categogia do identificador
	string Buscar_Categoria(string);
	//Buscar tipo do identificador
	string Buscar_Tipo(string);
	//Buscar quantidade de par�metros de uma fun��o/Procedimento
	int Buscar_Qtd_Params(string);
	//Buscar a posi��o da pilha da vari�vel
	int Buscar_Pos_Pilha(string);
	//Buscar o escopo pai
	int Buscar_Pai(string);
	//Busca o r�tulo de um identificador / Usado para Fun��es e Procedimentos
	string Buscar_Rotulo(string);
	//Busca dados da TS pelo identificador
	vector<S_Simbolos>::iterator Buscar_Simbolo(string);
	//Atualizar posi��o da pilha dos par�metros / Se for ref mant�m o valor da pilha da mepa, se for valor atualiza para o valor interno
	void Atualizar_Pilha_Param(int,int, stack<pair<int, int>>&);
	//Remover/Inutilizar vari�veis internas/filhos - Retorna quantidade de vari�veis desativadas
	int Remover_Internos(string);
	//Remover/Inutilizar vari�veis globais - Retorna quantidade de vari�veis desativadas
	int Remover_Globais();
	//Gravar arquivo txt com a tabela de s�mbolos
	void Imprimir_TS(string);
	//Gravar arquivo csv com a tabela de s�mbolos
	void Gravar_TS(string);

	void Erro(string);
	void Erro(string, S_Simbolos);
};

