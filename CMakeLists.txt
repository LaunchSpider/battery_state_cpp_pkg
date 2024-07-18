cmake_minimum_required(VERSION 3.8)
project(battery_state_cpp_pkg)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(custom_interfaces REQUIRED)

add_executable(LED_panel src/LED_panel.cpp)
ament_target_dependencies(LED_panel rclcpp custom_interfaces)

add_executable(battery src/battery.cpp)
ament_target_dependencies(battery rclcpp custom_interfaces)

install(TARGETS
  LED_panel
  battery
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()
