#ifndef _INDEX_H_
#define _INDEX_H_

#include "Relation.h"
#include "IDXFile.h"
#include "Table.h"
#include "IDXField.h"
#include "Field.h"

class Index : public Relation {
	protected:
		char *name;
		String *tabRelation;
		IDXFile *IDXpointer;
		char *fieldname;
	public:
		Index(char *name,String *tableName) : Relation(tableName) {

			this->name = name;
			this->tabRelation = tableName;
			this->IDXpointer = new IDXFile(name);
		}

		char *getName() {
			return this->name;
		}

		// busca os campos na tabela, calcula o byteoffset e escreve no index
		void writeIndex(Table *WorkingTable,int size,char *fieldname, int pos) {
			// pega vetor de fields da tabela a ser criado o index para trabalhar
			Field **WorkingFields = WorkingTable->getFields();
	
			//seleciona a forma de armazenamento do index, int ou char para selecao do tipo da template
			if(WorkingFields[pos]->gettype() == 2)
				IDXField<int*> **indexVector; // vetor de index na memoria primaria
			else if(WorkingFields[pos]->gettype() == 1) 
				IDXField<char*> **indexVector;
		}

		//atualiza o index ja existente a partir de um arquivo de registro modificado(ou nao)
		void UpdateIndex() {}

		// funcao que irah carregar um index na memoria para update ou buscas
		void LoadIDXFile() {
		/*	Index *takeIDX;
			this->IDXpointer->open(true,false,false); //abre o arquivo .td apenas para leitura
			do {
				takeIDX = NULL;
				takeIDX = new Index();
				takeIDX->read(IDXpointer);
				if(takeIDX->getname() != NULL) // condicao de parada indica que nao existem mais fields para ler
					addField(takefield, true);
				else break;
			}while(1);
			this->tdpointer->close();*/
		}

		virtual~Index() {
		
		}

		void create_file() {
			this->IDXpointer->create();
		}

		void getInformation() {}
};
#endif
