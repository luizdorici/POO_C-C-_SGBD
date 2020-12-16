#ifndef _SGBD__
#define _SGBD__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

#include <myvector.h>
#include <mystring.h>
#include <User.h>

/* 
	nao implementado
*/


class SGBD {
	
	private:
		char *dataName;


	public:
		
		SGBD();

		bool createDatabase();
	
		bool createUser();

		void start();

		void stop();
	
		void receiveNetworkConnection();

		void receiveLocalConnection();

		virtual ~SGBD();

	
};
#endif
