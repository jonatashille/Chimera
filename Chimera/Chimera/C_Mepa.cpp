#include "C_Mepa.h"



string C_Mepa::Cabelho()
{
	stringstream ss;
	ss << "include macros.asm" << endl;
	ss << "pilha SEGMENT PARA STACK \'STACK\'" << endl;
	ss << "dw  100  dup(? )" << endl;
	ss << "pilha ENDS" << endl;

	ss << "dados   SEGMENT PARA \'DATA\'" << endl;
	ss << "dw  10  dup(0)" << endl;
	ss << "buffer db 5 dup(\' \')" << endl;
	ss << "mesg   db  10, ? , 11 dup(0)" << endl;
	ss << "dados   ENDS" << endl;

	ss << "procs   SEGMENT PARA \'DATA\'" << endl;
	ss << "dw  32000 dup(0)" << endl;
	ss << "procs   ENDS" << endl;

	ss << "codigo  SEGMENT PARA \'CODE\'" << endl;
	ss << "ASSUME SS : pilha, CS : codigo, DS : dados, ES : dados" << endl;

	ss << "inicio PROC  FAR" << endl;
	ss << "; ----------Inicio das Instrucoes MEPA" << endl;
	ss << "INPP" << endl;

	return ss.str();
}

string C_Mepa::Rodape()
{
	stringstream ss;
	ss << "PARA" << endl;
	ss << "; ----------Fim das Instrucoes MEPA" << endl;
	ss << "inicio  ENDP" << endl;
	ss << "codigo  ENDS" << endl;
	ss << "END    inicio" << endl;
	return ss.str();
}

//Função que converte uma expressão para pos fixa e retorna um vetor
void C_Mepa::Converter_Pos_Fixa(stack<S_EXP>& _p, C_Tabela_Simbolos _ts)
{
	//Preciso reverter a pilha para pegar da posição certa
	stack<S_EXP> r_stack;
	while (!_p.empty())
	{
		r_stack.push(_p.top());
		_p.pop();
	}
	_p = r_stack;

	stack<S_EXP> p_temp;

	while (!_p.empty())
	{
		if (E_Operador(_p.top().token))
		{
			//Verificar peso operador
			while (!p_temp.empty() && p_temp.top().token != ABRE_PARENTESES && Get_Peso_Operador(p_temp.top().token) >= Get_Peso_Operador(_p.top().token))
			{
				Add_Comando(p_temp.top().token);
				p_temp.pop();
			}
			p_temp.push(_p.top());
			_p.pop();
		}
		else if (_p.top().token == ABRE_PARENTESES)
		{
			p_temp.push(_p.top());
			_p.pop();
		}
		else if (_p.top().token == FECHA_PARENTESES)
		{
			while (!p_temp.empty() && p_temp.top().token != ABRE_PARENTESES)
			{
				Add_Comando(p_temp.top().token);
				p_temp.pop();
			}
			p_temp.pop();
			_p.pop();
		}
		//Caso seja operando, empilho na pos_fixa e desempilho da expressão
		else
		{
			//Se for identificador, preciso inserir o endereço da memória dele
			if (Validar_Identificador(_p.top().token))
			{
				S_Id_Pai sidpai;
				int pai = _p.top().parente;
				if (pai != 0)
					pai = 1;

				if (_p.top().end_elemento)
					CREN(to_string(pai), to_string(_ts.Buscar_Pos_Pilha(sidpai.make_Id_Pai(_p.top().token, _p.top().parente))));
				else if (_ts.Verificar_Ponteiro(_p.top().token))
				{
					if (_ts.Buscar_Tipo(_p.top().token) != TIPO_STRING)
						CRVI(to_string(pai), to_string(_ts.Buscar_Pos_Pilha(sidpai.make_Id_Pai(_p.top().token, _p.top().parente))));
					else
					{
						int pos_pilha_ini_str = _ts.Buscar_Pos_Pilha_Ini_Str(sidpai.make_Id_Pai(_p.top().token, _p.top().parente));
						CRVI_String_Param(to_string(pai), pos_pilha_ini_str, _ts.Buscar_Tamanho_String(sidpai.make_Id_Pai(_p.top().token, _p.top().parente)));
					}
				}
				else
				{
					if (_ts.Buscar_Tipo(_p.top().token) != TIPO_STRING)
					{
						CRVL(to_string(pai), to_string(_ts.Buscar_Pos_Pilha(sidpai.make_Id_Pai(_p.top().token, _p.top().parente))));
					}
					else
					{
						if (_p.top().escopo == CLASSE && _p.top().categoria == PARAM)
						{
							int pos_pilha_ini_str = _ts.Buscar_Pos_Pilha_Ini_Str(sidpai.make_Id_Pai(_p.top().token, _p.top().parente));
							int pos_pilha = _ts.Buscar_Pos_Pilha(sidpai.make_Id_Pai(_p.top().token, _p.top().parente));
							int pai_classe = _ts.Buscar_Pai(_p.top().chave_atribuido);

							vector<S_Simbolos> l_simbolos;
							l_simbolos = _ts.Buscar_Var_Pelo_Pai(pai_classe);

							int tot_param_acima = -3;
							for (auto it = l_simbolos.begin(); it != l_simbolos.end() && it->chave < _p.top().chave_atribuido; it++)
							{
								if (it->tipo == TIPO_STRING)
								{
									tot_param_acima -= TAM_STRING;
								}
								else if (it->tipo == TIPO_INT)
								{
									tot_param_acima -= 1;
								}
							}
							CRVL_String_Param_Classe(to_string(pai), pos_pilha_ini_str, pos_pilha, tot_param_acima);
						}
						else
						{
							int pos_pilha_ini_str = _ts.Buscar_Pos_Pilha_Ini_Str(sidpai.make_Id_Pai(_p.top().token, _p.top().parente));
							int pos_pilha = _ts.Buscar_Pos_Pilha(sidpai.make_Id_Pai(_p.top().token, _p.top().parente));
							CRVL_String_Param(to_string(pai), pos_pilha_ini_str, pos_pilha);
						}

					}
				}
			}
			else
			{
				if (_p.top().tipo == TIPO_STRING)
				{
					CRCT_String_Param(_p.top().token);
				}
				else
				{
					CRCT(_p.top().token);
				}
			}
			_p.pop();
		}
	}

	while (!p_temp.empty())
	{
		Add_Comando(p_temp.top().token);
		p_temp.pop();
	}
}

//Função que verifica se um caracter é um simbolo operador ou não
bool C_Mepa::E_Operador(string _str)
{
	if (_str == "SOMA" || _str == "SUBT" || _str == "MULT" || _str == "DIVI" || _str == "%" ||
		_str == "CMME" || _str == "CMEG" || _str == "CMDG" || _str == "CMIG" || _str == "CMMA" || _str == "CMAG")
		return true;

	return false;
}

//Função que retorna o peso do operador. Operador com o peso mais alto tem maior precedência
int C_Mepa::Get_Peso_Operador(string _str)
{
	if (_str == "SOMA" || _str == "SUBT" ||
		_str == "CMME" || _str == "CMEG" || _str == "CMDG" || _str == "CMIG" || _str == "CMMA" || _str == "CMAG")
		return 1;
	else if (_str == "MULT" || _str == "DIVI")
		return 2;
	else
		return -1;
}

//Função que valida se a string é ou não um identificador
bool C_Mepa::Validar_Identificador(string _str)
{
	if (regex_match(_str, regex("(_)*[a-zA-Z][a-zA-Z0-9_]*")))
		return true;
	else
		return false;
}

C_Mepa::C_Mepa()
{
	linha_mepa = 0;
}


C_Mepa::~C_Mepa()
{
}

void C_Mepa::CRCT(string _c)
{
	linha_mepa++;
	mepa << "CRCT " << _c << endl;
	ultimo_comando = "CRCT";
}

void C_Mepa::CRCT_String(string _c, int _pos_ini, int _pos_fim)
{
	if (_c != "")
	{
		int i = 0;
		mepa << "; ---------- Inicio Armazenamento String" << endl;
		// +1 e -1 para eliminar o "
		for (auto it = _c.begin() + 1; it != _c.end() - 1 && i < TAM_STRING; it++, i++)
		{
			linha_mepa++;
			mepa << "CRCT \'" << string(1, *it) << "\'" << endl;
			ARMZ(to_string(_pos_ini));
			_pos_ini++;
			ultimo_comando = "CRCT";
		}
		mepa << "; ---------- Fim Armazenamento String" << endl;
	}
}

void C_Mepa::CRCT_String_Param(string _c)
{
	int i = 0;
	mepa << "; ---------- Inicio Carrega String" << endl;
	if (_c != "")
	{
		// +1 e -1 para eliminar o "
		for (auto it = _c.begin() + 1; it != _c.end() - 1 && i < TAM_STRING; it++, i++)
		{
			linha_mepa++;
			mepa << "CRCT \'" << string(1, *it) << "\'" << endl;
		}
	}
	//Complemento o restante com espaço vazio
	while (i < TAM_STRING)
	{
		mepa << "CRCT \' \'" << endl;
		i++;
	}
	mepa << "; ---------- Fim Carrega String" << endl;
	ultimo_comando = "CRCT";
}

void C_Mepa::CRCT_String_ARMI(string _c, int _pos)
{
	if (_c != "")
	{
		int i = 0;
		mepa << "; ---------- Inicio Armazenamento String" << endl;
		// +1 e -1 para eliminar o "
		for (auto it = _c.begin() + 1; it != _c.end() - 1 && i < TAM_STRING; it++, i++)
		{
			linha_mepa++;
			mepa << "CRCT \'" << string(1, *it) << "\'" << endl;
			ARMI(to_string(_pos));
			_pos++;
			ultimo_comando = "CRCT";
		}
		mepa << "; ---------- Fim Armazenamento String" << endl;
	}
}

void C_Mepa::CRVL(string _k, string _n)
{
 	linha_mepa++;
	mepa << "CRVL " << _k << "," << _n << endl;
	ultimo_comando = "CRVL";
}

void C_Mepa::CRVL_String(string _k, int _pos_ini, int _pos_fim)
{
	mepa << "; ---------- Inicio Leitura String" << endl;
	for (int i = _pos_ini; i <= _pos_fim; i++)
	{
		linha_mepa++;
		mepa << "CRVL " << _k << "," << i << endl;
		IMPC();
	}
	mepa << "; ---------- Fim Leitura String" << endl;
}

void C_Mepa::CRVL_String_Param(string _k, int _pos_ini, int _pos_fim)
{
	mepa << "; ---------- Inicio Carregamento String" << endl;
	for (int i = _pos_ini; i <= _pos_fim; i++)
	{
		linha_mepa++;
		mepa << "CRVL " << _k << "," << i << endl;
	}
	mepa << "; ---------- Fim Carregamento String" << endl;
}

void C_Mepa::CRVL_String_Param_Classe(string _k, int _pos_ini, int _pos_fim, int _end_param)
{
	_end_param -= TAM_STRING;
	mepa << "; ---------- Inicio Carregamento String" << endl;
	for (int i = _pos_ini; i <= _pos_fim; i++)
	{
		linha_mepa++;
		mepa << "CRVL " << _k << "," << i << endl;
		mepa << "ARMI " << _k << "," << _end_param << endl;
		_end_param++;
	}
	mepa << "; ---------- Fim Carregamento String" << endl;
}

void C_Mepa::CRVI(string _k, string _n)
{
	linha_mepa++;
	mepa << "CRVI " << _k << "," << _n << endl;
	ultimo_comando = "CRVI";
}

void C_Mepa::CRVI_String(string _k, int _pos_ini, int _pos_fim)
{
	mepa << "; ---------- Inicio Leitura String - Valor Indireto" << endl;
	for (int i = _pos_fim; i <= _pos_ini; i++)
	{
		linha_mepa++;
		mepa << "CRVI " << _k << "," << i << endl;
		IMPC();
	}
	mepa << "; ---------- Fim Leitura String - Valor Indireto" << endl;
}

void C_Mepa::CRVI_String_Param(string _k, int _pos_ini, int _pos_fim)
{
	linha_mepa++;
	mepa << "CRVI " << _k << "," << _pos_ini << endl;
}

void C_Mepa::CREN(string _k, string _n)
{
	linha_mepa++;
	mepa << "CREN " << _k << "," << _n << endl;
	ultimo_comando = "CREN";
}

void C_Mepa::CREN_String(string _k, int _pos_ini, int _pos_fim)
{
	mepa << "; ---------- Inicio Leitura Endereço String" << endl;
	for (int i = _pos_ini; i <= _pos_fim; i++)
	{
		linha_mepa++;
		mepa << "CREN " << _k << "," << i << endl;
	}
	mepa << "; ---------- Fim Leitura Endereço String" << endl;
	ultimo_comando = "CREN";
}

void C_Mepa::ARMZ(string _k, string _n)
{
	linha_mepa++;
	mepa << "ARMZ " << _k << "," << _n << endl;
	ultimo_comando = "ARMZ";
}

void C_Mepa::ARMZ(string _n)
{
	linha_mepa++;
	mepa << "ARMZ " << 1 << "," << _n << endl;
	ultimo_comando = "ARMZ";
}

void C_Mepa::ARMI(string _k, string _n)
{
	linha_mepa++;
	mepa << "ARMI " << _k << "," << _n << endl;
	ultimo_comando = "ARMI";
}

void C_Mepa::ARMI(string _n)
{
	linha_mepa++;
	mepa << "ARMI " << 1 << "," << _n << endl;
	ultimo_comando = "ARMI";
}

void C_Mepa::LEIT()
{
	linha_mepa++;
	mepa << "LEIT" << endl;
	ultimo_comando = "LEIT";
}

void C_Mepa::LECH()
{
	linha_mepa++;
	mepa << "LECH" << endl;
	ultimo_comando = "LECH";
}

void C_Mepa::IMPR()
{
	linha_mepa++;
	mepa << "IMPR" << endl;
	ultimo_comando = "IMPR";
}

void C_Mepa::IMPC()
{
	linha_mepa++;
	mepa << "IMPC" << endl;
	ultimo_comando = "IMPC";
}

void C_Mepa::IMPC(string _c)
{
	mepa << "; ----------Inicio Impressao" << endl;
	// +1 e -1 para eliminar o "
	for (auto it = _c.begin() + 1; it != _c.end() - 1; it++)
	{
		linha_mepa++;
		mepa << "CRCT \'" << string(1, *it) << "\'" << endl;
		linha_mepa++;
		mepa << "IMPC" << endl;
		ultimo_comando = "IMPC";
	}
	mepa << "; ----------FIM Impressao" << endl;
}

void C_Mepa::IMPE()
{
	linha_mepa++;
	mepa << "IMPE" << endl;
	ultimo_comando = "IMPE";
}

void C_Mepa::SOMA()
{
	linha_mepa++;
	mepa << "SOMA" << endl;
	ultimo_comando = "SOMA";
}

void C_Mepa::SUBT()
{
	linha_mepa++;
	mepa << "SUBT" << endl;
	ultimo_comando = "SUBT";
}

void C_Mepa::MULT()
{
	linha_mepa++;
	mepa << "MULT" << endl;
	ultimo_comando = "MULT";
}

void C_Mepa::DIVI()
{
	linha_mepa++;
	mepa << "DIVI" << endl;
	ultimo_comando = "DIVI";
}

void C_Mepa::MODI()
{
	linha_mepa++;
	mepa << "MODI" << endl;
	ultimo_comando = "MODI";
}

void C_Mepa::CONJ()
{
	linha_mepa++;
	mepa << "CONJ" << endl;
	ultimo_comando = "CONJ";
}

void C_Mepa::DISJ()
{
	linha_mepa++;
	mepa << "DISJ" << endl;
	ultimo_comando = "DISJ";
}

void C_Mepa::NEGA()
{
	linha_mepa++;
	mepa << "NEGA" << endl;
	ultimo_comando = "NEGA";
}

void C_Mepa::CMME()
{
	linha_mepa++;
	mepa << "CMME" << endl;
	ultimo_comando = "CMME";
}

void C_Mepa::CMMA()
{
	linha_mepa++;
	mepa << "CMMA" << endl;
	ultimo_comando = "CMMA";
}

void C_Mepa::CMIG()
{
	linha_mepa++;
	mepa << "CMIG" << endl;
	ultimo_comando = "CMIG";
}

void C_Mepa::CMDG()
{
	linha_mepa++;
	mepa << "CMDG" << endl;
	ultimo_comando = "CMDG";
}

void C_Mepa::CMAG()
{
	linha_mepa++;
	mepa << "CMAG" << endl;
	ultimo_comando = "CMAG";
}

void C_Mepa::CMEG()
{
	linha_mepa++;
	mepa << "CMEG" << endl;
	ultimo_comando = "CMEG";
}

void C_Mepa::INRV()
{
	linha_mepa++;
	mepa << "INRV" << endl;
	ultimo_comando = "INRV";
}

string C_Mepa::DSVF()
{
	linha_mepa++;
	mepa << "DSVF DSVF" << linha_mepa  << endl;
	ultimo_comando = "DSVF";
	return "DSVF" + to_string(linha_mepa);
}

string C_Mepa::DSVS()
{
	linha_mepa++;
	mepa << "DSVS DSVS" << linha_mepa << endl;
	ultimo_comando = "DSVS";
	return "DSVS" + to_string(linha_mepa);
}

void C_Mepa::DSVS(string _str)
{
	linha_mepa++;
	mepa << "DSVS " << _str << endl;
	ultimo_comando = "DSVS";
}

void C_Mepa::NADA(string _str)
{
	linha_mepa++;
	mepa << _str << ":NADA" << endl;
	ultimo_comando = "NADA";
}

string C_Mepa::NADA()
{
	string rotulo;
	linha_mepa++;
	rotulo = "R" + to_string(linha_mepa);
	mepa << "; ---------- Entrada Rotulo " << rotulo << endl;
	mepa << rotulo << ":NADA" << endl;
	ultimo_comando = "NADA";
	return rotulo;
}

void C_Mepa::CHPR(string _p, string _k)
{
	mepa << "; ---------- Chama Procedimento/Funcao - " << _p << endl;
	linha_mepa++;
	mepa << "CHPR " << _p  << ","<< _k << endl;
	ultimo_comando = "CHPR";
}

void C_Mepa::ENPR(string _k)
{
	linha_mepa++;
	mepa << "; ---------- Entrada Procedimento/Funcao" << endl;
	mepa << "ENPR " << _k << endl;
	ultimo_comando = "ENPR";
}

void C_Mepa::RTPR(string _k, string _n)
{
	linha_mepa++;
	mepa << "RTPR " << _k << "," << _n << endl;
	mepa << "; ---------- Fim/Retorno de Procedimento/Funcao" << endl;
	ultimo_comando = "RTPR";
}

void C_Mepa::INPP()
{
	linha_mepa++;
	mepa << "INPP" << endl;
	ultimo_comando = "INPP";
}

void C_Mepa::PARA()
{
	linha_mepa++;
	mepa << "PARA" << endl;
	ultimo_comando = "PARA";
}

void C_Mepa::AMEM(string _m)
{
	if (stoi(_m) > 0)
	{
		mepa << "; ---------- Alocacao de Memoria (" << _m << ")" << endl;
		linha_mepa++;
		mepa << "AMEM " << _m << endl;
		ultimo_comando = "AMEM";
	}
}

void C_Mepa::DMEM(int _m)
{
	if (_m > 0)
	{
		mepa << "; ---------- Desalocacao de Memoria (" << _m << ")" << endl;
		linha_mepa++;
		mepa << "DMEM " << to_string(_m) << endl;
		ultimo_comando = "DMEM";
	}
}

void C_Mepa::Add_Comando(string _str)
{
	linha_mepa++;
	mepa << _str << endl;
	ultimo_comando = _str;
}

void C_Mepa::Avaliar_Expressao(stack<S_EXP>& _p, const C_Tabela_Simbolos& _ts)
{
	Converter_Pos_Fixa(_p, _ts);
}

void C_Mepa::Gerar_Arquivo(string _nome_arquivo)
{
	fstream arquivo;
	stringstream ss;
	string nome_arquivo_final;

	ss << Cabelho();
	//Conteúdo MEPA
	ss << mepa.str();
	ss << Rodape();

	//Remover a extensão .chi do arquivo
	nome_arquivo_final = _nome_arquivo.substr(0, _nome_arquivo.size() - 4);

	//Adicionar informações ao nome do arquivo e extensão .txt
	nome_arquivo_final = nome_arquivo_final + ".mep";

	arquivo.open(nome_arquivo_final, ios::out | ios::trunc);
	arquivo << ss.str();
	arquivo.close();

	cout << "Arquivo \"" << nome_arquivo_final << "\" gerado" << endl;
}
