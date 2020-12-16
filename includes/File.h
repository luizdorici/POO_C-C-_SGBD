#ifndef _FILE_H_
#define _FILE_H_

#include "FileSystemEntry.h"
#include <string.h>
#include <iostream>

using namespace std;

class File : public FileSystemEntry {
	private:

	protected:
		FILE *fstream;

	public:
		File() : FileSystemEntry() {
			this->fstream = NULL;
		}

		File(char *name) : FileSystemEntry(name) {
			this->fstream = NULL;
		}

		File(String *name) : FileSystemEntry(name) {
			this->fstream = NULL;
		}

		virtual ~File() {
			if (this->fstream != NULL)
				fclose(this->fstream);
		}

	protected:
		virtual void create() {
			FILE *fp = fopen(this->getName()->toCharArray(), "rb");
			if (!fp) {
				this->fstream = fopen(this->getName()->toCharArray(), "wb");
				if (!this->fstream) 
					fclose(this->fstream);
			} else {
				fclose(fp);
			}
		}
	public:
		virtual bool open(bool readonly, bool binary, bool end) {
			if (this->getName() == NULL) return false;

			// já existe?
			this->create();

			if (readonly) {
				this->fstream = fopen(this->getName()->toCharArray(),binary ? "rb" : "r");
			}else if(!end) {
				this->fstream = fopen(this->getName()->toCharArray(),binary ? "rb+" : "r+");
			}else {
				this->fstream = fopen(this->getName()->toCharArray(),binary ? "ab+" : "r+");
			}

			return this->fstream != NULL;
		}

		virtual bool close() {
			return !fclose(this->fstream) ? true : false;
		}

		virtual void seek(long offset) {
			// a partir do inicio do arquivo
			fseek(this->fstream, offset, SEEK_SET);
		}

		virtual void seekBack(long minus) {
			if (minus > 0) minus *= -1;
			// a partir da posicao atual
			fseek(this->fstream, minus, SEEK_CUR);
		}

		virtual void seekForward(long plus) {
			if (plus < 0) plus *= -1;
			// a partir da posicao atual
			fseek(this->fstream, plus, SEEK_CUR);
		}

		long getOffset() { return ftell(this->fstream); }

		virtual unsigned char *readBytes() = 0;
		virtual unsigned char *readBytes(int length) = 0;
		virtual bool writeBytes(unsigned char *buffer, int length) = 0;
};

#endif
