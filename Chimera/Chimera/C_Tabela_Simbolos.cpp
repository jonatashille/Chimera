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
	//Na inserção, defino como válido se não for escopo Estrutura ou Classe
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
			_simbolos.valido = false;
	}
	

	_simbolos.valido = true;
	//Antes de inserir, verifico se o identificador já foi declarado
	Existe_ID(_simbolos);

	//Só variável tem posição na pilha
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

bool C_Tabela_Simbolos::Consultar_Acesso(string _identificador, int _pai)
{
	for (auto it = tabela_simbolos.begin(); it != tabela_simbolos.end(); it++)
	{
		if (it->identificador == _identificador && it->pai == _pai)
			if (it->valido && !(Buscar_Categoria_Pai(_pai) != CLASSE && it->access))
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
		//Verificar se é o mesmo identificador e a mesma categoria
		if (it->identificador == _simbolo.identificador && it->categoria == _simbolo.categoria)
		{
			//Se for, verifico se estão no mesmo escopo e se está válido
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
	//Caso não encontre ou seja 0 (Global) retorna vazio
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

int C_Tabela_Simbolos::Buscar_Chave(S_Id_Pai _sidpai)
{
	vector<S_Simbolos>::iterator simbolo;
	simbolo = Buscar_Simbolo(_sidpai);
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

string C_Tabela_Simbolos::Buscar_Categoria(int _chave)
{
	vector<S_Simbolos>::iterator simbolo;
	simbolo = Buscar_Simbolo(_chave);
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

string C_Tabela_Simbolos::Buscar_Tipo(int _chave)
{
	vector<S_Simbolos>::iterator simbolo;
	simbolo = Buscar_Simbolo(_chave);
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
	if (simbolo._Ptr != nullptr)
		return simbolo->pos_pilha_ini_str;
	else
	{
		cout << "ID: " << _sidpai.identificador << " e PAI: " << _sidpai.parente << " - Nao encontrados na Tabela de Simbolos" << endl;
		return -5;
	}
}

int C_Tabela_Simbolos::Buscar_Pai(string _identificador)
{
	vector<S_Simbolos>::iterator simbolo;
	simbolo = Buscar_Simbolo(_identificador);
	return simbolo->pai;
}

string C_Tabela_Simbolos::Buscar_Categoria_Pai(int _pai)
{
	vector<S_Simbolos>::iterator simbolo;
	simbolo = Buscar_Simbolo(Buscar_ID_Pai(_pai));
	if (simbolo._Ptr != nullptr)
		return simbolo->categoria;
	return "";
}

int C_Tabela_Simbolos::Buscar_Qtd_Mem_Alocada_Params(int _pai)
{
	int count = 0;
	for (auto it = tabela_simbolos.begin(); it != tabela_simbolos.end(); it++)
	{
		if (it->pai == _pai && it->passby != REF)
		{
			if (it->tipo == TIPO_STRING)
				count += TAM_STRING;
			else
				count++;
		}
	}
	return count;
}

int C_Tabela_Simbolos::Buscar_Qtd_Tot_Params(int _pai)
{
	int count = 0;
	for (auto it = tabela_simbolos.begin(); it != tabela_simbolos.end(); it++)
	{
		if (it->pai == _pai)
		{
			if (it->tipo == TIPO_STRING)
				count += TAM_STRING;
			else
				count++;
		}
	}
	return count;
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
			if (it->valido && (Buscar_Categoria_Pai(it->pai) != ESTRUTURA && Buscar_Categoria_Pai(it->pai) != CLASSE))
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
		int temp_end = 0;
		for (auto it = tabela_simbolos.begin(); it != tabela_simbolos.end(); it++)
		{
			if (it->pai == _pai && it->categoria == PARAM && it->valido)
			{
				if (it->passby == VALUE)
				{
					if (it->tipo != TIPO_STRING)
					{
						temp_val++;
					}
					else
					{
						temp_val += TAM_STRING;
					}
				}
				else
				{
					if (it->tipo != TIPO_STRING)
					{
						temp_end++;
					}
					else
					{
						temp_end += TAM_STRING;
					}
				}
			}
		}
		//for (auto it = tabela_simbolos.end()-1; it != tabela_simbolos.begin(); it--)
		auto it = tabela_simbolos.end();
		while (it != tabela_simbolos.begin())
		{
			it--;
			if (it->pai == _pai && it->categoria == PARAM && it->valido)
			{
				if (it->tipo != TIPO_STRING)
				{
					if (it->passby == VALUE)
					{
						it->pos_pilha = (temp_val + temp_end + 3) * -1;
						temp_val--;
						_pilha.push(make_pair(it->pos_pilha, temp_val));
						//Preciso alterar o valor da tabela de símbolos, agora é o endereço local, pois foi passado por valor
						it->pos_pilha = temp_val;//Inicia com o 0 e vou incrementando
					}
					else
					{
						it->pos_pilha = -3 - (_qtd_params - temp_param);
						temp_end--;
					}
				}
				else
				{				
					if (it->passby == VALUE)
					{
						it->pos_pilha = (temp_val + temp_end + 3) * -1;
						it->pos_pilha_ini_str = it->pos_pilha + 3 + TAM_STRING;
						int limite = (it->pos_pilha + 4);
						for (int i = it->pos_pilha_ini_str; i >= limite; i--)
						{
							_pilha.push(make_pair(it->pos_pilha, (i*-1)));
							//Preciso alterar o valor da tabela de símbolos, agora é o endereço local, pois foi passado por valor
							//temp_val++;//Inicia com o 0 e vou incrementando
							it->pos_pilha += 1;
						}
						temp_val -= TAM_STRING;
					}
					else
					{
						it->pos_pilha = -3 - (_qtd_params - temp_param);
						it->pos_pilha_ini_str = it->pos_pilha + 4;
						temp_end--;
					}
					it->tam_str = TAM_STRING;
					it->pos_pilha_ini_str *= -1;
					it->pos_pilha = _pilha.top().second;
				}
				temp_param++;
			}
		}
	}
}

void C_Tabela_Simbolos::Atualizar_Pilha_Param_classe(int _pai, int _qtd_params, stack<pair<int, int>>& _pilha, int _qtd_var_classe)
{
	if (_qtd_params > 0)
	{
		int temp_param = 0;
		int temp_val = 0;
		int temp_end = 0;
		for (auto it = tabela_simbolos.begin(); it != tabela_simbolos.end(); it++)
		{
			if (it->pai == _pai && it->categoria == PARAM && it->valido)
			{
				if (it->passby == VALUE)
				{
					if (it->tipo != TIPO_STRING)
					{
						temp_val++;
					}
					else
					{
						temp_val += TAM_STRING;
					}
				}
				else
				{
					if (it->tipo != TIPO_STRING)
					{
						temp_end++;
					}
					else
					{
						temp_end += TAM_STRING;
					}
				}
			}
		}
		//for (auto it = tabela_simbolos.begin(); it != tabela_simbolos.end(); it++)
		auto it = tabela_simbolos.end();
		while (it != tabela_simbolos.begin())
		{
			it--;
			if (it->pai == _pai && it->categoria == PARAM && it->valido)
			{
				if (it->tipo != TIPO_STRING)
				{
					if (it->passby == VALUE)
					{
						it->pos_pilha = (temp_val + temp_end + 3 + _qtd_var_classe) * -1;
						temp_val--;
						_pilha.push(make_pair(it->pos_pilha, temp_val));
						//Preciso alterar o valor da tabela de símbolos, agora é o endereço local, pois foi passado por valor
						it->pos_pilha = temp_val;//Inicia com o 0 e vou incrementando
					}
					else
					{
						it->pos_pilha = -(3 + _qtd_var_classe) - (_qtd_params - temp_param++);
						temp_end--;
					}
				}
				else
				{
					if (it->passby == VALUE)
					{
						it->pos_pilha = (temp_val + temp_end + 3 + _qtd_var_classe) * -1;
						it->pos_pilha_ini_str = it->pos_pilha + 3 + TAM_STRING + _qtd_var_classe;
						int limite = (it->pos_pilha + 4);
						for (int i = it->pos_pilha_ini_str; i >= limite; i--)
						{
							_pilha.push(make_pair(it->pos_pilha, (i*-1)));
							//Preciso alterar o valor da tabela de símbolos, agora é o endereço local, pois foi passado por valor
							//temp_val++;//Inicia com o 0 e vou incrementando
							it->pos_pilha += 1;
						}
						temp_val -= TAM_STRING;
					}
					else
					{
						it->pos_pilha = -(3 + _qtd_var_classe) - (_qtd_params - temp_param++);
						it->pos_pilha_ini_str = it->pos_pilha + 4;
						temp_end--;
					}
					it->tam_str = TAM_STRING;
					it->pos_pilha_ini_str *= -1;
					it->pos_pilha = _pilha.top().second;
				}
				temp_param++;
			}
		}
	}
/*	if (_qtd_params > 0)
	{
		int temp_param = 0;
		int temp_val = 0;
		for (auto it = tabela_simbolos.begin(); it != tabela_simbolos.end(); it++)
		{
			if (it->pai == _pai && it->categoria == PARAM && it->valido)
			{
				if (it->tipo != TIPO_STRING && it->passby == VALUE)
				{
					temp_val++;
				}
			}
		}
		//for (auto it = tabela_simbolos.begin(); it != tabela_simbolos.end(); it++)
		auto it = tabela_simbolos.end();
		while (it != tabela_simbolos.begin())
		{
			it--;
			if (it->pai == _pai && it->categoria == PARAM && it->valido)
			{
				if (it->tipo != TIPO_STRING)
				{
					it->pos_pilha = -(3 + _qtd_var_classe) - (_qtd_params - temp_param++);
					if (it->passby == VALUE)
					{
						temp_val--;
						_pilha.push(make_pair(it->pos_pilha, temp_val));
						//Preciso alterar o valor da tabela de símbolos, agora é o endereço local, pois foi passado por valor
						it->pos_pilha = temp_val;//Inicia com o 0 e vou incrementando
					}
				}
				else
				{
					it->pos_pilha = -(3 + _qtd_var_classe) - (_qtd_params - temp_param++);
					if (it->passby == VALUE)
					{
						for (int i = TAM_STRING - 1; i >= 0; i--)
						{
							_pilha.push(make_pair(it->pos_pilha, i));
							//Preciso alterar o valor da tabela de símbolos, agora é o endereço local, pois foi passado por valor
							//temp_val++;//Inicia com o 0 e vou incrementando
							it->pos_pilha -= 1;
						}
					}
					it->tam_str = TAM_STRING;
					it->pos_pilha = temp_param - 1;
					it->pos_pilha_ini_str = it->pos_pilha;
				}
			}
		}
	}*/
}

void C_Tabela_Simbolos::Atualizar_Tamanho_String(S_Id_Pai _sidpai, string _str)
{
	if (_str != "")
	{
		vector<S_Simbolos>::iterator simbolo;
		simbolo = Buscar_Simbolo(_sidpai);

		simbolo->tam_str = 0;
		for (auto it = _str.begin(); it != _str.end() - 2; it++)
		{
			simbolo->tam_str++;
		}
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
	int cont = 0;


	vector<S_Simbolos>::iterator simbolo;
	simbolo = Buscar_Simbolo(_identificador);

	if (simbolo._Ptr != nullptr)
	{
		for (auto it = tabela_simbolos.begin(); it != tabela_simbolos.end(); it++)
		{
			//Só conto removidos as variáveis e parâmetros(que tenham sido passados por valor), mas desativo as constantes locais
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
	}
	//TODO 02 - Desativar ou não?
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

	//Ordenar tabela de símbolos pela chave
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
	ss << setw(20) << left << "Escopo";
    ss << setw(20) << left << "Pai_Heranca";

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
		ss << setw(20) << left << it->escopo;
		ss << setw(20) << left << it->pai_heranca;
		ss << endl;
		ss << "----------------------------------------------------------------------------------";
		ss << "----------------------------------------------------------------------------------" << endl;
	}

	//Remover a extensão .chi do arquivo
	nome_arquivo_final = _nome_arquivo.substr(0, _nome_arquivo.size() - 4);

	//Adicionar informações ao nome do arquivo e extensão .txt
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

	//Ordenar tabela de símbolos pela chave
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
	ss << "Escopo;";
	ss << "Pai_Heranca;";
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
		ss << it->escopo << ";";
		ss << it->pai_heranca << ";";
		ss << endl;
	}

	//Remover a extensão .chi do arquivo
	nome_arquivo_final = _nome_arquivo.substr(0, _nome_arquivo.size() - 4);

	//Adicionar informações ao nome do arquivo e extensão .csv
	nome_arquivo_final = nome_arquivo_final + "_TabSim.csv";

	arquivo.open(nome_arquivo_final, ios::out | ios::trunc);
	arquivo << ss.str();
	arquivo.close();

	cout << "Arquivo \"" << nome_arquivo_final << "\" gerado" << endl;
}

vector<S_Simbolos> C_Tabela_Simbolos::Buscar_Membros_Classe(string _identificador)
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

vector<S_Simbolos> C_Tabela_Simbolos::Buscar_Membros_Classe(int _chave_pai)
{
	vector<S_Simbolos> l_simbolos;
	for (auto it = tabela_simbolos.begin(); it != tabela_simbolos.end(); it++)
	{
		if (it->pai == _chave_pai)
		{
			l_simbolos.push_back(*it);
		}
	}
	return l_simbolos;
}

vector<S_Simbolos> C_Tabela_Simbolos::Buscar_Var_Classe_Pai(int _chave_pai)
{
	string categoria;
	categoria = Buscar_Categoria(Buscar_Tipo(_chave_pai));
	vector<S_Simbolos> l_simbolos;
	for (auto it = tabela_simbolos.begin(); it != tabela_simbolos.end(); it++)
	{
		if (categoria == CLASSE || categoria == ESTRUTURA)
		{
			if (it->pai == _chave_pai && it->categoria == VAR)
			{
				l_simbolos.push_back(*it);
			}
		}
	}
	return l_simbolos;
}

vector<S_Simbolos> C_Tabela_Simbolos::Buscar_Var_Pelo_Pai(int _chave_pai)
{
	vector<S_Simbolos> l_simbolos;
	for (auto it = tabela_simbolos.begin(); it != tabela_simbolos.end(); it++)
	{
		if (it->pai == _chave_pai && it->categoria == VAR)
		{
			l_simbolos.push_back(*it);
		}
	}
	return l_simbolos;
}

int C_Tabela_Simbolos::Retorna_Tot_Var_Mem_Classe()
{
	vector<S_Simbolos> l_simbolos;
	int l_cont = 0;
	for (auto it = tabela_simbolos.begin(); it != tabela_simbolos.end(); it++)
	{
		if (it->escopo != "")
		{
			if (it->tipo == TIPO_STRING)
				l_cont += TAM_STRING;
			else
				l_cont++;
		}
	}
	return l_cont;
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
