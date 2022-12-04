/*  
  需求：以某个固定频率发送文本“hello world!”，文本后缀编号，每发送一条消息，编号递增1。
  步骤：
    1.包含头文件；
    2.初始化 ROS2 客户端；
    3.定义节点类；
      3-1.创建发布方；
      3-2.创建定时器；
      3-3.组织消息并发布。
    4.调用spin函数，并传入节点对象指针；
    5.释放资源。
*/

// 1.包含头文件；
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std::chrono_literals;

// 定义节点类
class MinimalPublisher:public rclcpp::Node{
  public:
    MinimalPublisher():Node("minimal_publisher"),count_(0){
      // 创建发布方
      /*
      参数一、话题名称
      参数二、QOS(消息队列长度)
      返回值：发布对象指针
      */
      publisher_ = this->create_publisher<std_msgs::msg::String>("topic",10);
      // 创建定时器
      /*
      参数一、时间间隔（命名空间）
      参数二、回调函数
      返回值：定时器对象指针
      */
      timer_ = this->create_wall_timer(500ms,std::bind(&MinimalPublisher::timer_callback,this));

    }
  private:
    void timer_callback(){
      auto message = std_msgs::msg::String();
      message.data = "hello world! "+std::to_string(count_++);
      RCLCPP_INFO(this->get_logger(),"发布消息：'%s'",message.data.c_str());
      publisher_->publish(message);
    }
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
    size_t count_;
};

int main(int argc, char ** argv)
{
  // 2.初始化 ROS2 客户端；
  rclcpp::init(argc, argv);
  // 4.调用spin函数，并传入节点对象指针。
  rclcpp::spin(std::make_shared<MinimalPublisher>());
  // 5.释放资源；
  rclcpp::shutdown();
  return 0;
}
