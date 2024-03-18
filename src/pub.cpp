#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "std_msgs/msg/int16.hpp"

using namespace std::chrono_literals;

/* This example creates a subclass of Node and uses std::bind() to register a
* member function as a callback from the timer. */

class MinimalPublisher : public rclcpp::Node
{
  public:
    MinimalPublisher()
    : Node("minimal_publisher"), count_1(0), count_2(0)
    {
      publisher_1= this->create_publisher<std_msgs::msg::String>("string_pub", 10);
      timer_1 = this->create_wall_timer(1000ms, std::bind(&MinimalPublisher::timer_callback_1, this));

      publisher_2= this->create_publisher<std_msgs::msg::Int16>("int_pub", 10);
      timer_2 = this->create_wall_timer(500ms, std::bind(&MinimalPublisher::timer_callback_2, this));
    }

  private:
    void timer_callback_1()
    {
      auto message = std_msgs::msg::String();
      message.data = "Hi " + std::to_string(count_1++);
      RCLCPP_INFO(this->get_logger(), "String Publisher: '%s'", message.data.c_str());
      publisher_1->publish(message);
    }
    void timer_callback_2()
    {
      auto message = std_msgs::msg::Int16();
      message.data = count_2++;
      RCLCPP_WARN(this->get_logger(), "Int Publisher: '%d'", message.data);
      publisher_2->publish(message);
    }
    rclcpp::TimerBase::SharedPtr timer_1;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_1;
    rclcpp::TimerBase::SharedPtr timer_2;
    rclcpp::Publisher<std_msgs::msg::Int16>::SharedPtr publisher_2;

    size_t count_1;
    size_t count_2;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MinimalPublisher>());
  rclcpp::shutdown();
  return 0;
}