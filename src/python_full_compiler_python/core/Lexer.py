from core.Position import Position
from core.ConstDefine import *
from error.IllegalCharError import IllegalCharError
from core.Token import Token

class Lexer(object):
    def __init__(self, fn, text):
        self.fn = fn
        self.text = text
        self.pos = Position(-1, 0, -1, fn, text)
        self.current_char = None
        self.advance()

    def advance(self):
        self.pos.advance(self.current_char)
        if self.pos.idx < len(self.text):
            self.current_char = self.text[self.pos.idx]  # 内部 idx已经+1 所以获取到了下一个字符
        else:
            self.current_char = None

    def make_tokens(self):
        tokens = []
        """
        1. 遍历text
        2. 遍历的过程中，分别判断获取的内容 
        """
        while self.current_char is not None:
            if self.current_char in (' ', '\t'):
                # 空格或者制表符不处理
                self.advance()
            elif self.current_char == "\n":
                self.advance()
            elif self.current_char in DIGITS:
                tokens.append(self.make_number())
            elif self.current_char == "+":
                tokens.append(Token(TT_PLUS, pos_start=self.pos))
                self.advance()
            elif self.current_char == "-":
                tokens.append(Token(TT_MINUS, pos_start=self.pos))
                self.advance()
            elif self.current_char == "*":
                tokens.append(Token(TT_MUL, pos_start=self.pos))
                self.advance()
            elif self.current_char == "/":
                tokens.append(Token(TT_DIV, pos_start=self.pos))
                self.advance()
            elif self.current_char == "(":
                tokens.append(Token(TT_LPAREN, pos_start=self.pos))
                self.advance()
            elif self.current_char == ")":
                tokens.append(Token(TT_RPAREN, pos_start=self.pos))
                self.advance()
            else:
                # 没有匹配到 非法字符错误
                pos_start = self.pos.copy()
                char = self.current_char
                self.advance()
                return [], IllegalCharError(pos_start, self.pos, f"'{char}'")
        tokens.append(Token(TT_EOF, pos_start=self.pos))
        return tokens, None

    def make_number(self):
        """
        识别整数 或者小数
        """
        num_str = ''
        dot_count = 0  # 记录小数点的个数 用于 判断读入的是整形数字还是小数
        pos_start = self.pos.copy()

        while self.current_char is not None and self.current_char in DIGITS + '.':
            if self.current_char == '.':
                if dot_count == 1:
                    break
                dot_count += 1
                num_str += '.'
            else:
                num_str += self.current_char
            self.advance()
        if dot_count == 0:
            return Token(TT_INT, int(num_str),pos_start,self.pos)
        else:
            return Token(TT_FLOAT, float(num_str),pos_start,self.pos)
