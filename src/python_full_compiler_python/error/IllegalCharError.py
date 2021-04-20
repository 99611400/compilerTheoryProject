from error.Error import Error


class IllegalCharError(Error):
    # 非法字符错误
    def __init__(self, pos_start, pos_end, detail):
        super().__init__(pos_start, pos_end, "Illegal Character", detail)


