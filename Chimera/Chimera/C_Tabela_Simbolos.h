#pragma once
#include "stdafx.cpp"

using namespace std;

struct S_Id_Pai
{
	string identificador;
	int parente;

	S_Id_Pai make_Id_Pai(string _identificador, int _parente)
	{
		S_Id_Pai par;
		par.identificador = _identificador;
		par.parente = _parente;
		return par;
	}

}; typedef S_Id_Pai S_Id_Pai;

struct S_Simbolos
{
	int chave;
	string identificador;
	string categoria;
	string tipo;
	bool ponteiro = false;
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
	int pos_pilha_ini_str;
	int tam_str;
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

	//Insere um novo identificador na tabela de s�mbolos
	bool Inserir(S_Simbolos, string);
	//Retorna se o identificador existe ou n�o na tabela de s�mbolos
	bool Consultar(string);
	//Utilizado para inativar os s�mbolos, dado um escopo
	void Inativar_Simbolos(int);
	//Verifica se j� existe a ID na TS
	void Existe_ID(S_Simbolos);
	//Buscar Id pelo Pai de um identificador
	string Buscar_ID_Pai(int);
	//Verificar se Identificador � um Array
	bool Verificar_Array(string);
	//Buscar a chave de um identificador
	int Buscar_Chave(string);
	//Buscar categogia do identificador
	string Buscar_Categoria(string);
	//Buscar tipo do identificador
	string Buscar_Tipo(string);
	//Verifica se � um ponteiro
	bool Verificar_Ponteiro(string);
	//Buscar quantidade de par�metros de uma fun��o/Procedimento
	int Buscar_Qtd_Params(string);
	//Buscar a posi��o da pilha da vari�vel
	int Buscar_Pos_Pilha(S_Id_Pai);
	//Buscar a posi��o da pilha inicial da vari�vel String
	int Buscar_Pos_Pilha_Ini_Str(S_Id_Pai);
	//Buscar o escopo pai
	int Buscar_Pai(string);
	//Busca o r�tulo de um identificador / Usado para Fun��es e Procedimentos
	string Buscar_Rotulo(string);
	//Busca dados da TS pela chave
	vector<S_Simbolos>::iterator Buscar_Simbolo(int);
	//Busca dados da TS pelo identificador
	vector<S_Simbolos>::iterator Buscar_Simbolo(string);
	//Busca dados da TS pelo identificador + pai
	vector<S_Simbolos>::iterator Buscar_Simbolo(S_Id_Pai);
	//Atualizar posi��o da pilha dos par�metros / Se for ref mant�m o valor da pilha da mepa, se for valor atualiza para o valor interno
	void Atualizar_Pilha_Param(int,int, stack<pair<int, int>>&);
	//Atualziar o tamanho da string, quando for deste tipo
	void Atualizar_Tamanho_String(S_Id_Pai, string);
	//Buscar o Tamanho da String
	int Buscar_Tamanho_String(S_Id_Pai);
	//Remover/Inutilizar vari�veis internas/filhos - Retorna quantidade de vari�veis desativadas
	int Remover_Internos(string);
	//Remover/Inutilizar vari�veis globais - Retorna quantidade de vari�veis desativadas
	int Remover_Globais();
	//Gravar arquivo txt com a tabela de s�mbolos
	void Imprimir_TS(string);
	//Gravar arquivo csv com a tabela de s�mbolos
	void Gravar_TS(string);
	//Retorna vari�veis declaradas em uma estrutura
	vector<S_Simbolos> Buscar_Var_Estrutura(string);

	void Erro(string);
	void Erro(string, S_Simbolos);
};

