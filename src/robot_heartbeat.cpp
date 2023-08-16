#include <chrono>
#include <functional>
#include <memory>
#include <cstdlib>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std::chrono_literals;

/* This example creates a subclass of Node and uses std::bind() to register a
* member function as a callback from the timer. */

class HeartBeatPublisher : public rclcpp::Node
{
  public:
    HeartBeatPublisher()
    : Node("heartbeat_publisher"), count_(0)
    {
      publisher_ = this->create_publisher<std_msgs::msg::String>("heartbeat", 1);
      timer_ = this->create_wall_timer(
      1000ms, std::bind(&HeartBeatPublisher::timer_callback, this));
    }

  private:
    const std::string ros_namespace = get_namespace();
    void timer_callback()
    {
      auto message = std_msgs::msg::String();
      message.data = ros_namespace;
      RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", message.data.c_str());
      publisher_->publish(message);
    }
    const char* get_namespace()
    {
      const char* ros_namespace_ = std::getenv("ROS_NAMESPACE");
      if (ros_namespace_)
      {
        return ros_namespace_;
      }
      else
      {
        return "";
      }
    }
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
    size_t count_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<HeartBeatPublisher>());
  rclcpp::shutdown();
  return 0;
}