from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    return LaunchDescription([
        Node(package="cpp01_topic",executable="demo01_talker_str",name="talker1"),
        Node(package="cpp01_topic",executable="demo01_talker_str",namespace="t1"),
        Node(package="cpp01_topic",executable="demo01_talker_str",namespace="t1",name="talker1")
    ])