#include "C_Analise_Sintatica.h"




C_Analise_Sintatica::C_Analise_Sintatica(vector<S_Token_Lexema> _tabela_token_lexema)
{
	tabela_token_lexema = _tabela_token_lexema;
	if (tabela_token_lexema.begin() != tabela_token_lexema.end())
		iter_token_lexema = tabela_token_lexema.begin();
	else
	{
		cout << "A TABELA TOKEN X LEXEMA ESTA VAZIA!!!" << endl;
		cout << "E PRECISO GERAR OS TOKENS A PARTIR DA ANALISE LEXICA ANTES DE INICIAR A SINTATICA" << endl;
		system("pause");
		exit(EXIT_FAILURE);
	}
}


C_Analise_Sintatica::~C_Analise_Sintatica()
{
}

void C_Analise_Sintatica::Iniciar()
{
	token = iter_token_lexema->token; //Pego o primeiro token do programa, ja atribuído no construtor
	Programa();
}

void C_Analise_Sintatica::Proximo_Token_Lexema()
{
	if (iter_token_lexema != tabela_token_lexema.end())
		iter_token_lexema++;
}

//Método que apenas retorna o proximo token, sem avançar até ele
string C_Analise_Sintatica::Proximo_Token()
{
	vector<S_Token_Lexema>::iterator tmp_iter_token_lexema;
	tmp_iter_token_lexema = iter_token_lexema + 1;

	return tmp_iter_token_lexema->token;
}

void C_Analise_Sintatica::Erro(string _msg)
{
	cout << "ERRO-Linha: " << iter_token_lexema->linha << " - " << iter_token_lexema->lexema << " -> " << _msg << endl;
	system("pause");
	exit(EXIT_FAILURE);
}

string C_Analise_Sintatica::Aceitar_Token(string _token, string _msg)
{
	string lexema;
	if (token == _token)
	{
		lexema = iter_token_lexema->lexema;
		Proximo_Token_Lexema();
		token = iter_token_lexema->token;
	}
	else
		Erro(_msg);

	return lexema;
}

//P
void C_Analise_Sintatica::Programa()
{
	Lista_decl();
	if (token == FIM_PROGRAMA)
		cout << "Compilacao - OK" << endl;
	else
		cout << "Erro de compilacao" << endl;
}

//LD
void C_Analise_Sintatica::Lista_decl()
{
	if (token == CONSTANTE ||
		token == FUNCTION ||
		token == SUB ||
		token == VAR ||
		token == TIPO_STRUCT ||
		token == TIPO_CLASS)
	{
		Decl();
		Lista_decl();
	}
	else if (token != FIM_PROGRAMA)
		Erro("Esperado lista de declaracao");
	/*	Lista_decl_1();
	}
	else
		Erro("Esperado lista de declaracao");*/
}

//LD'
void C_Analise_Sintatica::Lista_decl_1()
{
	if (token == CONSTANTE ||
		token == FUNCTION ||
		token == SUB ||
		token == VAR ||
		token == TIPO_STRUCT ||
		token == TIPO_CLASS)
	{
		Lista_decl();
	}
	else if (token != FIM_PROGRAMA)
		Erro("Esperado lista de declaracao");
}

//D
void C_Analise_Sintatica::Decl()
{
	//Simbolo
	simbolos.escopo = GLOBAL;
	if (token == CONSTANTE)
	{
		Decl_const();
	}
	else if (token == FUNCTION)
	{
		Decl_func();
	}
	else if (token == SUB)
	{
		Decl_proc();
	}
	else if (token == VAR)
	{
		Decl_var();
	}
	else if (token == TIPO_STRUCT)
	{
		Decl_struct();
	}
	else if (token == TIPO_CLASS)
	{
		Decl_class();
	}
	else
		Erro("Esperado declaracao");
}

//DC
void C_Analise_Sintatica::Decl_const()
{
	Aceitar_Token(CONSTANTE, ERR_CONTANTE);

	//Simbolo
	simbolos.categoria = CONSTANTE;

	simbolos.identificador = Aceitar_Token(IDENTIFICADOR, ERR_IDENTIFICADOR);

	Aceitar_Token(OP_ATRIBUICAO, ERR_OP_ATRIBUICAO);

	Literal();

	simbolos.linha = iter_token_lexema->linha;
	ts.Inserir(simbolos);
	Limpar_Simbolos();

	Aceitar_Token(PONTO_VIRGULA, ERR_PONTO_VIRGULA);
}

//DV
void C_Analise_Sintatica::Decl_var()
{
	Aceitar_Token(VAR, ERR_VAR);

	simbolos.tipo = Espec_tipo();
	Lista_decl_var();

	Aceitar_Token(PONTO_VIRGULA, ERR_PONTO_VIRGULA);
}

string C_Analise_Sintatica::Espec_tipo()
{
	if (token == TIPO_BOOLEANO)
	{	
		Aceitar_Token(TIPO_BOOLEANO, ERR_TIPO_BOOLEANO);
	return TIPO_BOOLEANO;
	}
	else if (token == TIPO_CHAR)
	{
		Aceitar_Token(TIPO_CHAR, ERR_TIPO_CHAR);
		return TIPO_CHAR;
	}
	else if (token == TIPO_FLOAT)
	{
		Aceitar_Token(TIPO_FLOAT, ERR_TIPO_FLOAT);
		return TIPO_FLOAT;
	}
	else if (token == TIPO_INT)
	{
		Aceitar_Token(TIPO_INT, ERR_TIPO_INT);
		return TIPO_INT;
	}
	else if (token == TIPO_STRING)
	{
		Aceitar_Token(TIPO_STRING, ERR_TIPO_STRING);
		return TIPO_STRING;
	}
	else
		Erro("Esperado tipo");
}

//DP
void C_Analise_Sintatica::Decl_proc()
{
	Aceitar_Token(SUB, ERR_SUB);

	simbolos.linha = iter_token_lexema->linha;
	simbolos.identificador = Aceitar_Token(IDENTIFICADOR, ERR_IDENTIFICADOR);
	simbolos.escopo = LOCAL; //TODO 1 Nao deve ser so local aqui
	simbolos.categoria = SUB;
	ts.Inserir(simbolos);

	Aceitar_Token(ABRE_PARENTESES, ERR_ABRE_PARENTESES);

	Params();

	Aceitar_Token(FECHA_PARENTESES, ERR_FECHA_PARENTESES);

	Bloco();

	Aceitar_Token(END_SUB, ERR_END_SUB);
}

//DF
void C_Analise_Sintatica::Decl_func()
{
	Aceitar_Token(FUNCTION, ERR_FUNCTION);

	Espec_tipo();

	Aceitar_Token(IDENTIFICADOR, ERR_IDENTIFICADOR);
	
	Aceitar_Token(ABRE_PARENTESES, ERR_ABRE_PARENTESES);

	Params();

	Aceitar_Token(FECHA_PARENTESES, ERR_FECHA_PARENTESES);

	Bloco();

	Aceitar_Token(END_FUNCTION, ERR_END_FUNCTION);
}

//PS
void C_Analise_Sintatica::Params()
{
	if (token == VAR)
	{
		Lista_param();
	}
	else if (token != FECHA_PARENTESES)
		Erro(ERR_FECHA_PARENTESES);
}

//LP
void C_Analise_Sintatica::Lista_param()
{
	Param();
	Lista_param_1();
}

//LP'
void C_Analise_Sintatica::Lista_param_1()
{
	if (token == VIRGULA)
	{
		Aceitar_Token(VIRGULA, ERR_VIRGULA);
		Param();
		Lista_param_1();
	}
	else if (token != FECHA_PARENTESES)
		Erro(ERR_FECHA_PARENTESES);
}

//PM
void C_Analise_Sintatica::Param()
{
	Aceitar_Token(VAR, ERR_VAR);

	Espec_tipo();

	Lista_decl_var();

	Aceitar_Token(BY, ERR_BY);

	Mode();

	simbolos.parametro = true;
	ts.Inserir(simbolos);
	Limpar_Simbolos();
}

//MD
void C_Analise_Sintatica::Mode()
{
	if (token == VALUE)
	{
		Aceitar_Token(VALUE, ERR_VALUE);
	}
	else if (token == REF)
	{
		Aceitar_Token(REF, ERR_REF);
	}
	else
		Erro("Esperado mode(value ou ref)");
}

//DS
void C_Analise_Sintatica::Decl_struct()
{
	Aceitar_Token(TIPO_STRUCT, ERR_STRUCT);

	Aceitar_Token(IDENTIFICADOR, ERR_IDENTIFICADOR);

	Lista_decl_struct();

	Aceitar_Token(END_STRUCT, ERR_END_STRUCT);
}

//LDS
void C_Analise_Sintatica::Lista_decl_struct()
{
	if (token == VAR)
	{
		Decl_var();
		Lista_decl_struct();
	}
	else if (token != END_STRUCT)
		Erro(ERR_END_STRUCT);
}

//DCL
void C_Analise_Sintatica::Decl_class()
{
	Aceitar_Token(TIPO_CLASS, ERR_CLASS);

	Aceitar_Token(IDENTIFICADOR, ERR_IDENTIFICADOR);

	Bloco_class();

	Aceitar_Token(END_CLASS, ERR_END_CLASS);
}

//BC
void C_Analise_Sintatica::Bloco_class()
{
	if (token == ACESS_PRIVATE ||
		token == ACESS_PUBLIC)
	{
		Espec_acesso();
		Aceitar_Token(DOIS_PONTOS, ERR_DOIS_PONTOS);
	}

	if (token == CONSTANTE ||
		token == FUNCTION ||
		token == SUB ||
		token == VAR)
	{
		Lista_membro_class();
		Bloco_class();
	}
}

//EAC
void C_Analise_Sintatica::Espec_acesso()
{
	if (token == ACESS_PRIVATE)
	{
		Aceitar_Token(ACESS_PRIVATE, ERR_ACESS_PRIVATE);
	}
	else if (token == ACESS_PUBLIC)
	{
		Aceitar_Token(ACESS_PUBLIC, ERR_ACESS_PUBLIC);
	}
	else
		Erro("Esperado especificacao de acesso private ou public");
}

//LMC
void C_Analise_Sintatica::Lista_membro_class()
{
	if (token == CONSTANTE ||
		token == FUNCTION ||
		token == SUB ||
		token == VAR)
	{
		Membro_class();
		Lista_membro_class();
	}
}

//MC
void C_Analise_Sintatica::Membro_class()
{
	if (token == CONSTANTE)
	{
		Decl_const();
	}
	else if (token == FUNCTION)
	{
		Decl_func();
	}
	else if (token == SUB)
	{
		Decl_proc();
	}
	else if (token == VAR)
	{
		Decl_var();
	}
	else
	{
		Erro("Esperado membro de classe");
	}
}

/*                   COMANDOS                */

//BL
void C_Analise_Sintatica::Bloco()
{
	if (token == LACO_BREAK ||
		token == CONSTANTE ||
		token == LACO_CONTINUE ||
		token == LACO_DO ||
		token == LACO_FOR ||
		token == IDENTIFICADOR ||
		token == ID_SEL_IDENTIFICADOR||
		token == ID_SEL_PONTEIRO||
		token == CONDICAO_IF ||
		token == PRINT ||
		token == PRINTLN ||
		token == LACO_REPEAT ||
		token == RETORNO ||
		token == SCAN ||
		token == SCANLN ||
		token == VAR ||
		token == LACO_WHILE)
	{
		Lista_com();
	}
}

//LC
void C_Analise_Sintatica::Lista_com()
{
	if (token == LACO_BREAK ||
		token == CONSTANTE ||
		token == LACO_CONTINUE ||
		token == LACO_DO ||
		token == LACO_FOR ||
		token == IDENTIFICADOR ||
		token == ID_SEL_IDENTIFICADOR ||
		token == ID_SEL_PONTEIRO ||
		token == CONDICAO_IF ||
		token == PRINT ||
		token == PRINTLN ||
		token == LACO_REPEAT ||
		token == RETORNO ||
		token == SCAN ||
		token == SCANLN ||
		token == VAR ||
		token == LACO_WHILE)
	{
		//Só entra aqui se for a pelo bloco, lá já é feita a condição
		Comando();
		Lista_com();
	}
}

//CM
void C_Analise_Sintatica::Comando()
{
	if (token == LACO_BREAK ||
		token == LACO_CONTINUE ||
		token == RETORNO)
	{
		Com_desvio();
	}
	else if (token == CONSTANTE)
	{
		Decl_const();
	}
	else if (token == LACO_DO ||
		token == LACO_FOR ||
		token == LACO_REPEAT ||
		token == LACO_WHILE)
	{
		Com_repeticao();
	}
	else if (token == IDENTIFICADOR || token == ID_SEL_IDENTIFICADOR || token == ID_SEL_PONTEIRO)
	{
		Id_Composto();
		Comando_1();
	}
	else if (token == CONDICAO_IF)
	{
		Com_selecao();
	}
	else if (token == PRINT ||
		token == PRINTLN)
	{
		Com_escrita();
	}
	else if (token == SCAN ||
		token == SCANLN)
	{
		Com_leitura();
	}
	else if (token == VAR)
	{
		Decl_var();
	}
	else
		Erro("Esperado comando");
}

//CM'
void C_Analise_Sintatica::Comando_1()
{
	if (token == ABRE_PARENTESES)
	{
		Aceitar_Token(ABRE_PARENTESES, ERR_ABRE_PARENTESES);
		Args();
		Aceitar_Token(FECHA_PARENTESES, ERR_FECHA_PARENTESES);
		Aceitar_Token(PONTO_VIRGULA, ERR_PONTO_VIRGULA);
	}
	else if (token == ABRE_COLCHETES)
	{
		Aceitar_Token(ABRE_COLCHETES, ERR_ABRE_COLCHETES);
		Exp_soma();
		Aceitar_Token(FECHA_COLCHETES, ERR_FECHA_COLCHETES);
		Aceitar_Token(OP_ATRIBUICAO, ERR_OP_ATRIBUICAO);
		Exp();
		Aceitar_Token(PONTO_VIRGULA, ERR_PONTO_VIRGULA);
	}
	else if (token == OP_ATRIBUICAO)
	{
		Aceitar_Token(OP_ATRIBUICAO, ERR_OP_ATRIBUICAO);
		Exp();
		Aceitar_Token(PONTO_VIRGULA, ERR_PONTO_VIRGULA);
	}
}

//CS
void C_Analise_Sintatica::Com_selecao()
{
	Aceitar_Token(CONDICAO_IF, ERR_CONDICAO_IF);
	Exp();
	Aceitar_Token(CONDICAO_THEN, ERR_CONDICAO_THEN);
	Bloco();
	Com_selecao_1();
}

//CS'
void C_Analise_Sintatica::Com_selecao_1()
{
	if (token == CONDICAO_ELSE)
	{
		Aceitar_Token(CONDICAO_ELSE, ERR_CONDICAO_ELSE);
		Bloco();
	}
	//Com ou sem o ELSE, sempre terminar com end_if
	Aceitar_Token(CONDICAO_ENDIF, ERR_ENDIF);
}

//CR
void C_Analise_Sintatica::Com_repeticao()
{
	if (token == LACO_DO)
	{
		Aceitar_Token(LACO_DO, ERR_LACO_DO);
		Bloco();
		Aceitar_Token(LACO_WHILE, ERR_LACO_WHILE);
		Exp();
		Aceitar_Token(PONTO_VIRGULA, ERR_PONTO_VIRGULA);
	}
	else if (token == LACO_FOR)
	{
		Aceitar_Token(LACO_FOR, ERR_LACO_FOR);
		Aceitar_Token(IDENTIFICADOR, ERR_IDENTIFICADOR);
		Aceitar_Token(OP_ATRIBUICAO, ERR_OP_ATRIBUICAO);
		Exp_soma();
		Aceitar_Token(LACO_TO, ERR_LACO_TO);
		Exp_soma();
		Aceitar_Token(LACO_DO, ERR_LACO_DO);
		Bloco();
		Aceitar_Token(LACO_NEXT, ERR_LACO_NEXT);
	}
	else if (token == LACO_REPEAT)
	{
		Aceitar_Token(LACO_REPEAT, ERR_LACO_REPEAT);
		Bloco();
		Aceitar_Token(LACO_UNTIL, ERR_LACO_UNTIL);
		Exp();
		Aceitar_Token(PONTO_VIRGULA, ERR_PONTO_VIRGULA);
	}
	else if (token == LACO_WHILE)
	{
		Aceitar_Token(LACO_WHILE, ERR_LACO_WHILE);
		Exp();
		Aceitar_Token(LACO_DO, ERR_LACO_DO);
		Bloco();
		Aceitar_Token(LACO_LOOP, ERR_LACO_LOOP);
	}
	else
		Erro("Esperado comando de repeticao");
}

//CD
void C_Analise_Sintatica::Com_desvio()
{
	if (token == LACO_BREAK)
	{
		Aceitar_Token(LACO_BREAK, "Esperado break");
		Aceitar_Token(PONTO_VIRGULA, "Esperado ;");
	}
	else if (token == LACO_CONTINUE)
	{
		Aceitar_Token(LACO_CONTINUE, "Esperado continue");
		Aceitar_Token(PONTO_VIRGULA, "Esperado ;");
	}
	else if (token == RETORNO)
	{
		Aceitar_Token(RETORNO, "Esperado return");
		Exp();
		Aceitar_Token(PONTO_VIRGULA, "Esperado ;");
	}
	else
		Erro("Esperado comando de desvio");
}

//CL
void C_Analise_Sintatica::Com_leitura()
{
	if (token == SCAN)
	{
		Aceitar_Token(SCAN, ERR_SCAN);
		Aceitar_Token(ABRE_PARENTESES, ERR_ABRE_PARENTESES);
		Lista_var();
		Aceitar_Token(FECHA_PARENTESES, ERR_FECHA_PARENTESES);
		Aceitar_Token(PONTO_VIRGULA, ERR_PONTO_VIRGULA);
	}
	else if (token == SCANLN)
	{
		Aceitar_Token(SCANLN, ERR_SCANLN);
		Aceitar_Token(ABRE_PARENTESES, ERR_ABRE_PARENTESES);
		Lista_var();
		Aceitar_Token(FECHA_PARENTESES, ERR_FECHA_PARENTESES);
		Aceitar_Token(PONTO_VIRGULA, ERR_PONTO_VIRGULA);
	}
	else
		Erro("Esperado comando de leitura (scan / scanln)");
}

//CE
void C_Analise_Sintatica::Com_escrita()
{
	if (token == PRINT)
	{
		Aceitar_Token(PRINT, ERR_PRINT);
		Aceitar_Token(ABRE_PARENTESES, ERR_ABRE_PARENTESES);
		Lista_exp();
		Aceitar_Token(FECHA_PARENTESES, ERR_FECHA_PARENTESES);
		Aceitar_Token(PONTO_VIRGULA, ERR_PONTO_VIRGULA);
	}
	else if (token == PRINTLN)
	{
		Aceitar_Token(PRINTLN, ERR_PRINTLN);
		Aceitar_Token(ABRE_PARENTESES, ERR_ABRE_PARENTESES);
		Lista_exp();
		Aceitar_Token(FECHA_PARENTESES, ERR_FECHA_PARENTESES);
		Aceitar_Token(PONTO_VIRGULA, ERR_PONTO_VIRGULA);
	}
	else
		Erro("Esperado comando de escrita (print / println)");
}

//IC - Navegar até o membo de uma struct/classe
void C_Analise_Sintatica::Id_Composto()
{
	if (token == IDENTIFICADOR)
		simbolos.identificador = Aceitar_Token(IDENTIFICADOR, ERR_IDENTIFICADOR);
	else if (token == ID_SEL_IDENTIFICADOR)
	{
		Aceitar_Token(ID_SEL_IDENTIFICADOR, ERR_OP_SELECAO_IDENTIFICADOR);
		Id_Composto();
	}
		
	else if (token == ID_SEL_PONTEIRO)
	{
		Aceitar_Token(ID_SEL_PONTEIRO, ERR_OP_SELECAO_PONTEIRO);
		Id_Composto();
	}
		

	/*if (token == IDENTIFICADOR &&
		(Proximo_Token() == OP_SELECAO_IDENTIFICADOR || Proximo_Token() == OP_SELECAO_PONTEIRO))
	{
		simbolos.identificador = Aceitar_Token(IDENTIFICADOR, ERR_IDENTIFICADOR);
		if (token == OP_SELECAO_IDENTIFICADOR)
		{
			Aceitar_Token(OP_SELECAO_IDENTIFICADOR, ERR_OP_SELECAO_IDENTIFICADOR);
		}
		else if (token == OP_SELECAO_PONTEIRO)
		{
			Aceitar_Token(OP_SELECAO_PONTEIRO, ERR_OP_SELECAO_PONTEIRO);

		}
		Id_Composto();
	} 
	else
		simbolos.identificador = Aceitar_Token(IDENTIFICADOR, ERR_IDENTIFICADOR);*/
}


/*                   EXPRESSÕES                */

//LE
void C_Analise_Sintatica::Lista_exp()
{
	if (token == OP_SUBTRACAO ||
		token == ABRE_PARENTESES ||
		token == OP_ADICAO ||
		token == CARACTERE ||
		token == FALSO ||
		token == IDENTIFICADOR ||
		token == ID_SEL_IDENTIFICADOR ||
		token == ID_SEL_PONTEIRO ||
		token == OP_NEGACAO ||
		token == NUM_INT ||
		token == NUM_REAL ||
		token == STRING ||
		token == VERDADEIRO)
	{
		Exp();
		Lista_exp_1();
	}
	else
		Erro("Esperado lista de expressão");
}

//LE'
void C_Analise_Sintatica::Lista_exp_1()
{
	if (token == VIRGULA)
	{
		Aceitar_Token(VIRGULA, ERR_VIRGULA);
		Lista_exp();
	}
	else if (token != FECHA_PARENTESES)
		Erro(ERR_FECHA_PARENTESES);
}

//EX
void C_Analise_Sintatica::Exp()
{
	if (token == IDENTIFICADOR ||
		token == ID_SEL_IDENTIFICADOR ||
		token == ID_SEL_PONTEIRO)
	{
		Exp_soma();
		Exp_1();
	}
	else if (token == OP_SUBTRACAO ||
		token == ABRE_PARENTESES ||
		token == OP_ADICAO ||
		token == CARACTERE ||
		token == FALSO ||
		token == OP_NEGACAO ||
		token == NUM_INT ||
		token == NUM_REAL ||
		token == STRING ||
		token == VERDADEIRO)
	{
		Exp_soma();
		Exp_1();
	}
	else
		Erro("Esperado expressão");
}

//EX'
void C_Analise_Sintatica::Exp_1()
{
	if (token == OP_MENOR ||
		token == OP_MENOR_IGUAL ||
		token == OP_DIFERENTE ||
		token == OP_IGUALDADE ||
		token == OP_MAIOR ||
		token == OP_MAIOR_IGUAL)
	{
		Op_relac();
		Exp_soma();
		Exp_1();
	}
}

//OR
void C_Analise_Sintatica::Op_relac()
{
	if (token == OP_MENOR)
		Aceitar_Token(OP_MENOR, ERR_OP_MENOR);
	else if (token == OP_MENOR_IGUAL)
		Aceitar_Token(OP_MENOR_IGUAL, ERR_OP_MENOR_IGUAL);
	else if (token == OP_DIFERENTE)
		Aceitar_Token(OP_DIFERENTE, ERR_OP_DIFERENTE);
	else if (token == OP_IGUALDADE)
		Aceitar_Token(OP_IGUALDADE, ERR_OP_IGUALDADE);
	else if (token == OP_MAIOR)
		Aceitar_Token(OP_MAIOR, ERR_OP_MAIOR);
	else if (token == OP_MAIOR_IGUAL)
		Aceitar_Token(OP_MAIOR_IGUAL, ERR_OP_MAIOR_IGUAL);
	else
		Erro("Esperado operador relacional");
}

//EA
void C_Analise_Sintatica::Exp_soma()
{
	if (token == OP_SUBTRACAO ||
		token == ABRE_PARENTESES ||
		token == OP_ADICAO ||
		token == CARACTERE ||
		token == FALSO ||
		token == IDENTIFICADOR ||
		token == ID_SEL_IDENTIFICADOR ||
		token == ID_SEL_PONTEIRO ||
		token == OP_NEGACAO ||
		token == NUM_INT ||
		token == NUM_REAL ||
		token == STRING ||
		token == VERDADEIRO)
	{
		Exp_mult();
		Exp_soma_1();
	}
	else
		Erro("Esperado expressão soma");
}

//EA'
void C_Analise_Sintatica::Exp_soma_1()
{
	if (token == OP_SUBTRACAO ||
		token == OP_ADICAO ||
		token == OP_LOGICO_OU)
	{
		Op_soma();
		Exp_soma();
	}
}

//OS
void C_Analise_Sintatica::Op_soma()
{
	if (token == OP_SUBTRACAO)
		Aceitar_Token(OP_SUBTRACAO, ERR_OP_SUBTRACAO);
	else if (token == OP_ADICAO)
		Aceitar_Token(OP_ADICAO, ERR_OP_ADICAO);
	else if (token == OP_LOGICO_OU)
		Aceitar_Token(OP_LOGICO_OU, ERR_OP_LOGICO_OU);
	else
		Erro("Esperado operador soma");
}

//EM
void C_Analise_Sintatica::Exp_mult()
{
	if (token == OP_SUBTRACAO ||
		token == ABRE_PARENTESES ||
		token == OP_ADICAO ||
		token == CARACTERE ||
		token == FALSO ||
		token == IDENTIFICADOR ||
		token == ID_SEL_IDENTIFICADOR ||
		token == ID_SEL_PONTEIRO ||
		token == OP_NEGACAO ||
		token == NUM_INT ||
		token == NUM_REAL ||
		token == STRING ||
		token == VERDADEIRO)
	{
		Exp_simples();
		Exp_mult_1();
	}
	else
		Erro("Esperado expressão multiplicacao");
}

//EM'
void C_Analise_Sintatica::Exp_mult_1()
{
	if (token == OP_MULTIPLICACAO ||
		token == OP_DIVISAO ||
		token == OP_LOGICO_E ||
		token == OP_MOD)
	{
		Op_mult();
		Exp_simples();
		Exp_mult_1();
	}
}

//OM
void C_Analise_Sintatica::Op_mult()
{
	if (token == OP_MULTIPLICACAO)
		Aceitar_Token(OP_MULTIPLICACAO, ERR_OP_MULTIPLICACAO);
	else if (token == OP_DIVISAO)
		Aceitar_Token(OP_DIVISAO, ERR_OP_DIVISAO);
	else if (token == OP_LOGICO_E)
		Aceitar_Token(OP_LOGICO_E, ERR_OP_LOGICO_E);
	else if (token == OP_MOD)
		Aceitar_Token(OP_MOD, ERR_OP_MOD);
	else
		Erro("Esperado operador multiplicacao");
}

//ES
void C_Analise_Sintatica::Exp_simples()
{
	if (token == OP_SUBTRACAO ||
		token == OP_ADICAO ||
		token == OP_NEGACAO)
	{
		Op_unario();
		Exp();
	}
	else if (token == ABRE_PARENTESES)
	{
		Aceitar_Token(ABRE_PARENTESES, ERR_ABRE_PARENTESES);
		Exp();
		Aceitar_Token(FECHA_PARENTESES, ERR_FECHA_PARENTESES);
	}
	else if (token == CARACTERE ||
		token == FALSO ||
		token == NUM_INT ||
		token == NUM_REAL ||
		token == STRING ||
		token == VERDADEIRO)
	{
		Literal();
	}
	else if (token == IDENTIFICADOR || token == ID_SEL_IDENTIFICADOR || token == ID_SEL_PONTEIRO)
	{
		Id_Composto();
		Exp_simples_1();
	}
	else
		Erro("Esperado expressão simples");
}

//ES'
void C_Analise_Sintatica::Exp_simples_1()
{
	if (token == ABRE_PARENTESES)
	{
		Aceitar_Token(ABRE_PARENTESES, ERR_ABRE_PARENTESES);
		Args();
		Aceitar_Token(FECHA_PARENTESES, ERR_FECHA_PARENTESES);
	}
	else if (token == ABRE_COLCHETES)
	{
		Aceitar_Token(ABRE_COLCHETES, ERR_ABRE_COLCHETES);
		Exp_soma();
		Aceitar_Token(FECHA_COLCHETES, ERR_FECHA_COLCHETES);
	}
}

//LI
void C_Analise_Sintatica::Literal()
{
	if (token == FALSO ||
		token == VERDADEIRO)
		Valor_verdade();
	else if (token == CARACTERE)
	{
		simbolos.tipo = TIPO_CHAR;
		if (simbolos.categoria == CONSTANTE)
		{
			simbolos.valor = Aceitar_Token(CARACTERE, ERR_CARACTERE);
		}
		else
			Aceitar_Token(CARACTERE, ERR_CARACTERE);
	}
	else if (token == NUM_INT)
	{
		simbolos.tipo = TIPO_INT;
		if (simbolos.categoria == CONSTANTE)
		{
			simbolos.valor = Aceitar_Token(NUM_INT, ERR_NUM);
		}
		else
			Aceitar_Token(NUM_INT, ERR_NUM);
	}
	else if (token == NUM_REAL)
	{
		simbolos.tipo = TIPO_FLOAT;
		if (simbolos.categoria == CONSTANTE)
		{
			simbolos.valor = Aceitar_Token(NUM_REAL, ERR_NUM_REAL);
		}
		else
			Aceitar_Token(NUM_REAL, ERR_NUM_REAL);
	}
	else if (token == STRING)
	{
		simbolos.tipo = TIPO_STRING;
		if (simbolos.categoria == CONSTANTE)
		{
			simbolos.valor = Aceitar_Token(STRING, ERR_STRING);
		}
		else
			Aceitar_Token(STRING, ERR_STRING);
	}
	else
		Erro("Esperado literal");
}

//VV
void C_Analise_Sintatica::Valor_verdade()
{
	if (token == FALSO)
	{
		simbolos.tipo = TIPO_BOOLEANO;
		if (simbolos.categoria == CONSTANTE)
		{
			simbolos.valor = Aceitar_Token(FALSO, ERR_FALSO);
		}
		else
			Aceitar_Token(FALSO, ERR_FALSO);
	}
	else if (token == VERDADEIRO)
	{
		simbolos.tipo = TIPO_BOOLEANO;
		if (simbolos.categoria == CONSTANTE)
		{
			simbolos.valor = Aceitar_Token(VERDADEIRO, ERR_VERDADEIRO);
		}
		else
			Aceitar_Token(VERDADEIRO, ERR_VERDADEIRO);
	}
}

//AR
void C_Analise_Sintatica::Args()
{
	if (token == OP_SUBTRACAO ||
		token == ABRE_PARENTESES ||
		token == OP_ADICAO ||
		token == CARACTERE ||
		token == FALSO ||
		token == IDENTIFICADOR ||
		token == ID_SEL_IDENTIFICADOR ||
		token == ID_SEL_PONTEIRO ||
		token == OP_NEGACAO ||
		token == NUM_INT ||
		token == NUM_REAL ||
		token == STRING ||
		token == VERDADEIRO)
	{
		Lista_exp();
	}
	else if (token != FECHA_PARENTESES)
		Erro(ERR_FECHA_PARENTESES);
}

//LV
void C_Analise_Sintatica::Lista_var()
{
	Id_Composto();
	simbolos.linha = iter_token_lexema->linha;
	
	Lista_var_1();
}

//LV'
void C_Analise_Sintatica::Lista_var_1()
{
	if (token == VIRGULA)
	{
		Aceitar_Token(VIRGULA, ERR_VIRGULA);
		Lista_var();
	}
	else if (token == ABRE_COLCHETES)
	{
		Aceitar_Token(ABRE_COLCHETES, ERR_ABRE_COLCHETES);
		Exp_soma();
		Aceitar_Token(FECHA_COLCHETES, ERR_FECHA_COLCHETES);
		Lista_var_2();
	}
	else if (token != FECHA_PARENTESES &&
		token != PONTO_VIRGULA &&
		token != BY)
		Erro("Esperado \',\' ou \'[\' ou \')\' ou \';\' ou \'by\' )");
}

//LV''
void C_Analise_Sintatica::Lista_var_2()
{
	if (token == VIRGULA)
	{
		Aceitar_Token(VIRGULA, ERR_VIRGULA);
		Lista_var();
	}
	else if (token != FECHA_PARENTESES &&token != PONTO_VIRGULA &&
		token != BY)
		Erro("Esperado \',\' ou \')\' ou \';\' ou \'by\' )");
}

void C_Analise_Sintatica::Lista_decl_var()
{
	simbolos.identificador = Aceitar_Token(IDENTIFICADOR, ERR_IDENTIFICADOR);
	simbolos.linha = iter_token_lexema->linha;
	simbolos.categoria = VAR;
	ts.Inserir(simbolos);
	Limpar_Simbolos();

	Lista_decl_var_1();
}

void C_Analise_Sintatica::Lista_decl_var_1()
{
	if (token == VIRGULA)
	{
		Aceitar_Token(VIRGULA, ERR_VIRGULA);
		Lista_decl_var();
	}
	else if (token == ABRE_COLCHETES)
	{
		Aceitar_Token(ABRE_COLCHETES, ERR_ABRE_COLCHETES);
		Exp_soma();
		Aceitar_Token(FECHA_COLCHETES, ERR_FECHA_COLCHETES);
		Lista_decl_var_2();
	}
	else if (token != FECHA_PARENTESES &&
		token != PONTO_VIRGULA &&
		token != BY)
		Erro("Esperado \',\' ou \'[\' ou \')\' ou \';\' ou \'by\' )");
}

void C_Analise_Sintatica::Lista_decl_var_2()
{
	if (token == VIRGULA)
	{
		Aceitar_Token(VIRGULA, ERR_VIRGULA);
		Lista_decl_var();
	}
	else if (token != FECHA_PARENTESES &&token != PONTO_VIRGULA &&
		token != BY)
		Erro("Esperado \',\' ou \')\' ou \';\' ou \'by\' )");
}

//OU
void C_Analise_Sintatica::Op_unario()
{
	if (token == OP_SUBTRACAO)
		Aceitar_Token(OP_SUBTRACAO, ERR_OP_SUBTRACAO);
	else if (token == OP_ADICAO)
		Aceitar_Token(OP_ADICAO, ERR_OP_ADICAO);
	else if (token == OP_NEGACAO)
		Aceitar_Token(OP_NEGACAO, ERR_OP_NEGACAO);
	else
		Erro("Esperado operador unario");
}

void C_Analise_Sintatica::Limpar_Simbolos()
{
	simbolos.categoria = "";
	simbolos.chave = 0;
	simbolos.escopo = "";
	simbolos.identificador = "";
	simbolos.parametro = false;
	simbolos.params = 0;
	simbolos.passby = "";
	simbolos.tipo = "";
	simbolos.valido = false;
	simbolos.valor = "";
}
