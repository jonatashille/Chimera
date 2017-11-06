#pragma once
#include <iostream>
#include <vector>
#include <regex>
#include <string>
#include <sstream>
#include <fstream>
#include <stack>


//#include <utility>
#include <iomanip>
#include <map>


//Enumeradores
enum Acesso { PUBLIC, PRIVATE, PROTECTED };

//TAM String
#define TAM_STRING 15 

//TOKENS
#define FIM "FIM"
#define FIM_PROGRAMA "FIM_PROGRAMA"

#define IDENTIFICADOR "IDENTIFICADOR"
#define NUM_INT "NUMERO_INTEIRO"
#define NUM_REAL "NUMERO_REAL" 
#define ID_SEL_IDENTIFICADOR "ID_SEL_IDENTIFICADOR"
#define ID_SEL_PONTEIRO "ID_SEL_PONTEIRO"

#define CARACTERE "CARACTERE"
#define STRING "STRING"

//CARACTERES
#define TAB 9
#define FIM_LINHA 10
#define ESPACO ' '

//PALAVRAS RESERVADAS
#define TIPO_BOOLEANO "TIPO_BOOLEANO"
#define TIPO_CHAR "TIPO_CHAR"
#define TIPO_FLOAT "TIPO_FLOAT"
#define TIPO_INT "TIPO_INT"
#define TIPO_VOID "TIPO_VOID"
#define TIPO_STRING "TIPO_STRING"
#define ESTRUTURA "ESTRUTURA"
#define CLASSE "CLASSE"
#define END_STRUCT "END_STRUCT"
#define END_CLASS "END_CLASS"

#define LACO_DO "LACO_DO"
#define LACO_FOR "LACO_FOR"
#define LACO_WHILE "LACO_WHILE"
#define LACO_BREAK "LACO_BREAK"
#define LACO_CONTINUE "LACO_CONTINUE"
#define LACO_TO "LACO_TO" 
#define LACO_NEXT "LACO_NEXT" 
#define LACO_REPEAT "LACO_REPEAT" 
#define LACO_UNTIL "LACO_UNTIL"
#define LACO_LOOP "LACO_LOOP"

#define CONDICAO_IF "CONDICAO_IF"
#define CONDICAO_ELSE "CONDICAO_ELSE"
#define CONDICAO_ENDIF "CONDICAO_ENDIF" 
#define CONDICAO_THEN "CONDICAO_THEN" 

#define VERDADEIRO "VERDADEIRO"
#define FALSO "FALSO"

#define RETORNO "RETORNO"
#define GOTO "GOTO"

#define CONSTANTE "CONSTANTE"
#define SUB "SUB"
#define END_SUB "END_SUB"
#define VAR "VAR"
#define FUNCTION "FUNCTION"
#define END_FUNCTION "END_FUNCTION"
#define BY "BY" 
#define VALUE "VALUE" 
#define REF "REF" 
#define POINTER "POINTER" 
#define PRINT "PRINT" 
#define PRINTLN "PRINTLN" 
#define SCAN "SCAN" 
#define SCANLN "SCANLN" 

#define ACESS_PRIVATE "ACESS_PRIVATE" 
#define ACESS_PUBLIC "ACESS_PUBLIC" 

//OPERADORES
#define OP_ADICAO "OP_ADICAO"
#define OP_SUBTRACAO "OP_SUBTRACAO"
#define OP_MULTIPLICACAO "OP_MULTIPLICACAO"
#define OP_DIVISAO "OP_DIVISAO"
#define OP_RESTO "OP_RESTO"
#define OP_MOD "OP_MOD"

#define OP_MAIOR "OP_MAIOR"
#define OP_MENOR "OP_MENOR"
#define OP_MAIOR_IGUAL "OP_MAIOR_IGUAL"
#define OP_MENOR_IGUAL "OP_MENOR_IGUAL"

#define OP_DIFERENTE "OP_DIFERENTE" 
#define OP_IGUALDADE "OP_IGUALDADE"
#define OP_ATRIBUICAO "OP_ATRIBUICAO"

#define OP_NEGACAO "OP_NEGACAO" 
#define OP_NEGA_IGUALDADE "OP_NEGA_IGUALDADE"

#define OP_ATRIBUICAO_ADICAO "OP_ATRIBUICAO_ADICAO"
#define OP_ATRIBUICAO_SUBTRACAO "OP_ATRIBUICAO_SUBTRACAO"
#define OP_ATRIBUICAO_MULTIPLICACAO "OP_ATRIBUICAO_MULTIPLICACAO"
#define OP_ATRIBUICAO_DIVISAO "OP_ATRIBUICAO_DIVISAO"
#define OP_ATRIBUICAO_RESTO "OP_ATRIBUICAO_RESTO"

#define OP_LOGICO_E "OP_LOGICO_E"
#define OP_LOGICO_OU "OP_LOGICO_OU"

#define OP_INCREMENTO "OP_INCREMENTO"
#define OP_DECREMENTO "OP_DECREMENTO"

//#define OP_SELECAO_IDENTIFICADOR "OP_SELECAO_IDENTIFICADOR"
//#define OP_SELECAO_PONTEIRO "OP_SELECAO_PONTEIRO"


#define ENDERECO_ELEMENTO "ENDERECO_ELEMENTO"

//SINAIS DE PONTUAÇÃO
#define  VIRGULA "VIRGULA"
#define  PONTO_VIRGULA "PONTO_VIRGULA"
#define  ABRE_PARENTESES "ABRE_PARENTESES"
#define  FECHA_PARENTESES "FECHA_PARENTESES"
#define  ABRE_COLCHETES "ABRE_COLCHETES"
#define  FECHA_COLCHETES "FECHA_COLCHETES"
#define  ABRE_CHAVES "ABRE_CHAVES"
#define  FECHA_CHAVES "FECHA_CHAVES"
#define PONTO_INTERROGACAO "PONTO_INTERROGACAO"
#define DOIS_PONTOS "DOIS_PONTOS"

#define SEPARADOR_HASHTAG "########################"


//ERROS
#define ERR_ABRE_PARENTESES "Esperado ("
#define ERR_FECHA_PARENTESES "Esperado )"

#define ERR_ENDERECO_ELEMENTO "Esperado operador &"
#define ERR_IDENTIFICADOR "Esperado identificador"

#define ERR_END_FUNCTION "Esperado end_function"
#define ERR_END_SUB "Esperado end_sub"
#define ERR_ENDIF "Esperado end_if"
#define ERR_END_STRUCT "Esperado end_struct"
#define ERR_END_CLASS "Esperado end_class"

#define ERR_OP_MENOR "Esperado <"
#define ERR_OP_MENOR_IGUAL "Esperado <="
#define ERR_OP_DIFERENTE "Esperado <>"
#define ERR_OP_IGUALDADE "Esperado =="
#define ERR_OP_MAIOR "Esperado >"
#define ERR_OP_MAIOR_IGUAL "Esperado >="

#define ERR_OP_SUBTRACAO "Esperado -"
#define ERR_OP_ADICAO "Esperado +"
#define ERR_OP_LOGICO_OU "Esperado (or / ||)"

#define ERR_OP_MULTIPLICACAO "Esperado *"
#define ERR_OP_DIVISAO "Esperado /"
#define ERR_OP_LOGICO_E "Esperado (and / &&)"
#define ERR_OP_MOD "Esperado mod"

#define ERR_ABRE_COLCHETES "Esperado ["
#define ERR_FECHA_COLCHETES "Esperado ]"

#define ERR_CARACTERE "Esperado caractere"
#define ERR_FALSO "Esperado fase"
#define ERR_NUM "Esperado numero inteiro"
#define ERR_NUM_REAL "Esperado numero real"
#define ERR_STRING "Esperado string"
#define ERR_VERDADEIRO "Esperado true"

#define ERR_VIRGULA "Esperado ,"

#define ERR_OP_NEGACAO "Esperado \'not\' ou \'!\'"

#define ERR_CONTANTE "Esperado declaracao const"
#define ERR_OP_ATRIBUICAO "Esperado ="
#define ERR_PONTO_VIRGULA "Esperado ;"
#define ERR_VAR "Esperado var"
#define ERR_TIPO_BOOLEANO "Esperado declaracao bool"
#define ERR_TIPO_CHAR "Esperado declaracao char"
#define ERR_TIPO_FLOAT "Esperado declaracao float"
#define ERR_TIPO_INT "Esperado declaracao int"
#define ERR_TIPO_STRING "Esperado declaracao string"

#define ERR_SUB "Esperado declaracao sub"
#define ERR_FUNCTION "Esperado function"
#define ERR_STRUCT "Esperado struct"
#define ERR_CLASS "Esperado class"

#define ERR_DOIS_PONTOS "Esperado :"
#define ERR_ACESS_PRIVATE "Esperado private"
#define ERR_ACESS_PUBLIC "Esperado public"

#define ERR_OP_SELECAO_IDENTIFICADOR "Esperado ."
#define ERR_OP_SELECAO_PONTEIRO "Esperado ->"

#define ERR_BY "Esperado by"
#define ERR_VALUE "Esperado value"
#define ERR_REF "Esperado ref"
#define ERR_POINTER "Esperado pointer"

#define ERR_CONDICAO_IF "Esperado if"
#define ERR_CONDICAO_THEN "Esperado then"
#define ERR_CONDICAO_ELSE "Esperado else"

#define ERR_LACO_DO "Esperado do"
#define ERR_LACO_WHILE "Esperado while"
#define ERR_LACO_FOR "Esperado for"
#define ERR_LACO_TO "Esperado to"
#define ERR_LACO_NEXT "Esperado next"
#define ERR_LACO_REPEAT "Esperado repeat"
#define ERR_LACO_UNTIL "Esperado until"
#define ERR_LACO_LOOP "Esperado loop"

#define ERR_SCAN "Esperado scan"
#define ERR_SCANLN "Esperado scanln"
#define ERR_PRINT "Esperado print"
#define ERR_PRINTLN "Esperado println"


////////////////////////////////////
#define GLOBAL "GLOBAL"
#define LOCAL "LOCAL"
#define PARAM "PARAMETRO"


////////////////////////////////////
//ERROS SEMÂNTICOS
#define ERR_SEM_NAO_DECLARDO "Identificador nao declarado"
#define ERR_SEM_DECLARACAO_DUPLICADA " ja foi declarado"
#define ERR_SEM_INDEX_VAR_SIMPLES "Indexacao de variavel simples"
#define ERR_SEM_CONST_ATRIB "Constante nao pode ser atribuida"

#define ERR_SEM_IF "Comando IF aceita apenas tipo booleano ou tipo inteiro para expressao"
#define ERR_SEM_DO "Laco DO aceita apenas tipo booleano ou tipo inteiro para expressao"
#define ERR_SEM_FOR "Laco FOR aceita apenas tipo inteiro para expressao"
#define ERR_SEM_REAPEAT "Laco REPEAT aceita apenas tipo booleano ou tipo inteiro para expressao"
#define ERR_SEM_WHILE "Laco WHILE aceita apenas tipo booleano ou tipo inteiro para expressao"


#define ERR_SEM_INCOMPATIBILIDADE_TIPO "Tipos incompativeis"
#define ERR_SEM_NUM_PARAMS "Numero de parametros inconsistente com a declaracao da funcao/procedimento"

#define ERR_SEM_INDEX_TIPO_INT "Indice de vetores deve ser do tipo inteiro"
#define ERR_SEM_INDEX_DECL_TIPO_INT "Tamanho de vetores deve ser do tipo inteiro"

#define ERR_SEM_ATRIB_BOOL "Atribuicao a um tipo incompativel - BOOLEANO aceita BOOLEANO ou INTEIRO"
#define ERR_SEM_ATRIB_CHAR "Atribuicao a um tipo incompativel - CHAR aceita CHAR ou INTEIRO"
#define ERR_SEM_ATRIB_FLOAT "Atribuicao a um tipo incompativel - FLOAT aceita FLOAT ou INTEIRO"
#define ERR_SEM_ATRIB_INTEIRO "Atribuicao a um tipo incompativel - INTEIRO aceita INTEIRO ou CHAR"
#define ERR_SEM_ATRIB_STRING "Atribuicao a um tipo incompativel - STRING aceita apenas STRING"

#define ERR_SEM_NAO_ACESSIVEL_PRIVADO "Identificador nao acessivel: Membro PRIVADO"