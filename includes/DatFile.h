#ifndef _DATFILE_H_
#define _DATFILE_H_

#include "File.h"
#include <math.h>

class DatFile : public File {
	private:
	protected:
	public:
		DatFile() : File() {}
		DatFile(char *name) : File(name) {}
		DatFile(String *name) : File(name) {}
		virtual ~DatFile() {}

		unsigned char *readBytes() {
			unsigned char *buffer = NULL;

			if (!this->fstream) {
				buffer = new unsigned char[1];
				fread(&buffer, 1, 
					sizeof(unsigned char), 
						this->fstream);
			}

			return buffer;
		}
		unsigned char *readBytes(int length) {
			unsigned char *buffer = NULL;

			if (!this->fstream) {
				buffer = new unsigned char[length];
				fread(buffer, length, 
					sizeof(unsigned char), 
						this->fstream);
			}

			return buffer;
		}

		void create() {
			File::create();
		}

		bool writeBytes(unsigned char *buffer,int length) {
			return fwrite(buffer, length, sizeof(unsigned char), this->fstream) == length;
		}
		
		bool writeReg(char* campo, int tipo) {
			int size = strlen(campo);

			//escreve em char (tipo char, bool e blob)
			if(tipo == 1 || tipo == 4 || tipo == 5) {
				int size = strlen(campo);
				// antes de colocar o campo do tipo char no registro
				// calcula o tamanho do campo e escreve um int
				// para saber o tamanho do campo quando for ler do arquivo
				fwrite(&size,sizeof(int),1,this->fstream);
				fwrite(campo,sizeof(char),size,this->fstream);
			}
			//escreve em int tipo int
			if(tipo == 2) {
				int j = 0,i;
				int result = 0,aux = 0;

				for(i=0; i < size; size--) {
					aux = (int) campo[j] - 48;
					aux *= (int) pow(10,size-1);
					result += aux;
					j++;
				}
				fwrite(&result,sizeof(int),1,this->fstream);
			}
			//conversao e escrita como double
			if(tipo == 3) {
				char * retorno;
				retorno = strtok(campo,".");
				int tam = strlen(retorno);
				int j = 0,i,flag = 0;
				double result = 0.0,aux;
				char c;
			
				// antes da virgula(ponto)
				for(i=0; i < tam; tam--) {
					c = retorno[j];
					aux = (double) retorno[j] - 48;
					aux *= (double) pow(10,(tam-1));
					result += aux;
				}
				// depois da virgula(ponto)
				j = 0;
				retorno = strtok('\0',".");
				tam = strlen(retorno);
				for(i = 0; i < tam; tam--) {
					aux = (double) retorno[j] - 48;
					aux /= (double) pow(10,tam);
					result += aux;
					j++;
				}
				fwrite(&result,sizeof(double),1,this->fstream);
				retorno = NULL;
			}


			return true;
		}

		//ler do arquivo um campo do registro do tipo char
		char *readChar() {
			char *charaux = NULL;
			int size = 0;
			
			fread(&size,sizeof(int),1,this->fstream);
			charaux = new char[size];
			fread(charaux,sizeof(char),size,this->fstream);
			
			return charaux;
		}
		//ler do arquivo um campo do registro do tipo int
		int readInt() {
			int intaux = 0;

			fread(&intaux,sizeof(int),1,this->fstream);

			return intaux;
		}
		//ler do arquivo um campo do registro do tipo double
		double readDouble() {
			double doubleaux = 0.0;

			fread(&doubleaux,sizeof(double),1,this->fstream);

			return doubleaux;
		}
		//ler do arquivo um campo do registro do tipo bool
		bool readBool() {
			bool boolaux;
			
			fread(&boolaux,sizeof(bool),1,this->fstream);			
			
			return boolaux;
		}
		//ler do arquivo um campo do registro do tipo blob(unsigned char)
		unsigned char *readBlob() {
			unsigned char *blobaux = NULL;
			return blobaux;
		}
};

#endif
