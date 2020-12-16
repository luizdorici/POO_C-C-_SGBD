#ifndef _RELATION_H_
#define _RELATION_H_

#include "mystring.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

class Relation {
	protected:
		String *name;
	public:

		Relation(String *name) {
			this->name = new String(name);
		}
		virtual void getInformation() = 0;
};
#endif
