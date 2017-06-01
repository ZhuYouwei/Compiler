typedef enum { typeCon, typeId, typeAry, typeOpr, typeFunc } nodeEnum;

typedef enum { consInt, consChar, consString} consEnum;

union constant{
    int i;
    char c;
    char *s; 
};



/* constants */
typedef struct {
    consEnum consType;
    union constant value;                  /* value of constant */
} conNodeType;

/* identifiers */
typedef struct {
    char * i;                      /* subscript to sym array */
    int isGlobal;
} idNodeType;

/* array */
typedef struct {
    char * i;                      /* name of array */
    int isGlobal;
    struct nodeTypeTag *addr;       /* size of array */
    struct nodeTypeTag *subary;
} aryNodeType;

/* operators */
typedef struct {
    int oper;                   /* operator */
    int nops;                   /* number of operands */
    struct nodeTypeTag *op[1];  /* operands (expandable) */
} oprNodeType;

/* function */
typedef struct{
    char* name;
    int argc; 
    struct nodeTypeTag *arg;   //Actually a linked list of nodes. 
    struct nodeTypeTag *op;  //The stmt in the list
} funcNodeType;


typedef struct nodeTypeTag {
    nodeEnum type;              /* type of node */

    /* union must be last entry in nodeType */
    /* because operNodeType may dynamically increase */
    union {
        conNodeType con;        /* constants */
        idNodeType id;          /* identifiers */
        oprNodeType opr;        /* operators */
        funcNodeType func;
        aryNodeType ary;
    };
} nodeType;


char* global_sym[1024];
int global_sym_top;
char *funcNames[1024];
int func_local[1024];
int func_argc[1024];
int funcNameTop;

// local symbol table
char* local_sym[1024];
int local_sym_top;
// argv symbol table
char* args_sym[128];
int args_sym_top;

// array 
nodeType* get_dim(nodeType *p, int n, int inFunc);
int get_ary_index(char* name, int isGlobal, int inFunc);
int get_offset(nodeType *p,int blbl,int clbl,int inFunc);

int get_sym_index(char *sym, int inFunc, int isGlobal);
int push_sym(char *sym, int inFunc, int isGlobal);
int get_func_index(char *sym);
int push_func(char *sym, int nlocal, int argv_length);

// pop and print sb or fp statment for vari
int pop_sym_index(char *sym, int inFunc, int isGlobal);
int push_sym_index(char *sym, int inFunc, int isGlobal);



struct array_info {
    char *name;
    int ndim;
    int *dim; 
};


typedef struct array_info array_info;

array_info *global_ary_infos[128]; 
array_info *local_ary_infos[128];
int global_ary_top;
int local_ary_top; 

int in_array_definition;
int in_func_argv_call;
int cur_func_index;

#define MAIN 999
#define EMPTY 998
#define CALL 997