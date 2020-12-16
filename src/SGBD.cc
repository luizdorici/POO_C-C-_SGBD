#include <SGBD.h>

using namespace std;

SGBD::SGBD() {
	this->dataName = new char[20]();
}

void SGBD::start() {
	bool matchResult = true;
	bool login = false;
	char *userId;
	char *userPwd;

	while (matchResult) {

		if (!login) {
			userId = new char[20]();
			userPwd = new char[20]();
			strcpy(this->dataName,"System");
			
			while(!login) {

				cin >> userId;
				cin >> userPwd;
				User *loadUser = new User(userId,userPwd);

				login = loadUser->authenticate(this->dataName);
				if(login)
					matchResult = loadUser->RunUserShell();
				delete loadUser;
			}
		}
		delete[] userId;
		delete[] userPwd;
	}						
}

SGBD::~SGBD() {
	delete[] this->dataName;
}
