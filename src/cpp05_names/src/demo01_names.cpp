#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

class MyNode:public rclcpp::Node{
  public:
    MyNode():Node("mynode_node_cpp","t1_ns"){
      // 全局话题，和命名空间、节点名称没关系
      // pub_=this->create_publisher<std_msgs::msg::String>("/topic1",10);
      // 相对话题，位于命名空间下面
      pub_=this->create_publisher<std_msgs::msg::String>("topic1",10);
      // 私有话题，位于命名空间和节点名称之下
      pub_=this->create_publisher<std_msgs::msg::String>("~/topic1",10);
    }
  private:
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr pub_;
};

int main(int argc,char const *argv[]){
  rclcpp::init(argc,argv);
  rclcpp::spin(std::make_shared<MyNode>());
  rclcpp::shutdown();

}