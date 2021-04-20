class Position(object):
    def __init__(self, idx, row, col, fn, text):
        self.idx = idx  # 索引
        self.row = row  # 行号
        self.col = col  # 列号
        self.fn = fn  # 文件名 自定义报错
        self.text = text  # 内容

    def advance(self, current_char):
        self.idx += 1  # 索引加一
        self.col += 1  # 列号加一
        if current_char == "\n":
            self.col = 0
            self.row += 1

    def copy(self):
        return Position(self.idx, self.row, self.col, self.fn, self.text)