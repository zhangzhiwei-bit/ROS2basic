import rclpy
from rclpy.node import Node
from base_interfaces_demo.srv import AddInts

class AddIntsServer(Node):
    def __init__(self):
        super().__init__("add_inits_server_node_py")
        self.get_logger().info("服务器端创建了！(python)")
        self.server = self.create_service(AddInts,"add_ints",self.add)
    
    def add(self,request,response):
        response.sum=request.num1+request.num2
        self.get_logger().info('请求数据:(%d,%d),响应结果:%d' % (request.num1, request.num2, response.sum))
        return response


def main():
    rclpy.init()
    rclpy.spin(AddIntsServer())
    rclpy.shutdown()


if __name__ == '__main__':
    main()
