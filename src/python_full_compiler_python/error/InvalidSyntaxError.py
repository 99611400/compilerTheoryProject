from error.Error import Error


class InvalidSyntaxError(Error):
    # 无效语法
    def __init__(self, pos_start, pos_end, detail):
        super().__init__(pos_start, pos_end, "InvalidSyntaxError", detail)
