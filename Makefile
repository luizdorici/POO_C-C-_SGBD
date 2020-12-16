CC=g++
BINARY=./build/SGBD
INCLUDES=./includes
LIBS=-lm -lpcre
LIBDIR=.
SRC= src/main.cc src/SGBD.cc src/User.cc src/DataBase.cc src/Shell.cc
RM=rm -rf
CLEANFILES=./build/SGBD
FINDALL=find -name "*~" | xargs rm -rf

all: clean compile run

compile: 
	@$(CC) -o $(BINARY) -I$(INCLUDES) -L$(LIBDIR) $(SRC) $(LIBS)

clean: 
	@$(RM) $(CLEANFILES)
	@$(FINDALL)

run:
	@$(BINARY)

zip:
	@zip -r sgbd.zip Makefile ./build ./includes ./src ./System ./Documentation ./Modelagem
