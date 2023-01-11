digit   [0-9]
letter  [A-Za-z]
crlf    '\r\n'
at      (@)

%%

"nick"      printf("NICK");
"quit"      exit(0);
{digit}     printf("DIGIT");
{digit}+    printf("DIGITS");
{letter}+   printf("IDENTIFIER");
{crlf}      printf("CRLF");

%%
int main(void){
    yylex();
    return 0;
}
