#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "base_interfaces_demo/action/progress.hpp"

using base_interfaces_demo::action::Progress;
using namespace std::chrono_literals;
using std::placeholders::_1;
using std::placeholders::_2;

class ProgressActionClient:public rclcpp::Node{
    public:
        ProgressActionClient():Node("progress_action_client_node_cpp"){
            RCLCPP_INFO(this->get_logger(),"action 客户端创建");
            this->client_ =rclcpp_action::create_client<Progress>(this,"get_sum");
        }
        // 发送请求
        void send_goal(int num){
            // 连接服务器
            if(!client_->wait_for_action_server(10s)){
                RCLCPP_ERROR(this->get_logger(),"服务连接失败");
                return;
            }
            auto goal = Progress::Goal();
            goal.num=num;
            rclcpp_action::Client<Progress>::SendGoalOptions options;
            options.goal_response_callback = std::bind(&ProgressActionClient::goal_response_callback,this,_1);
            options.feedback_callback = std::bind(&ProgressActionClient::feedback_callback,this,_1,_2);
            options.result_callback = std::bind(&ProgressActionClient::result_callback,this,_1);
            // 发送具体请求
            auto future = client_->async_send_goal(goal,options);
        }
        void goal_response_callback(rclcpp_action::ClientGoalHandle<Progress>::SharedPtr goal_handle){
            if(!goal_handle){
                RCLCPP_INFO(this->get_logger(),"目标请求被服务端拒绝");
            }else{
                RCLCPP_INFO(this->get_logger(),"目标处理中");

            }
        }
        void feedback_callback(rclcpp_action::ClientGoalHandle<Progress>::SharedPtr goal_handle,const std::shared_ptr<const Progress::Feedback> feedback){
            (void)goal_handle;
            double progress = feedback->progress;
            int pro= (int)(progress*100);
            RCLCPP_INFO(this->get_logger(),"当前进度：%d%%",pro);

        }
        void result_callback(const rclcpp_action::ClientGoalHandle<Progress>::WrappedResult &result){
            // 通过状态码判断最终状态
            if(result.code==rclcpp_action::ResultCode::SUCCEEDED){
                RCLCPP_INFO(this->get_logger(),"最终结果:%d",result.result->sum);
                return;
            }else if(result.code==rclcpp_action::ResultCode::ABORTED){
                RCLCPP_INFO(this->get_logger(),"被中断！");
                return;
            }else if(result.code==rclcpp_action::ResultCode::CANCELED){
                RCLCPP_INFO(this->get_logger(),"被取消！");
                return;
            }else{
                RCLCPP_INFO(this->get_logger(),"未知异常");
                return;
            }
        }
    private:
        rclcpp_action::Client<Progress>::SharedPtr client_;
};

int main(int argc,char const *argv[]){
    if(argc!=2){
        RCLCPP_ERROR(rclcpp::get_logger("rclcpp"),"请提交一个整型数据");
        return 1;
    }
    
    rclcpp::init(argc,argv);
    auto node = std::make_shared<ProgressActionClient>();
    node->send_goal(atoi(argv[1]));
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}