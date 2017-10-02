#include "C_Tabela_Simbolos.h"



C_Tabela_Simbolos::C_Tabela_Simbolos()
{
	chave = 0;
	tabela_simbolos.clear();
}

C_Tabela_Simbolos::~C_Tabela_Simbolos()
{
}

bool C_Tabela_Simbolos::Inserir(S_Simbolos _simbolos, string _escopo)
{
	//Na inser��o, defino como v�lido se n�o for escopo Estrutura ou Classe
	if (_escopo != ESTRUTURA && _escopo != CLASSE)
	{
		_simbolos.valido = true;
	}
	else
	{
		if (_simbolos.categoria == ESTRUTURA || _simbolos.categoria == CLASSE)
		{
			_simbolos.valido = true;
		}
		else
			_simbolos.valido = true;
	}
	

	//Antes de inserir, verifico se o identificador j� foi declarado
	Existe_ID(_simbolos);

	//S� vari�vel tem posi��o na pilha
	if (_simbolos.categoria != VAR)
		_simbolos.pos_pilha = -1;

	tabela_simbolos.push_back(_simbolos);
	return true;
}

bool C_Tabela_Simbolos::Consultar(string _identificador)
{
	for (auto it = tabela_simbolos.begin(); it != tabela_simbolos.end(); it++)
	{
		if (it->identificador == _identificador)
			if (it->valido)
				return true;
	}
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

string C_Tabela_Simbolos::Buscar_ID_Pai(int _pai)
{
	for (auto it = tabela_simbolos.begin(); it != tabela_simbolos.end(); it++)
	{
		if (it->chave == _pai)
			if (it->valido)
				return it->identificador;
	}
	//Caso n�o encontre ou seja 0 (Global) retorna vazio
	return "";
}

bool C_Tabela_Simbolos::Verificar_Array(string _identificador)
{
	vector<S_Simbolos>::iterator simbolo;
	simbolo = Buscar_Simbolo(_identificador);
	if (simbolo._Ptr != nullptr && simbolo->array)
		return true;
	return false;
}

int C_Tabela_Simbolos::Buscar_Chave(string _identificador)
{
	vector<S_Simbolos>::iterator simbolo;
	simbolo = Buscar_Simbolo(_identificador);
	if (simbolo._Ptr != nullptr)
		return simbolo->chave;
	return 0;
}

string C_Tabela_Simbolos::Buscar_Categoria(string _identificador)
{
	vector<S_Simbolos>::iterator simbolo;
	simbolo = Buscar_Simbolo(_identificador);
	if (simbolo._Ptr != nullptr)
		return simbolo->categoria;
	return "";
}

string C_Tabela_Simbolos::Buscar_Tipo(string _identificador)
{
	vector<S_Simbolos>::iterator simbolo;
	simbolo = Buscar_Simbolo(_identificador);
	if (simbolo._Ptr != nullptr)
		return simbolo->tipo;
	return "";
}

bool C_Tabela_Simbolos::Verificar_Ponteiro(string _identificador)
{
	vector<S_Simbolos>::iterator simbolo;
	simbolo = Buscar_Simbolo(_identificador);
	if (simbolo._Ptr != nullptr && simbolo->ponteiro)
		return true;
	return false;
}

int C_Tabela_Simbolos::Buscar_Qtd_Params(string _identificador)
{
	vector<S_Simbolos>::iterator simbolo;
	simbolo = Buscar_Simbolo(_identificador);
	return simbolo->qtd_params;
}

int C_Tabela_Simbolos::Buscar_Pos_Pilha(S_Id_Pai _sidpai)
{
	vector<S_Simbolos>::iterator simbolo;
	simbolo = Buscar_Simbolo(_sidpai);
	return simbolo->pos_pilha;
}

int C_Tabela_Simbolos::Buscar_Pos_Pilha_Ini_Str(S_Id_Pai _sidpai)
{
	vector<S_Simbolos>::iterator simbolo;
	simbolo = Buscar_Simbolo(_sidpai);
	return simbolo->pos_pilha_ini_str;
}

int C_Tabela_Simbolos::Buscar_Pai(string _identificador)
{
	vector<S_Simbolos>::iterator simbolo;
	simbolo = Buscar_Simbolo(_identificador);
	return simbolo->pai;
}

string C_Tabela_Simbolos::Buscar_Rotulo(string _identificador)
{
	vector<S_Simbolos>::iterator simbolo;
	simbolo = Buscar_Simbolo(_identificador);
	if (simbolo._Ptr != nullptr)
		return simbolo->rotulo;
	return "";
}

vector<S_Simbolos>::iterator C_Tabela_Simbolos::Buscar_Simbolo(int _chave)
{
	for (auto it = tabela_simbolos.begin(); it != tabela_simbolos.end(); it++)
	{
		if (it->chave == _chave)
			if (it->valido)
				return it;
	}
	return vector<S_Simbolos>::iterator();
}

vector<S_Simbolos>::iterator C_Tabela_Simbolos::Buscar_Simbolo(string _identificador)
{
	for (auto it = tabela_simbolos.begin(); it != tabela_simbolos.end(); it++)
	{
		if (it->identificador == _identificador)
			if (it->valido)
				return it;
	}
	return vector<S_Simbolos>::iterator();
}

vector<S_Simbolos>::iterator C_Tabela_Simbolos::Buscar_Simbolo(S_Id_Pai _sidpai)
{
	for (auto it = tabela_simbolos.begin(); it != tabela_simbolos.end(); it++)
	{
		if (it->identificador == _sidpai.identificador && it->pai == _sidpai.parente)
			if (it->valido)
				return it;
	}
	return vector<S_Simbolos>::iterator();
}

void C_Tabela_Simbolos::Atualizar_Pilha_Param(int _pai, int _qtd_params, stack<pair<int,int>>& _pilha)
{
	if (_qtd_params > 0)
	{
		int temp_param = 0;
		int temp_val = 0;
		for (auto it = tabela_simbolos.begin(); it != tabela_simbolos.end(); it++)
		{
			if (it->pai == _pai && it->categoria == PARAM && it->valido)
			{
				it->pos_pilha = -3 - (_qtd_params - temp_param++);
				if (it->passby == VALUE)
				{
					_pilha.push(make_pair(it->pos_pilha, temp_val));
					//Preciso alterar o valor da tabela de s�mbolos, agora � o endere�o local, pois foi passado por valor
					it->pos_pilha = temp_val++;//Inicia com o 0 e vou incrementando
				}
			}
		}
	}
}

void C_Tabela_Simbolos::Atualizar_Tamanho_String(S_Id_Pai _sidpai, string _str)
{
	vector<S_Simbolos>::iterator simbolo;
	simbolo = Buscar_Simbolo(_sidpai);

	simbolo->tam_str = 0;
	for (auto it = _str.begin(); it != _str.end() -2; it++)
	{
		simbolo->tam_str++;
	}
}

int C_Tabela_Simbolos::Buscar_Tamanho_String(S_Id_Pai _sidpai)
{
	vector<S_Simbolos>::iterator simbolo;
	simbolo = Buscar_Simbolo(_sidpai);
	return simbolo->tam_str;
}

int C_Tabela_Simbolos::Remover_Internos(string _identificador)
{
	vector<S_Simbolos>::iterator simbolo;
	simbolo = Buscar_Simbolo(_identificador);
	
	int cont = 0;

	for (auto it = tabela_simbolos.begin(); it != tabela_simbolos.end(); it++)
	{
		//S� conto removidos as vari�veis e par�metros(que tenham sido passados por valor), mas desativo as constantes locais
		if (it->pai == simbolo->chave && (it->categoria == VAR || (it->categoria == PARAM && it->passby == VALUE)))
		{
			it->valido = false;
			cont++;
		}
		else if (it->pai == simbolo->chave && it->categoria == CONSTANTE)
		{
			it->valido = false;
		}
			
	}
	//TODO 02 - Desativar ou n�o?
	//simbolo->valido = false;
	return cont;
}

int C_Tabela_Simbolos::Remover_Globais()
{
	int cont = 0;

	for (auto it = tabela_simbolos.begin(); it != tabela_simbolos.end(); it++)
	{
		if (it->pai == 0 && it->categoria == VAR)
		{
			it->valido = false;
			cont++;
		}
	}

	return cont;
}

void C_Tabela_Simbolos::Imprimir_TS(string _nome_arquivo)
{
	fstream arquivo;
	stringstream ss;
	string nome_arquivo_final;

	//Ordenar tabela de s�mbolos pela chave
	sort(tabela_simbolos.begin(), tabela_simbolos.end());
	
	ss << setw(5) << left << "#";
	ss << setw(20) << left << "ID";
	ss << setw(20) << left << "CATEGORIA";
	ss << setw(20) << left << "TIPO";
	ss << setw(20) << left << "Ponteiro";
	ss << setw(10) << left << "Array";
	ss << setw(15) << left << "valor";
	ss << setw(10) << left << "QtdParam";
	ss << setw(10) << left << "Pass By";
	ss << setw(10) << left << "Pai";
	ss << setw(10) << left << "Classe";
	ss << setw(10) << left << "Acesso";
	ss << setw(10) << left << "Ativo";
	ss << setw(10) << left << "Linha";
	ss << setw(10) << left << "Pilha"; 
	ss << setw(15) << left << "Pilha_Ini_Str";
	ss << setw(10) << left << "Tam Str";
	ss << setw(20) << left << "Rotulo";

	ss << endl;
	ss << "----------------------------------------------------------------------------------";
	ss << "----------------------------------------------------------------------------------" << endl;
	for (auto it = tabela_simbolos.begin(); it != tabela_simbolos.end(); it++)
	{
		ss << setw(5) << left << it->chave;
		ss << setw(20) << left << it->identificador;
		ss << setw(20) << left << it->categoria;
		ss << setw(20) << left << it->tipo;
		ss << setw(20) << left << it->ponteiro;
		ss << setw(10) << left << it->array;
		ss << setw(15) << left << it->valor;
		ss << setw(10) << left << it->qtd_params;
		ss << setw(10) << left << it->passby;
		ss << setw(10) << left << it->pai;
		ss << setw(10) << left << it->classe;
		ss << setw(10) << left << it->access;
		ss << setw(10) << left << it->valido;
		ss << setw(10) << left << it->linha;
		ss << setw(10) << left << it->pos_pilha;
		ss << setw(15) << left << it->pos_pilha_ini_str;
		ss << setw(10) << left << it->tam_str;
		ss << setw(20) << left << it->rotulo;
		ss << endl;
		ss << "----------------------------------------------------------------------------------";
		ss << "----------------------------------------------------------------------------------" << endl;
	}

	//Remover a extens�o .chi do arquivo
	nome_arquivo_final = _nome_arquivo.substr(0, _nome_arquivo.size() - 4);

	//Adicionar informa��es ao nome do arquivo e extens�o .txt
	nome_arquivo_final = nome_arquivo_final + "_TabSim.txt";

	arquivo.open(nome_arquivo_final, ios::out | ios::trunc);
	arquivo << ss.str();
	arquivo.close();

	cout << "Arquivo \"" << nome_arquivo_final << "\" gerado" << endl;
}

void C_Tabela_Simbolos::Gravar_TS(string _nome_arquivo)
{
	fstream arquivo;
	stringstream ss;
	string nome_arquivo_final;

	//Ordenar tabela de s�mbolos pela chave
	sort(tabela_simbolos.begin(), tabela_simbolos.end());

	ss << "sep=;" << endl;
	ss << "#;";
	ss << "ID;";
	ss << "CATEGORIA;";
	ss << "TIPO;";
	ss << "Ponteiro;";
	ss << "Array;";
	ss << "valor;";
	ss << "QtdParam;";
	ss << "Pass By;";
	ss << "Pai;";
	ss << "Classe;";
	ss << "Acesso;";
	ss << "Ativo;";
	ss << "Linha;";
	ss << "Pilha;";
	ss << "Pilha_Ini_Str;";
	ss << "Tam Str;";
	ss << "Rotulo;";
	ss << endl;

	for (auto it = tabela_simbolos.begin(); it != tabela_simbolos.end(); it++)
	{
		ss << it->chave << ";";
		ss << it->identificador << ";";
		ss << it->categoria << ";";
		ss << it->tipo << ";";
		ss << it->ponteiro << ";";
		ss << it->array << ";";
		ss << it->valor << ";";
		ss << it->qtd_params << ";";
		ss << it->passby << ";";
		ss << it->pai << ";";
		ss << it->classe << ";";
		ss << it->access << ";";
		ss << it->valido << ";";
		ss << it->linha << ";";
		ss << it->pos_pilha << ";";
		ss << it->pos_pilha_ini_str << ";";
		ss << it->tam_str << ";";
		ss << it->rotulo << ";";
		ss << endl;
	}

	//Remover a extens�o .chi do arquivo
	nome_arquivo_final = _nome_arquivo.substr(0, _nome_arquivo.size() - 4);

	//Adicionar informa��es ao nome do arquivo e extens�o .csv
	nome_arquivo_final = nome_arquivo_final + "_TabSim.csv";

	arquivo.open(nome_arquivo_final, ios::out | ios::trunc);
	arquivo << ss.str();
	arquivo.close();

	cout << "Arquivo \"" << nome_arquivo_final << "\" gerado" << endl;
}

vector<S_Simbolos> C_Tabela_Simbolos::Buscar_Var_Estrutura(string _identificador)
{
	vector<S_Simbolos> l_simbolos;
	for (auto it = tabela_simbolos.begin(); it != tabela_simbolos.end(); it++)
	{
		if (it->identificador == _identificador)
		{
			for (auto it2 = tabela_simbolos.begin(); it2 != tabela_simbolos.end(); it2++)
			{
				if (it->chave == it2->pai)
				{
					l_simbolos.push_back(*it2);
				}
			}
		}
	}
	return l_simbolos;
}

void C_Tabela_Simbolos::Erro(string _msg)
{
	cout << _msg << endl;
	system("pause");
	exit(EXIT_FAILURE);
}

void C_Tabela_Simbolos::Erro(string _msg, S_Simbolos _simbolo)
{
	cout << "ERRO-Linha: " << _simbolo.linha << " - " << _simbolo.identificador << " -> " << _msg << endl;
	system("pause");
	exit(EXIT_FAILURE);
}
