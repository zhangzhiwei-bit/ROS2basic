#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "base_interfaces_demo/action/progress.hpp"

using base_interfaces_demo::action::Progress;
using std::placeholders::_1;
using std::placeholders::_2;

class ProgressActionServer:public rclcpp::Node{
  public:
    ProgressActionServer():Node("progress_action_server_node"){
      RCLCPP_INFO(this->get_logger(),"action 服务端创建");
      this->server_ = rclcpp_action::create_server<Progress>(
        this,
        "get_sum",
        std::bind(&ProgressActionServer::handle_goal,this,_1,_2),
        std::bind(&ProgressActionServer::handle_cancel,this,_1),
        std::bind(&ProgressActionServer::handle_accepted,this,_1)
        );
    }
    // 处理提交的目标值（回调函数）
    rclcpp_action::GoalResponse handle_goal(const rclcpp_action::GoalUUID &uuid,std::shared_ptr<const Progress::Goal> goal){
      (void)uuid;
      // 判断提交的数字是否大于1
      if(goal->num<=1){
        RCLCPP_INFO(this->get_logger(),"提交的目标值必须大于1"); 
        return rclcpp_action::GoalResponse::REJECT;
      }

      RCLCPP_INFO(this->get_logger(),"提交的目标值合法"); 
      return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
    }
    // 处理取消请求（回调函数）
    rclcpp_action::CancelResponse handle_cancel(std::shared_ptr<rclcpp_action::ServerGoalHandle<Progress>> goal_handle){
      (void)goal_handle;
      RCLCPP_INFO(this->get_logger(),"接收到任务取消请求");
      return rclcpp_action::CancelResponse::ACCEPT;
    }

    void execute(std::shared_ptr<rclcpp_action::ServerGoalHandle<Progress>> goal_handle){
      // 生成连续反馈的api
      // goal_handle->publish_feedback();
      // 首先获取目标值，然后累加，每循环一次计算进度
      int num= goal_handle->get_goal()->num;
      int sum=0;
      auto feedback = std::make_shared<Progress::Feedback>();
      auto result = std::make_shared<Progress::Result>();
      // 设置休眠
      rclcpp::Rate rate(1.0);
      for(int i=1;i<=num;i++){
        sum+=i;
        double progress =i/(double)num;
        feedback->progress=progress;
        goal_handle->publish_feedback(feedback);
        RCLCPP_INFO(this->get_logger(),"连续反馈中，进度:%.2f",progress);
        // 判断是否接收到取消请求
        if(goal_handle->is_canceling()){
          result->sum=sum;
          // 终止程序
          goal_handle->canceled(result);
          RCLCPP_INFO(this->get_logger(),"任务被取消了！");
          return;
        }
        rate.sleep();
      }
      // 生成最终结果
      // goal_handle->succeed();
      if(rclcpp::ok()){
        result->sum=sum;
        goal_handle->succeed(result);
        RCLCPP_INFO(this->get_logger(),"最终结果:%d",sum);
      }
    }

    void handle_accepted(std::shared_ptr<rclcpp_action::ServerGoalHandle<Progress>> goal_handle){
      // 新建子线程处理耗时的主逻辑实现
      std::thread(std::bind(&ProgressActionServer::execute,this,goal_handle)).detach();

    }

  private:
    rclcpp_action::Server<Progress>::SharedPtr server_;
};

int main(int argc,char const *argv[]){
  rclcpp::init(argc,argv);
  rclcpp::spin(std::make_shared<ProgressActionServer>());
  rclcpp::shutdown();
  return 0;
}