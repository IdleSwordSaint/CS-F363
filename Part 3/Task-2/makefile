all: a.out

a.out: mylex.l parser.y
	bison -d parser.y
	flex mylex.l
	gcc -o a.out parser.tab.c lex.yy.c -lfl

clean:
	rm -f a.out parser.tab.c parser.tab.h lex.yy.c
