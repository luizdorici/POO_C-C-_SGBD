#ifndef __VECTOR_
#define __VECTOR_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pcre.h>

template <class T>
class Vector {
	private:
		T *vector;
		int counter;
	public:
		Vector() {
			vector = NULL;
			counter = 0;
		}

		virtual ~Vector() {
			if (vector != NULL) free(vector);
		}

		// T = Book
		void add(T elem) {
			vector = (T *) realloc(vector, sizeof(T) *
					(counter + 1));
			vector[counter++] = elem;
		}

		void remove(int pos) {
			if (pos >= 0 && pos < counter) {
				for (int i = pos; i < counter-1; i++) {
					vector[i] = vector[i+1];
				}	
				vector = (T *) realloc(vector,
					sizeof(T) * --counter);
			}
		}

		T get(int pos) {
			if (pos >= 0 && pos < counter)
				return vector[pos];
			return NULL;
		}

		int size() {
			return counter;
		}
};
#endif
