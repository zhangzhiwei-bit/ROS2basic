#include "rclcpp/rclcpp.hpp"

class ParamServer:public rclcpp::Node{
  public:
    ParamServer():Node("param_server_node_cpp",rclcpp::NodeOptions().allow_undeclared_parameters(true)){
      RCLCPP_INFO(this->get_logger(),"参数服务端创建了");
    }
    // 新增
    void declare_param(){
        RCLCPP_INFO(this->get_logger(),"--------增-------");
        this->declare_parameter("car_name","tiger");
        this->declare_parameter("width",1.55);
        this->declare_parameter("wheels",5);
        this->set_parameter(rclcpp::Parameter("heigh",2.00));
    }
    // 查
    void get_param(){
        RCLCPP_INFO(this->get_logger(),"--------查-------");
        // 获取指定参数
        auto car = this->get_parameter("car_name");
        RCLCPP_INFO(this->get_logger(),"key=%s,value=%s",car.get_name().c_str(),car.as_string().c_str());
        // 获取一些参数
        auto params = this->get_parameters({"car_name","width","wheels"});
        for(auto &&param:params){
            RCLCPP_INFO(this->get_logger(),"(%s = %s)",param.get_name().c_str(),param.value_to_string().c_str());
        }
        RCLCPP_INFO(this->get_logger(),"是否包含car_name:%d",this->has_parameter("car_name"));
        RCLCPP_INFO(this->get_logger(),"是否包含height:%d",this->has_parameter("height"));
    }
    // 改
    void update_param(){
        RCLCPP_INFO(this->get_logger(),"--------改-------");
        this->set_parameter(rclcpp::Parameter("width",1.75));
        RCLCPP_INFO(this->get_logger(),"width=%.2f",this->get_parameter("width").as_double());
    }
    // 删
    void del_param(){
        RCLCPP_INFO(this->get_logger(),"--------删-------");
        // this->undeclare_parameter("car_name");//不能删除声明的参数
        this->undeclare_parameter("heigh");//可以删除未声明而设置的一些数据
        RCLCPP_INFO(this->get_logger(),"删除后还包含heigh吗？%d",this->has_parameter("heigh"));
    }

};

int main(int argc,char const *argv[]){
  rclcpp::init(argc,argv);
  auto node = std::make_shared<ParamServer>();
  node->declare_param();
  node->get_param();
  node->update_param();
  node->del_param();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}