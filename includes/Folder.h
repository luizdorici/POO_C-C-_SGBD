#ifndef _FOLDER_H_
#define _FOLDER_H_

#include "FileSystemEntry.h"
#include <myvector.h>
#include <mystring.h>
#include <iostream>

using namespace std;

class Folder : public FileSystemEntry {
	private:
		Vector<FileSystemEntry *> *entries;
	public:
		Folder() : FileSystemEntry() {}
		Folder(char *name) : FileSystemEntry(name) {}
		Folder(String *name) : FileSystemEntry(name) {}
		virtual ~Folder() {

		}

		void addEntry(FileSystemEntry *entry) {}
		void removeEntry(FileSystemEntry *entry) {}
		FileSystemEntry *listEntries() {}

		void create(char *dataname) {

			if(chdir(dataname) != 0) { 
				if (mkdir(dataname, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH !=0)) {
					cout << "Não foi possível criar o diretório " << dataname << "." << endl;
				}
				else cout << dataname << endl;
			}
			else cout << "Erro: bando de dados existente." << endl;
		}
		void remove() { /* man 3 rmdir */ }

};

#endif
