import rclpy
from rclpy.node import Node
from std_msgs.msg import String

class Talker(Node):
    def __init__(self):
        super().__init__("taker_node_py")
        self.count = 0
        self.get_logger().info("发布方创建了(python)")
        # 消息类型，话题名称，队列长度，返回发布对象
        self.publisher = self.create_publisher(String,"chatter",10)
        self.timer = self.create_timer(1.0,self.on_timer)

    def on_timer(self):
        message = String()
        message.data = "hello world(python)!"+ str(self.count)
        self.publisher.publish(message)
        self.count+=1
        self.get_logger().info("发布的数据是:%s" %message.data)


def main():
    rclpy.init()
    rclpy.spin(Talker())
    rclpy.shutdown()


if __name__ == '__main__':
    main()
