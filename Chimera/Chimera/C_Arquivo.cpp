#include "stdafx.cpp"
#include "C_Arquivo.h"

/****************************************************************************************
Objetivo >> Construir um objeto Arquivo.

Observações >>

Parâmetros >> _arquivo = nome do arquivo a ser aberto

Criador >> Jônatas Alexandre Hille - 28/04/2013
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

Observações >>

Criador >> Jônatas Alexandre Hille - 28/04/2013
*****************************************************************************************/
C_Arquivo::~C_Arquivo()
{
	arquivo.close();
}

/****************************************************************************************
Objetivo >> Buscar a próxima linha do arquivo.

Observações >> A linha do arquivo é gravada na propriedade linha, que poderá ter seu valor
valor resgatado através da função getLinha(). Caso não exista próxima linha,
retorna false

Criador >> Jônatas Alexandre Hille - 28/04/2013
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

Observações >> Retorna a linha que a função ProximaLinha() atribuiu ao atributo linha

Criador >> Jônatas Alexandre Hille - 28/04/2013
*****************************************************************************************/
const string C_Arquivo::getLinha() const
{
	return linha;
}

/****************************************************************************************
Objetivo >> Retorna o número da linha do arquivo.

Observações >> Retorna o número da linha que a função ProximaLinha() incrementou.

Criador >> Jônatas Alexandre Hille - 15/05/2016
*****************************************************************************************/
const int C_Arquivo::getNumeroLinha() const
{
	return numLinha;
}

/****************************************************************************************
Objetivo >> Retorna todo o conteúdo do arquivo.

Observações >>

Criador >> Jônatas Alexandre Hille - 13/03/2016
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
