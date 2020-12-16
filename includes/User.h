#ifndef _USER__
#define _USER__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

#include <myvector.h>
#include <mystring.h>
#include <Shell.h>

using namespace std;

class User {
	private:
		char *login;
		char *password;
		Shell *myShell;

	public:

		User(char *login, char *password);

		bool RunUserShell();

		bool authenticate(char *dataName);

		virtual ~User();
};
#endif
