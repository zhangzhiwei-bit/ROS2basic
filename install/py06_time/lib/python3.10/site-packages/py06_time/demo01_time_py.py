import rclpy
from rclpy.node import Node
import threading
from rclpy.time import Time
from rclpy.time import Duration

class MyNode(Node):
    def __init__(self):
        super().__init__("time_node_py")
        # self.demo_rate()
        # self.demo_time()
        self.demo_duration()

    def demo_duration(self):
        du1 = Duration(seconds=10,nanoseconds=800000000)
        self.get_logger().info("s = %.2f,ns = %d" %(du1.seconds_nanoseconds()[0],du1.seconds_nanoseconds()[1]))



    def demo_time(self):
        t1 = Time(seconds=5,nanoseconds= 500000000)
        right_now = self.get_clock().now()

        self.get_logger().info("s = %.2f,ns = %d" %(t1.seconds_nanoseconds()[0],t1.seconds_nanoseconds()[1]))
        self.get_logger().info("s = %.2f,ns = %d" %(right_now.seconds_nanoseconds()[0],right_now.seconds_nanoseconds()[1]))

    def demo_rate(self):
        self.rate= self.create_rate(1.0)
        # 这种方法会导致程序阻塞
        # while rclpy.ok():
        #     self.get_logger().info("+++++++++++")
        #     self.rate.sleep()
        thread = threading.Thread(target=self.do_some)
        thread.start()
    
    def do_some(self):
        while rclpy.ok():
            self.get_logger().info("+++++++++++")
            self.rate.sleep()
        pass
def main():
    rclpy.init()
    rclpy.spin(MyNode())
    rclpy.shutdown()

 
if __name__ == '__main__':
    main()
