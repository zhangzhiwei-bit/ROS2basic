#include "rclcpp/rclcpp.hpp"

using namespace std::chrono_literals;
class ParamClient:public rclcpp::Node{
    public:
        ParamClient():Node("param_client_node_cpp"){
            RCLCPP_INFO(this->get_logger(),"参数客户端创建了");
            // 创建参数客户端对象
            // 参数1、所依赖的节点，2、远程连接服务端的节点名称
            param_client_ = std::make_shared<rclcpp::SyncParametersClient>(this,"param_server_node_cpp");
        }
        bool connect_server(){
            while(!this->param_client_->wait_for_service(1s)){
                if(!rclcpp::ok()){
                    return false;
                }
                RCLCPP_INFO(this->get_logger(),"服务连接中......");
            }
            return true;
        }
        // 查询参数
        void get_param(){
                RCLCPP_INFO(this->get_logger(),"参数查询操作......");
                // 获取某个参数
                std::string car_name = param_client_->get_parameter<std::string>("car_name");
                double width = param_client_->get_parameter<double>("width");
                RCLCPP_INFO(this->get_logger(),"car_name=%s",car_name.c_str());
                RCLCPP_INFO(this->get_logger(),"width=%.2f",width);
                // 获取多个参数
                auto params = param_client_->get_parameters({"car_name","width","wheels"});
                for(auto &&param:params){
                    RCLCPP_INFO(this->get_logger()," %s = %s ",param.get_name().c_str(),param.value_to_string().c_str());
                }
                // 判断是否包含某个参数
                RCLCPP_INFO(this->get_logger(),"包含car_name吗？%d",param_client_->has_parameter("car_name"));
                RCLCPP_INFO(this->get_logger(),"包含heigh吗？%d",param_client_->has_parameter("height"));
        }
        // 修改参数
        void update_param(){
                RCLCPP_INFO(this->get_logger(),"参数修改操作......");
                param_client_->set_parameters({
                    rclcpp::Parameter("car_name","dog"),
                    rclcpp::Parameter("width",3.33),
                    rclcpp::Parameter("length")//增加一个原本不存在的值
                });
        }

    private:
        rclcpp::SyncParametersClient::SharedPtr param_client_;
};

int main(int argc,char const *argv[]){
  rclcpp::init(argc,argv);
  auto client = std::make_shared<ParamClient>();
  bool flag = client->connect_server();
  if(!flag){
    return 0;
  }
  client->get_param();
  client->update_param();
  client->get_param();
//   rclcpp::spin();
  rclcpp::shutdown();
  return 0;
}