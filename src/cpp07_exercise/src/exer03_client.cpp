#include "rclcpp/rclcpp.hpp"
#include "base_interfaces_demo/srv/distance.hpp"
using base_interfaces_demo::srv::Distance;
using namespace std::chrono_literals;
/*
客户端提交目标点坐标
*/


class Exer03Client:public rclcpp::Node{
    public:
        Exer03Client():Node("exer03_client_node_cpp"){
            RCLCPP_INFO(this->get_logger(),"客户端创建了");
           client_=this->create_client<Distance>("distance");
        }
        // 客户端连接服务端
        bool connect_server(){
            while(!client_->wait_for_service(1s)){
                // 用户终止的情况
                if(!rclcpp::ok()){
                    RCLCPP_INFO(rclcpp::get_logger("rclcpp"),"节点被强制退出！");
                    return false;
                }
                RCLCPP_INFO(this->get_logger(),"服务连接中");
            }
            return true;
        }
        // 发送请求数据
        rclcpp::Client<Distance>::FutureAndRequestId send_goal(float x,float y,float theta){
            std::shared_ptr<base_interfaces_demo::srv::Distance_Request> request=std::make_shared<Distance::Request>();
            request->x=x;
            request->y=y;
            request->theta=theta;
            return client_->async_send_request(request);
        }
    private:
        rclcpp::Client<Distance>::SharedPtr client_;
};

int main(int argc,char *argv[]){
    // 传入的参数有4个，加上当前文件，共有5个，argc=5
    if(argc!=5){
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"),"请提交x,y,theta三个参数");
        return 1;
    }
    
    // 解析提交的参数
    float goal_x=atof(argv[1]);
    float goal_y=atof(argv[2]);
    float goal_theta=atof(argv[3]);
    RCLCPP_INFO(rclcpp::get_logger("rclcpp:"),"%.2f,%.2f,%.2f",goal_x,goal_y,goal_theta);

    rclcpp::init(argc,argv);
    auto node=std::make_shared<Exer03Client>();
    bool flag=node->connect_server();
    if(!flag){
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"),"服务连接失败");
        return 1;
    }
    // 发送请求，处理响应
    auto future=node->send_goal(goal_x,goal_y,goal_theta);
    // 判断响应结果状态
    if(rclcpp::spin_until_future_complete(node,future)==rclcpp::FutureReturnCode::SUCCESS){
        RCLCPP_INFO(node->get_logger(),"两只乌龟距离%.2f",future.get()->distance);
    }else{
        RCLCPP_INFO(node->get_logger(),"服务响应失败！");
    }
    rclcpp::shutdown();
}