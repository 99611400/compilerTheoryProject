class Error(object):
    def __init__(self, pos_start, pos_end, error_name, details):
        self.pos_start = pos_start  # 错误起始位置
        self.pos_end = pos_end  # 错误终止位置
        self.error_name = error_name  # 错误类型名称
        self.details = details  # 错误细节

    def as_string(self):
        res = f'{self.error_name}:{self.details}'
        res += f'File {self.pos_start.fn},line {self.pos_end.row + 1}'
        return res


