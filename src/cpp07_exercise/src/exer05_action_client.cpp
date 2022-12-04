#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "base_interfaces_demo/action/nav.hpp"

using base_interfaces_demo::action::Nav;
using namespace std::chrono_literals;
using std::placeholders::_1;
using std::placeholders::_2;
/*
向动作服务端发送目标点数据，并处理响应结果
需要解析launch文件传入的参数
*/

class Exer05ActionClient:public rclcpp::Node{
    public:
        Exer05ActionClient():Node("exer04_action_client_node_cpp"){
            RCLCPP_INFO(this->get_logger(),"动作客户端！");
            // 创建动作客户端
            client_ = rclcpp_action::create_client<Nav>(this,"nav");
        }
        // 连接服务端，发送请求
        void send_goal(float x,float y,float theta){
            // 连接服务端
            if(!client_->wait_for_action_server(10s)){
                RCLCPP_INFO(this->get_logger(),"服务连接超时");
                return ;
            }
            // 组织并发布数据
            Nav::Goal goal;
            goal.goal_x = x;
            goal.goal_y = y;
            goal.goal_theta = theta;
            rclcpp_action::Client<Nav>::SendGoalOptions options;

            //std::function<void (std::shared_ptr<rclcpp_action::ClientGoalHandle<base_interfaces_demo::action::Nav>>)> rclcpp_action::Client<base_interfaces_demo::action::Nav>::SendGoalOptions::goal_response_callback
            options.goal_response_callback = std::bind(&Exer05ActionClient::goal_response_callback,this,_1);
            // std::function<void (std::shared_ptr<rclcpp_action::ClientGoalHandle<base_interfaces_demo::action::Nav>>, std::shared_ptr<const base_interfaces_demo::action::Nav_Feedback>)> rclcpp_action::Client<base_interfaces_demo::action::Nav>::SendGoalOptions::feedback_callback
            options.feedback_callback= std::bind(&Exer05ActionClient::feedback_callback,this,_1,_2); 

            //std::function<void (const rclcpp_action::ClientGoalHandle<base_interfaces_demo::action::Nav>::WrappedResult &result)> rclcpp_action::Client<base_interfaces_demo::action::Nav>::SendGoalOptions::result_callback
            options.result_callback=std::bind(&Exer05ActionClient::result_callback,this,_1);
            client_->async_send_goal(goal,options);
        }
        // 处理目标值相关响应结果
        void goal_response_callback(std::shared_ptr<rclcpp_action::ClientGoalHandle<Nav>> goal_handle){
            if(!goal_handle){
                RCLCPP_INFO(this->get_logger(),"请求目标值非法");
            }else {
                RCLCPP_INFO(this->get_logger(),"目标值被接受");
            }
        }
        // 处理连续反馈
        void feedback_callback(std::shared_ptr<rclcpp_action::ClientGoalHandle<Nav>> goal_handle, std::shared_ptr<const base_interfaces_demo::action::Nav_Feedback> feedback){
            (void)goal_handle;
            RCLCPP_INFO(this->get_logger(),"剩余%.2f米",feedback->distance);
        }

        // 处理最终响应结果
        void result_callback(const rclcpp_action::ClientGoalHandle<Nav>::WrappedResult &result){
            if(result.code==rclcpp_action::ResultCode::SUCCEEDED){
                // 成功响应
                RCLCPP_INFO(this->get_logger(),"乌龟最终的位姿信息，坐标：(%.2f,%.2f),航向:%.2f",result.result->turtle_x,result.result->turtle_y,result.result->turtle_theta);
            }else{
                // 响应失败
                RCLCPP_INFO(this->get_logger(),"响应失败");
            }
        }
    private:
        rclcpp_action::Client<Nav>::SharedPtr client_;
};

int main(int argc,char *argv[])
{
    if(argc!=5){
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"),"请输入合法的目标点数据");
        return 1;
    }
    rclcpp::init(argc,argv);
    auto client=std::make_shared<Exer05ActionClient>();
    client->send_goal(atof(argv[1]),atof(argv[2]),atof(argv[3]));
    rclcpp::spin(client);
    rclcpp::shutdown();
}