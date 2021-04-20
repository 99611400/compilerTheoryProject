class BinOpNode(object):
    # 二元操作 + - * 、 , 1 + 2 left_node = 1 ，op_token + ， right_node = 2
    def __init__(self, left_node, op_token, right_node):
        self.left_node = left_node
        self.op_token = op_token
        self.right_node = right_node

    def __repr__(self):
        return f'({self.left_node}, {self.op_token}, {self.right_node})'
