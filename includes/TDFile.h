#ifndef _TDFILE_H_
#define _TDFILE_H_

#include "File.h"

class TDFile : public File {
	private:
		String *RelationWith;
	protected:
	public:
		TDFile() : File() {}
		TDFile(char *name) : File(name) {}
		TDFile(String *name) : File(name) {}
		virtual ~TDFile() {}

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

		//escreve um field para o arquivo binario (.td)
		virtual bool writeField(char * name,int type,int length) {
			int size = strlen(name);
			fwrite(&size,sizeof(int),1,this->fstream);
			fwrite(name,sizeof(char),size,this->fstream);
			fwrite(&type,sizeof(int),1,this->fstream);
			fwrite(&length,sizeof(int),1,this->fstream);
			return true;
		}
	
		// le field do arquivo binario (.td)
		char *readField(int *type,int *length) {
			int size;
			fread(&size,sizeof(int),1,this->fstream);
			if(feof(this->fstream))
				return NULL;
			char *fieldname = new char[size+1]();
			fread(fieldname,sizeof(char),size,this->fstream);
			fread(type,sizeof(int),1,this->fstream);
			fread(length,sizeof(int),1,this->fstream);
			return fieldname;
		}
};

#endif
