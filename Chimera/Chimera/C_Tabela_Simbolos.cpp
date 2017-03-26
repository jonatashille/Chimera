#include "C_Tabela_Simbolos.h"



C_Tabela_Simbolos::C_Tabela_Simbolos()
{
	tabela_simbolos.clear();
}


C_Tabela_Simbolos::~C_Tabela_Simbolos()
{
	Imprimir_TS();
}

bool C_Tabela_Simbolos::Inserir(S_Simbolos _simbolos)
{
	tabela_simbolos.push_back(_simbolos);
	return true;
}

bool C_Tabela_Simbolos::Constultar(string)
{
	return false;
}

string C_Tabela_Simbolos::Retornar_Tipo(string)
{
	return string();
}

bool C_Tabela_Simbolos::Remover(string)
{
	return false;
}

void C_Tabela_Simbolos::Imprimir_TS()
{
	fstream arquivo;
	stringstream ss;

	ss << setw(5) << left << "#";
	ss << setw(15) << left << "ID";
	ss << setw(20) << left << "CATEGORIA";
	ss << setw(20) << left << "TIPO";
	ss << setw(8) << left << "Escopo";
	ss << setw(10) << left << "Param";
	ss << setw(10) << left << "Parametros";
	ss << setw(10) << left << "Pass By";
	ss << setw(20) << left << "valor";
	ss << endl;
	ss << "------------------------------------------------------------------";
	ss << "------------------------------------------------------------------" << endl;
	for (auto it = tabela_simbolos.begin(); it != tabela_simbolos.end(); it++)
	{
		ss << setw(5) << left << it->chave;
		ss << setw(15) << left << it->identificador;
		ss << setw(20) << left << it->categoria;
		ss << setw(20) << left << it->tipo;
		ss << setw(8) << left << it->escopo;
		ss << setw(10) << left << it->parametro;
		ss << setw(10) << left << it->params;
		ss << setw(10) << left << it->passby;
		ss << setw(20) << left << it->valor;
		ss << endl;
	}

	arquivo.open("TS.txt", ios::out | ios::trunc);
	arquivo << ss.str();
	arquivo.close();

	cout << "Arquivo  TS.txt gerado" << endl;
}
