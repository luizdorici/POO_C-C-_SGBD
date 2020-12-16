#include <User.h>


User::User(char *login, char *password) {
	this->login = new char[20]();
	this->password = new char[20]();
	strcpy(this->login,login);
	strcpy(this->password,password);
	this->myShell = NULL;
}

bool User::RunUserShell() {
	this->myShell->commandLine();
	return false;
}

bool User::authenticate(char *dataName) {
	
	//chdir(dataName);

	if(strcmp(this->login,"root") == 0) {
		if(strcmp(this->password,"scc204") == 0) {
			this->myShell = new Shell();
			return true;
		}
	}
	cout << "Erro: Nome de usuario ou senha invalidos." << endl;
	return false;
}

User::~User() {
	delete login;
	delete password;
	if(this->myShell != NULL)
		delete this->myShell;
}
