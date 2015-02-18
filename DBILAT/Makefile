CC = g++ -O2 -Wno-deprecated -std=c++11 -w 
SRC = ./src/
OUT = bin
tag = -i

ifdef linux
tag = -n
endif

objects=$(addprefix $(OUT)/, Record.o Comparison.o ComparisonEngine.o Schema.o File.o DBFile.o test.o) y.tab.o lex.yy.o

$(OUT)/test: 	$(objects)
	$(CC) -o $(OUT)/test $(objects) -lfl
	
main: Record.o Comparison.o ComparisonEngine.o Schema.o File.o y.tab.o lex.yy.o main.o
	$(CC) -o main Record.o Comparison.o ComparisonEngine.o Schema.o File.o y.tab.o lex.yy.o main.o -lfl
	
$(OUT)/%.o: $(SRC)%.cc
	$(CC) -g -c -o $@ $<
	
y.tab.o: $(SRC)Parser.y
	yacc -d $(SRC)Parser.y
	sed $(tag) $(SRC)y.tab.c -e "s/  __attribute__ ((__unused__))$$/# ifndef __cplusplus\n  __attribute__ ((__unused__));\n# endif/" 
	g++ -c -w $(SRC)y.tab.c

lex.yy.o: $(SRC)Lexer.l
	lex  -w $(SRC)Lexer.l
	gcc  -c -w $(SRC)lex.yy.c

clean: 
	rm -f $(OUT)/*.o
	rm -f $(OUT)/*.out
	rm -f $(OUT)/test
	rm -f y.tab.c
	rm -f lex.yy.c
	rm -f y.tab.h
	rm -f lex.yy.o
	rm -f y.tab.o
