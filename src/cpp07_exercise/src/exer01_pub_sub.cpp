#include "rclcpp/rclcpp.hpp"
# include "geometry_msgs/msg/twist.hpp"
# include "turtlesim/msg/pose.hpp"

/*
订阅乌龟1的位姿信息，解析出线速度和角速度，生成并发布控制乌龟2运动的速度指令
  订阅话题：/turtle1/pose
  订阅消息：turtlesim/msg/Pose
          "x: 0.0
          y: 0.0
          theta: 0.0
          linear_velocity: 0.0
          angular_velocity: 0.0

  发布话题：/t2/turtle1/cmd_vel
  发布消息：geometry_msgs/msg/Twist
            "linear:（线速度）
            x: 0.0  --- 前后
            y: 0.0  --- 左右
            z: 0.0  --- 上下
          angular:（角速度）
            x: 0.0  --- 翻滚
            y: 0.0  --- 俯仰
            z: 0.0  ---左右转（偏航）
          "

*/
class Exer01PubSub:public rclcpp::Node{
  public:
    Exer01PubSub():Node("exer01_pub_sub_node_cpp"){
      RCLCPP_INFO(this->get_logger(),"案例1对象创建"); 
      // 发布方对象
      pub_=this->create_publisher<geometry_msgs::msg::Twist>("/t2/turtle1/cmd_vel",10);
      // 订阅方对象
      sub_ = this->create_subscription<turtlesim::msg::Pose>("/turtle1/pose",10,std::bind(&Exer01PubSub::pose_cb,this,std::placeholders::_1));
    }
  private:
    void pose_cb(const turtlesim::msg::Pose &pose){
      // 订阅方的回调函数，处理速度，并发布乌龟2的速度指令
      // 创建速度指令
      geometry_msgs::msg::Twist twist;
      twist.linear.x = pose.linear_velocity;
      twist.angular.z = -pose.angular_velocity;
      // 发布
      pub_->publish(twist);
    }
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr pub_;
    rclcpp::Subscription<turtlesim::msg::Pose>::SharedPtr sub_;
};

int main(int argc,char const *argv[]){
  rclcpp::init(argc,argv);
  rclcpp::spin(std::make_shared<Exer01PubSub>());
  rclcpp::shutdown();
  return 0;
}