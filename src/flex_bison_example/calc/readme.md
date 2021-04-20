执行步骤

```shell
> flex calc.l
> bison -d calc.y
> gcc  calc.tab.c  lex.yy.c -o calc
> calc.exe
```

