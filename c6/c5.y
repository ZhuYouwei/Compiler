%{
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "calc3.h"
#include <string.h>
#include <ctype.h>

/* prototypes */
nodeType *opr(int oper, int nops, ...);
nodeType *id(char * i, int isGlobal);
nodeType *con(int i, char c, char* s, consEnum type);
nodeType *create_func(nodeType *name, nodeType *arg, nodeType *stmt);
nodeType *ary(nodeType *subary, nodeType *arg, int isId );



void freeNode(nodeType *p);
void freeStack(void);
int ex(nodeType *p, int, int, int);
int firstpass(nodeType *p, int);
void eop();
void print_global();

int yylex(void);

void yyerror(char *s);



//int sym[26];                    /* symbol table */
//char *global_sym[1024];
//int global_sym_top;
//char *funcNames[1024];
//int funcNameTop = -1;
int argc;

%}

%union {
    int iValue;                 /* integer value */
    char * sIndex;                /* symbol table index */
    char cValue;                // constant char
    char *strValue;             // constant string
    nodeType *nPtr;             /* node pointer */
};

%token <iValue> INTEGER
%token <sIndex> VARIABLE 
%token <cValue> CHAR
%token <strValue> STRING
%token FOR WHILE IF PRINT READ BREAK CONTINUE RETURN OFFSET RVALUE 
%token GETI PUTI PUTI_ GETC PUTC PUTC_ GETS PUTS PUTS_ ARRAY
%nonassoc IFX
%nonassoc ELSE
%start program



%left AND OR

%left GE LE EQ NE '>' '<'
%left '+' '-'
%left '*' '/' '%'
%nonassoc UMINUS

%type <nPtr> stmt expr stmt_list vari s func args_call ary_def ary





%%

program: 
        s              { firstpass($1,0); 
                         print_global();
                         ex($1, 998, 998, 0); 
                         
                         freeNode($1);
                         freeStack(); 
                         // fprintf(stderr, "[Debug] var_num: %d\n",global_sym_top + 1);
                         // fprintf(stderr, "[Debug] func_num %d\n", funcNameTop + 1);
                         eop(); 
                         exit(0); }
        ;
s:       
        s stmt          {  $$ = opr(MAIN, 2,  $1, $2);}
        | s func        {  $$ = opr(MAIN, 2,  $1, $2);}
        | /* NULL */    {  $$ = opr(EMPTY, 0);}
        ;         

func:   vari '(' args_call ')' '{' stmt_list '}' {$$ = create_func($1, $3, $6);}
        | vari '(' ')' '{' stmt_list '}' {$$ = create_func($1, NULL, $5);}

ary_def:
        ary { $$ = $1;}
        | ary ',' ary_def { $$ = opr(',', 2, $1, $3);}
        | /* NULL */    { $$ = opr(EMPTY, 0);}

args_call:   
        expr                                  { $$ = $1; }
        | expr ',' args_call                  { $$ = opr(',', 2, $1, $3);}
        ;




stmt:
          ';'                                 { $$ = opr(';', 2, NULL, NULL); }
        | PRINT expr ';'                      { $$ = opr(PRINT, 1, $2); }
        | READ vari ';'                       { $$ = opr(READ, 1, $2); }
        | vari '=' expr ';'                   { $$ = opr('=', 2, $1, $3); }
        | FOR '(' stmt stmt stmt ')' stmt     { $$ = opr(FOR, 4, $3, $4, $5, $7); }
        | WHILE '(' expr ')' stmt             { $$ = opr(WHILE, 2, $3, $5); }
        | IF '(' expr ')' stmt %prec IFX      { $$ = opr(IF, 2, $3, $5); }
        | IF '(' expr ')' stmt ELSE stmt      { $$ = opr(IF, 3, $3, $5, $7); }
        | '{' stmt_list '}'                   { $$ = $2; }
        | BREAK ';'                           { $$ = opr(BREAK, 0); }
        | CONTINUE ';'                        { $$ = opr(CONTINUE, 0); }
        | GETI '(' vari ')' ';'               { $$ = opr(GETI, 1, $3);}
        | PUTI '(' expr ')' ';'               { $$ = opr(PUTI, 1, $3);}
        | PUTI_ '(' expr ')' ';'              { $$ = opr(PUTI_, 1, $3);}
        | GETC '(' vari ')' ';'               { $$ = opr(GETC, 1, $3);}
        | PUTC '(' expr ')' ';'               { $$ = opr(PUTC, 1, $3);}
        | PUTC_ '(' expr ')' ';'              { $$ = opr(PUTC_, 1, $3);}
        | GETS '(' vari ')' ';'               { $$ = opr(GETS, 1, $3);}
        | PUTS '(' expr ')' ';'               { $$ = opr(PUTS, 1, $3);}
        | PUTS_ '(' expr ')' ';'              { $$ = opr(PUTS_, 1, $3);}
        | RETURN expr ';'                     { $$ = opr(RETURN,1,$2);}
        | ARRAY ary_def ';'                   { $$ = opr(ARRAY,1,$2);}
        | ARRAY ary '=' expr ';'              { $$ = opr(ARRAY,2,$2,$4);}
        | expr ';'                            { $$ = $1; }
        ;





vari:
          VARIABLE { $$ = id($1,0); }
        | '@'   VARIABLE { $$ = id($2,1);}
        | '&'   VARIABLE { $$ = id($2,2);}
        | '*'   VARIABLE { $$ = id($2,3);}
        | ary {$$ = $1;}
        ;

ary:
        VARIABLE '[' expr ']' { $$ = ary(id($1,0), $3, 1); }
        | '@' VARIABLE '[' expr ']' {$$ = ary(id($2,1), $4, 1);}
        | '*' VARIABLE '[' expr ']' {$$ = ary(id($2,3), $4, 1);}
        | ary '[' expr ']'     { $$ = ary($1, $3, 0); }
        ;
stmt_list:
          stmt                  { $$ = $1; }
        | stmt_list stmt        { $$ = opr(';', 2, $1, $2); }
        ;

expr:

          INTEGER               { $$ = con($1, 'a', NULL, consInt); }
        | CHAR                  { $$ = con(0, $1, NULL, consChar); }
        | STRING                { $$ = con(0, 'a', $1, consString);}
        | vari                  { $$ = $1; }
        | '-' expr %prec UMINUS { $$ = opr(UMINUS, 1, $2); }
        | expr '+' expr         { $$ = opr('+', 2, $1, $3); }
        | expr '-' expr         { $$ = opr('-', 2, $1, $3); }
        | expr '*' expr         { $$ = opr('*', 2, $1, $3); }
        | expr '%' expr         { $$ = opr('%', 2, $1, $3); }
        | expr '/' expr         { $$ = opr('/', 2, $1, $3); }
        | expr '<' expr         { $$ = opr('<', 2, $1, $3); }
        | expr '>' expr         { $$ = opr('>', 2, $1, $3); }
        | expr GE expr          { $$ = opr(GE, 2, $1, $3); }
        | expr LE expr          { $$ = opr(LE, 2, $1, $3); }
        | expr NE expr          { $$ = opr(NE, 2, $1, $3); }
        | expr EQ expr          { $$ = opr(EQ, 2, $1, $3); }
        | expr AND expr         { $$ = opr(AND, 2, $1, $3); }
        | expr OR expr          { $$ = opr(OR, 2, $1, $3); }
        | '(' expr ')'          { $$ = $2; }
        | vari '(' args_call ')'{ $$ = opr(CALL, 2, $1, $3);}
        | vari '(' ')'          { $$ = opr(CALL, 2, $1, NULL);}
        ;

%%

#define SIZEOF_NODETYPE ((char *)&p->con - (char *)p)




nodeType *con(int i, char c, char* s, consEnum type) {

    nodeType *p;
    size_t nodeSize;

    /* allocate node */
    nodeSize = SIZEOF_NODETYPE + sizeof(conNodeType);
    if ((p = malloc(nodeSize)) == NULL)
        yyerror("out of memory");

    /* copy information */
    p->type = typeCon;
    p->con.consType = type;
    switch(type){
        case consInt: 
            p->con.value.i = i;
            break;
        case consChar:
            p->con.value.c = c;
            break;
        case consString:
            p->con.value.s = s;
            break;
    }

    return p;
}

nodeType *id(char * i, int isGlobal) {

    nodeType *p;
    size_t nodeSize;

    /* allocate node */
    nodeSize = SIZEOF_NODETYPE + sizeof(idNodeType);
    if ((p = malloc(nodeSize)) == NULL)
        yyerror("out of memory");
    /* change to upper */
    int index = 0;
    for(index=0; index < strlen(i); index++){
        i[index] = toupper(i[index]);
    }
    /* copy information */

    p->type = typeId;
    p->id.i = i;
 

   // free(i);


    p->id.isGlobal = isGlobal;
    return p;
}
nodeType *ary(nodeType *subary, nodeType *arg, int isId ) {
    nodeType *p;
    size_t nodeSize;
    /* allocate node */
    nodeSize = SIZEOF_NODETYPE + sizeof(aryNodeType);
    if ((p = malloc(nodeSize)) == NULL)
        yyerror("out of memory");
    /* change to upper */
    p -> type = typeAry;
    p -> ary.i = subary -> ary.i;
    p -> ary.addr = arg;

    if (isId){
        p -> ary.subary = NULL;
        p -> ary.isGlobal = subary -> id.isGlobal;
    } else {
        p -> ary.subary = subary;
        p -> ary.isGlobal = subary -> ary.isGlobal;
    }
    return p;
    
}
nodeType *opr(int oper, int nops, ...) {

    va_list ap;
    nodeType *p;
    size_t nodeSize;
    int i;

    /* allocate node */
    nodeSize = SIZEOF_NODETYPE + sizeof(oprNodeType) +
        (nops - 1) * sizeof(nodeType*);
    if ((p = malloc(nodeSize)) == NULL)
        yyerror("out of memory");

    /* copy information */
    p->type = typeOpr;
    p->opr.oper = oper;
    p->opr.nops = nops;
    va_start(ap, nops);
    for (i = 0; i < nops; i++)
        p->opr.op[i] = va_arg(ap, nodeType*);
    va_end(ap);
    return p;
}




nodeType *create_func(nodeType *name, nodeType *arg, nodeType *stmt){

    nodeType *p;
    size_t nodeSize;


    nodeSize = SIZEOF_NODETYPE + sizeof(funcNodeType);
    if ((p = malloc(nodeSize)) == NULL)
        yyerror("out of memory");
    p->type = typeFunc; 

    if ((p->func.name = (char*) malloc(strlen(name -> id.i) + 1)) == NULL)
        yyerror("out of memory");
    strcpy(p->func.name, name -> id.i);
    p->func.arg = arg; 
    p->func.op = stmt;

    free(name->id.i);
    free(name);


    return p;
}

//FIXIT: ADD free;
void freeNode(nodeType *p) {
    int i; 

    

    if (!p) return;
    if (p->type == typeOpr) {
        for (i = 0; i < p->opr.nops; i++)
            freeNode(p->opr.op[i]);
    }
    if (p->type == typeFunc) {
        free(p->func.name);
        freeNode(p->func.arg);
        freeNode(p->func.op);
    }
    if (p->type == typeId){

        free(p->id.i);
    }

    free (p);
}

void freeStack(){
    int i; 
    for (i = 0; i < global_sym_top; i++){
        free(global_sym[i]);
    }
    for (i = 0; i < funcNameTop; i++){
        free(funcNames[i]);
    }
    for (i = 0; i < local_sym_top; i++){
        free(local_sym[i]);
    }
    for (i = 0; i<args_sym_top ; i++){
        free(args_sym[i]);
    }
}

void yyerror(char *s) {
    fprintf(stdout, "%s\n", s);
}

int main(int argc, char **argv) {
    in_func_argv_call = 0;
    cur_func_index = 0;
    global_sym_top = -1;
    funcNameTop = -1;
    global_ary_top = -1;
    in_array_definition = 0;
    extern FILE* yyin;
    yyin = fopen(argv[1], "r");
    yyparse();
    return 0;
}
