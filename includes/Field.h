#ifndef _FIELD__
#define _FIELD__

/*
	classe field responsavel por pegar os parametros da criacao da tabela para serem inseridos no arquivo.td
*/
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include "TDFile.h"

using namespace std;

class Field {

	private:
		char *name;
		int type;
		int length;
	
	public:
		
		Field() {
			this->name = NULL;
		}
		~Field() {
			if(name != NULL)
				delete[] name;
		}

		void setname(char *name) { 
			this->name = new char[strlen(name)+1]();
			strcpy(this->name, name); 
		}
		void settype(int type) { this->type = type; }
		void setlength(int length) { this->length = length; }
		
		char *getname() { return this->name; }
		int gettype() { return this->type; }
		int getlength() { return this->length; }

		//escreve field no arquivo .td
		void write(TDFile *tdpointer) {
			tdpointer->writeField(this->name,this->type,this->length);
		}
		//le field do arquivo .td
		void read(TDFile *tdpointer) {
			this->name = tdpointer->readField(&this->type,&this->length);
		}
};
#endif
