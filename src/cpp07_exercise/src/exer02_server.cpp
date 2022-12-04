#include "rclcpp/rclcpp.hpp"
#include "turtlesim/msg/pose.hpp"
#include "base_interfaces_demo/srv/distance.hpp"

/*
计算客户端提交的目标点坐标，获取原生乌龟坐标，计算二者距离响应返回
创建一个订阅方：获取原生乌龟位姿 /turtle1/pose
服务端解析客户端数据并响应结果到客户端
*/
using std::placeholders::_1;
using std::placeholders::_2;
using base_interfaces_demo::srv::Distance;

class Exer02Server:public rclcpp::Node{
    public:
        Exer02Server():Node("exer02_server_node_cpp"),x(0.0),y(0.0){
            RCLCPP_INFO(this->get_logger(),"服务端创建了");
             // 创建订阅方
            sub_=this->create_subscription<turtlesim::msg::Pose>("/turtle1/pose",10,std::bind(&Exer02Server::pose_cb,this,_1));
            // 创建一个服务端
            // server_=this->create_service<Distance>("distance",std::bind(&Exer02Server::distance_cb,this,_1,_2));
            // 使用lambda表达式
            server_=this->create_service<Distance>("distance",[=](const Distance::Request::SharedPtr request,Distance::Response::SharedPtr response){
                // 解析出目标点坐标
                float goal_x=request->x;
                float goal_y=request->y;
                //根据原生乌龟坐标计算距离
                float distance_x=goal_x-x;
                float distance_y=goal_y-y;
                float distance=std::sqrt(distance_x*distance_x+distance_y*distance_y);
                // 设置到响应里面
                response->distance=distance;
                RCLCPP_INFO(this->get_logger(),"目标点坐标(%.2f,%.2f),原生乌龟坐标(%.2f,%.2f),二者距离:%.2f",
                goal_x,goal_y,x,y,distance);
            });
        }
    private:
        float x,y;
        void pose_cb(const turtlesim::msg::Pose::SharedPtr pose){
            x=pose->x;
            y=pose->y;
        }
        void distance_cb(const Distance::Request::SharedPtr request,Distance::Response::SharedPtr response){
            // 解析出目标点坐标
            float goal_x=request->x;
            float goal_y=request->y;
            //根据原生乌龟坐标计算距离
            float distance_x=goal_x-x;
            float distance_y=goal_y-y;
            float distance=std::sqrt(distance_x*distance_x+distance_y*distance_y);
            // 设置到响应里面
            response->distance=distance;
            RCLCPP_INFO(this->get_logger(),"目标点坐标(%.2f,%.2f),原生乌龟坐标(%.2f,%.2f),二者距离:%.2f",
            goal_x,goal_y,x,y,distance);
        }
        rclcpp::Subscription<turtlesim::msg::Pose>::SharedPtr sub_;
        rclcpp::Service<Distance>::SharedPtr server_;
};

int main(int argc,char *argv[]){
    rclcpp::init(argc,argv);
    rclcpp::spin(std::make_shared<Exer02Server>());
    rclcpp::shutdown();
}