#include "C_Analise_Lexica.h"


C_Analise_Lexica::C_Analise_Lexica(string _conteudo)
{
	conteudo = _conteudo;
	numLinha = 1;
	//Carregar palavras reservadas
	Montar_Tabela_Palavras_Reservadas();
}


C_Analise_Lexica::~C_Analise_Lexica()
{
}

void C_Analise_Lexica::Iniciar()
{
	Proximo_Token();

	while (tabela_token_lexema.back().token != FIM_PROGRAMA)
		Proximo_Token();
}

void C_Analise_Lexica::Proximo_Token()
{
	Realizar_Analise_Lexica();
	while (resultado.first == FIM)
	{
		Realizar_Analise_Lexica();
		Incrementar_Linha();
	}

	//Carrego a struct com os valores
	token_lexema.lexema = resultado.first;
	token_lexema.token = resultado.second;
	token_lexema.linha = numLinha;
	//Insiro no vetor 
	tabela_token_lexema.push_back(token_lexema);
}

void C_Analise_Lexica::Montar_Tabela_Palavras_Reservadas()
{
	palavras_reservadas.clear();
	palavras_reservadas.push_back(make_pair("bool", TIPO_BOOLEANO));
	palavras_reservadas.push_back(make_pair("char", TIPO_CHAR));
	palavras_reservadas.push_back(make_pair("float", TIPO_FLOAT));
	palavras_reservadas.push_back(make_pair("int", TIPO_INT));
	palavras_reservadas.push_back(make_pair("void", TIPO_VOID));
	palavras_reservadas.push_back(make_pair("string", TIPO_STRING));
	palavras_reservadas.push_back(make_pair("do", LACO_DO));
	palavras_reservadas.push_back(make_pair("for", LACO_FOR));
	palavras_reservadas.push_back(make_pair("while", LACO_WHILE));
	palavras_reservadas.push_back(make_pair("break", LACO_BREAK));
	palavras_reservadas.push_back(make_pair("continue", LACO_CONTINUE));
	palavras_reservadas.push_back(make_pair("if", CONDICAO_IF));
	palavras_reservadas.push_back(make_pair("else", CONDICAO_ELSE));
	palavras_reservadas.push_back(make_pair("true", VERDADEIRO));
	palavras_reservadas.push_back(make_pair("false", FALSO));
	palavras_reservadas.push_back(make_pair("return", RETORNO));
	palavras_reservadas.push_back(make_pair("goto", GOTO));
	palavras_reservadas.push_back(make_pair("const", CONSTANTE));
	palavras_reservadas.push_back(make_pair("sub", SUB));
	palavras_reservadas.push_back(make_pair("function", FUNCTION));
	palavras_reservadas.push_back(make_pair("var", VAR));
	palavras_reservadas.push_back(make_pair("by", BY));
	palavras_reservadas.push_back(make_pair("value", VALUE));
	palavras_reservadas.push_back(make_pair("ref", REF));
	palavras_reservadas.push_back(make_pair("pointer", POINTER));
	palavras_reservadas.push_back(make_pair("print", PRINT));
	palavras_reservadas.push_back(make_pair("println", PRINTLN));
	palavras_reservadas.push_back(make_pair("repeat", LACO_REPEAT));
	palavras_reservadas.push_back(make_pair("scan", SCAN));
	palavras_reservadas.push_back(make_pair("scanln", SCANLN));
	palavras_reservadas.push_back(make_pair("then", CONDICAO_THEN));
	palavras_reservadas.push_back(make_pair("end_if", CONDICAO_ENDIF));
	palavras_reservadas.push_back(make_pair("end_sub", END_SUB));
	palavras_reservadas.push_back(make_pair("end_function", END_FUNCTION));
	palavras_reservadas.push_back(make_pair("to", LACO_TO));
	palavras_reservadas.push_back(make_pair("next", LACO_NEXT));
	palavras_reservadas.push_back(make_pair("until", LACO_UNTIL));
	palavras_reservadas.push_back(make_pair("loop", LACO_LOOP));
	palavras_reservadas.push_back(make_pair("not", OP_NEGACAO));
	palavras_reservadas.push_back(make_pair("or", OP_LOGICO_OU));
	palavras_reservadas.push_back(make_pair("div", OP_DIVISAO));
	palavras_reservadas.push_back(make_pair("mod", OP_MOD));
	palavras_reservadas.push_back(make_pair("and", OP_LOGICO_E));
	palavras_reservadas.push_back(make_pair("struct", ESTRUTURA));
	palavras_reservadas.push_back(make_pair("class", CLASSE));
	palavras_reservadas.push_back(make_pair("end_struct", END_STRUCT));
	palavras_reservadas.push_back(make_pair("end_class", END_CLASS));
	palavras_reservadas.push_back(make_pair("private", ACESS_PRIVATE));
	palavras_reservadas.push_back(make_pair("public", ACESS_PUBLIC));
	palavras_reservadas.push_back(make_pair("protected", ACESS_PROTECTED));
}

void C_Analise_Lexica::Realizar_Analise_Lexica()
{
	continua = true;
	estado = 1;
	lexema.clear();

	do
	{
		switch (estado)
		{
		case 1: Estado_01(); break;
		case 2: Estado_02(); break;
		case 3: Estado_03(); break;
		case 4: Estado_04(); break;
		case 5: Estado_05(); break;
		case 6: Estado_06(); break;
		case 7: Estado_07(); break;
		case 8: Estado_08(); break;
		case 9: Estado_09(); break;
		case 10: Estado_10(); break;
		case 11: Estado_11(); break;
		case 12: Estado_12(); break;
		case 13: Estado_13(); break;
		case 14: Estado_14(); break;
		case 15: Estado_15(); break;
		case 16: Estado_16(); break;
		case 17: Estado_17(); break;
		case 18: Estado_18(); break;
		case 19: Estado_19(); break;
		case 20: Estado_20(); break;
		default:
			break;
		}
	} while (continua);
}

char C_Analise_Lexica::ProximoChar()
{
	if (posicao._Ptr == nullptr)
		posicao = conteudo.begin();
	else if (posicao == conteudo.end())
		return FIM_LINHA;
	else
		posicao++;
	if (posicao == conteudo.end())
		return EOF;
	return *posicao;
}

void C_Analise_Lexica::Estado_01()
{
	ch = ProximoChar();

	if (Validar_Digito())
	{
		estado = 2;
		return;
	}

	if (Validar_Caractere())
	{
		estado = 17;
		return;
	}

	switch (ch)
	{
	case ESPACO: estado = 1; return;
	case TAB: estado = 1; return;
	case '\'': estado = 4; return;
	case '&': estado = 5; return;
	case '"': estado = 6; return;
	case '>': estado = 7; return;
	case '<': estado = 8; return;
	case '=': estado = 9; return;
	case '!': estado = 10; return;
	case '%': estado = 11; return;
	case '+': estado = 12; return;
	case '-': estado = 13; return;
	case '*': estado = 14; return;
	case '/': estado = 15; return;
	case '|': estado = 16; return;
	case ',': Set_Resultado(string(1, ch), VIRGULA); break;
	//case '.': Set_Resultado(string(1, ch), OP_SELECAO_IDENTIFICADOR); break;
	case ';': Set_Resultado(string(1, ch), PONTO_VIRGULA); break;
	case '(': Set_Resultado(string(1, ch), ABRE_PARENTESES); break;
	case ')': Set_Resultado(string(1, ch), FECHA_PARENTESES); break;
	case '[': Set_Resultado(string(1, ch), ABRE_COLCHETES); break;
	case ']': Set_Resultado(string(1, ch), FECHA_COLCHETES); break;
	case '{': Set_Resultado(string(1, ch), ABRE_CHAVES); break;
	case '}': Set_Resultado(string(1, ch), FECHA_CHAVES); break;
	case '?': Set_Resultado(string(1, ch), PONTO_INTERROGACAO); break;
	case ':': Set_Resultado(string(1, ch), DOIS_PONTOS); break;
	case FIM_LINHA: Set_Resultado(FIM, "FIM"); break;
	case EOF: Set_Resultado(FIM_PROGRAMA, FIM_PROGRAMA); break;
	default:
		ERRO("CARACTERE VALIDO", "este caracter nao e reconhecido pela linguagem Chimera: " + string(1, ch));
		break;
	}
	continua = false;
}

void C_Analise_Lexica::Estado_02()
{
	lexema.clear();

	while (Validar_Digito())
	{
		lexema += ch;
		ch = ProximoChar();
	}

	if (ch == '.')
	{
		estado = 3;
		lexema += ch;
		ch = ProximoChar();
	}
	else
	{
		if (ch == FIM_LINHA)
		{
			continua = false;
		}

		Voltar_Um();
		Set_Resultado(lexema, NUM_INT);
		continua = false;
	}
}

void C_Analise_Lexica::Estado_03()
{
	while (Validar_Digito())
	{
		lexema += ch;
		ch = ProximoChar();
	}


	if (ch == FIM_LINHA)
	{
		continua = false;
	}

	Voltar_Um();
	Set_Resultado(lexema, NUM_REAL);
	continua = false;
}

void C_Analise_Lexica::Estado_04()
{
	//concateno abertura das aspas simples
	lexema += ch;
	ch = ProximoChar();

	//Se for aspas simples logo após outra aspas simples, é um caso de caractere vazio
	if (ch == '\'')
	{
		lexema += ch;
		Set_Resultado(lexema, CARACTERE);
		continua = false;
		return;
	}

	//cancateno o caracter, pode ser qualquer coisa
	lexema += ch;
	ch = ProximoChar();
	//Se não for aspas simples, deve dar erro pois não se trata de um caractere
	if (ch != '\'' && lexema != "\'\'")
	{
		ERRO("\'", "caractere invalido: " + string(1, ch));
		//Parar tudo neste erro
	}

	//concateno fechamento das aspas simples, caso não seja caractere vazio
	if (lexema != "\'\'")
		lexema += ch;

	Set_Resultado(lexema, CARACTERE);
	continua = false;
}

void C_Analise_Lexica::Estado_05()
{
	lexema += ch;

	ch = ProximoChar();

	if (ch == '&')
	{
		lexema += ch;
		Set_Resultado(lexema, OP_LOGICO_E);
		continua = false;
		return;
	}

	Voltar_Um();
	Set_Resultado(lexema, ENDERECO_ELEMENTO);
	continua = false;
}

void C_Analise_Lexica::Estado_06()
{
	lexema += ch;
	ch = ProximoChar();

	while (ch != '"')
	{
		lexema += ch;
		ch = ProximoChar();
		if (ch == FIM_LINHA)
		{
			ERRO("\"", "String incompleta");
		}
	}

	lexema += ch;
	Set_Resultado(lexema, STRING);
	continua = false;
}

void C_Analise_Lexica::Estado_07()
{
	lexema += ch;

	ch = ProximoChar();

	if (ch == '=')
	{
		lexema += ch;
		Set_Resultado(lexema, OP_MAIOR_IGUAL);
		continua = false;
		return;
	}

	Voltar_Um();
	Set_Resultado(lexema, OP_MAIOR);
	continua = false;
}

void C_Analise_Lexica::Estado_08()
{
	lexema += ch;

	ch = ProximoChar();

	if (ch == '=')
	{
		lexema += ch;
		Set_Resultado(lexema, OP_MENOR_IGUAL);
		continua = false;
		return;
	}

	if (ch == '>')
	{
		lexema += ch;
		Set_Resultado(lexema, OP_DIFERENTE);
		continua = false;
		return;
	}

	Voltar_Um();
	Set_Resultado(lexema, OP_MENOR);
	continua = false;
}

void C_Analise_Lexica::Estado_09()
{
	lexema += ch;

	ch = ProximoChar();

	if (ch == '=')
	{
		lexema += ch;
		Set_Resultado(lexema, OP_IGUALDADE);
		continua = false;
		return;
	}

	Voltar_Um();
	Set_Resultado(lexema, OP_ATRIBUICAO);
	continua = false;
}

void C_Analise_Lexica::Estado_10()
{
	lexema += ch;

	ch = ProximoChar();

	if (ch == '=')
	{
		lexema += ch;
		Set_Resultado(lexema, OP_NEGA_IGUALDADE);
		continua = false;
		return;
	}

	Voltar_Um();
	Set_Resultado(lexema, OP_NEGACAO);
	continua = false;
}

void C_Analise_Lexica::Estado_11()
{
	lexema += ch;

	ch = ProximoChar();

	if (ch == '=')
	{
		lexema += ch;
		Set_Resultado(lexema, OP_ATRIBUICAO_RESTO);
		continua = false;
		return;
	}

	Voltar_Um();
	Set_Resultado(lexema, OP_RESTO);
	continua = false;
}

void C_Analise_Lexica::Estado_12()
{
	lexema += ch;

	ch = ProximoChar();

	if (ch == '=')
	{
		lexema += ch;
		Set_Resultado(lexema, OP_ATRIBUICAO_ADICAO);
		continua = false;
		return;
	}

	if (ch == '+')
	{
		lexema += ch;
		Set_Resultado(lexema, OP_INCREMENTO);
		continua = false;
		return;
	}

	Voltar_Um();
	Set_Resultado(lexema, OP_ADICAO);
	continua = false;
}

void C_Analise_Lexica::Estado_13()
{
	lexema += ch;

	ch = ProximoChar();

	if (ch == '=')
	{
		lexema += ch;
		Set_Resultado(lexema, OP_ATRIBUICAO_SUBTRACAO);
		continua = false;
		return;
	}

	if (ch == '-')
	{
		lexema += ch;
		Set_Resultado(lexema, OP_DECREMENTO);
		continua = false;
		return;
	}

	/*if (ch == '>')
	{
		lexema += ch;
		Set_Resultado(lexema, OP_SELECAO_PONTEIRO);
		continua = false;
		return;
	}*/

	Voltar_Um();
	Set_Resultado(lexema, OP_SUBTRACAO);
	continua = false;
}

void C_Analise_Lexica::Estado_14()
{
	lexema += ch;

	ch = ProximoChar();

	if (ch == '=')
	{
		lexema += ch;
		Set_Resultado(lexema, OP_ATRIBUICAO_MULTIPLICACAO);
		continua = false;
		return;
	}

	Voltar_Um();
	Set_Resultado(lexema, OP_MULTIPLICACAO);
	continua = false;
}

void C_Analise_Lexica::Estado_15()
{
	lexema += ch;

	ch = ProximoChar();

	if (ch == '=')
	{
		lexema += ch;
		Set_Resultado(lexema, OP_ATRIBUICAO_DIVISAO);
		continua = false;
		return;
	}

	if (ch == '/')
	{
		//Início de comentário de linha
		estado = 18;
		return;
	}

	if (ch == '*')
	{
		//Início de comentário multiplas linhas
		estado = 19;
		return;
	}

	Voltar_Um();
	Set_Resultado(lexema, OP_DIVISAO);
	continua = false;
}

void C_Analise_Lexica::Estado_16()
{
	lexema += ch;

	ch = ProximoChar();

	if (ch != '|')
	{
		ERRO("|", "caractere invalido: " + string(1, ch));
	}

	lexema += ch;
	Set_Resultado(lexema, OP_LOGICO_OU);
	continua = false;
}

void C_Analise_Lexica::Estado_17()
{
	string token_palavra_reservada;
	while (Validar_Caractere())
	{
		lexema += ch;
		ch = ProximoChar();
	}

	//verificar se é objeto. ou objeto->
	if (ch == '.')
	{
		lexema += ch;
		Set_Resultado(lexema, ID_SEL_IDENTIFICADOR);
		continua = false;
	}
	else if (ch == '-')
	{
		char temp_ch = ch;
		ch = ProximoChar();
		if (ch != '>')
		{
			Voltar_Um();
			if (Validar_Identificador())
			{
				token_palavra_reservada = Buscar_Palavra_Reservada();
				if (token_palavra_reservada == "")
					Set_Resultado(lexema, IDENTIFICADOR);
				else
					Set_Resultado(lexema, token_palavra_reservada);

				continua = false;
				Voltar_Um();
				return;
			}
			else
				ERRO("Identificador: " + lexema + string(1, ch) + " - Caractere invalido: " + string(1, ch));
		}
		lexema += temp_ch;
		lexema += ch;
		Set_Resultado(lexema, ID_SEL_PONTEIRO);
		continua = false;
	}
	else if (Validar_Identificador())
	{
		token_palavra_reservada = Buscar_Palavra_Reservada();
		if (token_palavra_reservada == "")
			Set_Resultado(lexema, IDENTIFICADOR);
		else
			Set_Resultado(lexema, token_palavra_reservada);
		
		continua = false;
		Voltar_Um();
	}
	else
	{
		ERRO("Identificador: " + lexema + string(1, ch) + " - Caractere invalido: " + string(1, ch));
	}
}

void C_Analise_Lexica::Estado_18()
{
	//Comentário não gera token, assim que acabar a linha retorno TOKEN de FIM
	while (ch != FIM_LINHA && ch != EOF)
	{
		ch = ProximoChar();
	}

	if (ch == FIM_LINHA)
		Set_Resultado(FIM, "FIM");
	else
		Set_Resultado(FIM_PROGRAMA, FIM_PROGRAMA);
	continua = false;
}

void C_Analise_Lexica::Estado_19()
{
	ch = ProximoChar();
	while (ch != '*')
	{
		if (ch == FIM_LINHA)
			Incrementar_Linha();
		ch = ProximoChar();
	}

	//Se for * vou para o próximo estado verificar se o próximo caractere é / para finalizar o comentário, caso não seja, volto para o estado 19
	estado = 20;
}

void C_Analise_Lexica::Estado_20()
{
	//Se for / acabou o comentário, caso contrário volto ao estado 19
	ch = ProximoChar();
	if (ch == '/')
	{
		estado = 1;
		//Limpar a variável lexema pois comentário não gera token
		lexema.clear();
	}
	else if (ch == '*')
		estado = 20; //Se for * fico verificando se acho o / para finalizar o comentário
	else
		estado = 19;
}

bool C_Analise_Lexica::Validar_Digito()
{
	if (regex_match(string(1, ch), regex("[0-9]")))
		return true;
	else
		return false;
}

bool C_Analise_Lexica::Validar_Caractere()
{
	if (regex_match(string(1, ch), regex("[a-zA-Z0-9_]*")))
		return true;
	else
		return false;
}

bool C_Analise_Lexica::Validar_Identificador()
{
	if (regex_match(lexema, regex("(_)*[a-zA-Z][a-zA-Z0-9_]*")))
		return true;
	else
		return false;
}

string C_Analise_Lexica::Buscar_Palavra_Reservada()
{
	for (auto it = palavras_reservadas.begin(); it != palavras_reservadas.end(); it++)
	{
		if (lexema == it->first)
			return it->second;
	}
	return "";
}

void C_Analise_Lexica::Voltar_Um()
{
	posicao--;
}

void C_Analise_Lexica::Set_Resultado(string _lexema, string _token)
{
	resultado.first = _lexema;
	resultado.second = _token;
}

void C_Analise_Lexica::ERRO(string _esperado, string _mensagem)
{
	cout << "ERRO: Linha " << numLinha << ", esperado " << _esperado << ", " << _mensagem << endl;
	system("pause");
	exit(EXIT_FAILURE);
}

void C_Analise_Lexica::ERRO(string _mensagem)
{
	cout << "ERRO: Linha " << numLinha << ", " << _mensagem << endl;
	system("pause");
	exit(EXIT_FAILURE);
}

bool C_Analise_Lexica::Voltar_Estado_01()
{
	switch (ch)
	{
	case ' ': estado = 1; return true;
	case TAB: estado = 1; return true;
	default: return false;
		break;
	}
}

void C_Analise_Lexica::Incrementar_Linha()
{
	numLinha++;
}

vector<S_Token_Lexema> C_Analise_Lexica::Get_Tabela_Token_Lexema()
{
	return tabela_token_lexema;
}

void C_Analise_Lexica::Gravar_Tabela_Token(string _nome_arquivo)
{
	fstream arquivo;
	stringstream ss;
	string nome_arquivo_final;

	ss << "sep=;" << endl;
	ss << "LEXEMA;TOKEN;LINHA" << endl;
	for (auto it = tabela_token_lexema.begin(); it != tabela_token_lexema.end(); it++)
	{
		if (it->lexema == ";")
			ss << "\"" << it->lexema << "\"" << ";" << it->token << ";" << it->linha << endl;
		else
			ss << it->lexema << ";" << it->token << ";" << it->linha << endl;
	}

	//Remover a extensão .chi do arquivo
	nome_arquivo_final = _nome_arquivo.substr(0, _nome_arquivo.size() - 4);

	//Adicionar informações ao nome do arquivo e extensão .csv
	nome_arquivo_final = nome_arquivo_final + "_TokLex.csv";

	arquivo.open(nome_arquivo_final, ios::out | ios::trunc);
	arquivo << ss.str();
	arquivo.close();

	cout << "Arquivo \"" << nome_arquivo_final << "\" gerado" << endl;
}
