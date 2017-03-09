#include "TU_Analise_Lexica.h"



TU_Analise_Lexica::TU_Analise_Lexica()
{
}


TU_Analise_Lexica::~TU_Analise_Lexica()
{
}

void TU_Analise_Lexica::Realizar_Testes(string _expressao, string _testeNum)
{
	C_Analise_Lexica lexica(_expressao);
	
	bool testeOk = true;

	lexica.Iniciar();

	tabelaResultado = lexica.Get_Tabela_Token_Lexema();

	auto it_real = tabelaResultado.begin();
	auto it_esperado = this->tabelaTeste.begin();

	for (; it_real != tabelaResultado.end() && it_esperado != this->tabelaTeste.end(); it_real++, it_esperado++)
	{
		if (it_real->lexema != it_esperado->first && it_real->token != it_esperado->second)
		{
			cout << _testeNum << " - Erro no token: " << it_real->lexema << " - " << it_real->token
				<< " - ESPERADO: " << it_esperado->first << " - " << it_esperado->second << endl;
			testeOk = false;
		}
	}
	if (testeOk)
		cout << _testeNum << " - OK" << endl;
}

void TU_Analise_Lexica::Executar()
{
	cout << SEPARADOR_HASHTAG << endl;
	cout << "Iniciando testes Lexicos" << endl;
	cout << SEPARADOR_HASHTAG << endl;
	Teste_001();
	Teste_002();
	Teste_003();
	Teste_004();
	Teste_005();
	Teste_006();
	Teste_007();
	Teste_008();
	Teste_009();
	Teste_010();
	Teste_011();
	Teste_012();
	Teste_013();
	Teste_014();
	Teste_015();
	Teste_016();
	Teste_017();
	Teste_018();
	Teste_019();
	Teste_020();
	Teste_021();
	Teste_022();
	Teste_023();
	Teste_024();
	Teste_025();
	Teste_026();
	Teste_027();
	Teste_028();
	Teste_029();
	Teste_030();
	Teste_031();
	Teste_032();
	Teste_033();
	Teste_034();
	Teste_035();
	Teste_036();
	Teste_037();
	Teste_038();
	Teste_039();
	Teste_040();
	Teste_041();
	Teste_042();
	Teste_043();
	Teste_044();
	Teste_045();
	Teste_046();
	Teste_047();
	Teste_048();
	Teste_049();
	Teste_050();
	Teste_051();
	Teste_052();
	Teste_053();
	Teste_054();
	Teste_055();
	Teste_056();
	Teste_057();
	Teste_058();
	Teste_059();
}


//Teste Digitos e sinal de adição
void TU_Analise_Lexica::Teste_001()
{
	string expressao = "587+999";

	this->tabelaTeste.clear();
	this->tabelaTeste.push_back(make_pair("587", NUM_INT));
	this->tabelaTeste.push_back(make_pair("+", OP_ADICAO));
	this->tabelaTeste.push_back(make_pair("999", NUM_INT));
	//this->tabelaTeste.push_back(make_pair("FIM", FIM));

	Realizar_Testes(expressao, "TESTE_01");
}

//Teste Digitos e operador de adição com espaços e tabs
void TU_Analise_Lexica::Teste_002()
{
	string expressao = "	587 +		999 ";

	this->tabelaTeste.clear();
	this->tabelaTeste.push_back(make_pair("587", NUM_INT));
	this->tabelaTeste.push_back(make_pair("+", OP_ADICAO));
	this->tabelaTeste.push_back(make_pair("999", NUM_INT));
	//this->tabelaTeste.push_back(make_pair("FIM", FIM));

	Realizar_Testes(expressao, "TESTE_02");
}

//Teste Digitos e divesos operadores com espaços e tabs
void TU_Analise_Lexica::Teste_003()
{
	string expressao = "	587 -		4487874 / 557 * 123 + 9 % 11 -=";

	this->tabelaTeste.clear();
	this->tabelaTeste.push_back(make_pair("587", NUM_INT));
	this->tabelaTeste.push_back(make_pair("-", OP_SUBTRACAO));
	this->tabelaTeste.push_back(make_pair("4487874", NUM_INT));
	this->tabelaTeste.push_back(make_pair("/", OP_DIVISAO));
	this->tabelaTeste.push_back(make_pair("557", NUM_INT));
	this->tabelaTeste.push_back(make_pair("*", OP_MULTIPLICACAO));
	this->tabelaTeste.push_back(make_pair("123", NUM_INT));
	this->tabelaTeste.push_back(make_pair("+", OP_ADICAO));
	this->tabelaTeste.push_back(make_pair("9", NUM_INT));
	this->tabelaTeste.push_back(make_pair("%", OP_RESTO));
	this->tabelaTeste.push_back(make_pair("11", NUM_INT));
	this->tabelaTeste.push_back(make_pair("-=", OP_ATRIBUICAO_SUBTRACAO));
	//this->tabelaTeste.push_back(make_pair("FIM", FIM));

	Realizar_Testes(expressao, "TESTE_03");
}

//Teste Numeros reais e sinal de adição
void TU_Analise_Lexica::Teste_004()
{
	string expressao = "5.87+9.12";

	this->tabelaTeste.clear();
	this->tabelaTeste.push_back(make_pair("5.87", NUM_REAL));
	this->tabelaTeste.push_back(make_pair("+", OP_ADICAO));
	this->tabelaTeste.push_back(make_pair("9.12", NUM_REAL));
	//this->tabelaTeste.push_back(make_pair("FIM", FIM));

	Realizar_Testes(expressao, "TESTE_04");
}

//Teste Numeros reais e sinal de adição com espaços e tabs
void TU_Analise_Lexica::Teste_005()
{
	string expressao = "	5.87 +	9.12 ";

	this->tabelaTeste.clear();
	this->tabelaTeste.push_back(make_pair("5.87", NUM_REAL));
	this->tabelaTeste.push_back(make_pair("+", OP_ADICAO));
	this->tabelaTeste.push_back(make_pair("9.12", NUM_REAL));
	//this->tabelaTeste.push_back(make_pair("FIM", FIM));

	Realizar_Testes(expressao, "TESTE_05");
}

//Teste de caractere
void TU_Analise_Lexica::Teste_006()
{
	string expressao = "'a'";

	this->tabelaTeste.clear();
	this->tabelaTeste.push_back(make_pair("'a'", CARACTERE));
	//this->tabelaTeste.push_back(make_pair("FIM", FIM));

	Realizar_Testes(expressao, "TESTE_06");
}

//Teste de caractere vazio
void TU_Analise_Lexica::Teste_007()
{
	string expressao = "''";

	this->tabelaTeste.clear();
	this->tabelaTeste.push_back(make_pair("''", CARACTERE));
	//this->tabelaTeste.push_back(make_pair("FIM", FIM));

	Realizar_Testes(expressao, "TESTE_07");
}

//Teste de string
void TU_Analise_Lexica::Teste_008()
{
	string expressao = "\"abcdefghijklmnopqrstuvzx\"";

	this->tabelaTeste.clear();
	this->tabelaTeste.push_back(make_pair("\"abcdefghijklmnopqrstuvzx\"", STRING));
	//this->tabelaTeste.push_back(make_pair("FIM", FIM));

	Realizar_Testes(expressao, "TESTE_08");
}

//Teste de string vazia
void TU_Analise_Lexica::Teste_009()
{
	string expressao = "\"\"";

	this->tabelaTeste.clear();
	this->tabelaTeste.push_back(make_pair("\"\"", STRING));
	//this->tabelaTeste.push_back(make_pair("FIM", FIM));

	Realizar_Testes(expressao, "TESTE_09");
}

//Teste operador maior
void TU_Analise_Lexica::Teste_010()
{
	string expressao = ">";

	this->tabelaTeste.clear();
	this->tabelaTeste.push_back(make_pair(">", OP_MAIOR));
	//this->tabelaTeste.push_back(make_pair("FIM", FIM));

	Realizar_Testes(expressao, "TESTE_10");
}

//Teste operador menor
void TU_Analise_Lexica::Teste_011()
{
	string expressao = "<";

	this->tabelaTeste.clear();
	this->tabelaTeste.push_back(make_pair("<", OP_MENOR));
	//this->tabelaTeste.push_back(make_pair("FIM", FIM));

	Realizar_Testes(expressao, "TESTE_11");
}

//Teste operador maior igual
void TU_Analise_Lexica::Teste_012()
{
	string expressao = ">=";

	this->tabelaTeste.clear();
	this->tabelaTeste.push_back(make_pair(">=", OP_MAIOR_IGUAL));
	//this->tabelaTeste.push_back(make_pair("FIM", FIM));

	Realizar_Testes(expressao, "TESTE_12");
}

//Teste operador maior
void TU_Analise_Lexica::Teste_013()
{
	string expressao = " <=		";

	this->tabelaTeste.clear();
	this->tabelaTeste.push_back(make_pair("<=", OP_MENOR_IGUAL));
	//this->tabelaTeste.push_back(make_pair("FIM", FIM));

	Realizar_Testes(expressao, "TESTE_13");
}

//Teste operador atribuicao
void TU_Analise_Lexica::Teste_014()
{
	string expressao = "=";

	this->tabelaTeste.clear();
	this->tabelaTeste.push_back(make_pair("=", OP_ATRIBUICAO));
	//this->tabelaTeste.push_back(make_pair("FIM", FIM));

	Realizar_Testes(expressao, "TESTE_14");
}

//Teste operador igualdade
void TU_Analise_Lexica::Teste_015()
{
	string expressao = "==";

	this->tabelaTeste.clear();
	this->tabelaTeste.push_back(make_pair("==", OP_IGUALDADE));
	//this->tabelaTeste.push_back(make_pair("FIM", FIM));

	Realizar_Testes(expressao, "TESTE_15");
}

//Teste operador negação
void TU_Analise_Lexica::Teste_016()
{
	string expressao = "!";

	this->tabelaTeste.clear();
	this->tabelaTeste.push_back(make_pair("!", OP_NEGACAO));
	//this->tabelaTeste.push_back(make_pair("FIM", FIM));

	Realizar_Testes(expressao, "TESTE_16");
}

//Teste operador diferente !=
void TU_Analise_Lexica::Teste_017()
{
	string expressao = "!=";

	this->tabelaTeste.clear();
	this->tabelaTeste.push_back(make_pair("!=", OP_NEGA_IGUALDADE));
	//this->tabelaTeste.push_back(make_pair("FIM", FIM));

	Realizar_Testes(expressao, "TESTE_17");
}

//Teste operador resto %
void TU_Analise_Lexica::Teste_018()
{
	string expressao = "%";

	this->tabelaTeste.clear();
	this->tabelaTeste.push_back(make_pair("%", OP_RESTO));
	//this->tabelaTeste.push_back(make_pair("FIM", FIM));

	Realizar_Testes(expressao, "TESTE_18");
}

//Teste operador atribuição adição
void TU_Analise_Lexica::Teste_019()
{
	string expressao = "+=";

	this->tabelaTeste.clear();
	this->tabelaTeste.push_back(make_pair("+=", OP_ATRIBUICAO_ADICAO));
	//this->tabelaTeste.push_back(make_pair("FIM", FIM));

	Realizar_Testes(expressao, "TESTE_19");
}

//Teste operador atribuição subtracao
void TU_Analise_Lexica::Teste_020()
{
	string expressao = "-=";

	this->tabelaTeste.clear();
	this->tabelaTeste.push_back(make_pair("-=", OP_ATRIBUICAO_SUBTRACAO));
	//this->tabelaTeste.push_back(make_pair("FIM", FIM));

	Realizar_Testes(expressao, "TESTE_20");
}

//Teste operador atribuição multiplicacao
void TU_Analise_Lexica::Teste_021()
{
	string expressao = "*=";

	this->tabelaTeste.clear();
	this->tabelaTeste.push_back(make_pair("*=", OP_ATRIBUICAO_MULTIPLICACAO));
	//this->tabelaTeste.push_back(make_pair("FIM", FIM));

	Realizar_Testes(expressao, "TESTE_21");
}

//Teste operador atribuição Divisao
void TU_Analise_Lexica::Teste_022()
{
	string expressao = "/=";

	this->tabelaTeste.clear();
	this->tabelaTeste.push_back(make_pair("/=", OP_ATRIBUICAO_DIVISAO));
	//this->tabelaTeste.push_back(make_pair("FIM", FIM));

	Realizar_Testes(expressao, "TESTE_22");
}

//Teste operador atribuição Resto
void TU_Analise_Lexica::Teste_023()
{
	string expressao = "%=";

	this->tabelaTeste.clear();
	this->tabelaTeste.push_back(make_pair("%=", OP_ATRIBUICAO_RESTO));
	//this->tabelaTeste.push_back(make_pair("FIM", FIM));

	Realizar_Testes(expressao, "TESTE_23");
}

//Teste Endereço Elemento
void TU_Analise_Lexica::Teste_024()
{
	string expressao = "&";

	this->tabelaTeste.clear();
	this->tabelaTeste.push_back(make_pair("&", ENDERECO_ELEMENTO));
	//this->tabelaTeste.push_back(make_pair("FIM", FIM));

	Realizar_Testes(expressao, "TESTE_24");
}

//Teste operador Lógico E
void TU_Analise_Lexica::Teste_025()
{
	string expressao = "&&";

	this->tabelaTeste.clear();
	this->tabelaTeste.push_back(make_pair("&&", OP_LOGICO_E));
	//this->tabelaTeste.push_back(make_pair("FIM", FIM));

	Realizar_Testes(expressao, "TESTE_25");
}

//Teste operador incremento
void TU_Analise_Lexica::Teste_026()
{
	string expressao = "++";

	this->tabelaTeste.clear();
	this->tabelaTeste.push_back(make_pair("++", OP_INCREMENTO));
	//this->tabelaTeste.push_back(make_pair("FIM", FIM));

	Realizar_Testes(expressao, "TESTE_26");
}

//Teste operador decremento
void TU_Analise_Lexica::Teste_027()
{
	string expressao = "--";

	this->tabelaTeste.clear();
	this->tabelaTeste.push_back(make_pair("--", OP_DECREMENTO));
	//this->tabelaTeste.push_back(make_pair("FIM", FIM));

	Realizar_Testes(expressao, "TESTE_27");
}

//Teste operador de selecao de elemento por identificador 
void TU_Analise_Lexica::Teste_028()
{
	string expressao = "objeto.propriedade";

	this->tabelaTeste.clear();
	this->tabelaTeste.push_back(make_pair("objeto", IDENTIFICADOR));
	this->tabelaTeste.push_back(make_pair(".", OP_SELECAO_IDENTIFICADOR));
	this->tabelaTeste.push_back(make_pair("propriedade", IDENTIFICADOR));
	//this->tabelaTeste.push_back(make_pair("FIM", FIM));

	Realizar_Testes(expressao, "TESTE_28");
}

//Teste operador de selecao de elemento por ponteiro 
void TU_Analise_Lexica::Teste_029()
{
	string expressao = " ->		";

	this->tabelaTeste.clear();
	this->tabelaTeste.push_back(make_pair("->", OP_SELECAO_PONTEIRO));
	//this->tabelaTeste.push_back(make_pair("FIM", FIM));

	Realizar_Testes(expressao, "TESTE_29");
}

//Teste operador lógico ou
void TU_Analise_Lexica::Teste_030()
{
	string expressao = "||";

	this->tabelaTeste.clear();
	this->tabelaTeste.push_back(make_pair("||", OP_LOGICO_OU));
	//this->tabelaTeste.push_back(make_pair("FIM", FIM));

	Realizar_Testes(expressao, "TESTE_30");
}

//Teste diverso operadores
void TU_Analise_Lexica::Teste_031()
{
	string expressao = "\"TU_Analise_LexicaTRING\"+\'C\'->555&.99.99.";

	this->tabelaTeste.clear();
	this->tabelaTeste.push_back(make_pair("\"TU_Analise_LexicaTRING\"", STRING));
	this->tabelaTeste.push_back(make_pair("+", OP_ADICAO));
	this->tabelaTeste.push_back(make_pair("\'C\'", CARACTERE));
	this->tabelaTeste.push_back(make_pair("->", OP_SELECAO_PONTEIRO));
	this->tabelaTeste.push_back(make_pair("555", NUM_INT));
	this->tabelaTeste.push_back(make_pair("&", ENDERECO_ELEMENTO));
	this->tabelaTeste.push_back(make_pair(".", OP_SELECAO_IDENTIFICADOR));
	this->tabelaTeste.push_back(make_pair("99.99", NUM_REAL));
	this->tabelaTeste.push_back(make_pair(".", OP_SELECAO_IDENTIFICADOR));
	//this->tabelaTeste.push_back(make_pair("FIM", FIM));

	Realizar_Testes(expressao, "TESTE_31");
}

//Teste de valor do tipo booleano
void TU_Analise_Lexica::Teste_032()
{
	string expressao = " true false	";

	this->tabelaTeste.clear();
	this->tabelaTeste.push_back(make_pair("true", VERDADEIRO));
	this->tabelaTeste.push_back(make_pair("false", FALSO));
	//this->tabelaTeste.push_back(make_pair("FIM", FIM));

	Realizar_Testes(expressao, "TESTE_32");
}

//Teste tipo booleano
void TU_Analise_Lexica::Teste_033()
{
	string expressao = "bool";

	this->tabelaTeste.clear();
	this->tabelaTeste.push_back(make_pair("bool", TIPO_BOOLEANO));
	//this->tabelaTeste.push_back(make_pair("FIM", FIM));

	Realizar_Testes(expressao, "TESTE_33");
}

//Teste tipo char
void TU_Analise_Lexica::Teste_034()
{
	string expressao = "char";

	this->tabelaTeste.clear();
	this->tabelaTeste.push_back(make_pair("char", TIPO_CHAR));
	//this->tabelaTeste.push_back(make_pair("FIM", FIM));

	Realizar_Testes(expressao, "TESTE_34");
}

//Teste tipo float
void TU_Analise_Lexica::Teste_035()
{
	string expressao = "float";

	this->tabelaTeste.clear();
	this->tabelaTeste.push_back(make_pair("float", TIPO_FLOAT));
	//this->tabelaTeste.push_back(make_pair("FIM", FIM));

	Realizar_Testes(expressao, "TESTE_35");
}

//Teste tipo INT
void TU_Analise_Lexica::Teste_036()
{
	string expressao = "int";

	this->tabelaTeste.clear();
	this->tabelaTeste.push_back(make_pair("int", TIPO_INT));
	//this->tabelaTeste.push_back(make_pair("FIM", FIM));

	Realizar_Testes(expressao, "TESTE_36");
}

//Teste tipo void
void TU_Analise_Lexica::Teste_037()
{
	string expressao = "void";

	this->tabelaTeste.clear();
	this->tabelaTeste.push_back(make_pair("void", TIPO_VOID));
	//this->tabelaTeste.push_back(make_pair("FIM", FIM));

	Realizar_Testes(expressao, "TESTE_37");
}

//Teste laço do
void TU_Analise_Lexica::Teste_038()
{
	string expressao = "do";

	this->tabelaTeste.clear();
	this->tabelaTeste.push_back(make_pair("do", LACO_DO));
	//this->tabelaTeste.push_back(make_pair("FIM", FIM));

	Realizar_Testes(expressao, "TESTE_38");
}

//Teste laço for
void TU_Analise_Lexica::Teste_039()
{
	string expressao = "for";

	this->tabelaTeste.clear();
	this->tabelaTeste.push_back(make_pair("for", LACO_FOR));
	//this->tabelaTeste.push_back(make_pair("FIM", FIM));

	Realizar_Testes(expressao, "TESTE_39");
}

//Teste laço while
void TU_Analise_Lexica::Teste_040()
{
	string expressao = "while";

	this->tabelaTeste.clear();
	this->tabelaTeste.push_back(make_pair("while", LACO_WHILE));
	//this->tabelaTeste.push_back(make_pair("FIM", FIM));

	Realizar_Testes(expressao, "TESTE_40");
}

//Teste laço break
void TU_Analise_Lexica::Teste_041()
{
	string expressao = "break";

	this->tabelaTeste.clear();
	this->tabelaTeste.push_back(make_pair("break", LACO_BREAK));
	//this->tabelaTeste.push_back(make_pair("FIM", FIM));

	Realizar_Testes(expressao, "TESTE_41");
}

//Teste laço continue
void TU_Analise_Lexica::Teste_042()
{
	string expressao = "continue";

	this->tabelaTeste.clear();
	this->tabelaTeste.push_back(make_pair("continue", LACO_CONTINUE));
	//this->tabelaTeste.push_back(make_pair("FIM", FIM));

	Realizar_Testes(expressao, "TESTE_42");
}

//Teste condição IF
void TU_Analise_Lexica::Teste_043()
{
	string expressao = "if";

	this->tabelaTeste.clear();
	this->tabelaTeste.push_back(make_pair("if", CONDICAO_IF));
	//this->tabelaTeste.push_back(make_pair("FIM", FIM));

	Realizar_Testes(expressao, "TESTE_43");
}

//Teste condição ELSE
void TU_Analise_Lexica::Teste_044()
{
	string expressao = "else";

	this->tabelaTeste.clear();
	this->tabelaTeste.push_back(make_pair("else", CONDICAO_ELSE));
	//this->tabelaTeste.push_back(make_pair("FIM", FIM));

	Realizar_Testes(expressao, "TESTE_44");
}

//Teste return
void TU_Analise_Lexica::Teste_045()
{
	string expressao = "return";

	this->tabelaTeste.clear();
	this->tabelaTeste.push_back(make_pair("return", RETORNO));
	//this->tabelaTeste.push_back(make_pair("FIM", FIM));

	Realizar_Testes(expressao, "TESTE_45");
}

//Teste goto
void TU_Analise_Lexica::Teste_046()
{
	string expressao = "goto";

	this->tabelaTeste.clear();
	this->tabelaTeste.push_back(make_pair("goto", GOTO));
	//this->tabelaTeste.push_back(make_pair("FIM", FIM));

	Realizar_Testes(expressao, "TESTE_46");
}

//Teste identificadores
void TU_Analise_Lexica::Teste_047()
{
	string expressao = "Joao Maria Jose bool5";

	this->tabelaTeste.clear();
	this->tabelaTeste.push_back(make_pair("Joao", IDENTIFICADOR));
	this->tabelaTeste.push_back(make_pair("Maria", IDENTIFICADOR));
	this->tabelaTeste.push_back(make_pair("Jose", IDENTIFICADOR));
	this->tabelaTeste.push_back(make_pair("bool5", IDENTIFICADOR));
	//this->tabelaTeste.push_back(make_pair("FIM", FIM));

	Realizar_Testes(expressao, "TESTE_47");
}

//Teste ponto de interrogação ? e dois pontos :
void TU_Analise_Lexica::Teste_048()
{
	string expressao = " ?	:	";

	this->tabelaTeste.clear();
	this->tabelaTeste.push_back(make_pair("?", PONTO_INTERROGACAO));
	this->tabelaTeste.push_back(make_pair(":", DOIS_PONTOS));
	//this->tabelaTeste.push_back(make_pair("FIM", FIM));

	Realizar_Testes(expressao, "TESTE_48");
}

//Teste ponto e vírgula
void TU_Analise_Lexica::Teste_049()
{
	string expressao = " ;		";

	this->tabelaTeste.clear();
	this->tabelaTeste.push_back(make_pair(";", PONTO_VIRGULA));
	//this->tabelaTeste.push_back(make_pair("FIM", FIM));

	Realizar_Testes(expressao, "TESTE_49");
}

//Teste Abre e fecha parenteses ()
void TU_Analise_Lexica::Teste_050()
{
	string expressao = " () (	) ) (";

	this->tabelaTeste.clear();
	this->tabelaTeste.push_back(make_pair("(", ABRE_PARENTESES));
	this->tabelaTeste.push_back(make_pair(")", FECHA_PARENTESES));
	this->tabelaTeste.push_back(make_pair("(", ABRE_PARENTESES));
	this->tabelaTeste.push_back(make_pair(")", FECHA_PARENTESES));
	this->tabelaTeste.push_back(make_pair(")", FECHA_PARENTESES));
	this->tabelaTeste.push_back(make_pair("(", ABRE_PARENTESES));
	//this->tabelaTeste.push_back(make_pair("FIM", FIM));

	Realizar_Testes(expressao, "TESTE_50");
}

//Teste Abre e fecha colchetes []
void TU_Analise_Lexica::Teste_051()
{
	string expressao = " [] [	] ] [";

	this->tabelaTeste.clear();
	this->tabelaTeste.push_back(make_pair("[", ABRE_COLCHETES));
	this->tabelaTeste.push_back(make_pair("]", FECHA_COLCHETES));
	this->tabelaTeste.push_back(make_pair("[", ABRE_COLCHETES));
	this->tabelaTeste.push_back(make_pair("]", FECHA_COLCHETES));
	this->tabelaTeste.push_back(make_pair("]", FECHA_COLCHETES));
	this->tabelaTeste.push_back(make_pair("[", ABRE_COLCHETES));
	//this->tabelaTeste.push_back(make_pair("FIM", FIM));

	Realizar_Testes(expressao, "TESTE_51");
}

//Teste Abre e fecha chaves {}
void TU_Analise_Lexica::Teste_052()
{
	string expressao = " {} {	} } {";

	this->tabelaTeste.clear();
	this->tabelaTeste.push_back(make_pair("{", ABRE_CHAVES));
	this->tabelaTeste.push_back(make_pair("}", FECHA_CHAVES));
	this->tabelaTeste.push_back(make_pair("{", ABRE_CHAVES));
	this->tabelaTeste.push_back(make_pair("}", FECHA_CHAVES));
	this->tabelaTeste.push_back(make_pair("}", FECHA_CHAVES));
	this->tabelaTeste.push_back(make_pair("{", ABRE_CHAVES));
	//this->tabelaTeste.push_back(make_pair("FIM", FIM));

	Realizar_Testes(expressao, "TESTE_52");
}

//Teste comentário linha //
void TU_Analise_Lexica::Teste_053()
{
	string expressao = "{5+5} // teste nao deve gerar token : = + ; () bool true int float";

	this->tabelaTeste.clear();
	this->tabelaTeste.push_back(make_pair("{", ABRE_CHAVES));
	this->tabelaTeste.push_back(make_pair("5", NUM_INT));
	this->tabelaTeste.push_back(make_pair("+", OP_ADICAO));
	this->tabelaTeste.push_back(make_pair("5", NUM_INT));
	this->tabelaTeste.push_back(make_pair("}", FECHA_CHAVES));

	Realizar_Testes(expressao, "TESTE_53");
}

//Teste comentário /* */
void TU_Analise_Lexica::Teste_054()
{
	string expressao = "{} {}/* \n \t 10 20 30 **** / /* */}{";

	this->tabelaTeste.clear();
	this->tabelaTeste.push_back(make_pair("{", ABRE_CHAVES));
	this->tabelaTeste.push_back(make_pair("}", FECHA_CHAVES));
	this->tabelaTeste.push_back(make_pair("{", ABRE_CHAVES));
	this->tabelaTeste.push_back(make_pair("}", FECHA_CHAVES));
	this->tabelaTeste.push_back(make_pair("}", FECHA_CHAVES));
	this->tabelaTeste.push_back(make_pair("{", ABRE_CHAVES));

	Realizar_Testes(expressao, "TESTE_54");
}

//Teste geral, com diversos operadores e identificadores
void TU_Analise_Lexica::Teste_055()
{
	stringstream expressao;

	expressao << "int numero=14;/* teste" << endl;
	expressao << "maluco */void main(){//teste" << endl;
	expressao << "}";

	this->tabelaTeste.clear();
	this->tabelaTeste.push_back(make_pair("int", TIPO_INT));
	this->tabelaTeste.push_back(make_pair("numero", IDENTIFICADOR));
	this->tabelaTeste.push_back(make_pair("=", OP_ATRIBUICAO));
	this->tabelaTeste.push_back(make_pair("14", NUM_INT));
	this->tabelaTeste.push_back(make_pair(";", PONTO_VIRGULA));
	this->tabelaTeste.push_back(make_pair("void", TIPO_VOID));
	this->tabelaTeste.push_back(make_pair("main", IDENTIFICADOR));
	this->tabelaTeste.push_back(make_pair("(", ABRE_PARENTESES));
	this->tabelaTeste.push_back(make_pair(")", FECHA_PARENTESES));
	this->tabelaTeste.push_back(make_pair("{", ABRE_CHAVES));
	this->tabelaTeste.push_back(make_pair("}", FECHA_CHAVES));

	Realizar_Testes(expressao.str(), "TESTE_55");
}

//Teste palavra reservada const
void TU_Analise_Lexica::Teste_056()
{
	string expressao = "const var sub string";

	this->tabelaTeste.clear();
	this->tabelaTeste.push_back(make_pair("const", CONSTANTE));
	this->tabelaTeste.push_back(make_pair("var", VAR));
	this->tabelaTeste.push_back(make_pair("sub", SUB));
	this->tabelaTeste.push_back(make_pair("string", TIPO_STRING));

	Realizar_Testes(expressao, "TESTE_56");
}

//Teste virgula
void TU_Analise_Lexica::Teste_057()
{
	string expressao = "const, var , sub ,string";

	this->tabelaTeste.clear();
	this->tabelaTeste.push_back(make_pair("const", CONSTANTE));
	this->tabelaTeste.push_back(make_pair(",", VIRGULA));
	this->tabelaTeste.push_back(make_pair("var", VAR));
	this->tabelaTeste.push_back(make_pair(",", VIRGULA));
	this->tabelaTeste.push_back(make_pair("sub", SUB));
	this->tabelaTeste.push_back(make_pair(",", VIRGULA));
	this->tabelaTeste.push_back(make_pair("string", TIPO_STRING));

	Realizar_Testes(expressao, "TESTE_57");
}

//Teste operador <>
void TU_Analise_Lexica::Teste_058()
{
	string expressao = " <> < >	";

	this->tabelaTeste.clear();
	this->tabelaTeste.push_back(make_pair("<>", OP_DIFERENTE));
	this->tabelaTeste.push_back(make_pair("<", OP_MENOR));
	this->tabelaTeste.push_back(make_pair(">", OP_MAIOR));
	Realizar_Testes(expressao, "TESTE_58");
}

//Teste palavras reservadas novas
void TU_Analise_Lexica::Teste_059()
{
	string expressao = " by value ref print println repeat return scan scanln then end_if end_sub end_function to next until loop not or div mod and";

	this->tabelaTeste.clear();
	this->tabelaTeste.push_back(make_pair("by", BY));
	this->tabelaTeste.push_back(make_pair("value", VALUE));
	this->tabelaTeste.push_back(make_pair("ref", REF));
	this->tabelaTeste.push_back(make_pair("print", PRINT));
	this->tabelaTeste.push_back(make_pair("println", PRINTLN));
	this->tabelaTeste.push_back(make_pair("repeat", LACO_REPEAT));
	this->tabelaTeste.push_back(make_pair("return", RETORNO));
	this->tabelaTeste.push_back(make_pair("scan", SCAN));
	this->tabelaTeste.push_back(make_pair("scanln", SCANLN));
	this->tabelaTeste.push_back(make_pair("then", CONDICAO_THEN));
	this->tabelaTeste.push_back(make_pair("end_if", CONDICAO_ENDIF));
	this->tabelaTeste.push_back(make_pair("end_sub", END_SUB));
	this->tabelaTeste.push_back(make_pair("end_function", END_FUNCTION));
	this->tabelaTeste.push_back(make_pair("to", LACO_TO));
	this->tabelaTeste.push_back(make_pair("next", LACO_NEXT));
	this->tabelaTeste.push_back(make_pair("until", LACO_UNTIL));
	this->tabelaTeste.push_back(make_pair("loop", LACO_LOOP));
	this->tabelaTeste.push_back(make_pair("not", OP_NEGACAO));
	this->tabelaTeste.push_back(make_pair("or", OP_LOGICO_OU));
	this->tabelaTeste.push_back(make_pair("div", OP_DIVISAO));
	this->tabelaTeste.push_back(make_pair("mod", OP_MOD));
	this->tabelaTeste.push_back(make_pair("and", OP_LOGICO_E));
	Realizar_Testes(expressao, "TESTE_59");
}

