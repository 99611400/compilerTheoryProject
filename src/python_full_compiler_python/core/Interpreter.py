"""
获得运行时结果

"""

from core.ConstDefine import *
from error.RTError import RTError


class RTResult(object):
    def __init__(self):
        self.value = None
        self.error = None

    def success(self, value):
        self.value = value
        return self

    def failure(self, error):
        self.error = error
        return self

    def register(self, res):
        if res.error:
            self.error = res.error
        return res.value


class Number(object):
    def __init__(self, value):
        self.value = value
        self.set_pos()  # 报错的位置
        self.set_context()  # 方便定义错误,运行时 报错的上下文

    def set_pos(self, pos_start=None, pos_end=None):
        self.pos_start = pos_start
        self.pos_end = pos_end
        return self

    def set_context(self, context=None):
        self.context = context
        return self

    def added_by(self, other):
        if isinstance(other, Number):
            return Number(self.value + other.value).set_context(self.context), None

    def subbed_by(self, other):
        if isinstance(other, Number):
            return Number(self.value - other.value).set_context(self.context), None

    def muled_by(self, other):
        if isinstance(other, Number):
            return Number(self.value * other.value).set_context(self.context), None

    def dived_by(self, other):
        if isinstance(other, Number):
            if other.value == 0:
                return None, RTError(other.pos_start, other.pos_end, "分母不可为零", self.context)
            return Number(self.value / other.value).set_context(self.context), None

    def __repr__(self):
        return str(self.value)


"""
context 上下文
"""


class Context(object):
    def __init__(self, display_name, parent=None, parent_entry_pos=None):
        self.display_name = display_name
        self.parent = parent
        self.parent_entry_pos = parent_entry_pos


class Interpreter(object):
    def visit(self, node, context):
        """
        递归下降算法 实现 ast node
        :param node: 起始节点
        :param context:   上下文 方便定位错误
        :return:
        """
        method_name = f"visit_{type(node).__name__}"
        method = getattr(self, method_name, self.no_visit_method)
        return method(node, context)

    def no_visit_method(self):
        raise Exception(f"visit_{type(node).__name__}")

    def visit_NumberNode(self, node, context):
        return RTResult().success(
            # token 的 value
            Number(node.token.value).set_context(context).set_pos(node.pos_start, node.pos_end)
        )

    def visit_BinOpNode(self, node, context):
        res = RTResult()

        #  左递归
        left = res.register(self.visit(node.left_node, context))
        if res.error:
            return res
        #  右递归
        right = res.register(self.visit(node.right_node, context))
        if res.error:
            return res

        if node.op_token.type == TT_PLUS:
            result, error = left.added_by(right)
        elif node.op_token.type == TT_MINUS:
            result, error = left.subbed_by(right)
        elif node.op_token.type == TT_MUL:
            result, error = left.muled_by(right)
        elif node.op_token.type == TT_DIV:
            result, error = left.dived_by(right)

        if error:
            return res.failure(error)
        else:
            return res.success(result.set_pos(node.pos_start, node.pos_end))

    def visit_UnaryOpNode(self, node, context):
        res = RTResult()
        number = res.register(self.visit(node.node, context))
        if res.error:
            return res
        error = None
        if node.op_token.type == TT_MINUS:
            number, error = number.muled_by(Number(-1))
        if error:
            return res.failure(error)
        else:
            return res.success(number.set_pos(node.pos_start, node.pos_end))