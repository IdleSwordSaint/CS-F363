all: a.out

a.out: mylex.l myparser.y ast.c ast.h
	bison -d myparser.y
	flex mylex.l
	gcc -o a.out myparser.tab.c lex.yy.c ast.c -lfl

clean:
	rm -f a.out myparser.tab.c myparser.tab.h lex.yy.c
