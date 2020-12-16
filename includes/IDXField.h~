#ifndef _IDXFIELD_
#define _IDXFIELD_

/*
	classe auxiliar que ajuda a carregar o index na memoria primaria
	para busca, update e ordenacao
	template utilizada pois o indice pode ser feito por campos do tipo char ou int
	levando em consideracao indices de chave primaria ou secundaria
	pode tbm expandir para utilizar double ou bool
	caso por exemplo alguem queria listar todos com salario maior que 1k ou coisas semelhantes
*/

template <class TKEY>
class IDXField {
	private:
		TKEY *key;
		int byteoffset;
	public:
		IDXField(int *field) {
			this->key = new int();
			this->key = field;
		}
		IDXField(char *field) {
			this->key = new char[strlen(field)+1];
			strcpy(this->key,field);
		}
		TKEY *getkey() { return this->key; }
		int getoffset() { return this->byteoffset; }
		virtual~IDXField() {}
};

#endif
