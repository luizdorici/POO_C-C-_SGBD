//Luiz Eduardo Dorici NUSP: 4165850

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pcre.h>

#include <SGBD.h>


int main(int argc, char *argv[]) {

	SGBD *s = new SGBD();
	s->start();
	delete s;

	return 0;
}
