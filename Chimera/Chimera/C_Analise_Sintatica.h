#pragma once
#include "stdafx.cpp"
#include "S_Token_Lexema.h"
#include "C_Tabela_Simbolos.h"
#include "C_Mepa.h"

using namespace std;

struct S_Param
{
	int val;
	int ref;
}; typedef S_Param S_Param;

struct S_Selelecao_ID
{
	string idendificador;
	string passby;
	S_Selelecao_ID make_pair(string _idendificador, string _passby)
	{
		S_Selelecao_ID pair;
		pair.idendificador = _idendificador;
		pair.passby = _passby;
		return pair;
	}
}; typedef S_Selelecao_ID S_Selelecao_ID;

class C_Analise_Sintatica
{
private:
	vector<S_Token_Lexema> tabela_token_lexema;
	vector<S_Token_Lexema>::iterator iter_token_lexema;
	S_Token_Lexema token_lexema;
	string token;
	S_Simbolos sdummy;
	S_Simbolos sfuncao;
	S_Simbolos sproc;
	S_Simbolos sconst;
	S_Simbolos svar;
	S_Simbolos sstruct;
	S_Simbolos sclasse;
	S_Simbolos sparams;
	Acesso acesso_membro;
	C_Tabela_Simbolos ts;
	int chave;
	int classe;
	stack<int> pilha_parente;
	//int posicao_pilha_local;
	//int posicao_pilha_global;
	C_Mepa mepa;
	bool existe_main;
	bool existe_CHPR_main;
	bool eh_argumento;
	S_Param qtd_param; //Struct para tratar a quantidade de parâmetros (valor/referência) 
	string Proc_Func_Atual; //Variável que armazena o procedimento/função atual -> Auxilia na construção da MEPA
	stack<int> pilha_var_mem; //Contém a quantidade de memória separada para as variáveis. O Topo da pilha é sempre a função/Procedure corrente.
	int var_mem; //contém a quantidade de variáveis declaradas em uma linha de declaração. Será usada para o AMEM
	S_EXP sexpressao;
	S_Id_Pai sidpai;
	bool escopo_estrutura; //Para verificar se as declarações estão sendo feitas dentra de uma estrutura
	bool escopo_classe; //Para verificar se as declarações estão sendo feitas dentra de uma classe
	string escopo;
	stack<S_Selelecao_ID> pilha_selecao;
	string val_string;
public:
	C_Analise_Sintatica(vector<S_Token_Lexema>);
	~C_Analise_Sintatica();

	void Iniciar();

	void Proximo_Token_Lexema();
	string Proximo_Token();

	void Erro(string);
	void Erro(string, string);
	string Aceitar_Token(string, string);

	//Declarações
	void Programa(); //P
	void Lista_decl();
	void Decl();
	void Decl_const();
	void Decl_const_class(); // Classe
	void Decl_var();
	void Decl_var_class(); // Classe
	string Espec_tipo();
	void Decl_proc();
	void Decl_proc_class(); // Classe
	void Decl_func();
	void Decl_func_class(); //Classe
	int Params();
	int Lista_param();
	void Lista_param_1();
	void Param();
	void Mode();
	void Decl_struct();
	void Lista_decl_struct();
	void Decl_class();
	void Bloco_class();
	void Espec_acesso();
	void Lista_membro_class();
	void Membro_class();

	//Comandos
	void Bloco();
	void Lista_com();
	void Comando();
	int Comando_1(string);
	void Com_selecao();
	void Com_selecao_1();
	void Com_repeticao();
	void Com_desvio();
	void Com_leitura();
	void Com_escrita();
	string Id_Composto();

	//Expressões
	int Lista_exp();
	int Lista_exp_1();
	string Exp();
	void Exp_1(string);
	void Op_relac();
	string Exp_soma();
	string Exp_soma_1(string);
	void Op_soma();
	string Exp_mult();
	string Exp_mult_1(string);
	void Op_mult();
	string Exp_simples();
	int Exp_simples_1();
	string Literal(S_Simbolos&);
	void Valor_verdade(S_Simbolos&);
	int Args();
	void Lista_var();
	void Lista_var_1();
	void Lista_var_2();
	void Lista_decl_var();
	void Lista_decl_var_1();
	void Lista_decl_var_2();
	void Op_unario();

	//Outros métodos
	void Iniciar_Simbolos(S_Simbolos&);
	void Inserir_AMEM_MEPA();
	void Inserir_DMEM_MEPA(int);
	void Inserir_AMEM_MEPA_STRUCT(string, int);
	bool Verificar_Tipo_Padrao(string);
	//Métods para a semântica
	string Retorna_Tipo_Comparado(string, string);
	void Validar_Atribuicao(string, string);

	//Imprimir Arquivos (TS, MEPA)
	void Gravar_Arquivos(string);
};
