from launch import LaunchDescription
from launch_ros.actions import Node



def generate_launch_description():
    # 服务端启动节点1turtlesim_node
    t=Node(
        package="turtlesim",
        executable="turtlesim_node"
    )
    # 自定义的节点服务端
    server=Node(
        package="cpp07_exercise",
        executable="exer02_server"
    )
    return LaunchDescription([t,server])

