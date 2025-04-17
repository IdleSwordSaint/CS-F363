all:
	flex mylex.l
	bison -d myparser_ast.y
	gcc -o a.out lex.yy.c myparser_ast.tab.c

clean:
	rm -f lex.yy.c myparser_ast.tab.c myparser_ast.tab.h a.out