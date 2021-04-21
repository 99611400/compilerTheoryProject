from error.Error import Error


class RTError(Error):
    # 运行时错误
    def __init__(self, pos_start, pos_end, detail, context):
        super().__init__(pos_start, pos_end, "Runtime Error", detail)
        self.context = context
    def as_string(self):
        result = self.generate_traceStack()
        result = f'{self.error_name}:{self.details}'
        result += f'File {self.pos_start.fn},line {self.pos_end.row + 1}'
        return result

    def generate_traceStack(self):
        """
        生成错误栈信息
        :return:
        """
        result  = ''
        pos = self.pos_start
        ctxt = self.context
        while ctxt:
            result = f' File {pos.fn} line {str(pos.row+1)}, in {ctx.display_name}\n' +result
            pos = ctxt.parent_entry_pos
            ctxt = ctxt.parent
        return "Traceback (most recent call last):\n" + result