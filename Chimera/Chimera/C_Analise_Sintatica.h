#pragma once
#include "stdafx.cpp"
#include "S_Token_Lexema.h"
#include "C_Tabela_Simbolos.h"

using namespace std;

class C_Analise_Sintatica
{
private:
	vector<S_Token_Lexema> tabela_token_lexema;
	vector<S_Token_Lexema>::iterator iter_token_lexema;
	S_Token_Lexema token_lexema;
	string token;
	S_Simbolos simbolos;
	C_Tabela_Simbolos ts;
public:
	C_Analise_Sintatica(vector<S_Token_Lexema>);
	~C_Analise_Sintatica();

	void Iniciar();

	void Proximo_Token_Lexema();
	string Proximo_Token();

	void Erro(string);
	string Aceitar_Token(string, string);

	//Declarações
	void Programa(); //P
	void Lista_decl();
	void Lista_decl_1();
	void Decl();
	void Decl_const();
	void Decl_var();
	string Espec_tipo();
	void Decl_proc();
	void Decl_func();
	void Params();
	void Lista_param();
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
	void Comando_1();
	void Com_selecao();
	void Com_selecao_1();
	void Com_repeticao();
	void Com_desvio();
	void Com_leitura();
	void Com_escrita();
	void Id_Composto();

	//Expressões
	void Lista_exp();
	void Lista_exp_1();
	void Exp();
	void Exp_1();
	void Op_relac();
	void Exp_soma();
	void Exp_soma_1();
	void Op_soma();
	void Exp_mult();
	void Exp_mult_1();
	void Op_mult();
	void Exp_simples();
	void Exp_simples_1();
	void Literal();
	void Valor_verdade();
	void Args();
	void Lista_var();
	void Lista_var_1();
	void Lista_var_2();
	void Lista_decl_var();
	void Lista_decl_var_1();
	void Lista_decl_var_2();
	void Op_unario();

	//Outros métodos
	void Limpar_Simbolos();
};

