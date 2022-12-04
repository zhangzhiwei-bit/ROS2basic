#include "rclcpp/rclcpp.hpp"

using namespace std::chrono_literals;

class MyNode:public rclcpp::Node{
  public:
    MyNode():Node("time_node_cpp"){
      // demo_rate();
      // demo_time();
      // demo_duration();
      demo_opt();
    }
  private:

    void demo_opt(){
      rclcpp::Time t1(10,0);
      rclcpp::Time t2(30,0);

      rclcpp::Duration du1(8,0);
      rclcpp::Duration du2(17,0);

      // 比较运算
      RCLCPP_INFO(this->get_logger(),"t1 >= t2 ? %d",t1>=t2);
      RCLCPP_INFO(this->get_logger(),"t1 <= t2 ? %d",t1<=t2);

    }
    // duration实现
    void demo_duration(){
      rclcpp::Duration du1(1s);
      rclcpp::Duration du2(2,500000000);
      RCLCPP_INFO(this->get_logger(),"s = %.2f,ns=%ld",du1.seconds(),du1.nanoseconds());
      RCLCPP_INFO(this->get_logger(),"s = %.2f,ns=%ld",du2.seconds(),du2.nanoseconds());
    }
    // time的实现
    void demo_time(){
      // 创建Time对象
      rclcpp::Time t1(500000000L);//纳秒
      rclcpp::Time t2(2,500000000L);//秒+纳秒
      // rclcpp::Time right_now = this->get_clock()->now();
      rclcpp::Time right_now = this->now();

      RCLCPP_INFO(this->get_logger(),"s = %.2f,ns=%ld",t1.seconds(),t1.nanoseconds());
      RCLCPP_INFO(this->get_logger(),"s = %.2f,ns=%ld",t2.seconds(),t2.nanoseconds());
      RCLCPP_INFO(this->get_logger(),"s = %.2f,ns=%ld",right_now.seconds(),right_now.nanoseconds());

    }
    // rate的实现
    void demo_rate(){
      // 创建rate对象
      rclcpp::Rate rate1(500ms);
      rclcpp::Rate rate2(1.0);//1.0hz
      while(rclcpp::ok()){
        RCLCPP_INFO(this->get_logger(),"---------");
        rate1.sleep();
      }

    }
};

int main(int argc,char const *argv[]){
  rclcpp::init(argc,argv);
  rclcpp::spin(std::make_shared<MyNode>());
  rclcpp::shutdown();

}