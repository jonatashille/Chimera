#pragma once
#include "stdafx.cpp"
#include "S_Token_Lexema.h"
#include "C_Tabela_Simbolos.h"
#include "C_Mepa.h"

using namespace std;

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
	int posicao_pilha_local;
	int posicao_pilha_global;
	C_Mepa mepa;
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
	void Decl_var();
	string Espec_tipo();
	void Decl_proc();
	void Decl_func();
	int Params();
	int Lista_param();
	int Lista_param_1(int);
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
	//Métods para a semântica
	string Retorna_Tipo_Comparado(string, string);
	void Validar_Atribuicao(string, string);

	//Imprimir Arquivos (TS, MEPA)
	void Gravar_Arquivos(string);
};

