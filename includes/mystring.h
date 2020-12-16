#ifndef __STRING_
#define __STRING_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pcre.h>

class String {
	private:
		char *string;

	public:
		String() { this->string = NULL; }

		String(char *string) { 
			this->string = NULL;
			this->setString(string);
		}

		String(const char *string) { 
			this->string = NULL;
			this->setString(string);
		}

		String(String *string) { 
			this->string = NULL;
			this->setString(string);
		}

		String(const char *string, int start, int end) { 
			this->string = NULL;
			this->string = (char*) malloc(sizeof(char)*(end-start)+1);
			strncpy(this->string, string + start, end - start);
			this->string[end - start] = '\0';
		}

		virtual ~String() {
			if (this->string != NULL)
				free(this->string);
		}

		void setString(char *string) {
			if (this->string != NULL) free(this->string);
			this->string = (char *) malloc(sizeof(char) *
					(strlen(string)+1));
			strcpy(this->string, string);
		}

		void setString(const char *string) {
			if (this->string != NULL) free(this->string);
			this->string = (char *) malloc(sizeof(char) *
					(strlen(string)+1));
			strcpy(this->string, string);
		}

		void setString(String *string) {
			if (this->string != NULL) free(this->string);
			this->string = (char *) malloc(sizeof(char) *
					(string->length()+1));
			strcpy(this->string, string->toCharArray());
		}

		char *toCharArray() {
			return this->string;
		}

		int length() {
			if (this->string != NULL)
				return strlen(this->string);
			return 0;
		}

		String *clone() {
			return new String(this);
		}
};
#endif
