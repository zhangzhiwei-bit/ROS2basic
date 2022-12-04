import rclpy
from rclpy.node import Node
from rclpy.logging import get_logger
import sys
from base_interfaces_demo.srv import AddInts


class AddIntsClient(Node):
    def __init__(self):
        super().__init__("add_inits_client_node_py")
        self.get_logger().info("客户端创建了！(python)")
        self.client = self.create_client(AddInts,"add_ints")
        self.request = AddInts.Request()
        while not self.client.wait_for_service(timeout_sec=1.0):
            self.get_logger().info('服务连接中，请稍候...')
         
    
    def send_request(self):
        self.request.num1=int(sys.argv[1])
        self.request.num2=int(sys.argv[2])
        self.future = self.client.call_async(self.request)
        return self.future

def main():
    if len(sys.argv)!=3:
        get_logger("rclpy").error("请提交两个整型数据")
        return
    rclpy.init()
    client = AddIntsClient()
    # 发送请求
    future=client.send_request()
    rclpy.spin_until_future_complete(client,future)
    try:
        response = future.result()
        client.get_logger().info("响应结果：%d + %d = %d" %(client.request.num1,client.request.num2,response.sum))
    except Exception:
        client.get_logger().error("服务响应失败！")
    rclpy.shutdown()


if __name__ == '__main__':
    main()
