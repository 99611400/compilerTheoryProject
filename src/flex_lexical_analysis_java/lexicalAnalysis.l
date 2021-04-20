/*********************************************************************************
 * @author: 丁志鹏(OliverDD)
 * @email: oliverdd@std.uestc.edu.cn
 * @desctipiton: JAVA的词法分析器。因为不需要编写编译器后续部分，所以只是简单识别
 *               每一个Tokens并输出，而不保存并制作符号表。
 * @todo:
 *
*********************************************************************************/
%{
#define LOCAL
/*
        char TAB[][][] = {
                    {"abstract","default","goto*","null","switch","boolean","do","if","package","nchronzed","break",
                    "double","implements","private","this","byte","else","import","protected","throw","throws",
                    "case","extends","instanceof","public","transient","catch","false","int","return","true","char",
                    "final","interface","short","try","class","finally","long","static","void","const*","float",
                    "native","strictfp","volatile","continue","for","new","super","while","assert","enum"},
                    {"=","+","-","*","/","<",">","<=",">=","==","!=","&&","||","!","."},
                    {"{","}","(",")","[","]",";",":"}
                    };
*/
%}
/* 关键字定义 */
ABSTRACT                    abstract
DEFAULT                     default
GOTO                        goto
NULL                        null
SWITCH                      switch
BOOLEAN                     boolean
DO                          do
IF                          if
PACKAGE                     package
NCHRONZED                   nchronzed
BREAK                       break
DOUBLE                      double
IMPLEMENTS                  implements
PRIVATE                     private
THIS                        this
BYTE                        byte
ELSE                        else
IMPORT                      import
PROTECTED                   protected
THROW                       throw
THROWS                      throws
CASE                        case
EXTENDS                     extends
INSTANCEOF                  instanceof
PUBLIC                      public
TRANSIENT                   transient
CATCH                       catch
INT                         int
RETURN                      return
CHAR                        char
FINAL                       final
INTERFACE                   interface
SHORT                       short
TRY                         try
CLASS                       class
FINALLY                     finally
LONG                        long
STATIC                      static
VOID                        void
CONST                       const
FLOAT                       float
NATIVE                      native
STRICTFP                    strictfp
VOLATILE                    volatile
CONTINUE                    continue
FOR                         for
NEW                         new
SUPER                       super
WHILE                       while
ASSERT                      assert
ENUM                        enum
/* 关键字定义结束 */

/* 运算符定义 */
QUES                        \?
COLON                       [:]
ASSIGNOP                    [=]
RELOP                       [>]|[<]|[>][=]|[<][=]|[=][=]|[!][=]
PLUS                        [+]
SUB                         [-]
MUL                         [*]
DIV                         [/]
AND                         [&][&]
OR                          [|][|]
/* 运算符定义结束 */

/* 分界符定义 */
ATT                         \@
DOT                         [.]
COMMA                       [,]
NOT                         [!]
SEM                         [;]
LP                          \(
RP                          \)
LB                          \[
RB                          \]
LC                          \{
RC                          \}
/* 分界符定义结束 */

/* 常数定义 */
STA_INT                         (-?[1-9][0-9]*|0)
STA_FLOAT                       -?(0|[1-9]+)\.[0-9]*
STA_CHAR                        \'[a-zA-Z]\'
STA_STRING                      \".*\"
STA_BOOLEAN                     (true|false)
/* 常数定义结束 */

/* 标识符定义 */
ID                          [a-zA-Z_$][a-zA-Z_$]*
/* 标识符定义结束 */

LINECOMMENT                 \/\/.*

COMMENT                     ((\/\*((\*[^\/]+)|[^\*])+\*\/)|(\/\/))

%option noyywrap
%%
{ABSTRACT}                  |
{DEFAULT}                   |
{GOTO}                      |
{NULL}                      |
{SWITCH}                    |
{BOOLEAN}                   |
{DO}                        |
{IF}                        |
{PACKAGE}                   |
{NCHRONZED}                 |
{BREAK}                     |
{DOUBLE}                    |
{IMPLEMENTS}                |
{PRIVATE}                   |
{THIS}                      |
{BYTE}                      |
{ELSE}                      |
{IMPORT}                    |
{PROTECTED}                 |
{THROW}                     |
{THROWS}                    |
{CASE}                      |
{EXTENDS}                   |
{INSTANCEOF}                |
{PUBLIC}                    |
{TRANSIENT}                 |
{CATCH}                     |
{INT}                       |
{RETURN}                    |
{CHAR}                      |
{FINAL}                     |
{INTERFACE}                 |
{SHORT}                     |
{TRY}                       |
{CLASS}                     |
{FINALLY}                   |
{LONG}                      |
{STATIC}                    |
{VOID}                      |
{CONST}                     |
{FLOAT}                     |
{NATIVE}                    |
{STRICTFP}                  |
{VOLATILE}                  |
{CONTINUE}                  |
{FOR}                       |
{NEW}                       |
{SUPER}                     |
{WHILE}                     |
{ASSERT}                    |
{ENUM}                      {
                                printf("< 关键字 , %10s >\n", yytext);
                            }
{QUES}                      |
{COLON}                     |
{ASSIGNOP}                  |
{RELOP}                     |
{PLUS}                      |
{SUB}                       |
{MUL}                       |
{DIV}                       |
{AND}                       |
{OR}                        {
                                printf("< 运算符 , %10s >\n", yytext);
                            }
{ATT}                       |
{COMMA}                     |
{DOT}                       |
{NOT}                       |
{SEM}                       |
{LP}                        |
{RP}                        |
{LB}                        |
{RB}                        |
{LC}                        |
{RC}                        {
                                printf("< 分界符 , %10s >\n", yytext);
                            }
{STA_INT}                   |
{STA_FLOAT}                 |
{STA_CHAR}                  |
{STA_STRING}                |
{STA_BOOLEAN}               {
                                printf("<  常数  , %10s >\n", yytext);
                            }
{ID}                        {
                                printf("< 标识符 , %10s >\n", yytext);
                            }
{LINECOMMENT}               |
{COMMENT}                   |
[ \n\t]                       //eat the white space
%%
int main(int argc, char *argv[]){
    argc--;
    argv++;
    if(argc == 0){
        printf("致命错误：无输入常数\n");
        return 1;
    }else{
        while(argc > 0){
            yyin = fopen(argv[0], "r");
            yylex();
            argv++;
            argc--;
        }
    }
}
