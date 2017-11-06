#include "C_Analise_Sintatica.h"




C_Analise_Sintatica::C_Analise_Sintatica(vector<S_Token_Lexema> _tabela_token_lexema)
{
	chave = 0;
	//posicao_pilha_global = -1;
	existe_main = false;
	existe_CHPR_main = false;
	eh_argumento = false;
	Proc_Func_Atual = "";
	escopo_estrutura = false;
	escopo_classe = false;
	escopo = "";
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
	Remover_Mem_Classe();
	Inserir_DMEM_MEPA(pilha_var_mem.top());

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
		escopo = FUNCTION;

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

		escopo = "";
	}
	else if (token == SUB)
	{
		escopo = SUB;

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

		escopo = "";
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
		escopo = ESTRUTURA;

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

		escopo = "";
	}
	else if (token == CLASSE)
	{
		escopo = CLASSE;

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

		escopo = "";
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

	ts.Inserir(sconst, escopo);
}

void C_Analise_Sintatica::Decl_const_class()
{
	sconst.identificador = Aceitar_Token(IDENTIFICADOR, ERR_IDENTIFICADOR);

	sconst.linha = iter_token_lexema->linha;

	Aceitar_Token(OP_ATRIBUICAO, ERR_OP_ATRIBUICAO);

	Literal(sconst);

	Aceitar_Token(PONTO_VIRGULA, ERR_PONTO_VIRGULA);

	sconst.access = acesso_membro;

	ts.Inserir(sconst, escopo);
}

//DV
void C_Analise_Sintatica::Decl_var()
{
	Aceitar_Token(VAR, ERR_VAR);

	svar.categoria = VAR;

	if (token == REF) 
		Aceitar_Token(REF, ERR_REF);
	else if (token == POINTER)
	{
		Aceitar_Token(POINTER, ERR_POINTER);
		svar.ponteiro = true;
	}
		
	svar.tipo = Espec_tipo();

	//Zerar var_mem a cada declaração
	var_mem = 0;
	//posicao_pilha_local = -1;
	Lista_decl_var();

	if (escopo_estrutura)
	{
		sstruct.qtd_params += var_mem;
	}
	else if (escopo_classe)
	{
		sclasse.qtd_params += var_mem;
	}

	svar.access = acesso_membro;

	//Verifico as condições para reservar memória para a MEPA
	Inserir_AMEM_MEPA();

	Aceitar_Token(PONTO_VIRGULA, ERR_PONTO_VIRGULA);
}

void C_Analise_Sintatica::Decl_var_class()
{
	Aceitar_Token(VAR, ERR_VAR);

	svar.categoria = VAR;

	if (token == REF)
		Aceitar_Token(REF, ERR_REF);
	else if (token == POINTER)
	{
		Aceitar_Token(POINTER, ERR_POINTER);
		svar.ponteiro = true;
	}

	svar.tipo = Espec_tipo();

	//Zerar var_mem a cada declaração
	var_mem = 0;
	//posicao_pilha_local = -1;
	Lista_decl_var();

	if (escopo_estrutura)
	{
		sstruct.qtd_params += var_mem;
	}
	else if (escopo_classe)
	{
		sclasse.qtd_params += var_mem;
	}

	svar.access = acesso_membro;

	//Inserir as variáveis da classe em uma pilha. Após isso, utilizo ela para carregar as variáveis em todas as chamadas de funções 

	//Verifico as condições para reservar memória para a MEPA
	Inserir_AMEM_MEPA();

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
	else if (token == IDENTIFICADOR)
	{
		string identificador;
		string categoria;
		identificador = Id_Composto();
		categoria = ts.Buscar_Categoria(identificador);
		if (categoria == ESTRUTURA || categoria == CLASSE)
		{
			//Aceitar_Token(IDENTIFICADOR, ERR_IDENTIFICADOR);
			return identificador;
		}
		else
			Erro("Esperado tipo estrutura/classe");
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
		mepa.Add_Comando("; ----------Inicio MAIN");
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

	int qtd_mem_alocada_param = ts.Buscar_Qtd_Mem_Alocada_Params(sproc.chave);
	int qtd_tot_params = ts.Buscar_Qtd_Tot_Params(sproc.chave);

	//Se existir parametros por valor, preciso armazenar memória
	if (qtd_mem_alocada_param > 0)
	{
		mepa.AMEM(to_string(qtd_mem_alocada_param));
		pilha_var_mem.top() += qtd_mem_alocada_param;
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
	ts.Inserir(sproc, escopo);

	Aceitar_Token(END_SUB, ERR_END_SUB);
	
	//Quando precisar ir para o fim do procedimento
	//mepa.Add_Comando(sproc.rotulo + "_FIM:NADA");

	//Inativar todas as variáveis que foram declaradas dentro desta procedure/subprograma
	//int qtd_removida = ts.Remover_Internos(sproc.identificador);
	ts.Remover_Internos(sproc.identificador);
	//Preciso desempilhar a variável que controla a posição da pilha, somente se tiver removido
	if (!pilha_var_mem.empty() && pilha_var_mem.top() > 0)
	{
		Inserir_DMEM_MEPA(pilha_var_mem.top());
	}
	//MEPA - RTPR Retorno da procedure
	mepa.RTPR("1", to_string(qtd_tot_params));

	qtd_param.val = 0;
	qtd_param.ref = 0;
	Proc_Func_Atual = "";
	//Tiro do topo o contador de memoria desta função (Caso não tenha sido desempilhado anterirmente por um retorno)
	if (!pilha_var_mem.empty())
		pilha_var_mem.pop();
}

void C_Analise_Sintatica::Decl_proc_class()
{
	Aceitar_Token(SUB, ERR_SUB);

	sproc.linha = iter_token_lexema->linha;
	sproc.identificador = Aceitar_Token(IDENTIFICADOR, ERR_IDENTIFICADOR);

	Proc_Func_Atual = sproc.identificador;

	//Garanto que exista um Subprograma Main
	if (sproc.identificador == "main")
	{
		existe_main = true;
		mepa.Add_Comando("; ----------Inicio MAIN");
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

	int qtd_mem_alocada_param = ts.Buscar_Qtd_Mem_Alocada_Params(sproc.chave);
	int qtd_tot_params = ts.Buscar_Qtd_Tot_Params(sproc.chave);

	//Procurar variáveis da classe, para inserir como endereço
	vector<S_Simbolos> l_simbolos;
	l_simbolos = ts.Buscar_Var_Pelo_Pai(sclasse.chave);
	
	/*for (auto it = l_simbolos.begin(); it != l_simbolos.end(); it++)
	{
		mepa.CREN("1", to_string(it->pos_pilha));
	}*/



	//Se existir parametros por valor, preciso armazenar memória
	if (qtd_mem_alocada_param > 0)
	{
		mepa.AMEM(to_string(qtd_mem_alocada_param));
		pilha_var_mem.top() += qtd_mem_alocada_param;
	}


	//Atualizar posição da pilha dos parâmetros e envio uma pilha para depois conseguir atribuir os valores para os parâmetros passados por cópia
	if (sproc.qtd_params > 0)
	{
		stack<pair<int, int>> pparamval;
		ts.Atualizar_Pilha_Param_classe(sproc.chave, sproc.qtd_params, pparamval, l_simbolos.size());
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
	ts.Inserir(sproc, escopo);

	Aceitar_Token(END_SUB, ERR_END_SUB);

	//Quando precisar ir para o fim do procedimento
	//mepa.Add_Comando(sproc.rotulo + "_FIM:NADA");

	//Inativar todas as variáveis que foram declaradas dentro desta procedure/subprograma
	//int qtd_removida = ts.Remover_Internos(sproc.identificador);
	ts.Remover_Internos(sproc.identificador);
	//Preciso desempilhar a variável que controla a posição da pilha, somente se tiver removido
	if (!pilha_var_mem.empty() && pilha_var_mem.top() > 0)
	{
		Inserir_DMEM_MEPA(pilha_var_mem.top());
	}

	int simb_retorno = 0; //Armazeno o total de memoria para retorno
	for (auto it = l_simbolos.begin(); it != l_simbolos.end(); it++)
	{
		if (it->tipo == TIPO_STRING)
		{
			simb_retorno += it->pos_pilha_ini_str + it->pos_pilha + 1;
		}
		else
		{
			simb_retorno += 1;
		}
	}

	//MEPA - RTPR Retorno da procedure
	mepa.RTPR("1", to_string(qtd_tot_params + simb_retorno));

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

	int qtd_mem_alocada_param = ts.Buscar_Qtd_Mem_Alocada_Params(sfuncao.chave);
	int qtd_tot_params = ts.Buscar_Qtd_Tot_Params(sfuncao.chave);

	//Se existir parametros por valor, preciso armazenar memória
	if (qtd_mem_alocada_param > 0)
	{
		mepa.AMEM(to_string(qtd_mem_alocada_param));
		pilha_var_mem.top() += qtd_mem_alocada_param;
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
	ts.Inserir(sfuncao, escopo);

	Bloco();

	Aceitar_Token(END_FUNCTION, ERR_END_FUNCTION);

	//Quando precisar ir para o fim da função
	//mepa.Add_Comando(sfuncao.rotulo + "_FIM:NADA");

	//Inativar todas as variáveis que foram declaradas dentro desta função
	int qtd_removida =	ts.Remover_Internos(sfuncao.identificador);

	//Preciso desempilhar a variável que controla a posição da pilha, somente se tiver removido
	if (qtd_removida > 0)
	{
		Inserir_DMEM_MEPA(qtd_removida);
	}

	//MEPA - RTPR Retorno da procedure
	mepa.RTPR("1", to_string(qtd_tot_params));

	qtd_param.val = 0;
	qtd_param.ref = 0;
	Proc_Func_Atual = "";
	//Tiro do topo o contador de memoria desta função (Caso não tenha sido desempilhado anterirmente por um retorno)
	if (!pilha_var_mem.empty())
		pilha_var_mem.pop();
}

void C_Analise_Sintatica::Decl_func_class()
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

	int qtd_mem_alocada_param = ts.Buscar_Qtd_Mem_Alocada_Params(sfuncao.chave);
	int qtd_tot_params = ts.Buscar_Qtd_Tot_Params(sfuncao.chave);

	//Procurar variáveis da classe, para inserir como endereço
	vector<S_Simbolos> l_simbolos;
	l_simbolos = ts.Buscar_Var_Pelo_Pai(sclasse.chave);

	//Se existir parametros por valor, preciso armazenar memória
	if (qtd_mem_alocada_param > 0)
	{
		mepa.AMEM(to_string(qtd_mem_alocada_param));
		pilha_var_mem.top() += qtd_mem_alocada_param;
	}


	//Atualizar posição da pilha dos parâmetros e envio uma pilha para depois conseguir atribuir os valores para os parâmetros passados por cópia
	if (sfuncao.qtd_params > 0)
	{
		stack<pair<int, int>> pparamval;
		ts.Atualizar_Pilha_Param_classe(sfuncao.chave, sfuncao.qtd_params, pparamval, l_simbolos.size());
		//Para os params que foram passador por valor, armazeno os valores localmente 
		while (!pparamval.empty())
		{
			mepa.CRVL("1", to_string(pparamval.top().first));
			mepa.ARMZ("1", to_string(pparamval.top().second));
			pparamval.pop();
		}
	}

	//Faço a inserção na TS logo após a declaração, isso precisa ser feito em caso seja uma função recursiva
	ts.Inserir(sfuncao, escopo);

	Bloco();

	Aceitar_Token(END_FUNCTION, ERR_END_FUNCTION);

	//Quando precisar ir para o fim da função
	//mepa.Add_Comando(sfuncao.rotulo + "_FIM:NADA");

	//Inativar todas as variáveis que foram declaradas dentro desta função
	int qtd_removida = ts.Remover_Internos(sfuncao.identificador);
	//Preciso desempilhar a variável que controla a posição da pilha, somente se tiver removido
	if (qtd_removida > 0)
	{
		Inserir_DMEM_MEPA(qtd_removida);
	}

	int simb_retorno = 0; //Armazeno o total de memoria para retorno
	for (auto it = l_simbolos.begin(); it != l_simbolos.end(); it++)
	{
		if (it->tipo == TIPO_STRING)
		{
			simb_retorno += it->pos_pilha_ini_str + it->pos_pilha + 1;
		}
		else
		{
			simb_retorno += 1;
		}
	}

	//MEPA - RTPR Retorno da procedure
	mepa.RTPR("1", to_string(qtd_tot_params + simb_retorno));

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
	ts.Inserir(sparams, escopo);

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
		sparams.ponteiro = true;
		qtd_param.ref++;
	}
	else
		Erro("Esperado mode(value ou ref)");
}

//DS
void C_Analise_Sintatica::Decl_struct()
{
	Aceitar_Token(ESTRUTURA, ERR_STRUCT);

	escopo_estrutura = true; //é um escopo de estrutura. Utilizado para declarar ou não as variáveis dessa estrutura na MEPA
	sstruct.linha = iter_token_lexema->linha;

	sstruct.identificador = Aceitar_Token(IDENTIFICADOR, ERR_IDENTIFICADOR);
	

	Lista_decl_struct();

	Aceitar_Token(END_STRUCT, ERR_END_STRUCT);

	sstruct.access = acesso_membro;
	ts.Inserir(sstruct, escopo);

	pilha_var_mem.top() = 0; //Não preciso fazer o DMEM, por isso fica 0
	escopo_estrutura = false; //Volto para false, pois terminou a declaração
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

	escopo_classe = true; //é um escopo de classe. Utilizado para declarar ou não as variáveis dessa classe na MEPA
	sclasse.linha = iter_token_lexema->linha;

	sclasse.identificador = Aceitar_Token(IDENTIFICADOR, ERR_IDENTIFICADOR);

	//Caso seja herança
	if (token == DOIS_PONTOS)
	{
		Aceitar_Token(DOIS_PONTOS, ERR_DOIS_PONTOS);
		sclasse.pai_heranca = ts.Buscar_Chave(Aceitar_Token(IDENTIFICADOR, ERR_IDENTIFICADOR));
		Inserir_Membros_Classe_Pai(sclasse.pai_heranca, sclasse.chave);
	}

	Bloco_class();

	Aceitar_Token(END_CLASS, ERR_END_CLASS);

	ts.Inserir(sclasse, escopo);


	escopo_classe = false; //Volto para false, pois terminou a declaração
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

		Decl_const_class();
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

		Decl_func_class();

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

		Decl_proc_class();

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

		Decl_var_class();
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
	else if (token == ENDERECO_ELEMENTO || token == IDENTIFICADOR || token == ID_SEL_IDENTIFICADOR || token == ID_SEL_PONTEIRO)
	{
		string identificador;
		string categoria;
		string tipo;
		string qtd_params_decl;
		string qtd_params_chamada;
		int parente;
		identificador = Id_Composto();
		categoria = ts.Buscar_Categoria(identificador);
	    if (categoria == CONSTANTE) //Constante não pode ser atribuída
			Erro(identificador, ERR_SEM_CONST_ATRIB);
		qtd_params_decl = ts.Buscar_Qtd_Params(identificador);

		if (!pilha_selecao.empty())
		{
			parente = ts.Buscar_Chave(pilha_selecao.top().idendificador);
			pilha_selecao.pop();
			while (!pilha_selecao.empty())
				pilha_selecao.pop();
		}
		else
			parente = ts.Buscar_Pai(identificador);
			

		//Verifico se o acesso ao membro é permitido
		if (escopo != CLASSE && !ts.Consultar_Acesso(identificador, parente))
			Erro(identificador, ERR_SEM_NAO_ACESSIVEL_PRIVADO);

		//MEPA - Empilho a variável, irá desempilhar somente depois que inserir a CRCT/CRVL do valor atribuído
		if (categoria == VAR || categoria == PARAM)
			//Par para o pilha_armz - Com isso no armz saberei a diferenca
			mepa.pilha_ARMZ.push(sidpai.make_Id_Pai(identificador, parente));

		//TODO Separar memória para retorno de função

		//TODO Separar memória para os parâmetros passador por referência


		tipo = ts.Buscar_Tipo(identificador);
		//Comando_1 retorna a quantidade de parâmetros e envio o tipo do identificador para verificação de tipos
		qtd_params_chamada = Comando_1(tipo);

		//MEPA - Desempilho
		if ((categoria == VAR || categoria == PARAM) && !mepa.pilha_ARMZ.empty())
		{
			//pai para verificar se é variável global ou está dentro do main. 0=Global 1=Main
			int pai = mepa.pilha_ARMZ.top().parente;
			if (pai != 0)
				pai = 1;
			if (ts.Verificar_Ponteiro(mepa.pilha_ARMZ.top().identificador) && mepa.ultimo_comando != "CREN")
				mepa.ARMI(to_string(pai), to_string(ts.Buscar_Pos_Pilha(sidpai.make_Id_Pai(mepa.pilha_ARMZ.top().identificador, mepa.pilha_ARMZ.top().parente))));
			else
			{
				if (escopo != CLASSE)
				{
					if (tipo == TIPO_STRING)
					{
						int pos_pilha_ini = ts.Buscar_Pos_Pilha_Ini_Str(sidpai.make_Id_Pai(mepa.pilha_ARMZ.top().identificador, mepa.pilha_ARMZ.top().parente));
						int pos_pilha = ts.Buscar_Pos_Pilha(sidpai.make_Id_Pai(mepa.pilha_ARMZ.top().identificador, mepa.pilha_ARMZ.top().parente));
						mepa.CRCT_String(val_string, pos_pilha_ini, pos_pilha);
						ts.Atualizar_Tamanho_String(sidpai.make_Id_Pai(mepa.pilha_ARMZ.top().identificador, mepa.pilha_ARMZ.top().parente), val_string);
					}
					else
						mepa.ARMZ(to_string(pai), to_string(ts.Buscar_Pos_Pilha(sidpai.make_Id_Pai(mepa.pilha_ARMZ.top().identificador, mepa.pilha_ARMZ.top().parente))));
				}
				else
				{
					if (tipo == TIPO_STRING)
					{
						int pos_pilha_ini = ts.Buscar_Pos_Pilha_Ini_Str(sidpai.make_Id_Pai(mepa.pilha_ARMZ.top().identificador, mepa.pilha_ARMZ.top().parente)) + 4 *(-1);
						int pos_pilha = ts.Buscar_Pos_Pilha(sidpai.make_Id_Pai(mepa.pilha_ARMZ.top().identificador, mepa.pilha_ARMZ.top().parente));
						mepa.CRCT_String_ARMI(val_string, pos_pilha_ini - pos_pilha);
						ts.Atualizar_Tamanho_String(sidpai.make_Id_Pai(mepa.pilha_ARMZ.top().identificador, mepa.pilha_ARMZ.top().parente), val_string);
					}
					else
						mepa.ARMI(to_string(pai), to_string((ts.Buscar_Pos_Pilha(sidpai.make_Id_Pai(mepa.pilha_ARMZ.top().identificador, mepa.pilha_ARMZ.top().parente))+4)*(-1)));
				}
			}
				
			mepa.pilha_ARMZ.pop();
		}

		
		if (categoria == FUNCTION || categoria == SUB)
		{
			vector<S_Simbolos> l_simbolos;
			l_simbolos = ts.Buscar_Var_Classe_Pai(parente);
			auto it = l_simbolos.end();
			while (it != l_simbolos.begin())
			{
				it--;
				if (it->tipo == TIPO_STRING)
				{
					for (int i = it->pos_pilha; i >= it->pos_pilha_ini_str; i--)
					{
						mepa.CREN("1", to_string(i));
					}
				}
				else
				{
					mepa.CREN("1", to_string(it->pos_pilha));
				}
			}

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
		if (tipo_direita != TIPO_STRING)
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
	mepa.Add_Comando("; ----------Inicio Comando de Selecao IF");
	Aceitar_Token(CONDICAO_IF, ERR_CONDICAO_IF);
	tipo_exp = Exp();
	//Avalidar Expressão
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
	mepa.Add_Comando("; ----------FIM Comando de Selecao IF");
}

//CS'
void C_Analise_Sintatica::Com_selecao_1()
{
	if (token == CONDICAO_ELSE)
	{
		mepa.Add_Comando("; ----------Comando de Selecao ELSE");
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
		mepa.Add_Comando("; ----------Inicio Comando repeticao FOR");
		Aceitar_Token(LACO_FOR, ERR_LACO_FOR);
		identificador = Id_Composto();
		int parente = 0;
		if (!pilha_selecao.empty())
		{
			parente = ts.Buscar_Chave(pilha_selecao.top().idendificador);
			pilha_selecao.pop();
			while (!pilha_selecao.empty())
				pilha_selecao.pop();
		}
		else
			parente = ts.Buscar_Pai(identificador);

		//Verifico se o acesso ao membro é permitido
		if (escopo != CLASSE && !ts.Consultar_Acesso(identificador, parente))
			Erro(identificador, ERR_SEM_NAO_ACESSIVEL_PRIVADO);

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
		int pai = parente;
		if (parente == 0)
			pai = ts.Buscar_Pai(identificador);
		if (pai != 0)
			pai = 1;
		mepa.ARMZ(to_string(pai), to_string(ts.Buscar_Pos_Pilha(sidpai.make_Id_Pai(identificador, parente))));
		//MEPA - NADA -> Desvia sempre pra cá, só não quando a expressão é falsa
		DSVS = mepa.NADA();
		Aceitar_Token(LACO_TO, ERR_LACO_TO);
		//MEPA - CRVL Carrego o valor atual do identificador
		mepa.CRVL(to_string(pai), to_string(ts.Buscar_Pos_Pilha(sidpai.make_Id_Pai(identificador, parente))));
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
		mepa.CRVL(to_string(pai), to_string(ts.Buscar_Pos_Pilha(sidpai.make_Id_Pai(identificador, parente))));
		//MEPA - CRCT Carrego constante 1 para adicionar no NEXT
		mepa.CRCT("1");
		//MEPA - SOMA Somo +1 no identificador para o NEXT
		mepa.SOMA();
		//MEPA - ARMZ Armazeno o novo valor no identificador
		mepa.ARMZ(to_string(pai), to_string(ts.Buscar_Pos_Pilha(sidpai.make_Id_Pai(identificador, parente))));
		//MEPA - DSVS - Desvio sempre ao fim do bloco, nova verificação do FOR
		mepa.DSVS(DSVS);
		//MEPA - NADA - caso o FOR tenha sido falso cai aqui
		mepa.NADA(DSVF);
		mepa.Add_Comando("; ----------FIM Comando repeticao FOR");
	}
	else if (token == LACO_REPEAT)
	{
		mepa.Add_Comando("; ----------Inicio Comando repeticao REPEAT");
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
		mepa.Add_Comando("; ----------FIM Comando repeticao REPEAT");
	}
	else if (token == LACO_WHILE)
	{
		mepa.Add_Comando("; ----------Inicio Comando repeticao WHILE");
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
		mepa.Add_Comando("; ----------FIM Comando repeticao WHILE");
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
			Inserir_DMEM_MEPA(pilha_var_mem.top());
			mepa.RTPR("1", to_string(qtd_params));
		}
		else
		{
			if (!pilha_var_mem.empty() && pilha_var_mem.top() > 0)
				Inserir_DMEM_MEPA(pilha_var_mem.top());
			mepa.RTPR("1", to_string(qtd_params));
		}
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

	if (token == ENDERECO_ELEMENTO)
	{
		Aceitar_Token(ENDERECO_ELEMENTO, ERR_ENDERECO_ELEMENTO);
	}
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
		//Empilho a classe/estrutura que terá a propriedade seguinte
		S_Selelecao_ID sselid;
		pilha_selecao.push(sselid.make_pair(identificador, VALUE));
		if (!ts.Consultar(identificador))
			Erro(identificador, ERR_SEM_NAO_DECLARDO);
		identificador = Id_Composto();
	}
		
	else if (token == ID_SEL_PONTEIRO)
	{
		identificador = Aceitar_Token(ID_SEL_PONTEIRO, ERR_OP_SELECAO_PONTEIRO);
		//Remover o -> 
		identificador = identificador.substr(0, identificador.size() - 2);
		//Empilho a classe/estrutura que terá a propriedade seguinte
		S_Selelecao_ID sselid;
		pilha_selecao.push(sselid.make_pair(identificador, REF));
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
		token == ENDERECO_ELEMENTO ||
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
			S_Exp_Args s_exp_args;
			s_exp_args = s_exp_args.make_pair(mepa.pilha_EXP_args, ts);
			mepa.pilha_EXP_args = stack<S_EXP>();
			//TODO 06 - Validar Expressão aqui
			//mepa.Avaliar_Expressao(mepa.pilha_EXP_args, ts);
			pilha_Exp_args.push(s_exp_args);
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
	if (token == ENDERECO_ELEMENTO ||
		token == IDENTIFICADOR ||
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
		//sexpressao = { "CMME", false };
		if (eh_argumento)
			mepa.pilha_EXP_args.push(sexpressao.make_exp("CMME", false, 0));
		else
			mepa.pilha_EXP.push(sexpressao.make_exp("CMME", false, 0));
	}
	else if (token == OP_MENOR_IGUAL)
	{
		Aceitar_Token(OP_MENOR_IGUAL, ERR_OP_MENOR_IGUAL);
		//sexpressao = { "CMEG", false };
		if (eh_argumento)
			mepa.pilha_EXP_args.push(sexpressao.make_exp("CMEG", false, 0));
		else
			mepa.pilha_EXP.push(sexpressao.make_exp("CMEG", false, 0));
	}
	else if (token == OP_DIFERENTE)
	{ 
		Aceitar_Token(OP_DIFERENTE, ERR_OP_DIFERENTE);
		//sexpressao = { "CMDG", false };
		if (eh_argumento)
			mepa.pilha_EXP_args.push(sexpressao.make_exp("CMDG", false, 0));
		else
			mepa.pilha_EXP.push(sexpressao.make_exp("CMDG", false, 0));
	}
	else if (token == OP_IGUALDADE)
	{
		Aceitar_Token(OP_IGUALDADE, ERR_OP_IGUALDADE);
		//sexpressao = { "CMIG", false };
		if (eh_argumento)
			mepa.pilha_EXP_args.push(sexpressao.make_exp("CMIG", false, 0));
		else
			mepa.pilha_EXP.push(sexpressao.make_exp("CMIG", false, 0));
	}
	else if (token == OP_MAIOR)
	{
		Aceitar_Token(OP_MAIOR, ERR_OP_MAIOR);
		//sexpressao = { "CMMA", false };
		if (eh_argumento)
			mepa.pilha_EXP_args.push(sexpressao.make_exp("CMMA", false, 0));
		else
			mepa.pilha_EXP.push(sexpressao.make_exp("CMMA", false, 0));
	}
	else if (token == OP_MAIOR_IGUAL)
	{
		Aceitar_Token(OP_MAIOR_IGUAL, ERR_OP_MAIOR_IGUAL);
		//sexpressao = { "CMAG", false };
		if (eh_argumento)
			mepa.pilha_EXP_args.push(sexpressao.make_exp("CMAG", false, 0));
		else
			mepa.pilha_EXP.push(sexpressao.make_exp("CMAG", false, 0));
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
		token == ENDERECO_ELEMENTO ||
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
		//sexpressao = { "SUBT", false };
		//MEPA - SUBT Operador subtração
		if (eh_argumento)
			mepa.pilha_EXP_args.push(sexpressao.make_exp("SUBT", false, 0));
		else
			mepa.pilha_EXP.push(sexpressao.make_exp("SUBT", false, 0));
	}
	else if (token == OP_ADICAO)
	{
		Aceitar_Token(OP_ADICAO, ERR_OP_ADICAO);
		//sexpressao = { "SOMA", false };
		//MEPA - SOMA Operador soma
		if (eh_argumento)
			mepa.pilha_EXP_args.push(sexpressao.make_exp("SOMA", false, 0));
		else
			mepa.pilha_EXP.push(sexpressao.make_exp("SOMA", false, 0));
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
		token == ENDERECO_ELEMENTO ||
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
		//sexpressao = { "MULT", false };
		if (eh_argumento)
			mepa.pilha_EXP_args.push(sexpressao.make_exp("MULT", false, 0));
		else
			mepa.pilha_EXP.push(sexpressao.make_exp("MULT", false, 0));
	}
	else if (token == OP_DIVISAO)
	{
		Aceitar_Token(OP_DIVISAO, ERR_OP_DIVISAO);
		//sexpressao = { "DIVI", false };
		if (eh_argumento)
			mepa.pilha_EXP_args.push(sexpressao.make_exp("DIVI", false, 0));
		else
			mepa.pilha_EXP.push(sexpressao.make_exp("DIVI", false, 0));
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
		//sexpressao = { ABRE_PARENTESES, false };
		if (eh_argumento)
			mepa.pilha_EXP_args.push(sexpressao.make_exp(ABRE_PARENTESES, false, 0));
		else
			mepa.pilha_EXP.push(sexpressao.make_exp(ABRE_PARENTESES, false, 0));
		tipo_exp = Exp();
		Aceitar_Token(FECHA_PARENTESES, ERR_FECHA_PARENTESES);
		//sexpressao = { FECHA_PARENTESES, false };
		if (eh_argumento)
			mepa.pilha_EXP_args.push(sexpressao.make_exp(FECHA_PARENTESES, false, 0));
		else
			mepa.pilha_EXP.push(sexpressao.make_exp(FECHA_PARENTESES, false, 0));
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
	else if (token == ENDERECO_ELEMENTO || token == IDENTIFICADOR || token == ID_SEL_IDENTIFICADOR || token == ID_SEL_PONTEIRO)
	{
		string temp_token = token;
		string identificador;
		string categoria;
		identificador = Id_Composto();
		int parente = 0;
		if (!pilha_selecao.empty())
		{
			parente = ts.Buscar_Chave(pilha_selecao.top().idendificador);
			pilha_selecao.pop();
			while (!pilha_selecao.empty())
				pilha_selecao.pop();
		}
		else
			parente = ts.Buscar_Pai(identificador);

		//Verifico se o acesso ao membro é permitido
		if (escopo != CLASSE && !ts.Consultar_Acesso(identificador, parente))
			Erro(identificador, ERR_SEM_NAO_ACESSIVEL_PRIVADO);

		//Buscar a categoria do identificador
		categoria = ts.Buscar_Categoria(identificador);
		//Buscar o tipo do identificador
		tipo_exp = ts.Buscar_Tipo(identificador);

		//MEPA
		if (categoria == VAR || categoria == PARAM)
		{
			int pai = parente;
			if (parente == 0)
				pai = ts.Buscar_Pai(identificador);
			if (pai != 0)
				pai = 1;
			if (eh_argumento)
				mepa.pilha_EXP_args.push(sexpressao.make_exp(identificador, temp_token == ENDERECO_ELEMENTO, parente));
			else
				mepa.pilha_EXP.push(sexpressao.make_exp(identificador, temp_token == ENDERECO_ELEMENTO, parente));

			//Se tiver algum comando de escrita empilhado, preciso inserir ele na MEPA
			if (!mepa.pilha_Com_Escrita.empty())
			{
				if (escopo != CLASSE)
				{
					if (tipo_exp == TIPO_STRING)
					{
						int pos_pilha_ini_str = ts.Buscar_Pos_Pilha_Ini_Str(sidpai.make_Id_Pai(identificador, parente));
						int pos_pilha = ts.Buscar_Pos_Pilha(sidpai.make_Id_Pai(identificador, parente));
						if (ts.Verificar_Ponteiro(identificador))
							mepa.CRVI_String(to_string(pai), pos_pilha_ini_str, pos_pilha - pos_pilha_ini_str);
						else
							mepa.CRVL_String(to_string(pai), pos_pilha_ini_str, pos_pilha);
					}
					else
					{
						if (ts.Verificar_Ponteiro(identificador))
							mepa.CRVI(to_string(pai), to_string(ts.Buscar_Pos_Pilha(sidpai.make_Id_Pai(identificador, parente))));
						else
							mepa.CRVL(to_string(pai), to_string(ts.Buscar_Pos_Pilha(sidpai.make_Id_Pai(identificador, parente))));
						mepa.IMPR();
					}
				}
				else
				{
					if (tipo_exp == TIPO_STRING)
					{
						//int pos_pilha_ini_str = ts.Buscar_Pos_Pilha_Ini_Str(sidpai.make_Id_Pai(identificador, parente));
						int pos_pilha_ini = ts.Buscar_Pos_Pilha_Ini_Str(sidpai.make_Id_Pai(identificador, parente)) + 4 * (-1);
						int pos_pilha = ts.Buscar_Pos_Pilha(sidpai.make_Id_Pai(identificador, parente));
						mepa.CRVI_String(to_string(pai), pos_pilha_ini, pos_pilha_ini - pos_pilha);
					}
					else
					{
						mepa.CRVI(to_string(pai), to_string((ts.Buscar_Pos_Pilha(sidpai.make_Id_Pai(identificador, parente)) + 4)*(-1)));
						mepa.IMPR();
					}
				}
				//MEPA - IMPE Adiciono enter somente se for PRINTLN, PRINT imprime na mesma linha
				if (mepa.pilha_Com_Escrita.top() == PRINTLN)
					mepa.IMPE();
				mepa.pilha_Com_Escrita.pop();
				mepa.pilha_EXP.pop(); //Retiro a var que foi impressa 
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
		//sexpressao = { _simbolo.valor, false };
		if (eh_argumento)
			mepa.pilha_EXP_args.push(sexpressao.make_exp(_simbolo.valor, false, 0));
		else
			mepa.pilha_EXP.push(sexpressao.make_exp(_simbolo.valor, false, 0));
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
			else if (_simbolo.valor != "")
				val_string = _simbolo.valor;
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
		token == ENDERECO_ELEMENTO ||
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
		while (!pilha_Exp_args.empty())
		{
			//mepa.Avaliar_Expressao(mepa.pilha_EXP_args, ts);
			mepa.Avaliar_Expressao(pilha_Exp_args.top().pilha_EXP_args, pilha_Exp_args.top().ts_EXP_args);
			pilha_Exp_args.pop();
		}
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
	int parente = 0;
	if (!pilha_selecao.empty())
	{
		parente = ts.Buscar_Chave(pilha_selecao.top().idendificador);
		pilha_selecao.pop();
		while (!pilha_selecao.empty())
			pilha_selecao.pop();
	}
	else
		parente = ts.Buscar_Pai(identificador);
	tipo = ts.Buscar_Tipo(identificador);

	//Verifico se o acesso ao membro é permitido
	if (escopo != CLASSE && !ts.Consultar_Acesso(identificador, parente))
		Erro(identificador, ERR_SEM_NAO_ACESSIVEL_PRIVADO);

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
		if (ts.Verificar_Ponteiro(identificador))
			mepa.ARMI(to_string(ts.Buscar_Pos_Pilha(sidpai.make_Id_Pai(identificador, parente))));
		else
			mepa.ARMZ(to_string(ts.Buscar_Pos_Pilha(sidpai.make_Id_Pai(identificador, parente))));
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
	if (svar.tipo == TIPO_STRING)
	{
		svar.pos_pilha_ini_str = pilha_var_mem.top();
		//Por padrão, reservo 30 caracteres para uma String, define TAM_STRING
		pilha_var_mem.top() += TAM_STRING;
		var_mem += TAM_STRING;
	}
	else
	{
		pilha_var_mem.top()++;
		var_mem++; 
		svar.pos_pilha_ini_str = -1; //Quando não é String
	}




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

		if (escopo == CLASSE)
			svar.escopo = sclasse.identificador;
		else if (escopo == ESTRUTURA)
			svar.escopo = sstruct.identificador;
		//Se encontrei virgula, inserir variável na tabela de símbolo
		ts.Inserir(svar, escopo);
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
		ts.Inserir(svar, escopo);
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
	_simbolo.ponteiro = false;
	_simbolo.array = false;
	_simbolo.valido = false;
	_simbolo.valor = "";
	_simbolo.linha = 0;
	_simbolo.passby = "";
	_simbolo.classe = 0;
	_simbolo.pos_pilha = -1;
	_simbolo.rotulo = "";
	_simbolo.escopo = "";
	_simbolo.pai_heranca = -1;
}

void C_Analise_Sintatica::Inserir_AMEM_MEPA()
{
	//Se o escopo for estrutura ou classe, não reservo memória agora, apenas quando for declarado
	if (!escopo_classe && !escopo_estrutura)
	{
		//Caso seja tipo padrão do compilador, declaro normalmente
		if (Verificar_Tipo_Padrao(svar.tipo))
		{
			//Insiro a posição da pilha
			svar.pos_pilha = pilha_var_mem.top() - 1;
			//Incluo a VAR na TS
			ts.Inserir(svar, escopo);
			//MEPA - AMEM com a quantidade de variáveis declaradas
			mepa.AMEM(to_string(var_mem));
		}
		else
		{
			if (ts.Buscar_Categoria(svar.tipo) == ESTRUTURA || ts.Buscar_Categoria(svar.tipo) == CLASSE)
			{
				//Insiro a posição da pilha, -1 nesse caso por ser struct
				svar.pos_pilha = -1;
				pilha_var_mem.top()--;
				//Incluo a VAR na TS
				ts.Inserir(svar, escopo);
				//Inserir as variáveis de uma Struct na TS e MEPA
				Inserir_AMEM_MEPA_STRUCT(svar.tipo, svar.chave);
			}
				
		}
	}
	else
	{
		//Insiro a posição da pilha, -1 nesse caso por ser struct
		svar.pos_pilha = pilha_var_mem.top() -1;
		
		if (escopo == CLASSE)
			svar.escopo = sclasse.identificador;
		else if (escopo == ESTRUTURA)
			svar.escopo = sstruct.identificador;
		//Incluo a VAR na TS
		ts.Inserir(svar, escopo);

		
	}
}

void C_Analise_Sintatica::Inserir_DMEM_MEPA(int _mem)
{
	mepa.DMEM(_mem);
}

void C_Analise_Sintatica::Inserir_AMEM_MEPA_STRUCT(string _identificador, int _pai)
{
	//int l_pospilha = 0;
	S_Simbolos l_simb;
	int count = 0;
	int count_str = 0;

	vector<S_Simbolos> l_simbolos;
	l_simbolos = ts.Buscar_Membros_Classe(_identificador);

	for (auto it = l_simbolos.begin(); it != l_simbolos.end(); it++)
	{
		if (it->categoria == VAR)
		{
			l_simb = (*it);

			l_simb.chave = ++chave;
			l_simb.linha = iter_token_lexema->linha;
			l_simb.pai = _pai;
			l_simb.escopo = "";

			if (l_simb.tipo == TIPO_STRING)
			{
				int diff = (l_simb.pos_pilha - l_simb.pos_pilha_ini_str);
				count = diff + count;
				pilha_var_mem.top() = pilha_var_mem.top() + diff;
				l_simb.pos_pilha = pilha_var_mem.top();
				//pilha_var_mem.top() = l_svar.pos_pilha + 1;
				l_simb.pos_pilha_ini_str = pilha_var_mem.top() - diff;
				pilha_var_mem.top()++;
				count_str++;
			}

			else
			{
				count++;
				l_simb.pos_pilha = pilha_var_mem.top()++;
				l_simb.pos_pilha_ini_str = -1;
			}
			ts.Inserir(l_simb, escopo);
			if (!Verificar_Tipo_Padrao(l_simb.tipo))
			{
				Inserir_AMEM_MEPA_STRUCT(l_simb.tipo, l_simb.chave);
			}
		}
		else if (it->categoria == SUB || it->categoria == FUNCTION)
		{
			l_simb = (*it);

			l_simb.chave = ++chave;
			l_simb.linha = iter_token_lexema->linha;
			l_simb.pai = _pai;

			ts.Inserir(l_simb, escopo);
		}
	}
	mepa.AMEM(to_string(count + count_str));
	//mepa.AMEM(to_string(pilha_var_mem.top()));
}

bool C_Analise_Sintatica::Verificar_Tipo_Padrao(string _tipo)
{
	if (_tipo == TIPO_BOOLEANO ||
		_tipo == TIPO_CHAR ||
		_tipo == TIPO_FLOAT ||
		_tipo == TIPO_INT ||
		_tipo == TIPO_STRING ||
		_tipo == TIPO_VOID)
		return true;
	else
		return false;
}

void C_Analise_Sintatica::Remover_Mem_Classe()
{
	int l_total_class_mem;
	
	l_total_class_mem = ts.Retorna_Tot_Var_Mem_Classe();

	for (int i = 0; i < l_total_class_mem; i++)
		pilha_var_mem.top()--;
}

void C_Analise_Sintatica::Inserir_Membros_Classe_Pai(int _chave_pai, int _chave_classe)
{
	vector<S_Simbolos> l_simbolos;
	l_simbolos = ts.Buscar_Membros_Classe(_chave_pai);

	S_Simbolos l_simb;
	//int count = 0;
	int count_str = 0;

	for (auto it = l_simbolos.begin(); it != l_simbolos.end(); it++)
	{
		if (it->categoria == VAR)
		{
			l_simb = (*it);

			l_simb.chave = ++chave;
			l_simb.linha = iter_token_lexema->linha;
			l_simb.pai = _chave_classe;
			//l_simb.classe = _chave_classe;
			l_simb.escopo = "";

			/*if (l_simb.tipo == TIPO_STRING)
			{
				int diff = (l_simb.pos_pilha - l_simb.pos_pilha_ini_str + 1);
				count = diff + count;
				pilha_var_mem.top() = pilha_var_mem.top() + diff;
				l_simb.pos_pilha = pilha_var_mem.top();
				l_simb.pos_pilha_ini_str = pilha_var_mem.top() - diff;
				pilha_var_mem.top()++;
				count_str++;
			}

			else
			{
				count++;
				l_simb.pos_pilha = pilha_var_mem.top()++;
				l_simb.pos_pilha_ini_str = -1;
			}*/
			ts.Inserir(l_simb, escopo);
		}
		else if (it->categoria == SUB || it->categoria == FUNCTION)
		{
			l_simb = (*it);

			l_simb.chave = ++chave;
			l_simb.linha = iter_token_lexema->linha;
			l_simb.pai = _chave_classe;

			ts.Inserir(l_simb, escopo);
		}
	}
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