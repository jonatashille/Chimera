#include "stdafx.cpp"
#include "C_Arquivo.h"

C_Arquivo::C_Arquivo()
{
}

/****************************************************************************************
Objetivo >> Construir um objeto Arquivo.

Observa��es >>

Par�metros >> _arquivo = nome do arquivo a ser aberto

Criador >> J�natas Alexandre Hille - 28/04/2013
*****************************************************************************************/
C_Arquivo::C_Arquivo(const string& _arquivo)
{
	arquivo.open(_arquivo);
	if (!arquivo.is_open())
	{
		cout << "ERRO: Nome do Arquivo Invalido" << endl;
		system("pause");
		exit(1);
	}
	numLinha = 0;
}

/****************************************************************************************
Objetivo >> Destruir um objeto Arquivo.

Observa��es >>

Criador >> J�natas Alexandre Hille - 28/04/2013
*****************************************************************************************/
C_Arquivo::~C_Arquivo()
{
	arquivo.close();
}

/****************************************************************************************
Objetivo >> Buscar a pr�xima linha do arquivo.

Observa��es >> A linha do arquivo � gravada na propriedade linha, que poder� ter seu valor
valor resgatado atrav�s da fun��o getLinha(). Caso n�o exista pr�xima linha,
retorna false

Criador >> J�natas Alexandre Hille - 28/04/2013
*****************************************************************************************/
bool C_Arquivo::ProximaLinha()
{
	getline(arquivo, linha);
	//Somo numLinha para cada linha do arquivo
	if (arquivo.good())
		numLinha++;

	return arquivo.good();
}

/****************************************************************************************
Objetivo >> Retorna a linha do arquivo.

Observa��es >> Retorna a linha que a fun��o ProximaLinha() atribuiu ao atributo linha

Criador >> J�natas Alexandre Hille - 28/04/2013
*****************************************************************************************/
const string C_Arquivo::getLinha() const
{
	return linha;
}

/****************************************************************************************
Objetivo >> Retorna o n�mero da linha do arquivo.

Observa��es >> Retorna o n�mero da linha que a fun��o ProximaLinha() incrementou.

Criador >> J�natas Alexandre Hille - 15/05/2016
*****************************************************************************************/
const int C_Arquivo::getNumeroLinha() const
{
	return numLinha;
}

/****************************************************************************************
Objetivo >> Retorna todo o conte�do do arquivo.

Observa��es >>

Criador >> J�natas Alexandre Hille - 13/03/2016
*****************************************************************************************/
const string C_Arquivo::getConteudoArquivo()
{
	stringstream conteudoArquivo;

	while (ProximaLinha())
	{
		conteudoArquivo << getLinha() << '\n';
	}

	return conteudoArquivo.str();
}

bool C_Arquivo::CompararArquivos(const string & _p1, const string & _p2)
{
	ifstream f1(_p1, ifstream::binary | ifstream::ate);
	ifstream f2(_p2, ifstream::binary | ifstream::ate);

	if (f1.fail() || f2.fail()) {
		return false; //Problema no arquivo
	}

	if (f1.tellg() != f2.tellg()) {
		return false; //Tamanhos diferentes
	}

	//Retorna ao inicio do arquivo e utiliza o std::equal para comparar o conteudo dos arquivos.
	f1.seekg(0, ifstream::beg);
	f2.seekg(0, ifstream::beg);
	return equal(istreambuf_iterator<char>(f1.rdbuf()),
		istreambuf_iterator<char>(),
		istreambuf_iterator<char>(f2.rdbuf()));
}
