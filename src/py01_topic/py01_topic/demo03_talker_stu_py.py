from email import message
import rclpy
from rclpy.node import Node
from base_interfaces_demo.msg import Student

class TalkerStu(Node):
    def __init__(self):
        super().__init__("Talker_node_py")
        self.count=0
        self.get_logger().info("发布方创建了(python)")
        self.publisher = self.create_publisher(Student,"chatter",10)
        self.timer = self.create_timer(1.0,self.on_timer)

    def on_timer(self):
        stu = Student()
        stu.name = "ros"
        self.count+=1
        stu.age = self.count
        stu.height = 1.80
        self.publisher.publish(stu)
        self.get_logger().info("发布的数据为:%s,%d,%.2f" %(stu.name,stu.age,stu.height))


def main():
    rclpy.init()
    rclpy.spin(TalkerStu())
    rclpy.shutdown()

if __name__=='__main__':
    main()