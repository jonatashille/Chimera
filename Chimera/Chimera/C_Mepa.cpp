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

	return ss.str();
}

string C_Mepa::Rodape()
{
	stringstream ss;
	ss << "; ----------Fim das Instrucoes MEPA" << endl;
	ss << "inicio  ENDP" << endl;
	ss << "codigo  ENDS" << endl;
	ss << "END    inicio" << endl;
	return ss.str();
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

void C_Mepa::CRVL(string _k, string _n)
{
	mepa << "CRVL " << _k << "," << _n << endl;
}

void C_Mepa::LEIT()
{
	mepa << "LEIT" << endl;
}

void C_Mepa::IMPR()
{
	mepa << "IMPR" << endl;
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

void C_Mepa::Gerar_Arquivo(string _nome_arquivo)
{
	fstream arquivo;
	stringstream ss;

	ss << Cabelho();
	//Conte�do MEPA
	ss << mepa.str();
	ss << Rodape();

	arquivo.open(_nome_arquivo, ios::out | ios::trunc);
	arquivo << ss.str();
	arquivo.close();
}