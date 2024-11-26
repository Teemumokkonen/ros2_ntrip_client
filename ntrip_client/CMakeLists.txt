cmake_minimum_required(VERSION 3.8)
project(ntrip_client)

# Enable warnings for GCC/Clang
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)

# Add the client library
add_library(client src/client.cpp)
target_include_directories(client PRIVATE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ntrip_client>
  $<INSTALL_INTERFACE:include>
)
ament_target_dependencies(client rclcpp)

# Add the executable
add_executable(ntrip_node src/main.cpp)
target_include_directories(ntrip_node PRIVATE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ntrip_client>
  $<INSTALL_INTERFACE:include>
)
target_link_libraries(ntrip_node client)

# Install targets
install(TARGETS
  client
  ntrip_node
  RUNTIME DESTINATION lib/${PROJECT_NAME}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
)

# Install headers
install(DIRECTORY include/ DESTINATION include)

# Enable testing
if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
