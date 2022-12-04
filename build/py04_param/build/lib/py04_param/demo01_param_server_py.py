import rclpy
from rclpy.node import Node


class ParamServer(Node):
    def __init__(self):
        super().__init__("param_server_node_py",allow_undeclared_parameters=True)
        self.get_logger().info("参数服务端")

    def declare_param(self):
        self.get_logger().info("-----新增参数-----")
        self.declare_parameter("car_name","tiger")
        self.declare_parameter("width",1.55)
        self.declare_parameter("wheels",5)

    def get_param(self):
        self.get_logger().info("-----查询参数-----")
        car_name = self.get_parameter("car_name")
        self.get_logger().info(" %s = %s " %(car_name.name,car_name.value))

        params = self.get_parameters(["car_name","wheels","width"])
        for param in params:
            self.get_logger().info("%s=====%s" %(param.name,param.value))

        # 判断是否包含某个参数
        self.get_logger().info("包含 car_name 吗？%d" %(self.has_parameter("car_name")))
        self.get_logger().info("包含 height 吗？%d" %(self.has_parameter("height")))


    def update_param(self):
        self.get_logger().info("-----修改参数-----")
        self.set_parameters([rclpy.Parameter("car_name",value="Mouse")])
        car_name = self.get_parameter("car_name")
        self.get_logger().info(" 修改后%s = %s " %(car_name.name,car_name.value))

    def del_param(self):
        self.get_logger().info("-----删除参数-----")
        self.get_logger().info("删除前，包含 car_name 吗？%d" %(self.has_parameter("car_name")))
        self.undeclare_parameter("car_name")
        self.get_logger().info("删除后，包含 car_name 吗？%d" %(self.has_parameter("car_name")))
        


def main():
    rclpy.init()
    node = ParamServer()
    node.declare_param()
    node.get_param()
    node.update_param()
    node.del_param()
    rclpy.spin(node)
    rclpy.shutdown()

if __name__=='__main__':
    main()