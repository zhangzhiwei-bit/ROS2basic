from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import ExecuteProcess,RegisterEventHandler
from launch.event_handlers import OnProcessExit


def generate_launch_description():
    # 启动两个turtlesim_node,其中一个要设置命名空间
    t1 = Node(package="turtlesim",executable="turtlesim_node")
    t2 = Node(package="turtlesim",executable="turtlesim_node",namespace="t2")
    # 控制第二个乌龟掉头
    rotate = ExecuteProcess(
        cmd=["ros2 action send_goal /t2/turtle1/rotate_absolute turtlesim/action/RotateAbsolute \"{'theta':3.14}\""],
        output="both",
        shell=True
    )

    # 调用自定义节点要在掉头完毕执行
    exer01 = Node(package="cpp07_exercise",executable="exer01_pub_sub")

    # 通过注册事件控制节点的执行顺序
    register_rotate_exit_event =RegisterEventHandler(
        event_handler=OnProcessExit(
            target_action=rotate,
            on_exit=exer01
        )
    ) 

    return LaunchDescription([t1,t2,rotate,register_rotate_exit_event])