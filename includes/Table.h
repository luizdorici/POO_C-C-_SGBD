#ifndef _TABLE__
#define _TABLE__

#include <iostream>

#include "myvector.h"
#include "mystring.h"
#include "Field.h"
#include "TDFile.h"
#include "DatFile.h"
#include "Relation.h"

using namespace std;

class Table : public Relation {

	private:
		TDFile *tdpointer;  //ponteiro para arquivo nome_da_tabela.td
		DatFile *datpointer; //ponteiro para arquivo nome_da_tabela.dat
		Field **fields; // fields desta tabela
		int nFields; //numero de fields nesta tabela
		int *sortFields; // vetor com a ordem correta dos fields
		int *fieldsize; // vetor com o tamanho que cada field deve ocupar
		int *fieldtype; // vetor com o tipo de cada field
		Vector<String *> *registro; //vetor de strings que guarda os fields do registro a serem colocados no arquivo (.dat)
		

	public:
		
		Table(String *name) : Relation(name) {

			this->sortFields = NULL;
			int size = strlen(name->toCharArray());

			//criar o arquivo td com o nome do arquivo .td
			char *newname = (char*) malloc(sizeof(char) *size+1);
			strcpy(newname,name->toCharArray());
			strcat(newname,".td");

			this->tdpointer = new TDFile(newname);
			free(newname);

			//cria o arquivo dat com o nome do arquivo .dat
			newname = (char*) malloc(sizeof(char) *size+1);
			strcpy(newname,name->toCharArray());
			strcat(newname,".dat");

			this->datpointer = new DatFile(newname);
			free(newname);

			this->nFields = 0;

			this->fields = NULL;
		}
/*
	funcao que pega o nome da tabela
	usada para fazer busca por tabelas
	no banco de dados, com isso eh possivel
	saber se uma tabela existe ou nao neste database
*/
		virtual char *getName() {
			int size = strlen(name->toCharArray());
			char *newname = new char[size+1]();
			strcpy(newname,this->name->toCharArray());
			return newname;
		}
		//retorna os fields(file.td) da tabela para trabalhar no index, no select e no delete
		// para saber a forma como ler os dados dos registros
		Field **getFields() { return this->fields; }
		//retorna o numero de fields na tabela
		int getNfields() { return this->nFields; }

		//verifica se um campo existe na tabela
		bool CheckField(char *name,int pos) {
			int i;
			if(pos == 0) {
				this->fieldtype = new int[this->nFields]();
				this->sortFields = new int[this->nFields]();
				this->fieldsize = new int[this->nFields]();
			}
			for(i=0; i < nFields; i++) {

				if(strcmp(this->fields[i]->getname(),name) == 0) {
					//este vetor vai ter o tamanho de cada campo
					// se o campo for char ou blob ele pode variar de tamanho
					//caso contrario ele vai ser sempre 0 pois existe um unico int ou double
					//caso seja um unico char tbm tera o valor 0(nao deve ser concatenado nenhum espaco adicional)
					this->fieldsize[pos] = this->fields[i]->getlength();
					// vetor com os tipos de cada campo para depois imprimir corretamente no arquivo
					this->fieldtype[pos] = this->fields[i]->gettype();
					//vetor com as posicoes corregas para cada field
					//caso alguma posicao tenha o valor 0 entao o field foi duplicado
					//caso contrario este vetor indica as posicoes corretas de cada field
					this->sortFields[pos] = i+1;
					return true;
				}
			}
			return false;
		}	

		//verifica se o campo foi duplicado
		bool isDuplicate() {
			int i;
			for(i=0; i<this->nFields; ++i)
				if(this->sortFields[i] == 0)
					return true;
			return false;	
		}

		//em caso de create table(o outro caso possivel seria o insert)
		void create_files() {
			this->tdpointer->create();
			this->datpointer->create();
		}
/*
	carrega os fields do arquivo .td ja existente
*/
		void LoadTdFile() {
			Field *takefield;
			this->tdpointer->open(true,false,false); //abre o arquivo .td apenas para leitura
			do {
				takefield = NULL;
				takefield = new Field();
				takefield->read(tdpointer);
				if(takefield->getname() != NULL) // condicao de parada indica que nao existem mais fields para ler
					addField(takefield, true);
				else break;
			}while(1);
			this->tdpointer->close();
		}
/*
	adiciona um field a tabela
	obs: esta funcao ainda nao adiciona novos fields
	a uma tabela ja existente, apenas adiciona fields
	a uma tabela recem criada...
*/
		void addField(Field *field,bool load) {
			this->fields = (Field**) realloc(this->fields,sizeof(Field*)*nFields+1);
			nFields++;
			this->fields[nFields-1] = field;
			if(!load) {
				tdpointer->open(false,true,true); // abre o arquivo .td para escrita no final "ab"
				field->write(tdpointer);	  // apenas se for para escrita
				tdpointer->close();
			}
		}

		void removeField() {
		}		
		//ordena o registro a ser inserido no arquivo (.dat)
		void insertReg() {
			int i,j;
			Vector<String *> *sorted = new Vector<String *>();
			int *sortedtypes = new int[this->nFields]();

			for(i=0; i < this->nFields; i++) {
				for(j=0; j < this->nFields; j++) {
					if(this->sortFields[j] == i+1) {
						sorted->add(this->registro->get(j));
						sortedtypes[i] = this->fieldtype[j];
					}
				}
			}

			// imprime na tela o registro a ser inserido
			// e insere no arquivo (.dat)
			datpointer->open(false,true,true);

			String *aux;
			aux = sorted->get(0);
			cout << aux->toCharArray();
			this->datpointer->writeReg(aux->toCharArray(),sortedtypes[0]);

			for(i=1 ;i < this->nFields; i++) {
				aux = sorted->get(i);
				this->datpointer->writeReg(aux->toCharArray(),sortedtypes[i]);
				cout << "|" << aux->toCharArray();
			}
			cout << endl;

			datpointer->close();

			aux = NULL;
			delete sorted;
			delete[] sortedtypes;
		}
		
		// retorna o numero do campo na tabela e o tamanho em bytes de cada registro
		// o tamanho do registro foi calculado da forma como descrita no trabalho 2
		// nao significa que o registro tem esse tamanho ja que um int por exemplo
		// tem 4 bytes e nao 1(porem no PDF do trabalho foi contabilizado como 1)
		int Util(char *campo,int *size) {
			int i;
			int aux=0;
			
			//calcula a quantidade em bytes de cada registro
			for(i=0; i< this->nFields; i++) {
				aux = this->fields[i]->getlength();
				if(aux == 0)
					aux = 1;

				*size += aux;
			}

			//retorna a posicao do campo caso ele exista e -1 caso contrario
			for(i=0; i< this->nFields; i++) {
				if(strcmp(campo,this->fields[i]->getname()) == 0)
					return i;
			}
			return -1;
		}
/*
	remocao logica dos registros selecionados
*/		
		void deleteReg() {

		}
				
		//update()
/*
	seleciona determinados campos de um determinado grupo de registros
	imprime na tela os campos selecionados dos registros
*/
		void select(char *campos) {
			char *aux = new char[2];
			strcpy(aux,"*");
			//verifica se o campo do comando foi *
			//(o que significa que todos os campos devem ser impressos)
			if(strcmp(aux,campos) == 0) {
			
			}else {}

			delete[] aux;
		}

		void getInformation() {}

/*
	funcao que imprime os parametros do insert na tela e escreve no arquivo .dat(ainda nao implementado) caso ele exista
*/
		void setRegister(char *field, int counter) {
			char *clonefield = NULL;
			int i,j=1;
			int nspaces = 0;

			if(counter == 1)
				this->registro = new Vector<String *>();
			
			if(this->fieldsize[counter-1] > 0) {
				//calcula o numero de espacos para concatenar
				nspaces = (this->fieldsize[counter-1]) - (strlen(field));
				//cria backup da variavel field para modificar
				clonefield = new char[strlen(field)+1]();
				strcpy(clonefield,field);
				int fieldsize = strlen(clonefield);

				clonefield = (char*) realloc (clonefield,(sizeof(char)*fieldsize+nspaces+1));
				for(i=0 ;i < nspaces; i++) {
					clonefield[fieldsize+i]=32;// concatena espaco
				}
				clonefield[strlen(clonefield)+i] = '\0';

				this->registro->add(new String(clonefield));
			}
			else {
				this->registro->add(new String(field));
			}
			if(clonefield != NULL)
				delete[] clonefield;
		}

		virtual ~Table() {
			int i;
			if(this->registro != NULL)
				delete this->registro;
			if(this->fieldtype != NULL)
				delete[] this->fieldtype;
			if(this->fieldsize != NULL)
				delete[] this->fieldsize;
			if(this->sortFields != NULL)
				delete[] this->sortFields;
			for(i=0 ;i < this->nFields; ++i)
				free(this->fields[i]);
			if(this->fields != NULL)
				free(this->fields);
		}
};
#endif
