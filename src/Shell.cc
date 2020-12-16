#include <Shell.h>

Vector<String *> *Shell::match(const char *command, const char *pattern, bool *matchResult) {

	pcre *reCompiled = NULL;
	pcre_extra *pcreExtra = NULL;
	int pcreExecRet;
	const char *pcreErrorStr;
	int pcreErrorOffset;
	const char *psubStrMatchStr;
	int j, len = 300;
	int *subStrVec = new int[len];

	Vector<String *> *myVector = new Vector<String *>();

	// no match for now...
	*matchResult = false;

	//printf("Pattern is: %s\n", pattern);

	// Compiling the pattern to be applied later...
	reCompiled = pcre_compile(pattern, 0, &pcreErrorStr, &pcreErrorOffset, NULL);

	// pcre_compile returns NULL on error, and sets pcreErrorOffset & pcreErrorStr
	if(reCompiled == NULL) {
	  printf("ERROR: Could not compile '%s': %s\n", pattern, pcreErrorStr);
	  return myVector;
	}

	// Optimize the regex
	pcreExtra = pcre_study(reCompiled, 0, &pcreErrorStr);

	/* pcre_study() returns NULL for both errors and when it can not optimize
	   the regex.  The last argument is how one checks for errors (it is NULL
	   if everything works, and points to an error string otherwise. */
	if (pcreErrorStr != NULL) {
	  printf("ERROR: Could not study '%s': %s\n", pattern, pcreErrorStr);
	  return myVector;
	}

	/* Try to find the regex in command, and report results. */
	pcreExecRet = pcre_exec(reCompiled,
	      		    pcreExtra,
	      		    command, 
	      		    strlen(command),  // length of string
	      		    0,                // Start looking at this point
	      		    0,                // OPTIONS
	      		    subStrVec,
	      		    len);              // Length of subStrVec
	// Something bad happened..
	if (pcreExecRet < 0) { 
	} else {
	    *matchResult = true;
	   // printf("Result: We have a match!\n");
	      
	    // At this point, rc contains the number of substring matches found...
	    if(pcreExecRet == 0) {
	      printf("But too many substrings were found to fit in subStrVec!\n");
	      // Set rc to the max number of substring matches possible.
	      pcreExecRet = 30 / 3;
	    }

	    // PCRE contains a handy function to do the above for you:
	    for(j=0; j<pcreExecRet; j++) {
	      pcre_get_substring(command, subStrVec, pcreExecRet, j, 
			      &(psubStrMatchStr));

	      myVector->add(new String(command, subStrVec[j*2], subStrVec[j*2+1]));
	    }

	    // Free up the substring
	    pcre_free_substring(psubStrMatchStr);
	}

	// Free up the regular expression.
	pcre_free(reCompiled);
	    
	// Free up the EXTRA PCRE value (may be NULL at this point)
	if(pcreExtra != NULL)
	  pcre_free(pcreExtra);

	return myVector;
}

char* Shell::nextCommand(FILE *stream) {
	char *buffer = NULL;
	int counter = 1;
	char c;
	bool matchResult;

	while (true) {
		c = fgetc(stream);

		if(c == '\n')
			continue;

		buffer = (char *) realloc(buffer, sizeof(char) * (counter+1));
		buffer[counter-1] = c;
		buffer[counter] = '\0';
		counter++;

		this->match(buffer, "^(.*[^\\\\];)$", &matchResult);

		if (matchResult) {
			char *aux = new char[counter];
			strcpy(aux, buffer);
			if (buffer != NULL) free(buffer);

			return aux;
		}
	}
}

bool Shell::isCreateTable(const char *command) {
	bool matchResult;

	Vector<String *> *result = 
		this->match(command, "^(?i)\\s*create\\s+table\\s+([a-zA-Z][a-zA-Z0-9_-]*)\\s+\\(\\s*([^\\)]+)\\s*\\)\\s*;\\s*$", &matchResult);

	if (matchResult) {

		if(this->usingData == false) {
			cout << "Erro: nenhum banco de dados foi definido." << endl;
			delete result;
			return true;
		}

		String *tableName = result->get(1);
		String *fields = result->get(2);

		char *buffer = new char[BUFFER_SIZE];
		char *clonename = new char[strlen(tableName->toCharArray())+1];
		bool matchField;
		int counter = 0;
		
		strcpy(clonename,tableName->toCharArray());
		strcat(clonename,".td");
		FILE *check = fopen(clonename,"rb");
	
		//verifica se a tabela ja existe
		if(check != NULL) {
			cout << "Erro: tabela nao foi criada." << endl;
			fclose(check);
		}else {
				cout << tableName->toCharArray() << endl;
				this->runningData->create_table(tableName,true);

				//laco usado para printar os fields da tabela a ser criada
				do {
					matchField = false;
					sprintf(buffer, "(?:([^,]+)(,|$)){%d}", ++counter);

					Vector<String *> *field = 
						this->match(fields->toCharArray(), buffer, &matchField);

					if (matchField) {
						String *fieldInfo = field->get(1);
						this->runningData->printFields(counter,fieldInfo->toCharArray());
					}

					delete field;
				} while (matchField);

				counter = 0;
				//seta os fields na tabela criando ela de fato
				do {
					matchField = false;
					sprintf(buffer, "(?:([^,]+)(,|$)){%d}", ++counter);

					Vector<String *> *field = 
						this->match(fields->toCharArray(), buffer, &matchField);

					if (matchField) {
						String *fieldInfo = field->get(1);

						this->runningData->setFields(counter,fieldInfo->toCharArray());
					}

					delete field;
				} while (matchField);

			this->buffer = buffer;
			delete[] buffer;
			delete[] clonename;
		}
		delete result;
		return true;
	}
	delete result;
	return false;
}

bool Shell::isCreateIndex(const char *command) {
	bool matchResult;

	Vector<String *> *result = this->match(command,"^(?i)\\s*create\\s+index\\s+([a-z[a-z0-9_-]*)\\s*on\\s+([a-z[a-z0-9_-]*)\\s+\\(\\s*([^\\\\)]+)\\s*\\)\\s+([a-z[a-z0-9_-]*)\\s*;\\s*$",&matchResult);

	if (matchResult) {

		if(this->usingData == false) {
			cout << "Erro: nenhum banco de dados foi definido." << endl;
			delete result;
			return true;
		}
		String *idxName = result->get(1);
		String *tablename = result->get(2);
		String *fieldName = result->get(3);
		String *searchName = result->get(4);

		char *clonename = new char[strlen(tablename->toCharArray())+1];

		//antes da criacao do index, verifica se a tabela existe
		strcpy(clonename,tablename->toCharArray());
		strcat(clonename,".td");
		FILE *fp = fopen(clonename,"rb");

		if(fp != NULL) {
			fclose(fp);

			//cria o nome do arquivo de indice
			char *IDXName = (char*) new char[strlen(tablename->toCharArray())+
											strlen(fieldName->toCharArray())+
											strlen(idxName->toCharArray())+
											strlen(searchName->toCharArray())+8];
			IDXName[0] = '\0';
			strcat(IDXName,tablename->toCharArray());
			strcat(IDXName,"_");
			strcat(IDXName,fieldName->toCharArray());
			strcat(IDXName,"_");
			strcat(IDXName,idxName->toCharArray());
			strcat(IDXName,"_");
			strcat(IDXName,searchName->toCharArray());
			strcat(IDXName,".idx");
			
			//verifica se o indice ja existe
			fp = fopen(IDXName,"r");
			if(fp != NULL) {
				cout << "Erro: Indice nome_do_indice existente." << endl;
				delete[] clonename;
				delete result;
				return true;
			} else {

				Index *WorkingIndex = NULL;
				//cria o arquivo de index e o objeto index
				WorkingIndex = this->runningData->create_index(IDXName,tablename,true);
				
				// carrega tabela para gerar o index
				Table *WorkingTable = this->runningData->getTableToWork(tablename);

				int size = 0;
				//pega a posicao do campo na tabela e o tamanho de cada registro
				int pos = WorkingTable->Util(fieldName->toCharArray(),&size);

				if(pos == -1) {
					cout << "Erro: Campo inexistente." << endl;
					delete[] IDXName;
					delete[] clonename;
					delete result;
					return true;
				}
				
				WorkingIndex->writeIndex(WorkingTable,size,fieldName->toCharArray(),pos);
				delete[] IDXName;
			}
			
		}
		else cout << "Erro: Tabela inexistente." << endl;

		delete[] clonename;
		delete result;
		return true;
	}
	delete result;
	return false;	
}

bool Shell::isCreateDatabase(const char *command) {
	bool matchResult;

	Vector<String *> *result = this->match(command,"^(?i)\\s*create\\s+database\\s+([a-zA-z][a-zA-Z0-9_-]*)\\s*;\\s*$",&matchResult);
		
	if (matchResult) {

		String *DataName = result->get(1);

		DataBase *newData = new DataBase(DataName->toCharArray());

		newData->CreateFolder(BASEDIR);

		delete DataName;
		delete newData;
		delete result;
		return true;
	}
	delete result;
	return false;
}

bool Shell::isInsert(const char *command) {
	bool matchResult;

	Vector<String *> *result = this->match(command,"^(?i)\\s*insert\\s+into\\s+([a-z[a-z0-9_-]*)\\s+\\(\\s*([^\\)]+)\\s*\\)\\s+values\\s+\\(\\s*([^\\)]+)\\s*\\);\\s*$",&matchResult);

	if (matchResult) {

		if(this->usingData == false) {
			cout << "Erro: nenhum banco de dados foi definido." << endl;
			delete result;
			return true;
		}
		String *tableName = result->get(1);
		String *nameFields = result->get(2);
		String *fields = result->get(3);

		char *buffer = new char[BUFFER_SIZE];
		char *clonename = new char[strlen(tableName->toCharArray())+3]();
		bool matchField;
		int counter = 0;
		int countersize = 0;
		char limpeza;
		int i = 0;
		bool duplicate = false;

		
		strcpy(clonename,tableName->toCharArray());
		strcat(clonename,".td");
		FILE *fp = fopen(clonename,"rb");

		if(fp != NULL) {
			fclose(fp);
			Table *WorkingTable = this->runningData->getTableToWork(tableName);

			//este laco eh responsavel por pegar os campos do insert e chegar se eles
			//correspondem aos campos do arquivo .td, assim validando o insert
			do {
				matchField = false;
				sprintf(buffer, "(?:[\\s]*(?:[\"]?|[\']?)([^,\"\']+)(?:[\"]?|[\']?)\\s*(?:,\\s*|$)){%d}", ++countersize);

				Vector<String *> *field = 
					this->match(nameFields->toCharArray(), buffer, &matchField);

				if (matchField) {
					String *fieldInfo = field->get(1);
					//verifica se os fields a serem inseridos existem
					bool exist = WorkingTable->CheckField(fieldInfo->toCharArray(),countersize-1);
					if(!exist) {
						cout << "Erro: campo inexistente." << endl;
						countersize = -1;
						break;
					}
				}
				delete field;
			} while (matchField);

			//verifica se existe campo duplicado
			if(countersize != -1)
				duplicate = WorkingTable->isDuplicate();
			if(duplicate)
				cout << "Erro: campo duplicado." << endl;

			// verifica se numero de campos digitados eh igual ao numero de campos exigido pela tabela
			if((WorkingTable->getNfields()) == (countersize-1) && !duplicate) {
			
				cout << tableName->toCharArray() << endl;

				do {
					matchField = false;
					sprintf(buffer, "(?:[\\s]*(?:[\"]?|[\']?)([^,\"\']+)(?:[\"]?|[\']?)\\s*(?:,\\s*|$)){%d}", ++counter);

					Vector<String *> *field = 
						this->match(fields->toCharArray(), buffer, &matchField);

					if (matchField) {
						String *fieldInfo = field->get(1);
						
						//seta os dados do registro para depois inserir
						WorkingTable->setRegister(fieldInfo->toCharArray(),counter);
					}
					
					delete field;
				} while (matchField);

				//apos ordenar o registro da forma correta(caso necessario) imprime na tela
				// e insere no arquivo (.dat)
				WorkingTable->insertReg();				
				
				delete[] clonename;
				this->buffer = buffer;
				delete[] buffer;	
			}
			WorkingTable = NULL;

		} else cout << "Erro: tabela inexistente." << endl;

		delete result;
		return true;
	}
	delete result;
	return false;
}

bool Shell::isUseDatabase(const char *command) {
	bool matchResult;
	Vector<String *> *result = this->match(command,"^(?i)\\s*use\\s+([a-zA-z][a-zA-Z0-9_-]*)\\s*;\\s*$",&matchResult);

	if (matchResult) {

		String *DataName = result->get(1);

		if(this->runningData != NULL)
			delete this->runningData;

		//tenta acessar o banco de dados a partir da pasta raiz do sgbd
		chdir(BASEDIR);
		if(chdir(DataName->toCharArray()) != 0) {
			cout << "Erro: banco de dados inexistente." << endl;
			delete DataName;
			delete result;
			return true;
		}
		else cout << "Sucesso: conectado ao banco de dados " << DataName->toCharArray() << "." << endl;

		//ponteiro runningData sempre aponta para o banco de dados que esta em uso
		this->runningData = new DataBase(DataName->toCharArray());

		delete result;
		delete DataName;
		this->usingData = true;
		return true;
	}
	delete result;
	return false;
}

bool Shell::isUpdateIndex(const char *command) {
	bool matchResult;
	Vector<String *> *result = this->match(command,"^(?i)\\s*update\\s+index\\s+([a-z[a-z0-9_-]*)\\s*on\\s+([a-z[a-z0-9_-]*)\\s+\\(\\s*([^\\\\)]+)\\s*\\)\\s+([a-z[a-z0-9_-]*)\\s*;\\s*$",&matchResult);



	if (matchResult) {
		if(this->usingData == false) {
			cout << "Erro: nenhum banco de dados foi definido." << endl;
			delete result;
			return true;
		}
		String *idxName = result->get(1);
		String *tablename = result->get(2);
		String *fieldName = result->get(3);
		String *searchName = result->get(4);

		//cria o nome do index
		char *IDXName = (char*) malloc (sizeof(char)+strlen(tablename->toCharArray())+
										strlen(fieldName->toCharArray())+
										strlen(idxName->toCharArray())+
										strlen(searchName->toCharArray())+8);
		IDXName[0] = '\0';
		strcat(IDXName,tablename->toCharArray());
		strcat(IDXName,"_");
		strcat(IDXName,fieldName->toCharArray());
		strcat(IDXName,"_");
		strcat(IDXName,idxName->toCharArray());
		strcat(IDXName,"_");
		strcat(IDXName,searchName->toCharArray());
		strcat(IDXName,".idx");
		
		//verifica se o indice ja existe
		FILE *fp = fopen(IDXName,"r");
		if(fp == NULL) {
			cout << "Erro: Arquivo de indice inexistente." << endl;
			delete result;
			return true;
		}

		delete result;
		return true;
	}
	delete result;
	return false;
}

// select
bool Shell::isSelect(const char *command) {
	bool matchResult = false;
	Vector<String *> *result = NULL;

	//expressao regular para select com comando where
	result = this->match(command,"(?i)\\s*select\\s+([^\\)]+)\\s+from\\s+([a-z[a-z0-9_-]*)\\s+where\\s+([a-z[a-z0-9_-]*)\\s*([!=<>]+)\\s*([^\\)]+)\\s*;\\s*$",&matchResult);

	if(!matchResult)
		// expressao regular para select sem campo where
		result = this->match(command,"^(?i)\\s*select\\s+([^\\)]+)\\s+from\\s+([a-z[a-z0-9_-]*)\\s*;\\s*$",&matchResult);

	if (matchResult) {

		String *tableName = result->get(2);

		char *clonename = NULL;
		//antes de selecionar com o comando select, verifica se a tabela existe
		clonename = new char[strlen(tableName->toCharArray())+4];
		strcpy(clonename,tableName->toCharArray());
		strcat(clonename,".td");
		FILE *fp = fopen(clonename,"rb");

		if(fp != NULL) {
			fclose(fp);
		}else cout << "Erro: Tabela invalida." << endl;

		delete[] clonename;
		delete result;
		return true;
	}

	delete result;
	return false;
}

//delete
bool Shell::isDelete(const char *command) {

	bool matchResult;

	Vector<String *> *result = this->match(command,"^(?i)\\s*delete\\s*from\\s+([a-z[a-z0-9_-]*)\\s*where\\s+([a-z[a-z0-9_-]*)\\s*([!=<>]+)\\s*([^\\)]+)\\s*;\\s*$",&matchResult);

	if(!matchResult)
		Vector<String *> *result = this->match(command,"^(?i)\\s*delete\\s*from\\s+([a-z[a-z0-9_-]*)\\s*;\\s*$",&matchResult);

	if (matchResult) {

		String *tableName = result->get(1);

		//antes de deletar da tabela, verifica se a tabela existe
		char *clonename = new char[strlen(tableName->toCharArray())+4];
		strcpy(clonename,tableName->toCharArray());
		strcat(clonename,".td");
		FILE *fp = fopen(clonename,"rb");
		delete[] clonename;

		if(fp != NULL) {
			fclose(fp);
		}else cout << "Erro: Tabela invalida." << endl;

		delete result;
		return true;
	}

	delete result;
	return false;
}

bool Shell::isExit(const char *command) {
	bool matchResult;

	Vector<String *> *result = this->match(command,"^(?i)\\s*exit\\s*;\\s*$",&matchResult);
		
	delete result;
	if (matchResult) {

		return false;
	}
	else return true;

}

Shell::Shell() {
	this->buffer = NULL;
	this->counter = 0;
	this->BUFFER_SIZE = 1024;
}

void Shell::commandLine() {
	bool matchResult = true;
	this->usingData = false;
	bool cmdvalidation[8];
	
	char BASEDIR[255];

	if (getcwd(BASEDIR, sizeof(BASEDIR)) == NULL) {
		cout << "Não foi possível obter o diretório base." << endl;
	}

	this->BASEDIR = BASEDIR;

	while (matchResult) {

		char *command = nextCommand(stdin);

		cmdvalidation[0] = isCreateDatabase(command);
		cmdvalidation[1] = isCreateTable(command);
		cmdvalidation[2] = isInsert(command);
		cmdvalidation[3] = isUseDatabase(command);
		cmdvalidation[4] = isCreateIndex(command);
		cmdvalidation[5] = isUpdateIndex(command);
		cmdvalidation[6] = isSelect(command);
		cmdvalidation[7] = isDelete(command);
		matchResult = isExit(command);
		
		if(cmdvalidation[0] == false && cmdvalidation[1] == false && cmdvalidation[2] == false && 
			cmdvalidation[3] == false && cmdvalidation[4] == false && cmdvalidation[5] == false &&
			 cmdvalidation[6] == false && cmdvalidation[7] == false && matchResult == true)
			cout << "Erro: comando invalido." << endl;
	}
}
Shell::~Shell() {
}
