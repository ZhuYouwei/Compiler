all: nas c6c

lex.yy.c: c5.l
	flex c5.l

y.tab.c: c5.y
	bison -y -d c5.y

c6c: lex.yy.c y.tab.c c5c.c
	gcc -o c6c lex.yy.c y.tab.c c5c.c

nas: nas.c nas.tab.c
	gcc -o nas nas.c nas.tab.c

nas.c: nas.l
	flex -o nas.c nas.l

nas.tab.c: nas.y
	bison -d nas.y

clean:
	$(RM) lex.yy.c y.tab.c y.tab.h nas.c nas.tab.c nas.tab.h output
