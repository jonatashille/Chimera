#pragma once
#include "stdafx.cpp"
#include "C_Arquivo.h"
#include "C_Analise_Lexica.h"
#include "C_Analise_Sintatica.h"


void Compilar(string _nome_arquivo)
{
	C_Arquivo arquivo(_nome_arquivo); //Nome do arquivo a ser aberto para leitura do programa
	string conteudo = arquivo.getConteudoArquivo();

	//Realizo a analise lexica com o conte�do do arquivo
	C_Analise_Lexica lexica(conteudo);
	//Inicio ela
	lexica.Iniciar();
	//A analise lexica me gera uma tabela com token e lexemas, que ser�o utilizado nas analises seguintes
	vector<S_Token_Lexema> tabela_token_lexema = lexica.Get_Tabela_Token_Lexema();

	//Realizo a analise sint�tica
	C_Analise_Sintatica sintatica(tabela_token_lexema);
	//Inicio a analise sintatica
	sintatica.Iniciar();

	lexica.Gravar_Tabela_Token(_nome_arquivo + ".csv");

}
