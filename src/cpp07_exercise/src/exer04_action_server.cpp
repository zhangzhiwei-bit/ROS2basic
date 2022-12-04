#include "rclcpp/rclcpp.hpp"
#include "turtlesim/msg/pose.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "base_interfaces_demo/action/nav.hpp"

using std::placeholders::_1;
using std::placeholders::_2;
using base_interfaces_demo::action::Nav;


class Exer04ActionServer:public rclcpp::Node{
    public:
        Exer04ActionServer():Node("exer04_action_server_node_cpp"),x(0.0),y(0.0){
            RCLCPP_INFO(this->get_logger(),"动作服务端！");
            // 创建原生乌龟位姿订阅方
            sub_=this->create_subscription<turtlesim::msg::Pose>("/turtle1/pose",10,std::bind(&Exer04ActionServer::pose_cb,this,_1));
            // 创建发布方
            cmd_pub_=this->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel",10);
            // 创建一个动作服务端
            action_server_= rclcpp_action::create_server<Nav>(
                this,
                "nav",
                std::bind(&Exer04ActionServer::handle_goal,this,_1,_2),
                std::bind(&Exer04ActionServer::handle_cancel,this,_1),
                std::bind(&Exer04ActionServer::handle_accepted,this,_1)
            );
        }
    private:
        // 请求目标处理
        //GoalResponse(const GoalUUID &, std::shared_ptr<const typename ActionT::Goal>)
        rclcpp_action::GoalResponse handle_goal(const rclcpp_action::GoalUUID &uuid, std::shared_ptr<const Nav::Goal> goal){
            (void)uuid;
            // 取出目标中的x,y坐标，分别判断是否超出[0,11.08]的范围
            if(goal->goal_x<0 || goal->goal_y>11.08 || goal->goal_y < 0 || goal->goal_y>11.08){
                RCLCPP_INFO(this->get_logger(),"目标点超出正常范围了！");
            }
            RCLCPP_INFO(this->get_logger(),"目标点合法！");
            return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
        }
        // 取消请求处理
        // CancelResponse(std::shared_ptr<ServerGoalHandle<ActionT>>)
        rclcpp_action::CancelResponse handle_cancel(std::shared_ptr<rclcpp_action::ServerGoalHandle<Nav>> goal_handle){
            (void)goal_handle;
            RCLCPP_INFO(this->get_logger(),"取消任务！");
            return rclcpp_action::CancelResponse::ACCEPT;
        }
        // 主逻辑处理
        void execute(std::shared_ptr<rclcpp_action::ServerGoalHandle<Nav>> goal_handle){
            // 子线程处理主要逻辑
            // 生成连续反馈

            RCLCPP_INFO(this->get_logger(),"主逻辑开始执行");
            // 最终结果
            auto result = std::make_shared<Nav::Result>();
            auto feedback = std::make_shared<Nav::Feedback>();
            geometry_msgs::msg::Twist twist;
            rclcpp::Rate rate(1.0);
            while(true){
                // 如果要取消任务，需要特殊处理
                if(goal_handle->is_canceling()){
                    // 设置取消后的最终结果
                    goal_handle->canceled(result);
                    return;
                }
                // 解析目标点坐标和原生乌龟坐标
                float goal_x = goal_handle->get_goal()->goal_x;
                float goal_y = goal_handle->get_goal()->goal_y;
                // 计算剩余距离，并发布
                float distance_x = goal_x-x;
                float distance_y = goal_y-y;
                float distance = std::sqrt(distance_x * distance_y+distance_y * distance_y);
                feedback->distance=distance;
                goal_handle->publish_feedback(feedback);
                // 发布乌龟运动指令
                // 根据剩余距离计算速度指令并发布
                float scale = 0.5;
                float linear_x = scale*distance_x;
                float linear_y = scale*distance_y;
                twist.linear.x=linear_x;
                twist.linear.y=linear_y;
                cmd_pub_->publish(twist);
                
                //循环结束的条件
                if(distance<=0.05){
                    RCLCPP_INFO(this->get_logger(),"已经导航至目标点");
                    break;
                }
                rate.sleep();
            }
            if(rclcpp::ok()){
                result->turtle_x=x;
                result->turtle_y=y;
                // 生成最终响应
                goal_handle->succeed(result);
            }
           
        }
        // void (std::shared_ptr<ServerGoalHandle<ActionT>>)
        void handle_accepted(std::shared_ptr<rclcpp_action::ServerGoalHandle<Nav>> goal_handle){
            (void)goal_handle;
            std::thread(std::bind(&Exer04ActionServer::execute,this,goal_handle)).detach();
        }
        float x,y;
        void pose_cb(const turtlesim::msg::Pose::SharedPtr pose){
            x=pose->x;
            y=pose->y;
        }
        rclcpp::Subscription<turtlesim::msg::Pose>::SharedPtr sub_;
        rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr cmd_pub_;
        rclcpp_action::Server<Nav>::SharedPtr action_server_;
};

int main(int argc,char *argv[])
{
    rclcpp::init(argc,argv);
    rclcpp::spin(std::make_shared<Exer04ActionServer>());
    rclcpp::shutdown();
}