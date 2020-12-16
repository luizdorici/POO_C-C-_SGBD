#ifndef _SHELL__
#define _SHELL__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pcre.h>
#include <sys/stat.h>
#include <iostream>

#include <myvector.h>
#include <mystring.h>
#include "Index.h"
#include "Field.h"
#include "Table.h"
#include "DataBase.h"

using namespace std;

class Shell {

	private:
		int BUFFER_SIZE;
		char *buffer;
		int counter;
		DataBase *runningData; //banco de dados que esta sendo usado
		char *BASEDIR; //caminho do diretorio raiz do SGBD
		bool usingData; //true caso banco de dados selecionado, false caso contrario

	public:
/*
	retorna os matchs da expressao regular como vetor de strings
*/
		Vector<String *> *match(const char *command, const char *pattern, bool *matchResult);
/*
	quebra comandos diferentes
*/
		char* nextCommand(FILE *stream);
/*
	cria a tabela e seus arquivos .td e .dat (migrando para nova plataforma)
*/
		bool isCreateTable(const char *command);
/*
	parcialmente implementado(quase terminado,ja possui todas as informacoes necessarias para criacao)
*/
		bool isCreateIndex(const char *command);
/*
	valida o comando create database, cria o folder do banco de dados
	como o bando de dados criado nao esta necessariamente em uso
	ele nao mantem um ponteiro para o objeto do database criado
*/
		bool isCreateDatabase(const char *command);
/*
	valida o comando insert e printa na tela os parametros a serem inseridos na tabela, caso estejam corretos
	parcialmente implementado (migrando para nova plataforma)
*/
		bool isInsert(const char *command);

/*
	valida o comando use, ativa o bando de dados como running database caso ele exista
	caso nao exista simplesmente imprime na tela a menssagem de erro
*/
		bool isUseDatabase(const char *command);

/*
	parcialmente implementado
*/
		bool isUpdateIndex(const char *command);
/*
	verifica comando de saida do shell para sair do shell no caso de dar match
*/
/*
	faz a validacao do comando select, verifica se a tabela existe,
	caso exista, pega os campos indicados no comando e joga para o select
	do objeto table buscar e imprimir na tela os campos indicados no comando
*/
		bool isSelect(const char *command);
/*
	ainda nao implementado
*/
		bool isDelete(const char *command);

		// fecha o shell do usuario
		bool isExit(const char *command);

		Shell();

		void commandLine();

		virtual~Shell();
};
#endif
