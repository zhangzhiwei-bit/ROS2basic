from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import ExecuteProcess


def generate_launch_description():
    # 在目标点生成一只新乌龟
    x=6
    y=9
    theta=0.0
    name="t2"
    # ros2 service call /spawn turtlesim/srv/Spawn "{'x':6.0,'y':6,'theta':0.0,'name':'t2'}"
    spawn = ExecuteProcess(
        cmd=[f"ros2 service call /spawn turtlesim/srv/Spawn \"{{'x':{x},'y':{y},'theta':{theta},'name':'{name}'}}\""],
        output="both",
        shell=True
    )
    # 调用客户端发送目标点坐标
    client=Node(
        package="cpp07_exercise",
        executable="exer03_client",
        arguments=[str(x),str(y),str(theta)]
        # 参数，最后的运行效果是ros2 run cpp07_exercise exer03_client 6 9 0.0 --ros-args
    )
    return LaunchDescription([spawn,client])