%{
#include <stdlib.h>
#include "calc3.h"
#include "y.tab.h"
void yyerror(char *);
%}

%%

'.'         {   //printf("char");
                yylval.cValue = yytext[1]; 
                return CHAR;
            }
\".*\"      {   //printf("String");
                
                yylval.strValue = (char*) malloc((yyleng - 2) * sizeof(char));
                memcpy(yylval.strValue, &yytext[1], yyleng - 2);
                return STRING;
            }

0           {
                yylval.iValue = atoi(yytext);
                return INTEGER;
            }

[1-9][0-9]* {
                yylval.iValue = atoi(yytext);
                return INTEGER;
            }

[-()<>=+*/;{}.%\[\],@&*] {
                return *yytext;
             }

">="        return GE;
"<="        return LE;
"=="        return EQ;
"!="        return NE;
"for"       return FOR;
"while"     return WHILE;
"if"        return IF;
"else"      return ELSE;
"print"     return PRINT;
"read"      return READ;
"&&"        return AND;
"||"        return OR;
"break"     return BREAK;
"continue"  return CONTINUE;
"return"    return RETURN;
"geti"      return GETI;
"puti"      return PUTI;
"puti_"     return PUTI_;
"getc"      return GETC;
"putc"      return PUTC;
"putc_"     return PUTC_;
"gets"      return GETS;
"puts"      return PUTS;
"puts_"     return PUTS_;
"array"     return ARRAY;

[a-zA-Z][a-zA-Z0-9]*   {   
                yylval.sIndex = (char*) malloc((yyleng) * sizeof(char) + 1);
                strcpy(yylval.sIndex, yytext);
                yylval.sIndex[yyleng] = '\0';
                return VARIABLE;
            }

\/\/.*\n    ;    /* skip comments */

[ \t\n]+    ;       /* ignore whitespace */

.           yyerror("Unknown character");

%%

int yywrap(void) {
    return 1;
}
