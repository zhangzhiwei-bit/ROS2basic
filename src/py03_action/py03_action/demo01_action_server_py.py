import time
import rclpy
from rclpy.node import Node
from rclpy.action import ActionServer,server,GoalResponse,CancelResponse
from base_interfaces_demo.action import Progress

class ProgressActionServer(Node):
    def __init__(self):
        super().__init__("progress_action_client_node_py")
        self.get_logger().info("动作通信服务端创建！")
        # 创建动作服务对象
        self.server = ActionServer(
            self,
            Progress,
            'get_sum',
            self.execute_callback,
            goal_callback=self.goal_callback,
            cancel_callback=self.cancel_callback
        )

    def cancel_callback(self,cancel_request):
        self.get_logger().info("接收到任务取消请求")
        return CancelResponse.ACCEPT
    
    def goal_callback(self,goal_request):
        if goal_request.num<=1:
            self.get_logger().info("提交的目标值必须大于1")
            return GoalResponse.REJECT
        self.get_logger().info("提交的目标值合法")
        return GoalResponse.ACCEPT
                
    def execute_callback(self,goal_handle:server.ServerGoalHandle):
        self.get_logger().info("开始执行任务")
        # 生成连续反馈
        num=goal_handle.request.num
        sum=0
        result = Progress.Result()
        feedback = Progress.Feedback()
        for i in range(1,num+1):
            sum+=i
            feedback.progress=i/num
            goal_handle.publish_feedback(feedback)
            self.get_logger().info("连续反馈：%.2f" %feedback.progress)
            
            # if not goal_handle.is_active:
            #     result.sum=sum
            #     goal_handle.canceled(result)
            #     self.get_logger().info("任务被取消了")
            #     return;
            time.sleep(1.0)
        # 响应最终结果
        goal_handle.succeed()
       
        result.sum = sum
        self.get_logger().info("计算结果：%d" % result.sum)
        return result

   

def main():
    rclpy.init()
    rclpy.spin(ProgressActionServer())
    rclpy.shutdown()

if __name__=='__main__':
    main()