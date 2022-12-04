#include "rclcpp/rclcpp.hpp"

using namespace std::chrono_literals;
//修改turtlesim_node节点的背景颜色

// 定义节点类
class Exer06Param:public rclcpp::Node{
  public:
    Exer06Param():Node("exer06_param_node_cpp"){
        RCLCPP_INFO(this->get_logger(),"参数客户端");
        // 创建参数客户端
        client_=std::make_shared<rclcpp::SyncParametersClient>(this,"/turtlesim");
    }
    // 连接服务端
    bool connect_server(){
      while(!client_->wait_for_service(1s)){
        if(!rclcpp::ok()){
          RCLCPP_INFO(rclcpp::get_logger("rclcpp"),"客户端强制退出");
          return false;
        }
        RCLCPP_INFO(this->get_logger(),"服务连接中！");
      }
      return true;
    }
    // 更新参数
    void update_param(){
      // 背景色递进式的修改
      // 获取参数
      int red=client_->get_parameter<int>("background_r");
      // 编写循环，修改参数
      rclcpp::Rate rate(30.0);
      int count=red;
      while(rclcpp::ok()){
       // red+=5;
        //red=red%255;
	if(count<=255){
		red+=5;
	}else{
		red-=5;
	}
	count+=5;
	if(count>511){
		count=0;
	}
        // 修改服务器端参数
        client_->set_parameters({rclcpp::Parameter("background_r",red)});
        rate.sleep();
      }
      // 
    }
  private:
    rclcpp::SyncParametersClient::SharedPtr client_;
};

int main(int argc, char ** argv)
{
  // 2.初始化 ROS2 客户端；
  rclcpp::init(argc, argv);
  // 4.调用spin函数，并传入节点对象指针。
  // rclcpp::spin(std::make_shared<Exer06Param>());
  auto client=std::make_shared<Exer06Param>();
  if(!client->connect_server()){
    return 1;
  }
  client->update_param();
  // 5.释放资源；
  rclcpp::shutdown();
  return 0;
}
