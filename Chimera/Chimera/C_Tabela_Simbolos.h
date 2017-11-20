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
	int chave; //Chave do registro
	string identificador; //Identificador
	string categoria; //Categoria do identificador
	string tipo; //Tipo de dados
	bool ponteiro; //Identifica se é um ponteiro
	bool array; //Identifica se é um array[]
	string valor; //Armazena o valor de uma constante
	int qtd_params; //Indica a quantidade de parâmetros de uma função/Procedimento
	string passby; //Passagem do parâmetro. VALUE | REF
	int pai; //Indica a chave que o registro pertence;
	int classe; //Indica a classe que o registro pertence
	Acesso access; //Indica o tipo de acesso ao membro. PRIVATE | PROTECTED | PUBLIC
	bool valido; //Indica se o registro ainda está ativo
	int linha; //Linha da declaração
	int pos_pilha; //Posição da pilha - Utilizado para a MEPA
	int pos_pilha_ini_str; //Posição inicial de uma String - MEPA
	int tam_str; //Tamanho da String
	string rotulo; //Armazena o rótulo a ser utilizado na MEPA
	string escopo; //A qual escopo o registro pertence. Utilizado em classes
	int pai_heranca; //Indica a classe pai

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

	//Insere um novo identificador na tabela de símbolos
	bool Inserir(S_Simbolos, string);
	//Retorna se o identificador existe ou não na tabela de símbolos
	bool Consultar(string);
	//Retorna se o identificador é acessível
	bool Consultar_Acesso(string, int);
	//Utilizado para inativar os símbolos, dado um escopo
	void Inativar_Simbolos(int);
	//Verifica se já existe a ID na TS
	void Existe_ID(S_Simbolos, string);
	//Buscar Id pelo Pai de um identificador
	string Buscar_ID_Pai(int);
	//Verificar se Identificador é um Array
	bool Verificar_Array(string);
	//Buscar a chave de um identificador por identificador
	int Buscar_Chave(string);
	//Buscar a chave de um identificador por identificador  + pai
	int Buscar_Chave(S_Id_Pai);
	//Buscar categogia do identificador
	string Buscar_Categoria(string);
	//Buscar categogia pela chave
	string Buscar_Categoria(int);
	//Buscar tipo do identificador
	string Buscar_Tipo(string);
	//Buscar tipo pela chave
	string Buscar_Tipo(int);
	//Verifica se é um ponteiro
	bool Verificar_Ponteiro(string);
	//Buscar quantidade de parâmetros de uma função/Procedimento
	int Buscar_Qtd_Params(string);
	//Buscar a posição da pilha da variável
	int Buscar_Pos_Pilha(S_Id_Pai);
	//Buscar a posição da pilha inicial da variável String
	int Buscar_Pos_Pilha_Ini_Str(S_Id_Pai);
	//Buscar o escopo pai
	int Buscar_Pai(string);
	//Buscar o escopo pai
	int Buscar_Pai(int);
	//Buscar pai do pai - Útil para descobrir o escopo de uma função através do param
	int Buscar_Pai_Pai(int);
	//Buscar tipo pai
	string Buscar_Categoria_Pai(int);
	//Busca a quantidade alocada pelo parametros de uma sub ou functions (Apenas passados por valor)
	int Buscar_Qtd_Mem_Alocada_Params(int);
	//Busca a quantidade alocada pelo parametros de uma sub ou functions (Todos os params, Valor e referencia)
	int Buscar_Qtd_Tot_Params(int);
	//Busca o rótulo de um identificador / Usado para Funções e Procedimentos
	string Buscar_Rotulo(S_Id_Pai);
	//Busca dados da TS pela chave
	vector<S_Simbolos>::iterator Buscar_Simbolo(int);
	//Busca dados da TS pelo identificador
	vector<S_Simbolos>::iterator Buscar_Simbolo(string);
	//Busca dados da TS pelo identificador + pai
	vector<S_Simbolos>::iterator Buscar_Simbolo(S_Id_Pai);
	//Atualizar posição da pilha dos parâmetros / Se for ref mantém o valor da pilha da mepa, se for valor atualiza para o valor interno
	void Atualizar_Pilha_Param(int,int, stack<pair<int, int>>&);
	//Atualizar posição da pilha dos parâmetros da classe / Se for ref mantém o valor da pilha da mepa, se for valor atualiza para o valor interno
	void Atualizar_Pilha_Param_classe(int, int, stack<pair<int, int>>&, int);
	//Atualziar o tamanho da string, quando for deste tipo
	void Atualizar_Tamanho_String(S_Id_Pai, string);
	//Buscar o Tamanho da String
	int Buscar_Tamanho_String(S_Id_Pai);
	//Remover/Inutilizar variáveis internas/filhos - Retorna quantidade de variáveis desativadas
	int Remover_Internos(string);
	//Remover/Inutilizar variáveis globais - Retorna quantidade de variáveis desativadas
	int Remover_Globais();
	//Gravar arquivo txt com a tabela de símbolos
	void Imprimir_TS(string);
	//Gravar arquivo csv com a tabela de símbolos
	void Gravar_TS(string);
	//Retorna os membors declarados em uma estrutura/classe - A Partir do identificador
	vector<S_Simbolos> Buscar_Membros_Classe(string);
	//Retorna os membors declarados em uma estrutura/classe - A Partir da chave
	vector<S_Simbolos> Buscar_Membros_Classe(int);
	//Retorna variáveis declaradas após declarar uma estrutura/classe - Traz as varáveis com o endereço certo da pilha
	vector<S_Simbolos> Buscar_Var_Classe_Pai(int);
	//Retorna variáveis declaradas pelo pai delas
	vector<S_Simbolos> Buscar_Var_Pelo_Pai(int);
	//Retorna quantidade de variáveis declaradas em classes e estruturas
	int Retorna_Tot_Var_Mem_Classe();
	//Verifica se a propriedade é um membro de uma classe
	bool Verifica_Propriedade_Membro_Classe(string, int);
	//Retorna a classe pai de uma classe
	int Buscar_Classe_pai(int);
	//Retorna a classe do identificador
	int Buscar_Classe(string, int);

	void Erro(string);
	void Erro(string, S_Simbolos);
};

