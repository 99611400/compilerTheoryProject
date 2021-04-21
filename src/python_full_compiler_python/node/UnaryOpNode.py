class UnaryOpNode(object):
    # 一元操作 -1 , op_token -  , node = 1
    def __init__(self, op_token, node):
        self.op_token = op_token
        self.node = node


        self.pos_start = self.op_token.pos_start
        self.pos_end = self.node.pos_end


    def __repr__(self):
        return  f'({self.op_token},{self.node})'
