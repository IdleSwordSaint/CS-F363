all: a.out

a.out: mylex.l myparser.y
	bison -d myparser.y
	flex mylex.l
	gcc -o a.out myparser.tab.c lex.yy.c -ll

clean:
	rm -f a.out myparser.tab.c myparser.tab.h lex.yy.c
