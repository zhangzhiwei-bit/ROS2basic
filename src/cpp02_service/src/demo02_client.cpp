#include "rclcpp/rclcpp.hpp"
#include "base_interfaces_demo/srv/add_ints.hpp"

using base_interfaces_demo::srv::AddInts;
using namespace std::chrono_literals;

class AddIntsClient:public rclcpp::Node{
  public:
    AddIntsClient():Node("add_ints_client_node_cpp"){
      RCLCPP_INFO(this->get_logger(),"客户端节点创建");
      client_ = this->create_client<AddInts>("add_ints");


  }
    //连接服务器
    bool connect_server(){
        // 在指定时间内连接服务器。成功返回true，错误返回false
        while (!client_->wait_for_service(2s)) //循环以2s为超时时间连接服务器
        {
            // 对ctrl+c进行特殊处理
            if(!rclcpp::ok()){
                 RCLCPP_INFO(rclcpp::get_logger("rclcpp"),"强行终止客户端");
                return false;
            }
            RCLCPP_INFO(rclcpp::get_logger("rclcpp"),"服务连接中！");
        }
        return true;
    }
    // 发送请求，参数是两个整型数据
    rclcpp::Client<AddInts>::FutureAndRequestId send_request(int num1,int num2){
        // 组织请求数据，并发送
        auto request = std::make_shared<AddInts::Request>();
        request->num1=num1;
        request->num2=num2;
        return client_->async_send_request(request);   
    }

  private:
    rclcpp::Client<AddInts>::SharedPtr client_;
};

int main(int argc,char const *argv[]){
    if(argc!=3){
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"),"请提交两个整型数字");
        return 1;
    }
    rclcpp::init(argc,argv);
    //创建客户端对象
    auto client = std::make_shared<AddIntsClient>();
    // 调用客户端对象连接服务器
    bool flag = client->connect_server();
    if(!flag){
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"),"服务器连接失败，程序退出！");
        return 0;
    }
    // 调用请求提交函数，接收并处理相应结果
    auto feature = client->send_request(atoi(argv[1]),atoi(argv[2]));
    if(rclcpp::spin_until_future_complete(client,feature)==rclcpp::FutureReturnCode::SUCCESS){
        RCLCPP_INFO(client->get_logger(),"响应成功！sum=%d",feature.get()->sum);
    }else{
        RCLCPP_INFO(client->get_logger(),"响应失败");
    }
    

    rclcpp::shutdown();
    return 0;
}