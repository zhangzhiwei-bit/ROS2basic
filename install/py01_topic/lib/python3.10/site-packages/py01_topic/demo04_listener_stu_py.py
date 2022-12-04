import rclpy
from rclpy.node import Node
from base_interfaces_demo.msg import Student

class ListenerStu(Node):
    def __init__(self):
        super().__init__("Listener_node_py")
        self.get_logger().info("订阅方创建了(python)")
        self.subscription = self.create_subscription(Student,"chatter",self.do_cb,10)
    
    def do_cb(self,stu):
        self.get_logger().info("接收到的数据为:%s,%d,%.2f" %(stu.name,stu.age,stu.height))

def main():
    rclpy.init()
    rclpy.spin(ListenerStu())
    rclpy.shutdown()

if __name__=='__main__':
    main()