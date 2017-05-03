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
void C_Mepa::Converter_Pos_Fixa(stack<string>& _p, C_Tabela_Simbolos _ts)
{
	//Preciso reverter a pilha para pegar da posição certa
	stack<string> r_stack;
	while (!_p.empty())
	{
		r_stack.push(_p.top());
		_p.pop();
	}
	_p = r_stack;

	stack<string> p_temp;

	while (!_p.empty())
	{
		if (E_Operador(_p.top()))
		{
			while (!p_temp.empty() && p_temp.top() != "(" && Get_Peso_Operador(p_temp.top()) >= Get_Peso_Operador(_p.top()))
			{
				Add_Comando(p_temp.top());
				p_temp.pop();
			}
			p_temp.push(_p.top());
			_p.pop();
		}
		else if (_p.top() == "(")
		{
			p_temp.push(_p.top());
			_p.pop();
		}
		else if (_p.top() == ")")
		{
			while (!p_temp.empty() && p_temp.top() != "(")
			{
				Add_Comando(p_temp.top());
				p_temp.pop();
			}
			p_temp.pop();
		}
		//Caso seja operando, empilho na pos_fixa e desempilho da expressão
		else
		{
			//Se for identificador, preciso inserir o endereço da memória dele
			if (Validar_Identificador(_p.top()))
				CRVL("1", to_string(_ts.Buscar_Pos_Pilha(_p.top())));
			else
				CRCT(_p.top());
			_p.pop();
		}
	}

	while (!p_temp.empty())
	{
		Add_Comando(p_temp.top());
		p_temp.pop();
	}
}

//Função que verifica se um caracter é um simbolo operador ou não
bool C_Mepa::E_Operador(string _str)
{
	if (_str == "SOMA" || _str == "SUBT" || _str == "MULT" || _str == "DIVI" || _str == "%")
		return true;

	return false;
}

//Função que retorna o peso do operador. Operador com o peso mais alto tem maior precedência
int C_Mepa::Get_Peso_Operador(string _str)
{
	if (_str == "SOMA" || _str == "SUBT")
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
}


C_Mepa::~C_Mepa()
{
}

void C_Mepa::CRCT(string _c)
{
	mepa << "CRCT " << _c << endl;
}

void C_Mepa::CRCT_String(string _c)
{
	// +1 e -1 para eliminar o "
	for (auto it = _c.begin() + 1; it != _c.end() - 1; it++)
	{
		mepa << "CRCT \'" << string(1, *it) << "\'" << endl;
	}
}

void C_Mepa::CRVL(string _k, string _n)
{
	mepa << "CRVL " << _k << "," << _n << endl;
}

void C_Mepa::ARMZ(string _k, string _n)
{
	mepa << "ARMZ " << _k << "," << _n << endl;
}

void C_Mepa::ARMZ(string _n)
{
	mepa << "ARMZ " << 1 << "," << _n << endl;
}

void C_Mepa::LEIT()
{
	mepa << "LEIT" << endl;
}

void C_Mepa::LECH()
{
	mepa << "LECH" << endl;
}

void C_Mepa::IMPR()
{
	mepa << "IMPR" << endl;
}

void C_Mepa::IMPC(string _c)
{
	// +1 e -1 para eliminar o "
	for (auto it = _c.begin() + 1; it != _c.end() - 1; it++)
	{
		mepa << "CRCT \'" << string(1, *it) << "\'" << endl;
		mepa << "IMPC" << endl;
	}
}

void C_Mepa::IMPE()
{
	mepa << "IMPE" << endl;
}

void C_Mepa::SOMA()
{
	mepa << "SOMA" << endl;
}

void C_Mepa::SUBT()
{
	mepa << "SUBT" << endl;
}

void C_Mepa::MULT()
{
	mepa << "MULT" << endl;
}

void C_Mepa::DIVI()
{
	mepa << "DIVI" << endl;
}

void C_Mepa::MODI()
{
	mepa << "MODI" << endl;
}

void C_Mepa::CONJ()
{
	mepa << "CONJ" << endl;
}

void C_Mepa::DISJ()
{
	mepa << "DISJ" << endl;
}

void C_Mepa::NEGA()
{
	mepa << "NEGA" << endl;
}

void C_Mepa::CMME()
{
	mepa << "CMME" << endl;
}

void C_Mepa::CMMA()
{
	mepa << "CMMA" << endl;
}

void C_Mepa::CMIG()
{
	mepa << "CMIG" << endl;
}

void C_Mepa::CMDG()
{
	mepa << "CMDG" << endl;
}

void C_Mepa::CMAG()
{
	mepa << "CMAG" << endl;
}

void C_Mepa::CMEG()
{
	mepa << "CMEG" << endl;
}

void C_Mepa::INRV()
{
	mepa << "INRV" << endl;
}

void C_Mepa::DSVF(string _p)
{
	mepa << "DSVF " << _p << endl;
}

void C_Mepa::DSVS(string _p)
{
	mepa << "DSVS " << _p << endl;
}

void C_Mepa::NADA()
{
	mepa << "NADA" << endl;
}

void C_Mepa::CHPR(string _p, string _k)
{
	mepa << "CHPR " << _p  << ","<< _k << endl;
}

void C_Mepa::ENPR(string _k)
{
	mepa << "ENPR " << _k << endl;
}

void C_Mepa::RTPR(string _k, string _n)
{
	mepa << "RTPR " << _k << "," << _n << endl;
}

void C_Mepa::INPP()
{
	mepa << "INPP" << endl;
}

void C_Mepa::PARA()
{
	mepa << "PARA" << endl;
}

void C_Mepa::AMEM(string _m)
{
	mepa << "AMEM " << _m << endl;
}

void C_Mepa::DMEM(string _m)
{
	mepa << "DMEM " << _m << endl;
}

void C_Mepa::Add_Comando(string _str)
{
	mepa << _str << endl;
}

void C_Mepa::Avaliar_Expressao(stack<string>& _p, const C_Tabela_Simbolos& _ts)
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
