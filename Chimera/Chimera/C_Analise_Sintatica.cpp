#include "C_Analise_Sintatica.h"




C_Analise_Sintatica::C_Analise_Sintatica(vector<S_Token_Lexema> _tabela_token_lexema)
{
	chave = 0;
	//posicao_pilha_global = -1;
	existe_main = false;
	existe_CHPR_main = false;
	eh_argumento = false;
	Proc_Func_Atual = "";
	//MEPA - Inicio a pilha que vai armazenar a quantidade de variáveis alocadas - Inicio com zero - Posição 0 é globais
	pilha_var_mem.push(0);

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

void C_Analise_Sintatica::Erro(string _identificador, string _msg)
{
	cout << "ERRO-Linha: " << iter_token_lexema->linha << " - " << _identificador << " -> " << _msg << endl;
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
	if (!existe_main)
		Erro("Nao existe subprograma main");
	//MEPA - DSVS Final, desalocar memória atribuídas globalmente
	mepa.NADA("FIM");
	mepa.DMEM(to_string(pilha_var_mem.top()));

	//Se cheguei ao fim do programa, compilação ok
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
		token == ESTRUTURA ||
		token == CLASSE)
	{
		//Garanto que o desvio para o main ocorra assim que não tiver mais constantes ou variáveis declaradas globalmente
		//Também garanto que crio apenas 1 desvio para o main
		if (token != CONSTANTE && token != VAR && !existe_CHPR_main)
		{
			mepa.CHPR("MAIN", "1");
			mepa.DSVS("FIM");
			existe_CHPR_main = true;
		}
		Decl();
		Lista_decl();
	}
	else if (token != FIM_PROGRAMA)
		Erro("Esperado lista de declaracao");
}

//D
void C_Analise_Sintatica::Decl()
{
	if (token == CONSTANTE)
	{
		//Iniciar simbolo
		Iniciar_Simbolos(sconst);
		sconst.categoria = CONSTANTE;
		//Publico por padrão
		acesso_membro = PUBLIC;
		sconst.pai = 0;

		Decl_const();
	}
	else if (token == FUNCTION)
	{
		//Iniciar simbolo
		Iniciar_Simbolos(sfuncao);
		//Empilho a chave atual
		pilha_parente.push(sfuncao.chave);
		//Publico por padrão
		acesso_membro = PUBLIC;

		sfuncao.categoria = FUNCTION;
		sfuncao.pai = 0;

		Decl_func();

		//Desempilho a chave, pois não terá mais filhos
		pilha_parente.pop();
		//Desativo todos os filhos, pois nao existirão mais na tabela de símbolos
		
	}
	else if (token == SUB)
	{
		//Iniciar simbolo
		Iniciar_Simbolos(sproc);
		//Empilho a chave atual
		pilha_parente.push(sproc.chave);
		//Publico por padrão
		acesso_membro = PUBLIC;

		sproc.categoria = SUB;
		sproc.pai = 0;

		Decl_proc();

		//Desempilho a chave, pois não terá mais filhos
		pilha_parente.pop();
	}
	else if (token == VAR)
	{
		//Iniciar simbolo
		Iniciar_Simbolos(svar);
		svar.pai = 0;
		//Publico por padrão
		acesso_membro = PUBLIC;

		Decl_var();
	}
	else if (token == ESTRUTURA)
	{
		//iniciar simbolo
		Iniciar_Simbolos(sstruct);
		//Empilho a chave atual
		pilha_parente.push(sstruct.chave);
		//Publico por padrão
		acesso_membro = PUBLIC;

		sstruct.categoria = ESTRUTURA;
		sstruct.pai = 0;

		Decl_struct();
		//Desempilho a chave, pois não terá mais filhos
		pilha_parente.pop();
	}
	else if (token == CLASSE)
	{
		//iniciar simbolo
		Iniciar_Simbolos(sclasse);
		//Empilho a chave atual
		pilha_parente.push(sclasse.chave);
		//Publico por padrão
		acesso_membro = PUBLIC;

		sclasse.categoria = CLASSE;
		sclasse.pai = 0;
		classe = sclasse.chave;

		Decl_class();
		//Desempilho a chave, pois não terá mais filhos
		pilha_parente.pop();

		classe = 0;
	}
	else
		Erro("Esperado declaracao");
}

//DC
void C_Analise_Sintatica::Decl_const()
{
	Aceitar_Token(CONSTANTE, ERR_CONTANTE);

	sconst.identificador = Aceitar_Token(IDENTIFICADOR, ERR_IDENTIFICADOR);

	sconst.linha = iter_token_lexema->linha;

	Aceitar_Token(OP_ATRIBUICAO, ERR_OP_ATRIBUICAO);

	Literal(sconst);

	//MEPA - CRCT Carrego o valor da constante
	//mepa.CRCT(sconst.valor);

	Aceitar_Token(PONTO_VIRGULA, ERR_PONTO_VIRGULA);

	sconst.access = acesso_membro;

	ts.Inserir(sconst);
}

//DV
void C_Analise_Sintatica::Decl_var()
{
	Aceitar_Token(VAR, ERR_VAR);

	svar.categoria = VAR;
	svar.tipo = Espec_tipo();
	//posicao_pilha_local = -1;
	Lista_decl_var();

	svar.access = acesso_membro;

	//Insiro a única ou última variável
	//if (svar.pai == 0) //Global
		//svar.pos_pilha = posicao_pilha_global;
	//else //Local
		svar.pos_pilha = pilha_var_mem.top()-1;

	ts.Inserir(svar);

	//MEPA - AMEM com a quantidade de variáveis declaradas
	mepa.AMEM(to_string(pilha_var_mem.top()));


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
	return "";
}

//DP
void C_Analise_Sintatica::Decl_proc()
{
	Aceitar_Token(SUB, ERR_SUB);

	sproc.linha = iter_token_lexema->linha;
	sproc.identificador = Aceitar_Token(IDENTIFICADOR, ERR_IDENTIFICADOR);

	Proc_Func_Atual = sproc.identificador;

	//Garanto que exista um Subprograma Main
	if (sproc.identificador == "main")
	{
		existe_main = true;
		mepa.NADA("MAIN");
		sproc.rotulo = "MAIN";
	}
	else
	{
		sproc.rotulo = mepa.NADA();
	}

	//MEPA - ENPR -Entrada do procedimento
	mepa.ENPR("1");

	Aceitar_Token(ABRE_PARENTESES, ERR_ABRE_PARENTESES);

	sproc.qtd_params = Params();

	Aceitar_Token(FECHA_PARENTESES, ERR_FECHA_PARENTESES);

	//MEPA - Inicio a pilha que vai armazenar a quantidade de variáveis alocadas - Inicio com zero
	pilha_var_mem.push(0);

	//Se existir parametros por valor, preciso armazenar memória
	if (qtd_param.val > 0)
	{
		mepa.AMEM(to_string(qtd_param.val));
		pilha_var_mem.top() += qtd_param.val;
	}


	//Atualizar posição da pilha dos parâmetros e envio uma pilha para depois conseguir atribuir os valores para os parâmetros passados por cópia
	if (sproc.qtd_params > 0)
	{
		stack<pair<int, int>> pparamval;
		ts.Atualizar_Pilha_Param(sproc.chave, sproc.qtd_params, pparamval);
		//Para os params que foram passador por valor, armazeno os valores localmente 
		while (!pparamval.empty())
		{
			mepa.CRVL("1", to_string(pparamval.top().first));
			mepa.ARMZ("1", to_string(pparamval.top().second));
			pparamval.pop();
		}
	}


	Bloco();

	sproc.access = acesso_membro;
	ts.Inserir(sproc);

	Aceitar_Token(END_SUB, ERR_END_SUB);
	
	//Quando precisar ir para o fim do procedimento
	//mepa.Add_Comando(sproc.rotulo + "_FIM:NADA");

	//Inativar todas as variáveis que foram declaradas dentro desta procedure/subprograma
	//int qtd_removida = ts.Remover_Internos(sproc.identificador);
	ts.Remover_Internos(sproc.identificador);
	//Preciso desempilhar a variável que controla a posição da pilha, somente se tiver removido
	if (!pilha_var_mem.empty() && pilha_var_mem.top() > 0)
	{
		mepa.DMEM(to_string(pilha_var_mem.top()));
		//posicao_pilha_global = posicao_pilha_global - qtd_removida;
	}
	//MEPA - RTPR Retorno da procedure
	mepa.RTPR("1", to_string(sproc.qtd_params));

	qtd_param.val = 0;
	qtd_param.ref = 0;
	Proc_Func_Atual = "";
	//Tiro do topo o contador de memoria desta função (Caso não tenha sido desempilhado anterirmente por um retorno)
	if (!pilha_var_mem.empty())
		pilha_var_mem.pop();
}

//DF
void C_Analise_Sintatica::Decl_func()
{
	Aceitar_Token(FUNCTION, ERR_FUNCTION);

	sfuncao.linha = iter_token_lexema->linha;
	sfuncao.tipo = Espec_tipo();

	sfuncao.identificador = Aceitar_Token(IDENTIFICADOR, ERR_IDENTIFICADOR);

	Proc_Func_Atual = sfuncao.identificador;

	sfuncao.access = acesso_membro;

	sfuncao.rotulo = mepa.NADA();

	//MEPA - ENPR -Entrada do procedimento
	mepa.ENPR("1");
		
	Aceitar_Token(ABRE_PARENTESES, ERR_ABRE_PARENTESES);

	sfuncao.qtd_params = Params();

	Aceitar_Token(FECHA_PARENTESES, ERR_FECHA_PARENTESES);

	//MEPA - Inicio a pilha que vai armazenar a quantidade de variáveis alocadas - Inicio com zero
	pilha_var_mem.push(0);

	//Se existir parametros por valor, preciso armazenar memória
	if (qtd_param.val > 0)
	{
		mepa.AMEM(to_string(qtd_param.val));
		pilha_var_mem.top() += qtd_param.val;
	}


	//Atualizar posição da pilha dos parâmetros e envio uma pilha para depois conseguir atribuir os valores para os parâmetros passados por cópia
	if (sfuncao.qtd_params > 0)
	{
		stack<pair<int, int>> pparamval;
		ts.Atualizar_Pilha_Param(sfuncao.chave, sfuncao.qtd_params, pparamval);
		//Para os params que foram passador por valor, armazeno os valores localmente 
		while (!pparamval.empty())
		{
			mepa.CRVL("1", to_string(pparamval.top().first));
			mepa.ARMZ("1", to_string(pparamval.top().second));
			pparamval.pop();
		}
	}

	//Faço a inserção na TS logo após a declaração, isso precisa ser feito em caso seja uma função recursiva
	ts.Inserir(sfuncao);

	Bloco();

	Aceitar_Token(END_FUNCTION, ERR_END_FUNCTION);

	//Quando precisar ir para o fim da função
	//mepa.Add_Comando(sfuncao.rotulo + "_FIM:NADA");

	//Inativar todas as variáveis que foram declaradas dentro desta função
	int qtd_removida =	ts.Remover_Internos(sfuncao.identificador);
	//Preciso desempilhar a variável que controla a posição da pilha, somente se tiver removido
	if (qtd_removida > 0)
	{
		//Verifico se é diferente de VOID, aí adiciona 1 DMEM por causa do retorno
		//if (sfuncao.tipo != TIPO_VOID)
			//mepa.DMEM(to_string(qtd_removida + 1));
		//else
			mepa.DMEM(to_string(qtd_removida));
		//posicao_pilha_global = posicao_pilha_global - qtd_removida;
	}
	//else if (sfuncao.tipo != TIPO_VOID) //Não teve parâmetros, mas é diferente de void então tem o DMEM do retorno
		//mepa.DMEM("1");


	//MEPA - RTPR Retorno da procedure
	mepa.RTPR("1", to_string(sfuncao.qtd_params));

	qtd_param.val = 0;
	qtd_param.ref = 0;
	Proc_Func_Atual = "";
	//Tiro do topo o contador de memoria desta função (Caso não tenha sido desempilhado anterirmente por um retorno)
	if (!pilha_var_mem.empty())
		pilha_var_mem.pop();
}

//PS
int C_Analise_Sintatica::Params()
{
	if (token == VAR)
	{
		return Lista_param();
	}
	else if (token != FECHA_PARENTESES)
		Erro(ERR_FECHA_PARENTESES);
}

//LP
int C_Analise_Sintatica::Lista_param()
{
	qtd_param.val = 0;
	qtd_param.ref = 0;
	Param();
	Lista_param_1();
	return qtd_param.val + qtd_param.ref;
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
	string tipo_exp;
	Aceitar_Token(VAR, ERR_VAR);

	Iniciar_Simbolos(sparams);
	
	//Categoria
	sparams.categoria = PARAM;
	//Especifico o tipo do parametro
	sparams.tipo = Espec_tipo();
	
	sparams.identificador = Aceitar_Token(IDENTIFICADOR, ERR_IDENTIFICADOR);
	sparams.linha = iter_token_lexema->linha;
	if (token == ABRE_COLCHETES)
	{
		Aceitar_Token(ABRE_COLCHETES, ERR_ABRE_COLCHETES);
		tipo_exp = Exp_soma();
		//validar se é int, array só pode ter indice int 
		if (tipo_exp != TIPO_INT)
			Erro(ERR_SEM_INDEX_DECL_TIPO_INT);
		Aceitar_Token(FECHA_COLCHETES, ERR_FECHA_COLCHETES);
		sparams.array = true;
	}

	Aceitar_Token(BY, ERR_BY);

	Mode();
	
	sparams.pai = pilha_parente.top();
	sparams.access = acesso_membro;
	sparams.classe = classe;
	ts.Inserir(sparams);

}

//MD
void C_Analise_Sintatica::Mode()
{
	if (token == VALUE)
	{
		Aceitar_Token(VALUE, ERR_VALUE);
		sparams.passby = VALUE;
		qtd_param.val++;
	}
	else if (token == REF)
	{
		Aceitar_Token(REF, ERR_REF);
		sparams.passby = REF;
		qtd_param.ref++;
	}
	else
		Erro("Esperado mode(value ou ref)");
}

//DS
void C_Analise_Sintatica::Decl_struct()
{
	Aceitar_Token(ESTRUTURA, ERR_STRUCT);
	sstruct.linha = iter_token_lexema->linha;

	sstruct.identificador = Aceitar_Token(IDENTIFICADOR, ERR_IDENTIFICADOR);
	

	Lista_decl_struct();

	Aceitar_Token(END_STRUCT, ERR_END_STRUCT);

	sstruct.access = acesso_membro;
	ts.Inserir(sstruct);
}

//LDS
void C_Analise_Sintatica::Lista_decl_struct()
{
	if (token == VAR)
	{
		Iniciar_Simbolos(svar);
		svar.pai = pilha_parente.top();
		svar.classe = classe;
		Decl_var();
		Lista_decl_struct();
	}
	else if (token != END_STRUCT)
		Erro(ERR_END_STRUCT);
}

//DCL
void C_Analise_Sintatica::Decl_class()
{
	Aceitar_Token(CLASSE, ERR_CLASS);
	sclasse.linha = iter_token_lexema->linha;

	sclasse.identificador = Aceitar_Token(IDENTIFICADOR, ERR_IDENTIFICADOR);
	

	Bloco_class();

	Aceitar_Token(END_CLASS, ERR_END_CLASS);

	ts.Inserir(sclasse);
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
		acesso_membro = PRIVATE;
	}
	else if (token == ACESS_PUBLIC)
	{
		Aceitar_Token(ACESS_PUBLIC, ERR_ACESS_PUBLIC);
		acesso_membro = PUBLIC;
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
		//Iniciar simbolo
		Iniciar_Simbolos(sconst);
		sconst.categoria = CONSTANTE;
		sconst.pai = pilha_parente.top();
		sconst.classe = classe;

		Decl_const();
	}
	else if (token == FUNCTION)
	{
		//Iniciar simbolo
		Iniciar_Simbolos(sfuncao);
		//O pai é o topo da pilha
		sfuncao.pai = pilha_parente.top();
		//Gravo a classe que a funcao pertence
		sfuncao.classe = classe;
		//Empilho a chave atual
		pilha_parente.push(sfuncao.chave);

		sfuncao.categoria = FUNCTION;

		Decl_func();

		//Desempilho a chave, pois não terá mais filhos
		pilha_parente.pop();
	}
	else if (token == SUB)
	{
		//Iniciar simbolo
		Iniciar_Simbolos(sproc);
		//O pai é o topo da pilha
		sproc.pai = pilha_parente.top();
		//Gravo a classe que a procedure pertence
		sproc.classe = classe;
		//Empilho a chave atual
		pilha_parente.push(sproc.chave);

		sproc.categoria = SUB;

		Decl_proc();

		//Desempilho a chave, pois não terá mais filhos
		pilha_parente.pop();
	}
	else if (token == VAR)
	{
		//Iniciar simbolo
		Iniciar_Simbolos(svar);
		//O pai é o topo da pilha
		svar.pai = pilha_parente.top();
		//Gravo a classe que a variavel pertence
		svar.classe = classe;

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
		string identificador;
		string categoria;
		string qtd_params_decl;
		string qtd_params_chamada;
		identificador = Id_Composto();
		categoria = ts.Buscar_Categoria(identificador);
	    if (categoria == CONSTANTE) //Constante não pode ser atribuída
			Erro(identificador, ERR_SEM_CONST_ATRIB);
		qtd_params_decl = ts.Buscar_Qtd_Params(identificador);

		//MEPA - Empilho a variável, irá desempilhar somente depois que inserir a CRCT/CRVL do valor atribuído
		if (categoria == VAR || categoria == PARAM)
			mepa.pilha_ARMZ.push(identificador);

		//TODO Separar memória para retorno de função

		//TODO Separar memória para os parâmetros passador por referência

		//Comando_1 retorna a quantidade de parâmetros e envio o tipo do identificador para verificação de tipos
		qtd_params_chamada = Comando_1(ts.Buscar_Tipo(identificador));

		//MEPA - Desempilho
		if ((categoria == VAR || categoria == PARAM) && !mepa.pilha_ARMZ.empty())
		{
			int pai = ts.Buscar_Pai(mepa.pilha_ARMZ.top());
			if (pai != 0)
				pai = 1;
			mepa.ARMZ(to_string(pai), to_string(ts.Buscar_Pos_Pilha(mepa.pilha_ARMZ.top())));
			mepa.pilha_ARMZ.pop();
		}

		
		if (categoria == FUNCTION || categoria == SUB)
		{
			//Validação semântica para verificar a quantidade de parâmetros da chamada 
			if (qtd_params_decl != qtd_params_chamada)
				Erro(identificador, ERR_SEM_NUM_PARAMS);
			//MEPA - CHPR Chamada de procedimento
			mepa.CHPR(ts.Buscar_Rotulo(identificador), "1");
			//TODO Atualizar valores que foram passador como referência

		}
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
		Iniciar_Simbolos(svar);
		svar.pai = pilha_parente.top();
		svar.classe = classe;
		Decl_var();
	}
	else
		Erro("Esperado comando");
}

//CM'
int C_Analise_Sintatica::Comando_1(string _tipo_esquerda)
{
	string tipo_exp;
	string tipo_direita;
	int qtd_params;
	if (token == ABRE_PARENTESES)
	{
		Aceitar_Token(ABRE_PARENTESES, ERR_ABRE_PARENTESES);
		qtd_params = Args();
		Aceitar_Token(FECHA_PARENTESES, ERR_FECHA_PARENTESES);
		Aceitar_Token(PONTO_VIRGULA, ERR_PONTO_VIRGULA);
		return qtd_params;
	}
	else if (token == ABRE_COLCHETES)
	{
		Aceitar_Token(ABRE_COLCHETES, ERR_ABRE_COLCHETES);
		tipo_exp = Exp_soma();
		//validar se é int, array só pode ter indice int 
		if (tipo_exp != TIPO_INT)
			Erro(ERR_SEM_INDEX_TIPO_INT);
		Aceitar_Token(FECHA_COLCHETES, ERR_FECHA_COLCHETES);
		Aceitar_Token(OP_ATRIBUICAO, ERR_OP_ATRIBUICAO);
		tipo_direita = Exp();
		//Validar se a atribuição tem os tipos compatíveis 
		Validar_Atribuicao(_tipo_esquerda, tipo_direita);
		Aceitar_Token(PONTO_VIRGULA, ERR_PONTO_VIRGULA);
	}
	else if (token == OP_ATRIBUICAO)
	{
		Aceitar_Token(OP_ATRIBUICAO, ERR_OP_ATRIBUICAO);
		tipo_direita = Exp();
		//TODO 03 - Validar Expressão aqui
		mepa.Avaliar_Expressao(mepa.pilha_EXP, ts);
		
		//Validar se a atribuição tem os tipos compatíveis 
		Validar_Atribuicao(_tipo_esquerda, tipo_direita);
		Aceitar_Token(PONTO_VIRGULA, ERR_PONTO_VIRGULA);
	}
}

//CS
void C_Analise_Sintatica::Com_selecao()
{
	string tipo_exp;
	string DSVF, DSVS;

	Aceitar_Token(CONDICAO_IF, ERR_CONDICAO_IF);
	tipo_exp = Exp();
	//TODO 05 - Validar Expressão aqui
	mepa.Avaliar_Expressao(mepa.pilha_EXP, ts);
	//MEPA - Desvia se for falso
	DSVF = mepa.DSVF();
	if (tipo_exp != TIPO_INT && tipo_exp != TIPO_BOOLEANO)
		Erro(ERR_SEM_IF);
	Aceitar_Token(CONDICAO_THEN, ERR_CONDICAO_THEN);
	Bloco();
	//MEPA - Desvia sempre
	DSVS = mepa.DSVS();
	//MEPA - NADA - caso o IF tenha sido falso cai aqui
	mepa.NADA(DSVF);
	Com_selecao_1();
	//MEPA - NADA - Caso o IF tenha sido verdadeiro, cai direto aqui
	mepa.NADA(DSVS);
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
	string tipo_exp;
	string identificador;
	string DSVS, DSVF;
	/*if (token == LACO_DO) //Laço DO pode ser um problema, probabilidade de tirar ele
	{
		Aceitar_Token(LACO_DO, ERR_LACO_DO);
		Bloco();
		Aceitar_Token(LACO_WHILE, ERR_LACO_WHILE);
		tipo_exp = Exp();
		if (tipo_exp != TIPO_INT || tipo_exp != TIPO_BOOLEANO)
			Erro(ERR_SEM_DO);
		Aceitar_Token(PONTO_VIRGULA, ERR_PONTO_VIRGULA);
	}
	else*/
	if (token == LACO_FOR)
	{
		Aceitar_Token(LACO_FOR, ERR_LACO_FOR);
		identificador = Id_Composto();
		//Preciso saber o tipo do identificador para verificar se é int
		tipo_exp = ts.Buscar_Tipo(identificador);
		if (tipo_exp != TIPO_INT)
			Erro(identificador, ERR_SEM_FOR);
		Aceitar_Token(OP_ATRIBUICAO, ERR_OP_ATRIBUICAO);
		tipo_exp = Exp_soma();
		//Expressão precisa ser int
		if (tipo_exp != TIPO_INT)
			Erro(ERR_SEM_FOR);
		//TODO 08 - Validar Expressão aqui
		mepa.Avaliar_Expressao(mepa.pilha_EXP, ts);
		//MEPA - ARMZ Armazeno o resultado da expressão no identificador
		int pai = ts.Buscar_Pai(identificador);
		if (pai != 0)
			pai = 1;
		mepa.ARMZ(to_string(pai), to_string(ts.Buscar_Pos_Pilha(identificador)));
		//MEPA - NADA -> Desvia sempre pra cá, só não quando a expressão é falsa
		DSVS = mepa.NADA();
		Aceitar_Token(LACO_TO, ERR_LACO_TO);
		//MEPA - CRVL Carrego o valor atual do identificador
		mepa.CRVL(to_string(pai), to_string(ts.Buscar_Pos_Pilha(identificador)));
		tipo_exp = Exp_soma();
		if (tipo_exp != TIPO_INT)
			Erro(ERR_SEM_FOR);
		//TODO 08 - Validar Expressão aqui
		mepa.Avaliar_Expressao(mepa.pilha_EXP, ts);
		//MEPA - CMEG Comparar se é menor igual
		mepa.CMEG();
		Aceitar_Token(LACO_DO, ERR_LACO_DO);
		//MEPA - Desvia se for falso
		DSVF = mepa.DSVF();
		Bloco();
		Aceitar_Token(LACO_NEXT, ERR_LACO_NEXT);
		//MEPA - CRVL carrega valor atual do identificador
		mepa.CRVL(to_string(pai), to_string(ts.Buscar_Pos_Pilha(identificador)));
		//MEPA - CRCT Carrego constante 1 para adicionar no NEXT
		mepa.CRCT("1");
		//MEPA - SOMA Somo +1 no identificador para o NEXT
		mepa.SOMA();
		//MEPA - ARMZ Armazeno o novo valor no identificador
		mepa.ARMZ(to_string(pai), to_string(ts.Buscar_Pos_Pilha(identificador)));
		//MEPA - DSVS - Desvio sempre ao fim do bloco, nova verificação do FOR
		mepa.DSVS(DSVS);
		//MEPA - NADA - caso o FOR tenha sido falso cai aqui
		mepa.NADA(DSVF);
	}
	else if (token == LACO_REPEAT)
	{
		Aceitar_Token(LACO_REPEAT, ERR_LACO_REPEAT);
		//MEPA - NADA -> Desvia sempre pra cá, só não quando a expressão é falsa
		DSVS = mepa.NADA();
		Bloco();
		Aceitar_Token(LACO_UNTIL, ERR_LACO_UNTIL);
		tipo_exp = Exp();
		//TODO 07 - Validar Expressão aqui
		mepa.Avaliar_Expressao(mepa.pilha_EXP, ts);
		DSVF = mepa.DSVF();
		//MEPA - DSVS - Desvio sempre ao fim do bloco, nova verificação do REPEAT
		mepa.DSVS(DSVS);
		//MEPA - NADA - caso o UNTIL tenha sido falso cai aqui
		mepa.NADA(DSVF);
		//Expreção repeat apenas pode ser int ou booleano
		if (!(tipo_exp == TIPO_INT || tipo_exp == TIPO_BOOLEANO))
			Erro(ERR_SEM_REAPEAT);
		Aceitar_Token(PONTO_VIRGULA, ERR_PONTO_VIRGULA);
	}
	else if (token == LACO_WHILE)
	{
		Aceitar_Token(LACO_WHILE, ERR_LACO_WHILE);
		//MEPA - NADA -> Desvia sempre pra cá, só não quando a expressão é falsa
		DSVS = mepa.NADA();
		tipo_exp = Exp();
		//TODO 06 - Validar Expressão aqui
		mepa.Avaliar_Expressao(mepa.pilha_EXP, ts);
		DSVF = mepa.DSVF();
		//Expreção while apenas pode ser int ou booleano
		if (!(tipo_exp == TIPO_INT || tipo_exp == TIPO_BOOLEANO))
			Erro(ERR_SEM_WHILE);
		Aceitar_Token(LACO_DO, ERR_LACO_DO);
		Bloco();
		//MEPA - DSVS - Desvio sempre ao fim do bloco, nova verificação do while
		mepa.DSVS(DSVS);
		//MEPA - NADA - caso o while tenha sido falso cai aqui
		mepa.NADA(DSVF);
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
		//TODO 10 - Validar Expressão aqui
		mepa.Avaliar_Expressao(mepa.pilha_EXP, ts);
		Aceitar_Token(PONTO_VIRGULA, "Esperado ;");

		//PBuscar quantidade de parâmetros da função/procedimento atual
		int qtd_params = ts.Buscar_Qtd_Params(Proc_Func_Atual);
		//Caso seja função, preciso armazenar o retorno
		if (ts.Buscar_Categoria(Proc_Func_Atual) == FUNCTION && ts.Buscar_Tipo(Proc_Func_Atual) != TIPO_VOID)
		{
			//Armazenar o valor na pilha para o retorno (ARMZ k, -(4+n))
			mepa.ARMZ("1", to_string(-(4 + qtd_params)));
			//mepa.DMEM("1");
			mepa.DMEM(to_string(pilha_var_mem.top()));
			mepa.RTPR("1", to_string(qtd_params));
			//Função/Procedure retornou aqui, acabo com o topo da pilha dela
			//pilha_var_mem.pop();
		}
		else
		{
			if (!pilha_var_mem.empty() && pilha_var_mem.top() > 0)
				mepa.DMEM(to_string(pilha_var_mem.top()));
			mepa.RTPR("1", to_string(qtd_params));
		}
		//Desvio para o fim do procedimento/função
		//mepa.DSVS(ts.Buscar_Rotulo(Proc_Func_Atual) + "_FIM");
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
		//MEPA - Para tratar escritas, preciso inserir o comando em uma pilha
		mepa.pilha_Com_Escrita.push(PRINT);
		Lista_exp();
		Aceitar_Token(FECHA_PARENTESES, ERR_FECHA_PARENTESES);
		Aceitar_Token(PONTO_VIRGULA, ERR_PONTO_VIRGULA);
	}
	else if (token == PRINTLN)
	{
		Aceitar_Token(PRINTLN, ERR_PRINTLN);
		Aceitar_Token(ABRE_PARENTESES, ERR_ABRE_PARENTESES);
		//MEPA - Para tratar escritas, preciso inserir o comando em uma pilha
		mepa.pilha_Com_Escrita.push(PRINTLN);
		Lista_exp();
		Aceitar_Token(FECHA_PARENTESES, ERR_FECHA_PARENTESES);
		Aceitar_Token(PONTO_VIRGULA, ERR_PONTO_VIRGULA);
	}
	else
		Erro("Esperado comando de escrita (print / println)");
}

//IC - Navegar até o membo de uma struct/classe
string C_Analise_Sintatica::Id_Composto()
{
	string identificador;
	if (token == IDENTIFICADOR)
	{
		identificador = Aceitar_Token(IDENTIFICADOR, ERR_IDENTIFICADOR);
		//Caso abra colchetes, é array. Verificar se é mesmo na TS
		if (token == ABRE_COLCHETES && !ts.Verificar_Array(identificador))
			Erro(ERR_SEM_INDEX_VAR_SIMPLES);
		if (!ts.Consultar(identificador))
			Erro(identificador, ERR_SEM_NAO_DECLARDO);
	}
	else if (token == ID_SEL_IDENTIFICADOR)
	{
		identificador = Aceitar_Token(ID_SEL_IDENTIFICADOR, ERR_OP_SELECAO_IDENTIFICADOR);
		//Remover o . 
		identificador = identificador.substr(0, identificador.size() - 1);
		if (!ts.Consultar(identificador))
			Erro(identificador, ERR_SEM_NAO_DECLARDO);
		identificador = Id_Composto();
	}
		
	else if (token == ID_SEL_PONTEIRO)
	{
		identificador = Aceitar_Token(ID_SEL_PONTEIRO, ERR_OP_SELECAO_PONTEIRO);
		//Remover o -> 
		identificador = identificador.substr(0, identificador.size() - 2);
		if (!ts.Consultar(identificador))
			Erro(identificador, ERR_SEM_NAO_DECLARDO);
		identificador = Id_Composto();
	}
	return identificador;
}


/*                   EXPRESSÕES                */

//LE
int C_Analise_Sintatica::Lista_exp()
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
		//MEPA se for argumento, já valido e insiro no vetor
		if (eh_argumento)
		{
			//TODO 06 - Validar Expressão aqui
			mepa.Avaliar_Expressao(mepa.pilha_EXP_args, ts);
		}
		return Lista_exp_1();
	}
	else
		Erro("Esperado lista de expressão");
}

//LE'
int C_Analise_Sintatica::Lista_exp_1()
{
	if (token == VIRGULA)
	{
		Aceitar_Token(VIRGULA, ERR_VIRGULA);
		return Lista_exp() + 1;
	}
	else if (token != FECHA_PARENTESES)
		Erro(ERR_FECHA_PARENTESES);
}

//EX
string C_Analise_Sintatica::Exp()
{
	string tipo_exp;
	if (token == IDENTIFICADOR ||
		token == ID_SEL_IDENTIFICADOR ||
		token == ID_SEL_PONTEIRO ||
		token == OP_SUBTRACAO ||
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
		tipo_exp = Exp_soma();
		Exp_1(tipo_exp);
	}
	else
		Erro("Esperado expressão");
	return tipo_exp;
}

//EX'
void C_Analise_Sintatica::Exp_1(string _tipo_exp)
{
	string tipo_exp;
	if (token == OP_MENOR ||
		token == OP_MENOR_IGUAL ||
		token == OP_DIFERENTE ||
		token == OP_IGUALDADE ||
		token == OP_MAIOR ||
		token == OP_MAIOR_IGUAL)
	{
		Op_relac();
		tipo_exp = Exp_soma();

		//Verificar se os tipos são compatíveis para a operação
		if (tipo_exp != _tipo_exp)
			Erro(ERR_SEM_INCOMPATIBILIDADE_TIPO);
		Exp_1(tipo_exp);

		//TODO 01 Verificar a possibilidade de retornar tipo int quando for comparação 
	}
}

//OR
void C_Analise_Sintatica::Op_relac()
{
	if (token == OP_MENOR)
	{
		Aceitar_Token(OP_MENOR, ERR_OP_MENOR);
		if (eh_argumento)
			mepa.pilha_EXP_args.push("CMME");
		else
			mepa.pilha_EXP.push("CMME");
	}
	else if (token == OP_MENOR_IGUAL)
	{
		Aceitar_Token(OP_MENOR_IGUAL, ERR_OP_MENOR_IGUAL);
		if (eh_argumento)
			mepa.pilha_EXP_args.push("CMEG");
		else
			mepa.pilha_EXP.push("CMEG");
	}
	else if (token == OP_DIFERENTE)
	{
		Aceitar_Token(OP_DIFERENTE, ERR_OP_DIFERENTE);
		if (eh_argumento)
			mepa.pilha_EXP_args.push("CMDG");
		else
			mepa.pilha_EXP.push("CMDG");
	}
	else if (token == OP_IGUALDADE)
	{
		Aceitar_Token(OP_IGUALDADE, ERR_OP_IGUALDADE);
		if (eh_argumento)
			mepa.pilha_EXP_args.push("CMIG");
		else
			mepa.pilha_EXP.push("CMIG");
	}
	else if (token == OP_MAIOR)
	{
		Aceitar_Token(OP_MAIOR, ERR_OP_MAIOR);
		if (eh_argumento)
			mepa.pilha_EXP_args.push("CMMA");
		else
			mepa.pilha_EXP.push("CMMA");
	}
	else if (token == OP_MAIOR_IGUAL)
	{
		Aceitar_Token(OP_MAIOR_IGUAL, ERR_OP_MAIOR_IGUAL);
		if (eh_argumento)
			mepa.pilha_EXP_args.push("CMAG");
		else
			mepa.pilha_EXP.push("CMAG");
	}
	else
		Erro("Esperado operador relacional");
}

//EA
string C_Analise_Sintatica::Exp_soma()
{
	string tipo_exp;
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
		tipo_exp = Exp_mult();
		Exp_soma_1(tipo_exp);
		return tipo_exp;
	}
	else
		Erro("Esperado expressão soma");

	return "";
}

//EA'
string C_Analise_Sintatica::Exp_soma_1(string _tipo_exp)
{
	string tipo_exp;
	if (token == OP_SUBTRACAO ||
		token == OP_ADICAO ||
		token == OP_LOGICO_OU)
	{
		Op_soma();
		tipo_exp = Exp_soma();

		//Validar quem tem a predominância na expressão
		tipo_exp = Retorna_Tipo_Comparado(tipo_exp, _tipo_exp);
		return tipo_exp;
	}
	else
		return _tipo_exp;
}

//OS
void C_Analise_Sintatica::Op_soma()
{
	if (token == OP_SUBTRACAO)
	{
		Aceitar_Token(OP_SUBTRACAO, ERR_OP_SUBTRACAO);
		//MEPA - SUBT Operador subtração
		if (eh_argumento)
			mepa.pilha_EXP_args.push("SUBT");
		else
			mepa.pilha_EXP.push("SUBT");
	}
	else if (token == OP_ADICAO)
	{
		Aceitar_Token(OP_ADICAO, ERR_OP_ADICAO);
		//MEPA - SOMA Operador soma
		if (eh_argumento)
			mepa.pilha_EXP_args.push("SOMA");
		else
			mepa.pilha_EXP.push("SOMA");
	}
	else if (token == OP_LOGICO_OU)
		Aceitar_Token(OP_LOGICO_OU, ERR_OP_LOGICO_OU);
	else
		Erro("Esperado operador soma");
}

//EM
string C_Analise_Sintatica::Exp_mult()
{
	string tipo_exp;
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
		tipo_exp = Exp_simples();
		return Exp_mult_1(tipo_exp);
	}
	else
		Erro("Esperado expressão multiplicacao");

	return "";
}

//EM'
string C_Analise_Sintatica::Exp_mult_1(string _tipo_exp)
{
	string tipo_exp;
	if (token == OP_MULTIPLICACAO ||
		token == OP_DIVISAO ||
		token == OP_LOGICO_E ||
		token == OP_MOD)
	{
		Op_mult();
		tipo_exp = Exp_simples();

		//Validar quem tem a predominância na expressão
		tipo_exp = Retorna_Tipo_Comparado(tipo_exp, _tipo_exp);

		return Exp_mult_1(tipo_exp);
	}
	else
		return _tipo_exp;
}

//OM
void C_Analise_Sintatica::Op_mult()
{
	if (token == OP_MULTIPLICACAO)
	{
		Aceitar_Token(OP_MULTIPLICACAO, ERR_OP_MULTIPLICACAO);
		if (eh_argumento)
			mepa.pilha_EXP_args.push("MULT");
		else
			mepa.pilha_EXP.push("MULT");
	}
	else if (token == OP_DIVISAO)
	{
		Aceitar_Token(OP_DIVISAO, ERR_OP_DIVISAO);
		if (eh_argumento)
			mepa.pilha_EXP_args.push("DIVI");
		else
			mepa.pilha_EXP.push("DIVI");
	}
	else if (token == OP_LOGICO_E)
		Aceitar_Token(OP_LOGICO_E, ERR_OP_LOGICO_E);
	else if (token == OP_MOD)
		Aceitar_Token(OP_MOD, ERR_OP_MOD);
	else
		Erro("Esperado operador multiplicacao");
}

//ES
string C_Analise_Sintatica::Exp_simples()
{
	string tipo_exp;
	if (token == OP_SUBTRACAO ||
		token == OP_ADICAO ||
		token == OP_NEGACAO)
	{
		Op_unario();
		tipo_exp = Exp();
	}
	else if (token == ABRE_PARENTESES)
	{
		Aceitar_Token(ABRE_PARENTESES, ERR_ABRE_PARENTESES);
		if (eh_argumento)
			mepa.pilha_EXP_args.push(ABRE_PARENTESES);
		else
			mepa.pilha_EXP.push(ABRE_PARENTESES);
		tipo_exp = Exp();
		Aceitar_Token(FECHA_PARENTESES, ERR_FECHA_PARENTESES);
		if (eh_argumento)
			mepa.pilha_EXP_args.push(FECHA_PARENTESES);
		else
			mepa.pilha_EXP.push(FECHA_PARENTESES);
	}
	else if (token == CARACTERE ||
		token == FALSO ||
		token == NUM_INT ||
		token == NUM_REAL ||
		token == STRING ||
		token == VERDADEIRO)
	{
		//Mando um tipo S_Simbolo dummy pois não gravo nada na tabela de símbolos pela expressão simples
		tipo_exp = Literal(sdummy);
	}
	else if (token == IDENTIFICADOR || token == ID_SEL_IDENTIFICADOR || token == ID_SEL_PONTEIRO)
	{
		string identificador;
		string categoria;
		identificador = Id_Composto();
		//Buscar a categoria do identificador
		categoria = ts.Buscar_Categoria(identificador);
		//Buscar o tipo do identificador
		tipo_exp = ts.Buscar_Tipo(identificador);

		//MEPA
		if (categoria == VAR || categoria == PARAM)
		{
			int pai = ts.Buscar_Pai(identificador);
			if (pai != 0)
				pai = 1;
			//Sempre carrego o valor na MEPA se for uma variável
			//mepa.CRVL("1", to_string(ts.Buscar_Pos_Pilha(identificador)));
			if (eh_argumento)
				mepa.pilha_EXP_args.push(identificador);
			else
				mepa.pilha_EXP.push(identificador);

			//Se tiver algum comando de escrita empilhado, preciso inserir ele na MEPA
			if (!mepa.pilha_Com_Escrita.empty())
			{
				mepa.CRVL(to_string(pai), to_string(ts.Buscar_Pos_Pilha(identificador)));
				mepa.IMPR();
				//MEPA - IMPE Adiciono enter somente se for PRINTLN, PRINT imprime na mesma linha
				if (mepa.pilha_Com_Escrita.top() == PRINTLN)
					mepa.IMPE();
				mepa.pilha_Com_Escrita.pop();
			}
		}

		if (categoria == FUNCTION || categoria == SUB)
		{
			//Caso seja função, preciso inserir um AMEM 1 para o retorno da função
			//Se for TIPO VOID não retorna e não é inserido o AMEM
			if (categoria == FUNCTION && tipo_exp != TIPO_VOID)
			{
				mepa.AMEM("1");
				pilha_var_mem.top()++;
			}
			int qtd_params = ts.Buscar_Qtd_Params(identificador);
			//Validação semântica para garantir que a quantidade de parâmetros na chamada da função é a mesma que a da declaração
			if (Exp_simples_1() != qtd_params)
				Erro(identificador, ERR_SEM_NUM_PARAMS);
			//MEPA - CHPR Chamada de procedimento
			mepa.CHPR(ts.Buscar_Rotulo(identificador), "1");
			//TODO Atualizar valores que foram passador como referência
			if (categoria == FUNCTION && tipo_exp != TIPO_VOID)
			{
				mepa.CRVL("1", to_string(pilha_var_mem.top()-1));
			}
		}
		else
			Exp_simples_1();
	}
	else
		Erro("Esperado expressão simples");

	return tipo_exp;
}

//ES'
int C_Analise_Sintatica::Exp_simples_1()
{
	string tipo_exp;
	int qtd_params;
	if (token == ABRE_PARENTESES)
	{
		Aceitar_Token(ABRE_PARENTESES, ERR_ABRE_PARENTESES);
		qtd_params = Args();
		Aceitar_Token(FECHA_PARENTESES, ERR_FECHA_PARENTESES);
		return qtd_params;
	}
	else if (token == ABRE_COLCHETES)
	{
		Aceitar_Token(ABRE_COLCHETES, ERR_ABRE_COLCHETES);
		tipo_exp = Exp_soma();
		//validar se é int, array só pode ter indice int 
		if (tipo_exp != TIPO_INT)
			Erro(ERR_SEM_INDEX_TIPO_INT);
		Aceitar_Token(FECHA_COLCHETES, ERR_FECHA_COLCHETES);
	}
}

//LI
string C_Analise_Sintatica::Literal(S_Simbolos& _simbolo)
{
	if (token == FALSO ||
		token == VERDADEIRO)
		Valor_verdade(_simbolo);
	else if (token == CARACTERE)
	{
		_simbolo.tipo = TIPO_CHAR;
		_simbolo.valor = Aceitar_Token(CARACTERE, ERR_CARACTERE);
		//MEPA - CRCT Carrego o valor da constante
		mepa.CRCT(_simbolo.valor);
	}
	else if (token == NUM_INT)
	{
		_simbolo.tipo = TIPO_INT;
		_simbolo.valor = Aceitar_Token(NUM_INT, ERR_NUM);
		//MEPA - CRCT Carrego o valor da constante
		//mepa.CRCT(_simbolo.valor);
		if (eh_argumento)
			mepa.pilha_EXP_args.push(_simbolo.valor);
		else
			mepa.pilha_EXP.push(_simbolo.valor);
	}
	else if (token == NUM_REAL)
	{
		_simbolo.tipo = TIPO_FLOAT;
		_simbolo.valor = Aceitar_Token(NUM_REAL, ERR_NUM_REAL);
		//MEPA - CRCT Carrego o valor da constante
		mepa.CRCT(_simbolo.valor);
	}
	else if (token == STRING)
	{
		_simbolo.tipo = TIPO_STRING;
		_simbolo.valor = Aceitar_Token(STRING, ERR_STRING);
		if (_simbolo.categoria == CONSTANTE)
		{
			//MEPA - CRCT Carrego o valor da constante - Neste caso de String, desmembro ela e grava o CRCT
			mepa.CRCT_String(_simbolo.valor);
		}
		else
		{
			//MEPA - Verifico se é comando escrita
			if (!mepa.pilha_Com_Escrita.empty())
			{
				// MEPA - IMPC Se não for da categoria constante, leio o valor e imprimo o caractere
				mepa.IMPC(_simbolo.valor);
				//MEPA - IMPE Adiciono enter somente se for PRINTLN, PRINT imprime na mesma linha
				if (mepa.pilha_Com_Escrita.top() == PRINTLN)
					mepa.IMPE();
				mepa.pilha_Com_Escrita.pop();
			}
		}
	}
	else
		Erro("Esperado literal");

	return _simbolo.tipo;
}

//VV
void C_Analise_Sintatica::Valor_verdade(S_Simbolos& _simbolo)
{
	if (token == FALSO)
	{
		_simbolo.tipo = TIPO_BOOLEANO;
		_simbolo.valor = Aceitar_Token(FALSO, ERR_FALSO);
	}
	else if (token == VERDADEIRO)
	{
		_simbolo.tipo = TIPO_BOOLEANO;
		_simbolo.valor = Aceitar_Token(VERDADEIRO, ERR_VERDADEIRO);
	}
}

//AR
int C_Analise_Sintatica::Args()
{
	int resultado;
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
		//MEPA - Inicio o armazenamento dos argumentos
		//Inicio a flag para argumentos para true
		eh_argumento = true;
		//limpo o vetor que vai armazenar as expressões 
		mepa.vetor_EXP.clear();
		//Preciso adicionar 1 pois a função de lista_exp devolve a quantidade contada do segundo argumento em diantes
		resultado = Lista_exp() + 1;
		//Valido as expressões de cada argumento
		//TODO
		//Fecho a flag para argumentos = false
		eh_argumento = false;
		return resultado;
	}
	else if (token != FECHA_PARENTESES)
		Erro(ERR_FECHA_PARENTESES);
}

//LV
void C_Analise_Sintatica::Lista_var()
{
	string identificador;
	string tipo;

	identificador = Id_Composto();
	tipo = ts.Buscar_Tipo(identificador);

	if (tipo != CARACTERE || tipo != STRING)
	{
		//MEPA - LEIT Leitura de inteiro
		mepa.LEIT();
		//MEPA - IMPE Imprime enter após a leitura
		mepa.IMPE();
	}
	else
	{
		//MEPA - LECH Leitura de caractere
		mepa.LECH();
		//MEPA - IMPE Imprime enter após a leitura
		mepa.IMPE();
	}

	if (ts.Buscar_Categoria(identificador) == VAR)
	{
		//MEPA - ARMZ Armazenar o valor da variável na posição correta
		mepa.ARMZ(to_string(ts.Buscar_Pos_Pilha(identificador)));
	}
		
	Lista_var_1();
}

//LV'
void C_Analise_Sintatica::Lista_var_1()
{
	string tipo_exp;
	if (token == VIRGULA)
	{
		Aceitar_Token(VIRGULA, ERR_VIRGULA);
		Lista_var();
	}
	else if (token == ABRE_COLCHETES)
	{
		Aceitar_Token(ABRE_COLCHETES, ERR_ABRE_COLCHETES);
		tipo_exp = Exp_soma();
		//validar se é int, array só pode ter indice int 
		if (tipo_exp != TIPO_INT)
			Erro(ERR_SEM_INDEX_DECL_TIPO_INT);
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
	//svar.pos_pilha++;
	//posicao_pilha_local++;
	//posicao_pilha_global++;
	pilha_var_mem.top()++;

	svar.identificador = Aceitar_Token(IDENTIFICADOR, ERR_IDENTIFICADOR);
	svar.linha = iter_token_lexema->linha;

	Lista_decl_var_1();
}

void C_Analise_Sintatica::Lista_decl_var_1()
{
	string tipo_exp;
	if (token == VIRGULA)
	{
		Aceitar_Token(VIRGULA, ERR_VIRGULA);
		svar.access = acesso_membro;
		//Atribuo o valor da pilha
		//if (svar.pai == 0) //Global
			//svar.pos_pilha = posicao_pilha_global;
		//else //Local
			svar.pos_pilha = pilha_var_mem.top()-1;
		//Se encontrei virgula, inserir variável na tabela de símbolo
		ts.Inserir(svar);
		//Nova variável virá, incremento a chave
		svar.chave = ++chave;
		//Volto Array para falso
		svar.array = false;

		Lista_decl_var();
	}
	else if (token == ABRE_COLCHETES)
	{
		Aceitar_Token(ABRE_COLCHETES, ERR_ABRE_COLCHETES);
		tipo_exp = Exp_soma();
		//validar se é int, array só pode ter indice int 
		if (tipo_exp != TIPO_INT)
			Erro(ERR_SEM_INDEX_DECL_TIPO_INT);
		Aceitar_Token(FECHA_COLCHETES, ERR_FECHA_COLCHETES);
		//Se abre colchetes, é array
		svar.array = true;
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
		svar.access = acesso_membro;
		//Se encontrei virgula, inserir variável na tabela de símbolo
		ts.Inserir(svar);
		//Nova variável virá, incremento a chave
		svar.chave = ++chave;
		//Volto Array para falso
		svar.array = false;

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

void C_Analise_Sintatica::Iniciar_Simbolos(S_Simbolos &_simbolo)
{
	_simbolo.chave = ++chave;
	_simbolo.categoria = "";
	_simbolo.identificador = "";
	_simbolo.tipo = "";
	_simbolo.array = false;
	_simbolo.valido = false;
	_simbolo.valor = "";
	_simbolo.linha = 0;
	_simbolo.passby = "";
	_simbolo.classe = 0;
	_simbolo.pos_pilha = -1;
	_simbolo.rotulo = "";
}

string C_Analise_Sintatica::Retorna_Tipo_Comparado(string _tipo1, string _tipo2)
{
	if (_tipo1 == TIPO_STRING ||
		_tipo1 == TIPO_VOID ||
		_tipo2 == TIPO_STRING ||
		_tipo2 == TIPO_VOID)
		Erro(ERR_SEM_INCOMPATIBILIDADE_TIPO);
	else if (_tipo1 == TIPO_FLOAT || _tipo2 == TIPO_FLOAT)
		return TIPO_FLOAT;

	//Se cair aqui é porque é TIPO_INT ou TIPO_CHAR ou TIPO_BOOLEANO
	return _tipo1;
}

void C_Analise_Sintatica::Validar_Atribuicao(string _tipo_esquerda, string _tipo_direita)
{
	if (_tipo_esquerda == TIPO_BOOLEANO)
	{
		//BOOL aceita BOOL ou INT
		if (_tipo_direita != TIPO_BOOLEANO && _tipo_direita != TIPO_INT)
			Erro(ERR_SEM_ATRIB_BOOL);
	}
	else if (_tipo_esquerda == TIPO_CHAR)
	{
		//CHAR aceita CHAR ou INT
		if (_tipo_direita != TIPO_CHAR && _tipo_direita != TIPO_INT)
			Erro(ERR_SEM_ATRIB_CHAR);
	}
	else if (_tipo_esquerda == TIPO_FLOAT)
	{
		//FLOAT aceita FLOAT ou INT
		if (_tipo_direita != TIPO_FLOAT && _tipo_direita != TIPO_INT)
			Erro(ERR_SEM_ATRIB_FLOAT);
	}
	else if (_tipo_esquerda == TIPO_INT)
	{
		//INT aceita INT ou CHAR
		if (_tipo_direita != TIPO_CHAR && _tipo_direita != TIPO_INT)
			Erro(ERR_SEM_ATRIB_INTEIRO);
	}
	else if (_tipo_esquerda == TIPO_STRING)
	{
		//STRING aceita apenas STRING
		if (_tipo_direita != _tipo_esquerda)
			Erro(ERR_SEM_ATRIB_STRING);
	}
	else
		Erro(ERR_SEM_INCOMPATIBILIDADE_TIPO);
}

void C_Analise_Sintatica::Gravar_Arquivos(string _nome_arquivo)
{
	ts.Imprimir_TS(_nome_arquivo);
	ts.Gravar_TS(_nome_arquivo);
	mepa.Gerar_Arquivo(_nome_arquivo);
}
