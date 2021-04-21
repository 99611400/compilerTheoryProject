from core.ConstDefine import *
from node.NumberNode import NumberNode
from node.UnaryOpNode import UnaryOpNode
from node.BinOpNode import BinOpNode
from error.InvalidSyntaxError import InvalidSyntaxError


class Parser(object):
    def __init__(self, tokens):
        self.tokens = tokens
        self.token_idx = -1
        self.advance()

    def advance(self):
        self.token_idx += 1
        if self.token_idx < len(self.tokens):
            self.current_token = self.tokens[self.token_idx]
        return self.current_token

    def parse(self):
        res = self.expr()
        if not res.error and self.current_token.type != TT_EOF:
            # 返回报错
            return res.failure(InvalidSyntaxError(
                self.current_token.pos_start, self.current_token.pos_end,
                'expected  "+","-","*" or "/" '
            ))
        return res

    """
    factor -> INT | FLOAT
           -> (PLUS |MINUS) factor
           -> LPAREN expr R  PAREN
    """

    def factor(self):
        res = ParserResult()
        token = self.current_token
        if token.type in (TT_INT, TT_FLOAT):
            # factor -> INT | FLOAT
            res.register(self.advance())
            return res.success(NumberNode(token))
        elif token.type in (TT_PLUS, TT_MINUS):
            #  -> (PLUS |MINUS) factor
            res.register(self.advance())
            factor = res.register(self.factor())
            if res.error:
                return res
            return res.success(UnaryOpNode(token, factor))
        elif token.type == TT_LPAREN:
            res.register(self.advance())
            expr = res.register(self.expr())
            if res.error:
                return res
            if self.current_token.type == TT_RPAREN:
                res.register(self.advance())
                return res.success(expr)
            else:
                # 返回报错
                return res.failure(InvalidSyntaxError(
                    self.current_token.pos_start, self.current_token.pos_end,
                    'expected  ")" '
                ))

        else:
            # 返回报错
            return res.failure(InvalidSyntaxError(
                self.current_token.pos_start, self.current_token.pos_end,
                'expected  int or float '
            ))

    """
    term -> factor((MUL | DIV) factor)*
    """

    def term(self):
        #  term -> factor((MUL | DIV) factor)*
        return self.bin_op(self.factor, (TT_MUL, TT_DIV))

    """
    expr -> term ((PLUS | MINUS) term)*
    """

    def expr(self):
        return self.bin_op(self.term, (TT_PLUS, TT_MINUS))

    def bin_op(self, func, ops):
        # 递归调用 构建出AST
        res = ParserResult()
        left = res.register(func())
        while self.current_token.type in ops:
            op_token = self.current_token
            res.register(self.advance())
            right = res.register(func())
            left = BinOpNode(left, op_token, right)
        return res.success(left)


class ParserResult(object):
    def __init__(self):
        self.error = None
        self.node = None

    def success(self, node):
        self.node = node
        return self

    def failure(self, error):
        self.error = error
        return self

    def register(self, res):
        if isinstance(res, ParserResult):
            if res.error:
                self.error = res.error
            return res.node
        return res
