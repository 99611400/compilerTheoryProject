from core.Lexer import Lexer
from core.Parser import Parser
fromFile = False

def run(fn, text):
    lexer = Lexer(fn, text)
    tokens, error = lexer.make_tokens()
    print(tokens)
    # 生产ast
    parser = Parser(tokens)
    ast = parser.parse()
    return ast.node,ast.error


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

