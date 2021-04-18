
/* 关键字定义 */
AUTO      				auto
BREAK     				break
CASE      				case
D_CHAR      		    char
CONST     				const
CONTINUE  				continue
DEFAULT   				default
DO        				do
D_DOUBLE    		    double
ELSE      				else
ENUM      				enum
EXTERN    				extern
D_FLOAT     		    float
FOR       				for
GOTO      				goto
IF        				if
D_INT       			int
D_LONG      			long
REGISTER  				register
RETURN    				return
D_SHORT     			short
SIGNED    				signed
SIZEOF    				sizeof
STATIC    				static
STRUCT    				struct
SWITCH    				switch
TYPEDEF   				typedef
UNION     				union
UNSIGNED  				unsigned
D_VOID      			void
VOLATILE  				volatile
WHILE     				while
/* 关键字定义结束 */

/* 运算符定义 */
QUES                        \?
COLON                       [:]
ASSIGNMENT                  [=]
RELOP                       [<][=]|[>][=]|[<][=]|[=][=]|[!][=]
PLUS                        [+]
SUB                         [-]
MUL                         [*]
DIV                         [/]
AND                         [&][&]
OR                          [|][|]
/* 运算符定义结束 */

/* 分界符定义 */
AT                          \@
DOT                         [.]
COMMA                       [,]
NOT                         [!]
SEM                         [;]
LEFT_LITTLE_EBRACKET        \(
RIGHT_LITTLE_BRACKET        \)
LEFT_MIDDLE_BRACKET         \[
RIGHT_MIDDLE_BRACKET        \]
LEFT_LARGE_BRACKET          \{
RIGHT_LARGE_BRACKET         \}
/* 分界符定义结束 */

/* 常数定义 */
INT                         ([1-9][0-9]*|0)
FLOAT                       (0|[1-9]+)\.[0-9]*
CHAR                        \'[a-zA-Z]\'
STRING                      \".*\"
BOOLEAN                     (true|false)
/* 常数定义结束 */

/* 标识符定义 */
ID                          [a-zA-Z_$][a-zA-Z_$]*
/* 标识符定义结束 */

LINECOMMENT                 \/\/.*
COMMENT                     ((\/\*((\*[^\/]+)|[^\*])+\*\/)|(\/\/))

%option noyywrap
%%
{AUTO}                  |
{BREAK}                 |
{CASE}                  |
{CONST}                 |
{CONTINUE}              |
{DEFAULT}               |
{DO}                    |
{D_CHAR}                |
{D_DOUBLE}              |
{D_FLOAT}               |
{D_INT}                 |
{D_LONG}                |
{D_SHORT}               |
{D_VOID}                |
{ELSE}                  |
{ENUM}                  |
{EXTERN}                |
{FOR}                   |
{GOTO}                  |
{IF}                    |
{REGISTER}              |
{RETURN}                |
{SIGNED}                |
{SIZEOF}                |
{STATIC}                |
{STRUCT}                |
{SWITCH}                |
{TYPEDEF}               |
{UNION}                 |
{UNSIGNED}              |
{VOLATILE}              |
{WHILE}                 {
                            printf("< 关键字(1), %10s >\n", yytext);
                        }
{AND}                   |
{ASSIGNMENT}            |
{COLON}                 |
{DIV}                   |
{MUL}                   |
{OR}                    |
{PLUS}                  |
{QUES}                  |
{RELOP}                 |
{SUB}                   {
                            printf("< 运算符(4) , %10s >\n", yytext);
                        }

{INT}                   |
{FLOAT}                 |
{CHAR}                  |
{STRING}                |
{BOOLEAN}               {
                            printf("<  常数(3)  , %10s >\n", yytext);
                        }

{AT}                    |
{COMMA}                 |
{DOT}                   |
{LEFT_LARGE_BRACKET}    |
{LEFT_LITTLE_EBRACKET}  |
{LEFT_MIDDLE_BRACKET}   |
{NOT}                   |
{RIGHT_LARGE_BRACKET}   |
{RIGHT_LITTLE_BRACKET}  |
{RIGHT_MIDDLE_BRACKET}  |
{SEM}                   {
                            printf("< 分割符(5) , %10s >\n", yytext);
                        }
{ID}                    {
                            printf("< 标识符(2) , %10s >\n", yytext);
                        }
{LINECOMMENT}           |
{COMMENT}               |
[ \n\t]                       //注释 空格与换行 和 制表符 忽略

.                   { printf("错误输入(6) %10s",yytext);        }
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
