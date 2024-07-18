#include "rclcpp/rclcpp.hpp"
#include "custom_interfaces/msg/battery_state.hpp"
#include "custom_interfaces/srv/set_led.hpp"

using std::placeholders::_1;
using std::placeholders::_2;

class LEDPanelServerNode : public rclcpp::Node
{
public:
    LEDPanelServerNode() : Node("set_led_server"), led_state(3, 0)
    {

        publisher_ = this->create_publisher<custom_interfaces::msg::BatteryState>("led_state", 10);
        timer_ = this->create_wall_timer(std::chrono::milliseconds(4000), std::bind(&LEDPanelServerNode::publish_led_state, this));
        server_ = this->create_service<custom_interfaces::srv::SetLED>("set_led", std::bind(&LEDPanelServerNode::callback_set_led, this, _1, _2));
        RCLCPP_INFO(this->get_logger(), "LED panel has been started...");
    }

private:
    void publish_led_state()
    {

        auto msg = custom_interfaces::msg::BatteryState();
        msg.led_state = led_state;
        publisher_->publish(msg);
    }

    void callback_set_led(const custom_interfaces::srv::SetLED::Request::SharedPtr request,
                          const custom_interfaces::srv::SetLED::Response::SharedPtr response)
    {

        int64_t led_number = request->led_number;
        int64_t state = request->state;

        if (led_number > (int64_t)(sizeof(led_state) / sizeof(led_state[0])) || led_number <= 0)
        {

            response->message = "ERROR! Led NUMBER is not valid!";
            response->succsess = false;
        }

        else if (state != 0 && state != 1)
        {
            response->message = "ERROR! Led STATE is not valid!";
            response->succsess = false;
        }

        led_state[led_number-1] = state;
        response->message = "DONE";
        response->succsess = true;
    }

    std::vector<int64_t> led_state;
    rclcpp::Publisher<custom_interfaces::msg::BatteryState>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Service<custom_interfaces::srv::SetLED>::SharedPtr server_;
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<LEDPanelServerNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}