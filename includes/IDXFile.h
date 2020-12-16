#ifndef _IDXFILE_H_
#define _IDXFILE_H_

#include "File.h"

class IDXFile : public File {
	private:
		String *RelationWith;
	protected:
	public:
		IDXFile() : File() {}
		IDXFile(char *name) : File(name) {}
		IDXFile(String *name) : File(name) {}
		virtual ~IDXFile() {}

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
};

#endif
