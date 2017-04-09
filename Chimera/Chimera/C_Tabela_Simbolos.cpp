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
	//Na inser��o, defino como v�lido
	_simbolos.valido = true;

	//Antes de inserir, verifico se o identificador j� foi declarado
	Existe_ID(_simbolos);

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

void C_Tabela_Simbolos::Inativar_Simbolos(int _id)
{

}

void C_Tabela_Simbolos::Existe_ID(S_Simbolos _simbolo)
{
	for (auto it = tabela_simbolos.begin(); it != tabela_simbolos.end(); it++)
	{
		//Verificar se � o mesmo identificador e a mesma categoria
		if (it->identificador == _simbolo.identificador && it->categoria == _simbolo.categoria)
		{
			//Se for, verifico se est�o no mesmo escopo e se est� v�lido
			if (it->pai == _simbolo.pai && it->valido == true)
			{
				stringstream msg;
				msg << _simbolo.categoria << " \'" << _simbolo.identificador << "\'" << ERR_SEM_DECLARACAO_DUPLICADA << endl;
				Erro(msg.str(), _simbolo);
			}
		}
	}
}

void C_Tabela_Simbolos::Imprimir_TS()
{
	fstream arquivo;
	stringstream ss;

	//Ordenar tabela de s�mbolos pela chave
	sort(tabela_simbolos.begin(), tabela_simbolos.end());

	ss << setw(5) << left << "#";
	ss << setw(20) << left << "ID";
	ss << setw(20) << left << "CATEGORIA";
	ss << setw(20) << left << "TIPO";
	ss << setw(15) << left << "valor";
	ss << setw(10) << left << "Pass By";
	ss << setw(10) << left << "Pai";
	ss << setw(10) << left << "Classe";
	ss << setw(10) << left << "Acesso";
	ss << setw(10) << left << "Ativo";
	ss << setw(10) << left << "Linha";

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
		ss << setw(10) << left << it->passby;
		ss << setw(10) << left << it->pai;
		ss << setw(10) << left << it->classe;
		ss << setw(10) << left << it->access;
		ss << setw(10) << left << it->valido;
		ss << setw(10) << left << it->linha;
		ss << endl;
		ss << "------------------------------------------------------------------";
		ss << "------------------------------------------------------------------" << endl;
	}

	arquivo.open("TS.txt", ios::out | ios::trunc);
	arquivo << ss.str();
	arquivo.close();

	cout << "Arquivo  TS.txt gerado" << endl;
}

void C_Tabela_Simbolos::Erro(string _msg, S_Simbolos _simbolo)
{
	cout << "ERRO-Linha: " << _simbolo.linha << " - " << _simbolo.identificador << " -> " << _msg << endl;
	system("pause");
	exit(EXIT_FAILURE);
}
