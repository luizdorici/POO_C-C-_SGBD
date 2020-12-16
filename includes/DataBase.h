#ifndef _DATABASE__
#define _DATABASE__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>

#include "myvector.h"
#include "mystring.h"
#include "Index.h"
#include "Field.h"
#include "Table.h"
#include "Folder.h"
#include "TDFile.h"
#include "DatFile.h"
#include "IDXFile.h"
#include "FileSystemEntry.h"

using namespace std;

class DataBase {

	private:
		
		char *dataname; //nome do banco de dados
		Table **tables; //vetor de tabelas no database que esta rodando
		Index **IDXlist; //vetor de index(possui os index de um database)
		int nIDX;//numero total de index na memoria(vetor de index)
		int nTables;  //numero de tabelas no vetor
		Folder *dataFolder; //ponteiro para o folder deste database
		

	public:

		DataBase(char *dataname);

		// cria folder do banco de dados
		int CreateFolder(char* BASEDIR);
/*
	cria os fields com os valores a serem inseridos no arquivo nome_da_tabela.td
*/
		void setFields(int counter,char *field);

		//imprime os parametros a serem inseridos no arquivo.td na tela
		void printFields(int counter,char *field);

/*
	o objetivo dessa funçao eh retornar uma tabela caso ela esteja nas tableas do running database
	caso ela nao esteja entre as tabelas do running database e ela exista entao ela e criada,
	adicionada nas tabelas do running database e retornada para ser usada no insert table
	(isso acontece nos casos onde o usuario fecha o shell e depois entra e tenta usar
		uma tabela que ele tinha criado anteriormente em um database ja existente,
		no caso esta table nao estaria na memoria entao ela precisa ser carregada)
*/
		Table *getTableToWork(String *tablename);
/*
	cria a tabela, caso bool create seja true esta funcao tbm cria os arquivos .dat e .td
	bool create sempre sera true quando o comando for create table e sera false caso seja um insert
	como o comando create table ja testa se a tabela existe antes de executar, entao nao existe
	a possibilidade de um mesmo arquivo ser criado duas vezes...
*/

		void create_table(String *tablename,bool create);
/*
	busca na lista de index do database um index para trabalhar, caso ele nao exista cria o objeto e retorna este
*/

		Index *getIDXtoWork(char *name, String *tablename);
/*
	cria o index caso a variavel create seja true, cria o arquivo de index, caso seja false nao cria o arquivo,
	apenas carrega um ja existente
*/

		Index *create_index(char *IDXName,String *relationName,bool create);

		//backup
		//restore

		virtual~DataBase();
};
#endif
