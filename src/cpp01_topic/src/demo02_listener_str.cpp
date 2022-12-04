/*  
    需求：订阅发布方发布的消息，并输出到终端。
    步骤：
        1.包含头文件；
        2.初始化 ROS2 客户端；
        3.定义节点类；
            3-1.创建订阅方；
            3-2.处理订阅到的消息。
        4.调用spin函数，并传入节点对象指针；
        5.释放资源。
*/

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
using std::placeholders::_1;

// 自定义节点类
class Listener:public rclcpp::Node {
    public:
        Listener():Node("listener_node_cpp"){
            RCLCPP_INFO(this->get_logger(),"订阅方创建");
            // 创建订阅方
            // 订阅方话题必须和发送方一致
            subscription_ = this->create_subscription<std_msgs::msg::String>("topic",10,std::bind(&Listener::do_cb,this,std::placeholders::_1));
            // 解析输出对象
        }

    private:
     void do_cb(const std_msgs::msg::String &msg){
        // 解析并输出数据
        RCLCPP_INFO(this->get_logger(),"订阅到的消息是:%s",msg.data.c_str());

     }
     rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
};

int main(int argc,char const *argv[]){
    rclcpp::init(argc,argv);
    rclcpp::spin(std::make_shared<Listener>());
    rclcpp::shutdown();
    return 0;
}