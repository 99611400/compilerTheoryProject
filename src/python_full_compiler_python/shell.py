from core.Lexer import Lexer
from core.Parser import Parser
from core.Interpreter import Interpreter
from core.Interpreter import Context


fromFile = False


def run(fn, text):
    #  词法解析
    lexer = Lexer(fn, text)
    tokens, error = lexer.make_tokens()
    print(tokens)

    #  生成 抽象语法树 (ast) 语法分析
    parser = Parser(tokens)
    ast = parser.parse()
    print(ast.node)

    # 解释器
    interpreter = Interpreter()
    context = Context('<program>')
    result = interpreter.visit(ast.node, context)

    return result.value, result.error
    # return ast.node,ast.error


if not fromFile:
    while True:
        text = input("basic >")
        res, error = run("<stdin>", text)
        if error:
            print(error.as_string())
        else:
            print(res)
else:
    with open("input") as file:
        text = file.read()
        print("input文件的内容为\n--------------------\n"+text+"-------------------\n")
    res, error = run("<file>", text)
    if error:
        print(error.as_string())
    else:
        print(res)

