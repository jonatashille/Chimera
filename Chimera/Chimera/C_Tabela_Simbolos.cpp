#include "C_Tabela_Simbolos.h"



C_Tabela_Simbolos::C_Tabela_Simbolos()
{
	chave = 0;
	tabela_simbolos.clear();
}


C_Tabela_Simbolos::~C_Tabela_Simbolos()
{
	Imprimir_TS();
}

bool C_Tabela_Simbolos::Inserir(S_Simbolos _simbolos)
{
	//Na inserção, defino como válido
	//_simbolos.chave = ++chave;
	_simbolos.valido = true;
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

	//Ordenar tabela de símbolos pela chave
	sort(tabela_simbolos.begin(), tabela_simbolos.end());

	ss << setw(5) << left << "#";
	ss << setw(20) << left << "ID";
	ss << setw(20) << left << "CATEGORIA";
	ss << setw(20) << left << "TIPO";
	ss << setw(15) << left << "valor";
	ss << setw(10) << left << "Pass By";
	ss << setw(10) << left << "Pai";
	ss << setw(10) << left << "Acesso";
	ss << setw(10) << left << "Ativo";

	ss << endl;
	ss << "------------------------------------------------------------------";
	ss << "------------------------------------------------------------------" << endl;
	for (auto it = tabela_simbolos.begin(); it != tabela_simbolos.end(); it++)
	{
		ss << setw(5) << left << it->chave;
		ss << setw(20) << left << it->identificador;
		ss << setw(20) << left << it->categoria;
		ss << setw(20) << left << it->tipo;
		ss << setw(15) << left << it->valor;
		ss << setw(10) << left << " ";
		ss << setw(10) << left << it->pai;
		ss << setw(10) << left << it->access;
		ss << setw(10) << left << it->valido;
		for (auto it2 = it->params.begin(); it2 != it->params.end(); it2++)
		{
			ss << endl;
			ss << "------------------------------------------------------------------";
			ss << "------------------------------------------------------------------" << endl;
			ss << setw(5) << left << it2->chave;
			ss << setw(20) << left << it2->identificador;
			ss << setw(20) << left << it2->categoria;
			ss << setw(20) << left << it2->tipo;
			ss << setw(15) << left << " ";
			ss << setw(10) << left << it2->passby;
			ss << setw(10) << left << it2->pai;
			ss << setw(10) << left << " ";
			ss << setw(10) << left << it->valido;
		}
		ss << endl;
		ss << "------------------------------------------------------------------";
		ss << "------------------------------------------------------------------" << endl;
	}

	arquivo.open("TS.txt", ios::out | ios::trunc);
	arquivo << ss.str();
	arquivo.close();

	cout << "Arquivo  TS.txt gerado" << endl;
}
