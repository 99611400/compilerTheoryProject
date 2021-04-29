class Token(object):
    def __init__(self, type, value=None, pos_start=None, pos_end=None):
        """
        :param type:
        :param value:
        :param pos_start: Position实例
        :param pos_end:
        """
        self.type = type
        self.value = value
        if pos_start:
            #  Token 单个字符串   +  => pos_start = pos_end , advance
            self.pos_start = pos_start.copy()
            self.pos_end = pos_start.copy()
            self.pos_end.advance(self.value)  # 下一个Token
        # token 是多个字符串 例如 123.456
        if pos_end:
            self.pos_end = pos_end

    def __repr__(self):
        if self.value:
            return f'{self.type}: {self.value}'
        return f'{self.type}'
