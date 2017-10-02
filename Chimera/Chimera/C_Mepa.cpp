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
			// TODO 04 - Verificar caso 1==1 + 3 * 5 tem ques er igual a 0 e é igual a 16
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
					CRVI(to_string(pai), to_string(_ts.Buscar_Pos_Pilha(sidpai.make_Id_Pai(_p.top().token, _p.top().parente))));
				else
					CRVL(to_string(pai), to_string(_ts.Buscar_Pos_Pilha(sidpai.make_Id_Pai(_p.top().token, _p.top().parente))));
			}
			else
				CRCT(_p.top().token);
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

void C_Mepa::CRCT_String(string _c, int _pos, int _limite)
{
	// +1 e -1 para eliminar o "
	for (auto it = _c.begin() + 1; it != _c.end() - 1 && _pos < _limite; it++)
	{
		linha_mepa++;
		mepa << "CRCT \'" << string(1, *it) << "\'" << endl;
		ARMZ(to_string(_pos));
		_pos++;
		ultimo_comando = "CRCT";
	}
}

void C_Mepa::CRVL(string _k, string _n)
{
	linha_mepa++;
	mepa << "CRVL " << _k << "," << _n << endl;
	ultimo_comando = "CRVL";
}

void C_Mepa::CRVL_String(string _k, int _pos_ini, int _tam_str)
{
	for (int i = 1; i <= _tam_str; i++)
	{
		linha_mepa++;
		mepa << "CRVL " << _k << "," << _pos_ini << endl;
		IMPC();
		_pos_ini++;
	}
}

void C_Mepa::CRVI(string _k, string _n)
{
	linha_mepa++;
	mepa << "CRVI " << _k << "," << _n << endl;
	ultimo_comando = "CRVI";
}

void C_Mepa::CRVI_String(string _k, int _pos_ini, int _pos_fim)
{
	linha_mepa++;
	mepa << "CRVI " << _k << "," << _pos_ini << endl;
	IMPC();
}

void C_Mepa::CREN(string _k, string _n)
{
	linha_mepa++;
	mepa << "CREN " << _k << "," << _n << endl;
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
	// +1 e -1 para eliminar o "
	for (auto it = _c.begin() + 1; it != _c.end() - 1; it++)
	{
		linha_mepa++;
		mepa << "CRCT \'" << string(1, *it) << "\'" << endl;
		linha_mepa++;
		mepa << "IMPC" << endl;
		ultimo_comando = "IMPC";
	}
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
	mepa << rotulo << ":NADA" << endl;
	ultimo_comando = "NADA";
	return rotulo;
}

void C_Mepa::CHPR(string _p, string _k)
{
	linha_mepa++;
	mepa << "CHPR " << _p  << ","<< _k << endl;
	ultimo_comando = "CHPR";
}

void C_Mepa::ENPR(string _k)
{
	linha_mepa++;
	mepa << "ENPR " << _k << endl;
	ultimo_comando = "ENPR";
}

void C_Mepa::RTPR(string _k, string _n)
{
	linha_mepa++;
	mepa << "RTPR " << _k << "," << _n << endl;
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
	linha_mepa++;
	mepa << "AMEM " << _m << endl;
	ultimo_comando = "AMEM";
}

void C_Mepa::DMEM(int _m)
{
	if (_m > 0)
	{
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
