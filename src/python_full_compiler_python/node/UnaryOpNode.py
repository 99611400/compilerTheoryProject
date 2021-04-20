class UnaryOpNode(object):
    # 一元操作 -1 , op_token -  , node = 1
    def __init__(self, op_token, node):
        self.op_token = op_token
        self.node = node

    def __repr__(self):
        return  f'({self.op_token},{self.node})'
