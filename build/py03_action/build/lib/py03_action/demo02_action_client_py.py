import rclpy
from rclpy.node import Node
from rclpy.action import ActionClient,client
from base_interfaces_demo.action import Progress,_progress
import sys
from rclpy.logging import get_logger
from rclpy import task


class ProgressActionClient(Node):
    def __init__(self):
        super().__init__("progress_action_server_node_py")
        self.get_logger().info("动作通信客户端创建！")
        self.client  = ActionClient(self,Progress,"get_sum")


    # 发送请求
    def send_goal(self,num):
        # 连接服务端
        self.client.wait_for_server()
        # 发送请求
        goal = Progress.Goal()
        goal.num = num
        self.future =self.client.send_goal_async(goal,self.fb_callback)
        self.future.add_done_callback(self.goal_response_callback)

    # 处理关于目标值的服务端响应的回调函数
    def goal_response_callback(self,future:task.Future):
        # 获取目标句柄
        goal_handle = future.result()
        
        # 判断目标是否被正常接收
        if not goal_handle.accepted:
            self.get_logger().info("目标被拒绝了！")
            return
        self.get_logger().info("目标被接收正在处理中......")
        
        # 处理最终响应结果
        self.result_future=goal_handle.get_result_async()
        self.result_future.add_done_callback(self.get_result_callback)
    
    # 处理最终响应的回调函数
    def get_result_callback(self,future):
        result=future.result().result
        self.get_logger().info("最终结果:%d" %result.sum)

    
    # 处理连续反馈的回调函数
    def fb_callback(self,fb_msg):
        progress = fb_msg.feedback.progress
        self.get_logger().info("连续反馈数据%.2f" %progress)

def main():
    if len(sys.argv)!=2:
        get_logger("rclpy").error("请提交一个整型数据")
        return
    rclpy.init()
    action_client = ProgressActionClient()
    action_client.send_goal(int(sys.argv[1]))
    rclpy.spin(action_client)
    rclpy.shutdown()

if __name__=='__main__':
    main()