#include <DataBase.h>

DataBase::DataBase(char *dataname) {

	this->dataname = new char[20]();
	strcpy(this->dataname,dataname);
	this->tables = NULL;
	this->IDXlist = NULL;
	nIDX = 0;
	nTables = 0;
	this->dataFolder = NULL;
}

int DataBase::CreateFolder(char* BASEDIR) {

	chdir(BASEDIR);

	this->dataFolder = new Folder();
	this->dataFolder->create(this->dataname);
	
	chdir(BASEDIR);
	return 1;

}

void DataBase::setFields(int counter,char *field) {
	int type = 0;
	int length = 0;
	int size;
	Field *newField = new Field();

	int flag = 0;
	char *retorno = NULL;

	newField->setlength(0);

	retorno = strtok(field,"[] ");

	while(retorno != NULL) {
		if(flag == 0) {
			newField->setname(retorno);
			flag = 1;
		}
		else if (flag == 1) {
			if(strcmp(retorno,"char") == 0)
				type = 1;
			else if(strcmp(retorno,"int") == 0)
				type = 2;
			else if(strcmp(retorno,"double") == 0)
				type = 3;
			else if(strcmp(retorno,"bool") == 0)
				type = 4;
			else if(strcmp(retorno,"blob") == 0)
				type = 5;
			
			newField->settype(type);
			flag = 2;
		}
		else { 
			length = atoi(retorno);
			newField->setlength(length);
			flag = 3;
		}					
		retorno = strtok('\0',"[] ");
	}
	tables[this->nTables-1]->addField(newField,false);
	newField = NULL;
}

void DataBase::printFields(int counter,char *field) {
	int flag = 0;
	char *retorno = NULL;
	retorno = strtok(field," ");

	while(retorno != NULL) {
		if(flag == 0) {
			cout << retorno << ": ";
			flag = 1;
		}
		else {
			cout << retorno << endl;
		}
		retorno = strtok('\0'," ");
		
	}

}

Table *DataBase::getTableToWork(String *tablename) {
	int i;
	for(i=0; i<nTables; i++)
		if(strcmp(tables[i]->getName(),tablename->toCharArray()) == 0)
			return this->tables[i];
	create_table(tablename,false);
	return this->tables[nTables-1];
}

void DataBase::create_table(String *tablename,bool create) {

	Table *newTable = new Table(tablename);

	this->tables = (Table**) realloc(this->tables,sizeof(Table*) *nTables+1);

	//nao cria os arquivos no caso de insert table
	if(create)
		newTable->create_files();
	else newTable->LoadTdFile();

	this->tables[nTables] = newTable;
	
	this->nTables++;

	newTable = NULL;
}

Index *DataBase::getIDXtoWork(char *name, String *tablename) {
	int i;
	for(i=0; i<this->nIDX; i++)
		if(strcmp(IDXlist[i]->getName(),name) == 0)
			return this->IDXlist[i];
	create_index(name,tablename,false);
	return this->IDXlist[nIDX-1];
}

Index *DataBase::create_index(char *IDXName,String *relationName,bool create) {

	Index *newIDX = new Index(IDXName,relationName);

	this->IDXlist = (Index**) realloc(this->IDXlist,sizeof(Index*) *nIDX+1);

	if(create)
		newIDX->create_file();
	else newIDX->LoadIDXFile();
	
	this->IDXlist[nIDX] = newIDX;

	this->nIDX++;

	newIDX = NULL;

	if(create)
		return this->IDXlist[nIDX-1];
	else return NULL;
}

//backup
//restore

DataBase::~DataBase() {
	if(this->tables != NULL) {
		int i;
		for(i=0 ;i < this->nTables; i++)
			free(this->tables[i]);
		if(this->tables != NULL)
			free(this->tables);
	}
	if(this->IDXlist != NULL) {
		int i;
		for(i=0 ;i < this->nIDX; i++)
			free(this->IDXlist[i]);
		if(this->IDXlist != NULL)
			free(this->IDXlist);
	}
	delete this->dataFolder;
}
