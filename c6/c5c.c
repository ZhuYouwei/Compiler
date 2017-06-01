#include <stdio.h>
#include "calc3.h"
#include "y.tab.h"
#include <stdlib.h>
#include <string.h>

int ex(nodeType *p, int blbl, int clbl, int inFunc);
static int lbl;

void print_local(){
    int i = 0;
    printf("global_table\n");
    for (i =0; i <= global_sym_top; i++){
        printf("%d: [%s]\n", i, global_sym[i]);
    }

    i = 0;
    printf("local_table\n");
    for (i =0; i <= local_sym_top; i++){
        fprintf(stderr, "%d: [%s]\n", i, local_sym[i]);
    }
    i = 0;
    printf("loca_ary_table\n");
    for (i =0; i <= local_ary_top; i++){
        printf("%d: [%s]\n", i, local_ary_infos[i]->name);
    }
    i = 0;
    printf("args_table\n");
    for (i =0; i <= args_sym_top; i++){
        printf("%d: [%s]\n", i, args_sym[i]);
    }
printf("func names\n");
    for (i =0; i <= funcNameTop; i++){
        printf("%d: [%s]\n", i, funcNames[i]);
    }
}


int get_sym_index(char *sym, int inFunc, int isGlobal){
    // printf("global_sym_top=%d, args_sym_top=%d, local_sym_top =%d\n", global_sym_top, args_sym_top, local_sym_top);

    int i=0;
    if (inFunc == 0 || isGlobal == 1){
        // not in function, only check global_sym
        for (i= 0; i<=global_sym_top; i++){
            if(strcmp(sym, global_sym[i]) == 0){
                return i;
            }
        }
        
    } else {
        // in func, local
        //search argv
        for (i= 0; i<=args_sym_top; i++){
            if(strcmp(sym, args_sym[i]) == 0){
                return (i - 4 - args_sym_top);
            }
        }
        //search local
        for (i= 0; i<=local_sym_top; i++){
            if(strcmp(sym, local_sym[i]) == 0){
                return i;
            }
        }
    }
    return -1;

}
int push_sym(char *sym, int inFunc, int isGlobal){
    // if sym == 0, it's offset
    if (get_sym_index(sym, inFunc, isGlobal) == -1 || sym[0] == '0' ){

        if (inFunc == 0 || isGlobal == 1){
            // in main, or isGlobal variable, push to global_sym
            global_sym_top++;
            global_sym[global_sym_top] = (char *) malloc(strlen(sym)+1 * sizeof(char));
            strcpy(global_sym[global_sym_top], sym);
            // if success return top
            return global_sym_top;
        } else {
            // in function, and local variable, push to local_sym
            local_sym_top++;
            local_sym[local_sym_top] = (char *) malloc(strlen(sym)+1 * sizeof(char));
            strcpy(local_sym[local_sym_top], sym);
            // if success return top
            return local_sym_top;
        }
    }
    // if failed return -1
    return -1;
}

int get_func_index(char *sym){
    if (get_sym_index(sym, 0, 0) != -1){
        printf("[Error] Function Name Clash\n");
        exit(1);
    }

    int i=0;
    for (i= 0; i<=funcNameTop; i++){
        if(strcmp(sym, funcNames[i]) == 0){
            return i;
        }
    }
    return -1;
}
int push_func(char *sym, int n_local, int argv_length){
    if (get_func_index(sym) == -1){
        funcNameTop++;
        lbl++;
        funcNames[funcNameTop] = (char *) malloc(strlen(sym)+1 * sizeof(char));
        func_local[funcNameTop] = n_local;
        func_argc[funcNameTop] = argv_length;


        strcpy(funcNames[funcNameTop], sym);
        // if success return top
        return funcNameTop;
    }
    // if failed return -1
    return -1;
}

int pop_sym_index(char *sym, int inFunc, int isGlobal){
    int index = get_sym_index(sym, inFunc, isGlobal);
    // printf("[inFunc=%d] [idGlobal=%d] index of %s: %d\n", inFunc, isGlobal, sym, index);

    if ((inFunc == 1 && isGlobal == 1) || inFunc == 0){
        // global_sym
        printf("\tpop\tsb[%d]\n",index);
    } else if (isGlobal != 2 && isGlobal != 3){
        //local_sym
        if (index == -1){
            index = push_sym(sym, inFunc, 0);
        }
        printf("\tpop\tfp[%d]\n",index);
    } else if (isGlobal == 3){ // *
       if (index < 0){ //argv
           printf("\tpush\tfp[%d]\n",index);
           printf("\tpop\tin\n");

       } else { // local
           printf("\tpush\tfp[-3]\n");
           printf("\tpush\t3\n"); //fp[0] - number of argv
           printf("\tadd\n"); 
           printf("\tpush\t%d\n",func_argc[cur_func_index]); // + argc
           printf("\tadd\n"); //fp
           printf("\tpush\t%d\n",index); //local index
           printf("\tadd\n");
           printf("\tpop\tin\n");
       }
       printf("\tpop\tsb[in]\n");
    }
    return 0; 
}
int push_sym_index(char *sym, int inFunc, int isGlobal){
    int index = get_sym_index(sym, inFunc, isGlobal);
    // printf("[inFunc=%d] [idGlobal=%d] index of %s: %d\n", inFunc, isGlobal, sym, index);

    if ((inFunc == 1 && isGlobal == 1) || inFunc == 0){
        // global_sym
        printf("\tpush\tsb[%d]\n",index);
    } else if (isGlobal != 2 && isGlobal != 3) {
        //local_sym
        if (index == -1){
            index = push_sym(sym, inFunc, 0);
        }
        printf("\tpush\tfp[%d]\n",index);
    }  else if (isGlobal == 2){ // & w1
            if (index < 0){ //argv
                printf("\tpush\tfp[%d]\n",index);
            } else { // local
                printf("\tpush\tfp[-3]\n");
                printf("\tpush\t3\n"); //fp[0] - number of argv
                printf("\tadd\n"); 
                printf("\tpush\t%d\n",func_argc[cur_func_index]); // + argc
                printf("\tadd\n"); //fp
                printf("\tpush\t%d\n",index); //local index
                printf("\tadd\n");
            }
    } else if (isGlobal == 3){ // *
        if (index < 0){ //argv
            printf("\tpush\tfp[%d]\n",index);
            printf("\tpop\tin\n");

        } else { // local
            printf("\tpush\tfp[-3]\n");
            printf("\tpush\t3\n"); //fp[0] - number of argv
            printf("\tadd\n"); 
            printf("\tpush\t%d\n",func_argc[cur_func_index]); // + argc
            printf("\tadd\n"); //fp
            printf("\tpush\t%d\n",index); //local index
            printf("\tadd\n");
            printf("\tpop\tin\n");
        }
        printf("\tpush\tsb[in]\n");
    }
    return 0; 
}
//push offset to `in` register
int get_offset(nodeType *p,int blbl,int clbl,int inFunc){

    int ndim;
    nodeType *cur;
    nodeType *tmp; 

    //tmp = get_ary_index(p->ary.i,p->ary.isGlobal,inFunc);
    int pos = get_sym_index(p->ary.i,inFunc, p->ary.isGlobal);

    int inGlobal = p->ary.isGlobal == 1 || inFunc == 0;
    

    cur = p;



    ndim = 0;
    while (cur != NULL){
        ndim++;
        //length *= cur -> ary.addr -> con.value.i;
        cur = cur -> ary.subary;                  
    }
    cur = p;
    
    tmp = p; 
        for (int j =0; j <ndim -1; j++){
            tmp = tmp->ary.subary; 
    }

    ex(tmp->ary.addr, blbl, clbl, inFunc);

    for( int i = 2; i <= ndim; i++){
        if (inGlobal){//global 
            printf("\tpush\tsb[%d]\n",pos+ndim-i);
        } else if (pos < 0){// argv
            printf("\tpush\tfp[%d]\n",pos);//base to in
            printf("\tpop\tin\n");

            printf("\tpush\t%d\n",ndim-i);
            printf("\tpush\tin\n");
            printf("\tadd\n");
            printf("\tpop\tin\n"); //base + ndim-i to in

            printf("\tpush\tsb[in]\n");
        } else { //local 
            printf("\tpush\tfp[%d]\n",pos+ndim-i);
        }
        printf("\tmul\n");
        tmp = p; 
        for (int j =0; j <ndim -i; j++){
            tmp = tmp->ary.subary; 
        }
        ex(tmp->ary.addr, blbl, clbl, inFunc);
        printf("\tadd\n");
    }
    if (inGlobal || pos >= 0){//global local
        printf("\tpush\t%d\n",pos); // index
    } else {// argv
        printf("\tpush\tfp[%d]\n",pos); // index
    }
    printf("\tadd\n");    
    printf("\tpush\t%d\n", ndim);
    printf("\tadd\n");     // index + offset
    printf("\tpop\tin\n");
    return 0;
}
// For A[10][5] get_dim(p,1) = 10 get_dim(p,2) = 5
// nodeType* get_dim(nodeType *p, int n, int inFunc, int ){
//     nodeType *cur = p;
//     int index = get_ary_index(p->ary.i,p->ary.isGlobal, inFunc); 
//     int ndim = 0;
//     if (p->ary.isGlobal == 1 || inFunc == 0){//return global
//         ndim = global_ary_infos[index]->ndim;
//     } else { //return local
//         ndim = local_ary_infos[index]->ndim;
//     }
    
//     while(ndim>n){
//         cur = cur -> ary.subary;
//         ndim--;
//     }
//     return cur;

// }
// for local or global
int get_ary_index(char* name, int isGlobal, int inFunc){

    if (isGlobal || !inFunc){
        for (int i = 0; i <= global_ary_top;i++){
            if(strcmp(global_ary_infos[i]->name,name) == 0) return i;
        }   
    } else {
        //search local
        for (int i = 0; i <= local_ary_top;i++){
            if(strcmp(local_ary_infos[i]->name,name) == 0) return i;
        } 
    }
    return -1;
}

int firstpass(nodeType *p, int inFunc){

        int argv_length;
        int vari_length;
        args_sym_top = -1;
        switch(p->type) {

        
        nodeType *cur;
        int length;
        int ndim; 
        int index; 
        case typeAry:
            /* 
                push symbol and offset for global
            */
            if (get_ary_index(p->ary.i, p ->ary.isGlobal,inFunc) != -1){
                break;
            }
            // push symbol to symbol table
            push_sym(p->ary.i, inFunc, p ->ary.isGlobal);
            length = 1;
            cur = p;
            ndim = 0;
            while (cur != NULL){
                ndim++;
                cur = cur -> ary.subary;
            }
            array_info *ary_info = (array_info *) malloc(sizeof(array_info));
            ary_info->name = p->ary.i;
            ary_info->ndim = ndim; 
            //ary_info->dim = (int *) malloc(ndim * sizeof(int));
            ary_info->dim = malloc(ndim * sizeof(int));
            cur = p;
            index = 0;
            while (cur != NULL){
                ary_info->dim[index] = cur -> ary.addr -> con.value.i;
                length *= cur -> ary.addr -> con.value.i;
                //printf("[%d]", cur -> ary.addr -> con.value.i);
                cur = cur -> ary.subary;
                index++;
            }
            // push symbol to ary table
            if (p->ary.isGlobal || inFunc == 0){
                global_ary_infos[++global_ary_top]=ary_info;
            } else {
                local_ary_infos[++local_ary_top]=ary_info;
            }
            
            for (int i = 1; i < length + ndim; i++){
                push_sym("0", inFunc, p -> ary.isGlobal);
            }
            // for (int i = 0; i <= global_ary_top; i++){
            //     printf("%s, dim = %d", global_ary_infos[i]->name, global_ary_infos[i]->ndim);
            //     for (int j = 0; j < global_ary_infos[i]->ndim; j++){
            //         printf("[%d]", global_ary_infos[i]->dim[j]);
            //     }
            //     printf("\n");
            // }

            //printf("Array Name: %s\n", p->ary.i);
            //printf("Array Length: %d\n", length);
            // printf("Infunc: %d\n", inFunc);
            // printf("isGlobal: %d\n", p -> ary.isGlobal);
            break;

        case typeFunc:

            // length = 0;
            // cur = p->func.arg;
            // while (cur != NULL){
            //     length ++;
            //     // fprintf(stderr, "        %s\n", cur->arg.name);
            //     cur = cur->arg.next;
            // }
            // p->func.argc = length;
            // fprintf(stderr, "   stmt:\n");

            // printf("Found an function definition\n");
            // printf("   name: %s\n", p->func.name);


            // printf("   argc: %d\n", p->func.argc);
            // Cal local_sym_top
            // Cal argv_length
            local_sym_top = -1;
            argv_length = 0;
            local_ary_top = -1;
            cur = p->func.arg; 
            if (cur != NULL){
                argv_length++;
                while (cur->type != typeId){
                     if (cur->type != typeOpr){
                        printf("Syntax Error, argument of function definition must be variable\n");
                        exit(1);
                     }
                     argv_length++;
                     cur = cur->opr.op[1];
                }
            }

            firstpass(p->func.op, 1);

            //printf("NUMBER OF LOCAL_VAR = %d\n", local_sym_top+1);
            push_func(p->func.name, local_sym_top+1, argv_length);
            break;

        case typeOpr:
            switch(p->opr.oper) {
            case MAIN:
                firstpass(p->opr.op[0], inFunc);
                firstpass(p->opr.op[1], inFunc);
                break;
            case FOR:
                firstpass(p->opr.op[0], inFunc);
                firstpass(p->opr.op[1], inFunc);
                firstpass(p->opr.op[3], inFunc);
                firstpass(p->opr.op[2], inFunc);// body
                break;
            case WHILE:
                firstpass(p->opr.op[1], inFunc); //body
                break;
            case IF:
                if (p->opr.nops > 2) {
                    firstpass(p->opr.op[1], inFunc);
                    firstpass(p->opr.op[2], inFunc);
                } else {
                    /* if */
                    firstpass(p->opr.op[1], inFunc);
                }
                break;
            case GETI:
            case GETC:
            case GETS:
                if(p->opr.op[0]->type == typeId) {
                    vari_length = strlen(p->opr.op[0]->id.i);
                    if (vari_length > 12){
                        printf("Variable Name invaliad\n");
                        exit(1);
                    }

                    push_sym(p->opr.op[0]->id.i, inFunc, p->opr.op[0]->id.isGlobal);
                } 
                break;
            case '=':   

                if(p->opr.op[0]->type == typeId) {
                    vari_length = strlen(p->opr.op[0]->id.i);
                    if (vari_length > 12){
                        printf("Variable Name invaliad\n");
                        exit(1);
                    }
                    push_sym(p->opr.op[0]->id.i, inFunc, p->opr.op[0]->id.isGlobal);
                } 
                break;
            case ARRAY:
                firstpass(p->opr.op[0],inFunc);
                break;
            case ',':
            case ';':
                firstpass(p->opr.op[0],inFunc);
                firstpass(p->opr.op[1],inFunc);
                break;
             // error message
        }
        default:
            break;
    }
        

    return 0;
}

void print_global(){
    printf("\tpush\tsp\n");
    printf("\tpush\t%d\n", global_sym_top + 1);
    printf("\tadd\n");
    printf("\tpop\tsp\n");
}


int ex(nodeType *p, int blbl, int clbl, int inFunc){


    int lblx, lbly, lblz;
    int index;
    nodeType *cur; 
    int offset;
    int argv_length;
    int ndim;
    int tmp;
    int length;
    int pos;

    if (!p) {
        return 0;
    }

    switch(p->type) {
    case typeFunc:
        // push to args_sym
        // cur = p->func.arg;
        args_sym_top = -1;
        // while (cur != NULL){
        //     args_sym_top++;
        //     args_sym[args_sym_top] = (char *) malloc(strlen(cur->arg.name)+1 * sizeof(char));
        //     strcpy(args_sym[args_sym_top], cur->arg.name);
        //     cur = cur->arg.next;
        // }
        cur = p->func.arg; 
        if (cur != NULL){
            while (cur->type != typeId){
                args_sym_top++;
                args_sym[args_sym_top] = (char *) malloc(strlen(cur->opr.op[0]->id.i)+1 * sizeof(char));
                strcpy(args_sym[args_sym_top], cur->opr.op[0]->id.i);
                cur = cur->opr.op[1];

            }
            //printf("[PARA]: %s", cur->id.i);
            args_sym_top++;
            args_sym[args_sym_top] = (char *) malloc(strlen(cur->id.i)+1 * sizeof(char));
            strcpy(args_sym[args_sym_top], cur->id.i);
        }

       
        //print jmp statement
        lblx = lbl++;
        printf("\tjmp\tL%03d\n", lblx);
        printf("L%03d:\n", get_func_index(p->func.name));
        // update sp
        printf("\tpush\tsp\n");
        printf("\tpush\t%d\n", func_local[get_func_index(p->func.name)]);
        printf("\tadd\n");
        printf("\tpop\tsp\n");
        //function definition
        local_sym_top = -1;
        local_ary_top = -1;
        cur_func_index = get_func_index(p->func.name);
        //printf("finding %s; updated cur func index to %d\n",p->func.name,  cur_func_index);

        ex(p->func.op, blbl, clbl, 1);
        //return 0 if no return value specified
        printf("\tpush\t0\n");
        printf("\tret\n");         
        //function end
        printf("L%03d:\n", lblx);
        cur_func_index = -1;
        args_sym_top = -1;
        local_sym_top = -1; //clear the local
        local_ary_top = -1;
        break;



    case typeCon:
        switch(p->con.consType){
            case consInt:
                printf("\tpush\t%d\n", p->con.value.i);
                break;
            case consChar:
                printf("\tpush\t'%c'\n", p->con.value.c);
                break;
            case consString:
                printf("\tpush\t\"%s\"\n", p->con.value.s);
                break;
        }
        // printf("\tpush\t%d\n", p->con.value);
        break;
    case typeId:
        index = get_sym_index(p->id.i, inFunc,p->id.isGlobal);
        tmp = get_ary_index(p->id.i, p->id.isGlobal, inFunc);
        if (index == -1){
            printf("[ERR] Undeclared variable: %s \n", p->id.i );
            exit(1);
        }
        //printf("ary index[%d]",tmp);
        //print_local();
        if (in_func_argv_call && tmp != -1){// array pass addr relative to sb
            //pass addr global: index, local: sp + argv + ... + index
            if (!inFunc || p->id.isGlobal){
                printf("\tpush\t%d\n",index);
            } else {
                if (index < 0){ //argv
                    printf("\tpush\tfp[%d]\n",index);
                } else { // local
                    printf("\tpush\tfp[-3]\n");
                    printf("\tpush\t3\n"); //fp[0] - number of argv
                    printf("\tadd\n"); 
                    //printf("cur_func_index = %d\n",  cur_func_index);
                    //printf("func name = %s\n", funcNames[cur_func_index]);

                    //print_local();

                    printf("\tpush\t%d\n",func_argc[cur_func_index]); // + argc
                    printf("\tadd\n"); //fp
                    printf("\tpush\t%d\n",index); //local index
                    printf("\tadd\n");
                }
            }
            break;
        }

        // get the value if isGlobal = 2 or 3;
        if (p->id.isGlobal == 2){ // &
                if (index < 0){ //argv
                    printf("\tpush\tfp[%d]\n",index);
                } else { // local
                    printf("\tpush\tfp[-3]\n");
                    printf("\tpush\t3\n"); //fp[0] - number of argv
                    printf("\tadd\n"); 
                    printf("\tpush\t%d\n",func_argc[cur_func_index]); // + argc
                    printf("\tadd\n"); //fp
                    printf("\tpush\t%d\n",index); //local index
                    printf("\tadd\n");
                }
            break;
        } else if (p->id.isGlobal == 3){ // *
            printf("\tpush\tfp[%d]\n",index);
            printf("\tpop\tin\n");
            printf("\tpush\tsb[in]\n");
            break;
        }
        // isGlobal = 0 or 1;
        push_sym_index(p->id.i, inFunc, p->id.isGlobal);
        //printf("\tpush\t%s\n", p->id.i);
        break;
    case typeAry:
        // index = get_sym_index(p->ary.i, inFunc,p->ary.isGlobal);
        if (in_array_definition){// it's array def
            //TODO: 
            //1. push local_sym_table : same size as firstpass
            //2. generate code to save dim info for both local and global
            if (!p->ary.isGlobal&&inFunc){
                push_sym(p->ary.i, inFunc, p->ary.isGlobal);
                //Local_ary_info
                array_info *ary_info = (array_info *) malloc(sizeof(array_info));
                ary_info->name = p->ary.i;
                local_ary_infos[++local_ary_top]=ary_info;
           

                length = 1;
                cur = p;
                ndim = 0;
                while (cur != NULL){
                    ndim++;
                    length *= cur -> ary.addr -> con.value.i;
                    cur = cur -> ary.subary;                  
                }
                // printf("ndim = %d\n", ndim);
                // fflush(stdout);
                for (int i = 1; i < length + ndim; i++){
                    push_sym("0", inFunc, p -> ary.isGlobal);
                }
            }
            int pos = get_sym_index(p->ary.i, inFunc, p->ary.isGlobal);
            cur = p;
            while (cur != NULL){

                printf("\tpush\t%d\n", cur -> ary.addr -> con.value.i);
                if (p->ary.isGlobal || inFunc == 0){
                    printf("\tpop\tsb[%d]\n",pos);
                } else {
                    printf("\tpop\tfp[%d]\n",pos);
                }
                //printf("[%d]", cur -> ary.addr -> con.value.i);
                cur = cur -> ary.subary;
                pos++;
            }  
            break;          
        }
        if(!in_array_definition){
            /*
                a[i]:
                    offset = i;
                a[j][i]: for a [J][I]
                    offset = j*I + i;
                a[k][j][i]: for a [K][J][I]
                    offset = (k*J + j)*I + i;

                i = cur -> ary.addr evaluated result;
                j = cur -> ary.subary -> ary.addr evaluated result;

                index = getindex(a) + offset;
            */
            get_offset(p,blbl,clbl,inFunc);
            if (inFunc == 0 || p->ary.isGlobal == 1 || p->ary.isGlobal == 3){
                printf("\tpush\tsb[in]\n"); //push the index
            } else {
                if (get_sym_index(p->ary.i,inFunc,p->ary.isGlobal) >= 0){
                    printf("\tpush\tfp[in]\n"); //push the index 
                } else { // in argv
                    printf("\tpush\tsb[in]\n");
                }    
            }
        }
        break;
        
    case typeOpr:
        switch(p->opr.oper) {
        case EMPTY:
            break;

        case MAIN:
                   
            ex(p->opr.op[0], blbl, clbl, inFunc);
            ex(p->opr.op[1], blbl, clbl, inFunc);
            break;
        case BREAK:
            printf("\tjmp\tL%03d\n", blbl);
            break;
        case CONTINUE:
            printf("\tjmp\tL%03d\n", clbl);
            break;
        case FOR:
            lblx = lbl++;
            lbly = lbl++;
            lblz = lbl++;
            ex(p->opr.op[0], blbl, clbl, inFunc);
            printf("L%03d:\n", lblx);
            ex(p->opr.op[1], blbl, clbl, inFunc);
            printf("\tj0\tL%03d\n", lbly);
            ex(p->opr.op[3], lbly, lblz, inFunc);
            printf("L%03d:\n", lblz);
            ex(p->opr.op[2], blbl, clbl, inFunc);
            printf("\tjmp\tL%03d\n", lblx);
            printf("L%03d:\n", lbly);
            break;
        case WHILE:
            lblx = lbl++;
            lbly = lbl++;
            printf("L%03d:\n", lblx);
            ex(p->opr.op[0], blbl, clbl, inFunc);
            printf("\tj0\tL%03d\n", lbly);
            ex(p->opr.op[1], lbly, lblx, inFunc);
            printf("\tjmp\tL%03d\n", lblx);
            printf("L%03d:\n", lbly);
            break;
        case IF:
            ex(p->opr.op[0], blbl, clbl, inFunc);
            if (p->opr.nops > 2) {
                /* if else */
                printf("\tj0\tL%03d\n", lblx = lbl++);
                ex(p->opr.op[1], blbl, clbl, inFunc);
                printf("\tjmp\tL%03d\n", lbly = lbl++);
                printf("L%03d:\n", lblx);
                ex(p->opr.op[2], blbl, clbl, inFunc);
                printf("L%03d:\n", lbly);
            } else {
                /* if */
                printf("\tj0\tL%03d\n", lblx = lbl++);
                ex(p->opr.op[1], blbl, clbl, inFunc);
                printf("L%03d:\n", lblx);
            }
            break;
        case RETURN:
            ex(p->opr.op[0], blbl, clbl, inFunc);
            if (inFunc) printf("\tret\n");
            break;
        case CALL:
            // push all the argument index p->opr.op[1]
            // int length = 0;
            // while (cur != NULL){
            //     length ++;
            //     push_sym_index(cur->arg.name,inFunc,0);
            //     cur = cur->arg.next;
            // }
            index = get_func_index(p->opr.op[0]->id.i);
            in_func_argv_call = 1;
            
            // push argv
            ex(p->opr.op[1], blbl, clbl, inFunc);
            in_func_argv_call = 0;
            
            // call func
            printf("\tcall\tL%03d,\t%d\n",index, func_argc[index]);

            break;
        case GETI:
            printf("\tgeti\n");
            if(p->opr.op[0]->type == typeId) {
                pop_sym_index(p->opr.op[0]->id.i, inFunc, p->opr.op[0]->id.isGlobal);
            } 
            break;
        case GETC:
            printf("\tgetc\n");
            if(p->opr.op[0]->type == typeId) {
                pop_sym_index(p->opr.op[0]->id.i, inFunc, p->opr.op[0]->id.isGlobal);
            } 
            break;
        case GETS:
            printf("\tgets\n");
            if(p->opr.op[0]->type == typeId) {
                pop_sym_index(p->opr.op[0]->id.i, inFunc, p->opr.op[0]->id.isGlobal);
            } 
            break;
        case PUTI:
            ex(p->opr.op[0], blbl, clbl, inFunc);
            printf("\tputi\n");
            break;
        case PUTC:
            ex(p->opr.op[0], blbl, clbl, inFunc);
            printf("\tputc\n");
            break;
        case PUTS:
            ex(p->opr.op[0], blbl, clbl, inFunc);
            printf("\tputs\n");
            break;
        case PUTI_:
            ex(p->opr.op[0], blbl, clbl, inFunc);
            printf("\tputi_\n");
            break;
        case PUTC_:
            ex(p->opr.op[0], blbl, clbl, inFunc);
            printf("\tputc_\n");
            break;
        case PUTS_:
            ex(p->opr.op[0], blbl, clbl, inFunc);
            printf("\tputs_\n");
            break;
        case '=':
            ex(p->opr.op[1], blbl, clbl, inFunc);
            if(p->opr.op[0]->type == typeId) {
                pop_sym_index(p->opr.op[0]->id.i, inFunc, p->opr.op[0]->id.isGlobal); 
            } else if (p->opr.op[0]->type == typeAry){
                // printf("P:name[%s]",p->opr.op[0]->ary.i);
                // fflush(stdout);
                index = get_sym_index(p->opr.op[0]->ary.i,inFunc,p->opr.op[0]->ary.isGlobal);
                get_offset(p->opr.op[0],blbl,clbl,inFunc);
                if (p->opr.op[0]->ary.isGlobal || inFunc == 0 || index < 0){
                    printf("\tpop\tsb[in]\n"); //push the index for argv or global
                } else {
                    printf("\tpop\tfp[in]\n"); //push the index
                }   
            }
            break;
        case UMINUS:
            ex(p->opr.op[0], blbl, clbl, inFunc);
            printf("\tneg\n");
            break;
        case ARRAY:
            in_array_definition = 1;
            ex(p->opr.op[0], blbl, clbl, inFunc);
            if (p->opr.nops == 2){
                ex(p->opr.op[1], blbl, clbl, inFunc);

                length = 1;
                cur = p->opr.op[0];                
                ndim = 0;                
                while (cur != NULL){                    
                    ndim++;                    
                    length *= cur -> ary.addr -> con.value.i;                    
                    cur = cur -> ary.subary;                                  
                }
               
                pos = get_sym_index(p->opr.op[0]->ary.i, inFunc, p->opr.op[1]->ary.isGlobal);
            // global   
                //init in to the start of the array            
                printf("\tpush\t0\n");
                printf("\tpush\t%d\n", pos);
                printf("\tadd\n");
                printf("\tpush\t%d\n", ndim);
                printf("\tadd\n");
                printf("\tpop\tin\n");
                lblx = lbl++;
                lbly = lbl++; 
                lblz = lbl++;
                //entry judgement
                printf("L%03d:\n", lblx);
                printf("\tpush\t%d\n", pos+ndim+length);
                printf("\tpush\tin\n");
                printf("\tcompge\n");
                printf("\tj1\tL%03d\n", lbly);
                printf("\tjmp\tL%03d\n", lblz);
                //loop body
                printf("L%03d:\n", lbly);
                if (p->opr.op[1]->ary.isGlobal == 1 || !inFunc){
                    printf("\tpop\tsb[in]\n");
                    printf("\tpush\tsb[in]\n");
                }
                else{
                    printf("\tpop\tfp[in]\n");
                    printf("\tpush\tfp[in]\n");
                }
                printf("\tpush\tin\n");
                printf("\tpush\t1\n");
                printf("\tadd\n");
                printf("\tpop\tin\n");
                printf("\tjmp\tL%03d\n", lblx);
                //finalize
                printf("L%03d:\n", lblz);
                printf("\tpop\tin\n");
                

            }

            in_array_definition = 0;
            break;
        case ',':
            ex(p->opr.op[0], blbl, clbl, inFunc);
            ex(p->opr.op[1], blbl, clbl, inFunc);
            break;
        default:
            ex(p->opr.op[0], blbl, clbl, inFunc);
            ex(p->opr.op[1], blbl, clbl, inFunc);
            switch(p->opr.oper) {
                case '+':   printf("\tadd\n"); break;
                case '-':   printf("\tsub\n"); break;
                case '*':   printf("\tmul\n"); break;
                case '/':   printf("\tdiv\n"); break;
                case '%':   printf("\tmod\n"); break;
                case '<':   printf("\tcompLT\n"); break;
                case '>':   printf("\tcompGT\n"); break;
                case GE:    printf("\tcompGE\n"); break;
                case LE:    printf("\tcompLE\n"); break;
                case NE:    printf("\tcompNE\n"); break;
                case EQ:    printf("\tcompEQ\n"); break;
                case AND:   printf("\tand\n"); break;
                case OR:    printf("\tor\n"); break;
                
            }
        }
    default:
        return 0; // error message
    }
    // printf("beofre return ex, inFunc = %d\n",inFunc );
    return 0;
}

void eop() {
    // add end of program label
    if (get_func_index("MAIN") != -1){
        printf("\tcall\tL%03d,\t%d\n",get_func_index("MAIN"), 0);
    }
    printf("\tjmp\tL999\n");
    printf("L998:\n");
    printf("\tpush\t999999\n");
    printf("\tputi\n");
    printf("L999:\n");
    printf("\tend\n");
}